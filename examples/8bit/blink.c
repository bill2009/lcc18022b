/*
    blink the q led
*/
#include "stdint.h"
#include "olduino.h"
void mysetqOn(){
	asm("	seq\n");
}
void main()
{
	while(1){
		mysetqOn();
		delay(250);
		setqOff();
		delay(750);
	}
}

#include "olduino.c" //for the delay routine
