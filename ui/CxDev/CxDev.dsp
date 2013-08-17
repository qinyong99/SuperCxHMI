# Microsoft Developer Studio Project File - Name="CxDev" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=CxDev - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CxDev.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CxDev.mak" CFG="CxDev - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CxDev - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "CxDev - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/CXDEV", BAAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CxDev - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W2 /GX /O2 /I "..\CxOle" /I "..\CxPropCtrl" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXDLL" /D "_WIN32_DCOM" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 shlwapi.lib Gdiplus.lib htmlhelp.lib /nologo /subsystem:windows /machine:I386 /out:"../../bin/CxDev.exe"
# SUBTRACT LINK32 /profile /incremental:yes /map /debug /nodefaultlib

!ELSEIF  "$(CFG)" == "CxDev - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\CxOle" /I "..\CxPropCtrl" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_WIN32_DCOM" /Yu"stdafx.h" /FD /GZ /c
# SUBTRACT CPP /Fr
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /I "..\CxOle" /D "_DEBUG" /win32
# SUBTRACT MTL /mktyplib203
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 shlwapi.lib Gdiplus.lib htmlhelp.lib /nologo /subsystem:windows /debug /machine:I386 /out:"../../bin/CxDev.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "CxDev - Win32 Release"
# Name "CxDev - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Action.cpp
# End Source File
# Begin Source File

SOURCE=.\ActionObj.cpp
# End Source File
# Begin Source File

SOURCE=.\AmbientProperty.cpp
# End Source File
# Begin Source File

SOURCE=.\AppSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\BarListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\BoxCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ClrButton.cpp
# End Source File
# Begin Source File

SOURCE=.\CodeModuleDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\CtrlItem.cpp
# End Source File
# Begin Source File

SOURCE=.\CtrlObj.cpp
# End Source File
# Begin Source File

SOURCE=.\CxDev.cpp
# End Source File
# Begin Source File

SOURCE=.\CxDev.odl

!IF  "$(CFG)" == "CxDev - Win32 Release"

!ELSEIF  "$(CFG)" == "CxDev - Win32 Debug"

# ADD MTL /h "CxDevIF.h" /iid "CxDev_i.c"
# SUBTRACT MTL /mktyplib203

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CxDev.rc
# End Source File
# Begin Source File

SOURCE=.\DevDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\DevObj.cpp
# End Source File
# Begin Source File

SOURCE=.\DevObjDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\DevTool.cpp
# End Source File
# Begin Source File

SOURCE=.\DirWatcher.cpp
# End Source File
# Begin Source File

SOURCE=.\DocManager2.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawObj.cpp
# End Source File
# Begin Source File

SOURCE=.\EnumExtendProp.cpp
# End Source File
# Begin Source File

SOURCE=.\EnumString.cpp
# End Source File
# Begin Source File

SOURCE=.\ExpertBar.cpp
# End Source File
# Begin Source File

SOURCE=.\ExplorerBar.cpp
# End Source File
# Begin Source File

SOURCE=.\ExplorerTree.cpp
# End Source File
# Begin Source File

SOURCE=.\ExtendedControl.cpp
# End Source File
# Begin Source File

SOURCE=.\FileNewDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FolderNameDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FormDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\FormObj.cpp
# End Source File
# Begin Source File

SOURCE=.\GridSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\GroupObj.cpp
# End Source File
# Begin Source File

SOURCE=.\HYPERLINK.CPP
# End Source File
# Begin Source File

SOURCE=.\InplaceEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\InsertControlDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LayerSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LayoutFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\LayoutView.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MiddleWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\ModuleDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ModuleWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\OEMSetup.cpp
# End Source File
# Begin Source File

SOURCE=.\OPCDataSource.cpp
# End Source File
# Begin Source File

SOURCE=.\OPCServerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PageGen.cpp
# End Source File
# Begin Source File

SOURCE=.\PageMainForm.cpp
# End Source File
# Begin Source File

SOURCE=.\PageObject.cpp
# End Source File
# Begin Source File

SOURCE=.\PageSubForm.cpp
# End Source File
# Begin Source File

SOURCE=.\PopupList.cpp
# End Source File
# Begin Source File

SOURCE=.\PropertyBar.cpp
# End Source File
# Begin Source File

SOURCE=.\RectF.cpp
# End Source File
# Begin Source File

SOURCE=.\ReportDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\ReportFieldDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ReportLayoutDlg.cpp
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

SOURCE=.\ReportWizardDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SavePromptDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ScriptAnalyzer.cpp
# End Source File
# Begin Source File

SOURCE=.\ScriptEngine.cpp
# End Source File
# Begin Source File

SOURCE=.\ScriptFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\ScriptView.cpp
# End Source File
# Begin Source File

SOURCE=.\Splash.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\SymbolDesignDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SymbolLibDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\SymbolLibWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\SymbolObj.cpp
# End Source File
# Begin Source File

SOURCE=.\SymbolPreviewWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\SymbolPropDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SymbolPropList.cpp
# End Source File
# Begin Source File

SOURCE=.\TabOrderDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ToolboxBar.cpp
# End Source File
# Begin Source File

SOURCE=.\TypeChanger.cpp
# End Source File
# Begin Source File

SOURCE=.\TypeInfoBrowse.cpp
# End Source File
# Begin Source File

SOURCE=.\UndoStack.cpp
# End Source File
# Begin Source File

SOURCE=.\Varutils.cpp
# End Source File
# Begin Source File

SOURCE=.\ZoomDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Action.h
# End Source File
# Begin Source File

SOURCE=.\ActionObj.h
# End Source File
# Begin Source File

SOURCE=.\AmbientProperty.h
# End Source File
# Begin Source File

SOURCE=.\AppSetting.h
# End Source File
# Begin Source File

SOURCE=.\BarListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\BoxCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ChineseVarUtils.h
# End Source File
# Begin Source File

SOURCE=.\ClrButton.h
# End Source File
# Begin Source File

SOURCE=.\CodeModuleDoc.h
# End Source File
# Begin Source File

SOURCE=.\CtrlItem.h
# End Source File
# Begin Source File

SOURCE=.\CtrlObj.h
# End Source File
# Begin Source File

SOURCE=.\CxDev.h
# End Source File
# Begin Source File

SOURCE=.\DebugHeap.h
# End Source File
# Begin Source File

SOURCE=.\DevDoc.h
# End Source File
# Begin Source File

SOURCE=.\DevInc.h
# End Source File
# Begin Source File

SOURCE=.\DevObj.h
# End Source File
# Begin Source File

SOURCE=.\DevObjDoc.h
# End Source File
# Begin Source File

SOURCE=.\DevTool.h
# End Source File
# Begin Source File

SOURCE=.\DirWatcher.h
# End Source File
# Begin Source File

SOURCE=.\DocManager2.h
# End Source File
# Begin Source File

SOURCE=.\DrawObj.h
# End Source File
# Begin Source File

SOURCE=.\EnumExtendProp.h
# End Source File
# Begin Source File

SOURCE=.\EnumString.h
# End Source File
# Begin Source File

SOURCE=.\ExpertBar.h
# End Source File
# Begin Source File

SOURCE=.\ExplorerBar.h
# End Source File
# Begin Source File

SOURCE=.\ExplorerTree.h
# End Source File
# Begin Source File

SOURCE=.\ExtendedControl.h
# End Source File
# Begin Source File

SOURCE=.\FileNewDlg.h
# End Source File
# Begin Source File

SOURCE=.\FolderNameDlg.h
# End Source File
# Begin Source File

SOURCE=.\FormDoc.h
# End Source File
# Begin Source File

SOURCE=.\FormObj.h
# End Source File
# Begin Source File

SOURCE=.\GridSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\GroupObj.h
# End Source File
# Begin Source File

SOURCE=.\HYPERLINK.H
# End Source File
# Begin Source File

SOURCE=.\Ini.h
# End Source File
# Begin Source File

SOURCE=.\InplaceEdit.h
# End Source File
# Begin Source File

SOURCE=.\InsertControlDlg.h
# End Source File
# Begin Source File

SOURCE=.\LayerSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\LayoutFrm.h
# End Source File
# Begin Source File

SOURCE=.\LayoutView.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MiddleWnd.h
# End Source File
# Begin Source File

SOURCE=.\ModuleDoc.h
# End Source File
# Begin Source File

SOURCE=.\ModuleWnd.h
# End Source File
# Begin Source File

SOURCE=.\OEMSetup.h
# End Source File
# Begin Source File

SOURCE=.\OPCDataSource.h
# End Source File
# Begin Source File

SOURCE=.\OPCServerDlg.h
# End Source File
# Begin Source File

SOURCE=.\PageGen.h
# End Source File
# Begin Source File

SOURCE=.\PageMainForm.h
# End Source File
# Begin Source File

SOURCE=.\PageObject.h
# End Source File
# Begin Source File

SOURCE=.\PageSubForm.h
# End Source File
# Begin Source File

SOURCE=.\PopupList.h
# End Source File
# Begin Source File

SOURCE=.\PreviewWnd.h
# End Source File
# Begin Source File

SOURCE=.\PropertyBar.h
# End Source File
# Begin Source File

SOURCE=.\RectF.h
# End Source File
# Begin Source File

SOURCE=.\ReportDoc.h
# End Source File
# Begin Source File

SOURCE=.\ReportFieldDlg.h
# End Source File
# Begin Source File

SOURCE=.\ReportLayoutDlg.h
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

SOURCE=.\ReportWizardDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SavePromptDlg.h
# End Source File
# Begin Source File

SOURCE=.\ScriptAnalyzer.h
# End Source File
# Begin Source File

SOURCE=.\ScriptEngine.h
# End Source File
# Begin Source File

SOURCE=.\ScriptFrame.h
# End Source File
# Begin Source File

SOURCE=.\ScriptFrm.h
# End Source File
# Begin Source File

SOURCE=.\ScriptView.h
# End Source File
# Begin Source File

SOURCE=.\Splash.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\SymbolDesignDlg.h
# End Source File
# Begin Source File

SOURCE=.\SymbolLibDoc.h
# End Source File
# Begin Source File

SOURCE=.\SymbolLibWnd.h
# End Source File
# Begin Source File

SOURCE=.\SymbolObj.h
# End Source File
# Begin Source File

SOURCE=.\SymbolPreviewWnd.h
# End Source File
# Begin Source File

SOURCE=.\SymbolPropDlg.h
# End Source File
# Begin Source File

SOURCE=.\SymbolPropList.h
# End Source File
# Begin Source File

SOURCE=.\TabOrderDlg.h
# End Source File
# Begin Source File

SOURCE=.\ToolboxBar.h
# End Source File
# Begin Source File

SOURCE=.\TypeChanger.h
# End Source File
# Begin Source File

SOURCE=.\TypeInfoBrowse.h
# End Source File
# Begin Source File

SOURCE=.\UndoStack.h
# End Source File
# Begin Source File

SOURCE=.\Varutils.h
# End Source File
# Begin Source File

SOURCE=.\ZoomDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_p.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00005.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00006.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00007.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00008.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00009.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp27534.bmp
# End Source File
# Begin Source File

SOURCE=.\res\columnar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cur00001.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur27536.cur
# End Source File
# Begin Source File

SOURCE=.\res\cur27537.cur
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\CxDev.ico
# End Source File
# Begin Source File

SOURCE=.\res\CxDev.rc2
# End Source File
# Begin Source File

SOURCE=.\res\DevDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\font.bmp
# End Source File
# Begin Source File

SOURCE=.\res\formatba.bmp
# End Source File
# Begin Source File

SOURCE=.\res\fullscre.bmp
# End Source File
# Begin Source File

SOURCE=.\res\handle_g.bmp
# End Source File
# Begin Source File

SOURCE=.\res\handle_r.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00004.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00005.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00006.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00007.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00008.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00009.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00010.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00011.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\idb_.bmp
# End Source File
# Begin Source File

SOURCE=.\res\idr_layo.ico
# End Source File
# Begin Source File

SOURCE=.\res\idr_main.ico
# End Source File
# Begin Source File

SOURCE=.\res\mainfram.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Palette.bmp
# End Source File
# Begin Source File

SOURCE=.\res\picture.ico
# End Source File
# Begin Source File

SOURCE=.\res\portrait.bmp
# End Source File
# Begin Source File

SOURCE=.\res\portrait.ico
# End Source File
# Begin Source File

SOURCE=.\res\report1.ico
# End Source File
# Begin Source File

SOURCE=.\Splsh16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\suform.ico
# End Source File
# Begin Source File

SOURCE=.\res\tabular1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbox1.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\CxDev.reg
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=..\CxSCBar\Release\CxSCBar.lib
# End Source File
# Begin Source File

SOURCE=..\CxPropSheet\Release\CxPropSheet.lib
# End Source File
# End Target
# End Project
# Section CxDev : {72ADFD78-2C39-11D0-9903-00A0C91BC942}
# 	1:10:IDB_SPLASH:175
# 	2:21:SplashScreenInsertKey:4.0
# End Section
