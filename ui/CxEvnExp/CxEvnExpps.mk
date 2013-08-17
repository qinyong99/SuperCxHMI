
CxEvnExpps.dll: dlldata.obj CxEvnExp_p.obj CxEvnExp_i.obj
	link /dll /out:CxEvnExpps.dll /def:CxEvnExpps.def /entry:DllMain dlldata.obj CxEvnExp_p.obj CxEvnExp_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del CxEvnExpps.dll
	@del CxEvnExpps.lib
	@del CxEvnExpps.exp
	@del dlldata.obj
	@del CxEvnExp_p.obj
	@del CxEvnExp_i.obj
