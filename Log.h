// Log.h: interface for the CLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOG_H__3A8E5346_25CD_43BC_83A0_50D64158E924__INCLUDED_)
#define AFX_LOG_H__3A8E5346_25CD_43BC_83A0_50D64158E924__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef enum _LogFileType {
	lftText,
	lftHTML,
	lftXML
} LogFileType;


// Type that defines a log file entry.
typedef struct _LOGENTRY {
	CString szName;					// Name of sender
	CString szIP;					// Mangled IP of sender
	MessageType mtType;				// Type of message.
	CString szDate;					// String form of timestamp of entry
	CString szMessage;				// The actual message
	MessageFormat mfFormat;			// Format of Message
	struct _LOGENTRY* pPrevious;	// Previous entry
	struct _LOGENTRY* pNext;		// Next entry
} LOGENTRY;


class CLog  
{
public:
	CLog();
	virtual ~CLog();

	void WriteFile(LogFileType iType, CString szFilename, DWORD dwFilter = SLF_ALL);
	void WriteEntry(CString szName, CString szIP, MessageType mtUserType, CString szMessage, MessageFormat mfFormat);
	void ClearLog();
	BOOL IsEmpty();
	void DebugPrintEntries();

protected:

	LOGENTRY*	m_pHead;		// Head of log entry list
	LOGENTRY*	m_pTail;		// Tail of log entry list (yes, this is queue-like)

	CCriticalSection m_csList;	// Critical section used for syncing access to the list. IS NECESSARY.

	// File writing methods
	void WriteTextFile(CString szFilename, DWORD dwFilter);
	void WriteHTMLFile(CString szFilename);
	void WriteXMLFile(CString szFilename); 

	CString TypeToString(MessageType mtType);
	CString FormatToString(MessageFormat mfFormat);
	void FixMessage(CString& szMessage, BOOL bXML = FALSE);

	// Log entry methods (will be expanded as necessary)
	LOGENTRY* AllocLogEntry(CString szName, CString szIP, MessageType mtType, CString szDate, CString szMessage, MessageFormat mfFormat);
	void AddLogEntry(LOGENTRY* pEntry);
	void DestroyEntries();
};

#endif // !defined(AFX_LOG_H__3A8E5346_25CD_43BC_83A0_50D64158E924__INCLUDED_)
