// NewEditBox.cpp : implementation file
//

#include "stdafx.h"
#include "Magenta.h"
#include "Constants.h"
#include "Person.h"
#include "NewEditBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewEditBox

CNewEditBox::CNewEditBox()
{
	// Basic initialization...
	m_iSelStart = 0;
	m_iSelLength = 0;
}

CNewEditBox::~CNewEditBox()
{
}

BEGIN_MESSAGE_MAP(CNewEditBox, CEdit)
	//{{AFX_MSG_MAP(CNewEditBox)
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_MESSAGE(WM_UNINITMENUPOPUP, OnUninitPopup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// Processes an autocomplete request.
void CNewEditBox::ProcessAutoComplete()
{
	int i;
	INT_PTR iCount = 0;
	int iMatches = 0;
	int iStart, iEnd, iMiddle;
	CString szText;
	CString szName, szItem, szTemp;
	CPoint ptCursor;

	GetSel(iStart, iMiddle); // Get the position of the cursor (considered to be iMiddle)
	GetWindowText(szText);   // Also grab ahold of the text in the window.

	// Prepare the menu

	if(m_mnuAuto.GetSafeHmenu() != NULL)
		m_mnuAuto.DestroyMenu();

	if(!m_mnuAuto.CreatePopupMenu()) {
		AfxMessageBox(_T("Unable to generate menu for Tab Complete!"), MB_ICONSTOP | MB_OK);
		return;
	}
	
	// Look for the start of the string.
	if(iMiddle > 0)
		iStart = FindRev(szText, " ", iMiddle - 1);
	else
		iStart = FindRev(szText, " ", 0);

	if(iStart == -1)
		iStart = 0;
	iEnd = szText.Find(_T(" "), iMiddle + 1);
	if(iEnd == -1)
		iEnd = szText.GetLength();
	SetSel(iStart, iEnd);	                    // Cosmetic reasons.
	szName = szText.Mid(iStart, iEnd - iStart); // Because MFC won't give us the selected text..><
	
	if(szName.GetLength() > 0) {
		//iCount = m_plstRegulars->GetCount();
		iCount = m_pcolUsers->GetSize();
		iMatches = 0;
		
		m_pcsUsers->Lock();
		//szName.MakeUpper();
		for(i = 0; i < iCount; i++) {
			szItem = ((CPerson*)m_pcolUsers->GetAt(i))->szName;
			//m_plstRegulars->GetText(i, szItem);
			szTemp = szItem;
			//szTemp.MakeUpper();
			if(szTemp.Left(szName.GetLength()).CompareNoCase(szName) == 0) {
				// We matched. Add one, and add it to the list.
				iMatches++;
				if(iMatches <= MATCH_LIMIT) {
					//m_plstOutput->AddString(szItem);
					m_mnuAuto.AppendMenu(MF_STRING, ID_AUTO_BASE + iMatches, szItem);
				}
			}
		}
		m_pcsUsers->Unlock();

		if(iMatches > 1) {
			// Lock the text in the message textbox and let them choose a completion.
			SetReadOnly(TRUE);
			// Cache selection info
			m_iSelStart = iStart;
			m_iSelLength = iEnd - iStart;
			// Get the insertion point position and popup the menu right where it is.
			ptCursor = GetCaretPos();
			ClientToScreen(&ptCursor);
			m_mnuAuto.TrackPopupMenu(TPM_CENTERALIGN | TPM_LEFTBUTTON,
				ptCursor.x, ptCursor.y, this);
		}
		else if(iMatches == 1) {
			// Merely paste in the last item.
			m_mnuAuto.GetMenuString(0, szItem, MF_BYPOSITION);
			m_mnuAuto.DestroyMenu();
			ReplaceSel(szItem);
		}

		// Fix the selection start.
		GetWindowText(szText);
		SetSel(szText.GetLength(), szText.GetLength());
	}
}

// Searches for a substring starting at the end of a string.
int CNewEditBox::FindRev(CString szString, CString szFind, int iStart)
{
	int iRet = -1;
	int iPos, iLen;

	// Reverse the strings...
	szString.MakeReverse();
	szFind.MakeReverse();

	iLen = szString.GetLength();
	iPos = szString.Find(szFind, iLen - iStart - 1);

	if(iPos != -1)
		iRet = szString.GetLength() - iPos - szFind.GetLength() + 1;

	return iRet;
}

/////////////////////////////////////////////////////////////////////////////
// CNewEditBox message handlers

void CNewEditBox::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch(nChar) {
	case VK_ESCAPE:
		if(!m_bHadMenu) {
			// We want to clear it.
			SetWindowText(_T(""));
		}
		else {
			m_bHadMenu = FALSE; // This flag only makes sense for this case, so far.
		}
		break;
	case VK_TAB:
		ProcessAutoComplete();
		break;
	default:
		// Nothing to do...
		break;
	}
	
	// Call default
	CEdit::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CNewEditBox::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// Doggone tabs! Don't process unless I say so!
	if(nChar != VK_TAB)
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CNewEditBox::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// Another attempt to keep tabs from processing...
	if(nChar != VK_TAB)
		CEdit::OnChar(nChar, nRepCnt, nFlags);
}

BOOL CNewEditBox::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	int iID;
	CString szItem;
	CString szMessage;

	iID = LOWORD(wParam); // Get the command ID. (which should be from the menu)
	if(iID & ID_AUTO_BASE) {
		// It's one of our popups, all right...
		m_mnuAuto.GetMenuString(iID, szItem, MF_BYCOMMAND);
		ASSERT(szItem.GetLength() > 0);
		SetSel(m_iSelStart, m_iSelLength + m_iSelStart);
		ReplaceSel(szItem);
		SetReadOnly(FALSE);
		m_mnuAuto.DestroyMenu();
	}

	return CEdit::OnCommand(wParam, lParam);
}

LRESULT CNewEditBox::OnUninitPopup(WPARAM wParam, LPARAM lParam)
{
	// Hm, let's see....we can try to do this this way. o.o
	// Unfortunately, now my program doesn't work on Win95, but
	// who uses Win95 anymore, anyway?
	if((HMENU)wParam == m_mnuAuto.GetSafeHmenu()) {
		SetReadOnly(FALSE);
		m_bHadMenu = TRUE;
	}

	return 0;
}
