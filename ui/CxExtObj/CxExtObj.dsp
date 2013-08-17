# Microsoft Developer Studio Project File - Name="CxExtObj" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CxExtObj - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CxExtObj.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CxExtObj.mak" CFG="CxExtObj - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CxExtObj - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CxExtObj - Win32 Unicode Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CxExtObj - Win32 Release MinSize" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CxExtObj - Win32 Release MinDependency" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CxExtObj - Win32 Unicode Release MinSize" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CxExtObj - Win32 Unicode Release MinDependency" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CxExtObj - Win32 Debug"

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
# ADD CPP /nologo /MTd /W3 /Gm /ZI /Od /I "..\CxOle" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD MTL /I "..\CxOle"
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"../CxDemo/CxExtObj.dll" /pdbtype:sept
# Begin Custom Build - Performing registration
OutDir=.\Debug
TargetPath=\spcx\ui\CxDemo\CxExtObj.dll
InputPath=\spcx\ui\CxDemo\CxExtObj.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CxExtObj - Win32 Unicode Debug"

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
TargetPath=.\DebugU\CxExtObj.dll
InputPath=.\DebugU\CxExtObj.dll
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

!ELSEIF  "$(CFG)" == "CxExtObj - Win32 Release MinSize"

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
# ADD CPP /nologo /MT /W3 /O1 /I "../CxOle" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_ATL_DLL" /Yu"stdafx.h" /FD /c
# ADD MTL /I "../CxOle"
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"../../bin/CxExtObj.dll"
# Begin Custom Build - Performing registration
OutDir=.\ReleaseMinSize
TargetPath=\spcx\bin\CxExtObj.dll
InputPath=\spcx\bin\CxExtObj.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CxExtObj - Win32 Release MinDependency"

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
TargetPath=.\ReleaseMinDependency\CxExtObj.dll
InputPath=.\ReleaseMinDependency\CxExtObj.dll
SOURCE="$(InputPath)"

"$(OutDir)\regsvr32.trg" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	regsvr32 /s /c "$(TargetPath)" 
	echo regsvr32 exec. time > "$(OutDir)\regsvr32.trg" 
	
# End Custom Build

!ELSEIF  "$(CFG)" == "CxExtObj - Win32 Unicode Release MinSize"

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
TargetPath=.\ReleaseUMinSize\CxExtObj.dll
InputPath=.\ReleaseUMinSize\CxExtObj.dll
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

!ELSEIF  "$(CFG)" == "CxExtObj - Win32 Unicode Release MinDependency"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseUMinDependency"
# PROP BASE Intermediate_Dir "ReleaseUMinDependency"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ReleaseUMinDependency"
# PROP Intermediate_Dir "ReleaseUMinDependency"
# PROP Ignore_Export_Lib 0
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
TargetPath=.\ReleaseUMinDependency\CxExtObj.dll
InputPath=.\ReleaseUMinDependency\CxExtObj.dll
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

# Name "CxExtObj - Win32 Debug"
# Name "CxExtObj - Win32 Unicode Debug"
# Name "CxExtObj - Win32 Release MinSize"
# Name "CxExtObj - Win32 Release MinDependency"
# Name "CxExtObj - Win32 Unicode Release MinSize"
# Name "CxExtObj - Win32 Unicode Release MinDependency"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AlphaNumEntryDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BlankPage.cpp
# End Source File
# Begin Source File

SOURCE=.\CxExtObj.cpp
# End Source File
# Begin Source File

SOURCE=.\CxExtObj.def
# End Source File
# Begin Source File

SOURCE=.\CxExtObj.idl

!IF  "$(CFG)" == "CxExtObj - Win32 Debug"

# ADD MTL /tlb ".\CxExtObj.tlb" /h "CxExtObj.h" /iid "CxExtObj_i.c" /Oicf

!ELSEIF  "$(CFG)" == "CxExtObj - Win32 Unicode Debug"

# ADD MTL /tlb ".\CxExtObj.tlb" /h "CxExtObj.h" /iid "CxExtObj_i.c" /Oicf

!ELSEIF  "$(CFG)" == "CxExtObj - Win32 Release MinSize"

# ADD MTL /tlb ".\CxExtObj.tlb" /h "CxExtObj.h" /iid "CxExtObj_i.c" /Oicf

!ELSEIF  "$(CFG)" == "CxExtObj - Win32 Release MinDependency"

# ADD MTL /tlb ".\CxExtObj.tlb" /h "CxExtObj.h" /iid "CxExtObj_i.c" /Oicf

!ELSEIF  "$(CFG)" == "CxExtObj - Win32 Unicode Release MinSize"

# ADD MTL /tlb ".\CxExtObj.tlb" /h "CxExtObj.h" /iid "CxExtObj_i.c" /Oicf

!ELSEIF  "$(CFG)" == "CxExtObj - Win32 Unicode Release MinDependency"

# ADD MTL /I "..\CxOle" /tlb ".\CxExtObj.tlb" /h "CxExtObj.h" /iid "CxExtObj_i.c" /Oicf

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\CxExtObj.rc
# End Source File
# Begin Source File

SOURCE=.\DataConnection.cpp
# End Source File
# Begin Source File

SOURCE=.\DataConnectionPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DataEntry.cpp
# End Source File
# Begin Source File

SOURCE=.\DataField.cpp
# End Source File
# Begin Source File

SOURCE=.\DataRecordset.cpp
# End Source File
# Begin Source File

SOURCE=.\DataRecordsetPage.cpp
# End Source File
# Begin Source File

SOURCE=.\DataSourceDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EventObj.cpp
# End Source File
# Begin Source File

SOURCE=.\OEMSetup.cpp
# End Source File
# Begin Source File

SOURCE=.\PushbuttonEntryDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ReturnInteger.cpp
# End Source File
# Begin Source File

SOURCE=.\SliderEntryDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SQLBuildDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SQLBuilder.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AlphaNumEntryDlg.h
# End Source File
# Begin Source File

SOURCE=.\BlankPage.h
# End Source File
# Begin Source File

SOURCE=.\DataConnection.h
# End Source File
# Begin Source File

SOURCE=.\DataConnectionPage.h
# End Source File
# Begin Source File

SOURCE=.\DataEntry.h
# End Source File
# Begin Source File

SOURCE=.\DataField.h
# End Source File
# Begin Source File

SOURCE=.\DataRecordset.h
# End Source File
# Begin Source File

SOURCE=.\DataRecordsetPage.h
# End Source File
# Begin Source File

SOURCE=.\DataSourceDlg.h
# End Source File
# Begin Source File

SOURCE=.\DialogSizeHelper.h
# End Source File
# Begin Source File

SOURCE=.\EventObj.h
# End Source File
# Begin Source File

SOURCE=.\OEMSetup.h
# End Source File
# Begin Source File

SOURCE=.\PopupList.h
# End Source File
# Begin Source File

SOURCE=.\PropertyPageImpl2.h
# End Source File
# Begin Source File

SOURCE=.\PushbuttonEntryDlg.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ReturnInteger.h
# End Source File
# Begin Source File

SOURCE=.\SliderEntryDlg.h
# End Source File
# Begin Source File

SOURCE=.\SQLBuildDlg.h
# End Source File
# Begin Source File

SOURCE=.\SQLBuilder.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\CxBlankPage.rgs
# End Source File
# Begin Source File

SOURCE=.\CxDataConnection.rgs
# End Source File
# Begin Source File

SOURCE=.\CxDataConnectionPage.rgs
# End Source File
# Begin Source File

SOURCE=.\CxDataEntry.rgs
# End Source File
# Begin Source File

SOURCE=.\CxDataField.rgs
# End Source File
# Begin Source File

SOURCE=.\CxDataRecordset.rgs
# End Source File
# Begin Source File

SOURCE=.\CxDataRecordsetPage.rgs
# End Source File
# Begin Source File

SOURCE=.\CxEventObj.rgs
# End Source File
# Begin Source File

SOURCE=.\CxReturnInteger.rgs
# End Source File
# Begin Source File

SOURCE=.\CxSQLBuilder.rgs
# End Source File
# End Group
# End Target
# End Project
# Section CxExtObj : {00000002-02A0-0034-A002-340000000000}
# 	1:23:IDS_HELPFILECxBlankPage:105
# 	1:15:IDR_CXBLANKPAGE:107
# 	1:20:IDS_TITLECxBlankPage:104
# 	1:15:IDD_CXBLANKPAGE:108
# 	1:24:IDS_DOCSTRINGCxBlankPage:106
# End Section
# Section CxExtObj : {7453206C-6475-6F69-5C43-6F6D6D6F6E5C}
# 	1:28:IDS_TITLECxDataRecordsetPage:113
# 	1:31:IDS_HELPFILECxDataRecordsetPage:114
# 	1:23:IDD_CXDATARECORDSETPAGE:117
# 	1:32:IDS_DOCSTRINGCxDataRecordsetPage:115
# 	1:23:IDR_CXDATARECORDSETPAGE:116
# End Section
# Section CxExtObj : {00100010-0010-0010-1000-100010001000}
# 	1:24:IDR_CXDATACONNECTIONPAGE:121
# 	1:24:IDD_CXDATACONNECTIONPAGE:122
# 	1:33:IDS_DOCSTRINGCxDataConnectionPage:120
# 	1:29:IDS_TITLECxDataConnectionPage:118
# 	1:32:IDS_HELPFILECxDataConnectionPage:119
# End Section
