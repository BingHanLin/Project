@echo off
rmdir /s /q build 
rmdir /s /q bin 

mkdir build
mkdir build

cd build

cmake .. -G "MinGW Makefiles"

if not defined ORIGINAL-PATH set ORIGINAL-PATH=%PATH%
set PATH=D:\msys32\mingw64\bin

mingw32-make

cd ..

set PATH=%ORIGINAL-PATH%
