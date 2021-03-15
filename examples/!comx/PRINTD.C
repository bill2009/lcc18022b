typedef unsigned char uint8_t;
typedef unsigned int uint16_t;
void _DISPLAY(int x, int y, uint8_t digit){}
void PRINTD(uint8_t x, uint8_t y, uint8_t length, uint16_t val)
{
	uint8_t i;
	uint8_t digits[5];

	for(i=0;i<length;++i)
	{
		digits[i] = (uint8_t) ((val)%10);
		val-= digits[i];
		val/=10;
		_DISPLAY(x+length-1-i,y, (uint8_t) (digits[i]));
	}
}
void main(){}
