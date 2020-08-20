#if !defined(AFX_NAMESLIDE_H__2F54F9C8_CCBF_4782_A9FE_3521EB477F37__INCLUDED_)
#define AFX_NAMESLIDE_H__2F54F9C8_CCBF_4782_A9FE_3521EB477F37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NameSlide.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNameSlide dialog

class CNameSlide : public CDialog
{
// Construction
public:
	CNameSlide(CWnd* pParent = NULL);   // standard constructor
	CString m_szName;

// Dialog Data
	//{{AFX_DATA(CNameSlide)
	enum { IDD = IDD_NAME_SLIDE };
	CEdit	m_txtName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNameSlide)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNameSlide)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NAMESLIDE_H__2F54F9C8_CCBF_4782_A9FE_3521EB477F37__INCLUDED_)
