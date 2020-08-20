// ChatConsole.cpp : implementation file
//

#include "stdafx.h"
#include "Person.h"
#include "Magenta.h"
#include "mdlsmain.h"
#include "ChatWindow.h"
#include "Resource.h"
#include "ChatConsole.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static TCHAR szFilter[] = _T("Log Files (*.log)|*.log|Text Files (*.txt)|*.txt|All Files (*.*)|*.*||");
static TCHAR szExt[] = _T("log");

/////////////////////////////////////////////////////////////////////////////
// CChatConsole dialog


CChatConsole::CChatConsole(CWnd* pParent /*=NULL*/)
	: CDialog(CChatConsole::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChatConsole)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CChatConsole::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChatConsole)
	DDX_Control(pDX, IDC_RTF_CONSOLE, m_rtfPanel);
	DDX_Control(pDX, IDC_CONSOLE_SAVE, m_cmdSave);
	DDX_Control(pDX, IDC_CONSOLE_CLOSE, m_cmdClose);
	DDX_Control(pDX, IDC_CONSOLE_CLEAR, m_cmdClear);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChatConsole, CDialog)
	//{{AFX_MSG_MAP(CChatConsole)
	ON_BN_CLICKED(IDC_CONSOLE_CLOSE, OnConsoleClose)
	ON_BN_CLICKED(IDC_CONSOLE_CLEAR, OnConsoleClear)
	ON_BN_CLICKED(IDC_CONSOLE_SAVE, OnConsoleSave)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChatConsole message handlers

void CChatConsole::OnCancel()
{
	// No-op
}

void CChatConsole::OnConsoleClose() 
{
	// Um...not much to do here. o.O
	// Pass along to OnClose.
	CChatConsole::OnClose();
}

void CChatConsole::OnConsoleClear() 
{
	CTime time = CTime::GetCurrentTime();
	// Wee...something EASY. (Sort of.)
	m_rtfPanel.SetWindowText(_T("[Magenta Console restarted at ") + time.Format(IDS_TIME_FORMAT) + _T("]\r\n"));
}

void CChatConsole::OnConsoleSave() 
{
	CString szText;
	CStdioFile filLog;
	CFileDialog dlgFile(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		szFilter, this);  // All the init. rigmarole...
		
	// All we're doing is dumping the text out to the file...weee...

	// First, we need to customize this here dialog, soooo...
	dlgFile.m_ofn.lpstrTitle = _T("Save Log File");
	dlgFile.m_ofn.lpstrDefExt = szExt;
	dlgFile.m_ofn.nFilterIndex = 1;
	if(dlgFile.DoModal() == IDOK) {
		// Right then..capture the text in the box and dump it into a file.
		m_rtfPanel.GetWindowText(szText);
		try {
			filLog.Open(dlgFile.GetPathName(), CFile::modeWrite | CFile::modeCreate | CFile::typeText);
			filLog.WriteString(szText);
			filLog.Close();
		}
		catch(CFileException* e) {
			AfxMessageBox(_T("An error occurred while trying to write the log file: ") + e->m_cause, MB_ICONSTOP);
		}
	}
	
}

void CChatConsole::PostNcDestroy() 
{
	CDialog::PostNcDestroy();
	delete this;
}

void CChatConsole::OnClose() 
{
	CChatWindow* pParent;

	// Call default, flip off the console active flag, then destroy the window.
	CDialog::OnClose();
	pParent = (CChatWindow*)GetParent();
	pParent->m_bConsole = FALSE;
	DestroyWindow();
}

BOOL CChatConsole::OnInitDialog() 
{
	CHARFORMAT cf;
	CTime time = CTime::GetCurrentTime();

	CDialog::OnInitDialog();
	
	// Mmkay...just change the font here...
	m_rtfPanel.GetDefaultCharFormat(cf);
	_tccpy(cf.szFaceName, _T("Courier New"));
	m_rtfPanel.SetDefaultCharFormat(cf);
	m_rtfPanel.SetWindowText(_T("[Magenta Console started at ") + time.Format(IDS_TIME_FORMAT) + _T("]\r\n"));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


// Public method for printing to the console itself.
// There's not much to it..just pull out the existing text, append the
// packet, and stuff it back in.
void CChatConsole::PrintConsole(CString szMessage)
{
	CString szBuffer;
	CTime time = CTime::GetCurrentTime();

	m_rtfPanel.GetWindowText(szBuffer);
	szBuffer += _T("[") + time.Format(IDS_TIME_FORMAT) + _T("]") + szMessage + _T("\r\n");
	m_rtfPanel.SetWindowText(szBuffer);
}