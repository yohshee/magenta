# Microsoft Developer Studio Project File - Name="Magenta" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Magenta - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Magenta.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Magenta.mak" CFG="Magenta - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Magenta - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Magenta - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Magenta - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Obj\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "ISOLATION_AWARE_ENABLED" /Fr /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "ISOLATION_AWARE_ENABLED"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"Obj/Release/Magenta.bsc"
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 version.lib htmlhelp.lib /nologo /subsystem:windows /map /machine:I386
# SUBTRACT LINK32 /verbose /debug

!ELSEIF  "$(CFG)" == "Magenta - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Obj\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo /o"Obj/Debug/Magenta.bsc"
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 version.lib htmlhelp.lib /nologo /subsystem:windows /profile /debug /machine:I386

!ENDIF 

# Begin Target

# Name "Magenta - Win32 Release"
# Name "Magenta - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Annotation.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoLog.cpp
# End Source File
# Begin Source File

SOURCE=.\BlockList.cpp
# End Source File
# Begin Source File

SOURCE=.\ChatConsole.cpp
# End Source File
# Begin Source File

SOURCE=.\ChatSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\ChatWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\dlgbars.cpp
# End Source File
# Begin Source File

SOURCE=.\Ignored.cpp
# End Source File
# Begin Source File

SOURCE=.\InputBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Log.cpp
# End Source File
# Begin Source File

SOURCE=.\Magenta.cpp
# End Source File
# Begin Source File

SOURCE=.\Magenta.rc
# End Source File
# Begin Source File

SOURCE=.\mdlsmain.cpp
# End Source File
# Begin Source File

SOURCE=.\modeless.cpp
# End Source File
# Begin Source File

SOURCE=.\NameSlide.cpp
# End Source File
# Begin Source File

SOURCE=.\NewEditBox.cpp
# End Source File
# Begin Source File

SOURCE=.\NewListBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Options.cpp
# End Source File
# Begin Source File

SOURCE=.\Person.cpp
# End Source File
# Begin Source File

SOURCE=.\PrivateChat.cpp
# End Source File
# Begin Source File

SOURCE=.\Search.cpp
# End Source File
# Begin Source File

SOURCE=.\StatusWindow.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Tokenizer.cpp
# End Source File
# Begin Source File

SOURCE=.\UrlGrabber.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Annotation.h
# End Source File
# Begin Source File

SOURCE=.\AutoLog.h
# End Source File
# Begin Source File

SOURCE=.\BlockList.h
# End Source File
# Begin Source File

SOURCE=.\ChatConsole.h
# End Source File
# Begin Source File

SOURCE=.\ChatSocket.h
# End Source File
# Begin Source File

SOURCE=.\ChatWindow.h
# End Source File
# Begin Source File

SOURCE=.\Constants.h
# End Source File
# Begin Source File

SOURCE=.\dlgbars.h
# End Source File
# Begin Source File

SOURCE=.\Ignored.h
# End Source File
# Begin Source File

SOURCE=.\InputBox.h
# End Source File
# Begin Source File

SOURCE=.\Log.h
# End Source File
# Begin Source File

SOURCE=.\Magenta.h
# End Source File
# Begin Source File

SOURCE=.\mdlsmain.h
# End Source File
# Begin Source File

SOURCE=.\modeless.h
# End Source File
# Begin Source File

SOURCE=.\NameSlide.h
# End Source File
# Begin Source File

SOURCE=.\NewEditBox.h
# End Source File
# Begin Source File

SOURCE=.\NewListBox.h
# End Source File
# Begin Source File

SOURCE=.\Options.h
# End Source File
# Begin Source File

SOURCE=.\Person.h
# End Source File
# Begin Source File

SOURCE=.\PrivateChat.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Search.h
# End Source File
# Begin Source File

SOURCE=.\StatusWindow.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Tokenizer.h
# End Source File
# Begin Source File

SOURCE=.\UrlGrabber.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Cyan.ico
# End Source File
# Begin Source File

SOURCE=.\res\Guest.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_mage.ico
# End Source File
# Begin Source File

SOURCE=.\Magenta.exe.manifest
# End Source File
# Begin Source File

SOURCE=.\res\Magenta.ico
# End Source File
# Begin Source File

SOURCE=.\res\Magenta.rc2
# End Source File
# Begin Source File

SOURCE=.\res\Normal.ico
# End Source File
# Begin Source File

SOURCE=.\res\setup.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=".\readme-src.txt"
# End Source File
# End Target
# End Project
# Section Magenta : {72ADFD7E-2C39-11D0-9903-00A0C91BC942}
# 	1:17:CG_IDS_DISK_SPACE:105
# 	1:19:CG_IDS_PHYSICAL_MEM:104
# 	1:25:CG_IDS_DISK_SPACE_UNAVAIL:106
# 	2:14:PhysicalMemory:CG_IDS_PHYSICAL_MEM
# 	2:9:DiskSpace:CG_IDS_DISK_SPACE
# 	2:16:SpaceUnavailable:CG_IDS_DISK_SPACE_UNAVAIL
# 	2:7:NewFunc:1
# 	2:10:SysInfoKey:1234
# End Section
