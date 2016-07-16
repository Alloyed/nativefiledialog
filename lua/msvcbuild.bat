setlocal
@set LUA_FUNC=luaopen_nfd
@set DLL=nfd.dll

@set NFD_CFLAGS=/D_CRT_SECURE_NO_WARNINGS /I"src/include"

@set LUA_INCDIR="F:\Downloads\LuaJIT\include"
@set LUA_LIBDIR="F:\Downloads\LuaJIT\lua51.lib"

@if /I LUA_INCDIR=="" ( echo LUA_INCDIR not specified in msvcbuild.bat & exit )
@if /I LUA_LIBDIR=="" ( echo LUA_LIBDIR not specified in msvcbuild.bat & exit )

@set CFLAGS=/nologo /O2
@set LIBFLAG=/nologo /dll /export:%LUA_FUNC% /out:%DLL%

rem nfd_common.obj
cl /c %CFLAGS% %NFD_CFLAGS% "src/nfd_common.c" /Fo"nfd_common.obj"

rem nfd_win.obj
cl /c %CFLAGS% %NFD_CFLAGS% "src/nfd_win.cpp" /Fo"nfd_win.obj"

rem nfd_wrap_lua.obj
cl /c %CFLAGS% %NFD_CFLAGS% /I%LUA_INCDIR% "lua/nfd_wrap_lua.c" /Fo"nfd_wrap_lua.obj"

rem nfd.dll
link %LIBFLAG% *.obj %LUA_LIBDIR%
