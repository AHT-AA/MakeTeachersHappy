rem if your compiler is not on PATH variable set path for compiler. for example

rem set path=C:\mingw64\bin



@del MakeTechersHappy.exe > NUL


gcc main.c -o MakeTechersHappy.exe -Wall -Wextra -mwindows -m32 -std=c11 -s -I .

@PAUSE
