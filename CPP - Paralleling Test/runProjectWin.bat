REM need to specified path to load dll files
@echo off
cd bin

if not defined ORIGINAL-PATH set ORIGINAL-PATH=%PATH%
set PATH=D:\msys32\mingw64\bin

MYAPP.exe

set PATH=%ORIGINAL-PATH% 

cd ..

