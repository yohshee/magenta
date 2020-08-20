// PrivateChat.cpp : implementation file
//

#include "stdafx.h"
#include "Person.h"
#include "Magenta.h"
#include "ChatWindow.h"
#include "Resource.h"
#include "PrivateChat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


static TCHAR szFilter[] = _T("Log Files (*.log)|*.log|Text Files (*.txt)|*.txt|All Files (*.*)|*.*||");
static TCHAR szExt[] = _T("log");

/////////////////////////////////////////////////////////////////////////////
// CPrivateChat dialog


CPrivateChat::CPrivateChat(CWnd* pParent /*=NULL*/)
	: CDialog(CPrivateChat::IDD, pParent)
{
	m_szReceiver = _T("");

	//{{AFX_DATA_INIT(CPrivateChat)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CPrivateChat::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPrivateChat)
	DDX_Control(pDX, IDC_RTF_PRIVATE_PANEL, m_txtPanel);
	DDX_Control(pDX, IDC_PRIVATE_SEND, m_cmdSend);
	DDX_Control(pDX, IDC_PRIVATE_SAVE, m_cmdSave);
	DDX_Control(pDX, IDC_PRIVATE_CLOSE, m_cmdClose);
	DDX_Control(pDX, IDC_EDIT_PRIVATE_MSG, m_txtMessage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPrivateChat, CDialog)
	//{{AFX_MSG_MAP(CPrivateChat)
	ON_BN_CLICKED(IDC_PRIVATE_CLOSE, OnPrivateClose)
	ON_BN_CLICKED(IDC_PRIVATE_SAVE, OnPrivateSave)
	ON_BN_CLICKED(IDC_PRIVATE_SEND, OnPrivateSend)
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// Prints a message to the panel...not much else to say.
void CPrivateChat::PrintMessage(CString szMessage)
{
	CString szPanelText;
	CTime time = CTime::GetCurrentTime();
	CHARFORMAT cf;

	// This works quite similar to the send command...and it's mostly copy
	// and paste. 9.9
	m_txtPanel.GetWindowText(szPanelText);
	// Set the selection to the end, and the length to zero.
	m_txtPanel.SetSel(szPanelText.GetLength() + 1, szPanelText.GetLength() + 1);
	// Get and change the format.
	m_txtPanel.GetSelectionCharFormat(cf);
	cf.dwMask |= CFM_COLOR;
	cf.crTextColor = RGB(0, 255, 255); // Set the text color to cyan.
	cf.dwEffects &= ~CFE_AUTOCOLOR;
	m_txtPanel.SetSelectionCharFormat(cf);
	// Set up the string.
	szPanelText = _T("");
	szPanelText = szPanelText + _T("[") + time.Format(IDS_TIME_FORMAT) + 
		_T("] ") + m_szReceiver + _T("> ") + szMessage + _T("\r\n");
	m_txtPanel.ReplaceSel(szPanelText);
}

/////////////////////////////////////////////////////////////////////////////
// CPrivateChat message handlers

// Yep, that's it...these just handle the buttons and...things.

void CPrivateChat::OnPrivateClose() 
{
	// Just pass on to onclose.
	OnClose();
}

void CPrivateChat::OnPrivateSave() 
{
	CString szText;
	CStdioFile filLog;
	CTime time = CTime::GetCurrentTime();
	CFileDialog dlgFile(FALSE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		szFilter, this);
		
	dlgFile.m_ofn.lpstrTitle = _T("Save Log File");
	dlgFile.m_ofn.lpstrDefExt = szExt;
	dlgFile.m_ofn.nFilterIndex = 1;
	if(dlgFile.DoModal() == IDOK) {
		m_txtPanel.GetWindowText(szText);
		try {
			filLog.Open(dlgFile.GetPathName(), CFile::modeWrite | CFile::modeCreate | CFile::typeText);
			filLog.WriteString(_T("[ Magenta Private Chat Log saved at ") +
				time.Format(IDS_TIME_FORMAT) + _T(". ]\n"));
			filLog.WriteString(szText);
			filLog.Close();
		}
		catch(CFileException* e) {
			AfxMessageBox(_T("An error occurred while trying to write the log file: ") + e->m_cause, MB_ICONSTOP);
		}
	}
}

void CPrivateChat::OnPrivateSend() 
{
	CChatWindow *pParent;
	CString szText, szPanelText, szName;
	CTime time = CTime::GetCurrentTime();
	CHARFORMAT cf;

	// Simple enough, just hand it off to the parent.
	pParent = (CChatWindow*)GetParent();
	m_txtMessage.GetWindowText(szText);
	pParent->SendPrivate(m_szReceiver, szText, FALSE);
	pParent->m_txtName.GetWindowText(szName);

	// Interpret the "/me" if it's there.
	szText.TrimLeft();
	if(szText.Left(4).CompareNoCase(_T("/me ")) == 0) {
		szText.Delete(0,4);
		szText = _T("*") + szText + _T("*");
	}

	// Print it to the display...
	m_txtPanel.GetWindowText(szPanelText);
	// Set the selection to the end, and the length to zero.
	m_txtPanel.SetSel(szPanelText.GetLength() + 1, szPanelText.GetLength() + 1);
	// Get and change the format.
	m_txtPanel.GetSelectionCharFormat(cf);
	cf.dwMask = cf.dwMask | CFM_COLOR;
	cf.crTextColor = RGB(255, 0, 255); // Set the text color to magenta.
	cf.dwEffects = cf.dwEffects & ~CFE_AUTOCOLOR;
	m_txtPanel.SetSelectionCharFormat(cf);
	// Set up the string.
	szPanelText = "";
	szPanelText = szPanelText + _T("[") + time.Format(IDS_TIME_FORMAT) + _T("] ") + szName + 
		_T("> ") + szText + _T("\r\n");
	m_txtPanel.ReplaceSel(szPanelText);
	m_txtMessage.SetWindowText(_T(""));
}

void CPrivateChat::OnClose() 
{
	CChatWindow* pParent;

	CDialog::OnClose();

	// Dissociate the window from the main window before destroying it.
	pParent = (CChatWindow*)GetParent();
	pParent->RemovePrivateChat(m_szReceiver);
	DestroyWindow();
}

BOOL CPrivateChat::OnInitDialog() 
{
	CString szText = _T("Private Chat - ");
	CDialog::OnInitDialog();
	
	// Set the title of the dialog to that of the name.
	SetWindowText(szText + m_szReceiver);
	// Set the background color of the panel.
	m_txtPanel.SetBackgroundColor(FALSE, RGB(0,0,0));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPrivateChat::PostNcDestroy() 
{
	CDialog::PostNcDestroy();
	delete this;
}

void CPrivateChat::OnCancel()
{
	// No-op
}

HBRUSH CPrivateChat::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	CChatWindow* pParent = (CChatWindow*)GetParent();
	
	// Only change the color of the listboxes and textboxes.
	switch(nCtlColor) {
	case CTLCOLOR_EDIT:
	case CTLCOLOR_MSGBOX:
	case CTLCOLOR_LISTBOX:
		pDC->SetBkColor(RGB(0,0,0));
		pDC->SetTextColor(RGB(255,255,255));
		return (HBRUSH)(pParent->m_pEditBkBrush->GetSafeHandle());
	default:
		return hbr; // Nothing to do, return default brush.
	}
}
