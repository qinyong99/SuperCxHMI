
CxDrawShapesps.dll: dlldata.obj CxDrawShapes_p.obj CxDrawShapes_i.obj
	link /dll /out:CxDrawShapesps.dll /def:CxDrawShapesps.def /entry:DllMain dlldata.obj CxDrawShapes_p.obj CxDrawShapes_i.obj \
		kernel32.lib rpcndr.lib rpcns4.lib rpcrt4.lib oleaut32.lib uuid.lib \

.c.obj:
	cl /c /Ox /DWIN32 /D_WIN32_WINNT=0x0400 /DREGISTER_PROXY_DLL \
		$<

clean:
	@del CxDrawShapesps.dll
	@del CxDrawShapesps.lib
	@del CxDrawShapesps.exp
	@del dlldata.obj
	@del CxDrawShapes_p.obj
	@del CxDrawShapes_i.obj
