#if !defined(AFX_SEARCH_H__FF4C9177_B72C_4941_83E0_A998B7A9EA89__INCLUDED_)
#define AFX_SEARCH_H__FF4C9177_B72C_4941_83E0_A998B7A9EA89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Search.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSearch dialog

class CSearch : public CDialog
{
// Construction
public:
	CSearch(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSearch)
	enum { IDD = IDD_SEARCH };
	CRichEditCtrl	m_txtSnap;
	CButton	m_cmdSearch;
	CEdit	m_txtSearch;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSearch)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	FINDTEXTEX m_fteCurrent;		// Current search
	BOOL m_bSearch;					// Is there an existing search?

	// Generated message map functions
	//{{AFX_MSG(CSearch)
	virtual void OnOK();
	afx_msg void OnChangeEditSearchText();
	virtual BOOL OnInitDialog();
	afx_msg void OnClickEditSearchSnap(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEARCH_H__FF4C9177_B72C_4941_83E0_A998B7A9EA89__INCLUDED_)
