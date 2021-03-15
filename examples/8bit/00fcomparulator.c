#include "stdint.h"
#include "nstdlib.h"
#include <cpu1802spd4port7.h>
#define putc(x) out(7,x)
void fcomparulator(float x,float y){
	//long lx=x; long ly=y;
	asm(" MACEXP_OVR ON\n");
	printf("for %f:%f fcomparulator sez ",x,y);
	if (x==y) printf(" =");
	if (x<y) printf(" <");
	if (x<=y) printf(" <=");
	if (x!=y) printf(" !=");
	if (x>y) printf(" >");
	if (x>=y) printf(" >=");
	printf("\n");
}
void lcomparulator(uint32_t x,uint32_t y){
	//unsigned int ix=x; unsigned int iy=y;
	printf("for %ld:%ld lcomparulator sez ",x,y);
	if (x<y) printf(" <");
	if (x<=y) printf(" <=");
	if (x==y) printf(" =");
	if (x!=y) printf(" !=");
	if (x>y) printf(" >");
	if (x>=y) printf(" >=");
	printf("\n");
}
void scomparulator(uint32_t x,uint32_t y){
	uint16_t ix=x; uint16_t iy=y;
	printf("for %d:%d scomparulator sez ",ix,iy);
	if (x<y) printf(" <");
	if (x<=y) printf(" <=");
	if (x==y) printf(" =");
	if (x!=y) printf(" !=");
	if (x>y) printf(" >");
	if (x>=y) printf(" >=");
	asm(" MACEXP_OVR OFF\n");
	printf("\n");
}
void main( ) {
	printf("hello %f\n",4.2);
//fcomparulator(10.8,-10.7);
fcomparulator(10,10);
fcomparulator(-10,10);
fcomparulator(0,10);
fcomparulator(10,0);
fcomparulator(0,-1);
fcomparulator(-1,-10);
fcomparulator(10,-10);
fcomparulator((float)16777215.0,-10);
scomparulator(10,-10);
scomparulator(-10,-1);
scomparulator(-11,-11);
scomparulator(-1,0);
scomparulator(0,0);
scomparulator(0,1);
scomparulator(1,1);
scomparulator(1,0);
lcomparulator(0,0);
lcomparulator(0,1);
lcomparulator(1,1);
lcomparulator(1,0);
}
#include "nstdlib.c"
