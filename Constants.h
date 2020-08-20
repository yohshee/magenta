///////////////////////////////////////////////////////////
// Necessary constants for getting Magenta to do its thing.
//


#ifndef _CONSTANTS_H_INCLUDED
#define _CONSTANTS_H_INCLUDED

// Enums
typedef enum _MessageFormat {
	mfPrivate,
	mfBroadcast,
	mfEnter,
	mfLeave
} MessageFormat;

typedef enum _NameType {
	ntRegular,
    ntCyan,
    ntChatServer,
    ntChatClient,
    ntSpecialGuest
} NameType;

typedef enum _MessageType {
    msgServer,
    msgChat,
    msgMagenta,
    msgGuest,
    msgCyan,
    msgPrivate // Protocol "extension" - for printing sent private message text.
} MessageType;

// Default host settings.
#define REMOTE_HOST		_T("cho.cyan.com")
#define	REMOTE_PORT		_T("1812")

// Count constants
#define NAME_LIMIT		19

// Colors
#define COLOR_LIME		RGB(0,0xFF,0)
#define COLOR_GOLD		RGB(0xFF,0xC8,00)
#define COLOR_GRAY		RGB(0xC0,0xC0,0xC0)

// Icon constants
#define ICON_REGULAR	0
#define ICON_CYAN		1
#define ICON_GUEST		2

// Autocomplete stuff
#define ID_AUTO_BASE	0x100		// I'm taking a helluva risk here...limit of 255 matches.
#define MATCH_LIMIT		255
#define ID_MASK			0xFF		// Well, we have to get rid of that upper bit somehow. o.o;

// Log filters
#define SLF_ALL				0x0
#define SLF_REVERSE			0x1
#define SLF_PRIVATE_ONLY	0x2
#define SLF_NORMAL_ONLY		0x4
#define SLF_DATE			0x10

// Additional menu constants
#define MENU_CHAT			0
#define MENU_TOOLS			1

#define SUBMENU_OPACITY		11
#define SUBMENU_LOGGING		1

// Server commands
#define CMD_SEND_BROADCAST  30
#define CMD_SEND_PRIVATE    20
#define CMD_SET_NAME        10
#define CMD_UNSET_NAME      15
#define CMD_SET_PROTOCOL    40
#define CMD_IGNORE_USER     70

#define CC_PROTOCOL_ID      1

// Server responses
#define RESP_NAME_ACK       11
#define RESP_NAME_NAK       10
#define RESP_PRIVATE_MSG    21
#define RESP_BROADCAST_MSG  31
#define RESP_NAME_LIST      35
#define RESP_CHATSERV_MSG   40
#define RESP_IGNORED        70

#endif // _CONSTANTS_H_INCLUDED