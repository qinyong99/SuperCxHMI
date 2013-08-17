
CxHmiObjps.dll: dlldata.obj CxHmiObj_p.obj CxHmiObj_i.obj
	link /dll /out:CxHmiObjps.dll /def:CxHmiObjps.def /entry:DllMain dlldata.obj CxHmiObj_p.obj CxHmiObj_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del CxHmiObjps.dll
	@del CxHmiObjps.lib
	@del CxHmiObjps.exp
	@del dlldata.obj
	@del CxHmiObj_p.obj
	@del CxHmiObj_i.obj
