
CxPropValps.dll: dlldata.obj CxPropVal_p.obj CxPropVal_i.obj
	link /dll /out:CxPropValps.dll /def:CxPropValps.def /entry:DllMain dlldata.obj CxPropVal_p.obj CxPropVal_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del CxPropValps.dll
	@del CxPropValps.lib
	@del CxPropValps.exp
	@del dlldata.obj
	@del CxPropVal_p.obj
	@del CxPropVal_i.obj
