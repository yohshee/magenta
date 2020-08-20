#if !defined(AFX_IGNORED_H__DA5191B4_CB74_48D2_9045_7F73EE64614D__INCLUDED_)
#define AFX_IGNORED_H__DA5191B4_CB74_48D2_9045_7F73EE64614D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Ignored.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIgnored dialog

class CIgnored : public CDialog
{
// Construction
public:
	CIgnored(CWnd* pParent = NULL);   // standard constructor
	void AddPerson(CPerson* person);			// Adds an entry to the list quite easily.
// Dialog Data
	//{{AFX_DATA(CIgnored)
	enum { IDD = IDD_SHOW_IGNORED };
	CButton	m_cmdUnignore;
	CButton	m_cmdClose;
	CListCtrl	m_lvwIgnored;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIgnored)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Protected members
	CObArray	m_colPeople;		// Array of person objects that're added.
	// Generated message map functions
	//{{AFX_MSG(CIgnored)
	virtual BOOL OnInitDialog();
	afx_msg void OnUnignore();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IGNORED_H__DA5191B4_CB74_48D2_9045_7F73EE64614D__INCLUDED_)
