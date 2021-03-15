#include "stdint.h"
#include "nstdlib.h"
#include <cpu1802spd4port7.h>
#define putc(x) out(7,x)
#include "defs.h"
#define check(EX) if(!(EX)) printf("check fail %s in %s line %d\n\007",(int16_t)#EX,(int16_t)__FILE__,(int16_t)__LINE__)
#define check2(EX1,EX2) check(EX1); check(EX2)
#define check3(EX1,EX2,EX3) check(EX1); check(EX2); check(EX3)
extern int16_t pigs;
int16_t pigs;
void l4(int16_t p1,int16_t p2, int16_t p3){
	printstr("level 4 checking parameter order - look for checkfail's\n");
	check3(p1==1, p2==2, p3==3);
	printstr("\nand we're done\n");
}

void l3(int16_t p1,int16_t p2, int16_t p3){
	l4(p3,p2,p1);
}

void l2(int16_t p1,int16_t p2, int16_t p3){
	l3(p3,p2,p1);
}


void l1(int16_t p1,int16_t p2, int16_t p3){
	l2(p3,p2,p1);
}

void main( ) {
l1(3,2,1);
}
#define nofloats 1
#include "nstdlib.c"
