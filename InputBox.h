#if !defined(AFX_INPUTBOX_H__8BE85E63_46AC_4DD5_8CAA_CF9EBDD00627__INCLUDED_)
#define AFX_INPUTBOX_H__8BE85E63_46AC_4DD5_8CAA_CF9EBDD00627__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InputBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInputBox dialog

class CInputBox : public CDialog
{
// Construction
public:
	CInputBox(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CInputBox)
	enum { IDD = IDD_INPUTBOX };
	CString	m_txtEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInputBox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInputBox)
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INPUTBOX_H__8BE85E63_46AC_4DD5_8CAA_CF9EBDD00627__INCLUDED_)
