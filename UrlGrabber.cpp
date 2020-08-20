// UrlGrabber.cpp : implementation file
//

#include "stdafx.h"
#include "Magenta.h"
#include "UrlGrabber.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUrlGrabber dialog


CUrlGrabber::CUrlGrabber(CWnd* pParent /*=NULL*/)
	: CDialog(CUrlGrabber::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUrlGrabber)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CUrlGrabber::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUrlGrabber)
	DDX_Control(pDX, IDC_LIST_URLS, m_lstURLs);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUrlGrabber, CDialog)
	//{{AFX_MSG_MAP(CUrlGrabber)
	ON_LBN_DBLCLK(IDC_LIST_URLS, OnDblclkListUrls)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUrlGrabber message handlers

BOOL CUrlGrabber::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	INT_PTR i, iSize;

	// You'd better hope that you've added all of your URLs. ;p
	iSize = m_szaURL.GetSize();
	for(i = 0; i < iSize; i++)
		m_lstURLs.AddString(m_szaURL.GetAt(i));
	
	m_szURL = _T("");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUrlGrabber::OnOK() 
{
	CString szURL;
	int iSel;

	// There's not much to do here. If there's a selection,
	// just ShellExecute it.
	if((iSel = m_lstURLs.GetCurSel()) == LB_ERR) {
		AfxMessageBox(_T("Nothing has been selected."));
		// Return so we don't close the dialog.
		return;
	}
	else {
		m_lstURLs.GetText(iSel, szURL);
		m_szURL = szURL;
	}

	CDialog::OnOK();
}

void CUrlGrabber::OnDblclkListUrls() 
{
	// Just invoke OnOK. It does the same thing.
	OnOK();
}


void CUrlGrabber::AddURL(CString szURL)
{
	// You know, I really hate this level of indirection I
	// have to make...
	//m_lstURLs.AddString(szURL);
	m_szaURL.Add(szURL);
}
