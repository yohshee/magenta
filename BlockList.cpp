// BlockList.cpp : implementation file
//

#include "stdafx.h"
#include "Magenta.h"
#include "BlockList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBlockList dialog


CBlockList::CBlockList(CWnd* pParent /*=NULL*/)
	: CDialog(CBlockList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBlockList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CBlockList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBlockList)
	DDX_Control(pDX, IDC_LIST_BLOCK, m_lstBlocked);
	DDX_Control(pDX, IDC_EDIT_BLOCK_NAME, m_txtName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBlockList, CDialog)
	//{{AFX_MSG_MAP(CBlockList)
	ON_BN_CLICKED(IDC_BLOCK_ADD, OnBlockAdd)
	ON_BN_CLICKED(IDC_BLOCK_REMOVE, OnBlockRemove)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBlockList message handlers

void CBlockList::OnBlockAdd() 
{
	CString szText;

	// First, we need to validate before we add.
	m_txtName.GetWindowText(szText);
	if(szText.GetLength() == 0) {
		AfxMessageBox(_T("You must enter a name."));
	}
	else {
		m_lstBlocked.AddString(szText);
		m_txtName.SetWindowText(_T(""));
	}
}

void CBlockList::OnBlockRemove() 
{
	int iSel;

	if((iSel = m_lstBlocked.GetCurSel()) != LB_ERR) {
		m_lstBlocked.DeleteString(iSel);
	}
	else {
		AfxMessageBox(_T("You must select a name from the list."));
	}
}

void CBlockList::OnOK() 
{
	CWinApp* pApp = AfxGetApp();
	int i, iCount;
	CString szKey, szEntry;

	// First, wipe out the old values.
	iCount = pApp->GetProfileInt(_T("Block List"), _T("Count"), 0);

	if(iCount > 0) {
		for(i = 0; i < iCount; i++) {
			szKey.Format(_T("Entry%d"), i);
			pApp->WriteProfileString(_T("Block List"), szKey, NULL);
		}
	}

	// Next, we just write the new ones to the registry.
	iCount = m_lstBlocked.GetCount();
	pApp->WriteProfileInt(_T("Block List"), _T("Count"), iCount);

	for(i = 0; i < iCount; i++) {
		szKey.Format(_T("Entry%d"), i);
		m_lstBlocked.GetText(i, szEntry);
		pApp->WriteProfileString(_T("Block List"), szKey, szEntry);
	}


	CDialog::OnOK();
}

BOOL CBlockList::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CWinApp* pApp = AfxGetApp();
	int i, iCount;
	CString szKey;

	// Fill up the list with the registry values.
	iCount = pApp->GetProfileInt(_T("Block List"), _T("Count"), 0);

	if(iCount > 0) {
		for(i = 0; i < iCount; i++) {
			szKey.Format(_T("Entry%d"), i);
			m_lstBlocked.AddString(pApp->GetProfileString(_T("Block List"), szKey));
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
