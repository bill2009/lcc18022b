/*
    blink the q led
*/
//#include "nstdlib.h"
#include "olduino.h"
void mysetqOn(){
	asm("	seq\n");
}
void main()
{
	while(1){
		mysetqOn();
		delay(255);
		setqOff();
		delay(255);
	}
}

#include "olduino.c" //for the delay routine
