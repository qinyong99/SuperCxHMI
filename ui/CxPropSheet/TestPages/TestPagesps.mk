
TestPagesps.dll: dlldata.obj TestPages_p.obj TestPages_i.obj
	link /dll /out:TestPagesps.dll /def:TestPagesps.def /entry:DllMain dlldata.obj TestPages_p.obj TestPages_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del TestPagesps.dll
	@del TestPagesps.lib
	@del TestPagesps.exp
	@del dlldata.obj
	@del TestPages_p.obj
	@del TestPages_i.obj
