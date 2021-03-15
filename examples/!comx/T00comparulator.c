#include "nstdlib.h"
#include <devkit\video\printf.h>
/*
#define putc(c) putupcomx(c)
void putcomx(unsigned char c, unsigned int r13val){
	asm(" glo 12\n"
	    " lbr 0x2cd3\n");
}
void putupcomx(unsigned char c){
	if ('\n' == c){//newline
		putcomx('\n',0x3302);putcomx('\r',0x3302);
	} else if (c>='a' & c<='z'){//lower case
		putcomx(c-0x20,0x3302);
	} else
		putcomx(c,0x3302);

}
*/
void ulcomparulator(unsigned long x,unsigned long y){
	//unsigned int ix=x; unsigned int iy=y;
	printf("for %ld:%ld ulcomparulator sez ",x,y);
	if (x<y) printf(" <");
	if (x<=y) printf(" <=");
	if (x==y) printf(" =");
	if (x!=y) printf(" !=");
	if (x>y) printf(" >");
	if (x>=y) printf(" >=");
	printf("\n");
}
void scomparulator(long x,long y){
	//int ix=x; int iy=y;
	printf("for %ld:%ld scomparulator sez ",x,y);
	if (x<y) printf(" <");
	if (x<=y) printf(" <=");
	if (x==y) printf(" =");
	if (x!=y) printf(" !=");
	if (x>y) printf(" >");
	if (x>=y) printf(" >=");
	printf("\n");
}
void main( ) {
printf("ABCD\n");/*
scomparulator(10,-10);
scomparulator(-10,-1);
scomparulator(-11,-11);
scomparulator(-1,0);
scomparulator(0,0);
scomparulator(0,1);
scomparulator(1,1);
scomparulator(1,0);
ulcomparulator(0,0);
ulcomparulator(0,1);
ulcomparulator(1,1);
ulcomparulator(1,0);*/
}
#include "nstdlib.c"
