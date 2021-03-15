
@echo compile %1.c for CIDELSA DRACO 32K

@c:\lcc42\bin\lcc.exe "-target=xr18CX" "-Wf-g,;"  "-Wf-volatile" -O "-Wp-D nofloats" "-Wa-D LCCNOLONG" -D__CIDELSA__ -D__DRACO__ -S %2 %3 %4 %5 %6 %1.c
@IF ERRORLEVEL 1 GOTO quit
@c:\lcc42\bin\copt c:\lcc42\include\lcc1806.opt -I %1.asm -O %1.oasm
@c:\lcc42\bin\asw -cpu 1802 -quiet -D NOFILLBSS=1 -D DATALOC=0x8000 -D CODELOC=0 -D STACKLOC=0x83ff -P -x -i ..\..\include -L %1.oasm
@c:\lcc42\bin\p2hex -r $-$ %1 %1_draco32k | more +2
@:quit

