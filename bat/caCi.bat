
@echo compile %1.c for Cidelsa

@c:\lcc42\bin\lcc.exe "-target=xr18CX" "-Wf-g,;"  "-Wf-volatile" "-D __CIDELSA__" -S %2 %3 %4 %5 %6 %1.c 
@IF ERRORLEVEL 1 GOTO quit
@c:\lcc42\bin\copt c:\lcc42\include\lcc1806.opt -I %1.asm -O %1.oasm
@c:\lcc42\bin\asw -cpu 1802 -quiet -D LCCCX=42 -D CODELOC=0x000 -D DATALOC=0x8000 -D STACKLOC=0x83FF  -D CIDELSA -D NOFILLBSS -P -x -i ..\..\include -L %1.oasm
@c:\lcc42\bin\p2hex -r $-$ %1 a | more +2
@:quit
