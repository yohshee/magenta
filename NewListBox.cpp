// NewListBox.cpp : implementation file
//

#include "stdafx.h"
#include "Magenta.h"
#include "Person.h"
#include "NewListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewListBox

CNewListBox::CNewListBox()
{
}

CNewListBox::~CNewListBox()
{
}


BEGIN_MESSAGE_MAP(CNewListBox, CListBox)
	//{{AFX_MSG_MAP(CNewListBox)
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewListBox message handlers


void CNewListBox::SetEditBox(CNewEditBox* edt)
{
	m_ptxtEdit = edt;
}

CNewEditBox* CNewListBox::GetEditBox()
{
	return m_ptxtEdit;
}


// It's really kinda sad that I had to make an entire new class just to
// override ONE behavior...but, c'est la vie.
void CNewListBox::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CString szName;
	CString szMessage;
	int iSelection, iCount;

	
	// For now, all we're seeing is if we've held down the control key.
	if(nFlags & MK_CONTROL) {
		// Mmkay, paste the current selection into the message textbox (if we can, anyway)
		iSelection = GetCurSel();
		iCount = GetCount();
		if(iCount > 0 && iSelection != LB_ERR) {
			m_pcsUsers->Lock();
			szName = ((CPerson*)m_pcolUsers->GetAt(GetItemData(iSelection)))->szName;
			m_pcsUsers->Unlock();
			szName += _T('>'); // Append this for formatting reasons.
			// Next, pull out the message and stick it on.
			m_ptxtEdit->GetWindowText(szMessage);
			szMessage.Insert(0, szName + _T(' '));
			m_ptxtEdit->SetWindowText(szMessage);
		}
	}

	CListBox::OnLButtonUp(nFlags, point);
}
