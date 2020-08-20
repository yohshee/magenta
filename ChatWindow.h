// ChatWindow.h : header file
//

#if !defined(AFX_CHATWINDOW_H__68A5BE04_6AAD_4156_A36F_569A0D4C3CAC__INCLUDED_)
#define AFX_CHATWINDOW_H__68A5BE04_6AAD_4156_A36F_569A0D4C3CAC__INCLUDED_

#include "mdlsmain.h"
#include "NewListBox.h"
#include "NewEditBox.h"
#include "Constants.h"
#include "Person.h"
#include "Log.h"
#include "ChatSocket.h"
#include "ChatConsole.h"
#include "StatusWindow.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


/////////////////////////////////////////////////////////////////////////////
// CChatWindow dialog

class CChatWindow : public CModelessMain
{
// Construction
public:
	//CChatWindow(CWnd* pParent = NULL);	// standard constructor
	BOOL Create();
	void RemovePrivateChat(CString szName);
	void SendPrivate(CString szName, CString szMessage, BOOL bLoud = TRUE, MessageFormat mfFormat = mfPrivate);
	void RemoveIgnored(CString szIP);
	void ProcessPacket();
	void ProcessClose(int iCode);
	void InvertTrayIcon(BOOL bInvert = TRUE);

	CBrush* m_pEditBkBrush;		// Add a brush to fix up the edit controls
	CChatConsole* m_pConsole;	// Console pointer
	CStatusWindow* m_pStatus;	// Status window pointer.
	BOOL m_bConsole;			// Indicates if the console is active.
	BOOL m_bStatus;				// Indicates if the status window is active.

// Dialog Data
	//{{AFX_DATA(CChatWindow)
	enum { IDD = IDD_MAGENTA_DIALOG };
	CNewEditBox	m_txtMessage;
	CNewListBox	m_lstRegulars;
	CNewListBox	m_lstCyan;
	CStatic	m_lblWho;
	CStatic	m_lblName;
	CButton	m_fraButtons;
	CStatic	m_lblCyan;
	CStatic	m_lblError;
	CButton	m_cmdSendPrivate;
	CButton	m_cmdSend;
	CRichEditCtrl	m_rtfScratch;
	CButton	m_cmdPrivate;
	CRichEditCtrl	m_txtPanel;
	CButton	m_cmdIgnore;
	CEdit	m_txtName;
	CButton	m_cmdChat;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChatWindow)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	// Protected members
	BOOL m_bConnected;			// Keeps track of whether the chatter is connected.
	BOOL m_bActivated;			// Has the window been "activated" for the first time?
	BOOL m_bIsNamed;			// Has the user named himself/herself?
	BOOL m_bCyan;				// Has the Cyanite listbox been clicked recently?
	BOOL m_bOnTop;              // Are we set to always be on top?
	
	CString m_szHost;			// Hostname to connect to.
	UINT	m_uPort;			// Port to which we connect.

	UINT m_uHours;				// Number of hours you've been connected.
	UINT m_uMins;				// Number of minutes you've been connected.
	UINT m_uSecs;				// ...I think you can figure this one out.
	UINT m_uTimerID;			// Connected timer id

	CString m_szName;			// The current name of the chatter
	CString m_szPoseFrame;		// What do we frame /me poses with?
	COLORREF m_crNameColor;		// The color of the chatter's name
	CObArray m_colUsers;		// A collection of connected users
	CCriticalSection m_csUsers; // Sync object for the users list (probably not necessary, but good to have)
	CCriticalSection m_csPanel;	// Sync object for who gets to print to the panel.
	CObArray m_colChatWindows;	// Collection of chat windows
	CObArray m_colIgnored;		// Collection of ignored users
	CStringArray m_szaBlocked;	// The block list
	CStringArray m_szaURLs;		// URL Grabber array

	CLog m_Log;					// Logging object
	BOOL m_bLogging;			// Are we logging?
	BOOL m_bCheckHour;          // Do we check every hour?
	BOOL m_bHour;               // Variable signaling to the log thread that an hour has passed.
	HANDLE m_hEventLog;			// Event for signaling the log thread
	CString m_szLogAnno;        // Annotation to log filename

	HACCEL	m_hAccel;			// Handle to accelerator table

	// Variables for options
	BOOL m_bNotify;				// Notify of new messages?
	BOOL m_bMutIgnore;			// Enable mutual ignores?
	BOOL m_bSendIgnore;			// Send the ignore command when you ignore someone?
	BOOL m_bShowIgnore;			// Display a message when you are ignored?
	BOOL m_bShowConsole;		// Show console on startup?
	BOOL m_bBlockServer;		// Display Chatserver messages?
	BOOL m_bBrowseNew;			// Open URLs in a new window?
	BOOL m_bHideIcon;			// Do we hide the tray icon?

	// The big socket thingies.
	CChatSocket m_sckCyan;
	CSocketFile* m_sfilCyan;
	CArchive* m_archCyanRecv;
	CArchive* m_archCyanSend;

protected:
	// Special functions that only we know about.
	void PrintHostVersionInfo();
	void ServerConnect();
	void ServerDisconnect();
	void NegotiateSession();
	void CreatePrivateChat(CString szName);
	void BroadcastMessage(CString szMessage, MessageFormat mfFormat = mfBroadcast, BOOL bPrivate = FALSE, CString szName = _T(""), BOOL bLoud = FALSE);
	void PrintPrivateMessage(CString szMessage, CString szSender, MessageType mtType, MessageFormat mfFormat);
	void PrintMessage(CString szMessage, MessageType mtType, CString szHeader = "", MessageFormat mfFormat = mfBroadcast, BOOL bPrivate = FALSE, CString szSender = _T(""));
	MessageType TranslateTypeToMessage(NameType nFlag);
	CString ResolveNameToIP(CString szName);
	CPerson* GetPersonByName(CString szName);
	CPerson* GetIgnoredByIP(CString szIP);
	BOOL IsIgnored(CString szIP, CString szName);
	BOOL IsBlocked(CString szName);
	void IgnoreUser(CString szName, CString szIP, BOOL bLoud = TRUE, BOOL bMutual = FALSE);
	void DestroyArrays();
	void DestroyArrays_Help(CObArray* obArray);
	void EnableAllControls();
	void DisableAllControls();
	void AttachSBToBottom();
	void LockInName();
	void UnlockName();
	void SplitUserInfo(CString szInfo, CString& szName, CString& szIP, NameType& ntType);
	void RefreshUserLists();
	void ParseCommands(CString& szString);
	void StartFlashThread();
	void StartLogThread();
	void SetTransparency(int iPercent);
	BOOL IsNT5();
	void SaveLog();
	void GrabURLs(CString szString);
	BOOL VerifyURL(CString& szURL);
	void URLDetect(int iLength = -1);
	BOOL GetScratchCharFormat(CHARFORMAT2& cf, int iType);
	BOOL SetScratchCharFormat(CHARFORMAT2 cf, int iType);
	BOOL GetPanelCharFormat(CHARFORMAT2& cf, int iType);
	BOOL SetPanelCharFormat(CHARFORMAT2 cf, int iType);
	void InstallTrayIcon(BOOL bInstall = TRUE);

	static UINT CChatWindow::LogThreadProc(LPVOID pvData);

	// Generated message map functions
	//{{AFX_MSG(CChatWindow)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnHelpAbout();
	afx_msg void OnChatExit();
	afx_msg void OnChangeEditName();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnChat();
	afx_msg void OnDblclkListRegulars();
	afx_msg void OnClose();
	afx_msg void OnChatConnect();
	afx_msg void OnChatDisconnect();
	afx_msg void OnSelchangeListCyan();
	afx_msg void OnSelchangeListRegulars();
	afx_msg void OnChatClearChatWindow();
	afx_msg void OnToolsSendRawCommand();
	afx_msg void OnPrivateChat();
	afx_msg void OnDblclkListCyan();
	afx_msg void OnCancel();
	afx_msg void OnToolsConsole();
	afx_msg void OnToolsFont();
	afx_msg void OnChatSaveLog();
	afx_msg void OnChatShowIgnored();
	afx_msg void OnToolsOptions();
	afx_msg void OnIgnore();
	afx_msg void OnSend();
	afx_msg void OnSendPrivate();
	afx_msg void OnToolsStatusWindow();
	afx_msg void OnUpdateChatConnect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateChatDisconnect(CCmdUI* pCmdUI);
	afx_msg void OnMaxtextPanel();
	afx_msg void OnErrspacePanel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBlockList();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnOpacity10();
	afx_msg void OnOpacity100();
	afx_msg void OnOpacity20();
	afx_msg void OnOpacity30();
	afx_msg void OnOpacity40();
	afx_msg void OnOpacity50();
	afx_msg void OnOpacity60();
	afx_msg void OnOpacity70();
	afx_msg void OnOpacity80();
	afx_msg void OnOpacity90();
	afx_msg void OnSaveFullLog();
	afx_msg void OnLogging();
	afx_msg void OnUpdateLogging(CCmdUI* pCmdUI);
	afx_msg void OnClearLog();
	afx_msg void OnHelpWebsite();
	afx_msg void OnHelpTopics();
	afx_msg void OnUrlGrabber();
	afx_msg void OnToolsSearch();
	afx_msg void OnChatSwitchName();
	afx_msg void OnUpdateChatSwitchName(CCmdUI* pCmdUI);
	afx_msg void OnToolsOnTop();
	afx_msg void OnUpdateToolsOnTop(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToolsSendRawCommand(CCmdUI* pCmdUI);
	afx_msg void OnToolsAnnotate();
	//}}AFX_MSG
	afx_msg void OnLinkPanel(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnTrayNotify(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHATWINDOW_H__68A5BE04_6AAD_4156_A36F_569A0D4C3CAC__INCLUDED_)
