typedef unsigned long uint16_t;
typedef unsigned int uint8_t;
#define asm(x)
//#include "nstdlib.h"
	void delay(uint16_t);
	void digitalWrite(uint8_t,uint8_t);
	int digitalRead(uint8_t); //nb, always reads ef3
	uint8_t PIN4=0; //used to save what's written to the output port
	#define LOW 0
	#define HIGH 1
	#define true 1
	#define false 0
void mysetqOn(){
	asm("	seq\n");
}
void main()
{
	while(1){
		mysetqOn();
		delay(500);
		setqOff();
		delay(500);
	}
}

void delay(uint16_t howlong){
	uint16_t i;
	for (i=1;i!=howlong;i++){
		oneMs();
	}
}
void olduinoincluder(){
	asm("\tinclude olduino.inc\n");
#ifdef __CPUSPEED4
	asm(" ifndef LCC1802CPUSPEED\n"); //for 4mhz 1802
	asm("LCC1802CPUSPEED set 4000000\n"); //for 4mhz 1802
	asm(" endif\n"); //for 4mhz 1802
#endif
}
