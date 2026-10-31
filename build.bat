@echo off

rem Usage: build.bat <day> [release]
rem Builds with -Od by default, -O2 when "release" is given.
set opt=-Od
if /i "%2"=="release" set opt=-O2

set c_opts=-nologo -c -Zi %opt% -std:c++latest -EHsc -Fo./obj/ -TP -I./include/
set lnk_opts=/NOLOGO /DEBUG /LIBPATH:./libs/

cl %c_opts% ./src/main.cpp ./src/day%1.cpp -DDAY=%1

link.exe %lnk_opts% ./obj/main.obj ./obj/day%1.obj ^
    /OUT:aoc2026.exe
