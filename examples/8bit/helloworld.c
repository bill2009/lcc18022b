/*
   print the string "hello World!"
*/
//#define asm(x) //x
#include "stdint.h"
#include "nstdlib.h"
#define putc(x) out(7,x)
void main()
{
	printf("hello World!\n");
}
#include "nstdlib.c"
