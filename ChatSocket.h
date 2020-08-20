#if !defined(AFX_CHATSOCKET_H__F729FF05_77F0_4AB6_99B7_3D8E9B6DAE1B__INCLUDED_)
#define AFX_CHATSOCKET_H__F729FF05_77F0_4AB6_99B7_3D8E9B6DAE1B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChatSocket.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CChatSocket command target

class CChatSocket : public CSocket
{
// Attributes
public:

// Operations
public:
	CChatSocket();
	virtual ~CChatSocket();

// Overrides
public:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChatSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CChatSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHATSOCKET_H__F729FF05_77F0_4AB6_99B7_3D8E9B6DAE1B__INCLUDED_)
