# Microsoft Developer Studio Generated NMAKE File, Based on Magenta.dsp
!IF "$(CFG)" == ""
CFG=Magenta - Win32 Debug
!MESSAGE No configuration specified. Defaulting to Magenta - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "Magenta - Win32 Release" && "$(CFG)" != "Magenta - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
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
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Magenta - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\Magenta.exe" "$(OUTDIR)\Magenta.bsc"


CLEAN :
	-@erase "$(INTDIR)\AutoLog.obj"
	-@erase "$(INTDIR)\AutoLog.sbr"
	-@erase "$(INTDIR)\BlockList.obj"
	-@erase "$(INTDIR)\BlockList.sbr"
	-@erase "$(INTDIR)\ChatConsole.obj"
	-@erase "$(INTDIR)\ChatConsole.sbr"
	-@erase "$(INTDIR)\ChatSocket.obj"
	-@erase "$(INTDIR)\ChatSocket.sbr"
	-@erase "$(INTDIR)\ChatWindow.obj"
	-@erase "$(INTDIR)\ChatWindow.sbr"
	-@erase "$(INTDIR)\dlgbars.obj"
	-@erase "$(INTDIR)\dlgbars.sbr"
	-@erase "$(INTDIR)\Ignored.obj"
	-@erase "$(INTDIR)\Ignored.sbr"
	-@erase "$(INTDIR)\InputBox.obj"
	-@erase "$(INTDIR)\InputBox.sbr"
	-@erase "$(INTDIR)\Log.obj"
	-@erase "$(INTDIR)\Log.sbr"
	-@erase "$(INTDIR)\Magenta.obj"
	-@erase "$(INTDIR)\Magenta.pch"
	-@erase "$(INTDIR)\Magenta.res"
	-@erase "$(INTDIR)\Magenta.sbr"
	-@erase "$(INTDIR)\mdlsmain.obj"
	-@erase "$(INTDIR)\mdlsmain.sbr"
	-@erase "$(INTDIR)\modeless.obj"
	-@erase "$(INTDIR)\modeless.sbr"
	-@erase "$(INTDIR)\NewEditBox.obj"
	-@erase "$(INTDIR)\NewEditBox.sbr"
	-@erase "$(INTDIR)\NewListBox.obj"
	-@erase "$(INTDIR)\NewListBox.sbr"
	-@erase "$(INTDIR)\Options.obj"
	-@erase "$(INTDIR)\Options.sbr"
	-@erase "$(INTDIR)\Person.obj"
	-@erase "$(INTDIR)\Person.sbr"
	-@erase "$(INTDIR)\PrivateChat.obj"
	-@erase "$(INTDIR)\PrivateChat.sbr"
	-@erase "$(INTDIR)\Search.obj"
	-@erase "$(INTDIR)\Search.sbr"
	-@erase "$(INTDIR)\StatusWindow.obj"
	-@erase "$(INTDIR)\StatusWindow.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\Tokenizer.obj"
	-@erase "$(INTDIR)\Tokenizer.sbr"
	-@erase "$(INTDIR)\UrlGrabber.obj"
	-@erase "$(INTDIR)\UrlGrabber.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\Magenta.bsc"
	-@erase "$(OUTDIR)\Magenta.exe"
	-@erase "$(OUTDIR)\Magenta.map"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "ISOLATION_AWARE_ENABLED" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Magenta.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Magenta.res" /d "NDEBUG" /d "ISOLATION_AWARE_ENABLED" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Magenta.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AutoLog.sbr" \
	"$(INTDIR)\BlockList.sbr" \
	"$(INTDIR)\ChatConsole.sbr" \
	"$(INTDIR)\ChatSocket.sbr" \
	"$(INTDIR)\ChatWindow.sbr" \
	"$(INTDIR)\dlgbars.sbr" \
	"$(INTDIR)\Ignored.sbr" \
	"$(INTDIR)\InputBox.sbr" \
	"$(INTDIR)\Log.sbr" \
	"$(INTDIR)\Magenta.sbr" \
	"$(INTDIR)\mdlsmain.sbr" \
	"$(INTDIR)\modeless.sbr" \
	"$(INTDIR)\NewEditBox.sbr" \
	"$(INTDIR)\NewListBox.sbr" \
	"$(INTDIR)\Options.sbr" \
	"$(INTDIR)\Person.sbr" \
	"$(INTDIR)\PrivateChat.sbr" \
	"$(INTDIR)\Search.sbr" \
	"$(INTDIR)\StatusWindow.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\Tokenizer.sbr" \
	"$(INTDIR)\UrlGrabber.sbr"

"$(OUTDIR)\Magenta.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=version.lib htmlhelp.lib /nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\Magenta.pdb" /map:"$(INTDIR)\Magenta.map" /machine:I386 /out:"$(OUTDIR)\Magenta.exe" 
LINK32_OBJS= \
	"$(INTDIR)\AutoLog.obj" \
	"$(INTDIR)\BlockList.obj" \
	"$(INTDIR)\ChatConsole.obj" \
	"$(INTDIR)\ChatSocket.obj" \
	"$(INTDIR)\ChatWindow.obj" \
	"$(INTDIR)\dlgbars.obj" \
	"$(INTDIR)\Ignored.obj" \
	"$(INTDIR)\InputBox.obj" \
	"$(INTDIR)\Log.obj" \
	"$(INTDIR)\Magenta.obj" \
	"$(INTDIR)\mdlsmain.obj" \
	"$(INTDIR)\modeless.obj" \
	"$(INTDIR)\NewEditBox.obj" \
	"$(INTDIR)\NewListBox.obj" \
	"$(INTDIR)\Options.obj" \
	"$(INTDIR)\Person.obj" \
	"$(INTDIR)\PrivateChat.obj" \
	"$(INTDIR)\Search.obj" \
	"$(INTDIR)\StatusWindow.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Tokenizer.obj" \
	"$(INTDIR)\UrlGrabber.obj" \
	"$(INTDIR)\Magenta.res"

"$(OUTDIR)\Magenta.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "Magenta - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\Magenta.exe" "$(OUTDIR)\Magenta.bsc"


CLEAN :
	-@erase "$(INTDIR)\AutoLog.obj"
	-@erase "$(INTDIR)\AutoLog.sbr"
	-@erase "$(INTDIR)\BlockList.obj"
	-@erase "$(INTDIR)\BlockList.sbr"
	-@erase "$(INTDIR)\ChatConsole.obj"
	-@erase "$(INTDIR)\ChatConsole.sbr"
	-@erase "$(INTDIR)\ChatSocket.obj"
	-@erase "$(INTDIR)\ChatSocket.sbr"
	-@erase "$(INTDIR)\ChatWindow.obj"
	-@erase "$(INTDIR)\ChatWindow.sbr"
	-@erase "$(INTDIR)\dlgbars.obj"
	-@erase "$(INTDIR)\dlgbars.sbr"
	-@erase "$(INTDIR)\Ignored.obj"
	-@erase "$(INTDIR)\Ignored.sbr"
	-@erase "$(INTDIR)\InputBox.obj"
	-@erase "$(INTDIR)\InputBox.sbr"
	-@erase "$(INTDIR)\Log.obj"
	-@erase "$(INTDIR)\Log.sbr"
	-@erase "$(INTDIR)\Magenta.obj"
	-@erase "$(INTDIR)\Magenta.pch"
	-@erase "$(INTDIR)\Magenta.res"
	-@erase "$(INTDIR)\Magenta.sbr"
	-@erase "$(INTDIR)\mdlsmain.obj"
	-@erase "$(INTDIR)\mdlsmain.sbr"
	-@erase "$(INTDIR)\modeless.obj"
	-@erase "$(INTDIR)\modeless.sbr"
	-@erase "$(INTDIR)\NewEditBox.obj"
	-@erase "$(INTDIR)\NewEditBox.sbr"
	-@erase "$(INTDIR)\NewListBox.obj"
	-@erase "$(INTDIR)\NewListBox.sbr"
	-@erase "$(INTDIR)\Options.obj"
	-@erase "$(INTDIR)\Options.sbr"
	-@erase "$(INTDIR)\Person.obj"
	-@erase "$(INTDIR)\Person.sbr"
	-@erase "$(INTDIR)\PrivateChat.obj"
	-@erase "$(INTDIR)\PrivateChat.sbr"
	-@erase "$(INTDIR)\Search.obj"
	-@erase "$(INTDIR)\Search.sbr"
	-@erase "$(INTDIR)\StatusWindow.obj"
	-@erase "$(INTDIR)\StatusWindow.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\Tokenizer.obj"
	-@erase "$(INTDIR)\Tokenizer.sbr"
	-@erase "$(INTDIR)\UrlGrabber.obj"
	-@erase "$(INTDIR)\UrlGrabber.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\Magenta.bsc"
	-@erase "$(OUTDIR)\Magenta.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Magenta.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x409 /fo"$(INTDIR)\Magenta.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\Magenta.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\AutoLog.sbr" \
	"$(INTDIR)\BlockList.sbr" \
	"$(INTDIR)\ChatConsole.sbr" \
	"$(INTDIR)\ChatSocket.sbr" \
	"$(INTDIR)\ChatWindow.sbr" \
	"$(INTDIR)\dlgbars.sbr" \
	"$(INTDIR)\Ignored.sbr" \
	"$(INTDIR)\InputBox.sbr" \
	"$(INTDIR)\Log.sbr" \
	"$(INTDIR)\Magenta.sbr" \
	"$(INTDIR)\mdlsmain.sbr" \
	"$(INTDIR)\modeless.sbr" \
	"$(INTDIR)\NewEditBox.sbr" \
	"$(INTDIR)\NewListBox.sbr" \
	"$(INTDIR)\Options.sbr" \
	"$(INTDIR)\Person.sbr" \
	"$(INTDIR)\PrivateChat.sbr" \
	"$(INTDIR)\Search.sbr" \
	"$(INTDIR)\StatusWindow.sbr" \
	"$(INTDIR)\StdAfx.sbr" \
	"$(INTDIR)\Tokenizer.sbr" \
	"$(INTDIR)\UrlGrabber.sbr"

"$(OUTDIR)\Magenta.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=version.lib htmlhelp.lib /nologo /subsystem:windows /profile /debug /machine:I386 /out:"$(OUTDIR)\Magenta.exe" 
LINK32_OBJS= \
	"$(INTDIR)\AutoLog.obj" \
	"$(INTDIR)\BlockList.obj" \
	"$(INTDIR)\ChatConsole.obj" \
	"$(INTDIR)\ChatSocket.obj" \
	"$(INTDIR)\ChatWindow.obj" \
	"$(INTDIR)\dlgbars.obj" \
	"$(INTDIR)\Ignored.obj" \
	"$(INTDIR)\InputBox.obj" \
	"$(INTDIR)\Log.obj" \
	"$(INTDIR)\Magenta.obj" \
	"$(INTDIR)\mdlsmain.obj" \
	"$(INTDIR)\modeless.obj" \
	"$(INTDIR)\NewEditBox.obj" \
	"$(INTDIR)\NewListBox.obj" \
	"$(INTDIR)\Options.obj" \
	"$(INTDIR)\Person.obj" \
	"$(INTDIR)\PrivateChat.obj" \
	"$(INTDIR)\Search.obj" \
	"$(INTDIR)\StatusWindow.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\Tokenizer.obj" \
	"$(INTDIR)\UrlGrabber.obj" \
	"$(INTDIR)\Magenta.res"

"$(OUTDIR)\Magenta.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("Magenta.dep")
!INCLUDE "Magenta.dep"
!ELSE 
!MESSAGE Warning: cannot find "Magenta.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "Magenta - Win32 Release" || "$(CFG)" == "Magenta - Win32 Debug"
SOURCE=.\AutoLog.cpp

"$(INTDIR)\AutoLog.obj"	"$(INTDIR)\AutoLog.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Magenta.pch"


SOURCE=.\BlockList.cpp

"$(INTDIR)\BlockList.obj"	"$(INTDIR)\BlockList.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Magenta.pch"


SOURCE=.\ChatConsole.cpp

"$(INTDIR)\ChatConsole.obj"	"$(INTDIR)\ChatConsole.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Magenta.pch"


SOURCE=.\ChatSocket.cpp

"$(INTDIR)\ChatSocket.obj"	"$(INTDIR)\ChatSocket.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Magenta.pch"


SOURCE=.\ChatWindow.cpp

"$(INTDIR)\ChatWindow.obj"	"$(INTDIR)\ChatWindow.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Magenta.pch"


SOURCE=.\dlgbars.cpp

"$(INTDIR)\dlgbars.obj"	"$(INTDIR)\dlgbars.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Magenta.pch"


SOURCE=.\Ignored.cpp

"$(INTDIR)\Ignored.obj"	"$(INTDIR)\Ignored.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Magenta.pch"


SOURCE=.\InputBox.cpp

"$(INTDIR)\InputBox.obj"	"$(INTDIR)\InputBox.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Magenta.pch"


SOURCE=.\Log.cpp

"$(INTDIR)\Log.obj"	"$(INTDIR)\Log.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Magenta.pch"


SOURCE=.\Magenta.cpp

"$(INTDIR)\Magenta.obj"	"$(INTDIR)\Magenta.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Magenta.pch"


SOURCE=.\Magenta.rc

"$(INTDIR)\Magenta.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\mdlsmain.cpp

"$(INTDIR)\mdlsmain.obj"	"$(INTDIR)\mdlsmain.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Magenta.pch"


SOURCE=.\modeless.cpp

"$(INTDIR)\modeless.obj"	"$(INTDIR)\modeless.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Magenta.pch"


SOURCE=.\NewEditBox.cpp

"$(INTDIR)\NewEditBox.obj"	"$(INTDIR)\NewEditBox.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Magenta.pch"


SOURCE=.\NewListBox.cpp

"$(INTDIR)\NewListBox.obj"	"$(INTDIR)\NewListBox.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Magenta.pch"


SOURCE=.\Options.cpp

"$(INTDIR)\Options.obj"	"$(INTDIR)\Options.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Magenta.pch"


SOURCE=.\Person.cpp

"$(INTDIR)\Person.obj"	"$(INTDIR)\Person.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Magenta.pch"


SOURCE=.\PrivateChat.cpp

"$(INTDIR)\PrivateChat.obj"	"$(INTDIR)\PrivateChat.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Magenta.pch"


SOURCE=.\Search.cpp

"$(INTDIR)\Search.obj"	"$(INTDIR)\Search.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Magenta.pch"


SOURCE=.\StatusWindow.cpp

"$(INTDIR)\StatusWindow.obj"	"$(INTDIR)\StatusWindow.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Magenta.pch"


SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "Magenta - Win32 Release"

CPP_SWITCHES=/nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "ISOLATION_AWARE_ENABLED" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\Magenta.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\Magenta.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "Magenta - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\Magenta.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\Magenta.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\Tokenizer.cpp

"$(INTDIR)\Tokenizer.obj"	"$(INTDIR)\Tokenizer.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Magenta.pch"


SOURCE=.\UrlGrabber.cpp

"$(INTDIR)\UrlGrabber.obj"	"$(INTDIR)\UrlGrabber.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\Magenta.pch"



!ENDIF 

