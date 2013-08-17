
CxDynObjsps.dll: dlldata.obj CxDynObjs_p.obj CxDynObjs_i.obj
	link /dll /out:CxDynObjsps.dll /def:CxDynObjsps.def /entry:DllMain dlldata.obj CxDynObjs_p.obj CxDynObjs_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del CxDynObjsps.dll
	@del CxDynObjsps.lib
	@del CxDynObjsps.exp
	@del dlldata.obj
	@del CxDynObjs_p.obj
	@del CxDynObjs_i.obj
