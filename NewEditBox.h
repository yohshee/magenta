#if !defined(AFX_NEWEDITBOX_H__38D12799_0C29_4276_A18F_B37ADA47CACE__INCLUDED_)
#define AFX_NEWEDITBOX_H__38D12799_0C29_4276_A18F_B37ADA47CACE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewEditBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewEditBox window

class CNewEditBox : public CEdit
{
// Construction
public:
	CNewEditBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewEditBox)
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNewEditBox();
	CListBox* m_plstRegulars;  // Members for working with the other two listboxes.
	CListBox* m_plstCyan;
	CObArray* m_pcolUsers;		// Pointer to the collection of connected users
	CCriticalSection* m_pcsUsers;	// Sync object for dealing with said collection

	// Generated message map functions
protected:

	CMenu m_mnuAuto;			// Autocorrect menu
	int m_iSelStart;			// Cached selstart and sellength
	int m_iSelLength;
	BOOL m_bHadMenu;

	void ProcessAutoComplete();
	int FindRev(CString szString, CString szFind, int iStart);

	//{{AFX_MSG(CNewEditBox)
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg LRESULT OnUninitPopup(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWEDITBOX_H__38D12799_0C29_4276_A18F_B37ADA47CACE__INCLUDED_)
