
CxDrawToolsps.dll: dlldata.obj CxDrawTools_p.obj CxDrawTools_i.obj
	link /dll /out:CxDrawToolsps.dll /def:CxDrawToolsps.def /entry:DllMain dlldata.obj CxDrawTools_p.obj CxDrawTools_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del CxDrawToolsps.dll
	@del CxDrawToolsps.lib
	@del CxDrawToolsps.exp
	@del dlldata.obj
	@del CxDrawTools_p.obj
	@del CxDrawTools_i.obj
