#include "nstdlib.h"
#define putc(c) putcomx(c,0x3302)
void putcomx(unsigned char c, unsigned int r13val){
	asm(" glo 12\n"
	    " lbr 0x2cd3\n");
}
void comparulator(unsigned long x,unsigned long y){
	unsigned int ix=x; unsigned int iy=y;
	printf("FOR %d:%d COMPARULATOR SEZ ",ix,iy);
	if (x<y) printf(" LT");
	if (x<=y) printf(" LE");
	if (x==y) printf(" EQ");
	if (x!=y) printf(" NE");
	if (x>y) printf(" GT");
	if (x>=y) printf(" GE");
	printf("\n\r");
}
void scomparulator(long x,long y){
	int ix=x; int iy=y;
	printf("FOR %d:%d COMPARULATOR SEZ ",ix,iy);
	if (x<y) printf(" LT");
	if (x<=y) printf(" LE");
	if (x==y) printf(" EQ");
	if (x!=y) printf(" NE");
	if (x>y) printf(" GT");
	if (x>=y) printf(" GE");
	printf("\n\r");
}
void main( ) {
scomparulator(10,-10);
scomparulator(-10,-1);
scomparulator(-11,-11);
scomparulator(-1,0);
scomparulator(0,0);
scomparulator(0,1);
scomparulator(1,1);
scomparulator(1,0);
comparulator(0,0);
comparulator(0,1);
comparulator(1,1);
comparulator(1,0);
}
#include "nstdlib.c"
