
CxExtObjps.dll: dlldata.obj CxExtObj_p.obj CxExtObj_i.obj
	link /dll /out:CxExtObjps.dll /def:CxExtObjps.def /entry:DllMain dlldata.obj CxExtObj_p.obj CxExtObj_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del CxExtObjps.dll
	@del CxExtObjps.lib
	@del CxExtObjps.exp
	@del dlldata.obj
	@del CxExtObj_p.obj
	@del CxExtObj_i.obj
