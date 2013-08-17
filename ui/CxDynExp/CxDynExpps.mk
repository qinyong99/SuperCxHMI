
CxDynExpps.dll: dlldata.obj CxDynExp_p.obj CxDynExp_i.obj
	link /dll /out:CxDynExpps.dll /def:CxDynExpps.def /entry:DllMain dlldata.obj CxDynExp_p.obj CxDynExp_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del CxDynExpps.dll
	@del CxDynExpps.lib
	@del CxDynExpps.exp
	@del dlldata.obj
	@del CxDynExp_p.obj
	@del CxDynExp_i.obj
