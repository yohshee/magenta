#if !defined(AFX_BLOCKLIST_H__82F075BE_7A99_4AE4_BBB8_EDD0970FFAC0__INCLUDED_)
#define AFX_BLOCKLIST_H__82F075BE_7A99_4AE4_BBB8_EDD0970FFAC0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BlockList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBlockList dialog

class CBlockList : public CDialog
{
// Construction
public:
	CBlockList(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CBlockList)
	enum { IDD = IDD_BLOCK_LIST };
	CListBox	m_lstBlocked;
	CEdit	m_txtName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBlockList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBlockList)
	afx_msg void OnBlockAdd();
	afx_msg void OnBlockRemove();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BLOCKLIST_H__82F075BE_7A99_4AE4_BBB8_EDD0970FFAC0__INCLUDED_)
