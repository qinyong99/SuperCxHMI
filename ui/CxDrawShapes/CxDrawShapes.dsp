# Microsoft Developer Studio Project File - Name="CxDrawShapes" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CxDrawShapes - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CxDrawShapes.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CxDrawShapes.mak" CFG="CxDrawShapes - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CxDrawShapes - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CxDrawShapes - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CxDrawShapes - Win32 Release MinSize" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CxDrawShapes - Win32 Release MinDependency" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CxDrawShapes - Win32 Unicode Release MinSize" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CxDrawShapes - Win32 Unicode Release MinDependency" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CxDrawShapes - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /ZI /Od /I "../CxOle" /I "../GdiInclude" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /Yu"stdafx.h" /FD /GZ /Zm200 /c
# ADD MTL /I "../CxOle"
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 comctl32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib gdiplus.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../CxDemo/CxDrawShapes.dll" /pdbtype:sept
# Begin Custom Build - Performing registration
OutDir=.\Debug
TargetPath=\spcx\ui\CxDemo\CxDrawShapes.dll
InputPath=\spcx\ui\CxDemo\CxDrawShapes.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CxDrawShapes - Win32 Unicode Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "DebugU"
# PROP BASE Intermediate_Dir "DebugU"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "DebugU"
# PROP Intermediate_Dir "DebugU"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# Begin Custom Build - Performing registration
OutDir=.\DebugU
TargetPath=.\DebugU\CxDrawShapes.dll
InputPath=.\DebugU\CxDrawShapes.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT 
	if not "%OS%"=="Windows_NT" goto NOTNT 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	goto end 
	:NOTNT 
	echo Warning : Cannot register Unicode DLL on Windows 95 
	:end 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CxDrawShapes - Win32 Release MinSize"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinSize"
# PROP BASE Intermediate_Dir "ReleaseMinSize"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseMinSize"
# PROP Intermediate_Dir "ReleaseMinSize"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_ATL_DLL" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /O1 /I "../CxOle" /I "../GdiInclude" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_ATL_DLL" /Yu"stdafx.h" /FD /Zm200 /c
# ADD MTL /I "../CxOle"
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib gdiplus.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../../bin/CxDrawShapes.dll"
# Begin Custom Build - Performing registration
OutDir=.\ReleaseMinSize
TargetPath=\spcx\bin\CxDrawShapes.dll
InputPath=\spcx\bin\CxDrawShapes.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CxDrawShapes - Win32 Release MinDependency"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseMinDependency"
# PROP BASE Intermediate_Dir "ReleaseMinDependency"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseMinDependency"
# PROP Intermediate_Dir "ReleaseMinDependency"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_ATL_STATIC_REGISTRY" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# Begin Custom Build - Performing registration
OutDir=.\ReleaseMinDependency
TargetPath=.\ReleaseMinDependency\CxDrawShapes.dll
InputPath=.\ReleaseMinDependency\CxDrawShapes.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CxDrawShapes - Win32 Unicode Release MinSize"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinSize"
# PROP BASE Intermediate_Dir "ReleaseUMinSize"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseUMinSize"
# PROP Intermediate_Dir "ReleaseUMinSize"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /D "_ATL_DLL" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /D "_ATL_DLL" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# Begin Custom Build - Performing registration
OutDir=.\ReleaseUMinSize
TargetPath=.\ReleaseUMinSize\CxDrawShapes.dll
InputPath=.\ReleaseUMinSize\CxDrawShapes.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT 
	if not "%OS%"=="Windows_NT" goto NOTNT 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	goto end 
	:NOTNT 
	echo Warning : Cannot register Unicode DLL on Windows 95 
	:end 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CxDrawShapes - Win32 Unicode Release MinDependency"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinDependency"
# PROP BASE Intermediate_Dir "ReleaseUMinDependency"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseUMinDependency"
# PROP Intermediate_Dir "ReleaseUMinDependency"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /D "_ATL_STATIC_REGISTRY" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_UNICODE" /D "_ATL_STATIC_REGISTRY" /D "_ATL_MIN_CRT" /Yu"stdafx.h" /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# Begin Custom Build - Performing registration
OutDir=.\ReleaseUMinDependency
TargetPath=.\ReleaseUMinDependency\CxDrawShapes.dll
InputPath=.\ReleaseUMinDependency\CxDrawShapes.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	if "%OS%"=="" goto NOTNT 
	if not "%OS%"=="Windows_NT" goto NOTNT 
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	goto end 
	:NOTNT 
	echo Warning : Cannot register Unicode DLL on Windows 95 
	:end 
	
# End Custom Build

!ENDIF 

# Begin Target

# Name "CxDrawShapes - Win32 Debug"
# Name "CxDrawShapes - Win32 Unicode Debug"
# Name "CxDrawShapes - Win32 Release MinSize"
# Name "CxDrawShapes - Win32 Release MinDependency"
# Name "CxDrawShapes - Win32 Unicode Release MinSize"
# Name "CxDrawShapes - Win32 Unicode Release MinDependency"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ButtonGenPage.cpp
# End Source File
# Begin Source File

SOURCE=.\CustCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\CxDrawShapes.cpp
# End Source File
# Begin Source File

SOURCE=.\CxDrawShapes.def
# End Source File
# Begin Source File

SOURCE=.\CxDrawShapes.idl
# ADD MTL /tlb ".\CxDrawShapes.tlb" /h "CxDrawShapes.h" /iid "CxDrawShapes_i.c" /Oicf
# End Source File
# Begin Source File

SOURCE=.\CxDrawShapes.rc
# End Source File
# Begin Source File

SOURCE=.\DataLinkGenPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawArc.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawButton.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawChord.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawDataLink.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawEllipse.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawImage.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawLine.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawPie.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawPipe.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawPolygon.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawPolyLine.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawRectangle.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawRoundRectangle.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawTimer.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawTxt.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawVariable.cpp
# End Source File
# Begin Source File

SOURCE=.\FloatPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\FontComboBox.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageGenPage.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuGenPage.cpp
# End Source File
# Begin Source File

SOURCE=.\PipeGenPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ShapeGenPage.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TextGenPage.cpp
# End Source File
# Begin Source File

SOURCE=.\TextTextPage.cpp
# End Source File
# Begin Source File

SOURCE=.\TimerGenPage.cpp
# End Source File
# Begin Source File

SOURCE=.\VariableGenPage.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ButtonGenPage.h
# End Source File
# Begin Source File

SOURCE=.\ChineseVarUtils.h
# End Source File
# Begin Source File

SOURCE=.\CustCtrl.h
# End Source File
# Begin Source File

SOURCE=.\DataLinkGenPage.h
# End Source File
# Begin Source File

SOURCE=.\DrawArc.h
# End Source File
# Begin Source File

SOURCE=.\DrawButton.h
# End Source File
# Begin Source File

SOURCE=.\DrawChord.h
# End Source File
# Begin Source File

SOURCE=.\DrawDataLink.h
# End Source File
# Begin Source File

SOURCE=.\DrawEllipse.h
# End Source File
# Begin Source File

SOURCE=.\DrawImage.h
# End Source File
# Begin Source File

SOURCE=.\DrawLine.h
# End Source File
# Begin Source File

SOURCE=.\DrawMenu.h
# End Source File
# Begin Source File

SOURCE=.\DrawPie.h
# End Source File
# Begin Source File

SOURCE=.\DrawPipe.h
# End Source File
# Begin Source File

SOURCE=.\DrawPolygon.h
# End Source File
# Begin Source File

SOURCE=.\DrawPolyLine.h
# End Source File
# Begin Source File

SOURCE=.\DrawRectangle.h
# End Source File
# Begin Source File

SOURCE=.\DrawRoundRectangle.h
# End Source File
# Begin Source File

SOURCE=.\DrawTimer.h
# End Source File
# Begin Source File

SOURCE=.\DrawTxt.h
# End Source File
# Begin Source File

SOURCE=.\DrawVariable.h
# End Source File
# Begin Source File

SOURCE=.\FloatPoint.h
# End Source File
# Begin Source File

SOURCE=.\FontComboBox.h
# End Source File
# Begin Source File

SOURCE=.\ImageGenPage.h
# End Source File
# Begin Source File

SOURCE=.\MenuGenPage.h
# End Source File
# Begin Source File

SOURCE=.\PipeGenPage.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ShapeGenPage.h
# End Source File
# Begin Source File

SOURCE=.\SimpleArray2.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TextGenPage.h
# End Source File
# Begin Source File

SOURCE=.\TextTextPage.h
# End Source File
# Begin Source File

SOURCE=.\TimerGenPage.h
# End Source File
# Begin Source File

SOURCE=.\VariableGenPage.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\CxButtonGenPage.rgs
# End Source File
# Begin Source File

SOURCE=.\CxDataLinkGenPage.rgs
# End Source File
# Begin Source File

SOURCE=.\CxImageGenPage.rgs
# End Source File
# Begin Source File

SOURCE=.\CxMenuGenPage.rgs
# End Source File
# Begin Source File

SOURCE=.\CxPipeGenPage.rgs
# End Source File
# Begin Source File

SOURCE=.\CxShapeGenPage.rgs
# End Source File
# Begin Source File

SOURCE=.\CxTextGenPage.rgs
# End Source File
# Begin Source File

SOURCE=.\CxTextTextPage.rgs
# End Source File
# Begin Source File

SOURCE=.\CxTimerGenPage.rgs
# End Source File
# Begin Source File

SOURCE=.\CxVariableGenPage.rgs
# End Source File
# Begin Source File

SOURCE=.\DrawArc.rgs
# End Source File
# Begin Source File

SOURCE=.\DrawButton.rgs
# End Source File
# Begin Source File

SOURCE=.\DrawChord.rgs
# End Source File
# Begin Source File

SOURCE=.\DrawDataLink.rgs
# End Source File
# Begin Source File

SOURCE=.\DrawEllipse.rgs
# End Source File
# Begin Source File

SOURCE=.\DrawImage.rgs
# End Source File
# Begin Source File

SOURCE=.\DrawLine.rgs
# End Source File
# Begin Source File

SOURCE=.\DrawMenu.rgs
# End Source File
# Begin Source File

SOURCE=.\DrawPie.rgs
# End Source File
# Begin Source File

SOURCE=.\DrawPipe.rgs
# End Source File
# Begin Source File

SOURCE=.\DrawPolygon.rgs
# End Source File
# Begin Source File

SOURCE=.\DrawPolyLine.rgs
# End Source File
# Begin Source File

SOURCE=.\DrawRectangle.rgs
# End Source File
# Begin Source File

SOURCE=.\DrawRoundRectangle.rgs
# End Source File
# Begin Source File

SOURCE=.\DrawTimer.rgs
# End Source File
# Begin Source File

SOURCE=.\DrawTxt.rgs
# End Source File
# Begin Source File

SOURCE=.\DrawVariable.rgs
# End Source File
# Begin Source File

SOURCE=.\dwon1.ico
# End Source File
# Begin Source File

SOURCE=.\FloatPoint.rgs
# End Source File
# Begin Source File

SOURCE=.\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\next.ico
# End Source File
# Begin Source File

SOURCE=.\TRUETYPE.BMP
# End Source File
# Begin Source File

SOURCE=.\up1.ico
# End Source File
# End Group
# End Target
# End Project
# Section CxDrawShapes : {20000012-0000-0000-0200-020088460200}
# 	1:17:IDR_CXMENUGENPAGE:170
# 	1:17:IDD_CXMENUGENPAGE:171
# 	1:25:IDS_HELPFILECxMenuGenPage:168
# 	1:26:IDS_DOCSTRINGCxMenuGenPage:169
# 	1:22:IDS_TITLECxMenuGenPage:167
# End Section
# Section CxDrawShapes : {03F6E5A8-0C74-07FA-5C0C-FA0718F1F907}
# 	1:28:IDS_DOCSTRINGCxButtonGenPage:130
# 	1:19:IDR_CXBUTTONGENPAGE:131
# 	1:24:IDS_TITLECxButtonGenPage:120
# 	1:19:IDD_CXBUTTONGENPAGE:132
# 	1:27:IDS_HELPFILECxButtonGenPage:129
# End Section
# Section CxDrawShapes : {00003B7A-04A7-0000-A704-0000D93A0000}
# 	1:23:IDS_TITLECxTextTextPage:144
# 	1:26:IDS_HELPFILECxTextTextPage:145
# 	1:18:IDD_CXTEXTTEXTPAGE:148
# 	1:18:IDR_CXTEXTTEXTPAGE:147
# 	1:27:IDS_DOCSTRINGCxTextTextPage:146
# End Section
# Section CxDrawShapes : {07E140A8-0000-0000-20D9-CD6B00000000}
# 	1:23:IDS_TITLECxImageGenPage:133
# 	1:18:IDD_CXIMAGEGENPAGE:137
# 	1:26:IDS_HELPFILECxImageGenPage:134
# 	1:18:IDR_CXIMAGEGENPAGE:136
# 	1:27:IDS_DOCSTRINGCxImageGenPage:135
# End Section
# Section CxDrawShapes : {7453206C-6475-6F69-5C43-6F6D6D6F6E5C}
# 	1:23:IDS_TITLECxTimerGenPage:161
# 	1:18:IDD_CXTIMERGENPAGE:165
# 	1:26:IDS_HELPFILECxTimerGenPage:162
# 	1:18:IDR_CXTIMERGENPAGE:164
# 	1:27:IDS_DOCSTRINGCxTimerGenPage:163
# End Section
# Section CxDrawShapes : {00000003-0348-0034-4803-340000000000}
# 	1:17:IDR_CXPIPEGENPAGE:118
# 	1:17:IDD_CXPIPEGENPAGE:119
# 	1:26:IDS_DOCSTRINGCxPipeGenPage:117
# 	1:25:IDS_HELPFILECxPipeGenPage:116
# 	1:22:IDS_TITLECxPipeGenPage:115
# End Section
# Section CxDrawShapes : {2000001C-0000-0000-0600-060069020000}
# 	1:26:IDS_HELPFILECxShapeGenPage:111
# 	1:18:IDR_CXSHAPEGENPAGE:113
# 	1:27:IDS_DOCSTRINGCxShapeGenPage:112
# 	1:23:IDS_TITLECxShapeGenPage:110
# 	1:18:IDD_CXSHAPEGENPAGE:114
# End Section
