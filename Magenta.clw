; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=COptions
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Magenta.h"

ClassCount=18
Class1=CMagenta
Class2=CChatWindow
Class3=CAboutDlg

ResourceCount=19
Resource1=IDD_ANNOTATION
Resource2=IDR_MAINFRAME
Resource3=IDD_SHOW_IGNORED
Class4=CNewListBox
Class5=CNewEditBox
Resource4=IDR_MENU_MAGENTA
Class6=CInputBox
Resource5=IDD_PRIVATE_CHAT
Class7=CPrivateChat
Resource6=IDD_INPUTBOX
Class8=CChatConsole
Resource7=IDD_STATUS_WINDOW
Class9=CIgnored
Resource8=IDD_AUTO_LOG_TYPE
Class10=COptions
Resource9=IDD_SEARCH
Resource10=IDD_ABOUTBOX
Class11=CStatusWindow
Class12=CChatSocket
Resource11=IDD_URL_GRABBER
Class13=CBlockList
Resource12=IDD_BLOCK_LIST
Class14=CAutoLog
Resource13=IDD_CHAT_CONSOLE
Class15=CUrlGrabber
Resource14=IDD_BUG_REPORT
Class16=CSearch
Resource15=IDD_OPTIONS
Class17=CNameSlide
Resource16=IDR_MENU_STATUS
Resource17=IDD_MAGENTA_DIALOG
Resource18=IDD_NAME_SLIDE
Class18=CAnnotation
Resource19=IDR_ACCELERATOR_MAGENTA

[CLS:CMagenta]
Type=0
HeaderFile=Magenta.h
ImplementationFile=Magenta.cpp
Filter=N
LastObject=CMagenta
BaseClass=CWinApp
VirtualFilter=AC

[CLS:CChatWindow]
Type=0
HeaderFile=ChatWindow.h
ImplementationFile=ChatWindow.cpp
Filter=W
BaseClass=CModelessMain
VirtualFilter=dWC
LastObject=CChatWindow

[CLS:CAboutDlg]
Type=0
HeaderFile=ChatWindow.h
ImplementationFile=ChatWindow.cpp
Filter=W
LastObject=IDC_EDIT_INFO
BaseClass=CDialog
VirtualFilter=dWC

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=6
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC_VERSION,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_FRAME_ABOUT,button,1342177287
Control6=IDC_EDIT_INFO,RICHEDIT,1342244868

[DLG:IDD_MAGENTA_DIALOG]
Type=1
Class=CChatWindow
ControlCount=16
Control1=IDC_STATIC_FRAME,button,1342177287
Control2=IDC_CHAT,button,1342177280
Control3=IDC_IGNORE,button,1342177280
Control4=IDC_PRIVATE_CHAT,button,1342177280
Control5=IDC_SEND_PRIVATE,button,1342177280
Control6=IDC_SEND,button,1342177280
Control7=IDC_STATIC_NAME,static,1342177280
Control8=IDC_EDIT_NAME,edit,1350566016
Control9=IDC_STATIC_ERROR,static,1342308352
Control10=IDC_PANEL,RICHEDIT,1352665156
Control11=IDC_STATIC_WHO,static,1342177280
Control12=IDC_LIST_REGULARS,listbox,1352663297
Control13=IDC_STATIC_CYAN,static,1342177280
Control14=IDC_LIST_CYAN,listbox,1352663297
Control15=IDC_SCRATCH,RICHEDIT,1082130560
Control16=IDC_MESSAGE,edit,1350566016

[MNU:IDR_MENU_MAGENTA]
Type=1
Class=CChatWindow
Command1=ID_CHAT_CONNECT
Command2=ID_CHAT_DISCONNECT
Command3=ID_CHAT_SWITCH_NAME
Command4=ID_CHAT_SHOW_IGNORED
Command5=ID_BLOCK_LIST
Command6=ID_SAVE_FULL_LOG
Command7=ID_CHAT_SAVE_LOG
Command8=ID_CHAT_CLEAR_CHAT_WINDOW
Command9=ID_CHAT_EXIT
Command10=ID_LOGGING
Command11=ID_CLEAR_LOG
Command12=ID_TOOLS_ANNOTATE
Command13=ID_TOOLS_OPTIONS
Command14=ID_TOOLS_FONT
Command15=ID_TOOLS_ON_TOP
Command16=ID_TOOLS_STATUS_WINDOW
Command17=ID_URL_GRABBER
Command18=ID_TOOLS_SEARCH
Command19=ID_TOOLS_CONSOLE
Command20=ID_TOOLS_SEND_RAW_COMMAND
Command21=ID_OPACITY_100
Command22=ID_OPACITY_90
Command23=ID_OPACITY_80
Command24=ID_OPACITY_70
Command25=ID_OPACITY_60
Command26=ID_OPACITY_50
Command27=ID_OPACITY_40
Command28=ID_OPACITY_30
Command29=ID_OPACITY_20
Command30=ID_OPACITY_10
Command31=ID_HELP_TOPICS
Command32=ID_HELP_WEBSITE
Command33=ID_HELP_ABOUT
CommandCount=33

[CLS:CNewListBox]
Type=0
HeaderFile=NewListBox.h
ImplementationFile=NewListBox.cpp
BaseClass=CListBox
Filter=C
VirtualFilter=bWC
LastObject=CNewListBox

[CLS:CNewEditBox]
Type=0
HeaderFile=NewEditBox.h
ImplementationFile=NewEditBox.cpp
BaseClass=CEdit
Filter=D
VirtualFilter=WC
LastObject=CNewEditBox

[DLG:IDD_INPUTBOX]
Type=1
Class=CInputBox
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC_RAW_COMMAND,static,1342308352
Control4=IDC_EDIT_COMMAND,edit,1350631552

[CLS:CInputBox]
Type=0
HeaderFile=InputBox.h
ImplementationFile=InputBox.cpp
BaseClass=CDialog
Filter=D
LastObject=CInputBox
VirtualFilter=dWC

[DLG:IDD_PRIVATE_CHAT]
Type=1
Class=CPrivateChat
ControlCount=5
Control1=IDC_RTF_PRIVATE_PANEL,RICHEDIT,1352730692
Control2=IDC_EDIT_PRIVATE_MSG,edit,1350631552
Control3=IDC_PRIVATE_SEND,button,1342242817
Control4=IDC_PRIVATE_CLOSE,button,1342242816
Control5=IDC_PRIVATE_SAVE,button,1342242816

[CLS:CPrivateChat]
Type=0
HeaderFile=PrivateChat.h
ImplementationFile=PrivateChat.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CPrivateChat

[DLG:IDD_CHAT_CONSOLE]
Type=1
Class=CChatConsole
ControlCount=4
Control1=IDC_CONSOLE_SAVE,button,1342242816
Control2=IDC_CONSOLE_CLOSE,button,1342242816
Control3=IDC_CONSOLE_CLEAR,button,1342242816
Control4=IDC_RTF_CONSOLE,RICHEDIT,1352730692

[CLS:CChatConsole]
Type=0
HeaderFile=ChatConsole.h
ImplementationFile=ChatConsole.cpp
BaseClass=CDialog
Filter=D
LastObject=CChatConsole
VirtualFilter=dWC

[DLG:IDD_SHOW_IGNORED]
Type=1
Class=CIgnored
ControlCount=3
Control1=IDCANCEL,button,1342242816
Control2=IDC_UNIGNORE,button,1342242816
Control3=IDC_LVW_IGNORED,SysListView32,1350664213

[CLS:CIgnored]
Type=0
HeaderFile=Ignored.h
ImplementationFile=Ignored.cpp
BaseClass=CDialog
Filter=D
LastObject=CIgnored
VirtualFilter=dWC

[DLG:IDD_OPTIONS]
Type=1
Class=COptions
ControlCount=23
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_FRAME_HOST,button,1342177287
Control4=IDC_STATIC_HOSTNAME,static,1342308864
Control5=IDC_STATIC_PORT,static,1342308864
Control6=IDC_EDIT_HOSTNAME,edit,1350631552
Control7=IDC_EDIT_PORT,edit,1350631552
Control8=IDC_CHECK_MUTUAL,button,1342242819
Control9=IDC_CHECK_SEND_IGNORE,button,1342242819
Control10=IDC_CHECK_DISPLAY_IGNORE,button,1342251011
Control11=IDC_CHECK_NOTIFY,button,1342242819
Control12=IDC_CHECK_CONSOLE,button,1342242819
Control13=IDC_FRAME_LOGIN,button,1342177287
Control14=IDC_CHECK_LOGIN,button,1342242819
Control15=IDC_STATIC_USERNAME,static,1342308352
Control16=IDC_EDIT_USERNAME,edit,1350631552
Control17=IDC_STATIC_OTHER_OPTS,button,1342177287
Control18=IDC_CHECK_CONNECT,button,1342242819
Control19=IDC_CHECK_CHATSERVER,button,1342242819
Control20=IDC_COLOR,button,1342242816
Control21=IDC_AUTO_LOG,button,1342242816
Control22=IDC_CHECK_BROWSE_NEW_WINDOW,button,1342242819
Control23=IDC_CHECK_HIDE_TRAY_ICON,button,1342242819

[CLS:COptions]
Type=0
HeaderFile=Options.h
ImplementationFile=Options.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=COptions

[DLG:IDD_STATUS_WINDOW]
Type=1
Class=CStatusWindow
ControlCount=1
Control1=IDC_LIST_STATUS,SysListView32,1350664192

[MNU:IDR_MENU_STATUS]
Type=1
Class=?
Command1=ID_STAT_FILE_SAVE
Command2=ID_STAT_FILE_LOG
Command3=ID_STAT_FILE_CLOSE
Command4=ID_STAT_VIEW_ICON
Command5=ID_STAT_VIEW_REPORT
CommandCount=5

[CLS:CStatusWindow]
Type=0
HeaderFile=StatusWindow.h
ImplementationFile=StatusWindow.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CStatusWindow

[CLS:CChatSocket]
Type=0
HeaderFile=ChatSocket.h
ImplementationFile=ChatSocket.cpp
BaseClass=CSocket
Filter=N
VirtualFilter=uq
LastObject=CChatSocket

[DLG:IDD_BLOCK_LIST]
Type=1
Class=CBlockList
ControlCount=8
Control1=IDOK,button,1342242816
Control2=IDCANCEL,button,1342242816
Control3=IDC_BLOCK_ADD,button,1342242817
Control4=IDC_EDIT_BLOCK_NAME,edit,1350631552
Control5=IDC_BLOCK_REMOVE,button,1342242816
Control6=IDC_STATIC_BLOCK_NAME,static,1342308864
Control7=IDC_LIST_BLOCK,listbox,1352728835
Control8=IDC_STATIC_BLOCK_LIST,static,1342308352

[CLS:CBlockList]
Type=0
HeaderFile=BlockList.h
ImplementationFile=BlockList.cpp
BaseClass=CDialog
Filter=D
LastObject=ID_CHAT_CLEAR_CHAT_WINDOW
VirtualFilter=dWC

[DLG:IDD_AUTO_LOG_TYPE]
Type=1
Class=CAutoLog
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC_OPTS,button,1342177287
Control4=IDC_RADIO_TEXT,button,1342308361
Control5=IDC_RADIO_HTML,button,1342177289
Control6=IDC_RADIO_XML,button,1342177289
Control7=IDC_CHECK_HOUR,button,1342242819

[CLS:CAutoLog]
Type=0
HeaderFile=AutoLog.h
ImplementationFile=AutoLog.cpp
BaseClass=CDialog
Filter=D
LastObject=CAutoLog
VirtualFilter=dWC

[DLG:IDD_URL_GRABBER]
Type=1
Class=CUrlGrabber
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC_URLS,static,1342308352
Control4=IDC_LIST_URLS,listbox,1352728835

[CLS:CUrlGrabber]
Type=0
HeaderFile=UrlGrabber.h
ImplementationFile=UrlGrabber.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CUrlGrabber

[DLG:IDD_SEARCH]
Type=1
Class=CSearch
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT_SEARCH_TEXT,edit,1350631552
Control4=IDC_STATIC_SEARCH_TEXT,static,1342308352
Control5=IDC_EDIT_SEARCH_SNAP,RICHEDIT,1352730692

[CLS:CSearch]
Type=0
HeaderFile=Search.h
ImplementationFile=Search.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_EDIT_SEARCH_SNAP

[DLG:IDD_NAME_SLIDE]
Type=1
Class=CNameSlide
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC_NAME,static,1342308352
Control4=IDC_EDIT_NAME_SLIDE,edit,1350631552

[CLS:CNameSlide]
Type=0
HeaderFile=NameSlide.h
ImplementationFile=NameSlide.cpp
BaseClass=CDialog
Filter=D
LastObject=CNameSlide
VirtualFilter=dWC

[DLG:IDD_BUG_REPORT]
Type=1
Class=?
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC_DESCRIPTION,static,1342308352
Control4=IDC_EDIT_BUG,edit,1352728708
Control5=IDC_LIST_CONFIG,listbox,1352728835

[ACL:IDR_ACCELERATOR_MAGENTA]
Type=1
Class=?
Command1=ID_OPACITY_100
Command2=ID_OPACITY_10
Command3=ID_OPACITY_20
Command4=ID_OPACITY_30
Command5=ID_OPACITY_40
Command6=ID_OPACITY_50
Command7=ID_OPACITY_60
Command8=ID_OPACITY_70
Command9=ID_OPACITY_80
Command10=ID_OPACITY_90
Command11=ID_BLOCK_LIST
Command12=ID_CHAT_CONNECT
Command13=ID_CHAT_DISCONNECT
Command14=ID_CLEAR_LOG
Command15=ID_TOOLS_FONT
Command16=ID_CHAT_SHOW_IGNORED
Command17=IDC_IGNORE
Command18=ID_LOGGING
Command19=ID_CHAT_CLEAR_CHAT_WINDOW
Command20=ID_TOOLS_OPTIONS
Command21=IDC_PRIVATE_CHAT
Command22=ID_TOOLS_SEND_RAW_COMMAND
Command23=ID_SAVE_FULL_LOG
Command24=IDC_SEND_PRIVATE
Command25=ID_CHAT_SAVE_LOG
Command26=IDC_CHAT
Command27=ID_HELP_TOPICS
Command28=ID_CHAT_SWITCH_NAME
CommandCount=28

[DLG:IDD_ANNOTATION]
Type=1
Class=CAnnotation
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC_CURRENT_ANN,static,1342308352
Control4=IDC_EDIT_OLD_ANN,edit,1342244992
Control5=IDC_STATIC_NEW_ANN,static,1342308352
Control6=IDC_EDIT_NEW_ANN,edit,1350631552

[CLS:CAnnotation]
Type=0
HeaderFile=Annotation.h
ImplementationFile=Annotation.cpp
BaseClass=CDialog
Filter=D
LastObject=CAnnotation
VirtualFilter=dWC

