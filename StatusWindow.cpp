// StatusWindow.cpp : implementation file
//

#include "stdafx.h"
#include "Magenta.h"
#include "Person.h"
#include "Constants.h"
#include "ChatWindow.h"
#include "StatusWindow.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static TCHAR szFilter[] = _T("Log Files (*.log)|*.log|Text Files (*.txt)|*.txt|All Files (*.*)|*.*||");
static TCHAR szExt[] = _T("log");

#define ADDR_HEAD		1
#define NAME_HEAD		0

/////////////////////////////////////////////////////////////////////////////
// CStatusWindow dialog


CStatusWindow::CStatusWindow(CWnd* pParent /*=NULL*/)
	: CDialog(CStatusWindow::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStatusWindow)
	//}}AFX_DATA_INIT

}


void CStatusWindow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatusWindow)
	DDX_Control(pDX, IDC_LIST_STATUS, m_lvwStatus);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatusWindow, CDialog)
	//{{AFX_MSG_MAP(CStatusWindow)
	ON_NOTIFY(LVN_GETINFOTIP, IDC_LIST_STATUS, OnGetInfoTipListStatus)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_STAT_FILE_CLOSE, OnStatFileClose)
	ON_COMMAND(ID_STAT_FILE_LOG, OnStatFileLog)
	ON_COMMAND(ID_STAT_FILE_SAVE, OnStatFileSave)
	ON_UPDATE_COMMAND_UI(ID_STAT_FILE_LOG, OnUpdateStatFileLog)
	ON_COMMAND(ID_STAT_VIEW_ICON, OnStatViewIcon)
	ON_UPDATE_COMMAND_UI(ID_STAT_VIEW_ICON, OnUpdateStatViewIcon)
	ON_COMMAND(ID_STAT_VIEW_REPORT, OnStatViewReport)
	ON_UPDATE_COMMAND_UI(ID_STAT_VIEW_REPORT, OnUpdateStatViewReport)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatusWindow message handlers

// Right...now we're sticking the info in a tooltip. Wee.
// This handler is invoked each time the mouse pointer is over an item
// for a certain amount of time.
void CStatusWindow::OnGetInfoTipListStatus(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVGETINFOTIP* pInfoTip;
	LVITEM lvi;
	CPerson* pPerson;
	static TCHAR szTooltip[1024];

	pInfoTip = (NMLVGETINFOTIP*)pNMHDR;

	if(pInfoTip->dwFlags == LVGIT_UNFOLDED) {
		// Right..the tooltip needs to be set.
		::ZeroMemory(&lvi, sizeof(lvi));
		lvi.mask = LVIF_PARAM | LVIF_TEXT;
		lvi.iItem = pInfoTip->iItem;
		if(m_lvwStatus.GetItem(&lvi)) {
			// Extract the person, format the string, and
			// stuff in the tooltip.
			pPerson = (CPerson*)lvi.lParam;
			::wsprintf(szTooltip, 
				_T("Username: %s\r\nMangled IP Address: %s"),
				pPerson->szName,
				pPerson->szIP
			);

			pInfoTip->pszText = szTooltip;
		}
		else {
			// Er. How in the WORLD do we notify failure, here?
			// Well, for now..I'll just stuff it into the tooltip and scare the
			// crap out of the user. 
			pInfoTip->pszText = _T("INTERNAL ERROR:\r\n\tUnable to fetch item data.");
		}

	}
	// Set the return code.
	*pResult = 0;
	
}

BOOL CStatusWindow::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CWinApp* pApp;

	pApp = AfxGetApp();

	m_bLogging = FALSE;
	m_bReport = FALSE;

	// Fix up the menus
	GetMenu()->GetSubMenu(1)->CheckMenuItem(ID_STAT_VIEW_ICON, MF_CHECKED | MF_BYCOMMAND);

	// Add an extended style onto the list view; that is, enable tooltips and full-row select.
	m_lvwStatus.SetExtendedStyle(m_lvwStatus.GetExtendedStyle() | LVS_EX_INFOTIP | LVS_EX_FULLROWSELECT);

	// Set up column headers
	OnStatViewReport();
	m_lvwStatus.InsertColumn(NAME_HEAD, _T("User Name"));
	m_lvwStatus.InsertColumn(ADDR_HEAD, _T("Mangled Address"));
	m_lvwStatus.SetColumnWidth(NAME_HEAD, LVSCW_AUTOSIZE_USEHEADER);
	m_lvwStatus.SetColumnWidth(ADDR_HEAD, LVSCW_AUTOSIZE_USEHEADER);
	OnStatViewIcon();

	// Load up the image list
	m_imlStatus.Create(32, 32, ILC_COLOR4, 3, 1);
	m_imlStatus.SetBkColor(RGB(255,0,255));
	iaImages[ICON_REGULAR] = m_imlStatus.Add(pApp->LoadIcon(IDI_NORMAL));
	iaImages[ICON_CYAN] = m_imlStatus.Add(pApp->LoadIcon(IDI_CYAN));
	iaImages[ICON_GUEST] = m_imlStatus.Add(pApp->LoadIcon(IDI_GUEST));

	m_lvwStatus.SetImageList(&m_imlStatus, LVSIL_NORMAL);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStatusWindow::PostNcDestroy() 
{
	CDialog::PostNcDestroy();
	// Necessary for modeless dialogs.
	delete this;
}

void CStatusWindow::OnSize(UINT nType, int cx, int cy) 
{

	CDialog::OnSize(nType, cx, cy);

	if(::IsWindow(m_lvwStatus.m_hWnd)) {
		m_lvwStatus.MoveWindow(0, 0, cx, cy);
	}
}

// Overridden to prevent ESC from being handled, and therefore closing the window.
void CStatusWindow::OnCancel()
{
	// No-op
}

void CStatusWindow::OnClose() 
{
	CChatWindow* pParent = (CChatWindow*)GetParent();
	CDialog::OnClose();
	if(m_bLogging)
		m_filLog.Close();
	
	pParent->m_bStatus = FALSE;

	DestroyWindow();
}

void CStatusWindow::OnStatFileClose() 
{
	// Not much to do here...
	CStatusWindow::OnClose();
}

void CStatusWindow::OnStatFileLog() 
{
	CTime time = CTime::GetCurrentTime();
	CFileException ex;
	CFileDialog dlgFile(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		szFilter, this);
		
	if(!m_bLogging) {
		dlgFile.m_ofn.lpstrTitle = _T("Save Log File");
		dlgFile.m_ofn.lpstrDefExt = szExt;
		dlgFile.m_ofn.nFilterIndex = 1;
		if(dlgFile.DoModal() == IDOK) {
			TRY {
				if(m_filLog.Open(dlgFile.GetPathName(), 
					CFile::modeWrite | CFile::modeCreate | CFile::typeText, &ex) == FALSE)
					THROW(&ex);

				m_filLog.WriteString(_T("[ Magenta log started at ") + time.Format(IDS_TIME_FORMAT) + _T("]\n"));
			}
			CATCH(CFileException, e) {
				AfxMessageBox(_T("An error occurred while trying to open the log file: ") + e->m_cause, MB_ICONSTOP);
				return;
			}
			END_CATCH
			m_bLogging = TRUE;
			GetMenu()->GetSubMenu(0)->CheckMenuItem(ID_STAT_FILE_LOG, MF_CHECKED | MF_BYCOMMAND);
		}
	}
	else {
		m_filLog.Close();
		m_bLogging = FALSE;
		GetMenu()->GetSubMenu(0)->CheckMenuItem(ID_STAT_FILE_LOG, MF_UNCHECKED | MF_BYCOMMAND);
	}
}

void CStatusWindow::OnStatFileSave() 
{
	int i;
	CPerson* pPerson;
	CString szText;
	CStdioFile filLog;
	CFileException ex;
	CFileDialog dlgFile(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		szFilter, this);  

	dlgFile.m_ofn.lpstrTitle = _T("Save Log File");
	dlgFile.m_ofn.lpstrDefExt = szExt;
	dlgFile.m_ofn.nFilterIndex = 1;
	if(dlgFile.DoModal() == IDOK) {
		// We need to iterate through all of the list items and capture the data within
		// to the file.
		try {
			if(filLog.Open(dlgFile.GetPathName(), 
				CFile::modeWrite | CFile::modeCreate | CFile::typeText, &ex) == NULL)
				THROW(&ex);
		}
		catch(CFileException* e) {
			AfxMessageBox(_T("An error occurred while trying to open the log file: ") + e->m_cause, MB_ICONSTOP);
			return;
		}

		for(i = 0; i < m_lvwStatus.GetItemCount(); i++) {
			pPerson = (CPerson*)m_lvwStatus.GetItemData(i);
	
			szText = _T("[") + pPerson->szName + _T("]\n");
			szText += _T("IPAddress=") + pPerson->szIP + _T("\n\n");

			try {
				filLog.WriteString(szText);
			}
			catch(CFileException* e) {
				AfxMessageBox(_T("An error occurred while trying to write the log file: ") + e->m_cause, MB_ICONSTOP);
				return;
			}
		}

		filLog.Close();
	}
}

// Public method for updating the status window (if present)
void CStatusWindow::InsertPerson(CPerson* pPerson, int iType)
{
	CString szText;
	CTime time = CTime::GetCurrentTime();
	int iIndex;

	// Insert it. Also save the pointer to this CPerson with the item so we
	// can get at it later.
	
	if((iIndex = m_lvwStatus.InsertItem(LVIF_PARAM | LVIF_TEXT | LVIF_IMAGE, 0, pPerson->szName, 
		0, 0, iaImages[iType], (LPARAM)pPerson)) == -1) {
		// Crap!
		AfxMessageBox(_T("Error in inserting new item!"));
	}
	else {
		m_lvwStatus.SetItemText(iIndex, 1, pPerson->szIP);

		if(m_bLogging) {
			// Add to the log...
			try {
				m_filLog.WriteString(_T("[") + time.Format(IDS_TIME_FORMAT) + 
					_T("]") + pPerson->szName + _T("/") + pPerson->szIP + _T("\n"));
			}
			catch(CFileException* e) {
				AfxMessageBox(_T("An error occurred while trying to write to the log file: ") + e->m_cause, MB_ICONSTOP);
			}
		}
	}
		
}

// Public method that just..wipes out everything in the listview.
void CStatusWindow::Clear()
{
	m_lvwStatus.DeleteAllItems();
	ASSERT(m_lvwStatus.GetItemCount() == 0);
}

void CStatusWindow::OnUpdateStatFileLog(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bLogging);
}

void CStatusWindow::OnStatViewIcon() 
{
	if(m_bReport) {
		// Just flip it into icon view...
		m_lvwStatus.ModifyStyle(LVS_REPORT, LVS_ICON);
		m_bReport = FALSE;
		GetMenu()->CheckMenuItem(ID_STAT_VIEW_ICON, MF_CHECKED | MF_BYCOMMAND);
		GetMenu()->CheckMenuItem(ID_STAT_VIEW_REPORT, MF_UNCHECKED | MF_BYCOMMAND);
	}
}

void CStatusWindow::OnUpdateStatViewIcon(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(!m_bReport);
}

void CStatusWindow::OnStatViewReport() 
{
	if(!m_bReport) {
		// ...and this changes into report view.
		m_lvwStatus.ModifyStyle(LVS_ICON, LVS_REPORT);
		m_bReport = TRUE;
		GetMenu()->CheckMenuItem(ID_STAT_VIEW_REPORT, MF_CHECKED | MF_BYCOMMAND);
		GetMenu()->CheckMenuItem(ID_STAT_VIEW_ICON, MF_UNCHECKED | MF_BYCOMMAND);
	}
}

void CStatusWindow::OnUpdateStatViewReport(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bReport);
}
