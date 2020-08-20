#if !defined(AFX_OPTIONS_H__B87C1C34_24A0_47B6_971F_B91FAF50E121__INCLUDED_)
#define AFX_OPTIONS_H__B87C1C34_24A0_47B6_971F_B91FAF50E121__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Options.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COptions dialog

class COptions : public CDialog
{
// Construction
public:
	COptions(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COptions)
	enum { IDD = IDD_OPTIONS };
	CButton	m_chkHideIcon;
	CButton	m_chkBrowseNew;
	CButton	m_chkChatServer;
	CButton	m_chkConnect;
	CEdit	m_txtUsername;
	CButton	m_chkLogin;
	CButton	m_chkConsole;
	CButton	m_chkSendIgnore;
	CButton	m_chkNotify;
	CButton	m_chkMutIgnore;
	CButton	m_chkDispIgnore;
	CButton	m_cmdOK;
	CButton	m_cmdCancel;
	CEdit	m_txtPort;
	CEdit	m_txtHostname;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COptions)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	BOOL m_bTextChange;		// Flag flipped when any changes are made to the hostname or port.
	COLORREF m_crColor;		// Temporary var for name color

	// Generated message map functions
	//{{AFX_MSG(COptions)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnChangeEditHostname();
	afx_msg void OnChangeEditPort();
	afx_msg void OnNameColor();
	afx_msg void OnAutoLog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONS_H__B87C1C34_24A0_47B6_971F_B91FAF50E121__INCLUDED_)
