#if !defined(AFX_STATUSWINDOW_H__A4242010_01D5_462A_94FC_B41ABCD5E851__INCLUDED_)
#define AFX_STATUSWINDOW_H__A4242010_01D5_462A_94FC_B41ABCD5E851__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// StatusWindow.h : header file
//
#include "Person.h"

/////////////////////////////////////////////////////////////////////////////
// CStatusWindow dialog

class CStatusWindow : public CDialog
{
// Construction
public:
	CStatusWindow(CWnd* pParent = NULL);   // standard constructor

	void InsertPerson(CPerson* pPerson, int iType);			// Inserts a new entry.
	void Clear();											// Clears out all of the entries.

// Dialog Data
	//{{AFX_DATA(CStatusWindow)
	enum { IDD = IDD_STATUS_WINDOW };
	CListCtrl	m_lvwStatus;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatusWindow)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	virtual void OnCancel();
	//}}AFX_VIRTUAL

// Implementation
protected:

	CStdioFile m_filLog;			// Logger file.
	CImageList m_imlStatus;			// Image list for the dialog.
	int iaImages[3];				// Array of ints holding image indices.
	BOOL m_bLogging;				// Indicates whether we are logging or not.
	BOOL m_bReport;					// Indicates whether we are using report view or not.

	// Generated message map functions
	//{{AFX_MSG(CStatusWindow)
	afx_msg void OnGetInfoTipListStatus(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnStatFileClose();
	afx_msg void OnStatFileLog();
	afx_msg void OnStatFileSave();
	virtual BOOL OnInitDialog();
	afx_msg void OnUpdateStatFileLog(CCmdUI* pCmdUI);
	afx_msg void OnStatViewIcon();
	afx_msg void OnUpdateStatViewIcon(CCmdUI* pCmdUI);
	afx_msg void OnStatViewReport();
	afx_msg void OnUpdateStatViewReport(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATUSWINDOW_H__A4242010_01D5_462A_94FC_B41ABCD5E851__INCLUDED_)
