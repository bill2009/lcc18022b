/*
    blink the q led
*/
#include "olduino.h"
void mysetqOn(){
	asm("	seq\n");
}
void main()
{
	unsigned char flippy=0;
	unsigned int i;
	while(1){
		for(i=0;i<3;i++){
			setqOn();
			delay(250);
			setqOff();
			delay(250);
		}
		delay(750);
	}
}

#include "olduino.c" //for the delay routine
