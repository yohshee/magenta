// AutoLog.cpp : implementation file
//

#include "stdafx.h"
#include "Magenta.h"
#include "AutoLog.h"
#include "Constants.h"
#include "Log.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAutoLog dialog


CAutoLog::CAutoLog(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoLog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAutoLog)
	//}}AFX_DATA_INIT
}


void CAutoLog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoLog)
	DDX_Control(pDX, IDC_CHECK_HOUR, m_chkHour);
	DDX_Control(pDX, IDC_RADIO_XML, m_optXML);
	DDX_Control(pDX, IDC_RADIO_TEXT, m_optText);
	DDX_Control(pDX, IDC_RADIO_HTML, m_optHTML);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAutoLog, CDialog)
	//{{AFX_MSG_MAP(CAutoLog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoLog message handlers

BOOL CAutoLog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	LogFileType ltType;
	CWinApp* pApp = AfxGetApp();

	// Initialize the option buttons.

	ltType = (LogFileType)pApp->GetProfileInt(_T("Settings"), _T("AutoLogType"), lftText);

	switch(ltType) {
	case lftText:
		m_optText.SetCheck(TRUE);
		break;
	case lftHTML:
		m_optHTML.SetCheck(TRUE);
		break;
	case lftXML:
		m_optXML.SetCheck(TRUE);
		break;
	default:
		// Default to text.
		m_optText.SetCheck(TRUE);
		break;
	}
	
	m_chkHour.SetCheck(pApp->GetProfileInt(_T("Settings"), _T("HourlyLog"), FALSE));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAutoLog::OnOK() 
{
	CWinApp* pApp = AfxGetApp();

	// Write the new value out to the registry...
	if(m_optText.GetCheck() == TRUE) {
		pApp->WriteProfileInt(_T("Settings"), _T("AutoLogType"), lftText);
	}
	else if(m_optHTML.GetCheck() == TRUE) {
		pApp->WriteProfileInt(_T("Settings"), _T("AutoLogType"), lftHTML);
	}
	else if(m_optXML.GetCheck() == TRUE) {
		pApp->WriteProfileInt(_T("Settings"), _T("AutoLogType"), lftXML);
	}

	pApp->WriteProfileInt(_T("Settings"), _T("HourlyLog"), m_chkHour.GetCheck());
	
	CDialog::OnOK();
}
