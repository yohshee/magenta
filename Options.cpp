// Options.cpp : implementation file
//

#include "stdafx.h"
#include "Magenta.h"
#include "Constants.h"
#include "Options.h"
#include "AutoLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptions dialog


COptions::COptions(CWnd* pParent /*=NULL*/)
	: CDialog(COptions::IDD, pParent)
{
	//{{AFX_DATA_INIT(COptions)
	//}}AFX_DATA_INIT
}


void COptions::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COptions)
	DDX_Control(pDX, IDC_CHECK_HIDE_TRAY_ICON, m_chkHideIcon);
	DDX_Control(pDX, IDC_CHECK_BROWSE_NEW_WINDOW, m_chkBrowseNew);
	DDX_Control(pDX, IDC_CHECK_CHATSERVER, m_chkChatServer);
	DDX_Control(pDX, IDC_CHECK_CONNECT, m_chkConnect);
	DDX_Control(pDX, IDC_EDIT_USERNAME, m_txtUsername);
	DDX_Control(pDX, IDC_CHECK_LOGIN, m_chkLogin);
	DDX_Control(pDX, IDC_CHECK_CONSOLE, m_chkConsole);
	DDX_Control(pDX, IDC_CHECK_SEND_IGNORE, m_chkSendIgnore);
	DDX_Control(pDX, IDC_CHECK_NOTIFY, m_chkNotify);
	DDX_Control(pDX, IDC_CHECK_MUTUAL, m_chkMutIgnore);
	DDX_Control(pDX, IDC_CHECK_DISPLAY_IGNORE, m_chkDispIgnore);
	DDX_Control(pDX, IDOK, m_cmdOK);
	DDX_Control(pDX, IDCANCEL, m_cmdCancel);
	DDX_Control(pDX, IDC_EDIT_PORT, m_txtPort);
	DDX_Control(pDX, IDC_EDIT_HOSTNAME, m_txtHostname);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COptions, CDialog)
	//{{AFX_MSG_MAP(COptions)
	ON_EN_CHANGE(IDC_EDIT_HOSTNAME, OnChangeEditHostname)
	ON_EN_CHANGE(IDC_EDIT_PORT, OnChangeEditPort)
	ON_BN_CLICKED(IDC_COLOR, OnNameColor)
	ON_BN_CLICKED(IDC_AUTO_LOG, OnAutoLog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptions message handlers

BOOL COptions::OnInitDialog() 
{
	CWinApp* pApp;
	CDialog::OnInitDialog();
	
	pApp = AfxGetApp();

	// Okay..what we want to do is read the old settings in
	// from the Registry, and use those to set up the controls on the thing.
	m_chkDispIgnore.SetCheck(pApp->GetProfileInt(_T("Settings"), _T("ShowIgnore"), TRUE));
	m_chkMutIgnore.SetCheck(pApp->GetProfileInt(_T("Settings"), _T("MutualIgnore"), TRUE));
	m_chkSendIgnore.SetCheck(pApp->GetProfileInt(_T("Settings"), _T("SendIgnore"), TRUE));
	m_chkNotify.SetCheck(pApp->GetProfileInt(_T("Settings"), _T("Notify"), FALSE));
	m_chkConsole.SetCheck(pApp->GetProfileInt(_T("Settings"), _T("ShowConsole"), FALSE));
	m_chkLogin.SetCheck(pApp->GetProfileInt(_T("Settings"), _T("AutoLogin"), FALSE));
	m_chkConnect.SetCheck(pApp->GetProfileInt(_T("Settings"), _T("Connect"), TRUE));
	m_chkChatServer.SetCheck(pApp->GetProfileInt(_T("Settings"), _T("BlockChatServer"), FALSE));
	m_chkBrowseNew.SetCheck(pApp->GetProfileInt(_T("Settings"), _T("BrowseNew"), TRUE));
	m_chkHideIcon.SetCheck(pApp->GetProfileInt(_T("Settings"), _T("HideTrayIcon"), FALSE));

	m_txtUsername.SetLimitText(19);
	m_txtUsername.SetWindowText(pApp->GetProfileString(_T("Settings"), _T("Username"), _T("")));
	m_txtHostname.SetWindowText(pApp->GetProfileString(_T("Settings"), _T("Host"), REMOTE_HOST));
	// The port is saved as a string because it's easier to deal with that way.
	m_txtPort.SetWindowText(pApp->GetProfileString(_T("Settings"), _T("Port"), REMOTE_PORT));

	m_crColor = pApp->GetProfileInt(_T("Settings"), _T("NameColor"), RGB(255,255,255));

	// Set this at the end..because the above changes inadvertently affect this flag.
	m_bTextChange = FALSE;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void COptions::OnOK() 
{
	CWinApp* pApp;
	CString szTemp;
	BOOL bValidName = TRUE;

	pApp = AfxGetApp();

	// Validate the username first.
	m_txtUsername.GetWindowText(szTemp);
	
	if(szTemp.FindOneOf(_T("|^,.")) > -1) {
		AfxMessageBox(_T("A username cannot contain one of the following characters:\r\n")
			_T("|, ^, commas, or periods."));
		bValidName = FALSE;
	}
	else {
		pApp->WriteProfileString(_T("Settings"), _T("Username"), szTemp);
	}

	// Shove all of the settings into the registry, cause we're fetching them out on
	// the other side.
	pApp->WriteProfileInt(_T("Settings"), _T("ShowIgnore"), m_chkDispIgnore.GetCheck());
	pApp->WriteProfileInt(_T("Settings"), _T("MutualIgnore"), m_chkMutIgnore.GetCheck());
	pApp->WriteProfileInt(_T("Settings"), _T("SendIgnore"), m_chkSendIgnore.GetCheck());
	pApp->WriteProfileInt(_T("Settings"), _T("Notify"), m_chkNotify.GetCheck());
	pApp->WriteProfileInt(_T("Settings"), _T("ShowConsole"), m_chkConsole.GetCheck());
	pApp->WriteProfileInt(_T("Settings"), _T("AutoLogin"), m_chkLogin.GetCheck());
	pApp->WriteProfileInt(_T("Settings"), _T("Connect"), m_chkConnect.GetCheck());
	pApp->WriteProfileInt(_T("Settings"), _T("BlockChatServer"), m_chkChatServer.GetCheck());
	pApp->WriteProfileInt(_T("Settings"), _T("BrowseNew"), m_chkBrowseNew.GetCheck());
	pApp->WriteProfileInt(_T("Settings"), _T("HideTrayIcon"), m_chkHideIcon.GetCheck());
	pApp->WriteProfileInt(_T("Settings"), _T("NameColor"), m_crColor);

	m_txtHostname.GetWindowText(szTemp);
	pApp->WriteProfileString(_T("Settings"), _T("Host"), szTemp);
	m_txtPort.GetWindowText(szTemp);
	pApp->WriteProfileString(_T("Settings"), _T("Port"), szTemp);

	if(m_bTextChange)
		AfxMessageBox(_T("Any changes to the hostname or port will not take effect\r\n")
					  _T("until you disconnect and reconnect Magenta."), MB_ICONINFORMATION);

	if(bValidName)
		CDialog::OnOK();
}

void COptions::OnChangeEditHostname() 
{
	// In both instances (this one and the following), just
	// flip the flag TRUE indicating that we have, indeed, changed the boxes.
	m_bTextChange = TRUE;
}

void COptions::OnChangeEditPort() 
{
	m_bTextChange = TRUE;
}

void COptions::OnNameColor() 
{
	CColorDialog dlgColor(m_crColor);  // Create a new color dialog, use current color as default.

	if(dlgColor.DoModal() == IDOK) {
		// Yay, we don't have to go hand over elbow for this one! @.x
		m_crColor = dlgColor.GetColor();
	}
}

void COptions::OnAutoLog() 
{
	CAutoLog dlgAutoLog;

	// Everything is handled within the auto-log dialog, so we just invoke
	// it.
	dlgAutoLog.DoModal();
}
