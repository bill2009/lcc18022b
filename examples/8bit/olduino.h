//nov 24 2013 added true and false, boolean, ifndef protection
//21-01-15 changed delay(int) to uint16_t for lcc18028B
#ifndef olduino_H
	#define olduino_H
	void delay(uint16_t);
	void digitalWrite(unsigned char,unsigned char);
	int digitalRead(unsigned char); //nb, always reads ef3
	unsigned char PIN4=0; //used to save what's written to the output port
	#define LOW 0
	#define HIGH 1
	#define true 1
	#define false 0
#endif
