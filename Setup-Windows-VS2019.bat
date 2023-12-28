@ECHO OFF

PUSHD $~dp0\..\
vendor\premake\premake5.exe --file=Build.lua vs2019
POPD

PAUSE
