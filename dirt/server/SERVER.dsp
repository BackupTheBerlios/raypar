# Microsoft Developer Studio Project File - Name="SERVER" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SERVER - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SERVER.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SERVER.mak" CFG="SERVER - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SERVER - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SERVER - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/SERVER", WAAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SERVER - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I ".." /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "YY_NEVER_INTERACTIVE" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 /nologo /version:1.0 /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "SERVER - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\BUILT"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GR /GX /ZI /Od /I ".." /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "YY_NEVER_INTERACTIVE" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /version:1.0 /subsystem:windows /map /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "SERVER - Win32 Release"
# Name "SERVER - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ChildView.cpp
# End Source File
# Begin Source File

SOURCE=..\client\environment.cpp
# End Source File
# Begin Source File

SOURCE=..\client\geometry.cpp
# End Source File
# Begin Source File

SOURCE=.\LexParser.cpp
# End Source File
# Begin Source File

SOURCE=..\common\LogBox.cpp
# End Source File
# Begin Source File

SOURCE=.\LogWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=..\COMMON\msg.cpp
# End Source File
# Begin Source File

SOURCE=.\Options.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionsDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\common\ParserVar.cpp
# End Source File
# Begin Source File

SOURCE=..\common\protocol.cpp

!IF  "$(CFG)" == "SERVER - Win32 Release"

!ELSEIF  "$(CFG)" == "SERVER - Win32 Debug"

# ADD CPP /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ray_lex.l.cpp

!IF  "$(CFG)" == "SERVER - Win32 Release"

!ELSEIF  "$(CFG)" == "SERVER - Win32 Debug"

# SUBTRACT CPP /YX /Yc /Yu

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\common\SceneArray.cpp
# End Source File
# Begin Source File

SOURCE=.\SERVER.cpp
# End Source File
# Begin Source File

SOURCE=.\SERVER.rc
# End Source File
# Begin Source File

SOURCE=.\ServerSceneBuilder.cpp
# End Source File
# Begin Source File

SOURCE=.\ServerThread.cpp
# End Source File
# Begin Source File

SOURCE=.\SrvCmd.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\common\utils.cpp
# End Source File
# Begin Source File

SOURCE=..\common\Vector.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ChildView.h
# End Source File
# Begin Source File

SOURCE=..\CLIENT\environment.h
# End Source File
# Begin Source File

SOURCE=..\CLIENT\geometry.h
# End Source File
# Begin Source File

SOURCE=.\LexParser.cpp.h
# End Source File
# Begin Source File

SOURCE=..\COMMON\LogBox.h
# End Source File
# Begin Source File

SOURCE=.\LogWnd.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=..\COMMON\msg.h
# End Source File
# Begin Source File

SOURCE=.\Options.h
# End Source File
# Begin Source File

SOURCE=.\OptionsDialog.h
# End Source File
# Begin Source File

SOURCE=..\common\ParserVar.h
# End Source File
# Begin Source File

SOURCE=..\common\protocol.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=..\common\SceneArray.h
# End Source File
# Begin Source File

SOURCE=.\SERVER.h
# End Source File
# Begin Source File

SOURCE=.\ServerSceneBuilder.h
# End Source File
# Begin Source File

SOURCE=.\ServerThread.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\common\utils.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\SERVER.ico
# End Source File
# Begin Source File

SOURCE=.\res\SERVER.rc2
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar2.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ray_lex.l

!IF  "$(CFG)" == "SERVER - Win32 Release"

!ELSEIF  "$(CFG)" == "SERVER - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__RAY_L="ray_lex.l.cpp"	
# Begin Custom Build - Building Lexer
InputPath=.\ray_lex.l
InputName=ray_lex

"$(InputName).l.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	flex.exe -Pyy -i -t $(InputName).l > $(InputName).l.cpp

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ray_lex.y

!IF  "$(CFG)" == "SERVER - Win32 Release"

!ELSEIF  "$(CFG)" == "SERVER - Win32 Debug"

# PROP Ignore_Default_Tool 1
USERDEP__RAY_LE="LexParser.cpp"	
# Begin Custom Build - Building Parser
InputPath=.\ray_lex.y
InputName=ray_lex

"$(InputName).cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	bison.exe -pyy -o LexParser.cpp -d $(InputName).y

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\DOCS\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\scene.sc

!IF  "$(CFG)" == "SERVER - Win32 Release"

!ELSEIF  "$(CFG)" == "SERVER - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ServerReadMe.txt

!IF  "$(CFG)" == "SERVER - Win32 Release"

!ELSEIF  "$(CFG)" == "SERVER - Win32 Debug"

# PROP Exclude_From_Build 1

!ENDIF 

# End Source File
# End Target
# End Project
