/*
   print the string "hello World!"
*/
//#include <nstdlib.h>
#include <cpu1802spd4port7.h>
#define putc(x) out(7,x)
void printstr(char *ptr){
    while(*ptr){
		putc(*ptr++); //jan 29
		asm(" nop1806\n nop1806\n nop1806\n"); //17-03-09
	}
}
void printint(int s){ //print an integer
	char buffer[8]="noitoa()";
	//itoa(s,buffer);
	printstr(buffer);
}
void printf(char *pptr,...){ //limited implementation of printf
//								supports only c,d,s,x,l formats without width or other qualifiers
	unsigned char c,xord;
	register char* ptr=pptr; //try to save on loads/spills
	int argslot=0;	//used to align longs
	int * this=(int *)&pptr;
	this++; argslot++; //advance argument pointer and slot #
    while(*ptr) {
		c=*ptr; ptr++;
		if (c!='%'){
			putc(c);
		} else{
			c=*ptr;ptr++;
			switch (c){
				case 'i': case 'd':
					putc('.'); //printint(*this++);
					argslot+=1; //next argument slot
					break;
				case 's':
					printstr((char*) *this++);
					argslot+=1; //next argument slot
					break;
				default:
					putc('%');putc(c);
			} //switch
		} //%
	} //while
} //prtf
void main()
{
	printf("Hello %x\n",42);
}
//#include <nstdlib.c>
