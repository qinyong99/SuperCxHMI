rem @echo off

rem 设置批处理文件所在目录为当前目录
d:
cd d:\supercxhmi\bld

echo 开始 SuperCx 每日构建...

rem 设置日期变量
@for /f "tokens=1-3 delims=/- " %%a in ('date /t') do (set DATE=%%a-%%b-%%c)

rem 将VC6.0和installshield可执行路径加入的环境路径变量
set PATH=%PATH%;C:\Program Files\Microsoft Visual Studio\Common\MSDev98\Bin\;C:\Program Files\Microsoft Visual Studio 10.0\Common7\IDE;C:\Program Files\InstallShield\Professional - Standard Edition\Program\;

set DRV_PATH=..\drv
set SVR_PATH=..\svr
set UI_PATH=..\ui
set BIN_FULL=..\bin_full
set LOG_PATH=.\BuildLog\%DATE%
set TARGET_PATH="..\rls\SuperCxSetup_3_0"

rd /s /q %LOG_PATH%
md %LOG_PATH%

set VC6_LOG=%LOG_PATH%\vc6.log
set INSTALL_LOG=%LOG_PATH%\install.log

rem 直接跳转到installshield
if "%1"=="install" goto :Install

rem @for /f "tokens=1 delims= " %%a in (uiprojects.inf) do (echo %%a)

rd /s /q ..\bin
md ..\bin
rd /s /q ..\bin_full
md ..\bin_full

echo build common ...
MSDEV.EXE %UI_PATH%\CxSCBar\CxSCBar.dsw /MAKE "CxSCBar - Win32 Release" /REBUILD >> %VC6_LOG%

echo build ui ...
MSDEV.EXE %UI_PATH%\CxHmiObj\CxHmiObj.dsw /MAKE "CxHmiObj - Win32 Release MinSize" /REBUILD >> %VC6_LOG%
MSDEV.EXE %UI_PATH%\CxOle\CxOle.dsw /MAKE "CxOle - Win32 Release" /REBUILD >> %VC6_LOG%
MSDEV.EXE %UI_PATH%\CxComObj\CxComObj.dsw /MAKE "CxComObj - Win32 Release MinSize" /REBUILD >> %VC6_LOG%
MSDEV.EXE %UI_PATH%\CxPropSheet\CxPropSheet.dsw /MAKE "CxPropSheet - Win32 Release" /REBUILD >> %VC6_LOG%
MSDEV.EXE %UI_PATH%\CxTagPick\CxTagPick.dsw /MAKE "CxTagPick - Win32 Release MinSize" /REBUILD >> %VC6_LOG%
MSDEV.EXE %UI_PATH%\CxScripCrt\CxScripCrt.dsw /MAKE "CxScripCrt - Win32 Release MinSize" /REBUILD >> %VC6_LOG%
MSDEV.EXE %UI_PATH%\scintilla\vcbuild\SciLexer.dsw /MAKE "SciLexer - Win32 Release" /REBUILD >> %VC6_LOG%
MSDEV.EXE %UI_PATH%\CxDynObjs\CxDynObjs.dsw /MAKE "CxDynObjs - Win32 Release MinSize" /REBUILD >> %VC6_LOG%
MSDEV.EXE %UI_PATH%\CxDrawShapes\CxDrawShapes.dsw /MAKE "CxDrawShapes - Win32 Release MinSize" /REBUILD >> %VC6_LOG%
MSDEV.EXE %UI_PATH%\CxChartCtrl\CxChartCtrl.dsw /MAKE "CxChartCtrl - Win32 Release MinSize" /REBUILD >> %VC6_LOG%
MSDEV.EXE %UI_PATH%\CxAlmSumCtrl\CxAlmSumCtrl.dsw /MAKE "CxAlmSumCtrl - Win32 Release MinSize" /REBUILD >> %VC6_LOG%
MSDEV.EXE %UI_PATH%\CxDrawTools\CxDrawTools.dsw /MAKE "CxDrawTools - Win32 Release MinSize" /REBUILD >> %VC6_LOG%
MSDEV.EXE %UI_PATH%\CxExtObj\CxExtObj.dsw /MAKE "CxExtObj - Win32 Release MinSize" /REBUILD >> %VC6_LOG%
MSDEV.EXE %UI_PATH%\CxPropVal\CxPropVal.dsw /MAKE "CxPropVal - Win32 Release MinSize" /REBUILD >> %VC6_LOG%
MSDEV.EXE %UI_PATH%\CxPropCtrl\CxPropCtrl.dsw /MAKE "CxPropCtrl - Win32 Release MinSize" /REBUILD >> %VC6_LOG%
MSDEV.EXE %UI_PATH%\CxAppearExp\CxAppearExp.dsw /MAKE "CxAppearExp - Win32 Release MinSize" /REBUILD >> %VC6_LOG%
MSDEV.EXE %UI_PATH%\CxDynExp\CxDynExp.dsw /MAKE "CxDynExp - Win32 Release MinSize" /REBUILD >> %VC6_LOG%
MSDEV.EXE %UI_PATH%\CxEvnExp\CxEvnExp.dsw /MAKE "CxEvnExp - Win32 Debug" /REBUILD >> %VC6_LOG%
MSDEV.EXE %UI_PATH%\CxDev\CxDev.dsw /MAKE "CxDev - Win32 Release" /REBUILD >> %VC6_LOG%
MSDEV.EXE %UI_PATH%\CxRunCtrl\CxRunCtrl.dsw /MAKE "CxRunCtrl - Win32 Release" /REBUILD >> %VC6_LOG%
MSDEV.EXE %UI_PATH%\WinLockDll\WinLockDll.dsw /MAKE "WinLockDll - Win32 Release" /REBUILD >> %VC6_LOG%
MSDEV.EXE %UI_PATH%\CxRun\CxRun.dsw /MAKE "CxRun - Win32 Release" /REBUILD >> %VC6_LOG%

echo copy files ...
xcopy ..\bin_svr %BIN_FULL% /s /y
xcopy ..\bin %BIN_FULL% /s /y
xcopy ..\other\bin %BIN_FULL% /s /y

echo register drv files ...
regsvr32 /s  %BIN_FULL%\CxCommTask.dll
regsvr32 /s  %BIN_FULL%\Drivers\CxDrvAIBUS.dll
regsvr32 /s  %BIN_FULL%\Drivers\CxDrvDDE.dll
regsvr32 /s  %BIN_FULL%\Drivers\CxDrvFx.dll
regsvr32 /s  %BIN_FULL%\Drivers\CxDrvHostLink.dll
regsvr32 /s  %BIN_FULL%\Drivers\CxDrvHWJT08C.dll
regsvr32 /s  %BIN_FULL%\Drivers\CxDrvIrrigate.dll
regsvr32 /s  %BIN_FULL%\Drivers\CxDrvLGMK.dll
regsvr32 /s  %BIN_FULL%\Drivers\CxDrvModbus.dll
regsvr32 /s  %BIN_FULL%\Drivers\CxDrvNNS.dll
regsvr32 /s  %BIN_FULL%\Drivers\CxDrvOPC.dll
regsvr32 /s  %BIN_FULL%\Drivers\CxDrvS7PPI.dll
regsvr32 /s  %BIN_FULL%\Drivers\CxDrvS7MPI.dll
regsvr32 /s  %BIN_FULL%\Drivers\CxDrvSimulator.dll
regsvr32 /s  %BIN_FULL%\Drivers\CxDrvWaterTank.dll
regsvr32 /s  %BIN_FULL%\Drivers\CxDrvFinsTcp.dll
regsvr32 /s  %BIN_FULL%\Drivers\CxDrvEC2006.dll
regsvr32 /s  %BIN_FULL%\Drivers\CxDrvCsMfc.dll
regsvr32 /s  %BIN_FULL%\Drivers\CxDrvPplRec.dll
regsvr32 /s  %BIN_FULL%\Drivers\CxDrvSCSQ.dll
regsvr32 /s  %BIN_FULL%\Drivers\CxDrvTianSuMine.dll

echo register svr files ...
regsvr32 /s  %BIN_FULL%\CxBasAcs.dll
regsvr32 /s  %BIN_FULL%\CxBasBlk.dll
regsvr32 /s  %BIN_FULL%\CxAlmAreMan.dll
regsvr32 /s  %BIN_FULL%\CxAppMan.dll
regsvr32 /s  %BIN_FULL%\CxDatMan.dll
regsvr32 /s  %BIN_FULL%\CxDevMan.dll
regsvr32 /s  %BIN_FULL%\CxMiscMan.dll
regsvr32 /s  %BIN_FULL%\CxNodeMan.dll
regsvr32 /s  %BIN_FULL%\CxPictMan.dll
regsvr32 /s  %BIN_FULL%\CxRecpMan.dll
regsvr32 /s  %BIN_FULL%\CxSchedMan.dll
regsvr32 /s  %BIN_FULL%\CxSecAreMan.dll
regsvr32 /s  %BIN_FULL%\CxUserMan.dll
regsvr32 /s  %BIN_FULL%\CxSACProxy.dll
regsvr32 /s  %BIN_FULL%\CxDatSes.dll
regsvr32 /s  %BIN_FULL%\CxDBProv.dll
regsvr32 /s  %BIN_FULL%\CxExcelUtil.dll
regsvr32 /s  %BIN_FULL%\CxGameManager.dll
%BIN_FULL%\CxSAC.exe /RegServer

:Install

pause


