/*
From Byte Magazine Sept 1981
A High Level Language Benchmark by Jim Gilbreath
Page 186
Listing 3: Implementation of the Sieve program, written in C.
*/


/* Eratosthenes Sieve Prime Number Program in C */
#define true 1
#define false 0
#define size 8190
#define sizepl 8191
#include <nstdlib.h>
#define nofloats 1
	char flags [sizepl] ;

void main ( ){
	int i , prime, k , count , iter ;

	printf("10 iterations\n");
	for(iter = 1;iter <= 10;iter ++) {
		count=0 ;
		for(i=0;i <= size;i ++)
			flags [i] =true;
		for(i = 0;i <= size;i ++) {
			if(flags[i]) {
				prime = i + i + 3;
				k = i + prime;
				while(k <= size) {
					flags[k] = false;
					k += prime;
				}
				count=count + 1;
			}
		}
	}
		printf ("\n%d primes" , count) ;
}
#include <nstdlib.c>
