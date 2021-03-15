/*
    terminal example by Marcel van Tongoren
*/

#include <stdint.h>
#include <nstdlib.h>

#define PIXIE_SPRITE

#include "devkit/video/pixie_video.h"
#include "devkit/input/joystick.h"
#include <stdint.h>
static const uint8_t shape_o[] =
{
	RES/8,
	0x60, 0x90, 0x90, 0x60, 0x60, 0x90, 0x90, 0x60, 0x60, 0x90, 0x90, 0x60, 0x60, 0x90, 0x90, 0x60
};

void main(){
	uint8_t x, y, delay, collision;
    unsigned char key;
    int8_t deltax,deltay;
	int lines = RES/8;

	initvideo();

	x = (int) ((X_SIZE-4)/2);
    y = (int) ((Y_SIZE-lines)/2);                   // Set x and y to middle of screen
	deltax=1,deltay=1;
	collision = drawsprite (x, y, shape_o);
	collision = 0;

	while (1) {
		key = get_stick();                  // get direction key value

		if (deltay<0 && y==0)
			deltay=1;
		if (y >= (Y_SIZE - lines) &&y>0)
			deltay=-1;
		if (deltax<0 && x==0)
			deltax=1;
		if ((x >= (X_SIZE - 4)) &&x>0)
			deltax=-1;
		if (key==MOVE_RIGHT)
			deltax=1;
		collision = drawsprite (x, y, shape_o);
		y+=deltay;x+=deltax;
		collision = drawsprite (x, y, shape_o);
		for (collision=0;collision<10;collision++)
			onems();
	}
}
