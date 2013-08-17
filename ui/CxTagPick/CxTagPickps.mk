
CxTagPickps.dll: dlldata.obj CxTagPick_p.obj CxTagPick_i.obj
	link /dll /out:CxTagPickps.dll /def:CxTagPickps.def /entry:DllMain dlldata.obj CxTagPick_p.obj CxTagPick_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del CxTagPickps.dll
	@del CxTagPickps.lib
	@del CxTagPickps.exp
	@del dlldata.obj
	@del CxTagPick_p.obj
	@del CxTagPick_i.obj
