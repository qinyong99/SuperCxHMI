# Microsoft Developer Studio Project File - Name="CxRunCtrl" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CxRunCtrl - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CxRunCtrl.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CxRunCtrl.mak" CFG="CxRunCtrl - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CxRunCtrl - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CxRunCtrl - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CxRunCtrl - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CxRunCtrl - Win32 Unicode Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CxRunCtrl - Win32 Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Ext "ocx"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Ext "ocx"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\CxOle" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "L_IMPL_OBJECTSAFETY" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 Gdiplus.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../../bin/CxRunCtrl.ocx"
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\Release
TargetPath=\SuperCx_HMI\bin\CxRunCtrl.ocx
InputPath=\SuperCx_HMI\bin\CxRunCtrl.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CxRunCtrl - Win32 Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Ext "ocx"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Ext "ocx"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\CxOle" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /D "L_IMPL_OBJECTSAFETY" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 Gdiplus.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../../bin/CxRunCtrl.ocx" /pdbtype:sept
# SUBTRACT LINK32 /incremental:no
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\Debug
TargetPath=\SuperCx_HMI\bin\CxRunCtrl.ocx
InputPath=\SuperCx_HMI\bin\CxRunCtrl.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CxRunCtrl - Win32 Unicode Debug"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugU"
# PROP BASE Intermediate_Dir "DebugU"
# PROP BASE Target_Ext "ocx"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugU"
# PROP Intermediate_Dir "DebugU"
# PROP Target_Ext "ocx"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_UNICODE" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\DebugU
TargetPath=.\DebugU\CxRunCtrl.ocx
InputPath=.\DebugU\CxRunCtrl.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CxRunCtrl - Win32 Unicode Release"

# PROP BASE Use_MFC 2
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseU"
# PROP BASE Intermediate_Dir "ReleaseU"
# PROP BASE Target_Ext "ocx"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseU"
# PROP Intermediate_Dir "ReleaseU"
# PROP Target_Ext "ocx"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_USRDLL" /D "_UNICODE" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /machine:I386
# Begin Custom Build - Registering ActiveX Control...
OutDir=.\ReleaseU
TargetPath=.\ReleaseU\CxRunCtrl.ocx
InputPath=.\ReleaseU\CxRunCtrl.ocx
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "CxRunCtrl - Win32 Release"
# Name "CxRunCtrl - Win32 Debug"
# Name "CxRunCtrl - Win32 Unicode Debug"
# Name "CxRunCtrl - Win32 Unicode Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ActivDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\AmbientProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\BitmapDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\BitmapView.cpp
# End Source File
# Begin Source File

SOURCE=.\CacheFile.cpp
# End Source File
# Begin Source File

SOURCE=.\CacheFileManager.cpp
# End Source File
# Begin Source File

SOURCE=.\CeXDib.cpp
# End Source File
# Begin Source File

SOURCE=.\ChangePasswordDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CheckListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\CtrlItem.cpp
# End Source File
# Begin Source File

SOURCE=.\CtrlObj.cpp
# End Source File
# Begin Source File

SOURCE=.\CxRunCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\CxRunCtrl.def
# End Source File
# Begin Source File

SOURCE=.\CxRunCtrl.odl

!IF  "$(CFG)" == "CxRunCtrl - Win32 Release"

# ADD MTL /tlb "CxRunCtrl.tlb" /h "CxRunCtrlIF.H" /iid "CxRunCtrl_i.c"

!ELSEIF  "$(CFG)" == "CxRunCtrl - Win32 Debug"

# ADD MTL /h "CxRunCtrlIF.H" /iid "CxRunCtrl_i.c"

!ELSEIF  "$(CFG)" == "CxRunCtrl - Win32 Unicode Debug"

!ELSEIF  "$(CFG)" == "CxRunCtrl - Win32 Unicode Release"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CxRunCtrl.rc
# End Source File
# Begin Source File

SOURCE=.\DevDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\DevObjDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawObj.cpp
# End Source File
# Begin Source File

SOURCE=.\EnumHelper.cpp
# End Source File
# Begin Source File

SOURCE=.\EnumVariant.cpp
# End Source File
# Begin Source File

SOURCE=.\ExtendedControl.cpp
# End Source File
# Begin Source File

SOURCE=.\FormDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\FormObj.cpp
# End Source File
# Begin Source File

SOURCE=.\FormScrEng.cpp
# End Source File
# Begin Source File

SOURCE=.\GroupObj.cpp
# End Source File
# Begin Source File

SOURCE=.\KeyBoardDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LoginDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ObjectCollection.cpp
# End Source File
# Begin Source File

SOURCE=.\PopupFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\PPTooltip.cpp
# End Source File
# Begin Source File

SOURCE=.\PreviewViewEx.cpp
# End Source File
# Begin Source File

SOURCE=.\PrintFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\PrintView.cpp
# End Source File
# Begin Source File

SOURCE=.\RectF.cpp
# End Source File
# Begin Source File

SOURCE=.\ReportDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ReportObj.cpp
# End Source File
# Begin Source File

SOURCE=.\ReportSection.cpp
# End Source File
# Begin Source File

SOURCE=.\ReportView.cpp
# End Source File
# Begin Source File

SOURCE=.\ReturnValue.cpp
# End Source File
# Begin Source File

SOURCE=.\RunCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\RunCtrlPpg.cpp
# End Source File
# Begin Source File

SOURCE=.\RunFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\RunObj.cpp
# End Source File
# Begin Source File

SOURCE=.\RunView.cpp
# End Source File
# Begin Source File

SOURCE=.\ScriptEngine.cpp
# End Source File
# Begin Source File

SOURCE=.\ScriptErrorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SymbolObj.cpp
# End Source File
# Begin Source File

SOURCE=.\UserItemDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UserManagerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Varutils.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ActivDoc.h
# End Source File
# Begin Source File

SOURCE=.\AmbientProperty.h
# End Source File
# Begin Source File

SOURCE=.\BitmapDoc.h
# End Source File
# Begin Source File

SOURCE=.\BitmapView.h
# End Source File
# Begin Source File

SOURCE=.\CacheFile.h
# End Source File
# Begin Source File

SOURCE=.\CacheFileManager.h
# End Source File
# Begin Source File

SOURCE=.\CeXDib.h
# End Source File
# Begin Source File

SOURCE=.\ChangePasswordDlg.h
# End Source File
# Begin Source File

SOURCE=.\CheckListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ChineseVarUtils.h
# End Source File
# Begin Source File

SOURCE=.\CtrlItem.h
# End Source File
# Begin Source File

SOURCE=.\CtrlObj.h
# End Source File
# Begin Source File

SOURCE=.\CxRunCtrl.h
# End Source File
# Begin Source File

SOURCE=.\DevDoc.h
# End Source File
# Begin Source File

SOURCE=.\DevObjDoc.h
# End Source File
# Begin Source File

SOURCE=.\DrawObj.h
# End Source File
# Begin Source File

SOURCE=.\EnumHelper.h
# End Source File
# Begin Source File

SOURCE=.\EnumVariant.h
# End Source File
# Begin Source File

SOURCE=.\ExtendedControl.h
# End Source File
# Begin Source File

SOURCE=.\FileLog.h
# End Source File
# Begin Source File

SOURCE=.\FormDoc.h
# End Source File
# Begin Source File

SOURCE=.\FormObj.h
# End Source File
# Begin Source File

SOURCE=.\FormScrEng.h
# End Source File
# Begin Source File

SOURCE=.\GroupObj.h
# End Source File
# Begin Source File

SOURCE=.\HELPERS.H
# End Source File
# Begin Source File

SOURCE=.\KeyBoardDlg.h
# End Source File
# Begin Source File

SOURCE=.\LoginDlg.h
# End Source File
# Begin Source File

SOURCE=.\ObjectCollection.h
# End Source File
# Begin Source File

SOURCE=.\PopupFrame.h
# End Source File
# Begin Source File

SOURCE=.\PPTooltip.h
# End Source File
# Begin Source File

SOURCE=.\PreviewViewEx.h
# End Source File
# Begin Source File

SOURCE=.\PrintFrame.h
# End Source File
# Begin Source File

SOURCE=.\PrintView.h
# End Source File
# Begin Source File

SOURCE=.\RectF.h
# End Source File
# Begin Source File

SOURCE=.\ReportDoc.h
# End Source File
# Begin Source File

SOURCE=.\ReportObj.h
# End Source File
# Begin Source File

SOURCE=.\ReportSection.h
# End Source File
# Begin Source File

SOURCE=.\ReportView.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ReturnValue.h
# End Source File
# Begin Source File

SOURCE=.\RunCtrl.h
# End Source File
# Begin Source File

SOURCE=.\RunCtrlPpg.h
# End Source File
# Begin Source File

SOURCE=.\RunFrame.h
# End Source File
# Begin Source File

SOURCE=.\RunInc.h
# End Source File
# Begin Source File

SOURCE=.\RunObj.h
# End Source File
# Begin Source File

SOURCE=.\RunView.h
# End Source File
# Begin Source File

SOURCE=.\ScriptEngine.h
# End Source File
# Begin Source File

SOURCE=.\ScriptErrorDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SymbolObj.h
# End Source File
# Begin Source File

SOURCE=.\UserItemDlg.h
# End Source File
# Begin Source File

SOURCE=.\UserManagerDlg.h
# End Source File
# Begin Source File

SOURCE=.\VARUTILS.H
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=".\res\Axmediabrowser 1.ico"
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\CxRunCtrl.bmp
# End Source File
# Begin Source File

SOURCE=.\res\hand1.cur
# End Source File
# Begin Source File

SOURCE=.\res\hand2.cur
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_cxru.ico
# End Source File
# Begin Source File

SOURCE=.\RunCtrl.bmp
# End Source File
# Begin Source File

SOURCE=.\res\RunDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\stateico.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
