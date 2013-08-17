
CxScripCrtps.dll: dlldata.obj CxScripCrt_p.obj CxScripCrt_i.obj
	link /dll /out:CxScripCrtps.dll /def:CxScripCrtps.def /entry:DllMain dlldata.obj CxScripCrt_p.obj CxScripCrt_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del CxScripCrtps.dll
	@del CxScripCrtps.lib
	@del CxScripCrtps.exp
	@del dlldata.obj
	@del CxScripCrt_p.obj
	@del CxScripCrt_i.obj
