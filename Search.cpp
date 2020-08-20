// Search.cpp : implementation file
//

#include "stdafx.h"
#include "Magenta.h"
#include "ChatWindow.h"
#include "Search.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSearch dialog


CSearch::CSearch(CWnd* pParent /*=NULL*/)
	: CDialog(CSearch::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSearch)
	//}}AFX_DATA_INIT
}


void CSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSearch)
	DDX_Control(pDX, IDC_EDIT_SEARCH_SNAP, m_txtSnap);
	DDX_Control(pDX, IDOK, m_cmdSearch);
	DDX_Control(pDX, IDC_EDIT_SEARCH_TEXT, m_txtSearch);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSearch, CDialog)
	//{{AFX_MSG_MAP(CSearch)
	ON_EN_CHANGE(IDC_EDIT_SEARCH_TEXT, OnChangeEditSearchText)
	ON_NOTIFY(NM_CLICK, IDC_EDIT_SEARCH_SNAP, OnClickEditSearchSnap)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSearch message handlers


void CSearch::OnOK() 
{
	CString szText;
	CHARRANGE chrgSel;

	// Sanity check, as this button wouldn't be enabled if the editbox
	// were empty.
	m_txtSearch.GetWindowText(szText);
	if(szText.GetLength() > 0) {
		if(!m_bSearch) {
			// Okay, restart the search if there isn't one already.
			m_fteCurrent.chrg.cpMin = 0;
			m_fteCurrent.chrg.cpMax = m_txtSnap.GetTextLength();
			m_fteCurrent.lpstrText = (LPCTSTR)szText;
			m_bSearch = TRUE;
		}
		else {
			// Start the search after the current match.
			m_fteCurrent.chrg.cpMin = m_fteCurrent.chrgText.cpMax + 1;
		}

		// Search from the selection point downwards.
		m_txtSnap.GetSel(chrgSel);
		if(chrgSel.cpMax != 0) {
			m_txtSnap.HideSelection(TRUE, 0);
			chrgSel.cpMax++;
			chrgSel.cpMin = chrgSel.cpMax;
			m_txtSnap.SetSel(chrgSel);
		}

		if(m_txtSnap.FindText(FR_DOWN, &m_fteCurrent) != -1) {
			// Jump down and select the text.
			m_txtSnap.SetSel(m_fteCurrent.chrgText);
			m_txtSnap.SendMessage(EM_SCROLLCARET, 0, 0);
			m_txtSnap.HideSelection(FALSE, 0);
		}
		else {
			AfxMessageBox(_T("There are no more matches."), MB_ICONINFORMATION);
		}
	}

	// We do not want this to close the dialog, so do not call to parent.
	//CDialog::OnOK();
}

 
void CSearch::OnChangeEditSearchText() 
{
	CString szText;

	// If we changed, the current search is gone.
	m_bSearch = FALSE;

	// We want the OK/Search button enabled only if
	// we have text.
	m_txtSearch.GetWindowText(szText);
	if(szText.GetLength() > 0) {
		m_cmdSearch.EnableWindow(TRUE);
		SetDefID(IDOK);
	}
	else {
		m_cmdSearch.EnableWindow(FALSE);
	}
}

BOOL CSearch::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CString szText;

	// Initialize its text.
	CChatWindow* pParent = (CChatWindow*)GetParent();
	pParent->m_txtPanel.GetWindowText(szText);
	m_txtSnap.SetWindowText(szText);

	// Doing this will also disable the search button for us.
	m_txtSearch.SetWindowText(_T(""));
	m_txtSearch.SetSel(0,0);
	m_bSearch = FALSE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSearch::OnClickEditSearchSnap(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// Okay, if they clicked in the snapshot list, they most certainly
	// moved the insertion point. The search is now invalid.
	m_bSearch = FALSE;

	*pResult = 0;
}
