@ECHO OFF

PUSHD $~dp0\..\
vendor\FancyBuildSystem\bin\windows\premake5.exe --file=Build.lua vs2019
vendor\FancyBuildSystem\bin\windows\premake5.exe --file=Sandbox/Build-Sandbox.lua vs2019
vendor\FancyBuildSystem\bin\windows\premake5.exe --file=BumCatcher/Build-BumCatcher.lua vs2019
POPD

PAUSE
