
CxComObjps.dll: dlldata.obj CxComObj_p.obj CxComObj_i.obj
	link /dll /out:CxComObjps.dll /def:CxComObjps.def /entry:DllMain dlldata.obj CxComObj_p.obj CxComObj_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del CxComObjps.dll
	@del CxComObjps.lib
	@del CxComObjps.exp
	@del dlldata.obj
	@del CxComObj_p.obj
	@del CxComObj_i.obj
