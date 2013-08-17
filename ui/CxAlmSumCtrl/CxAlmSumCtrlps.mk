
CxAlmSumCtrlps.dll: dlldata.obj CxAlmSumCtrl_p.obj CxAlmSumCtrl_i.obj
	link /dll /out:CxAlmSumCtrlps.dll /def:CxAlmSumCtrlps.def /entry:DllMain dlldata.obj CxAlmSumCtrl_p.obj CxAlmSumCtrl_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del CxAlmSumCtrlps.dll
	@del CxAlmSumCtrlps.lib
	@del CxAlmSumCtrlps.exp
	@del dlldata.obj
	@del CxAlmSumCtrl_p.obj
	@del CxAlmSumCtrl_i.obj
