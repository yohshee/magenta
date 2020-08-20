// Log.cpp: implementation of the CLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Constants.h"
#include "Magenta.h"
#include "Log.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLog::CLog()
{
	m_pHead = NULL;
	m_pTail = NULL;
}

CLog::~CLog()
{
	ClearLog();
}

//////////////////////////////////////////////////////////////////////
// Public Methods
//////////////////////////////////////////////////////////////////////

// If the log is empty, both the head and tail pointers will be null.
BOOL CLog::IsEmpty()
{
	return (m_pHead == NULL && m_pTail == NULL) ? TRUE : FALSE;
}

// Wipes the log from memory
void CLog::ClearLog()
{
	DestroyEntries();
}

// Writes the log file to disk, using one of several formats; to save space
// in this function, use a separate handler function for each format. This may have more
// options added later on. Also...I need to define the filters. ;p
void CLog::WriteFile(LogFileType iType, CString szFilename, DWORD dwFilter)
{
	// Pretty simple, really: just delegate to one of the
	// specialized writing functions. Though I'm not going to pass file pointers around.
	switch(iType) {
	case lftText:
		WriteTextFile(szFilename, dwFilter);
		break;
	case lftHTML:
		WriteHTMLFile(szFilename);
		break;
	case lftXML:
		WriteXMLFile(szFilename);
		break;
	default:
		// Uh-oh.
		TRACE1("Undefined type %d passed to CLog::WriteFile\n", iType);
		// It's fun being arcane:
		AfxMessageBox(_T("Unable to save file: internal error.\n"));
		break;
	}
}

// Writes an entry into the log. It fails silently if it can't do it, so...
void CLog::WriteEntry(CString szName, CString szIP, MessageType mtUserType, CString szMessage, MessageFormat mfFormat)
{
	CTime time = CTime::GetCurrentTime();
	CString szDate;
	LOGENTRY* pEntry;

	szDate = time.Format(IDS_TIME_FORMAT);

	if((pEntry = AllocLogEntry(szName, szIP, mtUserType, szDate, szMessage, mfFormat)) != NULL) {
		AddLogEntry(pEntry);
	}
}


//////////////////////////////////////////////////////////////////////
// File Methods
//////////////////////////////////////////////////////////////////////

void CLog::WriteTextFile(CString szFilename, DWORD dwFilter)
{
	LOGENTRY* pEntry;
	CStdioFile filLog;
	CTime time = CTime::GetCurrentTime();
	CString szDate = time.Format(IDS_TIME_FORMAT);
	CString szEntry;

	// First, we try to open up that there file thinger. Then we dump out data based
	// on the filters.
	try {
		CFileException ex;

		if(filLog.Open(szFilename, 
			CFile::modeWrite | CFile::modeCreate | CFile::typeText, &ex) == FALSE) {
			throw &ex;
		}

		// Print a banner
		szEntry.Format(_T("Magenta log generated at %s.\n\n"), szDate);
		filLog.WriteString(szEntry);
		
		// Start from the end and go to the beginning, for now.
		for(pEntry = m_pTail; pEntry != NULL; pEntry = pEntry->pPrevious) {
			szEntry.Empty();

			// We need to skip over entries if they violate filter conditions. The priority
			// of the filters is private - broadcast - all.
			if((dwFilter & SLF_PRIVATE_ONLY) && 
				((pEntry->mfFormat == mfBroadcast) || 
				(pEntry->mfFormat == mfEnter) || 
				(pEntry->mfFormat == mfLeave)))
				continue;
			if((dwFilter & SLF_NORMAL_ONLY) && (pEntry->mfFormat == mfPrivate))
				continue;

			// Next, if they wanted the date, then...give it to them. o.o;
			if(dwFilter & SLF_DATE)
				szEntry = _T("<") + pEntry->szDate + _T("> ");

			// Finish composing the line, and then write it.
			if(pEntry->mtType == msgPrivate) {
				// If this is sitting there, that means that this was something the
				// user sent. The szName field will have the receiver, then.
				szEntry += _T("Private message sent to [") + pEntry->szName + _T("]: ") + pEntry->szMessage;
			}
			else {
				if(pEntry->mfFormat != mfPrivate) {
					szEntry += _T("[") + pEntry->szName + _T("] ") + pEntry->szMessage;
					// Now, we need to see if it was an announcement...
					if(pEntry->mfFormat == mfEnter) {
						szEntry = _T("\\\\\\\\\\ ") + szEntry + _T(" /////");
					}
					else if(pEntry->mfFormat == mfLeave) {
						szEntry = _T("///// ") + szEntry + _T(" \\\\\\\\\\");
					}
				}
				else {
					szEntry += _T("Private message from [") + pEntry->szName + _T("] ") + pEntry->szMessage;
				}
			}

			szEntry += _T("\n");
			filLog.WriteString(szEntry);
		}
		filLog.Close();
	}
	catch(CFileException* pEx) {
		TCHAR szError[256];
		CString szMessage;

		pEx->GetErrorMessage(szError, 256);

		szMessage = _T("Error in writing log file: ");
		szMessage += szError;

		// Generally, not being able to open the log would be the biggest problem.
		AfxMessageBox(szMessage);
	}
}

// Every one of these functions shares the same basic framework, if you haven't
// noticed that by now. <.<
// By convention, HTML files do not give the time & date that the message was sent.
void CLog::WriteHTMLFile(CString szFilename)
{
	LOGENTRY* pEntry;
	CStdioFile filLog;
	CTime time = CTime::GetCurrentTime();
	CString szDate = time.Format(IDS_TIME_FORMAT);
	CString szEntry;
	CString szColor;
	CString szMessage, szName;

	try {
		CFileException ex;

		if(filLog.Open(szFilename, 
			CFile::modeWrite | CFile::modeCreate | CFile::typeText, &ex) == FALSE) {
			// Ray: Ready...throw it!
			throw &ex;
		}

		// Write doctype & header tags. 
		szEntry.Format(_T("<!doctype HTML PUBLIC \"-//W3C//DTD HTML 4.0 Strict//EN\">\n")
				_T("<html>\n")
				_T("\t<head>\n")
				_T("\t\t<title>Magenta log generated at %s</title>\n"), szDate);

		filLog.WriteString(szEntry);

		// Write the stylesheet. We use classes for each element of the 
		// log, just in case the logger wants to easily make it look different.
		filLog.WriteString(
			_T("\t\t<style type=\"text/css\">\n")			
			_T("\t\tBODY {\n")
			_T("\t\t\tbackground: black;\n")
			_T("\t\t\tcolor: white;\n")
			_T("\t\t}\n\n")
			_T("\t\tP {\n")
			_T("\t\t\tfont-family: MS Sans Serif, Verdana, sans-serif;\n")
			_T("\t\t\tfont-size: x-small;\n")
			_T("\t\t\tfont-style: normal;\n")
			_T("\t\t}\n")
			_T("\t\t.privatesend {\n")
			_T("\t\t\tcolor: red;\n")
			_T("\t\t}\n")
			_T("\t\t.privaterecv {\n")
			_T("\t\t\tcolor: magenta;\n")
			_T("\t\t}\n")
			_T("\t\t.message {\n")
			_T("\t\t\tcolor: gray;\n")
			_T("\t\t}\n")
			_T("\t\t/* Enter / leave markers */\n")
			_T("\t\t.markers {\n")
			_T("\t\t\tcolor: lime;\n")
			_T("\t\t}\n")
			_T("\t\t/* Header highlighting colors */\n")
			_T("\t\t.cyan {\n")
			_T("\t\t\tcolor: cyan;\n")
			_T("\t\t}\n")
			_T("\t\t.guest {\n")
			_T("\t\t\tcolor: orange;\n")
			_T("\t\t}\n")
			_T("\t\t.magenta {\n")
			_T("\t\t\tcolor: magenta;\n")
			_T("\t\t}\n")
			_T("\t\t.chatserver {\n")
			_T("\t\t\tcolor: lime;\n")
			_T("\t\t}\n")
			_T("\t\t.regular {\n")
			_T("\t\t\tcolor: white;\n")
			_T("\t\t}\n")
			_T("\t\t</style>\n")
			);

		filLog.WriteString(_T("\t</head>\n"));

		// Begin and write the body.
		filLog.WriteString(_T("\t<body>\n"));
		szEntry.Format(_T("\t<p>Magenta log generated at %s</p>\n"), szDate);
		filLog.WriteString(szEntry);

		for(pEntry = m_pTail; pEntry != NULL; pEntry = pEntry->pPrevious) {
			szEntry.Empty();

			szMessage = pEntry->szMessage;
			szName = pEntry->szName;

			FixMessage(szMessage);
			FixMessage(szName);

			// Write HTML (each entry is a pair of <p> tags, with <span> for formatting individual elements.)
			// Finish composing the line, and then write it.
			if(pEntry->mtType == msgPrivate) {
				// If this is sitting there, that means that this was something the
				// user sent. The szName field will have the receiver, then.
				szEntry += _T("<span class=\"privatesend\">Private message sent to</span> ");
				// Apparently, my log format has no method of detecting the type of WHO WE SENT IT TO. >.<
				// Of course, in the main code, we don't know the type of who we sent it to, either, so...
				szEntry += _T("[") + szName + _T("]: <span class=\"message\">") + szMessage + _T("</span>");
			}
			else {
				if(pEntry->mfFormat != mfPrivate) {
					// Select the proper color class
					switch(pEntry->mtType) {
						case msgChat:
							szColor = _T("regular");
							break;
						case msgCyan:
							szColor = _T("cyan");
							break;
						case msgGuest:
							szColor = _T("guest");
							break;
						case msgMagenta:
							szColor = _T("magenta");
							break;
						case msgServer:
							szColor = _T("chatserver");
							break;
						default:
							szColor = _T("regular");
							break;
					}

					// Form up the entry
					szEntry += _T("<span class=\"") +  szColor + _T("\">[") + szName + _T("]</span> ");
					szEntry += _T("<span class=\"message\">") + szMessage + _T("</span>");
					// Now, we need to see if it was an announcement...
					if(pEntry->mfFormat == mfEnter) {
						szEntry = _T("<span class=\"markers\">\\\\\\\\\\</span> ") + szEntry + _T(" <span class=\"markers\">/////</span>");
					}
					else if(pEntry->mfFormat == mfLeave) {
						szEntry = _T("<span class=\"markers\">/////</span> ") + szEntry + _T(" <span class=\"markers\">\\\\\\\\\\</span>");
					}
				}
				else {
					szEntry += _T("<span class=\"privaterecv\">Private message from</span> [") + szName + _T("] <span class=\"message\">") + szMessage + _T("</span>");
				}
			}
			
			// Frame it with <p> tags and write it.
			szEntry = _T("\t<p>") + szEntry + _T("</p>\n\n");
			filLog.WriteString(szEntry);
		}

		// Close up the HTML tags, and then close up the file.
		szEntry.Format(_T("\t</body>\n")
			_T("</html>\n"));
		filLog.WriteString(szEntry);

		filLog.Close();
	}
	catch(CFileException* pEx) {
		TCHAR szError[256];
		CString szErrMessage;

		pEx->GetErrorMessage(szError, 256);

		szErrMessage = _T("Error in writing log file: ");
		szErrMessage += szError;

		// Generally, not being able to open the log would be the biggest problem.
		AfxMessageBox(szErrMessage);
	}
}


// Use Magenta's XML format for this one. I'm not so sure about building in 
// functionality for letting people define their own tags...that could get pretty messy
// if you ask me. o.x The great thing about the xml format? I really don't have to worry about FORMATTING this crap.
// In fact...erm...we're going to ignore the filter, too. 
void CLog::WriteXMLFile(CString szFilename)
{
	LOGENTRY* pEntry;
	CStdioFile filLog;
	CTime time = CTime::GetCurrentTime();
	CString szDate = time.Format(IDS_TIME_FORMAT);
	CString szEntry;
	CString szMessage, szName;

	
	try {
		CFileException ex;

		if(filLog.Open(szFilename, 
			CFile::modeWrite | CFile::modeCreate | CFile::typeText | CFile::shareExclusive, &ex) == FALSE) {
			throw &ex;
		}
		
		// Okay, first, we need to write in all of the necessary XML junk, so let's go ahead
		// and do that.
		szEntry.Format(_T("<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>\n")
			_T("<!-- Uncomment the following line to use xsl stylesheet. Name is an example only. -->\n")
			_T("<!-- <?xml-stylesheet type=\"text/xsl\" href=\"magenta.xsl\" ?> -->\n")
			_T("<!-- Magenta log generated on %s -->\n"), szDate);
		filLog.WriteString(szEntry);

		// Write the opening tag for the document.
		filLog.WriteString(_T("<magentalog>\n"));

		// Timestamp it with the <stamp> tag.
		szEntry.Format(_T("\t<stamp time=\"%s\">Magenta log generated on %s</stamp>\n"), szDate, szDate);
		filLog.WriteString(szEntry);

		// Now we need to scribble in the entries.

		for(pEntry = m_pHead; pEntry != NULL; pEntry = pEntry->pNext) {
			szMessage = pEntry->szMessage;
			szName = pEntry->szName;

			FixMessage(szMessage, TRUE);
			FixMessage(szName, TRUE);

			// First, we need to start an entry tag.
			szEntry.Format(_T("\t<entry time=\"%s\">\n"), pEntry->szDate);
			filLog.WriteString(szEntry);

			// Write in a pair of user tags.
			szEntry.Format(_T("\t\t<user ip=\"%s\" type=\"%s\">%s</user>\n"), 
				pEntry->szIP, TypeToString(pEntry->mtType), szName);
			filLog.WriteString(szEntry);

			// Next, write in a pair of message tags.
			szEntry.Format(_T("\t\t<message type=\"%s\">%s</message>\n"), 
				FormatToString(pEntry->mfFormat), szMessage);
			filLog.WriteString(szEntry);

			// Close the entry tags.
			filLog.WriteString(_T("\t</entry>\n"));
		}

		// Close the document, then close the...file.
		filLog.WriteString(_T("</magentalog>\n"));
		filLog.Close();

		// Here is where we'd dump out an XSL file...but, uh...no. 
	}
	catch(CFileException* pEx) {
		TCHAR szError[256];
		CString szErrMessage;

		pEx->GetErrorMessage(szError, 256);

		szErrMessage = _T("Error in writing log file: ");
		szErrMessage += szError;

		// Generally, not being able to open the log would be the biggest problem.
		AfxMessageBox(szErrMessage);
	}
}

// The following functions are just simple translators from the constants to 
// strings. No biggy there. o.o
CString CLog::TypeToString(MessageType mtType)
{
	CString szRet;
	
	switch(mtType) {
	case msgServer:
		szRet = _T("chatserver");
		break;
	case msgChat:
		szRet = _T("user");
		break;
	case msgMagenta:
		szRet = _T("magenta");
		break;
	case msgGuest:
		szRet = _T("guest");
		break;
	case msgCyan:
		szRet = _T("cyan");
		break;
	case msgPrivate:
		szRet = _T("private");
		break;
	default:
		szRet = _T("unknown");
		break;
	}

	return szRet;
}

CString CLog::FormatToString(MessageFormat mfFormat)
{
	CString szRet;

	switch(mfFormat) {
	case mfPrivate:
		szRet = _T("private");
		break;
	case mfBroadcast:
		szRet = _T("broadcast");
		break;
	case mfEnter:
		szRet = _T("broadcast-enter");
		break;
	case mfLeave:
		szRet = _T("broadcast-leave");
		break;
	default:
		szRet = _T("unknown");
		break;
	}

	return szRet;
}

// Removes any bad things from a message that would trip up an XML/HTML parser. String is
// edited in-place.
void CLog::FixMessage(CString& szMessage, BOOL bXML)
{
	// Darn HTML entities. ^^
	szMessage.Replace(_T("&"), _T("&amp;"));
	szMessage.Replace(_T("<"), _T("&lt;"));
	szMessage.Replace(_T(">"), _T("&gt;"));
	szMessage.Replace(_T("\""), _T("&quot;"));
	if(bXML) 
		szMessage.Replace(_T("'"), _T("&apos;"));
}

//////////////////////////////////////////////////////////////////////
// Log Management Methods
//////////////////////////////////////////////////////////////////////

LOGENTRY* CLog::AllocLogEntry(CString szName, CString szIP, MessageType mtType, CString szDate, CString szMessage, MessageFormat mfFormat)
{
	LOGENTRY* pEntry = NULL;

	// Simple enough, allocate and pack the new entry. Yes, I'm very much aware that I'm using
	// C++ and I could've written this as a constructor...
	if((pEntry = new LOGENTRY) != NULL) {
		pEntry->szName = szName;
		pEntry->szIP = szIP;
		pEntry->mtType = mtType;
		pEntry->szDate = szDate;
		pEntry->szMessage = szMessage;
		pEntry->mfFormat = mfFormat;
		pEntry->pNext = NULL;
		pEntry->pPrevious = NULL;
	}

	return pEntry;
}

// Helper function for ClearLog()
void CLog::DestroyEntries()
{
	LOGENTRY* pEntry;
	LOGENTRY* pTemp;

	m_csList.Lock();

	// Iterate over the doubly-linked list, freeing up stuff.
	for(pEntry = m_pHead; pEntry != NULL; pEntry = pTemp) {
		pTemp = pEntry->pNext;
		delete pEntry;
	}

	// Defang the head and tail pointers
	m_pHead = NULL;
	m_pTail = NULL;

	m_csList.Unlock();
}

// Basically a helper function for WriteEntry's stuff. 
void CLog::AddLogEntry(LOGENTRY* pEntry)
{
	m_csList.Lock();

	if(m_pHead == NULL) {
		// Okay, the list hasn't been initialized; point head and tail at this passed
		// in pointer.
		m_pHead = pEntry;
		m_pTail = pEntry;
		m_pHead->pPrevious = NULL; // The head of the list ALWAYS has a pPrevious == NULL.
		m_pTail->pNext = NULL;
	}
	else {
		// Useless comments for the more academic-minded:
		// The point of having a head and tail pointer is to make this thing run faster; we
		// can make inserts O(1) rather than O(n). This is important, as logs get HUGE real
		// quick. Also, we really don't have to worry about deletes. There are only two things
		// you do with this log: add to it and clear it.
		//
		// I'm just making it doubly linked so I can do cool stuff with it, if need be.
		//
		// So, here's what we do, to be as verbose as possible:
		//
		// 1. Point pPrevious at m_pTail, since we're inserting after the tail pointer.
		pEntry->pPrevious = m_pTail;
		// 2. Point pNext at NULL. (i.e. properly terminate the list)
		pEntry->pNext = NULL;
		// 3. Point m_pTail->pNext at pEntry, so that it's actually inserted.
		m_pTail->pNext = pEntry;
		// 4. Set m_pTail to pEntry, advancing forward in the list. We never touch m_pHead.
		m_pTail = pEntry;
	}

	m_csList.Unlock();
}

// Prints all entries in the list to the debug console
void CLog::DebugPrintEntries()
{
	LOGENTRY* pEntry;

	for(pEntry = m_pHead; pEntry != NULL; pEntry = pEntry->pNext) {
		TRACE1("Entry at %s:\n", pEntry->szDate);
		TRACE3("\tName = %s, IP = %s, Message = %s\n", pEntry->szName, pEntry->szIP, pEntry->szMessage);
	}
}