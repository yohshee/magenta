// NameSlide.cpp : implementation file
//

#include "stdafx.h"
#include "Constants.h"
#include "Magenta.h"
#include "NameSlide.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNameSlide dialog


CNameSlide::CNameSlide(CWnd* pParent /*=NULL*/)
	: CDialog(CNameSlide::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNameSlide)
	//}}AFX_DATA_INIT

	m_szName = _T("");
}


void CNameSlide::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNameSlide)
	DDX_Control(pDX, IDC_EDIT_NAME_SLIDE, m_txtName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNameSlide, CDialog)
	//{{AFX_MSG_MAP(CNameSlide)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNameSlide message handlers

BOOL CNameSlide::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Set the limit on the name
	m_txtName.SetLimitText(NAME_LIMIT);
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNameSlide::OnOK() 
{
	// Validate the name.
	m_txtName.GetWindowText(m_szName);

	if(m_szName.FindOneOf(_T("|^,.")) == -1) {
		CDialog::OnOK();
	}
	else {
		AfxMessageBox(_T("Your name contains invalid characters."));
	}
}

