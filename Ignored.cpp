// Ignored.cpp : implementation file
//

#include "stdafx.h"
#include "Magenta.h"
#include "Person.h"
#include "ChatWindow.h"
#include "Ignored.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ADDR_HEAD		0
#define NAME_HEAD		1
#define CONN_HEAD		2

/////////////////////////////////////////////////////////////////////////////
// CIgnored dialog


CIgnored::CIgnored(CWnd* pParent /*=NULL*/)
	: CDialog(CIgnored::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIgnored)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CIgnored::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIgnored)
	DDX_Control(pDX, IDC_UNIGNORE, m_cmdUnignore);
	DDX_Control(pDX, IDCANCEL, m_cmdClose);
	DDX_Control(pDX, IDC_LVW_IGNORED, m_lvwIgnored);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIgnored, CDialog)
	//{{AFX_MSG_MAP(CIgnored)
	ON_BN_CLICKED(IDC_UNIGNORE, OnUnignore)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIgnored message handlers

BOOL CIgnored::OnInitDialog() 
{
	int i;
	CPerson* person;

	CDialog::OnInitDialog();
	
	// Hmm...we need some headers for this thing. Set them up to autosize as needed.
	m_lvwIgnored.InsertColumn(ADDR_HEAD, _T("Mangled Address"));
	m_lvwIgnored.InsertColumn(NAME_HEAD, _T("User Name"));
	m_lvwIgnored.InsertColumn(CONN_HEAD, _T("Connected"));
	m_lvwIgnored.SetColumnWidth(ADDR_HEAD, LVSCW_AUTOSIZE_USEHEADER);
	m_lvwIgnored.SetColumnWidth(NAME_HEAD, LVSCW_AUTOSIZE_USEHEADER);
	m_lvwIgnored.SetColumnWidth(CONN_HEAD, LVSCW_AUTOSIZE_USEHEADER);

	// Mmmkay. Iterate through the array and add all the list items.
	for(i = 0; i <= m_colPeople.GetUpperBound(); i++) {
		person = (CPerson*)m_colPeople.GetAt(i);
		// Insert the address
		m_lvwIgnored.InsertItem(i, person->szIP);
		// Set the name and connected columns
		m_lvwIgnored.SetItem(i, NAME_HEAD, LVIF_TEXT, person->szName, 0, 0, 0, NULL);
		m_lvwIgnored.SetItem(i, CONN_HEAD, LVIF_TEXT, person->bConnected ? _T("Yes") : _T("No"), 0, 0, 0, NULL);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CIgnored::AddPerson(CPerson* person)
{

	m_colPeople.Add(person);
}

void CIgnored::OnUnignore() 
{
	CChatWindow* pParent;
	LVITEM lvi;
	TCHAR szText[256];

	pParent = (CChatWindow*)GetParent();

	// Okay, we need to simply...remove the IP address from the
	// list maintained by the chat window.
	if(m_lvwIgnored.GetItemCount() > 0) {
		if(m_lvwIgnored.GetSelectedCount() > 0) {
			// Since single selection is forced here, we don't
			// have to worry about whether or not they selected more than one.
			::ZeroMemory(&lvi, sizeof(lvi));
			lvi.mask = LVIF_TEXT;
			lvi.iItem = m_lvwIgnored.GetNextItem(-1, LVNI_SELECTED);
			lvi.iSubItem = 0;
			lvi.pszText = szText;
			lvi.cchTextMax = 256;
			if(m_lvwIgnored.GetItem(&lvi)) {
				pParent->RemoveIgnored(lvi.pszText);
				m_lvwIgnored.DeleteItem(m_lvwIgnored.GetNextItem(-1, LVNI_SELECTED));
			}
			else {
				AfxMessageBox(_T("An error occurred getting the information on that ignored member."));
			}
		}
		else {
			AfxMessageBox(_T("You haven't selected a chatter!"));
		}
	}
	else {
		AfxMessageBox(_T("Nobody has been ignored."), MB_ICONINFORMATION);
	}
}
