# Microsoft Developer Studio Project File - Name="CxOle" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Generic Project" 0x010a

CFG=CxOle - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CxOle.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CxOle.mak" CFG="CxOle - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CxOle - Win32 Release" (based on "Win32 (x86) Generic Project")
!MESSAGE "CxOle - Win32 Debug" (based on "Win32 (x86) Generic Project")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
MTL=midl.exe

!IF  "$(CFG)" == "CxOle - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""

!ELSEIF  "$(CFG)" == "CxOle - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""

!ENDIF 

# Begin Target

# Name "CxOle - Win32 Release"
# Name "CxOle - Win32 Debug"
# Begin Source File

SOURCE=.\CxCatProp.h
# End Source File
# Begin Source File

SOURCE=.\CxDispid.h
# End Source File
# Begin Source File

SOURCE=.\CxDrawObj.h
# End Source File
# Begin Source File

SOURCE=.\CxDrawTool.h
# End Source File
# Begin Source File

SOURCE=.\CxDynObj.h
# End Source File
# Begin Source File

SOURCE=..\CxHmiObj\CxHmiMisc.idl
# End Source File
# Begin Source File

SOURCE=.\CxHmiObj.idl

!IF  "$(CFG)" == "CxOle - Win32 Release"

# ADD MTL /tlb "CxHmiObj.tlb"

!ELSEIF  "$(CFG)" == "CxOle - Win32 Debug"

# ADD MTL /tlb ".\CxHmiObj.tlb" /h "CxHmiObj.h" /iid "CxHmiObj_i.c" /Oicf

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\ExpressSerializer.h
# End Source File
# Begin Source File

SOURCE=.\PromptTimeElapse.h
# End Source File
# End Target
# End Project
