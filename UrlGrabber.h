#if !defined(AFX_URLGRABBER_H__D48E05FA_EA39_4FAB_A050_50ACDA08D20B__INCLUDED_)
#define AFX_URLGRABBER_H__D48E05FA_EA39_4FAB_A050_50ACDA08D20B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UrlGrabber.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUrlGrabber dialog

class CUrlGrabber : public CDialog
{
// Construction
public:
	CUrlGrabber(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUrlGrabber)
	enum { IDD = IDD_URL_GRABBER };
	CListBox	m_lstURLs;
	//}}AFX_DATA
	CString m_szURL;

// Operations
	void AddURL(CString szURL);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUrlGrabber)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	CStringArray m_szaURL;		// URLs to add

	// Generated message map functions
	//{{AFX_MSG(CUrlGrabber)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkListUrls();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_URLGRABBER_H__D48E05FA_EA39_4FAB_A050_50ACDA08D20B__INCLUDED_)
