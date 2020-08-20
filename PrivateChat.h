#if !defined(AFX_PRIVATECHAT_H__A6AC58EA_4218_4119_AA68_5B3862262BCD__INCLUDED_)
#define AFX_PRIVATECHAT_H__A6AC58EA_4218_4119_AA68_5B3862262BCD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PrivateChat.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPrivateChat dialog

class CPrivateChat : public CDialog
{
// Construction
public:
	CPrivateChat(CWnd* pParent = NULL);   // standard constructor
	void PrintMessage(CString szMessage);
	CString m_szReceiver;

// Dialog Data
	//{{AFX_DATA(CPrivateChat)
	enum { IDD = IDD_PRIVATE_CHAT };
	CRichEditCtrl	m_txtPanel;
	CButton	m_cmdSend;
	CButton	m_cmdSave;
	CButton	m_cmdClose;
	CEdit	m_txtMessage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPrivateChat)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPrivateChat)
	afx_msg void OnCancel();
	afx_msg void OnPrivateClose();
	afx_msg void OnPrivateSave();
	afx_msg void OnPrivateSend();
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRIVATECHAT_H__A6AC58EA_4218_4119_AA68_5B3862262BCD__INCLUDED_)
