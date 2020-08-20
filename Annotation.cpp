// Annotation.cpp : implementation file
//

#include "stdafx.h"
#include "Magenta.h"
#include "Annotation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnnotation dialog


CAnnotation::CAnnotation(CWnd* pParent /*=NULL*/)
	: CDialog(CAnnotation::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAnnotation)
	//}}AFX_DATA_INIT
}


void CAnnotation::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnnotation)
	DDX_Control(pDX, IDC_EDIT_OLD_ANN, m_txtOldAnn);
	DDX_Control(pDX, IDC_EDIT_NEW_ANN, m_txtNewAnn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAnnotation, CDialog)
	//{{AFX_MSG_MAP(CAnnotation)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnnotation message handlers

void CAnnotation::OnOK() 
{
	CString szAnno;
	CWinApp* pApp = AfxGetApp();

	// Get the string, and write it to the registry. Not hard.
	m_txtNewAnn.GetWindowText(szAnno);

	pApp->WriteProfileString(_T("Settings"), _T("LogAnnotation"), szAnno);
		
	CDialog::OnOK();
}

BOOL CAnnotation::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CWinApp* pApp = AfxGetApp();
	CString szAnno;

	szAnno = pApp->GetProfileString(_T("Settings"), _T("LogAnnotation"), _T(""));

	m_txtOldAnn.SetWindowText(szAnno);
	m_txtNewAnn.SetWindowText(szAnno);
	m_txtNewAnn.SetSel(0, szAnno.GetLength() - 1);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
