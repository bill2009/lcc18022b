
#include <stdint.h>
/*
 uint8_t invincibilityActive;

 uint8_t innerVerticalWallX;
 uint8_t innerVerticalWallY;
 uint8_t innerVerticalWallLength;

 uint8_t innerHorizontalWallX;
 uint8_t innerHorizontalWallY;
 uint8_t innerHorizontalWallLength;


uint8_t innerHorizontalWallReached(uint8_t x, uint8_t y)
{
    if ((y==innerHorizontalWallY) && (x >= innerHorizontalWallX))
		if (x<= (uint8_t)(innerHorizontalWallX + innerHorizontalWallLength-(uint8_t)1))
			return 1;
    return 0;
}


uint8_t innerVerticalWallReached(uint8_t x, uint8_t y)
{
    return (x==innerVerticalWallX) && (y >= innerVerticalWallY) && (y <= (innerVerticalWallY + innerVerticalWallLength-1));
}

uint8_t innerWallReached(uint8_t x, uint8_t y)
{
    return innerVerticalWallReached(x,y) || innerHorizontalWallReached(x,y);
}
*/
/*
char VERTICAL_BRICK_IMAGE;
void draw(	uint8_t x,	uint8_t y,	char * z){}
*/
void test(	uint8_t x,	uint8_t y,	uint8_t l){
	uint8_t i;
	for (i=l;i!=0;i--){
//		draw(x,y+i,&VERTICAL_BRICK_IMAGE);
	}
}
void main(){

}
