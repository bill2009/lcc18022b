/*
    terminal example by Marcel van Tongoren
*/

#include <stdint.h>
#include <nstdlib.h>

#include "devkit/video/pixie_video.h"

static const uint8_t shape_1[] =
{
	0x1C, 0x22, 0x26, 0x2A
};

static const uint8_t shape_2[] =
{
	0x32, 0x22, 0x1C, 0
};

void main(){
	initvideo();

	drawtile (0, 0, shape_1);
	drawtile (0, 1, shape_2);
	drawtile (2, 2, shape_1);
	while (1) {
	}
}
