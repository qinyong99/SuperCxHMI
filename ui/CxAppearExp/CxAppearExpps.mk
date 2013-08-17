
CxAppearExpps.dll: dlldata.obj CxAppearExp_p.obj CxAppearExp_i.obj
	link /dll /out:CxAppearExpps.dll /def:CxAppearExpps.def /entry:DllMain dlldata.obj CxAppearExp_p.obj CxAppearExp_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del CxAppearExpps.dll
	@del CxAppearExpps.lib
	@del CxAppearExpps.exp
	@del dlldata.obj
	@del CxAppearExp_p.obj
	@del CxAppearExp_i.obj
