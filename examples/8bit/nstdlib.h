//jan 7 trying a new vararg approach for printf
//feb 27, adding memcmp & memcpy
//mar 28 adding float support
//April 10 moving float constants to nstdlib.c, adding ifndef of nofloats around ftoa
//April 21 adding defs for putx memset
//May 26 adding declare for strlen
//21-01-15 update for lcc18028B
uint16_t strlen(char *str);
#define putchar putc
#define puts printstr
char * strcpy(char *, const char*);
void printstr(char*);
void putc(char);
char * itoa(int16_t, char *);
char * ltoa(int32_t, char *);
#ifndef nofloats
char * ftoa(float, char *,uint16_t);
#endif
void printf(char *, ...);
void exit(int16_t); //halt with a numeric error message
#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif
int16_tmemcmp(const void *Ptr1, const void *Ptr2, uint16_t Count);
void *memset(void *s, int16_t c, uint16_t n); //sets n bytes of memory at s to c
void* memcpy(void* dest, const void* src, uint16_t count);
char * dubdabx(int32_t, char *, int16_t);
void printlint(int32_t); //print a long integer


void putx(unsigned char x); //print an unsigned char as ascii hex
