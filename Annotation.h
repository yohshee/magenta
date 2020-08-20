#if !defined(AFX_ANNOTATION_H__996BB094_F2D4_4779_8BAA_68D8143CDA49__INCLUDED_)
#define AFX_ANNOTATION_H__996BB094_F2D4_4779_8BAA_68D8143CDA49__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Annotation.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAnnotation dialog

class CAnnotation : public CDialog
{
// Construction
public:
	CAnnotation(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAnnotation)
	enum { IDD = IDD_ANNOTATION };
	CEdit	m_txtOldAnn;
	CEdit	m_txtNewAnn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnnotation)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAnnotation)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANNOTATION_H__996BB094_F2D4_4779_8BAA_68D8143CDA49__INCLUDED_)
