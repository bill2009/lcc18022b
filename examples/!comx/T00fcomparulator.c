#include "nstdlib.h"
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
void fcomparulator(float x,float y){
	//long lx=x; long ly=y;
	printf("for %fd:%fd fcomparulator sez ",x,y);
	if (x<y) printf(" <");
	if (x<=y) printf(" <=");
	if (x==y) printf(" =");
	if (x!=y) printf(" !=");
	if (x>y) printf(" >");
	if (x>=y) printf(" >=");
	printf("\n");
}
void comparulator(unsigned long x,unsigned long y){
	unsigned int ix=x; unsigned int iy=y;
	printf("for %ld:%ld lcomparulator sez ",x,y);
	if (x<y) printf(" <");
	if (x<=y) printf(" <=");
	if (x==y) printf(" =");
	if (x!=y) printf(" !=");
	if (x>y) printf(" >");
	if (x>=y) printf(" >=");
	printf("\n");
}
void scomparulator(long x,long y){
	int ix=x; int iy=y;
	printf("for %d:%d scomparulator sez ",ix,iy);
	if (x<y) printf(" <");
	if (x<=y) printf(" <=");
	if (x==y) printf(" =");
	if (x!=y) printf(" !=");
	if (x>y) printf(" >");
	if (x>=y) printf(" >=");
	printf("\n");
}
void main( ) {
fcomparulator(10,-10);
fcomparulator(10,10);
fcomparulator(-10,10);
fcomparulator(0,10);
fcomparulator(10,0);
fcomparulator(0,-1);
fcomparulator(-1,-10);
fcomparulator(10,-10);
fcomparulator(16777215,-10);
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
