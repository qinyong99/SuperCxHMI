
CxChartCtrlps.dll: dlldata.obj CxChartCtrl_p.obj CxChartCtrl_i.obj
	link /dll /out:CxChartCtrlps.dll /def:CxChartCtrlps.def /entry:DllMain dlldata.obj CxChartCtrl_p.obj CxChartCtrl_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del CxChartCtrlps.dll
	@del CxChartCtrlps.lib
	@del CxChartCtrlps.exp
	@del dlldata.obj
	@del CxChartCtrl_p.obj
	@del CxChartCtrl_i.obj
