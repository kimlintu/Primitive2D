@echo off

set SDL_HEADERS_PATH="C:\Users\kimli\SDL2\include"

set SDL_LIBRARY_PATH="C:\Users\kimli\SDL2\lib\x64"

set LIBRARIES=SDL2.lib SDL2main.lib Shell32.lib

set COMPILER_OPTIONS=/external:W4 /external:I%SDL_HEADERS_PATH% /W4 /Zi

set LINKER_OPTIONS=/LIBPATH:%SDL_LIBRARY_PATH% /subsystem:windows 

set FILE_PATH="..\src"
set FILES=%FILE_PATH%\platform.cpp %FILE_PATH%\glad.c %FILE_PATH%\shader_program.cpp %FILE_PATH%\engine.cpp %FILE_PATH%\texture.cpp


IF NOT EXIST ..\build mkdir ..\build
pushd ..\build
cl %COMPILER_OPTIONS% %FILES% %LIBRARIES% /link %LINKER_OPTIONS%
popd
