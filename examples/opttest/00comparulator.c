//#define asm(x)
typedef unsigned long uint16_t;
typedef unsigned int uint8_t;
typedef unsigned long long uint32_t;
typedef long int16_t;
typedef long long int32_t;
#include "nstdlib.h"
#include <cpu1802spd4port7.h>
#define putc(x) out(7,x)

void comparulator(uint32_t x,uint32_t y){
	uint16_t ix=x; uint16_t iy=y;
	printf("for %d:%d comparulator sez ",ix,iy);
	if (x<y) printf(" <");
	if (x<=y) printf(" <=");
	if (x==y) printf(" =");
	if (x!=y) printf(" !=");
	if (x>y) printf(" >");
	if (x>=y) printf(" >=");
	printf("\n");
}/*
void scomparulator(long x,long y){
	int16_t ix=x; int16_t iy=y;
	printf("for %d:%d scomparulator sez ",ix,iy);
	if (x<y) printf(" <");
	if (x<=y) printf(" <=");
	if (x==y) printf(" =");
	if (x!=y) printf(" !=");
	if (x>y) printf(" >");
	if (x>=y) printf(" >=");
	printf("\n");
}*/
void main( ) {
	/*
scomparulator(10,-10);
scomparulator(-10,-1);
scomparulator(-11,-11);
scomparulator(-1,0);
scomparulator(0,0);
scomparulator(0,1);
scomparulator(1,1);
scomparulator(1,0);*/
comparulator(0,0);
comparulator(0,1);
comparulator(1,1);
comparulator(1,0);

}
#include "nstdlib.c"
