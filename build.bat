@echo off

set c_opts=-nologo -c -Zi -std:c++latest -EHsc -Fo./obj/ -TP -I./include/
set lnk_opts=/NOLOGO /DEBUG /LIBPATH:./libs/

cl %c_opts% ./src/main.cpp ./src/day%1.cpp -DDAY=%1

link.exe %lnk_opts% ./obj/main.obj ./obj/day%1.obj ^
    /OUT:aoc2026.exe
