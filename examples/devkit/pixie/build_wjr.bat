@set cpu=1802
@set speed=4000000
@set codeloc=0
@set stackloc=0x7EFF
@if not "%2"=="" set cpu=%2
@if not "%3"=="" set speed=%3
@if not "%4"=="" set codeloc=%4
@if not "%5"=="" set stackloc=%5
@echo pixie compile %1.c for cpu %cpu% speed %speed% codeloc %codeloc% stackloc %stackloc%

rem ..\..\..\bat\cabaPX %1 1802 1760900 0 0x0EFF "-DRES=32" "-DVIDMEM=0x0F00" "-D__VIP__" %1_32
c:\lcc42\bin\lcc.exe "-target=xr18CX" "-Wf-pixie" "-Wf-g,;" -DRES=32 -DVIDMEM=0x7F00 -D__VIP__ -S %1.c
c:\lcc42\bin\asw -cpu %cpu% -quiet -L -P -x -i c:\lcc42\include -D CPUSPEED=%speed% -D CODELOC=%codeloc% -D STACKLOC=%stackloc% -D LCCPX=1 %1.asm
c:\lcc42\bin\p2hex -r $-$ %1 %9 | more +2
