#if !defined(AFX_CHATCONSOLE_H__2DC25C42_BDA1_4F12_B756_A56E08A63AE8__INCLUDED_)
#define AFX_CHATCONSOLE_H__2DC25C42_BDA1_4F12_B756_A56E08A63AE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChatConsole.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChatConsole dialog

class CChatConsole : public CDialog
{
// Construction
public:
	CChatConsole(CWnd* pParent = NULL);   // standard constructor

	void PrintConsole(CString szMessage);

// Dialog Data
	//{{AFX_DATA(CChatConsole)
	enum { IDD = IDD_CHAT_CONSOLE };
	CRichEditCtrl	m_rtfPanel;
	CButton	m_cmdSave;
	CButton	m_cmdClose;
	CButton	m_cmdClear;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChatConsole)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChatConsole)
	afx_msg void OnConsoleClose();
	afx_msg void OnConsoleClear();
	afx_msg void OnConsoleSave();
	afx_msg void OnClose();
	afx_msg void OnCancel();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHATCONSOLE_H__2DC25C42_BDA1_4F12_B756_A56E08A63AE8__INCLUDED_)
