@ECHO OFF

PUSHD $~dp0\..\
vendor\premake\premake5.exe --file=Build.lua vs2019
vendor\premake\premake5.exe --file=Sandbox/Build-Sandbox.lua vs2019
POPD

PAUSE
