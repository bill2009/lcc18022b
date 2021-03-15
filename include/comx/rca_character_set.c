//
// *******************************************************************
// *** This software is copyright 2020 by Marcel van Tongeren      ***
// *** with support from Bill Rowe and Fabrizio Caruso.			   ***
// ***                                                             ***
// *** You have permission to use, modify, copy, and distribute    ***
// *** this software so long as this copyright notice is retained. ***
// *** This software may not be used in commercial applications    ***
// *** without express written permission from the author.         ***
// *******************************************************************
//

#include "comx\rca_character_set.h"
#include "comx\rca_vis_video.h"

#ifdef __CIDELSA__
void character_set(unsigned char number)
{
	number--;
	switch (number&3)
	{
		case 0:
			vis_text_color_mask = 0;
			shapechar(shape_space, 0x00, 1);
			shapechar(shape_space, 0x20, 1);
			shapechar(shape_numbers, 0x30, RCA_NUMBER_COLOR1+0xa);
			shapechar(shape_alpha, 0x41, RCA_ALPHA_COLOR1+0x1a);
		break;

		case 1:
			vis_text_color_mask = 0x80;
			shapechar(shape_space, 0x00, 1);
			shapechar(shape_space, 0x20, 1);
			shapechar(shape_numbers, 0x30, RCA_NUMBER_COLOR1+0xa);
			shapechar(shape_alpha, 0x41, RCA_ALPHA_COLOR1+0x1a);
			shapechar(shape_space, 0x80, 1);
			shapechar(shape_space, 0xA0, 1);
			shapechar(shape_numbers, 0xB0, RCA_NUMBER_COLOR2+0xa);
			shapechar(shape_alpha, 0xC1, RCA_ALPHA_COLOR2+0x1a);
		break;

		case 2:
		case 3:
			vis_text_color_mask = 0xC0;
			shapechar(shape_space, 0x00, 1);
			shapechar(shape_space, 0x20, 1);
			shapechar(shape_numbers, 0x30, RCA_NUMBER_COLOR1+0xa);
			shapechar(shape_alpha, 0x41, RCA_ALPHA_COLOR1+0x1a);
			shapechar(shape_space, 0x40, 1);
			shapechar(shape_space, 0x60, 1);
			shapechar(shape_numbers, 0x70, RCA_NUMBER_COLOR2+0xa);
			shapechar(shape_alpha, 0x81, RCA_ALPHA_COLOR2+0x1a);
			shapechar(shape_space, 0x80, 1);
			shapechar(shape_space, 0xA0, 1);
			shapechar(shape_numbers, 0xB0, RCA_NUMBER_COLOR3+0xa);
			shapechar(shape_alpha, 0xC1, RCA_ALPHA_COLOR3+0x1a);
			shapechar(shape_space, 0xC0, 1);
			shapechar(shape_space, 0xE0, 1);
			shapechar(shape_numbers, 0xF0, RCA_NUMBER_COLOR4+0xa);
			shapechar(shape_alpha, 0x01, RCA_ALPHA_COLOR4+0x1a);
		break;
	}
}
#endif
