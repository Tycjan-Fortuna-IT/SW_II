@ECHO OFF

PUSHD $~dp0\..\
vendor\premake\premake5.exe --file=Build.lua vs2022
vendor\premake\premake5.exe --file=Sandbox/Build-Sandbox.lua vs2022
vendor\premake\premake5.exe --file=BumCatcher/Build-BumCatcher.lua vs2022
POPD

PAUSE
