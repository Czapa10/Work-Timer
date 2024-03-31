
@echo off
call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat" > nul

set CompilerFlags=-O2 -std:c++20 -MTd -nologo -fp:fast -fp:except- -Gm- -GR- -EHa- -Zo -Oi -W3 -D_CRT_SECURE_NO_WARNINGS -wd4201 -wd4100 -wd4189 -wd4505 -wd4127 -FC -Z7
set LinkerFlags= -incremental:no -opt:ref user32.lib

cls

cl %CompilerFlags% code/read_timer.cpp /link %LinkerFlags% | more
rem cl %CompilerFlags% code/new_timer.cpp /link %LinkerFlags% | more
rem cl %CompilerFlags% code/pause_timer.cpp /link %LinkerFlags% | more
rem cl %CompilerFlags% code/start_timer.cpp /link %LinkerFlags% | more
rem cl %CompilerFlags% code/timer_add_break.cpp /link %LinkerFlags% | more
