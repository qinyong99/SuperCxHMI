
CxPropCtrlps.dll: dlldata.obj CxPropCtrl_p.obj CxPropCtrl_i.obj
	link /dll /out:CxPropCtrlps.dll /def:CxPropCtrlps.def /entry:DllMain dlldata.obj CxPropCtrl_p.obj CxPropCtrl_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del CxPropCtrlps.dll
	@del CxPropCtrlps.lib
	@del CxPropCtrlps.exp
	@del dlldata.obj
	@del CxPropCtrl_p.obj
	@del CxPropCtrl_i.obj
