@echo off
call "%VS100COMNTOOLS%\vsvars32.bat"
msbuild /t:clean dbl.vcxproj /p:Configuration=Debug
msbuild /t:clean dbl.vcxproj /p:Configuration=DebugLib
msbuild /t:clean dbl.vcxproj /p:Configuration=Release
msbuild /t:clean dbl.vcxproj /p:Configuration=ReleaseLib
