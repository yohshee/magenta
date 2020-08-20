#if !defined(AFX_AUTOLOG_H__E76F465B_FE37_423E_B10E_104EE3622A71__INCLUDED_)
#define AFX_AUTOLOG_H__E76F465B_FE37_423E_B10E_104EE3622A71__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoLog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAutoLog dialog

class CAutoLog : public CDialog
{
// Construction
public:
	CAutoLog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAutoLog)
	enum { IDD = IDD_AUTO_LOG_TYPE };
	CButton	m_chkHour;
	CButton	m_optXML;
	CButton	m_optText;
	CButton	m_optHTML;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoLog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAutoLog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOLOG_H__E76F465B_FE37_423E_B10E_104EE3622A71__INCLUDED_)
