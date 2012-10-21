@echo off
call "%VS100COMNTOOLS%\vsvars32.bat"

SET ProgRoot=%ProgramFiles%
SET VLDLibDir=lib
IF NOT "%PROGRAMFILES(x86)%" == "" SET ProgRoot=%ProgramFiles(x86)%
IF NOT "%PROGRAMFILES(x86)%" == "" SET VLDLibDir=lib\Win32

@SET INCLUDE=%ProgRoot%\Visual Leak Detector\include;%INCLUDE%
@SET LIB=%ProgRoot%\Visual Leak Detector\%VLDLibDir%;%LIB%

msbuild /p:useenv=true dbl.vcxproj /p:Configuration=Debug
msbuild /p:useenv=true dbl.vcxproj /p:Configuration=DebugLib
msbuild /p:useenv=true dbl.vcxproj /p:Configuration=Release
msbuild /p:useenv=true dbl.vcxproj /p:Configuration=ReleaseLib
