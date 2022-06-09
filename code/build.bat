@echo off

IF NOT EXIST ..\build mkdir ..\build
pushd ..\build
cl ..\code\platform_win32.cpp 
popd
