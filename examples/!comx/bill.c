#include <nstdlib.h>
#include <cpu1802spd4port7.h>
#define putc(x) out(7,x)
char WALL_1_IMAGE, WALL_2_IMAGE;
char TWO_WINDOW_WALL_1_IMAGE='1', TWO_WINDOW_WALL_2_IMAGE='2';
int *building_images[] = {
    &WALL_1_IMAGE, &WALL_2_IMAGE,
    &TWO_WINDOW_WALL_1_IMAGE, &TWO_WINDOW_WALL_2_IMAGE,
    };
void foo(){}
int fortythree=43;
void main(){
	printf("TWO_WINDOW_WALL_1_IMAGE=%c\n",TWO_WINDOW_WALL_1_IMAGE);
}
#include <nstdlib.c>
