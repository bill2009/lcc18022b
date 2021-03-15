
#include <nstdlib.h>
#include <comxio.h>
typedef unsigned char uint8_t;
struct CharacterStruct
{
	// character coordinates
	uint8_t _x;
	uint8_t _y;

	// _status decides whether the character is active
	uint8_t _status;

};

typedef struct CharacterStruct Character;
Character testchar;
Character * charptr=&testchar;
int main(unsigned char y)
{

 unsigned int A=36;
        unsigned int Q; /* the quotient */
	unsigned int R; /* the remainder */

	Q = ((A >> 1) + A) >> 1; /* Q = A*0.11 */
	Q = ((Q >> 4) + Q)     ; /* Q = A*0.110011 */
	Q = ((Q >> 8) + Q) >> 3; /* Q = A*0.00011001100110011 */
        /* either Q = A/10 or Q+1 = A/10 for all A < 534,890 */

	R = ((Q << 2) + Q) << 1;
	R = A - R; /* R = A - 10*Q */
	if (R >= 10) {
		R = R - 10;
		Q = Q + 1;
	}
        /* Q = A/10 for all A < 534,890 */
    printf("A=%i,Q=%i,R=%i\n",A,Q,R);
    return 0;

}

#include "nstdlib.c"
