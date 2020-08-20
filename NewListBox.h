#if !defined(AFX_NEWLISTBOX_H__6E8332BD_7883_498B_8067_EB610279412E__INCLUDED_)
#define AFX_NEWLISTBOX_H__6E8332BD_7883_498B_8067_EB610279412E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewListBox.h : header file
//
#include "NewEditBox.h"

/////////////////////////////////////////////////////////////////////////////
// CNewListBox window

class CNewListBox : public CListBox
{
// Construction
public:
	CNewListBox();

// Attributes
public:
	CObArray* m_pcolUsers;			// Pointer to the collection of connected users
	CCriticalSection* m_pcsUsers;	// Sync object for dealing with said collection
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewListBox)
	//}}AFX_VIRTUAL
	
// Implementation
public:
	void SetEditBox(CNewEditBox* edt);
	CNewEditBox* GetEditBox();
	virtual ~CNewListBox();

	// Generated message map functions
protected:
	CNewEditBox* m_ptxtEdit;

	//{{AFX_MSG(CNewListBox)
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWLISTBOX_H__6E8332BD_7883_498B_8067_EB610279412E__INCLUDED_)
