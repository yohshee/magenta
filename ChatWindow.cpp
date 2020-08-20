// ChatWindow.cpp : implementation file
//

#include "stdafx.h"
#include "Magenta.h"
#include "Constants.h"
#include "mdlsmain.h"
#include "Person.h"
#include "InputBox.h"
#include "Log.h"
#include "ChatSocket.h"
#include "ChatWindow.h"
#include "NewListBox.h"
#include "PrivateChat.h"
#include "Ignored.h"
#include "Options.h"
#include "ChatConsole.h"
#include "StatusWindow.h"
#include "BlockList.h"
#include "UrlGrabber.h"
#include "Search.h"
#include "Tokenizer.h"
#include "NameSlide.h"
#include "Annotation.h"
#include ".\chatwindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Custom message for the tray icon
#define WM_TRAYNOTIFY		(WM_APP + 100)

// Typedefs for dynamic linking
typedef BOOL (WINAPI *LPSETLAYEREDWINDOWATTRIBUTES)(HWND, COLORREF, BYTE, DWORD);

// Thread data
typedef struct _LOGTHREADDATA {
	HANDLE hEvent;   // Exit event
	CLog* pLog;
	BOOL* pbLogging;
	BOOL* pbCheckHour;
	BOOL* pbHour;
	LogFileType lftType;
} LOGTHREADDATA;

// List of protocol prefixes. We'll just use the most common ones for now.
// The list is always terminated with NULL.

typedef struct _URL_PREFIX {
	TCHAR* pszPrefix;
	TCHAR* pszInvalid;
} URL_PREFIX;

static const URL_PREFIX szaPrefixes[] = {
	{ _T("http://"),     NULL },
	{ _T("https://"),    NULL },
	{ _T("ftp://"),      NULL },
	{ _T("telnet://"),   NULL },
	{ _T("news://"),     NULL },
	{ _T("nntp://"),     NULL },
	{ _T("gopher://"),   NULL },
	{ _T("wais://"),     NULL },
	{ _T("file://"),     NULL },
	{ _T("mailto:"),     NULL },
	{ _T("www."),        _T("www..") },
	{ NULL,          NULL }
};


// Constant strings. The, ah, saving code relies on their positions. o.x
// Read the functions below dealing with saving logs, and do NOT change the order of these filters
// without adjusting the functions, too.
static const TCHAR szFilter[] = _T("Log Files (*.log)|*.log|Text Files (*.txt)|*.txt|Rich Text Format (*.rtf)|*.rtf|All Files (*.*)|*.*||");
static const TCHAR szLogFilter[] = _T("HTML Files (*.htm)|*.htm|XML Files (*.xml)|*.xml|Text Files (*.txt)|*.txt||");
static const TCHAR szLogExt[] = _T("txt");
static const TCHAR szExt[] = _T("log");

// Constant arrays
static UINT auIDStatusBar[] = {
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR
};

// Function prototypes local to this particular file.
static DWORD CALLBACK StreamInProc(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
static DWORD CALLBACK StreamOutProc(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb);
static UINT FlashThreadProc(LPVOID pvData);
static void ExecuteURL(CString szURL, BOOL bNewWindow);
static CString FetchDefaultBrowser();

// Other private members
static LogFileType g_ltType;

// Looks in the Registry to find the default browser. Wee.
// We need this so that we can open URLs in new windows.
// From some testing, it appears that whatever opens URLs is the default browser. Makes sense.s
static
CString FetchDefaultBrowser()
{
	CString szRet = _T("");
	TCHAR szOut[MAX_PATH];
	DWORD cbData = MAX_PATH;
	LONG lRet;
	HKEY hKey;

	// Yay, we get to use registry calls!
	lRet = ::RegOpenKeyEx(HKEY_CLASSES_ROOT, _T("http\\shell\\open\\command"), 0, 
		KEY_READ, &hKey);

	if(lRet != ERROR_SUCCESS) {
		// Um...okay. This key probably doesn't exist. Let's see who opens HTML files, then.
		lRet = ::RegOpenKeyEx(HKEY_CLASSES_ROOT, _T("htmlfile\\shell\\open\\command"), 0, 
			KEY_READ, &hKey);
	}

	if(lRet == ERROR_SUCCESS) {
		// Okay...now we just query for the default value and close the key.
		// It's the "(default)" value, so we pass in NULL for that. 
		if(::RegQueryValueEx(hKey, NULL, NULL, NULL, (LPBYTE)szOut, &cbData) == ERROR_SUCCESS) {
			// Copy it to our string.
			szRet = szOut;
			// Truncate the string at the last quotation mark in the exe. path, and we're done.
			szRet = szRet.Left(szRet.Find(_T("\""), 1) + 1);
		}
		::RegCloseKey(hKey);
	}

	return szRet;
}

// It pretty much does what it suggests. ;p Opens a URL up just as we'd like.
static
void ExecuteURL(CString szURL, BOOL bNewWindow)
{
	CString szBrowser;
	CString szCommand;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	BOOL bRet;
	LPTSTR lpszBuf;

	if(!bNewWindow) {
		// Easy enough.
		::ShellExecute(NULL, _T("open"), szURL, NULL, NULL, SW_SHOWNORMAL);
	}
	else {
		// Oh...yay. We have to shovel out a whole new process for this. ;p
		// Since the opennew verb doesn't apply to everything, of course.
		szBrowser = FetchDefaultBrowser();
		if(szBrowser.GetLength() > 0) {
			::ZeroMemory(&si, sizeof(STARTUPINFO));
			si.cb = sizeof(STARTUPINFO);
			// Form the command line and get a pointer to the raw buffer.
			szCommand = szBrowser + _T(" ") + szURL;
			lpszBuf = szCommand.LockBuffer();
			// Kick off the browser process.
			bRet = ::CreateProcess(NULL, 
									lpszBuf,
									NULL,
									NULL,
									FALSE,
									NORMAL_PRIORITY_CLASS,
									NULL,
									NULL,
									&si,
									&pi);
			if(bRet) {
				// Close those pesky handles.
				::CloseHandle(pi.hProcess);
				::CloseHandle(pi.hThread);
			}
			else {
				AfxMessageBox(_T("Magenta could not open a new browser window."));
			}
			szCommand.UnlockBuffer();
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CRichEditCtrl	m_txtPanel;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	afx_msg void OnLinkPanel(NMHDR* pNMHDR, LRESULT* pResult);

// Implementation
protected:
	virtual BOOL OnInitDialog();
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_EDIT_INFO, m_txtPanel);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
	ON_NOTIFY(EN_LINK, IDC_EDIT_INFO, OnLinkPanel)
END_MESSAGE_MAP()


BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();	// CG:  This was added by System Info Component.

	int iMajor = 1;
	int iMinor = 0;
	int iBuild = 0;
	int iLen;
	CString szVersion;
	CString szText;
	DWORD dwMask;
	FINDTEXTEX fte;
	CHARRANGE chrgFound;
	CHARFORMAT2 cf;

	CMagenta* pApp = (CMagenta*)AfxGetApp();
	
	dwMask = m_txtPanel.GetEventMask();
	dwMask |= ENM_LINK; // Enable getting link-click events.
	m_txtPanel.SetEventMask(dwMask);


	// Initialize character formatting.
	cf.cbSize = sizeof(CHARFORMAT2);

	m_txtPanel.SendMessage(EM_GETCHARFORMAT, SCF_DEFAULT, (LPARAM)&cf);

	cf.dwEffects &= ~CFE_AUTOCOLOR;
	cf.dwEffects &= ~CFE_AUTOBACKCOLOR;
	cf.dwEffects |= CFE_LINK;
	cf.dwMask |= CFM_LINK;
	cf.crTextColor = RGB(0,0,0);
	cf.crBackColor = GetSysColor(COLOR_BTNFACE);

	// Update the version information dynamically. 
	pApp->GetMagentaVersion(iMajor, iMinor, iBuild);
	szVersion.Format(_T("Magenta version %d.%d.%d"), iMajor, iMinor, iBuild);
	SetDlgItemText(IDC_STATIC_VERSION, szVersion);

	m_txtPanel.SetBackgroundColor(FALSE, GetSysColor(COLOR_BTNFACE));

	// Format and set the text for the screen
	szText = _T("A Cyan Chat client crafted by Rick Coogle (aka Yohshee).  ")
		     _T("Feel free to share and enjoy. Much thanks goes to Mark DeForest ")
			 _T("for bringing this service to the fans.\r\n\r\n");

	szText += _T("Website (check for updates...updates are good!): ")
              _T("http://www.fadedtwilight.org/Magenta\r\n\r\nComments? ")
			  _T("Bugs? Insults? Stick it to me. Mail me at yohshee@hotmail.com.");

	m_txtPanel.SetWindowText(szText);
	m_txtPanel.SetSel(0,0);

	iLen = szText.GetLength();
	fte.chrg.cpMin = 0;
	fte.chrg.cpMax = iLen;
	fte.lpstrText = _T("http://");
	if(m_txtPanel.FindText(FR_DOWN, &fte) != -1) {
		::CopyMemory(&chrgFound, &fte.chrgText, sizeof(CHARRANGE));
		fte.chrg.cpMin = chrgFound.cpMin;
		fte.chrg.cpMax = iLen;
		fte.lpstrText = _T("\r");
		m_txtPanel.FindText(FR_DOWN, &fte);
		m_txtPanel.SetSel(chrgFound.cpMin, fte.chrgText.cpMin);
		m_txtPanel.SendMessage(EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
	}

	return TRUE;	// CG:  This was added by System Info Component.

}


void CAboutDlg::OnLinkPanel(NMHDR* pNMHDR, LRESULT* pResult)
{
	ENLINK* pLink = (ENLINK*)pNMHDR;
	CString szURL;
	CHARRANGE cr;

	// They clicked.
	switch(pLink->msg) {
	case WM_LBUTTONUP:
		// Save the current selection
		m_txtPanel.GetSel(cr);
		// Get the selection of the URL text, fetch it, and execute it.
		m_txtPanel.SetSel(pLink->chrg);
		szURL = m_txtPanel.GetSelText();
		// Restore selection BEFORE execute.
		m_txtPanel.SetSel(cr);
		ExecuteURL(szURL, TRUE);
		break;
	default:
		// Ignore.
		break;
	}

	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
// CChatWindow dialog


void CChatWindow::DoDataExchange(CDataExchange* pDX)
{
	CModelessMain::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChatWindow)
	DDX_Control(pDX, IDC_MESSAGE, m_txtMessage);
	DDX_Control(pDX, IDC_LIST_REGULARS, m_lstRegulars);
	DDX_Control(pDX, IDC_LIST_CYAN, m_lstCyan);
	DDX_Control(pDX, IDC_STATIC_WHO, m_lblWho);
	DDX_Control(pDX, IDC_STATIC_NAME, m_lblName);
	DDX_Control(pDX, IDC_STATIC_FRAME, m_fraButtons);
	DDX_Control(pDX, IDC_STATIC_CYAN, m_lblCyan);
	DDX_Control(pDX, IDC_STATIC_ERROR, m_lblError);
	DDX_Control(pDX, IDC_SEND_PRIVATE, m_cmdSendPrivate);
	DDX_Control(pDX, IDC_SEND, m_cmdSend);
	DDX_Control(pDX, IDC_SCRATCH, m_rtfScratch);
	DDX_Control(pDX, IDC_PRIVATE_CHAT, m_cmdPrivate);
	DDX_Control(pDX, IDC_PANEL, m_txtPanel);
	DDX_Control(pDX, IDC_IGNORE, m_cmdIgnore);
	DDX_Control(pDX, IDC_EDIT_NAME, m_txtName);
	DDX_Control(pDX, IDC_CHAT, m_cmdChat);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CChatWindow, CModelessMain)
	//{{AFX_MSG_MAP(CChatWindow)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_HELP_ABOUT, OnHelpAbout)
	ON_COMMAND(ID_CHAT_EXIT, OnChatExit)
	ON_EN_CHANGE(IDC_EDIT_NAME, OnChangeEditName)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHAT, OnChat)
	ON_LBN_DBLCLK(IDC_LIST_REGULARS, OnDblclkListRegulars)
	ON_WM_CLOSE()
	ON_COMMAND(ID_CHAT_CONNECT, OnChatConnect)
	ON_COMMAND(ID_CHAT_DISCONNECT, OnChatDisconnect)
	ON_LBN_SELCHANGE(IDC_LIST_CYAN, OnSelchangeListCyan)
	ON_LBN_SELCHANGE(IDC_LIST_REGULARS, OnSelchangeListRegulars)
	ON_COMMAND(ID_CHAT_CLEAR_CHAT_WINDOW, OnChatClearChatWindow)
	ON_COMMAND(ID_TOOLS_SEND_RAW_COMMAND, OnToolsSendRawCommand)
	ON_BN_CLICKED(IDC_PRIVATE_CHAT, OnPrivateChat)
	ON_LBN_DBLCLK(IDC_LIST_CYAN, OnDblclkListCyan)
	ON_COMMAND(ID_TOOLS_CONSOLE, OnToolsConsole)
	ON_COMMAND(ID_TOOLS_FONT, OnToolsFont)
	ON_COMMAND(ID_CHAT_SAVE_LOG, OnChatSaveLog)
	ON_COMMAND(ID_CHAT_SHOW_IGNORED, OnChatShowIgnored)
	ON_COMMAND(ID_TOOLS_OPTIONS, OnToolsOptions)
	ON_BN_CLICKED(IDC_IGNORE, OnIgnore)
	ON_BN_CLICKED(IDC_SEND, OnSend)
	ON_BN_CLICKED(IDC_SEND_PRIVATE, OnSendPrivate)
	ON_COMMAND(ID_TOOLS_STATUS_WINDOW, OnToolsStatusWindow)
	ON_UPDATE_COMMAND_UI(ID_CHAT_CONNECT, OnUpdateChatConnect)
	ON_UPDATE_COMMAND_UI(ID_CHAT_DISCONNECT, OnUpdateChatDisconnect)
	ON_EN_MAXTEXT(IDC_PANEL, OnMaxtextPanel)
	ON_EN_ERRSPACE(IDC_PANEL, OnErrspacePanel)
	ON_WM_SIZE()
	ON_COMMAND(ID_BLOCK_LIST, OnBlockList)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_COMMAND(ID_OPACITY_10, OnOpacity10)
	ON_COMMAND(ID_OPACITY_100, OnOpacity100)
	ON_COMMAND(ID_OPACITY_20, OnOpacity20)
	ON_COMMAND(ID_OPACITY_30, OnOpacity30)
	ON_COMMAND(ID_OPACITY_40, OnOpacity40)
	ON_COMMAND(ID_OPACITY_50, OnOpacity50)
	ON_COMMAND(ID_OPACITY_60, OnOpacity60)
	ON_COMMAND(ID_OPACITY_70, OnOpacity70)
	ON_COMMAND(ID_OPACITY_80, OnOpacity80)
	ON_COMMAND(ID_OPACITY_90, OnOpacity90)
	ON_COMMAND(ID_SAVE_FULL_LOG, OnSaveFullLog)
	ON_COMMAND(ID_LOGGING, OnLogging)
	ON_UPDATE_COMMAND_UI(ID_LOGGING, OnUpdateLogging)
	ON_COMMAND(ID_CLEAR_LOG, OnClearLog)
	ON_COMMAND(ID_HELP_WEBSITE, OnHelpWebsite)
	ON_COMMAND(ID_HELP_TOPICS, OnHelpTopics)
	ON_COMMAND(ID_URL_GRABBER, OnUrlGrabber)
	ON_COMMAND(ID_TOOLS_SEARCH, OnToolsSearch)
	ON_COMMAND(ID_CHAT_SWITCH_NAME, OnChatSwitchName)
	ON_UPDATE_COMMAND_UI(ID_CHAT_SWITCH_NAME, OnUpdateChatSwitchName)
	ON_COMMAND(ID_TOOLS_ON_TOP, OnToolsOnTop)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_ON_TOP, OnUpdateToolsOnTop)
	ON_UPDATE_COMMAND_UI(ID_TOOLS_SEND_RAW_COMMAND, OnUpdateToolsSendRawCommand)
	ON_COMMAND(ID_TOOLS_ANNOTATE, OnToolsAnnotate)
	//}}AFX_MSG_MAP
	ON_NOTIFY(EN_LINK, IDC_PANEL, OnLinkPanel)
	ON_MESSAGE(WM_TRAYNOTIFY, OnTrayNotify)
END_MESSAGE_MAP()

BOOL CChatWindow::Create()
{
	CWinApp* pApp = AfxGetApp();
	int iCount, i;
	CString szEntry, szKey;

	// Initialize the brush, as well as some
	// important variables, then create the actual dialog using the
	// modeless main dialog classes.
	m_pEditBkBrush = new CBrush(RGB(0,0,0));

	m_archCyanRecv = NULL;
	m_archCyanSend = NULL;
	m_sfilCyan = NULL;

	m_bLogging = FALSE;
	
	m_bActivated = FALSE;
	m_bConnected = FALSE;
	m_bCyan = FALSE;
	m_bOnTop = FALSE;
	m_bIsNamed = FALSE;
	m_bConsole = FALSE;
	m_bStatus = FALSE;
	
	m_uHours = 0;
	m_uMins = 0;
	m_uSecs = 0;
	m_uTimerID = 0;

	m_hAccel = NULL;
	m_pStatus = NULL;
	m_pConsole = NULL;

	m_bCheckHour = FALSE;
	m_bHour = FALSE;
	m_szLogAnno = _T("");

	// Other variables will be initialized from registry calls.
	m_bShowIgnore = pApp->GetProfileInt(_T("Settings"), _T("ShowIgnore"), TRUE);
	m_bMutIgnore =	pApp->GetProfileInt(_T("Settings"), _T("MutualIgnore"), TRUE);
	m_bSendIgnore =	pApp->GetProfileInt(_T("Settings"), _T("SendIgnore"), TRUE);
	m_bNotify = pApp->GetProfileInt(_T("Settings"), _T("Notify"), FALSE);
	m_bShowConsole = pApp->GetProfileInt(_T("Settings"), _T("ShowConsole"), FALSE);
	m_bBlockServer = pApp->GetProfileInt(_T("Settings"), _T("BlockChatServer"), FALSE);
	m_bBrowseNew = pApp->GetProfileInt(_T("Settings"), _T("BrowseNew"), TRUE);
	m_bHideIcon = pApp->GetProfileInt(_T("Settings"), _T("HideTrayIcon"), FALSE);
	m_crNameColor = (COLORREF)pApp->GetProfileInt(_T("Settings"), _T("NameColor"), RGB(255,255,255)); // White
	m_szHost = pApp->GetProfileString(_T("Settings"), _T("Host"), REMOTE_HOST);
	m_uPort = (UINT)_ttoi(pApp->GetProfileString(_T("Settings"), _T("Port"), REMOTE_PORT));
	m_szPoseFrame = pApp->GetProfileString(_T("Settings"), _T("PoseFrame"), _T("*"));

	// Fetch out the block list
	iCount = pApp->GetProfileInt(_T("Block List"), _T("Count"), 0);
	if(iCount > 0) {
		for(i = 0; i < iCount; i++) {
			szKey.Format(_T("Entry%d"), i);
			m_szaBlocked.Add(pApp->GetProfileString(_T("Block List"), szKey));
		}
	}

	return CModelessMain::Create(CChatWindow::IDD,
								 auIDStatusBar,
								 sizeof(auIDStatusBar)/sizeof(UINT),
								 IDR_MAINFRAME);
}

// We don't use cancel here..so override it to do nothing.
// (Gee, wish I realized this before I spent an hour trying
//  to disable the escape key!)
void CChatWindow::OnCancel()
{
}

/////////////////////////////////////////////////////////////////////////////
// CChatWindow message handlers

BOOL CChatWindow::OnInitDialog()
{
	CHARFORMAT2 cf;
	CHARFORMAT2* lpCF;
	DWORD dwMask;
	CWinApp* pApp;
	UINT uCount;
	CString szName;
	LPRECT pRect;

	pApp = AfxGetApp();

	CModelessMain::OnInitDialog();

	// First, load the accelerators
	m_hAccel = ::LoadAccelerators(pApp->m_hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR_MAGENTA));

	// Initialize the list controls
	m_lstCyan.SetEditBox(&m_txtMessage);
	m_lstCyan.m_pcolUsers = &m_colUsers;
	m_lstCyan.m_pcsUsers = &m_csUsers;
	m_lstRegulars.SetEditBox(&m_txtMessage);
	m_lstRegulars.m_pcolUsers = &m_colUsers;
	m_lstRegulars.m_pcsUsers = &m_csUsers;

	// Initialize our special edit box
	m_txtMessage.m_plstCyan = &m_lstCyan;
	m_txtMessage.m_plstRegulars = &m_lstRegulars;
	m_txtMessage.m_pcolUsers = &m_colUsers;
	m_txtMessage.m_pcsUsers = &m_csUsers;

	// Attempt to fetch position information out of the registry
	if(pApp->GetProfileBinary(_T("Settings"), _T("Position"), (LPBYTE*)(&pRect), &uCount)) {
		SetWindowPos(NULL, pRect->left, pRect->top, pRect->right - pRect->left, 
			pRect->bottom - pRect->top, SWP_NOZORDER);
		delete [] pRect;
	}
	else {
		// Use the settings as defined in the dialog resource, and center the window.
		CenterWindow();
	}

	// Fix the background and foreground color of the richedit "panel" control.
	cf.cbSize = sizeof(CHARFORMAT2);
	GetPanelCharFormat(cf, SCF_DEFAULT);
	cf.crTextColor = RGB(255,255,255);
	cf.dwMask |= (CFM_COLOR | CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE | CFM_STRIKEOUT | CFM_LINK);
	cf.dwEffects &= ~CFE_AUTOCOLOR;
	SetPanelCharFormat(cf, SCF_DEFAULT);
	m_txtPanel.SetBackgroundColor(FALSE, RGB(0,0,0));

	// Set maximum to UINT_MAX
	m_txtPanel.LimitText(0); 
	
	// Set up the scratchpad panel, as well as reading font info out of the registry.
	// (If it's there)
	m_rtfScratch.SetBackgroundColor(FALSE, RGB(0,0,0));

	// Fetch the binary font data out of the registry.
	::ZeroMemory(&cf, sizeof(CHARFORMAT2));
	cf.cbSize = sizeof(CHARFORMAT2);

	// Originally, fetching the font tried to copy in data that might've been there from
	// a previous version. For stability reasons, I'm not doing that anymore.
	if(pApp->GetProfileBinary(_T("Settings"), _T("Font"), (LPBYTE*)(&lpCF), &uCount)) {
		// Check this, because there may be old font data in the registry.
		if(lpCF->cbSize == sizeof(CHARFORMAT2)) {
			ASSERT(lpCF->cbSize == uCount);
			::CopyMemory(&cf, lpCF, sizeof(CHARFORMAT2));
		}
		else {
			// Okay...we're going to be redundant here. Just fetch out the
			// current default scratch format; they're just going to lose their
			// font settings. ;p
			GetScratchCharFormat(cf, SCF_DEFAULT);
			cf.crTextColor = RGB(255,255,255);
			cf.dwEffects = cf.dwEffects & ~CFE_AUTOCOLOR;
		}

		delete [] lpCF;
	}
	else {
		// These settings that we're changing here are actually saved in
		// the registry, too.
		GetScratchCharFormat(cf, SCF_DEFAULT);
		cf.crTextColor = RGB(255,255,255);
		cf.dwEffects = cf.dwEffects & ~CFE_AUTOCOLOR;
	}

	// Just in case, let's set up the masks NOW, even though they're saved.
	cf.dwMask |= (CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE | CFM_STRIKEOUT | CFM_LINK);
	cf.dwEffects &= ~CFE_AUTOCOLOR;	// Does this keep happening?
	SetScratchCharFormat(cf, SCF_DEFAULT);
	
	// Enable the richedit control to send other interesting things.
	dwMask = m_txtPanel.GetEventMask();
	dwMask |= ENM_LINK; // Enable getting link-click events.
	m_txtPanel.SetEventMask(dwMask);

	// Set up the name editbox
	m_txtName.SetLimitText(NAME_LIMIT); // Names can only be so long.
	m_txtName.SetFocus();

	// Disable all non-connected controls
	if(!m_bConnected) {
		SetDefID(m_cmdChat.GetDlgCtrlID());
		DisableAllControls();
	}

	if(!m_bIsNamed) {
		m_txtMessage.SetReadOnly(TRUE);
	}

	// Disable the disconnect menu.
	CMenu* pMenu = GetMenu();
	pMenu->GetSubMenu(MENU_CHAT)->EnableMenuItem(ID_CHAT_DISCONNECT, 
		MF_GRAYED | MF_DISABLED | MF_BYCOMMAND);

	// Fix up the Opacity menus (by default enable)
	if(!IsNT5())
		pMenu->GetSubMenu(MENU_TOOLS)->EnableMenuItem(SUBMENU_OPACITY, 
			MF_GRAYED | MF_DISABLED | MF_BYPOSITION);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// Display version and localhost info
	PrintHostVersionInfo();

	// Put the icon in the tray
	if(!m_bHideIcon)
		InstallTrayIcon();

	// Start the logging thread
	StartLogThread();

	return FALSE; // We set the focus ourselves, so return false.
}

void CChatWindow::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CModelessMain::OnShowWindow(bShow, nStatus);
	
	CWinApp* pApp = AfxGetApp();
	CString szName;

	if(bShow == TRUE && m_bActivated == FALSE) {
		// If the user wants to see the console, let them. o.o
		if(m_bShowConsole)
			OnToolsConsole();

		// Attempt to connect to the server, if they want us to.
		if(pApp->GetProfileInt(_T("Settings"), _T("Connect"), TRUE) == TRUE) {
			ServerConnect();

			// Set the name if we have it.
			szName = pApp->GetProfileString(_T("Settings"), _T("Username"));
			if(szName.GetLength() > 0) {
				if(szName.GetLength() <= NAME_LIMIT && szName.FindOneOf(_T("|^,.")) == -1)
					m_txtName.SetWindowText(szName);
			}

			if((pApp->GetProfileInt(_T("Settings"), _T("AutoLogin"), FALSE) == TRUE) && m_bConnected) {
				// Hm. They want to go ahead and negotiate a connection, too. Fine, fine..
				if(szName.GetLength() > 0) {
					PrintMessage(_T("Auto-logging in as ") + szName + _T("..."), msgMagenta, _T("[Magenta]"));
					// Just in case somebody wants to change the name in the registry...
					// (I know that NegotiateSession does name validation, but throwing a message box
					// at someone like that is not necessarily the thing to do at startup.)
					if(szName.GetLength() <= NAME_LIMIT && szName.FindOneOf(_T("|^,.")) == -1) {
						NegotiateSession();
					}
					else {
						PrintMessage(_T("Invalid auto-login name stored. Check your Options settings (Tools -> Options)."), 
							msgMagenta, _T("[Magenta]"));
					}
				}
			}
		} 	

		
		m_bActivated = TRUE;
	}
}

void CChatWindow::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CModelessMain::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CChatWindow::OnPaint() 
{

	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CModelessMain::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CChatWindow::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

// We need to handle accelerators ourselves...and this is the place to do it.
BOOL CChatWindow::PreTranslateMessage(MSG* pMsg)
{
	// Override normal handling for keypresses.
	if(pMsg->message >= WM_KEYFIRST && pMsg->message <= WM_KEYLAST) {
		// Trap all non-normal keys except for the enter key; we need that one.
		if(pMsg->wParam != VK_RETURN) 
			return (m_hAccel != NULL && ::TranslateAccelerator(this->GetSafeHwnd(), m_hAccel, pMsg));	
	}

	return CModelessMain::PreTranslateMessage(pMsg);
}

void CChatWindow::OnHelpAbout() 
{
	CAboutDlg dlgAbout;

	// Just show the about dialog...
	dlgAbout.DoModal();
}

void CChatWindow::OnHelpWebsite() 
{
	// Thank goodness this is easy. X>
	ExecuteURL(_T("http://www.fadedtwilight.org/Magenta"), m_bBrowseNew);
}


void CChatWindow::OnHelpTopics() 
{
	HWND hWndHelp;

	// Load HTML Help for Magenta using this ever-so-nice API call.
	hWndHelp = ::HtmlHelp(this->GetSafeHwnd(), 
						 _T("magenta.chm::/start.htm>Magenta"),
						 HH_DISPLAY_TOPIC,
						 NULL);

	if(hWndHelp == NULL) {
		AfxMessageBox(_T("Unable to open help file.\r\n") 
			_T("Make sure that magenta.chm is in the same directory as Magenta itself."));	
	}
}

void CChatWindow::OnChatExit() 
{
	// Just pass along the buck to OnClose.
	CChatWindow::OnClose();
}

void CChatWindow::OnChangeEditName() 
{
	CString szText;

	// We need to see if the stuff inside is invalid -- if so, we
	// need to notify the user and fix it so they can't get in!
	m_txtName.GetWindowText(szText);
	if(szText.FindOneOf(_T("|^,.")) > -1 ) {
		m_lblError.SetWindowText(_T("(Invalid Name)"));
		m_cmdChat.EnableWindow(FALSE);
	}
	else {
		m_lblError.SetWindowText(_T(""));
		if(szText.GetLength() > 0) {
			m_cmdChat.EnableWindow(TRUE);
		}
		else {
			m_cmdChat.EnableWindow(FALSE);
		}
	}
}

HBRUSH CChatWindow::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CModelessMain::OnCtlColor(pDC, pWnd, nCtlColor);

	// Only change the color of the listboxes and textboxes.
	switch(nCtlColor) {
	case CTLCOLOR_EDIT:
	case CTLCOLOR_MSGBOX:
	case CTLCOLOR_LISTBOX:
		pDC->SetBkColor(RGB(0,0,0));
		if(pWnd->GetDlgCtrlID() == IDC_LIST_CYAN)
			pDC->SetTextColor(RGB(0,255,255));
		else
			pDC->SetTextColor(RGB(255,255,255));

		return (HBRUSH)(m_pEditBkBrush->GetSafeHandle());
	default:
		return hbr; // Nothing to do, return default brush.
	}
}

void CChatWindow::OnChat() 
{
	// Probably the easiest event handler of them all...
	NegotiateSession();
}

void CChatWindow::OnDblclkListRegulars() 
{
	CPerson* pPerson;
	CString szName, szMessage;
	int iSel, iCount;

	// So...is there anything selected?
	iSel = m_lstRegulars.GetCurSel();
	iCount = m_lstRegulars.GetCount();
	if(iSel != LB_ERR && iCount > 0) {
		// Fetch out the name, obtain a pointer to the object that holds the info,
		// and print it out.
		//m_lstRegulars.GetText(iSel, szName);
		//pPerson = GetPersonByName(szName);
		m_csUsers.Lock();
		pPerson = (CPerson*)m_colUsers.GetAt(m_lstRegulars.GetItemData(iSel));
		m_csUsers.Unlock();

		if(pPerson != NULL) {
			szMessage.Empty();
			szMessage = pPerson->szName + _T(" is from ") + pPerson->szIP + ".";
			PrintMessage(szMessage, msgMagenta, _T("[Magenta]"));
		}
		else {
			// Uh-oh...
			AfxMessageBox(_T("Magenta was unable to fetch data on that chatter."));
		}
	}
	else {
		AfxMessageBox(_T("You must select a name from the list."));
	}
}

void CChatWindow::OnClose() 
{
	CHARFORMAT2 cf;
	CWinApp* pApp = AfxGetApp();
	RECT rc;

	cf.cbSize = sizeof(CHARFORMAT2);

	m_statusBar.SetWindowText(_T("Exiting Magenta..."));

	// Destroy the brush, then close up shop.
	delete m_pEditBkBrush;

	if(m_archCyanSend) {
		delete m_archCyanSend;
		m_archCyanSend = NULL;
	}

	if(m_archCyanRecv) {
		delete m_archCyanRecv;
		m_archCyanRecv = NULL;
	}

	if(m_sfilCyan) {
		delete m_sfilCyan;
		m_sfilCyan = NULL;
	}

	m_sckCyan.Close();

	DestroyArrays();

	// Stop the status bar clock
	if(m_uTimerID)
		KillTimer(m_uTimerID);

	// Save the current window position. We don't really care if this fails.
	GetWindowRect(&rc);
	pApp->WriteProfileBinary(_T("Settings"), _T("Position"), (LPBYTE)(&rc), sizeof(RECT));

	// Save the font information out to the registry.
	GetScratchCharFormat(cf, SCF_DEFAULT);
	if(!pApp->WriteProfileBinary(_T("Settings"), _T("Font"), (LPBYTE)(&cf), sizeof(CHARFORMAT2)))
		TRACE0("Unable to write font data.\n");
	
	// Reset transparency
	SetTransparency(100);

	// Make sure we don't lose the log!
	if(!m_Log.IsEmpty()) {
		if(AfxMessageBox(_T("Do you want to save your log before exiting?"), MB_YESNO | MB_ICONQUESTION) == IDYES) {
			SaveLog();
		}
	}

	// Close all HTML help windows
	::HtmlHelp(NULL, NULL, HH_CLOSE_ALL, 0);

	// Kill threads; i.e. set the event that triggers off an exit on the log thread
	::SetEvent(m_hEventLog);
	// Wait for it to pass
	::Sleep(30);
	// Wait for the thread to signal that it's finished.
	TRACE0("Waiting on exit.\n");
	::WaitForSingleObject(m_hEventLog, INFINITE);
	// Destroy the event object
	TRACE0("Termination finished.\n");
	::CloseHandle(m_hEventLog);
	
	// Get rid of the tray icon
	InstallTrayIcon(FALSE);

	// Destroy those accelerators
	if(m_hAccel != NULL)
		::DestroyAcceleratorTable(m_hAccel);
	CModelessMain::OnClose();
}

// The connect/disconnect methods will probably be changed...moving the code
// into separate functions.
void CChatWindow::OnChatConnect() 
{
	// Defer to another function.
	ServerConnect();
}

void CChatWindow::OnChatDisconnect() 
{
	// As above.
	ServerDisconnect();
}

// Update the command UIs...arrr.
void CChatWindow::OnUpdateChatConnect(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_bConnected);
}

void CChatWindow::OnUpdateChatDisconnect(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bConnected);
}

void CChatWindow::OnSelchangeListCyan() 
{
	// We messed with the Cyan list last...so change the flag.
	m_bCyan = TRUE;

	// Remove the selection on the regular one.
	m_lstRegulars.SetCurSel(-1);
}

void CChatWindow::OnSelchangeListRegulars() 
{
	// We didn't touch the Cyan list here...
	m_bCyan = FALSE;

	// Remove its selection
	m_lstCyan.SetCurSel(-1);
}

void CChatWindow::OnChatClearChatWindow() 
{
	// This is...quite easy.
	m_txtPanel.SetWindowText(_T(""));
}


// Fortunately, this one does all the work for us. ;p
void CChatWindow::OnToolsSearch() 
{
	CSearch dlg;

	dlg.DoModal();
}

// Fetches out a raw command from the user, and sends it to the
// server, unchanged.
void CChatWindow::OnToolsSendRawCommand() 
{
	CString szCommand;
	CInputBox dlgInput;

	dlgInput.DoModal();
	szCommand = dlgInput.m_txtEdit;

	// If they didn't click cancel or just enter
	// nothing, send it to the server.
	if(szCommand.GetLength() != 0) {
		m_archCyanSend->WriteString(szCommand + "\r\n");
		m_archCyanSend->Flush();
	}
}

void CChatWindow::OnUpdateToolsSendRawCommand(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bConnected);	
}

// Woo! Console!
void CChatWindow::OnToolsConsole() 
{
	CChatConsole* pConsole;
	BOOL bRet;

	if(!m_bConsole) {
		pConsole = new CChatConsole;
		if(pConsole == NULL) {
			AfxMessageBox(_T("Magenta couldn't initialize the chat console window: perhaps you're low on memory?"));
		}
		else {
			// Yay! We got one. CREATE IT!
			bRet = pConsole->Create(IDD_CHAT_CONSOLE, this);
			if(bRet) {
				pConsole->ShowWindow(SW_SHOW);
				// Flip the flag to show that, yes, indeed, we have an active console.
				m_bConsole = TRUE;
				m_pConsole = pConsole; // Hang onto the pointer for other visual things.
			}
			else {
				AfxMessageBox(_T("Magenta could not create the chat console window. Low memory?"));
				delete pConsole;
			}
		}
	}
	else {
		// Um..do nothing. Bring it to the front.
		m_pConsole->SetFocus();
	}
}

// Um..status window. Yay.
void CChatWindow::OnToolsStatusWindow() 
{
	CStatusWindow* pStatus;
	BOOL bRet;

	if(!m_bStatus) {
		pStatus = new CStatusWindow;
		if(pStatus == NULL) {
			AfxMessageBox(_T("Magenta could not initialize the chat status window: perhaps you're low on memory?"));
		}
		else {
			// Yay! We got one. CREATE IT!
			bRet = pStatus->Create(IDD_STATUS_WINDOW, this);
			if(bRet) {
				pStatus->ShowWindow(SW_SHOW);
				// Flip the flag to show that, yes, indeed, we have an active console.
				m_bStatus = TRUE;
				m_pStatus = pStatus;
				RefreshUserLists();
			}
			else {
				AfxMessageBox(_T("Magenta could not create the chat status window. Low memory?"));
				delete pStatus;
			}
		}
	}
	else {
		// Well if we have it, just set the focus to it.
		m_pStatus->SetFocus();
	}
}

// Shows the URL grabber window. Note that if we receive any more URLs 
// while the window is shown, the window is not updated. The structure behind it, however,
// is.
void CChatWindow::OnUrlGrabber() 
{
	INT_PTR i, iSize;
	CUrlGrabber dlg;

	iSize = m_szaURLs.GetSize();

	for(i = 0; i < iSize; i++)
		dlg.AddURL(m_szaURLs.GetAt(i));		

	// On selection of an URL, execute it.
	if(dlg.DoModal() == IDOK)
		ExecuteURL(dlg.m_szURL, m_bBrowseNew);
}

// Woo! Private chat activation, baby!
void CChatWindow::OnPrivateChat() 
{
	CString szReceiver;
	int iSel, iCount;

	// Sanity check
	if(!m_bIsNamed)
		return;

	m_csUsers.Lock();

	// First, we have to see if they actually selected anything...
	if(m_bCyan) {
		iSel = m_lstCyan.GetCurSel();
		iCount = m_lstCyan.GetCount();
		if(iCount > 0) {
			if(iSel != LB_ERR) {
				// Aha! They have selected a Cyanite!
				szReceiver = ((CPerson*)m_colUsers.GetAt(m_lstCyan.GetItemData(iSel)))->szName;
			}
			else {
				AfxMessageBox(_T("You need to select a user from one of the lists first."));
				return;
			}
		}
	}
	else {
		iSel = m_lstRegulars.GetCurSel();
		iCount = m_lstRegulars.GetCount();
		if(iCount > 0) {
			if(iSel != LB_ERR) {
				// Aha! They have selected a...regular chatter! Yeah!
				szReceiver = ((CPerson*)m_colUsers.GetAt(m_lstRegulars.GetItemData(iSel)))->szName;
			}
			else {
				AfxMessageBox(_T("You need to select a user from one of the lists first."));
				return;
			}
		}
	}

	m_csUsers.Unlock();

	// Success! Create it!
	CreatePrivateChat(szReceiver);
}

void CChatWindow::OnDblclkListCyan() 
{
	CPerson* pPerson;
	CString szName, szMessage;
	int iSel, iCount;

	// So...is there anything selected?
	iSel = m_lstCyan.GetCurSel();
	iCount = m_lstCyan.GetCount();
	if(iSel != LB_ERR && iCount > 0) {
		// Fetch out the name, obtain a pointer to the object that holds the info,
		// and print it out.
		m_csUsers.Lock();
		pPerson = (CPerson*)m_colUsers.GetAt(m_lstCyan.GetItemData(iSel));
		m_csUsers.Unlock();

		if(pPerson != NULL) {
			szMessage.Empty();
			if(pPerson->ntType == ntCyan) {
				szMessage = pPerson->szName + _T(" is from Cyan Worlds, Inc.");
			}
			else {
				szMessage = pPerson->szName + _T(" is a guest of Cyan Worlds, Inc. They are from ") + pPerson->szIP;
			}
			PrintMessage(szMessage, msgMagenta, _T("[Magenta]"));
		}
		else {
			// Well, crap, this isn't good.
			AfxMessageBox(_T("Magenta could not fetch this chatter's data."));
		}
	}
	else {
		AfxMessageBox(_T("You must select a name from the list."));
	}
}


// Right...the user needs a way to set up the font for this..thingy.
void CChatWindow::OnToolsFont() 
{
	CFontDialog dlgFont;
	LOGFONT lf;
	CHARFORMAT2 cf;
	CFont newfont;
	CDC* pDC;

	cf.cbSize = sizeof(CHARFORMAT2);

	// First, we need to initialize the dialog. So, we 
	// first need to set up some basic config stuff.

	// Take the character format, and twiddle with the bits to see
	// what's going on.
	GetScratchCharFormat(cf, SCF_DEFAULT);

	// These are straight translations, really...
	::ZeroMemory(&lf, sizeof(LOGFONT));
	::lstrcpyn(lf.lfFaceName, cf.szFaceName, LF_FACESIZE);
	lf.lfItalic = (BYTE)(cf.dwEffects & CFE_ITALIC ? TRUE : FALSE);
	lf.lfStrikeOut = (BYTE)(cf.dwEffects & CFE_STRIKEOUT ? TRUE : FALSE);
	lf.lfUnderline = (BYTE)(cf.dwEffects & CFE_UNDERLINE ? TRUE : FALSE);
	lf.lfWeight = cf.dwEffects & CFE_BOLD ? FW_BOLD : FW_NORMAL;
	lf.lfCharSet = cf.bCharSet;
	lf.lfPitchAndFamily = cf.bPitchAndFamily;
	
	// We have to compute the point size using a modified verson of a given formula.
	if((pDC = GetDC()) != NULL) {
		lf.lfHeight = -MulDiv(cf.yHeight, pDC->GetDeviceCaps(LOGPIXELSY), 1440);
		ReleaseDC(pDC);
	}

	// Initialize all these wee flags so the bloody thing works the way we want.
	dlgFont.m_cf.lpLogFont = &lf;	// Insert the LOGFONT we built up there.
	dlgFont.m_cf.Flags |= CF_BOTH | CF_FORCEFONTEXIST | CF_INITTOLOGFONTSTRUCT;
	dlgFont.m_cf.hwndOwner = this->GetSafeHwnd();
	
	// So far so good. Show it.
	if(dlgFont.DoModal() == IDOK) {
		if(AfxMessageBox(_T("In order to change the display font, the display must ")
							_T("be cleared.  Clear the display?"), 
						MB_ICONQUESTION | MB_YESNO) == IDYES) {
			// Wipe out the text.
			m_txtPanel.SetWindowText(_T(""));
			// Then set the font and clean up
			dlgFont.GetCurrentFont(&lf);
			newfont.CreateFontIndirect(&lf);
			m_rtfScratch.SetFont(&newfont);
			newfont.DeleteObject();
			// Make sure that autocolor attribute didn't get set by accident
			GetScratchCharFormat(cf, SCF_DEFAULT);
			cf.dwEffects &= ~CFE_AUTOCOLOR;
			SetScratchCharFormat(cf, SCF_DEFAULT);
		}	
	}
}

void CChatWindow::OnChatSaveLog() 
{
	CString szText;
	CStdioFile filLog;
	CFileException ex;
	CFileDialog* pdlgFile = NULL;
	EDITSTREAM es;		// Struct for saving RTF files.

	pdlgFile = new CFileDialog(FALSE, 
		NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter, this);

	if(pdlgFile != NULL) {
		// First, we need to customize this here dialog, soooo...
		pdlgFile->m_ofn.lpstrTitle = _T("Save Log File");
		pdlgFile->m_ofn.lpstrDefExt = szExt;
		pdlgFile->m_ofn.nFilterIndex = 1;
		if(pdlgFile->DoModal() == IDOK) {
			// Right then..capture the text in the box and dump it into a file.
			try {
				if(filLog.Open(pdlgFile->GetPathName(), 
					CFile::modeWrite | CFile::modeCreate | CFile::typeText, &ex) == FALSE) {
					// This is poor design, the need to manually throw this exception.
					throw &ex;
				}

				// Er, right then. If they selected anything BUT RTF, we save like anything else.
				// Otherwise, we have to stream it out.

				if(pdlgFile->GetFileExt().CompareNoCase(_T("rtf")) == 0 || pdlgFile->m_ofn.nFilterIndex == 3) {
					// Pack the EDITSTREAM struct, then...
					es.dwCookie = (DWORD_PTR)&filLog;  // Cookie is merely a pointer to the file.
					es.pfnCallback = StreamOutProc;
					// Save it.
					m_txtPanel.StreamOut(SF_RTF, es);
				}
				else {
					// We'll do it differently here later on, with our CLog object.
					m_txtPanel.GetWindowText(szText);
					filLog.WriteString(szText);
				}
				filLog.Close();
			}
			catch(CFileException* e) {
				AfxMessageBox(_T("An error occurred while trying to write the log file: ") + e->m_cause, MB_ICONSTOP);
			}
		}

		delete pdlgFile;
	}
	else {
		AfxMessageBox(_T("Magenta could not create open dialog box!"));
	}
}


void CChatWindow::OnSaveFullLog() 
{
	// To be honest, there's not a whole lot to do here except call one
	// function, so let's call it. Oh, by the way, this comment? Yeah,
	// it's just filler.
	SaveLog();
}

void CChatWindow::OnLogging() 
{
	CMenu* pMenu = GetMenu();

	// Simple enough.
	m_bLogging = !m_bLogging;

	if(m_bLogging) {
		// Write a dummy entry into the log to indicate the log starting. This is necessary to distinguish
		// when the user starts and stops the log.
		// We don't need a timestamp because, well, the log timestamps itself.
		m_Log.WriteEntry(_T("Magenta"), _T("localhost"), msgMagenta, _T("Logging started."), mfBroadcast);
		PrintMessage(_T("Logging started."), msgMagenta, _T("[Magenta]"));
		// Check the menu item.
		pMenu->GetSubMenu(MENU_TOOLS)->CheckMenuItem(ID_LOGGING, MF_BYCOMMAND | MF_CHECKED);
	}
	else {
		// Do the same as above, but...different.
		m_Log.WriteEntry(_T("Magenta"), _T("localhost"), msgMagenta, _T("Logging stopped."), mfBroadcast);
		PrintMessage(_T("Logging stopped."), msgMagenta, _T("[Magenta]"));
		pMenu->GetSubMenu(MENU_TOOLS)->CheckMenuItem(ID_LOGGING, MF_BYCOMMAND | MF_UNCHECKED);
	}
}

void CChatWindow::OnUpdateLogging(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bLogging);
}

void CChatWindow::OnToolsAnnotate() 
{
	CWinApp* pApp = AfxGetApp();
	CAnnotation dlg;
	

	if(dlg.DoModal() == IDOK)
		m_szLogAnno = pApp->GetProfileString(_T("Settings"), _T("LogAnnotation"), _T(""));
}

// This one should be obvious...
void CChatWindow::OnClearLog() 
{
	m_Log.ClearLog();
	PrintMessage(_T("Log cleared."), msgMagenta, _T("[Magenta]"));
}

void CChatWindow::OnChatShowIgnored() 
{
	CIgnored dlgIgnored;
	CPerson* pPerson;
	INT_PTR i, iUpper;

	iUpper = m_colIgnored.GetUpperBound();

	// Iterate over the collection of ignored users (if there are any)
	// and add them to the list.
	for(i = 0; i <= iUpper; i++) {
			pPerson = (CPerson*)(m_colIgnored.GetAt(i));
			dlgIgnored.AddPerson(pPerson);			
	}
	
	// ... Show it!
	dlgIgnored.DoModal();
	RefreshUserLists();
}

void CChatWindow::OnToolsOptions() 
{
	COptions dlgOptions;
	CWinApp* pApp;

	pApp = AfxGetApp();

	// Wee...show the options dialog!
	if(dlgOptions.DoModal() == IDOK) {
		// Now, we just fetch the options out of the registry...
		m_bShowIgnore = pApp->GetProfileInt(_T("Settings"), _T("ShowIgnore"), TRUE);
		m_bMutIgnore =	pApp->GetProfileInt(_T("Settings"), _T("MutualIgnore"), TRUE);
		m_bSendIgnore =	pApp->GetProfileInt(_T("Settings"), _T("SendIgnore"), TRUE);
		m_bNotify = pApp->GetProfileInt(_T("Settings"), _T("Notify"), FALSE);
		m_bShowConsole = pApp->GetProfileInt(_T("Settings"), _T("ShowConsole"), FALSE);
		m_bBlockServer = pApp->GetProfileInt(_T("Settings"), _T("BlockChatServer"), FALSE);
		m_bHideIcon = pApp->GetProfileInt(_T("Settings"), _T("HideTrayIcon"), FALSE);

		m_szHost = pApp->GetProfileString(_T("Settings"), _T("Host"), REMOTE_HOST);
		m_uPort = (UINT)_ttoi(pApp->GetProfileString(_T("Settings"), _T("Port"), REMOTE_PORT));
		
		m_bCheckHour = pApp->GetProfileInt(_T("Settings"), _T("HourlyLog"), FALSE);

		if(m_bHideIcon) {
			InstallTrayIcon(FALSE);
		}
		else {
			InstallTrayIcon(TRUE);
		}

		m_crNameColor = (COLORREF)pApp->GetProfileInt(_T("Settings"), _T("NameColor"), RGB(255,255,255));
		
		m_szPoseFrame = pApp->GetProfileString(_T("Settings"), _T("PoseFrame"), _T("*"));

		g_ltType = (LogFileType)pApp->GetProfileInt(_T("Settings"), _T("AutoLogType"), lftText);
	}
}

void CChatWindow::OnIgnore() 
{
	CString szName, szIP;
	int iSel;

	// Remember; you are not allowed to ignore Cyanites or guests..
	if(m_bCyan) {
		AfxMessageBox(_T("You aren't allowed to ignore Cyan employees or guests."));
	}
	else {
		iSel = m_lstRegulars.GetCurSel();
		if(iSel != LB_ERR) {
			// We ignore IP addresses, not names. Therefore, this operation
			// should be allowed even if we're not chatting.
			m_lstRegulars.GetText(iSel, szName);
			szIP = ResolveNameToIP(szName);
			IgnoreUser(szName, szIP);
		}
		else {
			PrintMessage(_T("No user is selected in the Who List."), msgMagenta, _T("[Magenta]"));
		}
	}
}

void CChatWindow::OnSend() 
{
	CString szMessage;
	
	// Fortunately...this is very simple.
	// Sanity check
	if(!m_bIsNamed)
		return;

	m_txtMessage.GetWindowText(szMessage);
	BroadcastMessage(szMessage);
	m_txtMessage.SetWindowText(_T(""));
}

void CChatWindow::OnSendPrivate() 
{
	CString szMessage;
	CPerson* pPerson;
	int iSel, iCount;

	// Sanity check
	if(!m_bIsNamed)
		return;

	m_txtMessage.GetWindowText(szMessage);
	// Okay, we first have to see if the Cyan list was clicked last, because, well,
	// we don't want to get our user info from the wrong list, now do we?
	if(m_bCyan) {
		iSel = m_lstCyan.GetCurSel();
		iCount = m_lstCyan.GetCount();
		if(iCount > 0) {
			if(iSel != LB_ERR) {
				// Get the name, and send the message.
				//m_lstCyan.GetText(iSel, szName);
				m_csUsers.Lock();
				pPerson = (CPerson*)m_colUsers.GetAt(m_lstCyan.GetItemData(iSel));
				m_csUsers.Unlock();
				SendPrivate(pPerson->szName, szMessage);
				// Log it, too.
				if(m_bLogging) {
					m_Log.WriteEntry(pPerson->szName, pPerson->szIP, msgPrivate, szMessage, mfBroadcast);
				}
				m_txtMessage.SetWindowText(_T(""));
			}
			else {
				PrintMessage(_T("No user is selected in the Who List."), msgMagenta, _T("[Magenta]"));
			}
		}
		else {
			// Not that this should ever happen, but...
			AfxMessageBox(_T("There are no Cyan employees or guests currently connected to the chat."), MB_ICONINFORMATION);
		}
	}
	else {
		iSel = m_lstRegulars.GetCurSel();
		iCount = m_lstRegulars.GetCount();

		if(iCount > 0) {
			if(iSel != LB_ERR) {
				// Get the name, and send the message.
				//m_lstRegulars.GetText(iSel, szName);
				m_csUsers.Lock();
				pPerson = (CPerson*)m_colUsers.GetAt(m_lstRegulars.GetItemData(iSel));
				m_csUsers.Unlock();
				SendPrivate(pPerson->szName, szMessage);
				if(m_bLogging) {
					m_Log.WriteEntry(pPerson->szName, pPerson->szIP, msgPrivate, szMessage, mfBroadcast);
				}
				m_txtMessage.SetWindowText(_T(""));
			}
			else {
				PrintMessage(_T("No user is selected in the Who List."), msgMagenta, _T("[Magenta]"));
			}
		}
		else {
			// This should never EVER happen.
			AfxMessageBox(_T("There is nobody currently connected to the chat."), MB_ICONINFORMATION);
		}
	}
}

void CChatWindow::OnMaxtextPanel() 
{
	// Bah. Well, we'll just have to take action OURSELVES...
	OnChatClearChatWindow();
}

void CChatWindow::OnErrspacePanel() 
{
	// CLEAR IT! >_<
	OnChatClearChatWindow();
}

// Here is where we handle yon clickable links.
void CChatWindow::OnLinkPanel(NMHDR* pNMHDR, LRESULT* pResult)
{
	ENLINK* pLink = (ENLINK*)pNMHDR;
	CString szURL;
	CHARRANGE cr;

	// They clicked.
	switch(pLink->msg) {
	case WM_LBUTTONUP:
		m_csPanel.Lock();

		// Save the current selection
		m_txtPanel.GetSel(cr);
		// Get the selection of the URL text, fetch it, and execute it.
		m_txtPanel.SetSel(pLink->chrg);
		szURL = m_txtPanel.GetSelText();
		// Restore selection BEFORE execute.
		m_txtPanel.SetSel(cr);
		ExecuteURL(szURL, m_bBrowseNew);

		m_csPanel.Unlock();
		break;
	default:
		// Ignore.
		break;
	}

	*pResult = 0;
}


// Invoked when the window is resized
void CChatWindow::OnSize(UINT nType, int cx, int cy) 
{
	CModelessMain::OnSize(nType, cx, cy);

	CRect rcPanel, rcLbl, rcList;
	
	if(::IsWindow(this->GetSafeHwnd()) && ::IsWindow(m_statusBar.GetSafeHwnd())) {
		// First, stick that status bar there on the bottom.
		AttachSBToBottom();
		
		// Next, do all of the adjustments. I can't really explain this code, since most
		// of it you figure out through lots of trial and error.
		m_txtMessage.SetWindowPos(NULL, 0, 0, cx - 145, 20, SWP_NOZORDER | SWP_NOMOVE);
		m_fraButtons.SetWindowPos(NULL, 0, 0, cx - 145, 75, SWP_NOZORDER | SWP_NOMOVE);
		m_txtPanel.SetWindowPos(NULL, 0, 0, cx - 145, cy - 120, SWP_NOZORDER | SWP_NOMOVE);
		m_txtPanel.GetWindowRect(&rcPanel);
		ScreenToClient(&rcPanel);
		m_lblWho.GetWindowRect(&rcLbl);
		ScreenToClient(&rcLbl);
		m_lstRegulars.GetWindowRect(&rcList);
		ScreenToClient(&rcList);
		m_lblWho.SetWindowPos(NULL, rcPanel.right + 10, 0, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		m_lstRegulars.SetWindowPos(NULL, rcPanel.right + 10, rcLbl.bottom, rcList.Width(), cy - 240, SWP_NOZORDER);
		m_lstRegulars.GetWindowRect(&rcList);
		ScreenToClient(&rcList);
		m_lblCyan.SetWindowPos(NULL, rcPanel.right + 10, rcList.bottom + 10, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		m_lblCyan.GetWindowRect(&rcLbl);
		ScreenToClient(&rcLbl);
		m_lstCyan.SetWindowPos(NULL, rcPanel.right + 10, rcLbl.bottom, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		RedrawWindow();
	}
}

void CChatWindow::OnBlockList() 
{
	CBlockList dlgBlock;
	int iCount, i;
	CWinApp* pApp = AfxGetApp();
	CString szEntry, szKey;

	if(dlgBlock.DoModal() == IDOK) {
		// Dump the old block list, then read in the new one from the registry.
		m_szaBlocked.RemoveAll();
		iCount = pApp->GetProfileInt(_T("Block List"), _T("Count"), 0);
		if(iCount > 0) {
			for(i = 0; i < iCount; i++) {
				szKey.Format(_T("Entry%d"), i);
				m_szaBlocked.Add(pApp->GetProfileString(_T("Block List"), szKey));
			}
		}
	}
}

void CChatWindow::OnChatSwitchName() 
{
	CNameSlide dlg;
	CString szName, szCommand;

	// Allows the user to switch names. Nothing too fancy here.	
	if(dlg.DoModal() == IDOK) {
			szName = dlg.m_szName;
			m_txtName.SetWindowText(szName);
			// Hard coded command
			szCommand = "10|" + szName + "\r\n";
			m_archCyanSend->WriteString(szCommand);
			m_archCyanSend->Flush();
			m_szName = szName;	
	}
}

void CChatWindow::OnUpdateChatSwitchName(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bIsNamed);
}

// Message handlers for Opacity menus

void CChatWindow::OnOpacity10() 
{
	SetTransparency(10);	
}

void CChatWindow::OnOpacity100() 
{
	SetTransparency(100);
}

void CChatWindow::OnOpacity20() 
{
	SetTransparency(20);	
}

void CChatWindow::OnOpacity30() 
{
	SetTransparency(30);
}

void CChatWindow::OnOpacity40() 
{
	SetTransparency(40);
}

void CChatWindow::OnOpacity50() 
{
	SetTransparency(50);
}

void CChatWindow::OnOpacity60() 
{
	SetTransparency(60);
}

void CChatWindow::OnOpacity70() 
{
	SetTransparency(70);
}

void CChatWindow::OnOpacity80() 
{
	SetTransparency(80);
}

void CChatWindow::OnOpacity90() 
{
	SetTransparency(90);
}

void CChatWindow::OnTimer(UINT nIDEvent) 
{
	CString szPanel;

	// Update seconds...minutes...hours. Yay. o.o;
	m_uSecs++;
	if(m_uSecs == 60) {
		m_uSecs = 0;
		m_uMins++;
		if(m_uMins == 60) {
			m_uMins = 0;
			m_uHours++;

			// Notify the thread if the hour rolls over.
			// From the way things are structured, I seriously doubt
			// that a race condition could occur here.
			if(m_bCheckHour) {
				m_bHour = TRUE;
			}
		}
	}

	// Format the result nicely, then stuff it into the status bar
	szPanel.Format(_T("Conn: %02d:%02d:%02d"), m_uHours, m_uMins, m_uSecs);
	m_statusBar.SetPaneText(2, szPanel);

	// Additional, spastic checking before I do this. >.>
	if(m_bConnected && m_bIsNamed) {
		// Yes, this additional spastic checking is also necessary.
		if((m_uMins > 0) && (m_uSecs > 0) && (m_uMins % 5 == 0) 
			    && (m_uSecs % 50 == 0)) {
			SendPrivate("", "x", FALSE);
		}
	}

	CModelessMain::OnTimer(nIDEvent);
}


// Invoked when an event occurs on the tray icon
LRESULT CChatWindow::OnTrayNotify(WPARAM wParam, LPARAM lParam)
{
	UINT uID, uMsg;

	uID = (UINT)wParam;
	uMsg = (UINT)lParam;

	switch(uMsg) {
	case WM_MOUSEMOVE:
		break;
	case WM_LBUTTONDBLCLK:	
		SetForegroundWindow();
		break;
	case WM_RBUTTONUP:
		break;
	default:
		break;
	}

	return 0;
}

void CChatWindow::OnToolsOnTop() 
{
	CMenu* pMenu = GetMenu();
	CMenu* pToolMenu = pMenu->GetSubMenu(1);

	if(!m_bOnTop) {
		SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		pMenu->CheckMenuItem(ID_TOOLS_ON_TOP, MF_BYCOMMAND | MF_CHECKED);
		m_bOnTop = TRUE;
	}
	else {
		SetWindowPos(&wndNoTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
		pMenu->CheckMenuItem(ID_TOOLS_ON_TOP, MF_BYCOMMAND | MF_UNCHECKED);
		m_bOnTop = FALSE;
	}
}

void CChatWindow::OnUpdateToolsOnTop(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bOnTop);	
}

///////////////////////////////////// END OF MESSAGE HANDLERS

//////////////////////////////////////////////////////////////
// Now...here's where we place all of our utility functions.
//////////////////////////////////////////////////////////////

// Because this is ANNOYING.
void CChatWindow::AttachSBToBottom()
{
	CRect rcStatus, rcThis;
	int iHeight;

	// Okay, we've got to get this thing to stay where it needs to be.
	// First, we need to get the rects of both, and map the coordinates
	// of the status bar's rect onto the coordinates of the window.
	GetClientRect(&rcThis);
	m_statusBar.GetClientRect(&rcStatus);
	m_statusBar.MapWindowPoints(this, rcStatus);
	// Make the right side of the status bar follow the window
	rcStatus.right = rcThis.right;
	// Make the bottom follow the window, and fix the top and height.
	iHeight = rcStatus.Height();
	rcStatus.bottom = rcThis.bottom;
	rcStatus.top = rcStatus.bottom - iHeight;
	// Resize it.
	m_statusBar.MoveWindow(&rcStatus);
}

// Prints host and version info to the window. Usually used only
// at startup.
void CChatWindow::PrintHostVersionInfo()
{
	CString szOutput;				// Output thingy.
	int iMajor = 1;					// Version number output variables
	int iMinor = 0;
	int iBuild = 0;
	char szBuffer[256];				// Buffer to hold the hostname
	struct hostent* pHost;			// Pointer to info about this here host
	in_addr* pAddr;					// Pointer to an internet address.
	CMagenta* pApp = (CMagenta*)AfxGetApp();

	
	// First...we need to get the version information.
	// Everything hinges on that, by the way.
	if(pApp->GetMagentaVersion(iMajor, iMinor, iBuild)) {
		szOutput.Format(_T("Welcome to Magenta v%d.%d Build %d."), iMajor, iMinor, iBuild);
		PrintMessage(szOutput, msgMagenta, _T("[Magenta]"));
		
		// If we can't get the hostname or its associated entry, that's okay. 
		// It's just fluff information anyway.
		if(!::gethostname(szBuffer, 256)) {
			if((pHost = ::gethostbyname(szBuffer)) != NULL) {
				// Format and print the info.
				szOutput.Format(_T("Local Hostname: %s"), szBuffer);
				PrintMessage(szOutput, msgMagenta, _T("[Magenta]"));

				// Take the first address as the result. 
				// This is interesting. I have to cast it to an in_addr* to get it, as one
				// piece of the byte array is...the hostname.
				pAddr = (in_addr*)pHost->h_addr_list[0];
				szOutput.Format(_T("Local IP Address: %s"), ::inet_ntoa(*pAddr));
				PrintMessage(szOutput, msgMagenta, _T("[Magenta]"));
			}
		}
	}	
}

// Installs or removes the Magenta icon from the system tray
void CChatWindow::InstallTrayIcon(BOOL bInstall)
{
	NOTIFYICONDATA nid;
	HICON hIcon;
	CWinApp* pApp = AfxGetApp();

	hIcon = pApp->LoadIcon(IDD_MAGENTA_DIALOG);

	// First, pack the structure.
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.uID = 1;
	nid.hWnd = this->GetSafeHwnd();

	if(bInstall) {
		// Finish packing for install.
		nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
		nid.hIcon = hIcon;
		nid.uCallbackMessage = WM_TRAYNOTIFY;
		lstrcpy(nid.szTip, _T("Magenta"));
		Shell_NotifyIcon(NIM_ADD, &nid);
	}
	else {
		Shell_NotifyIcon(NIM_DELETE, &nid);	
	}

	if(hIcon)
		::DestroyIcon(hIcon);
}

// Inverts the application icon and sets the icon in the
// system tray to this new icon.
void CChatWindow::InvertTrayIcon(BOOL bInvert)
{
	NOTIFYICONDATA nid;
	HICON hIcon, hNewIcon;
	ICONINFO iif;
	CWinApp* pApp = AfxGetApp();
	CDC MemDC;
	HBITMAP hBmpOld;
	CRect rc;

	hIcon = pApp->LoadIcon(IDD_MAGENTA_DIALOG);

	// Pack the notify icon struct
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.uID = 1;
	nid.hWnd = GetSafeHwnd();
	nid.uFlags = NIF_ICON;

	if(bInvert) {
		// Now with some finagling, invert the icon.
		// You're going to have to figure out how I do it. :>
		::GetIconInfo(hIcon, &iif);
		MemDC.CreateCompatibleDC(NULL);
		hBmpOld = (HBITMAP)MemDC.SelectObject(iif.hbmColor);
		rc.SetRect(0, 0, 64, 64);
		MemDC.InvertRect(&rc);
		MemDC.SelectObject(hBmpOld);
		
		// Create and set the new icon.
		hNewIcon = ::CreateIconIndirect(&iif);
		nid.hIcon = hNewIcon;
		Shell_NotifyIcon(NIM_MODIFY, &nid);

		// Clean up.
		if(hNewIcon)
			::DestroyIcon(hNewIcon);
	}
	else {
		nid.hIcon = hIcon;
		Shell_NotifyIcon(NIM_MODIFY, &nid);
	}

	if(hIcon)
		::DestroyIcon(hIcon);
}

// Does just as their names say...
void CChatWindow::EnableAllControls()
{
	CMenu* pMenu = GetMenu();
	CMenu* pToolMenu = pMenu->GetSubMenu(1); // Get the tools menu

	
	pToolMenu->EnableMenuItem(ID_TOOLS_SEND_RAW_COMMAND,
		MF_ENABLED | MF_BYCOMMAND);

	m_lstCyan.EnableWindow(TRUE);
	m_lstRegulars.EnableWindow(TRUE);
	m_txtPanel.EnableWindow(TRUE);
	m_txtMessage.EnableWindow(TRUE);
	m_txtName.EnableWindow(TRUE);
	m_cmdChat.EnableWindow(TRUE);
	m_cmdIgnore.EnableWindow(TRUE);
	m_cmdPrivate.EnableWindow(TRUE);
	m_cmdSendPrivate.EnableWindow(TRUE);
	m_cmdSend.EnableWindow(TRUE);

	UpdateWindow();
}

void CChatWindow::DisableAllControls()
{
	CMenu* pMenu = GetMenu();
	CMenu* pToolMenu = pMenu->GetSubMenu(1); // Get the tools menu

	pToolMenu->EnableMenuItem(ID_TOOLS_SEND_RAW_COMMAND,
		MF_GRAYED | MF_DISABLED | MF_BYCOMMAND);

	m_lstCyan.EnableWindow(FALSE);
	m_lstRegulars.EnableWindow(FALSE);
	m_txtPanel.EnableWindow(FALSE);
	m_txtMessage.EnableWindow(FALSE);
	m_txtName.EnableWindow(FALSE);
	m_cmdPrivate.EnableWindow(FALSE);
	m_cmdChat.EnableWindow(FALSE);
	m_cmdIgnore.EnableWindow(FALSE);
	m_cmdSendPrivate.EnableWindow(FALSE);
	m_cmdSend.EnableWindow(FALSE);

	UpdateWindow();
}

void CChatWindow::LockInName()
{
	// Mmkay. What we need to do here is set the name
	// of the user, lock the input box for that, and change
	// the focus to the message text box.

	// Change the chat button.
	m_cmdChat.EnableWindow(TRUE);
	m_cmdChat.SetWindowText(_T("E&nd Chat"));

	// Then operate on the other controls.
	m_txtName.SetReadOnly(TRUE);
	m_cmdPrivate.EnableWindow(TRUE);
	m_cmdSend.EnableWindow(TRUE);
	SetDefID(m_cmdSend.GetDlgCtrlID());
	m_cmdSendPrivate.EnableWindow(TRUE);
	m_txtMessage.EnableWindow(TRUE);
	m_txtMessage.SetReadOnly(FALSE);
	m_txtMessage.SetFocus();
}

void CChatWindow::UnlockName()
{
	// And this just does the opposite of the above.
	m_bIsNamed = FALSE;
	m_cmdChat.SetWindowText(_T("S&tart Chat"));
	m_cmdChat.EnableWindow(TRUE);
	SetDefID(m_cmdChat.GetDlgCtrlID());
	m_txtName.SetReadOnly(FALSE);
	m_txtMessage.SetReadOnly(TRUE);
	m_txtMessage.EnableWindow(FALSE);
	m_cmdPrivate.EnableWindow(FALSE);
	m_cmdSend.EnableWindow(FALSE);
	m_cmdSendPrivate.EnableWindow(FALSE);
	m_txtName.SetFocus();
}

// Are we running on XP or 2000? Or maybe something even more futuristic?
BOOL CChatWindow::IsNT5()
{
	OSVERSIONINFO ovi;
	BOOL bRet = FALSE;

	ovi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	::GetVersionEx(&ovi);

	// If these hold, then we are.
	if(ovi.dwMajorVersion >= 5 && ovi.dwPlatformId == VER_PLATFORM_WIN32_NT)
		bRet = TRUE;

	return bRet;
}

// Sets the transparency of the window. Of course, this only works in XP and 2000.
void CChatWindow::SetTransparency(int iPercent)
{
	LPSETLAYEREDWINDOWATTRIBUTES lpfnSetLayeredWindowAttributes; // Yay, long variable names!@#
	HINSTANCE hInst;

	if(IsNT5()) {
		// Yay! We've got transparency.
		if(iPercent == 100) {
			// Reset opacity. We want to keep layering off when opaque,
			// so that the program runs faster.
			::SetWindowLongPtr(GetSafeHwnd(), GWL_EXSTYLE, GetExStyle() & ~WS_EX_LAYERED);
			RedrawWindow(NULL, NULL, RDW_INVALIDATE | RDW_FRAME | RDW_ERASE | RDW_ALLCHILDREN);
		}
		else {
			// Let's avoid setting the style, if we can.
			if(!(GetExStyle() & WS_EX_LAYERED)) {
				::SetWindowLongPtr(GetSafeHwnd(), GWL_EXSTYLE, 
					GetExStyle() | WS_EX_LAYERED);
			}
			// Okay...fun. Dynamic linking time!
#ifdef _AFXDLL
				hInst = AfxLoadLibrary(_T("user32.dll"));
#else
				hInst = ::LoadLibrary(_T("user32.dll"));
#endif
			if(hInst != NULL) {
				if((lpfnSetLayeredWindowAttributes = (LPSETLAYEREDWINDOWATTRIBUTES)::GetProcAddress(hInst, 
					"SetLayeredWindowAttributes")) != NULL) {
					// Set alpha percentage to the passed in value.
					lpfnSetLayeredWindowAttributes(GetSafeHwnd(), 0, (255 * iPercent) / 100, LWA_ALPHA);
				}
#ifdef _AFXDLL
				AfxFreeLibrary(hInst);
#else		
				::FreeLibrary(hInst);
#endif
			}
		}
	}
}

// Interprets anything in the passed in string which is a command.
// The string is modified in-place.
void CChatWindow::ParseCommands(CString& szString)
{
	// Interpret the "/me" if it's there. We have to include the space, too, which is
	// unfortunate. Two cases, too.
	szString.TrimLeft();
	if(szString.Left(6).CompareNoCase(_T("/me's ")) == 0) {
		szString.Delete(0,6);
		szString = m_szPoseFrame + _T("is ") + szString + m_szPoseFrame;
	}
	else if(szString.Left(4).CompareNoCase(_T("/me ")) == 0) {
		szString.Delete(0,4);
		szString = m_szPoseFrame + szString + m_szPoseFrame;
	}

}

// Extra function for abstracting out splitting up
// the user segment of a packet.
void CChatWindow::SplitUserInfo(CString szInfo, CString& szName, CString& szIP, NameType& ntType)
{
	int iPos;

	// Get the user type.
	ntType = (NameType)_ttoi(szInfo.Mid(0, 1));
	// Split this up into a name and an IP address.
	iPos = szInfo.Find(_T(","));
	if(iPos != 0) {
		szName = szInfo.Mid(1, iPos - 1);
		szIP = szInfo.Mid(iPos + 1);
	}
	else {
		szName = szInfo.Mid(1);
		szIP = "none";
	}
}

// Thread procedure for flashing the window; usually used only in ProcessPacket().
static
UINT FlashThreadProc(LPVOID pvData)
{
	int i;
	CChatWindow* pWnd;

	pWnd = (CChatWindow*)pvData;

	for(i = 0; i < 3; i++) {
		pWnd->InvertTrayIcon(TRUE);
		pWnd->FlashWindow(TRUE);
		Sleep(1000);
		pWnd->InvertTrayIcon(FALSE);
		pWnd->FlashWindow(FALSE);
	}

	return 0;
}

// Simple function that kicks off a thread that flashes the window.
// Made a single function in case of expansion later.
void CChatWindow::StartFlashThread()
{
	AfxBeginThread(FlashThreadProc, (LPVOID)this);
}

//////////////////////////
// CSocket Event Callbacks
//////////////////////////

// This one is the big daddy procedure: it takes a packet read from the
// socket and parses it out.
void CChatWindow::ProcessPacket()
{
	CString szData;			// The packet from the socket.
	int iType;				// Type of packet
	INT_PTR i, iIndex;
	BOOL bSent;				// Flag for indicating whether a message was dispatched to a chat window.
	NameType ntUserType;	// Type of sender.
	MessageFormat mfFormat; // Format of message (private, public, etc.)
	CString szName;			// Name of sender.
	CString szIP;			// Mangled IP address of sender.
	CString szBuffer;		// Just a buffer.
	CString szMessage;		// The sent message. (if there's only one)
	CPerson* pPerson;		// Temporary person pointer.
	CTokenizer tokSegment;	// Tokenizer for packet segments.
	CPrivateChat* ppcTemp;	// Temporary pointer
	WINDOWPLACEMENT wpl;	
	static int iBytesReceived = 0;	// Static counter for bytes received.

	tokSegment.SetDelimiter(_T("|"));
	wpl.length = sizeof(WINDOWPLACEMENT);
	GetWindowPlacement(&wpl);

	// Pull some data out of the socket/archive to get started, otherwise yer stuck.
	m_archCyanRecv->ReadString(szData);
	
	while(TRUE) {
		// Add it to our running total of byteage
		iBytesReceived += szData.GetLength();

		// All right. Tokenize the data.
		tokSegment.SetString(szData);
		if(m_bConsole) {
			// We may enable packet ignoring later (which is actually trivial), 
			// but for now, if we have the console open, print the packet to it.
			m_pConsole->PrintConsole(tokSegment.GetString());
		}

		// The first token is always the message type/command type.
		iType = _ttoi(tokSegment.GetToken());
		
		switch(iType) {
		case RESP_NAME_ACK:		// Name OK
			m_bIsNamed = TRUE;
			LockInName();
			break;
		case RESP_NAME_NAK:		// Name not OK.
			m_bIsNamed = FALSE;
			PrintMessage(_T("Your name is not unique or has other errors; please enter a different one."), 
						msgServer, _T("[Magenta]"));
			UnlockName();
			break;
		case RESP_PRIVATE_MSG:		// Received private message
			// Do just as we do with a regular message...
			tokSegment.SetString(szData, 3);
			tokSegment.Advance();

			// Fill in the user information and parse out the message stuff.
			szBuffer = tokSegment.GetToken();
			SplitUserInfo(szBuffer, szName, szIP, ntUserType);
			
			// If the name is zero length..the thing is probably from the chat server.
			// If we're blocking the chat server, then block it - but do NOT block if we're
			// just arriving.
			if(szName.GetLength() == 0) {
				if(m_bBlockServer && m_bIsNamed)
					break;
				else {
					szName = _T("ChatServer");
					szIP = m_szHost;
				}
			}

			if(!IsIgnored(szIP, szName) && !IsBlocked(szName)) {
				szBuffer = tokSegment.GetToken();
				mfFormat = (MessageFormat)_ttoi(szBuffer.Mid(1,1));
				szMessage = szBuffer.Mid(2);
				
				// See regular message processing for philosophy on the presence of this line.
				ParseCommands(szMessage);

				// Try to grab URLs from the message.
				GrabURLs(szMessage);

				// Do we need to dispatch it to a chat window?
				bSent = FALSE;
				for(i = 0; i <= m_colChatWindows.GetUpperBound(); i++) {
					ppcTemp = (CPrivateChat*)m_colChatWindows.GetAt(i);
					if(ppcTemp->m_szReceiver == szName) {
						// Send it this way.
						ppcTemp->PrintMessage(szMessage);
						bSent = TRUE;
						break;
					}
				}

				if(!bSent) {
					// Send it using more traditional means, then.
					PrintPrivateMessage(szMessage, szName, 
						TranslateTypeToMessage(ntUserType), mfFormat);
				}

				if(m_bNotify && wpl.showCmd == SW_SHOWMINIMIZED)
					StartFlashThread();
			
				// Finally, if we're logging, write it into the log.
				if(m_bLogging) {
					m_Log.WriteEntry(szName, szIP, TranslateTypeToMessage(ntUserType), 
						szMessage, mfFormat);
				}
			}
			break;
		case RESP_BROADCAST_MSG:		// Received normal message
			// Re-split it with a limit, and chuck the first token.
			tokSegment.SetString(szData, 3);
			tokSegment.Advance();

			// Again..fill in the user info.
			szBuffer = tokSegment.GetToken();
			SplitUserInfo(szBuffer, szName, szIP, ntUserType);
			
			// If we're blocking the server, and we just got a message from the server...
			// Um, no.
			if(ntUserType == ntChatServer && m_bBlockServer)
				break;

			if(!IsBlocked(szName)) {
				if(!IsIgnored(szIP, szName)) {
					szBuffer = tokSegment.GetToken();
					mfFormat = (MessageFormat)_ttoi(szBuffer.Mid(1,1)); // Message format.
					szMessage = szBuffer.Mid(2);

					// Some other clients do not interpret commands as they are sent (e.g. World's), 
					// so let's go ahead and attempt to parse any commands in this message.
					ParseCommands(szMessage);

					GrabURLs(szMessage);

					PrintMessage(szMessage, TranslateTypeToMessage(ntUserType), 
						_T("[") + szName + _T("]"), mfFormat);
					
					if(m_bNotify && wpl.showCmd == SW_SHOWMINIMIZED)
						StartFlashThread();
					
					if(m_bLogging) {
						m_Log.WriteEntry(szName, szIP, TranslateTypeToMessage(ntUserType), 
							szMessage, mfFormat);
					}
				}
				else {
					GetIgnoredByIP(szIP)->bConnected = FALSE;
				}
			}
			break;
		case RESP_NAME_LIST:		// Received a list of names
			m_csUsers.Lock();

			DestroyArrays_Help(&m_colUsers);	// We have to directly invoke the thing here..

			while(tokSegment.HasMoreTokens()) {
				// Get the user information out.
				szBuffer = tokSegment.GetToken();
				SplitUserInfo(szBuffer, szName, szIP, ntUserType);
				// Create and add the new entry
				pPerson = new CPerson;
				pPerson->szIP = szIP;
				pPerson->szName = szName;
				pPerson->ntType = ntUserType;
				iIndex = m_colUsers.Add(pPerson);
			}

			// We have to release the lock before we can refresh.
			m_csUsers.Unlock();
			RefreshUserLists();
			break;
		case RESP_CHATSERV_MSG:		// Chat server welcome announcement
			if(!m_bBlockServer) {
				// Just run through the stuff...
				szBuffer = tokSegment.GetToken();
				ntUserType = (NameType)_ttoi(szBuffer.Mid(0,1));
				szMessage = szBuffer.Mid(1);
				PrintMessage(szMessage, msgServer, _T("[ChatServer]"));
				if(m_bLogging)
					m_Log.WriteEntry(_T("ChatServer"), m_szHost, msgServer, szMessage, mfFormat);
			}
			break;
		case RESP_IGNORED:		// You have been ignored.
			// If enabled, activate mutual ignore and user notification
			// of the ignore.
			szBuffer = tokSegment.GetToken();
			SplitUserInfo(szBuffer, szName, szIP, ntUserType);

			if(m_bShowIgnore) {
				PrintMessage(_T("You have been ignored by ") + szName, msgServer, _T("[ChatServer]"));

				if(m_bNotify && wpl.showCmd == SW_SHOWMINIMIZED)
					StartFlashThread();
			}

			if(m_bMutIgnore)
				IgnoreUser(szName, szIP, FALSE, TRUE);

			break;
		default:		// Um...
			PrintMessage(_T("Invalid message type received."), msgMagenta, _T("[Magenta]"));
			break;
		}

		// Pull some data out of the socket/archive at the end, to keep going.
		// If it attempts to read when the buffer's empty, though..oy, that's 
		// one hard blocking thread. >_<
		if(!m_archCyanRecv->IsBufferEmpty())
			m_archCyanRecv->ReadString(szData);
		else
			break;
	}

	// Set the pane text appropriately
	szBuffer.Format(_T("Recv: %d"), iBytesReceived);
	m_statusBar.SetPaneText(1, szBuffer);
}

// Invoked by our socket whenever a close message is hit.
void CChatWindow::ProcessClose(int iCode)
{
	CString szError = _T("Error in connection: ");
	CString szTemp;

	// First, disconnect cleanly, if need be
	if(iCode != 0) {
		ServerDisconnect();
		// Next, drop a message into the status bar based on the code
		switch (iCode) {
		case WSAENETDOWN:
			szError += _T("Network failed. Check your Internet connection.");
			break;
		case WSAECONNRESET:
			szError += _T("Connection reset by peer.");
			break;
		case WSAECONNABORTED:
			szError += _T("Connection timed out.");
			break;
		case WSAENETUNREACH:
			szError += _T("Network unreachable.");
			break;
		case WSAENETRESET:
			szError += _T("Network dropped connection on reset.");
			break;
		default:
			szTemp.Format(_T("Unknown error, code %d."), iCode);
			szError += szTemp;
			break;
		}
		m_statusBar.SetWindowText(szError);
		PrintMessage(szError, msgMagenta, _T("[Magenta]"));
	}
}

///////////////////////////////
// End CSocket Event Callbacks
///////////////////////////////

// Merely translates a type of name into a broadcast message.
MessageType CChatWindow::TranslateTypeToMessage(NameType nFlag)
{
	MessageType nType = msgChat;

	switch(nFlag) {
	case ntRegular:
		nType = msgChat;
		break;
	case ntCyan:
		nType = msgCyan;
		break;
	case ntSpecialGuest:
		nType = msgGuest;
		break;
	case ntChatClient:
		nType = msgMagenta;
		break;
	case ntChatServer:
		nType = msgServer;
		break;
	default:
		// Nothing. Use default.
		break;
	}

	return nType;
}

// Callback functions for writing and reading streams for these two following functions.
// Slightly modified versions of samples from the MFC docs.
// (What's funny is that in the docs, the two functions' comments are backwards.)
static DWORD CALLBACK StreamInProc(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
   CFile* pFile = (CFile*)dwCookie;

   *pcb = pFile->Read(pbBuff, cb);

   return 0;
}

static DWORD CALLBACK StreamOutProc(DWORD_PTR dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
   CFile* pFile = (CFile*)dwCookie;

   pFile->Write(pbBuff, cb);
   *pcb = cb;

   return 0;
}

// Because MFC does not use CHARFORMAT2, and apparently does not give
// me the option to use otherwise...and it does NOT build under the current SDK...wonderful.
BOOL CChatWindow::GetScratchCharFormat(CHARFORMAT2& cf, int iType)
{
	return (BOOL)m_rtfScratch.SendMessage(EM_GETCHARFORMAT, (WPARAM)iType, (LPARAM)&cf);
}

BOOL CChatWindow::SetScratchCharFormat(CHARFORMAT2 cf, int iType)
{
	return (BOOL)m_rtfScratch.SendMessage(EM_SETCHARFORMAT, (WPARAM)iType, (LPARAM)&cf);
}

BOOL CChatWindow::GetPanelCharFormat(CHARFORMAT2& cf, int iType)
{
	return (BOOL)m_txtPanel.SendMessage(EM_GETCHARFORMAT, (WPARAM)iType, (LPARAM)&cf);
}

BOOL CChatWindow::SetPanelCharFormat(CHARFORMAT2 cf, int iType)
{
	return (BOOL)m_txtPanel.SendMessage(EM_SETCHARFORMAT, (WPARAM)iType, (LPARAM)&cf);
}

// Functions for printing results to the panel/screen.
// This includes both "any" message and private messages.
void CChatWindow::PrintMessage(CString szMessage, MessageType mtType, CString szHeader, MessageFormat mfFormat, BOOL bPrivate, CString szSender)
{
	COLORREF iColor = 0;
	CHARFORMAT2 cf;
	CHARRANGE chr;		// Holds the current selection.
	EDITSTREAM es;		// Stuff for transferring data
	CMemFile filRTF;	// between richedit controls.
	
	// This is..weird, where's the carriage return?
	szMessage += "\r\n";
	// Clean up non-breaking spaces, since wine's richedit
	// implementation doesn't do it right.
	szMessage.Replace(_T("\xa0"), _T(" "));

	cf.cbSize = sizeof(CHARFORMAT2);

	m_csPanel.Lock();

	// Clear out the scratchpad.
	m_rtfScratch.SetWindowText(_T(""));
	
	// Capture the current selection (to prevent user frustration, namely mine.)
	m_txtPanel.GetSel(chr);

	// Figure out what color we need to use.
	switch(mtType) {
	case msgChat:
		// We find the string just in case we, uh, got disconnected or something.
		// Sadly, this doesn't protect against bad impersonators. x.o
		if(szHeader.Find(m_szName) != -1)
			iColor = m_crNameColor; // Use the stored color
		else
			iColor = RGB(255,255,255); // White
		break;
	case msgCyan:
		// Cyan
		iColor = RGB(0,255,255);
		break;
	case msgGuest:
		// Gold
		iColor = COLOR_GOLD;
		break;
	case msgMagenta:
		// Magenta
		iColor = RGB(255,0,255);
		break;
	case msgServer:
		// Lime
		iColor = COLOR_LIME;
		break;
	default:
		// Err...nope. Not if they used it right. Set it to white.
		iColor = RGB(255,255,255);
		break;
	}

	// Use the scratchpad rich text box to build the string.
	m_rtfScratch.SetSel(0,0);
	// Get one format, then use it to set the rest, since the only thing that
	// changes is the color.
	GetScratchCharFormat(cf, SCF_SELECTION);
	cf.dwEffects = cf.dwEffects & ~CFE_AUTOCOLOR;

	if(!bPrivate) {
		if(mtType != msgPrivate) {
			cf.crTextColor = iColor;
			SetScratchCharFormat(cf, SCF_SELECTION);
			m_rtfScratch.ReplaceSel(szHeader + _T(" "));
		}
		else {
			// This is a private message sent announcement. Format accordingly.
			// (I really hate this kind of spam code)
			cf.crTextColor = RGB(255,0,0);  // Red.
			SetScratchCharFormat(cf, SCF_SELECTION);
			m_rtfScratch.ReplaceSel(_T("Private message sent to "));
			m_rtfScratch.SetSel(m_rtfScratch.GetTextLength()+1,m_rtfScratch.GetTextLength()+1);
			cf.crTextColor = iColor;
			SetScratchCharFormat(cf, SCF_SELECTION);
			m_rtfScratch.ReplaceSel(szHeader + _T(": "));
		}

		// Now, we insert the message into the scratchpad.
		m_rtfScratch.SetSel(m_rtfScratch.GetTextLength()+1, m_rtfScratch.GetTextLength()+1);
		cf.crTextColor = COLOR_GRAY;
		SetScratchCharFormat(cf, SCF_SELECTION);
		m_rtfScratch.ReplaceSel(szMessage);

		// Now, we need to format it based on what the message format is.
		if(mfFormat == mfEnter) {
			// First, we format the left side of the text, then the
			// right side. This is the same for the leave case.

			// Left side
			m_rtfScratch.SetSel(0,0);
			cf.crTextColor = COLOR_LIME;
			SetScratchCharFormat(cf, SCF_SELECTION);
			m_rtfScratch.ReplaceSel(_T("\\\\\\\\\\ "));

			// Right side
			m_rtfScratch.SetSel(m_rtfScratch.GetTextLength()+1,m_rtfScratch.GetTextLength()+1);
			SetScratchCharFormat(cf, SCF_SELECTION);
			m_rtfScratch.ReplaceSel(_T(" /////"));
		}
		else if(mfFormat == mfLeave) {
			// Right side is same as left side, except that the slash directions
			// are reversed.

			// Left side
			m_rtfScratch.SetSel(0,0);
			cf.crTextColor = COLOR_LIME;
			SetScratchCharFormat(cf, SCF_SELECTION);
			m_rtfScratch.ReplaceSel(_T("///// "));

			// Right side
			m_rtfScratch.SetSel(m_rtfScratch.GetTextLength()+1,m_rtfScratch.GetTextLength()+1);
			SetScratchCharFormat(cf, SCF_SELECTION);
			m_rtfScratch.ReplaceSel(_T(" \\\\\\\\\\"));
		}
	}
	else {
		// Private message styling 
		if(mfFormat == mfBroadcast) {
			// We got a private message that very much resembles a regular
			// broadcast. The chat server likes to do this.
			cf.crTextColor = iColor;
			SetScratchCharFormat(cf, SCF_SELECTION);
			m_rtfScratch.ReplaceSel(_T("[") + szSender + _T("] "));
		}
		else if (mfFormat == mfPrivate) {
			// This is a private message received notification.
			cf.crTextColor = RGB(255,0,255);  // Magenta.
			SetScratchCharFormat(cf, SCF_SELECTION);
			m_rtfScratch.ReplaceSel(_T("Private message from "));
			m_rtfScratch.SetSel(m_rtfScratch.GetTextLength()+1,m_rtfScratch.GetTextLength()+1);
			cf.crTextColor = iColor;
			SetScratchCharFormat(cf, SCF_SELECTION);
			m_rtfScratch.ReplaceSel(_T("[") + szSender + _T("] "));
		}

		// Now, we insert the message into the scratchpad.
		m_rtfScratch.SetSel(m_rtfScratch.GetTextLength()+1, m_rtfScratch.GetTextLength()+1);
		cf.crTextColor = COLOR_GRAY;
		SetScratchCharFormat(cf, SCF_SELECTION);
		m_rtfScratch.ReplaceSel(szMessage);
	}

	// Okay..we're going to do something kind of nifty here, but still ridiculous since
	// Richedit controls don't let us grab the RTF text easily. Stream the RTF out into
	// a memory-file and read it back into the one on the panel to get it.
	es.dwCookie = (DWORD_PTR)&filRTF;  // Cookie is the same between both.
	es.pfnCallback = StreamOutProc;
	m_rtfScratch.SetSel(0,-1);
	m_rtfScratch.StreamOut(SF_RTF, es);

	// Insert it into the main panel.
	filRTF.SeekToBegin();
	es.pfnCallback = StreamInProc;

	// Theoretically, we could reverse the scrollback by pointing the selection at the end.
	m_txtPanel.SetSel(0, 0);
	//m_txtPanel.SetSel(m_txtPanel.GetTextLength()+1, m_txtPanel.GetTextLength()+1);

	m_txtPanel.StreamIn(SF_RTF | SFF_SELECTION, es);  // Selection flag causes it to be at insertion point.
	
	// Finally, format any URLs, as streaming out apparently strips out any link formatting.
	URLDetect(m_rtfScratch.GetTextLength());

	// Restore the selection (if there is one)
	if((chr.cpMax - chr.cpMin) > 0) {
		chr.cpMin += m_rtfScratch.GetTextLength();
		chr.cpMax += m_rtfScratch.GetTextLength();
		m_txtPanel.SetSel(chr);
	}
	else {
		m_txtPanel.SetSel(0, 0);
	}

	m_txtPanel.SendMessage(EM_SCROLLCARET, 0, 0);
	
	m_csPanel.Unlock();
}

// Used to print a private message.
void CChatWindow::PrintPrivateMessage(CString szMessage, CString szSender, MessageType mtType, MessageFormat mfFormat)
{
	PrintMessage(szMessage, mtType, _T(""), mfFormat, TRUE, szSender);
}

// Functions for sending messages out to the server
void CChatWindow::BroadcastMessage(CString szMessage, MessageFormat mfFormat, BOOL bPrivate, CString szName, BOOL bLoud)
{
	CString szCommand;

	// Strip out all possible newline characters..
	szMessage.Replace(_T("\n"), _T(""));
	szMessage.Replace(_T("\r"), _T(""));
	
	// Interpret commands
	ParseCommands(szMessage);

	if(szMessage.GetLength() > 0) {
		// Build up the command and send it...
		if(!bPrivate) {
			szCommand.Format(_T("%d|^%d%s\r\n"), CMD_SEND_BROADCAST, mfFormat, szMessage);
		}
		else {
			szCommand.Format(_T("%d|0%s|^%d%s\r\n"), CMD_SEND_PRIVATE, szName, mfFormat, szMessage);

			if(bLoud)
				PrintMessage(szMessage, msgPrivate, _T("[") + szName + _T("]"));
		}

		m_archCyanSend->WriteString(szCommand);
		m_archCyanSend->Flush();
	}
}

void CChatWindow::SendPrivate(CString szName, CString szMessage, BOOL bLoud, MessageFormat mfFormat)
{
	BroadcastMessage(szMessage, mfFormat, TRUE, szName, bLoud);
}

// Functions for establishing a connection to a CC-protocol based server, as well as
// joining the chat session. These are two different things, of course.
void CChatWindow::NegotiateSession()
{
	CString szCommand;
	CString szName;

	if(!m_bIsNamed) {
		// We are currently not a part of the chat, then. Hm.
		m_txtName.GetWindowText(szName);
		if(szName.IsEmpty()) {
			AfxMessageBox(_T("You must enter a name to join the chat."));
		}
		else if(szName.FindOneOf(_T("|^,.")) > -1) {
			// Invalid characters in the name. Shame, shame.
			AfxMessageBox(_T("Your name must not contain a |, ^, comma, or period."));
		}
		else if(szName.GetLength() > NAME_LIMIT) {
			// This should never happen, but just in case someone is trying to do some
			// tomfoolery..
			CString szText;
			szText.Format(_T("Your name must be less than or equal to %d characters in length."), NAME_LIMIT);
			AfxMessageBox(szText);
		}
		else {
			// Signal to the server that we are joining, and do things appropriately.
			m_cmdChat.EnableWindow(FALSE);
			szCommand.Format(_T("%d|%s\r\n"), CMD_SET_NAME, szName);
			m_archCyanSend->WriteString(szCommand);
			m_archCyanSend->Flush();
			m_szName = szName;
			// Fixing up the dialog controls is handled in the packet handler...
		}
	}
	else {
		// Leave the chat, just as we'd like.
		m_cmdChat.EnableWindow(FALSE);
		szCommand.Format(_T("%d\r\n"), CMD_UNSET_NAME);
		m_archCyanSend->WriteString(szCommand);
		m_archCyanSend->Flush();
		
		// Next, fix up all the controls as if we'd never been there..
		UnlockName();
	}
}

void CChatWindow::ServerConnect()
{
	CMenu* pMenu = GetMenu();
	CString szCommand;

	szCommand.Format(_T("%d|%d\r\n"), CMD_SET_PROTOCOL, CC_PROTOCOL_ID); // Announcement command.

	// Prepare to connect.
	m_statusBar.SetWindowText(_T("Establishing connection to ") + m_szHost + _T("."));

	ASSERT(m_sckCyan.m_hSocket == INVALID_SOCKET);

	if(!m_sckCyan.Create()) {
		// Ah, shoot...
		m_statusBar.SetWindowText(_T("Error in connection: Unable to create socket."));
		return;
	}

	if(m_sckCyan.Connect(m_szHost, m_uPort)) {
		// Mmkay..attach it to a file.
		m_sfilCyan = new CSocketFile(&m_sckCyan);
		// ... And attach that file to the two archive objects.
		m_archCyanSend = new CArchive(m_sfilCyan, CArchive::store);
		m_archCyanRecv = new CArchive(m_sfilCyan, CArchive::load);
		
		if(m_sfilCyan && m_archCyanSend && m_archCyanRecv) {
			CString szTemp;

			// Aaaaw, yeah...
			m_bConnected = TRUE;
			
			EnableAllControls();
			m_cmdSend.EnableWindow(FALSE);
			m_cmdSendPrivate.EnableWindow(FALSE);
			
			m_txtName.GetWindowText(szTemp);
			if(szTemp.GetLength() == 0) {
				m_cmdChat.EnableWindow(FALSE);
			}
			else {
				m_cmdChat.EnableWindow(TRUE);
			}

			m_cmdPrivate.EnableWindow(FALSE);
			SetDefID(m_cmdChat.GetDlgCtrlID());

			// As well as do appropriate things to the menus.
			pMenu->GetSubMenu(MENU_CHAT)->EnableMenuItem(ID_CHAT_CONNECT, 
				MF_GRAYED | MF_DISABLED | MF_BYCOMMAND);
			pMenu->GetSubMenu(MENU_CHAT)->EnableMenuItem(ID_CHAT_DISCONNECT, 
				MF_ENABLED | MF_BYCOMMAND);
			m_statusBar.SetWindowText(_T("Connection established to ") + m_szHost + _T("."));

			// Now, we announce to the server that we're here.
			m_archCyanSend->WriteString(szCommand);
			m_archCyanSend->Flush();

			// Initialize and start the connected timer (times out every second)
			m_uHours = 0;
			m_uMins = 0;
			m_uSecs = 0;
			m_uTimerID = (UINT)SetTimer(1, 1000, NULL);  // If it fails, we don't care; this is just fluff anyway.
		
		}
		else {
			// Shoot. Make the error better later, but clean up our mess, too.
			m_statusBar.SetWindowText(_T("Error in connection: insufficient memory."));
			if(m_sfilCyan) {
				delete m_sfilCyan;
				m_sfilCyan = NULL;
			}

			if(m_archCyanSend) {
				delete m_archCyanSend;
				m_archCyanSend = NULL;
			}

			if(m_archCyanRecv) {
				delete m_archCyanRecv;
				m_archCyanRecv = NULL;
			}
		}
	}
	else {
		// Erm..make error handling better later.
		m_sckCyan.Close();
		m_statusBar.SetWindowText(_T("Error in connection: unable to connect to host."));
	}

}

void CChatWindow::ServerDisconnect()
{
	CMenu* pMenu = GetMenu();

	// Same as with OnChatConnect...with a sanity check.
	if(m_bConnected) {
		// Just flip the flag, and close up shop.
		m_bConnected = FALSE;

		if(m_archCyanSend) {
			delete m_archCyanSend;
			m_archCyanSend = NULL;
		}

		if(m_archCyanRecv) {
			delete m_archCyanRecv;
			m_archCyanRecv = NULL;
		}

		if(m_sfilCyan) {
			delete m_sfilCyan;
			m_sfilCyan = NULL;
		}

		m_sckCyan.Close();

		m_statusBar.SetWindowText(_T("Connection closed."));

		// Fix up the menus.
		pMenu->GetSubMenu(MENU_CHAT)->EnableMenuItem(ID_CHAT_DISCONNECT, 
			MF_GRAYED | MF_DISABLED | MF_BYCOMMAND);
		pMenu->GetSubMenu(MENU_CHAT)->EnableMenuItem(ID_CHAT_CONNECT, 
			MF_ENABLED | MF_BYCOMMAND);

		
		// Unlock the name field.
		UnlockName();
		// There's a lot of redundancy here, but we can tolerate that for
		// convenience.
		DisableAllControls();

		// Kill the timer
		if(m_uTimerID)
			KillTimer(m_uTimerID);
	}
}


// Grabs URLs from the passed in message text.
void CChatWindow::GrabURLs(CString szString)
{
	CString szURL;
	CTokenizer tok;

	// Okay. We have to *find* URLs first. And how do we do this?
	// Unfortunately, we have to make assumptions. One of these being
	// that the URLs we're looking for are properly escaped. I.e. NO SPACES.
	tok.SetString(szString);
	tok.SetDelimiter(_T(" ")); // It defaults to space, but this is here for clarity.
	while(tok.HasMoreTokens()) {
		// Try to parse a URL from the token.
		szURL = tok.GetToken();
		if(VerifyURL(szURL)) {
			// Sweet. Hang onto it.
			m_szaURLs.Add(szURL);
			TRACE1("URL grabbed: %s\n", szURL);
		}
	}
}

// Attempts to verify that the passed in string is indeed a URL. The string is also
// destructively modified to remove trailing characters that are not a part of the
// URL.
BOOL CChatWindow::VerifyURL(CString& szURL)
{
	BOOL bRet = FALSE;
	CString szTemp, szTemp2;
	int i;
	
	// First, check to see if we have a protocol. If we have one, it is a URL, no matter
	// what. If it's incorrect in syntax, that's their fault. ;p
	i = 0;
	while(szaPrefixes[i].pszPrefix != NULL) {
		szTemp = szURL.Left(CString(szaPrefixes[i].pszPrefix).GetLength());

		if(szaPrefixes[i].pszInvalid != NULL) {
			szTemp2 = szURL.Left(CString(szaPrefixes[i].pszInvalid).GetLength());
		}
		else {
			szTemp2 = _T("");
		}

		if(szTemp.CompareNoCase(szaPrefixes[i].pszPrefix) == 0) {
			if(szTemp2.GetLength() > 0) {
				// Need some additional verification on this one.
				if(szTemp2.CompareNoCase(szaPrefixes[i].pszInvalid) != 0) {
					bRet = TRUE;
				}
			}
			else {
				// Got one!
				bRet = TRUE;
			}

			break;
		}
		i++;
	}

	// Finally, assume that any character at the end is fine except most punctuation.
	// Eat 'em up.
	while(szURL.Right(1).FindOneOf(_T(",.;:{}[]()-`<>\\|")) != -1)
		szURL = szURL.Left(szURL.GetLength() - 1);

	return bRet;
}

// Examines the Richedit panel and formats any URLs that it finds.
// Assumes that a lock on the panel has already been acquired.
void CChatWindow::URLDetect(int iLength)
{
	int i;
	FINDTEXTEX fte; 
	CHARRANGE chrgFound;
	CHARFORMAT2 cf;
	CString szSelText, szTemp;

	cf.cbSize = sizeof(CHARFORMAT2);

	m_txtPanel.SetSel(0,0);

	// Initialize character formatting.
	GetPanelCharFormat(cf, SCF_DEFAULT);

	cf.dwEffects &= ~CFE_AUTOCOLOR;
	cf.dwEffects &= ~CFE_AUTOBACKCOLOR;
	cf.dwEffects |= CFE_LINK;
	cf.dwMask |= CFM_LINK;
	cf.crTextColor = RGB(255,255,255);
	cf.crBackColor = RGB(100,100,100);
	i = 0;
	while(szaPrefixes[i].pszPrefix != NULL) {
		::ZeroMemory(&fte, sizeof(FINDTEXTEX));
		fte.lpstrText = szaPrefixes[i].pszPrefix;
		// Look to the end of this line.
		fte.chrg.cpMin = 0;
		fte.chrg.cpMax = iLength;
		m_txtPanel.SetSel(0,iLength);

		while(m_txtPanel.FindText(FR_DOWN, &fte) != -1) {
			::CopyMemory(&chrgFound, &fte.chrgText, sizeof(CHARRANGE));

			// So we have a URL. Extend the selection until we get to the "end."
			// The end is a space or the end of the line, right now.
			fte.lpstrText = _T(" ");
			::CopyMemory(&fte.chrg, &chrgFound, sizeof(CHARRANGE));
			fte.chrg.cpMax = iLength;

			if(m_txtPanel.FindText(FR_DOWN, &fte) != -1) {
				// Okay, we found a space; HOWEVER, the URL might not end here, it may
				// end behind us somewhere.
				m_txtPanel.SetSel(chrgFound.cpMin, fte.chrgText.cpMax);
				while(m_txtPanel.GetSelText().Right(1).FindOneOf(_T(" ,.;:{}[]()-`<>\\|")) != -1) {
					fte.chrgText.cpMax--;
					m_txtPanel.SetSel(chrgFound.cpMin, fte.chrgText.cpMax);
				}

			}
			else {
				// Wee. Apparently, the end of the line is nigh. So, extend the selection to it.
				chrgFound.cpMax = iLength;
				m_txtPanel.SetSel(chrgFound);
			}


			// Format the selection, if we can.
			szSelText = m_txtPanel.GetSelText();
			
			// Check and see if this is a valid URL first...
			if(szaPrefixes[i].pszInvalid != NULL) {
				szTemp = szSelText.Left(CString(szaPrefixes[i].pszInvalid).GetLength());
				if(szTemp.CompareNoCase(szaPrefixes[i].pszInvalid) != 0) {
					SetPanelCharFormat(cf, SCF_SELECTION);	
				}
			}
			else {
				// Set it regardless.
				SetPanelCharFormat(cf, SCF_SELECTION);
			}
			// Advance the search.
			fte.lpstrText = szaPrefixes[i].pszPrefix;
			fte.chrg.cpMin = chrgFound.cpMax + 1;
			fte.chrg.cpMax = iLength;
		}
		i++;
	}
}

// Saves a memory-based log out to disk using the WriteFile method
void CChatWindow::SaveLog()
{
	CFileDialog* pdlgFile = NULL;

	if(m_Log.IsEmpty()) {
		// Ehh?
		AfxMessageBox(_T("The memory log contains no entries."));
	}
	else {
		// Create an instance of the dialog on the heap. I seem to be running into very weird
		// destructor problems if I don't do this now. 
		// ...
		// Never mind. The destructor problems were a result of how the static MFC libraries
		// were compiled. ;p Unfortunately, I can't ever seem to recompile them myself. c.c
		pdlgFile = new CFileDialog(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_EXPLORER,
			szLogFilter, this);
		
		if(pdlgFile != NULL) {
			// First, we need to customize this here dialog, soooo...
			pdlgFile->m_ofn.lpstrTitle = _T("Save Log File");
			pdlgFile->m_ofn.lpstrDefExt = szLogExt;
			pdlgFile->m_ofn.nFilterIndex = 1;
			if(pdlgFile->DoModal() == IDOK) {
				// We'll do this by filter index, in case someone changes the extension it's saved as.
				switch(pdlgFile->m_ofn.nFilterIndex) {
				case 1: // htm
					m_Log.WriteFile(lftHTML, pdlgFile->GetPathName());
					break;
				case 2: // xml
					m_Log.WriteFile(lftXML, pdlgFile->GetPathName());
					break;
				case 3: // txt
					m_Log.WriteFile(lftText, pdlgFile->GetPathName());
					break;
				default:
					// buh?! Um. <.<
					AfxMessageBox(_T("Invalid file type selected!"));
					break;
				}
			}
			delete pdlgFile;
		}
		else {
			AfxMessageBox(_T("Magenta could not create the save dialog box!"));
		}
	}
}


// Kicks off the logging thread; the thread itself periodically wakes itself up to write the 
// memory log out to disk. Some of the code in this routine is no longer necessary.
void CChatWindow::StartLogThread()
{
	LOGTHREADDATA* pData = NULL;
	LogFileType ltType;
	CWinApp* pApp = AfxGetApp();

	// Get the auto-log setting. This is important just in case we, oh, I don't know, CRASH or something.
	ltType = (LogFileType)pApp->GetProfileInt(_T("Settings"), _T("AutoLogType"), lftText);
	g_ltType = ltType;

	// Create the event objects	
	m_hEventLog = ::CreateEvent(NULL, TRUE, FALSE, NULL);

	// Kick off the thread.
	AfxBeginThread(CChatWindow::LogThreadProc, (LPVOID)this);
}

// Thread procedure for logging. Periodically writes the memory log out to disk to a single
// file. This file is an option in the registry, and not fully implemented yet..
UINT CChatWindow::LogThreadProc(LPVOID pvData)
{
	CChatWindow* pWnd = (CChatWindow*)pvData;
	CTime time;
	CString szFilename, szMsg;


	if(pvData != NULL) {
		// Just loop while we're here. This thread doesn't run often.
		while(::WaitForSingleObject(pWnd->m_hEventLog, 5000) == WAIT_TIMEOUT) {
			if(pWnd->m_bLogging == TRUE) {
				switch(g_ltType) {
				case lftText:
					pWnd->m_Log.WriteFile(lftText, _T("magenta0.txt"));
					break;
				case lftHTML:
					pWnd->m_Log.WriteFile(lftHTML, _T("magenta0.html"));
					break;
				case lftXML:
					pWnd->m_Log.WriteFile(lftXML, _T("magenta0.xml"));
					break;
				default:
					// Do nothing
					break;
				}
			}

			// Saving on the hour? We take the logging filetype as above
			// in order to determine the format.
			if(pWnd->m_bCheckHour == TRUE) {
				if(pWnd->m_bHour == TRUE) {
					time = CTime::GetCurrentTime();

					// Build the name, and write the log out to the current directory
					szFilename.Format(_T("log-%04d%02d%02d%02d%02d%02d"),
						time.GetYear(),
						time.GetMonth(),
						time.GetDay(),
						time.GetHour(),
						time.GetMinute(),
						time.GetSecond());

					if(pWnd->m_szLogAnno.GetLength() > 0) {
						szFilename += "-";
						szFilename += pWnd->m_szLogAnno;
						szFilename.TrimRight(); // Just in case there are trailing spaces.
					}

					// The calls are redundant, but I really don't feel like doing all the variable
					// assignment crap, really. ;p
					switch(g_ltType) {
					case lftText:
						szFilename += _T(".txt");
						pWnd->m_Log.WriteFile(lftText, szFilename);
						break;
					case lftHTML:
						szFilename += _T(".htm");
						pWnd->m_Log.WriteFile(lftHTML, szFilename);
						break;
					case lftXML:
						szFilename += _T(".xml");
						pWnd->m_Log.WriteFile(lftXML, szFilename);
						break;
					default:
						// Do nothing
						break;
					}

					szMsg.Format(_T("Hourly log file %s written."), szFilename);
					pWnd->PrintMessage(szMsg, msgMagenta, _T("[Magenta]"));
					pWnd->m_bHour = FALSE;
				}
			}
		}

		// Okay, so we're exiting. Set the event again, since the calling thread is now waiting on it.
		//TRACE0(_T("Exiting Log Thread...\n"));
		::SetEvent(pWnd->m_hEventLog);
	}

	return 0;
}


// Refreshes the user lists. Useful for..er..changes, like people being
// unignored and the like.
void CChatWindow::RefreshUserLists()
{
	INT_PTR i, iListItem;
	CPerson* pPerson;
	CString szUsername;
	HWND hwndCap;

	// Clear out the user lists, then iterate over the array of users and refill
	// them.
	m_lstCyan.ResetContent();
	m_lstRegulars.ResetContent();
	
	m_csUsers.Lock();

	if(m_bStatus)
		m_pStatus->Clear();

	for(i = 0; i < m_colUsers.GetSize(); i++) {
		pPerson = (CPerson*)m_colUsers.GetAt(i);
		if(!IsIgnored(pPerson->szIP, pPerson->szName)) {
			if(IsBlocked(pPerson->szName)) {
				szUsername = pPerson->szName + _T(" [Blocked]");
			}
			else {
				szUsername = pPerson->szName;
			}

			switch(pPerson->ntType) {
			case ntCyan:
			case ntSpecialGuest:
				// The names get added to the Cyan list...
				iListItem = m_lstCyan.AddString(szUsername);
				if(iListItem != LB_ERR && iListItem != LB_ERRSPACE) {
					m_lstCyan.SetItemData((int)iListItem, (DWORD_PTR)i);
				}
				break;
			case ntRegular:
				iListItem = m_lstRegulars.AddString(szUsername);
				if(iListItem != LB_ERR && iListItem != LB_ERRSPACE) {
					m_lstRegulars.SetItemData((int)iListItem, (DWORD_PTR)i);
				}
				break;
			default:
				// Nothing. Break silently.
				break;
			}

			if(m_bStatus) {
				// Send this info to the status window, too.
				switch(pPerson->ntType) {
				case ntCyan:
					m_pStatus->InsertPerson(pPerson, ICON_CYAN);
					break;
				case ntSpecialGuest:
					m_pStatus->InsertPerson(pPerson, ICON_GUEST);
					break;
				case ntRegular:
					m_pStatus->InsertPerson(pPerson, ICON_REGULAR);
					break;
				default:
					// Nothing.
					break;
				}
			}
		}
	}

	m_csUsers.Unlock();

	// Okay, for some reason, if this person had their mouse down on the lists while
	// they were refreshing, Windows forgets to release the mouse capture, which causes
	// Magenta to oddly freeze. I think I stumbled on a bug in Windows. c.c
	hwndCap = ::GetCapture();
	if(hwndCap != NULL)
		if(hwndCap == m_lstCyan.GetSafeHwnd() || hwndCap == m_lstRegulars.GetSafeHwnd())
			::ReleaseCapture();
}

// Functions for working with private chat windows (adding/removing/etc.)
void CChatWindow::CreatePrivateChat(CString szName)
{
	CPrivateChat* pPrivate = NULL;
	INT_PTR i, iUpper;
	BOOL bFound;

	// Of course...does this chat window already exist?
	iUpper = m_colChatWindows.GetUpperBound();
	bFound = FALSE;
	for(i = 0; i <= iUpper; i++) {
		pPrivate = (CPrivateChat*)m_colChatWindows.GetAt(i);
		if(pPrivate->m_szReceiver == szName) {
			// Well, looks like we can't make it.
			bFound = TRUE;
			break;
		}
	}
	
	// If we didn't find it, well...make it.
	if(!bFound) {		
		// Create a private chat, setting the parent to this window...
		if((pPrivate = new CPrivateChat) != NULL) {
			// Sweet. Go ahead and assign the receiver, then make the dialog.
			pPrivate->m_szReceiver = szName;
			BOOL ret = pPrivate->Create(IDD_PRIVATE_CHAT, this);
			if(ret) {
					// Success! Add it to the collection and show it.
					m_colChatWindows.Add(pPrivate);
					pPrivate->ShowWindow(SW_SHOW);
			}
			else {
				// Ah crap. We'll change the error message just a little bit for the sake of argument.
				AfxMessageBox(_T("Magenta could not create and initialize a private chat window."));
				delete pPrivate;
			}
		}
		else {
			// Uh-oh.
			AfxMessageBox(_T("Magenta could not create a private chat window."));
		}
	}
	else {
		// Shift focus to that window.
		pPrivate->SetFocus();
	}
}

// Remember: this does not free the memory! It only removes the pointer
// from the list.
void CChatWindow::RemovePrivateChat(CString szName)
{
	INT_PTR iUpper, i;
	CPrivateChat* pPrivate = NULL;
	
	iUpper = m_colChatWindows.GetUpperBound();

	for(i = 0; i <= iUpper; i++) {
		pPrivate = (CPrivateChat*)(m_colChatWindows.GetAt(i));
		if(pPrivate->m_szReceiver == szName) {
			// Got it!
			m_colChatWindows.RemoveAt(i);
			break;
		}
	}
}

// Public function that allows others (i.e. the ignored user dialog) to 
// easily remove ignored users. Fortunately, this list is never
// that long, so there shouldn't be any speed issues.
void CChatWindow::RemoveIgnored(CString szIP)
{
	INT_PTR iUpper, i;
	CPerson* pPerson = NULL;
	
	iUpper = m_colIgnored.GetUpperBound();

	for(i = 0; i <= iUpper; i++) {
		pPerson = (CPerson*)(m_colIgnored.GetAt(i));
		if(pPerson->szIP == szIP) {
			// Got it!
			m_colIgnored.RemoveAt(i);
			delete pPerson;
			break;
		}
	}
}

// Utility function that resolves a username to a mangled IP address
CString CChatWindow::ResolveNameToIP(CString szName)
{
	INT_PTR iUpper, i;
	CPerson* pPerson = NULL;
	CString szIP = _T("<empty>");
	
	m_csUsers.Lock();
	iUpper = m_colUsers.GetUpperBound();

	for(i = 0; i <= iUpper; i++) {
		pPerson = (CPerson*)(m_colUsers.GetAt(i));
		if(pPerson->szName == szName) {
			// Got it!
			szIP = pPerson->szIP;
			break;
		}
	}
	m_csUsers.Unlock();
	return szIP;
}

CPerson* CChatWindow::GetPersonByName(CString szName)
{
	CPerson* pPerson = NULL;
	CPerson* pToRet = NULL;
	INT_PTR i, iUpper;

	m_csUsers.Lock();
	iUpper = m_colUsers.GetUpperBound();

	for(i = 0; i <= iUpper; i++) {
		pPerson = (CPerson*)(m_colUsers.GetAt(i));
		if(pPerson->szName == szName) {
			// Got it!
			pToRet = pPerson;
			break;
		}
	}

	m_csUsers.Unlock();
	return pToRet;
}

CPerson* CChatWindow::GetIgnoredByIP(CString szIP)
{
	CPerson* pPerson = NULL;
	CPerson* pToRet = NULL;
	INT_PTR i, iUpper;

	iUpper = m_colIgnored.GetUpperBound();

	for(i = 0; i <= iUpper; i++) {
		pPerson = (CPerson*)(m_colIgnored.GetAt(i));
		if(pPerson->szIP == szIP) {
			// Got it!
			pToRet = pPerson;
			break;
		}
	}

	return pToRet;
}

// Checks to see if a particular name is contained in the block list, or if we block
// the chat server.
BOOL CChatWindow::IsBlocked(CString szName)
{
	BOOL bRet = FALSE;
	int i;

	if(szName == _T("ChatServer") && m_bBlockServer) {
		// Block it.
		bRet = TRUE;
	}
	else {
		for(i = 0; i < m_szaBlocked.GetSize(); i++) {
			if(szName == m_szaBlocked.GetAt(i)) {
				bRet = TRUE;
				break;
			}
		}
	}

	return bRet;
}

// This function, well, it simply checks to see
// if someone is ignored or not. Pretty simple really.
BOOL CChatWindow::IsIgnored(CString szIP, CString szName)
{
	INT_PTR iUpper, i;
	CPerson* pPerson = NULL;
	BOOL bResult = FALSE;

	// Again..this is another linear search...doy.
	iUpper = m_colIgnored.GetUpperBound();

	for(i = 0; i <= iUpper; i++) {
		pPerson = (CPerson*)(m_colIgnored.GetAt(i));
		if(pPerson->szIP == szIP) {
			// They've been ignored, all right.
			bResult = TRUE;
			if(pPerson->szName != szName) {
				// Update the name, for later use.
				pPerson->szName = szName;
			}
			break;
		}
	}
	return bResult;
}

// Performs the ignore function.
void CChatWindow::IgnoreUser(CString szName, CString szIP, BOOL bLoud, BOOL bMutual)
{
	CPerson* pPerson = NULL;
	CPerson* pIgnPerson = NULL;
	BOOL bFound = FALSE;
	CString szCommand;

	// Sanity check:
	// First, we need to see if we have someone already.
	if(IsIgnored(szName, szIP)) {
		// Well..er..they're already ignored. Ignoring someone again is bad. c.c
		// Of course, by the logic of the ignore button, this shouldn't ever happen.
		AfxMessageBox(_T("You are already ignoring the mangled IP ") + szIP + _T("."));
	}
	else {
		// Okay, they're...not. Let's get 'em.
		pIgnPerson = GetPersonByName(szName);

		// Create and initialize a new Person object, and insert it into the list.
		if((pPerson = new CPerson) != NULL) {
			pPerson->bConnected = TRUE;
			pPerson->szIP = szIP;
			pPerson->szName = szName;
			m_colIgnored.Add(pPerson);
			// Now, if we're sending the command (a "hard ignore") we send it to the
			// server here.
			if(m_bSendIgnore && !bMutual) {
				szCommand.Format(_T("%d|%d%s\r\n"), CMD_IGNORE_USER, pIgnPerson->ntType, pIgnPerson->szName);
				m_archCyanSend->WriteString(szCommand);
				m_archCyanSend->Flush();
			}
		
			// Notify the user, if need be.
			if(bLoud) {
				PrintMessage(_T("You are now ignoring [") + szName + 
					_T("] and all of their aliases from address ") + szIP + _T("."), 
					msgMagenta, _T("[Magenta]"));
			}
		}
		else {
			AfxMessageBox(_T("Unable to ignore user - out of memory."));
		}
	}
}

// This function just gets rid of all of those pesky name
// and chat window arrays that're sitting around.
void CChatWindow::DestroyArrays()
{
	// Just invoke the helper on all the arrays.
	// The actual memory used goes away when the class goes away.
	DestroyArrays_Help(&m_colChatWindows);
	DestroyArrays_Help(&m_colIgnored);
	DestroyArrays_Help(&m_colUsers);
}

// Helper function for the above.
void CChatWindow::DestroyArrays_Help(CObArray* obArray)
{
	int i;
	CObject* objTemp;

	for(i = 0; i <= obArray->GetUpperBound(); i++) {
		objTemp = obArray->GetAt(i);
		delete objTemp;
	}

	// Remove the dead pointers from the array
	obArray->RemoveAll();
}


