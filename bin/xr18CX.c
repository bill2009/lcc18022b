//stmt: ASGNP2(addr,ADDP2(reg,consm))  "\tincm R%c,%1\n\tst2 R%c,%0; ASGNP2(addr,ADDP2(reg,consm))**\n"  1
/* This is the XR1CX machine description file for the 1802 COMX-35.
 * adapted from xr16.md to target the 1802 microprocessor
 * jan 28 beginning work on the Birthday Compiler
 * jan 31 trying out inline assembly patch
 * feb 7 2013, trying a new option -volatile to run without integer variables 
 * feb 8 refining -volatile to save reg  and use nointerrupts/interrupts
 * feb 12 work in progress after fixing inc bug
 * feb 13 archived before trying to change address mode macros
 * feb 14 version 2.3, changing to reserve/release for stack frame
 * feb 17 changing from putc.inc to IO1802.inc - This is the Birthday Compiler
 * feb 26 putting R prefix on registers in zext sext, initializing unused storage (change ds to db )
 * mar 3 archiving before optimization push
 * mar 3 first optimization push xr18no.md
 * mar 4 improved register saves in function()
 * mar 5 inproved "IF" instruction code by incorporating jzi2 logic.
 * mar 15 archiving before second optimzation push
 * mar 15 trying some instruction combination optimizations but it started blowing up
 * mar 17 add decm for small subtractions, removed unused inc(replaced by incm)
 * mar 20 archiving before floating point push.
 * mar 20 xr18fl.md is the first try at floating point
 * mar 28 basic fp operations work and conversion from float->long & vice versa
 * mar 28 freg2 now based on IREG to clear the register assignment problem
 * april 5 experimenting with operation chaining to eliminate excess instructions
 * april 10 trying to fix int to float conversion problem
 * april 19 putting missing 'r' in zext & sext templates
 * april 22-25 experimenting optimization, add EQU2(reg,con0), avoid release/reserve 0 bytes
 * archived before monkeying with register definitions because of allocation overlaps.
 * may 8 putting move(a) cost function back on register copy rules, remove extra asgni2 rule
 * may 15 setting intvar and inttmp back
 * may 16 trying r6 in intvar 
 * may 24 packaging for uploading.
 * June 27 correction to defconst so that signed numbers are printed with %d instead of %u 
 * June 27 trying %A instead of %a+%F to get offset as a single figure
 * Oct 2 xr18DH.md is dhrystone optimization
 * Oct 2 break up global inits into 1000 byte chunks to prevent assembler error
 * Oct 13 wholesale change of register names in rules from r* to R* also typo in a 4 byte compare
 * Oct 19 experimenting with stmt: EQI2(CVUI2(INDIRU1(indaddr)),CVUI2(INDIRU1(indaddr))) "surely not %0,%1,%a\n" 0
 * Oct 23 redecorating ASGNI2(addr,reg)/U2/P2 so the same peephole rule picks them up
 * Dec 23 removing *s from loadx2(reg) so a common rule can pick them up
 * Nov 12 2016 fixing case of epilog& prolog.inc file names
 * Feb 22 2017 xr18NW.md begins adaptation for the 1804/5/66
 * Mar 5 more dicking with 1806 adaptation adding 1 to stack offsets as:
 addr: ADDRFP2  "'O',sp,(%A+1)"
 addr: ADDRLP2  "'O',sp,(%A+1)"
   also in open code
 * Mar 14 cleanup, get rid of cretn6
 * Mar 28 cleanup of working copy on dell laptop special case of EQUI2 with 8 bit constant. function begin/end markers
 * 17-10-20 SCRT routines in lcc1802proloNW now use clean stack so stack offsets are the same for 1802/1804/5/6
 * 17-10-21 adding option -cpu1805 to put regs 4&5 into variable pool, set CPU value for assembler
 *          changed how -volatile set the regs
 * 18-01-25 corrected how -volatile set the regs
 * 18-02-26 attempting some combinations 
 * 18-03-04 another combination rule CVIU4(CVUI2(INDIRU1(addr)))
 * 18-04-13 xr18NW is the default, commented out display of register mask
 * 20-04-28 xr18CX adapted for the COMX-35
 * 20-05-13 trying to avoid promotion of unsigned char comparisons
 * 20-05-23 adding include lcc1802finale.inc to the end of the module - mostly to accommodate wrapup comx code in include/comx
 * 20-05-23 removed GHI 15 before cretn.  now included in cretn
 * 20-06-02 updated segment() to support data relocation in combination with prolog changes.
            note uninitialized globals are now defined with the globss macro and zeroing is done by the macro when needed
 * 20-07-31 rule optimizations for cross-shoot(J2020), tweaking of function start/end markers.
 * 20-11-30 optimizations prompted by liveness analysis LV-x
 * Portions copyright (C) 1999, 2000, Gray Research LLC.  All rights reserved.
 * Portions of this file are subject to the XSOC License Agreement;
 * you may not use them except in compliance with this Agreement.
 * See the LICENSE file.
 *
 * This work is derived from the original src/mips.md file in the
 * lcc4.1 distribution.  See the CPYRIGHT file.
 *
 * Registers    Use
 * R0-R1	register variables, 2X16 bit, 1X32 bit
 * R2           stack pointer
 * R3         	main PC
 * R4		call register
 * R5		return register
 * R6		return address register - and integer variable !
 * R7		register variable, 1X16 bit
 * R8-R11	scratch registers
 * R12-R13      first function arguments, return value for longs
 * R14		memory address temp/macro work register	
 * R15		return value register for shorts
 * Operator terminals generated by ops c=1 s=2 i=2 l=4 h=4 f=4 d=4 x=4 p=2
 *
 * Floating point is implemented!
saving reg:  CVUI2(INDIRU1(addr))     "\tld1 R%c,%0\n\tzExt R%c ;CVUI2(INDIRU1(addr)): *widen unsigned char to signed int (zero extend)\n" 1
*/

#define INTTMP 0x0f00	//8-11 are temporaries
#define PX3TMP 0x0e00	//9-11 are temporaries
#define PX3VARS 0x0140	//8,6 for variables
#define INTVAR 0x00C3	//0-1 6-7 can hold variables 
#define REGSVOLATILE 0x0003	//registers 0 and 1 not available for variables in a volatile environment
#define REGSPIXIE2 0x0083	//registers 7, 0 and 1 not available for variables in a pixie2 environment
#define REGS1805 0x0030	//registers 4-5 are available for variables when -cpu1805 specified
#define INTRET 0x8000	//reg 15 is return value
//I'm going to try using the long integer registers for floats
//#define FLTTMP 0x0000007f //float regs 0-6 for temps
//#define FLTVAR 0x00000000 //no float register variables
//#define FLTRET 0x00000080 //float reg 7 is for return values

#define NUM_IREGS       16
#define REG_RETVAL      15       /* R15: return value */
#define REG_FIRST_ARG   12       /* R12: first argument register */
#define REG_LAST_ARG    13       /* R13: last argument register */
#define REG_FIRST_TEMP  8       /* R8: first temp register */
#define SZ_REG_FIRST_TEMP "8"   /* R8: first temp register */
#define REG_LAST_TEMP   11       /* R11: last temp register */
#define REG_SP          2      /* R2, sp: stack pointer */
#define REG_SP_VOLATILE 2      /* R2, is sp if we're accommodating interrupts */
#define REG_RETADDR     6      /* R6: return address */
#define REG_MEMADDR	14	/* used by macros */
#define NUM_ARG_REGS    (REG_LAST_ARG - REG_FIRST_ARG + 1)

#define INT_CALLEE_SAVE INTVAR  //wjr jan 8 return address is saved in the call - save only the intvars

#define readsreg(p) \
        (generic((p)->op)==INDIR && (p)->kids[0]->op==VREG+P)
#define setsrc(d) ((d) && (d)->x.regnode && \
        (d)->x.regnode->set == src->x.regnode->set && \
        (d)->x.regnode->mask&src->x.regnode->mask)

#define relink(a, b) ((b)->x.prev = (a), (a)->x.next = (b))

#include "c.h"
#include <time.h>

#define NODEPTR_TYPE Node
#define OP_LABEL(p) ((p)->op)
#define LEFT_CHILD(p) ((p)->kids[0])
#define RIGHT_CHILD(p) ((p)->kids[1])
#define STATE_LABEL(p) ((p)->x.state)
static void address(Symbol, Symbol, long);
static void blkfetch(int, int, int, int);
static void blkloop(int, int, int, int, int, int[]);
static void blkstore(int, int, int, int);
static void defaddress(Symbol);
static void defconst(int, int, Value);
static void defstring(int, char *);
static void defsymbol(Symbol);
static void doarg(Node);
static void emit2(Node);
static void export(Symbol);
static void clobber(Node);
static void function(Symbol, Symbol [], Symbol [], int);
static void global(Symbol);
static void import(Symbol);
static void local(Symbol);
static void progbeg(int, char **);
static void progend(void);
static void segment(int);
static void space(int);
static void target(Node);
static int fp();
static int      bitcount       (unsigned);
static Symbol   argreg         (int, int, int, int, int);

static Symbol ireg[32], lreg[32], freg2[32], rp1p2; //int regs, long regs, floats, borth parameter regs together
static Symbol iregw, lregw, freg2w;
static int tmpregs[] = {1, REG_FIRST_TEMP+1, REG_FIRST_TEMP+2};
static Symbol blkreg;

static int gnum = 8;
static int wjrpixie=0;		//controls whether 1861 is accommodated or not.  implies volatile
static int wjrvolatile=0;	//controls whether interrupts are supported or not
static int wjrcpu1805=0;	//controls whether static regs 4&5 are available for variables, sets cpu value for assembly
static char* wjrenv=0;		//controls whether an environment include precedes the prolog
static int wjrfloats=0;		//indicates whether floats have been used or not
static int wjrMulInlineWeight=5; //weight of 5 allows multiplies to be done inline
static int reg_sp_actual=REG_SP;		//stack pointer is reg 2 by default
static int cseg;

/*
generated at Mon Mar  1 08:44:52 2021
by $Id: lburg.c,v 2.10 2002/03/08 18:45:21 drh Exp $
*/
static void _kids(NODEPTR_TYPE, int, NODEPTR_TYPE[]);
static void _label(NODEPTR_TYPE);
static int _rule(void*, int);

#define _stmt_NT 1
#define _reg_NT 2
#define _con0_NT 3
#define _con2_NT 4
#define _consm_NT 5
#define _con8bit_NT 6
#define _con_NT 7
#define _acon_NT 8
#define _addr_NT 9
#define _indaddr_NT 10
#define _ar_NT 11

static char *_ntname[] = {
	0,
	"stmt",
	"reg",
	"con0",
	"con2",
	"consm",
	"con8bit",
	"con",
	"acon",
	"addr",
	"indaddr",
	"ar",
	0
};

struct _state {
	short cost[12];
	struct {
		unsigned int _stmt:7;
		unsigned int _reg:8;
		unsigned int _con0:2;
		unsigned int _con2:3;
		unsigned int _consm:2;
		unsigned int _con8bit:2;
		unsigned int _con:3;
		unsigned int _acon:2;
		unsigned int _addr:3;
		unsigned int _indaddr:1;
		unsigned int _ar:2;
	} rule;
};

static short _nts_0[] = { 0 };
static short _nts_1[] = { _reg_NT, 0 };
static short _nts_2[] = { _con_NT, 0 };
static short _nts_3[] = { _reg_NT, _acon_NT, 0 };
static short _nts_4[] = { _acon_NT, 0 };
static short _nts_5[] = { _reg_NT, _consm_NT, 0 };
static short _nts_6[] = { _con0_NT, 0 };
static short _nts_7[] = { _addr_NT, 0 };
static short _nts_8[] = { _addr_NT, _reg_NT, 0 };
static short _nts_9[] = { _indaddr_NT, _reg_NT, 0 };
static short _nts_10[] = { _indaddr_NT, _acon_NT, 0 };
static short _nts_11[] = { _indaddr_NT, _indaddr_NT, 0 };
static short _nts_12[] = { _addr_NT, _acon_NT, 0 };
static short _nts_13[] = { _indaddr_NT, 0 };
static short _nts_14[] = { _reg_NT, _reg_NT, 0 };
static short _nts_15[] = { _reg_NT, _con2_NT, 0 };
static short _nts_16[] = { _con8bit_NT, _reg_NT, 0 };
static short _nts_17[] = { _reg_NT, _addr_NT, 0 };
static short _nts_18[] = { _reg_NT, _con_NT, 0 };
static short _nts_19[] = { _reg_NT, _con8bit_NT, 0 };
static short _nts_20[] = { _reg_NT, _con0_NT, 0 };
static short _nts_21[] = { _ar_NT, 0 };

static short *_nts[] = {
	0,	/* 0 */
	_nts_0,	/* 1 */
	_nts_0,	/* 2 */
	_nts_0,	/* 3 */
	_nts_0,	/* 4 */
	_nts_0,	/* 5 */
	_nts_0,	/* 6 */
	_nts_0,	/* 7 */
	_nts_0,	/* 8 */
	_nts_1,	/* 9 */
	_nts_1,	/* 10 */
	_nts_1,	/* 11 */
	_nts_1,	/* 12 */
	_nts_1,	/* 13 */
	_nts_1,	/* 14 */
	_nts_1,	/* 15 */
	_nts_1,	/* 16 */
	_nts_0,	/* 17 */
	_nts_0,	/* 18 */
	_nts_0,	/* 19 */
	_nts_0,	/* 20 */
	_nts_0,	/* 21 */
	_nts_0,	/* 22 */
	_nts_0,	/* 23 */
	_nts_0,	/* 24 */
	_nts_0,	/* 25 */
	_nts_0,	/* 26 */
	_nts_0,	/* 27 */
	_nts_0,	/* 28 */
	_nts_0,	/* 29 */
	_nts_0,	/* 30 */
	_nts_0,	/* 31 */
	_nts_0,	/* 32 */
	_nts_0,	/* 33 */
	_nts_0,	/* 34 */
	_nts_0,	/* 35 */
	_nts_1,	/* 36 */
	_nts_2,	/* 37 */
	_nts_0,	/* 38 */
	_nts_3,	/* 39 */
	_nts_3,	/* 40 */
	_nts_3,	/* 41 */
	_nts_4,	/* 42 */
	_nts_1,	/* 43 */
	_nts_1,	/* 44 */
	_nts_0,	/* 45 */
	_nts_0,	/* 46 */
	_nts_5,	/* 47 */
	_nts_5,	/* 48 */
	_nts_5,	/* 49 */
	_nts_5,	/* 50 */
	_nts_5,	/* 51 */
	_nts_5,	/* 52 */
	_nts_6,	/* 53 */
	_nts_4,	/* 54 */
	_nts_7,	/* 55 */
	_nts_8,	/* 56 */
	_nts_9,	/* 57 */
	_nts_10,	/* 58 */
	_nts_8,	/* 59 */
	_nts_11,	/* 60 */
	_nts_9,	/* 61 */
	_nts_9,	/* 62 */
	_nts_12,	/* 63 */
	_nts_12,	/* 64 */
	_nts_8,	/* 65 */
	_nts_8,	/* 66 */
	_nts_8,	/* 67 */
	_nts_8,	/* 68 */
	_nts_8,	/* 69 */
	_nts_8,	/* 70 */
	_nts_12,	/* 71 */
	_nts_8,	/* 72 */
	_nts_8,	/* 73 */
	_nts_13,	/* 74 */
	_nts_13,	/* 75 */
	_nts_7,	/* 76 */
	_nts_7,	/* 77 */
	_nts_7,	/* 78 */
	_nts_7,	/* 79 */
	_nts_7,	/* 80 */
	_nts_7,	/* 81 */
	_nts_7,	/* 82 */
	_nts_7,	/* 83 */
	_nts_7,	/* 84 */
	_nts_7,	/* 85 */
	_nts_7,	/* 86 */
	_nts_7,	/* 87 */
	_nts_7,	/* 88 */
	_nts_7,	/* 89 */
	_nts_7,	/* 90 */
	_nts_7,	/* 91 */
	_nts_14,	/* 92 */
	_nts_15,	/* 93 */
	_nts_14,	/* 94 */
	_nts_14,	/* 95 */
	_nts_14,	/* 96 */
	_nts_14,	/* 97 */
	_nts_14,	/* 98 */
	_nts_14,	/* 99 */
	_nts_14,	/* 100 */
	_nts_14,	/* 101 */
	_nts_16,	/* 102 */
	_nts_16,	/* 103 */
	_nts_14,	/* 104 */
	_nts_14,	/* 105 */
	_nts_14,	/* 106 */
	_nts_17,	/* 107 */
	_nts_14,	/* 108 */
	_nts_14,	/* 109 */
	_nts_17,	/* 110 */
	_nts_14,	/* 111 */
	_nts_14,	/* 112 */
	_nts_14,	/* 113 */
	_nts_14,	/* 114 */
	_nts_14,	/* 115 */
	_nts_14,	/* 116 */
	_nts_14,	/* 117 */
	_nts_14,	/* 118 */
	_nts_14,	/* 119 */
	_nts_14,	/* 120 */
	_nts_14,	/* 121 */
	_nts_14,	/* 122 */
	_nts_14,	/* 123 */
	_nts_14,	/* 124 */
	_nts_14,	/* 125 */
	_nts_14,	/* 126 */
	_nts_14,	/* 127 */
	_nts_14,	/* 128 */
	_nts_14,	/* 129 */
	_nts_14,	/* 130 */
	_nts_18,	/* 131 */
	_nts_14,	/* 132 */
	_nts_18,	/* 133 */
	_nts_18,	/* 134 */
	_nts_18,	/* 135 */
	_nts_18,	/* 136 */
	_nts_18,	/* 137 */
	_nts_18,	/* 138 */
	_nts_18,	/* 139 */
	_nts_18,	/* 140 */
	_nts_18,	/* 141 */
	_nts_18,	/* 142 */
	_nts_18,	/* 143 */
	_nts_18,	/* 144 */
	_nts_18,	/* 145 */
	_nts_18,	/* 146 */
	_nts_18,	/* 147 */
	_nts_18,	/* 148 */
	_nts_18,	/* 149 */
	_nts_18,	/* 150 */
	_nts_18,	/* 151 */
	_nts_18,	/* 152 */
	_nts_18,	/* 153 */
	_nts_14,	/* 154 */
	_nts_14,	/* 155 */
	_nts_14,	/* 156 */
	_nts_14,	/* 157 */
	_nts_14,	/* 158 */
	_nts_14,	/* 159 */
	_nts_14,	/* 160 */
	_nts_14,	/* 161 */
	_nts_18,	/* 162 */
	_nts_18,	/* 163 */
	_nts_18,	/* 164 */
	_nts_18,	/* 165 */
	_nts_18,	/* 166 */
	_nts_18,	/* 167 */
	_nts_18,	/* 168 */
	_nts_18,	/* 169 */
	_nts_1,	/* 170 */
	_nts_1,	/* 171 */
	_nts_1,	/* 172 */
	_nts_1,	/* 173 */
	_nts_1,	/* 174 */
	_nts_1,	/* 175 */
	_nts_1,	/* 176 */
	_nts_1,	/* 177 */
	_nts_1,	/* 178 */
	_nts_5,	/* 179 */
	_nts_1,	/* 180 */
	_nts_1,	/* 181 */
	_nts_1,	/* 182 */
	_nts_1,	/* 183 */
	_nts_7,	/* 184 */
	_nts_8,	/* 185 */
	_nts_14,	/* 186 */
	_nts_14,	/* 187 */
	_nts_14,	/* 188 */
	_nts_14,	/* 189 */
	_nts_1,	/* 190 */
	_nts_1,	/* 191 */
	_nts_1,	/* 192 */
	_nts_1,	/* 193 */
	_nts_1,	/* 194 */
	_nts_1,	/* 195 */
	_nts_1,	/* 196 */
	_nts_1,	/* 197 */
	_nts_7,	/* 198 */
	_nts_1,	/* 199 */
	_nts_1,	/* 200 */
	_nts_1,	/* 201 */
	_nts_1,	/* 202 */
	_nts_1,	/* 203 */
	_nts_1,	/* 204 */
	_nts_0,	/* 205 */
	_nts_1,	/* 206 */
	_nts_4,	/* 207 */
	_nts_4,	/* 208 */
	_nts_14,	/* 209 */
	_nts_14,	/* 210 */
	_nts_14,	/* 211 */
	_nts_14,	/* 212 */
	_nts_14,	/* 213 */
	_nts_14,	/* 214 */
	_nts_14,	/* 215 */
	_nts_14,	/* 216 */
	_nts_14,	/* 217 */
	_nts_14,	/* 218 */
	_nts_14,	/* 219 */
	_nts_14,	/* 220 */
	_nts_14,	/* 221 */
	_nts_14,	/* 222 */
	_nts_14,	/* 223 */
	_nts_14,	/* 224 */
	_nts_14,	/* 225 */
	_nts_14,	/* 226 */
	_nts_14,	/* 227 */
	_nts_14,	/* 228 */
	_nts_14,	/* 229 */
	_nts_14,	/* 230 */
	_nts_14,	/* 231 */
	_nts_14,	/* 232 */
	_nts_14,	/* 233 */
	_nts_14,	/* 234 */
	_nts_14,	/* 235 */
	_nts_19,	/* 236 */
	_nts_18,	/* 237 */
	_nts_18,	/* 238 */
	_nts_18,	/* 239 */
	_nts_18,	/* 240 */
	_nts_18,	/* 241 */
	_nts_18,	/* 242 */
	_nts_18,	/* 243 */
	_nts_18,	/* 244 */
	_nts_18,	/* 245 */
	_nts_18,	/* 246 */
	_nts_18,	/* 247 */
	_nts_18,	/* 248 */
	_nts_18,	/* 249 */
	_nts_18,	/* 250 */
	_nts_18,	/* 251 */
	_nts_18,	/* 252 */
	_nts_18,	/* 253 */
	_nts_18,	/* 254 */
	_nts_18,	/* 255 */
	_nts_18,	/* 256 */
	_nts_18,	/* 257 */
	_nts_18,	/* 258 */
	_nts_18,	/* 259 */
	_nts_20,	/* 260 */
	_nts_20,	/* 261 */
	_nts_20,	/* 262 */
	_nts_20,	/* 263 */
	_nts_20,	/* 264 */
	_nts_20,	/* 265 */
	_nts_18,	/* 266 */
	_nts_18,	/* 267 */
	_nts_20,	/* 268 */
	_nts_18,	/* 269 */
	_nts_18,	/* 270 */
	_nts_19,	/* 271 */
	_nts_14,	/* 272 */
	_nts_14,	/* 273 */
	_nts_14,	/* 274 */
	_nts_14,	/* 275 */
	_nts_14,	/* 276 */
	_nts_14,	/* 277 */
	_nts_0,	/* 278 */
	_nts_21,	/* 279 */
	_nts_21,	/* 280 */
	_nts_21,	/* 281 */
	_nts_21,	/* 282 */
	_nts_21,	/* 283 */
	_nts_21,	/* 284 */
	_nts_21,	/* 285 */
	_nts_1,	/* 286 */
	_nts_0,	/* 287 */
	_nts_1,	/* 288 */
	_nts_1,	/* 289 */
	_nts_1,	/* 290 */
	_nts_1,	/* 291 */
	_nts_1,	/* 292 */
	_nts_1,	/* 293 */
	_nts_1,	/* 294 */
	_nts_1,	/* 295 */
	_nts_1,	/* 296 */
	_nts_1,	/* 297 */
	_nts_1,	/* 298 */
	_nts_1,	/* 299 */
	_nts_1,	/* 300 */
	_nts_1,	/* 301 */
	_nts_14,	/* 302 */
	_nts_0,	/* 303 */
};

static char *_templates[] = {
/* 0 */	0,
/* 1 */	"# read register\n",	/* reg: INDIRI1(VREGP) */
/* 2 */	"# read register\n",	/* reg: INDIRU1(VREGP) */
/* 3 */	"# read register\n",	/* reg: INDIRI2(VREGP) */
/* 4 */	"# read register\n",	/* reg: INDIRU2(VREGP) */
/* 5 */	"# read register\n",	/* reg: INDIRF4(VREGP) */
/* 6 */	"# read register\n",	/* reg: INDIRI4(VREGP) */
/* 7 */	"# read register\n",	/* reg: INDIRP2(VREGP) */
/* 8 */	"# read register\n",	/* reg: INDIRU4(VREGP) */
/* 9 */	"# write register\n",	/* stmt: ASGNI1(VREGP,reg) */
/* 10 */	"# write register\n",	/* stmt: ASGNU1(VREGP,reg) */
/* 11 */	"# write register\n",	/* stmt: ASGNI2(VREGP,reg) */
/* 12 */	"# write register\n",	/* stmt: ASGNU2(VREGP,reg) */
/* 13 */	"# write register\n",	/* stmt: ASGNP2(VREGP,reg) */
/* 14 */	"# write register\n",	/* stmt: ASGNF4(VREGP,reg) */
/* 15 */	"# write register\n",	/* stmt: ASGNI4(VREGP,reg) */
/* 16 */	"# write register\n",	/* stmt: ASGNU4(VREGP,reg) */
/* 17 */	"0",	/* con0: CNSTF4 */
/* 18 */	"0",	/* con0: CNSTU2 */
/* 19 */	"0",	/* con0: CNSTI2 */
/* 20 */	"2",	/* con2: CNSTU2 */
/* 21 */	"2",	/* con2: CNSTI2 */
/* 22 */	"2",	/* con2: CNSTU4 */
/* 23 */	"2",	/* con2: CNSTI4 */
/* 24 */	"%a",	/* consm: CNSTU2 */
/* 25 */	"%a",	/* consm: CNSTI2 */
/* 26 */	"%a",	/* consm: CNSTP2 */
/* 27 */	"%a",	/* con8bit: CNSTI2 */
/* 28 */	"%a",	/* con8bit: CNSTU2 */
/* 29 */	"%a",	/* con: CNSTI1 */
/* 30 */	"%a",	/* con: CNSTU1 */
/* 31 */	"%a",	/* con: CNSTI2 */
/* 32 */	"%a",	/* con: CNSTU2 */
/* 33 */	"%a",	/* con: CNSTP2 */
/* 34 */	"\tldI4 R%c,%a ;loading a long integer constant\n",	/* reg: CNSTI4 */
/* 35 */	"\tldI4 R%c,%a ;loading a long unsigned constant\n",	/* reg: CNSTU4 */
/* 36 */	"",	/* stmt: reg */
/* 37 */	"%0",	/* acon: con */
/* 38 */	"%a",	/* acon: ADDRGP2 */
/* 39 */	"'O',R%0,(%1)",	/* addr: ADDI2(reg,acon) */
/* 40 */	"'O',R%0,(%1)",	/* addr: ADDU2(reg,acon) */
/* 41 */	"'O',R%0,(%1)",	/* addr: ADDP2(reg,acon) */
/* 42 */	"'D',(%0),0",	/* addr: acon */
/* 43 */	"'O',R%0,0",	/* addr: reg */
/* 44 */	"R%0",	/* indaddr: reg */
/* 45 */	"'O',sp,(%A+1)",	/* addr: ADDRFP2 */
/* 46 */	"'O',sp,(%A+1)",	/* addr: ADDRLP2 */
/* 47 */	"?\tcpy2 R%c,R%0 ;reg:ADDI2(consm,reg)\n\tincm R%c,%1\n",	/* reg: ADDI2(reg,consm) */
/* 48 */	"?\tcpy2 R%c,R%0\n\tincm R%c,%1\n",	/* reg: ADDU2(reg,consm) */
/* 49 */	"?\tcpy2 R%c,R%0\n\tincm R%c,%1\n",	/* reg: ADDP2(reg,consm) */
/* 50 */	"?\tcpy2 R%c,R%0	;SUBI2(reg,consm)\n\tdecm R%c,%1	;SUBI2(reg,consm)\n",	/* reg: SUBI2(reg,consm) */
/* 51 */	"?\tcpy2 R%c,R%0	;SUBU2(reg,consm)\n\tdecm R%c,%1	;SUBU2(reg,consm)\n",	/* reg: SUBU2(reg,consm) */
/* 52 */	"?\tcpy2 R%c,R%0\n\tdecm R%c,%1	; SUBP2(reg,consm)\n",	/* reg: SUBP2(reg,consm) */
/* 53 */	"\tld2z R%c; reg:con0\n",	/* reg: con0 */
/* 54 */	"\tldaD R%c,%0; reg:acon\n",	/* reg: acon */
/* 55 */	"\tldA2 R%c,%0; reg:addr\n",	/* reg: addr */
/* 56 */	"\tst1 R%1,%0; ASGNI1\n",	/* stmt: ASGNI1(addr,reg) */
/* 57 */	"\tstr1 R%1,%0; ASGNI1(indaddr,reg)	DH\n",	/* stmt: ASGNI1(indaddr,reg) */
/* 58 */	"\tstr1I %1,%0; ASGNU1(indaddr,acon)	DH\n",	/* stmt: ASGNU1(indaddr,acon) */
/* 59 */	"\tst1 R%1,%0; ASGNU1\n",	/* stmt: ASGNU1(addr,reg) */
/* 60 */	"\tldn %1\n\tstr %0; ASGNU1(indaddr,INDIRU1(indaddr))J2020-1\n",	/* stmt: ASGNU1(indaddr,INDIRU1(indaddr)) */
/* 61 */	"\tstr1 R%1,%0; ASGNU1(indaddr,reg)		DH*\n",	/* stmt: ASGNU1(indaddr,reg) */
/* 62 */	"\tstr1 R%1,%0; ASGNU1(indaddr,LOADU1(LOADU2(reg))) 18-03-21\n",	/* stmt: ASGNU1(indaddr,LOADU1(LOADU2(reg))) */
/* 63 */	"\tst1I %1,%0; ASGNU1(addr,acon) LV-1 \n",	/* stmt: ASGNU1(addr,acon) */
/* 64 */	"\tst2I %1,%0; ASGNI2(addr,acon)\n",	/* stmt: ASGNI2(addr,acon) */
/* 65 */	"\tst2 R%1,%0; ASGNI2(addr,reg)\n",	/* stmt: ASGNI2(addr,reg) */
/* 66 */	"\tst2 R%1,%0; ASGNI2(addr,LOADI2(reg)) 18-02-26\n",	/* stmt: ASGNI2(addr,LOADI2(reg)) */
/* 67 */	"\tst2 R%1,%0; ASGNU2(addr,reg)\n",	/* stmt: ASGNU2(addr,reg) */
/* 68 */	"\tst2 R%1,%0; ASGNU2(addr,LOADU2(reg)) 18-02-26\n",	/* stmt: ASGNU2(addr,LOADU2(reg)) */
/* 69 */	"\tst2 R%1,%0; ASGNI2(addr,LOADI2(reg)) 18-03-22\n",	/* stmt: ASGNI2(addr,LOADI2(reg)) */
/* 70 */	"\tst2 R%1,%0; ASGNP2(addr,reg)\n",	/* stmt: ASGNP2(addr,reg) */
/* 71 */	"\tst2i %1,%0; ASGNP2(addr,acon) LV-3\n",	/* stmt: ASGNP2(addr,acon) */
/* 72 */	"\tst4 R%1,%0\n",	/* stmt: ASGNI4(addr,reg) */
/* 73 */	"\tst4 R%1,%0; ASGNU4\n",	/* stmt: ASGNU4(addr,reg) */
/* 74 */	"\tldn1 R%c,%0;reg:  INDIRI1(indaddr)\n",	/* reg: INDIRI1(indaddr) */
/* 75 */	"\tldn1 R%c,%0;reg:  INDIRU1(indaddr)\n",	/* reg: INDIRU1(indaddr) */
/* 76 */	"\tld1 R%c,%0\n",	/* reg: INDIRI1(addr) */
/* 77 */	"\tld1 R%c,%0\n",	/* reg: INDIRU1(addr) */
/* 78 */	"\tld2 R%c,%0 ;reg:INDIRI2(addr)\n",	/* reg: INDIRI2(addr) */
/* 79 */	"\tld2 R%c,%0 ;reg:INDIRU2(addr)\n",	/* reg: INDIRU2(addr) */
/* 80 */	"\tld2 R%c,%0 ;reg:INDIRP2(addr)\n",	/* reg: INDIRP2(addr) */
/* 81 */	"\tld4 R%c,%0;reg:  INDIRI4(addr)\n",	/* reg: INDIRI4(addr) */
/* 82 */	"\tld4 R%c,%0;reg:  INDIRU4(addr)\n",	/* reg: INDIRU4(addr) */
/* 83 */	"\tld1 R%c,%0\n\tsExt R%c ;CVII2: widen signed char to signed int (sign extend)\n",	/* reg: CVII2(INDIRI1(addr)) */
/* 84 */	"\tld1 R%c,%0\n\tzExt R%c ;CVUU2: widen unsigned char to unsigned int (zero extend)\n",	/* reg: CVUU2(INDIRU1(addr)) */
/* 85 */	"\tld1 R%c,%0\n\tzExt R%c ;CVUI2(INDIRU1(addr)): *widen unsigned char to signed int (zero extend)J2020-2\n",	/* reg: CVUI2(INDIRU1(addr)) */
/* 86 */	"\tld1 R%c,%0\n\tsext R%c\n\tsext4 R%c ;CVII4: widen signed char to long int(sign extend)\n",	/* reg: CVII4(INDIRI1(addr)) */
/* 87 */	"\tld2 R%c,%0\n\tsext4 R%c ;CVII4: widen signed int to long int(sign extend)\n",	/* reg: CVII4(INDIRI2(addr)) */
/* 88 */	"\tld1 R%c,%0\n\tzext R%c\n\tzext4 R%c ;CVUU4: widen unsigned char to unsigned long(zero extend)\n",	/* reg: CVUU4(INDIRU1(addr)) */
/* 89 */	"\tld2 R%c,%0\n\tzext4 R%c ;CVUU4: widen unsigned int to unsigned long (zero extend)\n",	/* reg: CVUU4(INDIRU2(addr)) */
/* 90 */	"\tld1 R%c,%0\n\tzext R%c\n\tzext4 R%c ;CVUI4: widen unsigned char to signed long(zero extend)\n",	/* reg: CVUI4(INDIRU1(addr)) */
/* 91 */	"\tld2 R%c,%0\n\tzext4 R%c ;CVUI4: widen unsigned int to signed long (zero extend)\n",	/* reg: CVUI4(INDIRU2(addr)) */
/* 92 */	"\tCcall _divi2\n",	/* reg: DIVI2(reg,reg) */
/* 93 */	"\tshri4I R%c,1; DIVI4(reg,2)\n",	/* reg: DIVI4(reg,con2) */
/* 94 */	"\tCcall _divi4; DIVI4(reg,reg)\n",	/* reg: DIVI4(reg,reg) */
/* 95 */	"\tCcall _divu2\n",	/* reg: DIVU2(reg,reg) */
/* 96 */	"\tCcall _divu4\n",	/* reg: DIVU4(reg,reg) */
/* 97 */	"\tCcall _modi2\n",	/* reg: MODI2(reg,reg) */
/* 98 */	"\tCcall _modi4\n",	/* reg: MODI4(reg,reg) */
/* 99 */	"\tCcall _modu2\n",	/* reg: MODU2(reg,reg) */
/* 100 */	"\tCcall _modu4\n",	/* reg: MODU4(reg,reg) */
/* 101 */	"\tCcall _mulu2; MULI2(reg,reg)\n",	/* reg: MULI2(reg,reg) */
/* 102 */	"# MULI2(con8bit,reg) j2020-10\n",	/* reg: MULI2(con8bit,reg) */
/* 103 */	"# MULU2(con8bit,reg) j2020-10\n",	/* reg: MULU2(con8bit,reg) */
/* 104 */	"\tCcall _mulu4\n",	/* reg: MULI4(reg,reg) */
/* 105 */	"\tCcall _mulu2; MULU2(reg,reg)\n",	/* reg: MULU2(reg,reg) */
/* 106 */	"\tCcall _mulu4\n",	/* reg: MULU4(reg,reg) */
/* 107 */	"\talu2RRS R%c,R%0,%1,add,adc; ADDI2(r,INDIRI2(addr))	DH3\n",	/* reg: ADDI2(reg,INDIRI2(addr)) */
/* 108 */	"\talu2 R%c,R%0,R%1,add,adc; ADDI2(r,r)\n",	/* reg: ADDI2(reg,reg) */
/* 109 */	"\talu4 R%c,R%0,R%1,add,adc\n",	/* reg: ADDI4(reg,reg) */
/* 110 */	"\talu2RRS R%c,R%0,%1,add,adc; ADDI2(r,INDIRP2(addr))	DH3.1\n",	/* reg: ADDP2(reg,INDIRP2(addr)) */
/* 111 */	"\talu2 R%c,R%0,R%1,add,adc	;ADDP2(reg,reg)\n",	/* reg: ADDP2(reg,reg) */
/* 112 */	"\talu2 R%c,R%0,R%1,add,adc; ADDU2(r,r)\n",	/* reg: ADDU2(reg,reg) */
/* 113 */	"\talu4 R%c,R%0,R%1,add,adc\n",	/* reg: ADDU4(reg,reg) */
/* 114 */	"\talu2 R%c,R%0,R%1,and,and\n",	/* reg: BANDI2(reg,reg) */
/* 115 */	"\talu4 R%c,R%0,R%1,and,and\n",	/* reg: BANDI4(reg,reg) */
/* 116 */	"\talu2 R%c,R%0,R%1,or,or\n",	/* reg: BORI2(reg,reg) */
/* 117 */	"\talu4 R%c,R%0,R%1,or,or\n",	/* reg: BORI4(reg,reg) */
/* 118 */	"\talu2 R%c,R%0,R%1,xor,xor\n",	/* reg: BXORI2(reg,reg) */
/* 119 */	"\talu4 R%c,R%0,R%1,xor,xor\n",	/* reg: BXORI4(reg,reg) */
/* 120 */	"\talu2 R%c,R%0,R%1,and,and; BANDU2(reg,reg)\n",	/* reg: BANDU2(reg,reg) */
/* 121 */	"\talu4 R%c,R%0,R%1,and,and\n",	/* reg: BANDU4(reg,reg) */
/* 122 */	"\talu2 R%c,R%0,R%1,or,or\n",	/* reg: BORU2(reg,reg) */
/* 123 */	"\talu4 R%c,R%0,R%1,or,or\n",	/* reg: BORU4(reg,reg) */
/* 124 */	"\talu2 R%c,R%0,R%1,xor,xor\n",	/* reg: BXORU2(reg,reg) */
/* 125 */	"\talu4 R%c,R%0,R%1,xor,xor\n",	/* reg: BXORU4(reg,reg) */
/* 126 */	"\talu2 R%c,R%0,R%1,sm,smb\n",	/* reg: SUBI2(reg,reg) */
/* 127 */	"\talu4 R%c,R%0,R%1,sm,smb\n",	/* reg: SUBI4(reg,reg) */
/* 128 */	"\talu2 R%c,R%0,R%1,sm,smb\n",	/* reg: SUBP2(reg,reg) */
/* 129 */	"\talu2 R%c,R%0,R%1,sm,smb\n",	/* reg: SUBU2(reg,reg) */
/* 130 */	"\talu4 R%c,R%0,R%1,sm,smb\n",	/* reg: SUBU4(reg,reg) */
/* 131 */	"\talu2I R%c,R%0,%1,adi,adci; ADDI2(reg,con)\n",	/* reg: ADDI2(reg,con) */
/* 132 */	"\talu1 R%c,R%0,R%1,add,adci; ADDI2(CVUI2(reg),CVUI2(reg))J2020-3\n",	/* reg: ADDI2(CVUI2(reg),CVUI2(reg)) */
/* 133 */	"\talu4I R%c,R%0,%1,adi,adci\n",	/* reg: ADDI4(reg,con) */
/* 134 */	"\talu2I R%c,R%0,%1,adi,adci; ADDP2(reg,con)\n",	/* reg: ADDP2(reg,con) */
/* 135 */	"\talu2I R%c,R%0,%1,adi,adci; ADDU2(reg,con)\n",	/* reg: ADDU2(reg,con) */
/* 136 */	"\talu4I R%c,R%0,%1,adi,adci\n",	/* reg: ADDU4(reg,con) */
/* 137 */	"\talu2I R%c,R%0,%1,ani,ani\n	;removed ?\tcpy2 R%c,R%0\n",	/* reg: BANDI2(reg,con) */
/* 138 */	"?\tcpy4 R%c,R%0\n\talu4I R%c,R%0,%1,ani,ani\n",	/* reg: BANDI4(reg,con) */
/* 139 */	"\talu2I R%c,R%0,%1,ori,ori ;removed copy\n",	/* reg: BORI2(reg,con) */
/* 140 */	"?\tcpy4 R%c,R%0\n\talu4I R%c,R%0,%1,ori,ori\n",	/* reg: BORI4(reg,con) */
/* 141 */	"\talu2I R%c,R%0,%1,xri,xri ;removed copy\n",	/* reg: BXORI2(reg,con) */
/* 142 */	"?\tcpy4 R%c,R%0\n\talu2I R%c,R%0,%1,xri,xri\n",	/* reg: BXORI4(reg,con) */
/* 143 */	"\talu2I R%c,R%0,%1,ani,ani ;removed copy;BANDU2(reg,con)  \n",	/* reg: BANDU2(reg,con) */
/* 144 */	"?\tcpy4 R%c,R%0\n\talu4I R%c,R%0,%1,ani,ani\n",	/* reg: BANDU4(reg,con) */
/* 145 */	"\talu2I R%c,R%0,%1,ori,ori ;removed copy\n",	/* reg: BORU2(reg,con) */
/* 146 */	"?\tcpy4 R%c,R%0\n\talu4I R%c,R%0,%1,ori,ori\n",	/* reg: BORU4(reg,con) */
/* 147 */	"\talu2I R%c,R%0,%1,xri,xri ;removed copy\n",	/* reg: BXORU2(reg,con) */
/* 148 */	"?\tcpy4 R%c,R%0\n\talu2I R%c,R%0,%1,xri,xri\n",	/* reg: BXORU4(reg,con) */
/* 149 */	"\talu2I R%c,R%0,%1,smi,smbi\n",	/* reg: SUBI2(reg,con) */
/* 150 */	"\talu4I R%c,R%0,%1,smi,smbi\n",	/* reg: SUBI4(reg,con) */
/* 151 */	"\talu2I R%c,R%0,%1,smi,smbi\n",	/* reg: SUBP2(reg,con) */
/* 152 */	"\talu2I R%c,R%0,%1,smi,smbi\n",	/* reg: SUBU2(reg,con) */
/* 153 */	"\talu4I R%c,R%0,%1,smi,smbi\n",	/* reg: SUBU4(reg,con) */
/* 154 */	"?\tcpy2 R%c,R%0\n\tshl2R R%c,R%1; lshi2(r,r)\n",	/* reg: LSHI2(reg,reg) */
/* 155 */	"?\tcpy4 R%c,R%0\n\tshl4R R%c,R%1\n",	/* reg: LSHI4(reg,reg) */
/* 156 */	"?\tcpy2 R%c,R%0\n\tshl2R R%c,R%1; lshu2(r,r)\n",	/* reg: LSHU2(reg,reg) */
/* 157 */	"?\tcpy4 R%c,R%0\n\tshl4R R%c,R%1\n",	/* reg: LSHU4(reg,reg) */
/* 158 */	"?\tcpy2 R%c,R%0\n\tshrI2R R%c,R%1\n",	/* reg: RSHI2(reg,reg) */
/* 159 */	"?\tcpy4 R%c,R%0\n\tshRI4R R%c,R%1\n",	/* reg: RSHI4(reg,reg) */
/* 160 */	"?\tcpy2 R%c,R%0\n\tshrU2R R%c,R%1\n",	/* reg: RSHU2(reg,reg) */
/* 161 */	"?\tcpy4 R%c,R%0\n\tshRU4R R%c,R%1\n",	/* reg: RSHU4(reg,reg) */
/* 162 */	"?\tcpy2 R%c,R%0\n\tshl2I R%c,%1\n",	/* reg: LSHI2(reg,con) */
/* 163 */	"?\tcpy4 R%c,R%0\n\tshl4I R%c,%1; LSHI4(reg,con)\n",	/* reg: LSHI4(reg,con) */
/* 164 */	"?\tcpy2 R%c,R%0\n\tshl2I R%c,%1\n",	/* reg: LSHU2(reg,con) */
/* 165 */	"?\tcpy4 R%c,R%0\n\tshl4I R%c,%1; LSHU4(reg,con)\n",	/* reg: LSHU4(reg,con) */
/* 166 */	"?\tcpy2 R%c,R%0\n\tshrI2I R%c,%1\n",	/* reg: RSHI2(reg,con) */
/* 167 */	"?\tcpy4 R%c,R%0\n\tshrI4I R%c,%1\n",	/* reg: RSHI4(reg,con) */
/* 168 */	"?\tcpy2 R%c,R%0\n\tshrU2I R%c,%1\n",	/* reg: RSHU2(reg,con) */
/* 169 */	"?\tcpy4 R%c,R%0\n\tshrU4I R%c,%1\n",	/* reg: RSHU4(reg,con) */
/* 170 */	"\talu2I R%c,R%0,-1,xri,xri; was?\tcpy2 R%c,R%0+xor2I R%c,-1\n",	/* reg: BCOMI2(reg) */
/* 171 */	"\talu4I R%c,R%0,-1,xri,xri; was?\tcpy4 R%c,R%0+xor4I R%c,-1\n",	/* reg: BCOMI4(reg) */
/* 172 */	"\talu2I R%c,R%0,-1,xri,xri; was?\tcpy2 R%c,R%0+xor2I R%c,-1\n",	/* reg: BCOMU2(reg) */
/* 173 */	"\talu4I R%c,R%0,-1,xri,xri; was?\tcpy4 R%c,R%0+xor4I R%c,-1\n",	/* reg: BCOMU4(reg) */
/* 174 */	"\tnegI2 R%c,R%0 ;was alu2I R%c,R%0,0,sdi,sdbi\n",	/* reg: NEGI2(reg) */
/* 175 */	"\tnegI4 R%c,R%0 ;was alu4I R%c,R%0,0,sdi,sdbi\n",	/* reg: NEGI4(reg) */
/* 176 */	"?\tcpy1 R%c,R%0;LOADI1(reg)\n",	/* reg: LOADI1(reg) */
/* 177 */	"?\tcpy1 R%c,R%0;LOADU1(reg)\n",	/* reg: LOADU1(reg) */
/* 178 */	"?\tcpy2 R%c,R%0 ;LOADI2(reg)\n",	/* reg: LOADI2(reg) */
/* 179 */	"\tdecm R%c,%1	;LOADU2(SUBU2(reg,consm))\n",	/* reg: LOADU2(SUBU2(reg,consm)) */
/* 180 */	"?\tcpy2 R%c,R%0 ;LOADU2*(reg)\n",	/* reg: LOADU2(reg) */
/* 181 */	"?\tcpy4 R%c,R%0; LOADI4*\n",	/* reg: LOADI4(reg) */
/* 182 */	"?\tcpy2 R%c,R%0 ;LOADP2(reg)\n",	/* reg: LOADP2(reg) */
/* 183 */	"?\tcpy4 R%c,R%0; LOADU4(reg)\n",	/* reg: LOADU4(reg) */
/* 184 */	"\tld4 R%c,%0;INDIRF4(addr)\n",	/* reg: INDIRF4(addr) */
/* 185 */	"\tst4 R%1,%0; ASGNF4(addr,reg)\n",	/* stmt: ASGNF4(addr,reg) */
/* 186 */	"\tCcall fp_add ;ADDF4(reg,reg)\n",	/* reg: ADDF4(reg,reg) */
/* 187 */	"\tCcall fp_div ;DIVF4(reg,reg)\n",	/* reg: DIVF4(reg,reg) */
/* 188 */	"\tCcall fp_mul ;MULF4(reg,reg)\n",	/* reg: MULF4(reg,reg) */
/* 189 */	"\tCcall fp_sub ;SUBF4(reg,reg)\n",	/* reg: SUBF4(reg,reg) */
/* 190 */	"?\tcpy4 R%c,R%0; LOADU4(reg)\n",	/* reg: LOADF4(reg) */
/* 191 */	"\tnegf4 R%c,R%0; NEGF4(reg)",	/* reg: NEGF4(reg) */
/* 192 */	"?\tcpy1 R%c,R%0\n\tsExt R%c ;CVII2: widen signed char to signed int (sign extend)\n",	/* reg: CVII2(reg) */
/* 193 */	"?\tcpy1 R%c,R%0\n\tsExt R%c ;CVIU2: widen signed char to signed int (sign extend)\n",	/* reg: CVIU2(reg) */
/* 194 */	"?\tcpy1 R%c,R%0\n\tzExt R%c ;CVUI2(reg): widen unsigned char to signed int (zero extend)*\n",	/* reg: CVUI2(reg) */
/* 195 */	"?\tcpy1 R%c,R%0\n\tzExt R%c ;CVUU2: widen unsigned char to unsigned int (zero extend)*\n",	/* reg: CVUU2(reg) */
/* 196 */	"?\tcpy2 R%c,R%0\n\tsext4 R%c; CVII4\n",	/* reg: CVII4(reg) */
/* 197 */	"?\tcpy2 R%c,R%0\n\tsext4 R%c; *CVIU4(reg)\n",	/* reg: CVIU4(reg) */
/* 198 */	"\tld1 R%c,%0\n\tzExt R%c\n\tzExt4 R%c; CVIU4(INDIRU1(addr)):*HOORAY*widen unsigned char to long\n",	/* reg: CVIU4(CVUI2(INDIRU1(addr))) */
/* 199 */	"?\tcpy2 R%c,R%0\n\tzext4 R%c; CVUI4 jan 16\n",	/* reg: CVUI4(reg) */
/* 200 */	"?\tcpy2 R%c,R%0\n\tzext4 R%c ; CVUU4\n",	/* reg: CVUU4(reg) */
/* 201 */	"",	/* reg: CVFF4(reg) */
/* 202 */	"#\tcvif4 %c,%0 ; CVIF4(reg) convert int/long to float\n",	/* reg: CVIF4(reg) */
/* 203 */	"\tccall cvfi4; CVFI4(reg) convert float to long(should work for int's)\n",	/* reg: CVFI2(reg) */
/* 204 */	"\tccall cvfi4; CVFI4(reg) convert float to long\n",	/* reg: CVFI4(reg) */
/* 205 */	"%a:\n",	/* stmt: LABELV */
/* 206 */	"\tjumpv R%0; JUMPV(reg)\n",	/* stmt: JUMPV(reg) */
/* 207 */	"\tlbr %0\n",	/* stmt: JUMPV(acon) */
/* 208 */	"\txbr %0 ;**PIXIES**\n",	/* stmt: JUMPV(acon) */
/* 209 */	"\tjeqI2 R%0,R%1,%a; EQI2(reg,reg)\n",	/* stmt: EQI2(reg,reg) */
/* 210 */	"\tjeqI2 R%0,R%1,%a;EQU2(reg,reg)\n",	/* stmt: EQU2(reg,reg) */
/* 211 */	"\tjcI2 R%0,R%1,lbdf,%a; GE is flipped test from LT\n",	/* stmt: GEI2(reg,reg) */
/* 212 */	"\tjcU2 R%0,R%1,lbdf,%a; GE is flipped test from LT\n",	/* stmt: GEU2(reg,reg) */
/* 213 */	"\tjcI2 R%1,R%0,lbnf,%a ;GT is reversed operands from LT\n",	/* stmt: GTI2(reg,reg) */
/* 214 */	"\tjcU2 R%1,R%0,lbnf,%a ;GT same as LT but operands reversed\n",	/* stmt: GTU2(reg,reg) */
/* 215 */	"\tjcU1 R%1,R%0,lbdf,%a ;LE is flipped test & operands **opt 20**\n",	/* stmt: LEI2(CVUI2(reg),CVUI2(reg)) */
/* 216 */	"\tjcI2 R%1,R%0,lbdf,%a ;LE is flipped test & operands\n",	/* stmt: LEI2(reg,reg) */
/* 217 */	"\tjcU2 R%1,R%0,lbdf,%a ;LE is flipped test & operands\n",	/* stmt: LEU2(reg,reg) */
/* 218 */	"\tjcU1 R%0,R%1,lbnf,%a; LT=lbnf i.e. subtract B from A and jump if borrow **opt 20**\n",	/* stmt: LTI2(CVUI2(reg),CVUI2(reg)) */
/* 219 */	"\tjcI2 R%0,R%1,lbnf,%a; LT=lbnf i.e. subtract B from A and jump if borrow \n",	/* stmt: LTI2(reg,reg) */
/* 220 */	"\tjcU2 R%0,R%1,lbnf,%a; LT=lbnf i.e. subtract B from A and jump if borrow \n",	/* stmt: LTU2(reg,reg) */
/* 221 */	"\tjneU1 R%0,R%1,%a; NE - nopromo 20-05-12 wjr\n",	/* stmt: NEI2(CVUI2(reg),CVUI2(reg)) */
/* 222 */	"\tjneU2 R%0,R%1,%a; NE\n",	/* stmt: NEI2(reg,reg) */
/* 223 */	"\tjneU2 R%0,R%1,%a; NE\n",	/* stmt: NEU2(reg,reg) */
/* 224 */	"\tjeqI4 R%0,R%1,%a\n",	/* stmt: EQI4(reg,reg) */
/* 225 */	"\tjeqI4 R%0,R%1,%a\n",	/* stmt: EQU4(reg,reg) */
/* 226 */	"\tjcI4 R%0,R%1,lbdf,%a; GE is flipped test from LT\n",	/* stmt: GEI4(reg,reg) */
/* 227 */	"\tjcU4 R%0,R%1,lbdf,%a; GE is flipped test from LT\n",	/* stmt: GEU4(reg,reg) */
/* 228 */	"\tjcI4 R%1,R%0,lbnf,%a ;GT is reveresed operands from LT\n",	/* stmt: GTI4(reg,reg) */
/* 229 */	"\tjcU4 R%1,R%0,lbnf,%a ;GT same as LT but operands reversed\n",	/* stmt: GTU4(reg,reg) */
/* 230 */	"\tjcI4 R%1,R%0,lbdf,%a ;LE is flipped test & operands\n",	/* stmt: LEI4(reg,reg) */
/* 231 */	"\tjcU4 R%1,R%0,lbdf,%a ;LE is flipped test & operands\n",	/* stmt: LEU4(reg,reg) */
/* 232 */	"\tjcI4 R%0,R%1,lbnf,%a; LT=lbnf i.e. subtract B from A and jump if borrow \n",	/* stmt: LTI4(reg,reg) */
/* 233 */	"\tjcU4 R%0,R%1,lbnf,%a; LT=lbnf i.e. subtract B from A and jump if borrow \n",	/* stmt: LTU4(reg,reg) */
/* 234 */	"\tjneU4 R%0,R%1,%a; NE\n",	/* stmt: NEI4(reg,reg) */
/* 235 */	"\tjneU4 R%0,R%1,%a; NE\n",	/* stmt: NEU4(reg,reg) */
/* 236 */	"\tjeqU1I R%0,%1,%a;EQI2(CVUI2(reg),con8bit)**opt20**\n",	/* stmt: EQI2(CVUI2(reg),con8bit) */
/* 237 */	"\tjeqU2I R%0,%1,%a;EQI2(reg,con)\n",	/* stmt: EQI2(reg,con) */
/* 238 */	"\tjeqU4I R%0,%1,%a\n",	/* stmt: EQI4(reg,con) */
/* 239 */	"\tjeqU2I R%0,%1,%a;EQU2(reg,con)*\n",	/* stmt: EQU2(reg,con) */
/* 240 */	"\tjeqU4I R%0,%1,%a\n",	/* stmt: EQU4(reg,con) */
/* 241 */	"\tjcI1I R%0,%1,lbdf,%a; GE is flipped test from LT J2020-4\n",	/* stmt: GEI2(CVUI2(reg),con) */
/* 242 */	"\tjcI2I R%0,%1,lbdf,%a; GE is flipped test from LT\n",	/* stmt: GEI2(reg,con) */
/* 243 */	"\tjgeI4I R%0,%1,%a; GE\n",	/* stmt: GEI4(reg,con) */
/* 244 */	"\tjcI2I R%0,%1,lbdf,%a; GE is flipped test from LT\n",	/* stmt: GEU2(reg,con) */
/* 245 */	"\tjgeU4I R%0,%1,%a; GE\n",	/* stmt: GEU4(reg,con) */
/* 246 */	"\tjnI2I R%0,%1,lbnf,%a; GT reverse  the subtraction\n",	/* stmt: GTI2(reg,con) */
/* 247 */	"\tjgtI4I R%0,%1,%a\n",	/* stmt: GTI4(reg,con) */
/* 248 */	"\tjnU2I R%0,%1,lbnf,%a; GT reverse the subtraction J2020-5\n",	/* stmt: GTU2(reg,con) */
/* 249 */	"\tjgtU4I R%0,%1,%a\n",	/* stmt: GTU4(reg,con) */
/* 250 */	"\tjnI2I R%0,%1,lbdf,%a ;LEI2 %1 %0 %a; LE is flipped test & subtraction\n",	/* stmt: LEI2(reg,con) */
/* 251 */	"\tjleI4I R%0,%1,%a\n",	/* stmt: LEI4(reg,con) */
/* 252 */	"\tjnU2I R%0,%1,lbdf,%a ;LEU2 %1 %0 %a; LE is flipped test & subtraction\n",	/* stmt: LEU2(reg,con) */
/* 253 */	"\tjleU4I R%0,%1,%a\n",	/* stmt: LEU4(reg,con) */
/* 254 */	"\tjcI1I R%0,%1,lbnf,%a  ;LTI2=lbnf i.e. subtract immedB from A and jump if borrow - nopromo 20-05-12 J2020-6\n",	/* stmt: LTI2(CVUI2(reg),con) */
/* 255 */	"\tjcI2I R%0,%1,lbnf,%a  ;LT=lbnf i.e. subtract immedB from A and jump if borrow J2020-7\n",	/* stmt: LTI2(reg,con) */
/* 256 */	"\tjltI4I R%0,%1,%a\n",	/* stmt: LTI4(reg,con) */
/* 257 */	"\tjcU1I R%0,%1,lbnf,%a ;LTU2=lbnf i.e. subtract immedB from A and jump if borrow - nopromo 20-05-12 J2020-8\n",	/* stmt: LTU2(CVUI2(reg),con) */
/* 258 */	"\tjcU2I R%0,%1,lbnf,%a ;LT=lbnf i.e. subtract immedB from A and jump if borrow J2020-9\n",	/* stmt: LTU2(reg,con) */
/* 259 */	"\tjltU4I R%0,%1,%a\n",	/* stmt: LTU4(reg,con) */
/* 260 */	"\tjnzU1 R%0,%a; NEI2(CVUI2(reg),con0)\n",	/* stmt: NEI2(CVUI2(reg),con0) */
/* 261 */	"\tjnzU2 R%0,%a; NE 0\n",	/* stmt: NEI2(reg,con0) */
/* 262 */	"\tjzU1 R%0,%a; EQ 0 - nopromo 20-05-12\n",	/* stmt: EQI2(CVUI2(reg),con0) */
/* 263 */	"\tjzU2 R%0,%a; EQ I 0\n",	/* stmt: EQI2(reg,con0) */
/* 264 */	"\tjzU2 R%0,%a; EQ U 0\n",	/* stmt: EQU2(reg,con0) */
/* 265 */	"\tjzU1 R%0,%a; EQ 0 - nopromo 20-05-12\n",	/* stmt: EQU2(CVUI2(reg),con0) */
/* 266 */	"\tjneU2I R%0,%1,%a; NE\n",	/* stmt: NEI2(reg,con) */
/* 267 */	"\tjneI4I R%0,%1,%a; NE\n",	/* stmt: NEI4(reg,con) */
/* 268 */	"\tjnzU2 R%0,%a; NE 0 \n",	/* stmt: NEU2(reg,con0) */
/* 269 */	"\tjneU2I R%0,%1,%a; NE\n",	/* stmt: NEU2(reg,con) */
/* 270 */	"\tjneU4IR R%0,%1,%a; NE\n",	/* stmt: NEU4(reg,con) */
/* 271 */	"\tjneU1I R%0,%1,%a	; DH 4\n",	/* stmt: NEI2(CVUI2(reg),con8bit) */
/* 272 */	"\tjeqI4 R%0,R%1,%a; EQF4(reg,reg)\n",	/* stmt: EQF4(reg,reg) */
/* 273 */	"\tjcF4 R%1,R%0,lbnf,%a;GTF4(reg,reg) - reverse operands\n",	/* stmt: GTF4(reg,reg) */
/* 274 */	"\tjcF4 R%0,R%1,lbdf,%a;GEF4(reg,reg) - reverse test\n",	/* stmt: GEF4(reg,reg) */
/* 275 */	"\tjcF4 R%1,R%0,lbdf,%a; LEF4(reg,reg) - reverse test and operands\n",	/* stmt: LEF4(reg,reg) */
/* 276 */	"\tjcF4 R%0,R%1,lbnf,%a;LTF4(reg,reg)LT=lbnf i.e. subtract B from A and jump if borrow\n",	/* stmt: LTF4(reg,reg) */
/* 277 */	"\tjneU4 R%0,R%1,%a; NEF4(reg,reg)\n",	/* stmt: NEF4(reg,reg) */
/* 278 */	"%a",	/* ar: ADDRGP2 */
/* 279 */	"\tCcall %0;CALLF4(ar)\n",	/* reg: CALLF4(ar) */
/* 280 */	"\tCcall %0; CALLI2(ar)\n",	/* reg: CALLI2(ar) */
/* 281 */	"\tCcall %0\n",	/* reg: CALLI4(ar) */
/* 282 */	"\tCcall %0\n",	/* reg: CALLP2(ar) */
/* 283 */	"\tCcall %0;CALLU2(ar)*\n",	/* reg: CALLU2(ar) */
/* 284 */	"\tCcall %0\n",	/* reg: CALLU4(ar) */
/* 285 */	"\tCcall %0\n",	/* stmt: CALLV(ar) */
/* 286 */	"*R%0",	/* ar: reg */
/* 287 */	"%a",	/* ar: CNSTP2 */
/* 288 */	"# retn\n",	/* stmt: RETF4(reg) */
/* 289 */	"# retn\n",	/* stmt: RETI2(reg) */
/* 290 */	"# retn\n",	/* stmt: RETI4(reg) */
/* 291 */	"# retn\n",	/* stmt: RETU2(reg) */
/* 292 */	"# retn\n",	/* stmt: RETU4(reg) */
/* 293 */	"# retn\n",	/* stmt: RETP2(reg) */
/* 294 */	"# retn\n",	/* stmt: RETV(reg) */
/* 295 */	"# arg\n",	/* stmt: ARGF4(reg) */
/* 296 */	"# arg\n",	/* stmt: ARGI2(reg) */
/* 297 */	"# arg\n",	/* stmt: ARGI4(reg) */
/* 298 */	"# arg\n",	/* stmt: ARGP2(reg) */
/* 299 */	"# arg\n",	/* stmt: ARGU2(reg) */
/* 300 */	"# arg\n",	/* stmt: ARGU4(reg) */
/* 301 */	"# argb %0\n",	/* stmt: ARGB(INDIRB(reg)) */
/* 302 */	"\tblkcpy R%0,R%1,%a; ASGNB(reg,INDIRB(reg))\n",	/* stmt: ASGNB(reg,INDIRB(reg)) */
/* 303 */	"# emit inline assembly\n",	/* stmt: IASMV */
};

static char _isinstruction[] = {
/* 0 */	0,
/* 1 */	1,	/* # read register\n */
/* 2 */	1,	/* # read register\n */
/* 3 */	1,	/* # read register\n */
/* 4 */	1,	/* # read register\n */
/* 5 */	1,	/* # read register\n */
/* 6 */	1,	/* # read register\n */
/* 7 */	1,	/* # read register\n */
/* 8 */	1,	/* # read register\n */
/* 9 */	1,	/* # write register\n */
/* 10 */	1,	/* # write register\n */
/* 11 */	1,	/* # write register\n */
/* 12 */	1,	/* # write register\n */
/* 13 */	1,	/* # write register\n */
/* 14 */	1,	/* # write register\n */
/* 15 */	1,	/* # write register\n */
/* 16 */	1,	/* # write register\n */
/* 17 */	0,	/* 0 */
/* 18 */	0,	/* 0 */
/* 19 */	0,	/* 0 */
/* 20 */	0,	/* 2 */
/* 21 */	0,	/* 2 */
/* 22 */	0,	/* 2 */
/* 23 */	0,	/* 2 */
/* 24 */	0,	/* %a */
/* 25 */	0,	/* %a */
/* 26 */	0,	/* %a */
/* 27 */	0,	/* %a */
/* 28 */	0,	/* %a */
/* 29 */	0,	/* %a */
/* 30 */	0,	/* %a */
/* 31 */	0,	/* %a */
/* 32 */	0,	/* %a */
/* 33 */	0,	/* %a */
/* 34 */	1,	/* \tldI4 R%c,%a ;loading a long integer constant\n */
/* 35 */	1,	/* \tldI4 R%c,%a ;loading a long unsigned constant\n */
/* 36 */	0,	/*  */
/* 37 */	0,	/* %0 */
/* 38 */	0,	/* %a */
/* 39 */	0,	/* 'O',R%0,(%1) */
/* 40 */	0,	/* 'O',R%0,(%1) */
/* 41 */	0,	/* 'O',R%0,(%1) */
/* 42 */	0,	/* 'D',(%0),0 */
/* 43 */	0,	/* 'O',R%0,0 */
/* 44 */	0,	/* R%0 */
/* 45 */	0,	/* 'O',sp,(%A+1) */
/* 46 */	0,	/* 'O',sp,(%A+1) */
/* 47 */	1,	/* ?\tcpy2 R%c,R%0 ;reg:ADDI2(consm,reg)\n\tincm R%c,%1\n */
/* 48 */	1,	/* ?\tcpy2 R%c,R%0\n\tincm R%c,%1\n */
/* 49 */	1,	/* ?\tcpy2 R%c,R%0\n\tincm R%c,%1\n */
/* 50 */	1,	/* ?\tcpy2 R%c,R%0	;SUBI2(reg,consm)\n\tdecm R%c,%1	;SUBI2(reg,consm)\n */
/* 51 */	1,	/* ?\tcpy2 R%c,R%0	;SUBU2(reg,consm)\n\tdecm R%c,%1	;SUBU2(reg,consm)\n */
/* 52 */	1,	/* ?\tcpy2 R%c,R%0\n\tdecm R%c,%1	; SUBP2(reg,consm)\n */
/* 53 */	1,	/* \tld2z R%c; reg:con0\n */
/* 54 */	1,	/* \tldaD R%c,%0; reg:acon\n */
/* 55 */	1,	/* \tldA2 R%c,%0; reg:addr\n */
/* 56 */	1,	/* \tst1 R%1,%0; ASGNI1\n */
/* 57 */	1,	/* \tstr1 R%1,%0; ASGNI1(indaddr,reg)	DH\n */
/* 58 */	1,	/* \tstr1I %1,%0; ASGNU1(indaddr,acon)	DH\n */
/* 59 */	1,	/* \tst1 R%1,%0; ASGNU1\n */
/* 60 */	1,	/* \tldn %1\n\tstr %0; ASGNU1(indaddr,INDIRU1(indaddr))J2020-1\n */
/* 61 */	1,	/* \tstr1 R%1,%0; ASGNU1(indaddr,reg)		DH*\n */
/* 62 */	1,	/* \tstr1 R%1,%0; ASGNU1(indaddr,LOADU1(LOADU2(reg))) 18-03-21\n */
/* 63 */	1,	/* \tst1I %1,%0; ASGNU1(addr,acon) LV-1 \n */
/* 64 */	1,	/* \tst2I %1,%0; ASGNI2(addr,acon)\n */
/* 65 */	1,	/* \tst2 R%1,%0; ASGNI2(addr,reg)\n */
/* 66 */	1,	/* \tst2 R%1,%0; ASGNI2(addr,LOADI2(reg)) 18-02-26\n */
/* 67 */	1,	/* \tst2 R%1,%0; ASGNU2(addr,reg)\n */
/* 68 */	1,	/* \tst2 R%1,%0; ASGNU2(addr,LOADU2(reg)) 18-02-26\n */
/* 69 */	1,	/* \tst2 R%1,%0; ASGNI2(addr,LOADI2(reg)) 18-03-22\n */
/* 70 */	1,	/* \tst2 R%1,%0; ASGNP2(addr,reg)\n */
/* 71 */	1,	/* \tst2i %1,%0; ASGNP2(addr,acon) LV-3\n */
/* 72 */	1,	/* \tst4 R%1,%0\n */
/* 73 */	1,	/* \tst4 R%1,%0; ASGNU4\n */
/* 74 */	1,	/* \tldn1 R%c,%0;reg:  INDIRI1(indaddr)\n */
/* 75 */	1,	/* \tldn1 R%c,%0;reg:  INDIRU1(indaddr)\n */
/* 76 */	1,	/* \tld1 R%c,%0\n */
/* 77 */	1,	/* \tld1 R%c,%0\n */
/* 78 */	1,	/* \tld2 R%c,%0 ;reg:INDIRI2(addr)\n */
/* 79 */	1,	/* \tld2 R%c,%0 ;reg:INDIRU2(addr)\n */
/* 80 */	1,	/* \tld2 R%c,%0 ;reg:INDIRP2(addr)\n */
/* 81 */	1,	/* \tld4 R%c,%0;reg:  INDIRI4(addr)\n */
/* 82 */	1,	/* \tld4 R%c,%0;reg:  INDIRU4(addr)\n */
/* 83 */	1,	/* \tld1 R%c,%0\n\tsExt R%c ;CVII2: widen signed char to signed int (sign extend)\n */
/* 84 */	1,	/* \tld1 R%c,%0\n\tzExt R%c ;CVUU2: widen unsigned char to unsigned int (zero extend)\n */
/* 85 */	1,	/* \tld1 R%c,%0\n\tzExt R%c ;CVUI2(INDIRU1(addr)): *widen unsigned char to signed int (zero extend)J2020-2\n */
/* 86 */	1,	/* \tld1 R%c,%0\n\tsext R%c\n\tsext4 R%c ;CVII4: widen signed char to long int(sign extend)\n */
/* 87 */	1,	/* \tld2 R%c,%0\n\tsext4 R%c ;CVII4: widen signed int to long int(sign extend)\n */
/* 88 */	1,	/* \tld1 R%c,%0\n\tzext R%c\n\tzext4 R%c ;CVUU4: widen unsigned char to unsigned long(zero extend)\n */
/* 89 */	1,	/* \tld2 R%c,%0\n\tzext4 R%c ;CVUU4: widen unsigned int to unsigned long (zero extend)\n */
/* 90 */	1,	/* \tld1 R%c,%0\n\tzext R%c\n\tzext4 R%c ;CVUI4: widen unsigned char to signed long(zero extend)\n */
/* 91 */	1,	/* \tld2 R%c,%0\n\tzext4 R%c ;CVUI4: widen unsigned int to signed long (zero extend)\n */
/* 92 */	1,	/* \tCcall _divi2\n */
/* 93 */	1,	/* \tshri4I R%c,1; DIVI4(reg,2)\n */
/* 94 */	1,	/* \tCcall _divi4; DIVI4(reg,reg)\n */
/* 95 */	1,	/* \tCcall _divu2\n */
/* 96 */	1,	/* \tCcall _divu4\n */
/* 97 */	1,	/* \tCcall _modi2\n */
/* 98 */	1,	/* \tCcall _modi4\n */
/* 99 */	1,	/* \tCcall _modu2\n */
/* 100 */	1,	/* \tCcall _modu4\n */
/* 101 */	1,	/* \tCcall _mulu2; MULI2(reg,reg)\n */
/* 102 */	1,	/* # MULI2(con8bit,reg) j2020-10\n */
/* 103 */	1,	/* # MULU2(con8bit,reg) j2020-10\n */
/* 104 */	1,	/* \tCcall _mulu4\n */
/* 105 */	1,	/* \tCcall _mulu2; MULU2(reg,reg)\n */
/* 106 */	1,	/* \tCcall _mulu4\n */
/* 107 */	1,	/* \talu2RRS R%c,R%0,%1,add,adc; ADDI2(r,INDIRI2(addr))	DH3\n */
/* 108 */	1,	/* \talu2 R%c,R%0,R%1,add,adc; ADDI2(r,r)\n */
/* 109 */	1,	/* \talu4 R%c,R%0,R%1,add,adc\n */
/* 110 */	1,	/* \talu2RRS R%c,R%0,%1,add,adc; ADDI2(r,INDIRP2(addr))	DH3.1\n */
/* 111 */	1,	/* \talu2 R%c,R%0,R%1,add,adc	;ADDP2(reg,reg)\n */
/* 112 */	1,	/* \talu2 R%c,R%0,R%1,add,adc; ADDU2(r,r)\n */
/* 113 */	1,	/* \talu4 R%c,R%0,R%1,add,adc\n */
/* 114 */	1,	/* \talu2 R%c,R%0,R%1,and,and\n */
/* 115 */	1,	/* \talu4 R%c,R%0,R%1,and,and\n */
/* 116 */	1,	/* \talu2 R%c,R%0,R%1,or,or\n */
/* 117 */	1,	/* \talu4 R%c,R%0,R%1,or,or\n */
/* 118 */	1,	/* \talu2 R%c,R%0,R%1,xor,xor\n */
/* 119 */	1,	/* \talu4 R%c,R%0,R%1,xor,xor\n */
/* 120 */	1,	/* \talu2 R%c,R%0,R%1,and,and; BANDU2(reg,reg)\n */
/* 121 */	1,	/* \talu4 R%c,R%0,R%1,and,and\n */
/* 122 */	1,	/* \talu2 R%c,R%0,R%1,or,or\n */
/* 123 */	1,	/* \talu4 R%c,R%0,R%1,or,or\n */
/* 124 */	1,	/* \talu2 R%c,R%0,R%1,xor,xor\n */
/* 125 */	1,	/* \talu4 R%c,R%0,R%1,xor,xor\n */
/* 126 */	1,	/* \talu2 R%c,R%0,R%1,sm,smb\n */
/* 127 */	1,	/* \talu4 R%c,R%0,R%1,sm,smb\n */
/* 128 */	1,	/* \talu2 R%c,R%0,R%1,sm,smb\n */
/* 129 */	1,	/* \talu2 R%c,R%0,R%1,sm,smb\n */
/* 130 */	1,	/* \talu4 R%c,R%0,R%1,sm,smb\n */
/* 131 */	1,	/* \talu2I R%c,R%0,%1,adi,adci; ADDI2(reg,con)\n */
/* 132 */	1,	/* \talu1 R%c,R%0,R%1,add,adci; ADDI2(CVUI2(reg),CVUI2(reg))J2020-3\n */
/* 133 */	1,	/* \talu4I R%c,R%0,%1,adi,adci\n */
/* 134 */	1,	/* \talu2I R%c,R%0,%1,adi,adci; ADDP2(reg,con)\n */
/* 135 */	1,	/* \talu2I R%c,R%0,%1,adi,adci; ADDU2(reg,con)\n */
/* 136 */	1,	/* \talu4I R%c,R%0,%1,adi,adci\n */
/* 137 */	1,	/* \talu2I R%c,R%0,%1,ani,ani\n	;removed ?\tcpy2 R%c,R%0\n */
/* 138 */	1,	/* ?\tcpy4 R%c,R%0\n\talu4I R%c,R%0,%1,ani,ani\n */
/* 139 */	1,	/* \talu2I R%c,R%0,%1,ori,ori ;removed copy\n */
/* 140 */	1,	/* ?\tcpy4 R%c,R%0\n\talu4I R%c,R%0,%1,ori,ori\n */
/* 141 */	1,	/* \talu2I R%c,R%0,%1,xri,xri ;removed copy\n */
/* 142 */	1,	/* ?\tcpy4 R%c,R%0\n\talu2I R%c,R%0,%1,xri,xri\n */
/* 143 */	1,	/* \talu2I R%c,R%0,%1,ani,ani ;removed copy;BANDU2(reg,con)  \n */
/* 144 */	1,	/* ?\tcpy4 R%c,R%0\n\talu4I R%c,R%0,%1,ani,ani\n */
/* 145 */	1,	/* \talu2I R%c,R%0,%1,ori,ori ;removed copy\n */
/* 146 */	1,	/* ?\tcpy4 R%c,R%0\n\talu4I R%c,R%0,%1,ori,ori\n */
/* 147 */	1,	/* \talu2I R%c,R%0,%1,xri,xri ;removed copy\n */
/* 148 */	1,	/* ?\tcpy4 R%c,R%0\n\talu2I R%c,R%0,%1,xri,xri\n */
/* 149 */	1,	/* \talu2I R%c,R%0,%1,smi,smbi\n */
/* 150 */	1,	/* \talu4I R%c,R%0,%1,smi,smbi\n */
/* 151 */	1,	/* \talu2I R%c,R%0,%1,smi,smbi\n */
/* 152 */	1,	/* \talu2I R%c,R%0,%1,smi,smbi\n */
/* 153 */	1,	/* \talu4I R%c,R%0,%1,smi,smbi\n */
/* 154 */	1,	/* ?\tcpy2 R%c,R%0\n\tshl2R R%c,R%1; lshi2(r,r)\n */
/* 155 */	1,	/* ?\tcpy4 R%c,R%0\n\tshl4R R%c,R%1\n */
/* 156 */	1,	/* ?\tcpy2 R%c,R%0\n\tshl2R R%c,R%1; lshu2(r,r)\n */
/* 157 */	1,	/* ?\tcpy4 R%c,R%0\n\tshl4R R%c,R%1\n */
/* 158 */	1,	/* ?\tcpy2 R%c,R%0\n\tshrI2R R%c,R%1\n */
/* 159 */	1,	/* ?\tcpy4 R%c,R%0\n\tshRI4R R%c,R%1\n */
/* 160 */	1,	/* ?\tcpy2 R%c,R%0\n\tshrU2R R%c,R%1\n */
/* 161 */	1,	/* ?\tcpy4 R%c,R%0\n\tshRU4R R%c,R%1\n */
/* 162 */	1,	/* ?\tcpy2 R%c,R%0\n\tshl2I R%c,%1\n */
/* 163 */	1,	/* ?\tcpy4 R%c,R%0\n\tshl4I R%c,%1; LSHI4(reg,con)\n */
/* 164 */	1,	/* ?\tcpy2 R%c,R%0\n\tshl2I R%c,%1\n */
/* 165 */	1,	/* ?\tcpy4 R%c,R%0\n\tshl4I R%c,%1; LSHU4(reg,con)\n */
/* 166 */	1,	/* ?\tcpy2 R%c,R%0\n\tshrI2I R%c,%1\n */
/* 167 */	1,	/* ?\tcpy4 R%c,R%0\n\tshrI4I R%c,%1\n */
/* 168 */	1,	/* ?\tcpy2 R%c,R%0\n\tshrU2I R%c,%1\n */
/* 169 */	1,	/* ?\tcpy4 R%c,R%0\n\tshrU4I R%c,%1\n */
/* 170 */	1,	/* \talu2I R%c,R%0,-1,xri,xri; was?\tcpy2 R%c,R%0+xor2I R%c,-1\n */
/* 171 */	1,	/* \talu4I R%c,R%0,-1,xri,xri; was?\tcpy4 R%c,R%0+xor4I R%c,-1\n */
/* 172 */	1,	/* \talu2I R%c,R%0,-1,xri,xri; was?\tcpy2 R%c,R%0+xor2I R%c,-1\n */
/* 173 */	1,	/* \talu4I R%c,R%0,-1,xri,xri; was?\tcpy4 R%c,R%0+xor4I R%c,-1\n */
/* 174 */	1,	/* \tnegI2 R%c,R%0 ;was alu2I R%c,R%0,0,sdi,sdbi\n */
/* 175 */	1,	/* \tnegI4 R%c,R%0 ;was alu4I R%c,R%0,0,sdi,sdbi\n */
/* 176 */	1,	/* ?\tcpy1 R%c,R%0;LOADI1(reg)\n */
/* 177 */	1,	/* ?\tcpy1 R%c,R%0;LOADU1(reg)\n */
/* 178 */	1,	/* ?\tcpy2 R%c,R%0 ;LOADI2(reg)\n */
/* 179 */	1,	/* \tdecm R%c,%1	;LOADU2(SUBU2(reg,consm))\n */
/* 180 */	1,	/* ?\tcpy2 R%c,R%0 ;LOADU2*(reg)\n */
/* 181 */	1,	/* ?\tcpy4 R%c,R%0; LOADI4*\n */
/* 182 */	1,	/* ?\tcpy2 R%c,R%0 ;LOADP2(reg)\n */
/* 183 */	1,	/* ?\tcpy4 R%c,R%0; LOADU4(reg)\n */
/* 184 */	1,	/* \tld4 R%c,%0;INDIRF4(addr)\n */
/* 185 */	1,	/* \tst4 R%1,%0; ASGNF4(addr,reg)\n */
/* 186 */	1,	/* \tCcall fp_add ;ADDF4(reg,reg)\n */
/* 187 */	1,	/* \tCcall fp_div ;DIVF4(reg,reg)\n */
/* 188 */	1,	/* \tCcall fp_mul ;MULF4(reg,reg)\n */
/* 189 */	1,	/* \tCcall fp_sub ;SUBF4(reg,reg)\n */
/* 190 */	1,	/* ?\tcpy4 R%c,R%0; LOADU4(reg)\n */
/* 191 */	0,	/* \tnegf4 R%c,R%0; NEGF4(reg) */
/* 192 */	1,	/* ?\tcpy1 R%c,R%0\n\tsExt R%c ;CVII2: widen signed char to signed int (sign extend)\n */
/* 193 */	1,	/* ?\tcpy1 R%c,R%0\n\tsExt R%c ;CVIU2: widen signed char to signed int (sign extend)\n */
/* 194 */	1,	/* ?\tcpy1 R%c,R%0\n\tzExt R%c ;CVUI2(reg): widen unsigned char to signed int (zero extend)*\n */
/* 195 */	1,	/* ?\tcpy1 R%c,R%0\n\tzExt R%c ;CVUU2: widen unsigned char to unsigned int (zero extend)*\n */
/* 196 */	1,	/* ?\tcpy2 R%c,R%0\n\tsext4 R%c; CVII4\n */
/* 197 */	1,	/* ?\tcpy2 R%c,R%0\n\tsext4 R%c; *CVIU4(reg)\n */
/* 198 */	1,	/* \tld1 R%c,%0\n\tzExt R%c\n\tzExt4 R%c; CVIU4(INDIRU1(addr)):*HOORAY*widen unsigned char to long\n */
/* 199 */	1,	/* ?\tcpy2 R%c,R%0\n\tzext4 R%c; CVUI4 jan 16\n */
/* 200 */	1,	/* ?\tcpy2 R%c,R%0\n\tzext4 R%c ; CVUU4\n */
/* 201 */	0,	/*  */
/* 202 */	1,	/* #\tcvif4 %c,%0 ; CVIF4(reg) convert int/long to float\n */
/* 203 */	1,	/* \tccall cvfi4; CVFI4(reg) convert float to long(should work for int's)\n */
/* 204 */	1,	/* \tccall cvfi4; CVFI4(reg) convert float to long\n */
/* 205 */	1,	/* %a:\n */
/* 206 */	1,	/* \tjumpv R%0; JUMPV(reg)\n */
/* 207 */	1,	/* \tlbr %0\n */
/* 208 */	1,	/* \txbr %0 ;**PIXIES**\n */
/* 209 */	1,	/* \tjeqI2 R%0,R%1,%a; EQI2(reg,reg)\n */
/* 210 */	1,	/* \tjeqI2 R%0,R%1,%a;EQU2(reg,reg)\n */
/* 211 */	1,	/* \tjcI2 R%0,R%1,lbdf,%a; GE is flipped test from LT\n */
/* 212 */	1,	/* \tjcU2 R%0,R%1,lbdf,%a; GE is flipped test from LT\n */
/* 213 */	1,	/* \tjcI2 R%1,R%0,lbnf,%a ;GT is reversed operands from LT\n */
/* 214 */	1,	/* \tjcU2 R%1,R%0,lbnf,%a ;GT same as LT but operands reversed\n */
/* 215 */	1,	/* \tjcU1 R%1,R%0,lbdf,%a ;LE is flipped test & operands **opt 20**\n */
/* 216 */	1,	/* \tjcI2 R%1,R%0,lbdf,%a ;LE is flipped test & operands\n */
/* 217 */	1,	/* \tjcU2 R%1,R%0,lbdf,%a ;LE is flipped test & operands\n */
/* 218 */	1,	/* \tjcU1 R%0,R%1,lbnf,%a; LT=lbnf i.e. subtract B from A and jump if borrow **opt 20**\n */
/* 219 */	1,	/* \tjcI2 R%0,R%1,lbnf,%a; LT=lbnf i.e. subtract B from A and jump if borrow \n */
/* 220 */	1,	/* \tjcU2 R%0,R%1,lbnf,%a; LT=lbnf i.e. subtract B from A and jump if borrow \n */
/* 221 */	1,	/* \tjneU1 R%0,R%1,%a; NE - nopromo 20-05-12 wjr\n */
/* 222 */	1,	/* \tjneU2 R%0,R%1,%a; NE\n */
/* 223 */	1,	/* \tjneU2 R%0,R%1,%a; NE\n */
/* 224 */	1,	/* \tjeqI4 R%0,R%1,%a\n */
/* 225 */	1,	/* \tjeqI4 R%0,R%1,%a\n */
/* 226 */	1,	/* \tjcI4 R%0,R%1,lbdf,%a; GE is flipped test from LT\n */
/* 227 */	1,	/* \tjcU4 R%0,R%1,lbdf,%a; GE is flipped test from LT\n */
/* 228 */	1,	/* \tjcI4 R%1,R%0,lbnf,%a ;GT is reveresed operands from LT\n */
/* 229 */	1,	/* \tjcU4 R%1,R%0,lbnf,%a ;GT same as LT but operands reversed\n */
/* 230 */	1,	/* \tjcI4 R%1,R%0,lbdf,%a ;LE is flipped test & operands\n */
/* 231 */	1,	/* \tjcU4 R%1,R%0,lbdf,%a ;LE is flipped test & operands\n */
/* 232 */	1,	/* \tjcI4 R%0,R%1,lbnf,%a; LT=lbnf i.e. subtract B from A and jump if borrow \n */
/* 233 */	1,	/* \tjcU4 R%0,R%1,lbnf,%a; LT=lbnf i.e. subtract B from A and jump if borrow \n */
/* 234 */	1,	/* \tjneU4 R%0,R%1,%a; NE\n */
/* 235 */	1,	/* \tjneU4 R%0,R%1,%a; NE\n */
/* 236 */	1,	/* \tjeqU1I R%0,%1,%a;EQI2(CVUI2(reg),con8bit)**opt20**\n */
/* 237 */	1,	/* \tjeqU2I R%0,%1,%a;EQI2(reg,con)\n */
/* 238 */	1,	/* \tjeqU4I R%0,%1,%a\n */
/* 239 */	1,	/* \tjeqU2I R%0,%1,%a;EQU2(reg,con)*\n */
/* 240 */	1,	/* \tjeqU4I R%0,%1,%a\n */
/* 241 */	1,	/* \tjcI1I R%0,%1,lbdf,%a; GE is flipped test from LT J2020-4\n */
/* 242 */	1,	/* \tjcI2I R%0,%1,lbdf,%a; GE is flipped test from LT\n */
/* 243 */	1,	/* \tjgeI4I R%0,%1,%a; GE\n */
/* 244 */	1,	/* \tjcI2I R%0,%1,lbdf,%a; GE is flipped test from LT\n */
/* 245 */	1,	/* \tjgeU4I R%0,%1,%a; GE\n */
/* 246 */	1,	/* \tjnI2I R%0,%1,lbnf,%a; GT reverse  the subtraction\n */
/* 247 */	1,	/* \tjgtI4I R%0,%1,%a\n */
/* 248 */	1,	/* \tjnU2I R%0,%1,lbnf,%a; GT reverse the subtraction J2020-5\n */
/* 249 */	1,	/* \tjgtU4I R%0,%1,%a\n */
/* 250 */	1,	/* \tjnI2I R%0,%1,lbdf,%a ;LEI2 %1 %0 %a; LE is flipped test & subtraction\n */
/* 251 */	1,	/* \tjleI4I R%0,%1,%a\n */
/* 252 */	1,	/* \tjnU2I R%0,%1,lbdf,%a ;LEU2 %1 %0 %a; LE is flipped test & subtraction\n */
/* 253 */	1,	/* \tjleU4I R%0,%1,%a\n */
/* 254 */	1,	/* \tjcI1I R%0,%1,lbnf,%a  ;LTI2=lbnf i.e. subtract immedB from A and jump if borrow - nopromo 20-05-12 J2020-6\n */
/* 255 */	1,	/* \tjcI2I R%0,%1,lbnf,%a  ;LT=lbnf i.e. subtract immedB from A and jump if borrow J2020-7\n */
/* 256 */	1,	/* \tjltI4I R%0,%1,%a\n */
/* 257 */	1,	/* \tjcU1I R%0,%1,lbnf,%a ;LTU2=lbnf i.e. subtract immedB from A and jump if borrow - nopromo 20-05-12 J2020-8\n */
/* 258 */	1,	/* \tjcU2I R%0,%1,lbnf,%a ;LT=lbnf i.e. subtract immedB from A and jump if borrow J2020-9\n */
/* 259 */	1,	/* \tjltU4I R%0,%1,%a\n */
/* 260 */	1,	/* \tjnzU1 R%0,%a; NEI2(CVUI2(reg),con0)\n */
/* 261 */	1,	/* \tjnzU2 R%0,%a; NE 0\n */
/* 262 */	1,	/* \tjzU1 R%0,%a; EQ 0 - nopromo 20-05-12\n */
/* 263 */	1,	/* \tjzU2 R%0,%a; EQ I 0\n */
/* 264 */	1,	/* \tjzU2 R%0,%a; EQ U 0\n */
/* 265 */	1,	/* \tjzU1 R%0,%a; EQ 0 - nopromo 20-05-12\n */
/* 266 */	1,	/* \tjneU2I R%0,%1,%a; NE\n */
/* 267 */	1,	/* \tjneI4I R%0,%1,%a; NE\n */
/* 268 */	1,	/* \tjnzU2 R%0,%a; NE 0 \n */
/* 269 */	1,	/* \tjneU2I R%0,%1,%a; NE\n */
/* 270 */	1,	/* \tjneU4IR R%0,%1,%a; NE\n */
/* 271 */	1,	/* \tjneU1I R%0,%1,%a	; DH 4\n */
/* 272 */	1,	/* \tjeqI4 R%0,R%1,%a; EQF4(reg,reg)\n */
/* 273 */	1,	/* \tjcF4 R%1,R%0,lbnf,%a;GTF4(reg,reg) - reverse operands\n */
/* 274 */	1,	/* \tjcF4 R%0,R%1,lbdf,%a;GEF4(reg,reg) - reverse test\n */
/* 275 */	1,	/* \tjcF4 R%1,R%0,lbdf,%a; LEF4(reg,reg) - reverse test and operands\n */
/* 276 */	1,	/* \tjcF4 R%0,R%1,lbnf,%a;LTF4(reg,reg)LT=lbnf i.e. subtract B from A and jump if borrow\n */
/* 277 */	1,	/* \tjneU4 R%0,R%1,%a; NEF4(reg,reg)\n */
/* 278 */	0,	/* %a */
/* 279 */	1,	/* \tCcall %0;CALLF4(ar)\n */
/* 280 */	1,	/* \tCcall %0; CALLI2(ar)\n */
/* 281 */	1,	/* \tCcall %0\n */
/* 282 */	1,	/* \tCcall %0\n */
/* 283 */	1,	/* \tCcall %0;CALLU2(ar)*\n */
/* 284 */	1,	/* \tCcall %0\n */
/* 285 */	1,	/* \tCcall %0\n */
/* 286 */	0,	/* *R%0 */
/* 287 */	0,	/* %a */
/* 288 */	1,	/* # retn\n */
/* 289 */	1,	/* # retn\n */
/* 290 */	1,	/* # retn\n */
/* 291 */	1,	/* # retn\n */
/* 292 */	1,	/* # retn\n */
/* 293 */	1,	/* # retn\n */
/* 294 */	1,	/* # retn\n */
/* 295 */	1,	/* # arg\n */
/* 296 */	1,	/* # arg\n */
/* 297 */	1,	/* # arg\n */
/* 298 */	1,	/* # arg\n */
/* 299 */	1,	/* # arg\n */
/* 300 */	1,	/* # arg\n */
/* 301 */	1,	/* # argb %0\n */
/* 302 */	1,	/* \tblkcpy R%0,R%1,%a; ASGNB(reg,INDIRB(reg))\n */
/* 303 */	1,	/* # emit inline assembly\n */
};

static char *_string[] = {
/* 0 */	0,
/* 1 */	"reg: INDIRI1(VREGP)",
/* 2 */	"reg: INDIRU1(VREGP)",
/* 3 */	"reg: INDIRI2(VREGP)",
/* 4 */	"reg: INDIRU2(VREGP)",
/* 5 */	"reg: INDIRF4(VREGP)",
/* 6 */	"reg: INDIRI4(VREGP)",
/* 7 */	"reg: INDIRP2(VREGP)",
/* 8 */	"reg: INDIRU4(VREGP)",
/* 9 */	"stmt: ASGNI1(VREGP,reg)",
/* 10 */	"stmt: ASGNU1(VREGP,reg)",
/* 11 */	"stmt: ASGNI2(VREGP,reg)",
/* 12 */	"stmt: ASGNU2(VREGP,reg)",
/* 13 */	"stmt: ASGNP2(VREGP,reg)",
/* 14 */	"stmt: ASGNF4(VREGP,reg)",
/* 15 */	"stmt: ASGNI4(VREGP,reg)",
/* 16 */	"stmt: ASGNU4(VREGP,reg)",
/* 17 */	"con0: CNSTF4",
/* 18 */	"con0: CNSTU2",
/* 19 */	"con0: CNSTI2",
/* 20 */	"con2: CNSTU2",
/* 21 */	"con2: CNSTI2",
/* 22 */	"con2: CNSTU4",
/* 23 */	"con2: CNSTI4",
/* 24 */	"consm: CNSTU2",
/* 25 */	"consm: CNSTI2",
/* 26 */	"consm: CNSTP2",
/* 27 */	"con8bit: CNSTI2",
/* 28 */	"con8bit: CNSTU2",
/* 29 */	"con: CNSTI1",
/* 30 */	"con: CNSTU1",
/* 31 */	"con: CNSTI2",
/* 32 */	"con: CNSTU2",
/* 33 */	"con: CNSTP2",
/* 34 */	"reg: CNSTI4",
/* 35 */	"reg: CNSTU4",
/* 36 */	"stmt: reg",
/* 37 */	"acon: con",
/* 38 */	"acon: ADDRGP2",
/* 39 */	"addr: ADDI2(reg,acon)",
/* 40 */	"addr: ADDU2(reg,acon)",
/* 41 */	"addr: ADDP2(reg,acon)",
/* 42 */	"addr: acon",
/* 43 */	"addr: reg",
/* 44 */	"indaddr: reg",
/* 45 */	"addr: ADDRFP2",
/* 46 */	"addr: ADDRLP2",
/* 47 */	"reg: ADDI2(reg,consm)",
/* 48 */	"reg: ADDU2(reg,consm)",
/* 49 */	"reg: ADDP2(reg,consm)",
/* 50 */	"reg: SUBI2(reg,consm)",
/* 51 */	"reg: SUBU2(reg,consm)",
/* 52 */	"reg: SUBP2(reg,consm)",
/* 53 */	"reg: con0",
/* 54 */	"reg: acon",
/* 55 */	"reg: addr",
/* 56 */	"stmt: ASGNI1(addr,reg)",
/* 57 */	"stmt: ASGNI1(indaddr,reg)",
/* 58 */	"stmt: ASGNU1(indaddr,acon)",
/* 59 */	"stmt: ASGNU1(addr,reg)",
/* 60 */	"stmt: ASGNU1(indaddr,INDIRU1(indaddr))",
/* 61 */	"stmt: ASGNU1(indaddr,reg)",
/* 62 */	"stmt: ASGNU1(indaddr,LOADU1(LOADU2(reg)))",
/* 63 */	"stmt: ASGNU1(addr,acon)",
/* 64 */	"stmt: ASGNI2(addr,acon)",
/* 65 */	"stmt: ASGNI2(addr,reg)",
/* 66 */	"stmt: ASGNI2(addr,LOADI2(reg))",
/* 67 */	"stmt: ASGNU2(addr,reg)",
/* 68 */	"stmt: ASGNU2(addr,LOADU2(reg))",
/* 69 */	"stmt: ASGNI2(addr,LOADI2(reg))",
/* 70 */	"stmt: ASGNP2(addr,reg)",
/* 71 */	"stmt: ASGNP2(addr,acon)",
/* 72 */	"stmt: ASGNI4(addr,reg)",
/* 73 */	"stmt: ASGNU4(addr,reg)",
/* 74 */	"reg: INDIRI1(indaddr)",
/* 75 */	"reg: INDIRU1(indaddr)",
/* 76 */	"reg: INDIRI1(addr)",
/* 77 */	"reg: INDIRU1(addr)",
/* 78 */	"reg: INDIRI2(addr)",
/* 79 */	"reg: INDIRU2(addr)",
/* 80 */	"reg: INDIRP2(addr)",
/* 81 */	"reg: INDIRI4(addr)",
/* 82 */	"reg: INDIRU4(addr)",
/* 83 */	"reg: CVII2(INDIRI1(addr))",
/* 84 */	"reg: CVUU2(INDIRU1(addr))",
/* 85 */	"reg: CVUI2(INDIRU1(addr))",
/* 86 */	"reg: CVII4(INDIRI1(addr))",
/* 87 */	"reg: CVII4(INDIRI2(addr))",
/* 88 */	"reg: CVUU4(INDIRU1(addr))",
/* 89 */	"reg: CVUU4(INDIRU2(addr))",
/* 90 */	"reg: CVUI4(INDIRU1(addr))",
/* 91 */	"reg: CVUI4(INDIRU2(addr))",
/* 92 */	"reg: DIVI2(reg,reg)",
/* 93 */	"reg: DIVI4(reg,con2)",
/* 94 */	"reg: DIVI4(reg,reg)",
/* 95 */	"reg: DIVU2(reg,reg)",
/* 96 */	"reg: DIVU4(reg,reg)",
/* 97 */	"reg: MODI2(reg,reg)",
/* 98 */	"reg: MODI4(reg,reg)",
/* 99 */	"reg: MODU2(reg,reg)",
/* 100 */	"reg: MODU4(reg,reg)",
/* 101 */	"reg: MULI2(reg,reg)",
/* 102 */	"reg: MULI2(con8bit,reg)",
/* 103 */	"reg: MULU2(con8bit,reg)",
/* 104 */	"reg: MULI4(reg,reg)",
/* 105 */	"reg: MULU2(reg,reg)",
/* 106 */	"reg: MULU4(reg,reg)",
/* 107 */	"reg: ADDI2(reg,INDIRI2(addr))",
/* 108 */	"reg: ADDI2(reg,reg)",
/* 109 */	"reg: ADDI4(reg,reg)",
/* 110 */	"reg: ADDP2(reg,INDIRP2(addr))",
/* 111 */	"reg: ADDP2(reg,reg)",
/* 112 */	"reg: ADDU2(reg,reg)",
/* 113 */	"reg: ADDU4(reg,reg)",
/* 114 */	"reg: BANDI2(reg,reg)",
/* 115 */	"reg: BANDI4(reg,reg)",
/* 116 */	"reg: BORI2(reg,reg)",
/* 117 */	"reg: BORI4(reg,reg)",
/* 118 */	"reg: BXORI2(reg,reg)",
/* 119 */	"reg: BXORI4(reg,reg)",
/* 120 */	"reg: BANDU2(reg,reg)",
/* 121 */	"reg: BANDU4(reg,reg)",
/* 122 */	"reg: BORU2(reg,reg)",
/* 123 */	"reg: BORU4(reg,reg)",
/* 124 */	"reg: BXORU2(reg,reg)",
/* 125 */	"reg: BXORU4(reg,reg)",
/* 126 */	"reg: SUBI2(reg,reg)",
/* 127 */	"reg: SUBI4(reg,reg)",
/* 128 */	"reg: SUBP2(reg,reg)",
/* 129 */	"reg: SUBU2(reg,reg)",
/* 130 */	"reg: SUBU4(reg,reg)",
/* 131 */	"reg: ADDI2(reg,con)",
/* 132 */	"reg: ADDI2(CVUI2(reg),CVUI2(reg))",
/* 133 */	"reg: ADDI4(reg,con)",
/* 134 */	"reg: ADDP2(reg,con)",
/* 135 */	"reg: ADDU2(reg,con)",
/* 136 */	"reg: ADDU4(reg,con)",
/* 137 */	"reg: BANDI2(reg,con)",
/* 138 */	"reg: BANDI4(reg,con)",
/* 139 */	"reg: BORI2(reg,con)",
/* 140 */	"reg: BORI4(reg,con)",
/* 141 */	"reg: BXORI2(reg,con)",
/* 142 */	"reg: BXORI4(reg,con)",
/* 143 */	"reg: BANDU2(reg,con)",
/* 144 */	"reg: BANDU4(reg,con)",
/* 145 */	"reg: BORU2(reg,con)",
/* 146 */	"reg: BORU4(reg,con)",
/* 147 */	"reg: BXORU2(reg,con)",
/* 148 */	"reg: BXORU4(reg,con)",
/* 149 */	"reg: SUBI2(reg,con)",
/* 150 */	"reg: SUBI4(reg,con)",
/* 151 */	"reg: SUBP2(reg,con)",
/* 152 */	"reg: SUBU2(reg,con)",
/* 153 */	"reg: SUBU4(reg,con)",
/* 154 */	"reg: LSHI2(reg,reg)",
/* 155 */	"reg: LSHI4(reg,reg)",
/* 156 */	"reg: LSHU2(reg,reg)",
/* 157 */	"reg: LSHU4(reg,reg)",
/* 158 */	"reg: RSHI2(reg,reg)",
/* 159 */	"reg: RSHI4(reg,reg)",
/* 160 */	"reg: RSHU2(reg,reg)",
/* 161 */	"reg: RSHU4(reg,reg)",
/* 162 */	"reg: LSHI2(reg,con)",
/* 163 */	"reg: LSHI4(reg,con)",
/* 164 */	"reg: LSHU2(reg,con)",
/* 165 */	"reg: LSHU4(reg,con)",
/* 166 */	"reg: RSHI2(reg,con)",
/* 167 */	"reg: RSHI4(reg,con)",
/* 168 */	"reg: RSHU2(reg,con)",
/* 169 */	"reg: RSHU4(reg,con)",
/* 170 */	"reg: BCOMI2(reg)",
/* 171 */	"reg: BCOMI4(reg)",
/* 172 */	"reg: BCOMU2(reg)",
/* 173 */	"reg: BCOMU4(reg)",
/* 174 */	"reg: NEGI2(reg)",
/* 175 */	"reg: NEGI4(reg)",
/* 176 */	"reg: LOADI1(reg)",
/* 177 */	"reg: LOADU1(reg)",
/* 178 */	"reg: LOADI2(reg)",
/* 179 */	"reg: LOADU2(SUBU2(reg,consm))",
/* 180 */	"reg: LOADU2(reg)",
/* 181 */	"reg: LOADI4(reg)",
/* 182 */	"reg: LOADP2(reg)",
/* 183 */	"reg: LOADU4(reg)",
/* 184 */	"reg: INDIRF4(addr)",
/* 185 */	"stmt: ASGNF4(addr,reg)",
/* 186 */	"reg: ADDF4(reg,reg)",
/* 187 */	"reg: DIVF4(reg,reg)",
/* 188 */	"reg: MULF4(reg,reg)",
/* 189 */	"reg: SUBF4(reg,reg)",
/* 190 */	"reg: LOADF4(reg)",
/* 191 */	"reg: NEGF4(reg)",
/* 192 */	"reg: CVII2(reg)",
/* 193 */	"reg: CVIU2(reg)",
/* 194 */	"reg: CVUI2(reg)",
/* 195 */	"reg: CVUU2(reg)",
/* 196 */	"reg: CVII4(reg)",
/* 197 */	"reg: CVIU4(reg)",
/* 198 */	"reg: CVIU4(CVUI2(INDIRU1(addr)))",
/* 199 */	"reg: CVUI4(reg)",
/* 200 */	"reg: CVUU4(reg)",
/* 201 */	"reg: CVFF4(reg)",
/* 202 */	"reg: CVIF4(reg)",
/* 203 */	"reg: CVFI2(reg)",
/* 204 */	"reg: CVFI4(reg)",
/* 205 */	"stmt: LABELV",
/* 206 */	"stmt: JUMPV(reg)",
/* 207 */	"stmt: JUMPV(acon)",
/* 208 */	"stmt: JUMPV(acon)",
/* 209 */	"stmt: EQI2(reg,reg)",
/* 210 */	"stmt: EQU2(reg,reg)",
/* 211 */	"stmt: GEI2(reg,reg)",
/* 212 */	"stmt: GEU2(reg,reg)",
/* 213 */	"stmt: GTI2(reg,reg)",
/* 214 */	"stmt: GTU2(reg,reg)",
/* 215 */	"stmt: LEI2(CVUI2(reg),CVUI2(reg))",
/* 216 */	"stmt: LEI2(reg,reg)",
/* 217 */	"stmt: LEU2(reg,reg)",
/* 218 */	"stmt: LTI2(CVUI2(reg),CVUI2(reg))",
/* 219 */	"stmt: LTI2(reg,reg)",
/* 220 */	"stmt: LTU2(reg,reg)",
/* 221 */	"stmt: NEI2(CVUI2(reg),CVUI2(reg))",
/* 222 */	"stmt: NEI2(reg,reg)",
/* 223 */	"stmt: NEU2(reg,reg)",
/* 224 */	"stmt: EQI4(reg,reg)",
/* 225 */	"stmt: EQU4(reg,reg)",
/* 226 */	"stmt: GEI4(reg,reg)",
/* 227 */	"stmt: GEU4(reg,reg)",
/* 228 */	"stmt: GTI4(reg,reg)",
/* 229 */	"stmt: GTU4(reg,reg)",
/* 230 */	"stmt: LEI4(reg,reg)",
/* 231 */	"stmt: LEU4(reg,reg)",
/* 232 */	"stmt: LTI4(reg,reg)",
/* 233 */	"stmt: LTU4(reg,reg)",
/* 234 */	"stmt: NEI4(reg,reg)",
/* 235 */	"stmt: NEU4(reg,reg)",
/* 236 */	"stmt: EQI2(CVUI2(reg),con8bit)",
/* 237 */	"stmt: EQI2(reg,con)",
/* 238 */	"stmt: EQI4(reg,con)",
/* 239 */	"stmt: EQU2(reg,con)",
/* 240 */	"stmt: EQU4(reg,con)",
/* 241 */	"stmt: GEI2(CVUI2(reg),con)",
/* 242 */	"stmt: GEI2(reg,con)",
/* 243 */	"stmt: GEI4(reg,con)",
/* 244 */	"stmt: GEU2(reg,con)",
/* 245 */	"stmt: GEU4(reg,con)",
/* 246 */	"stmt: GTI2(reg,con)",
/* 247 */	"stmt: GTI4(reg,con)",
/* 248 */	"stmt: GTU2(reg,con)",
/* 249 */	"stmt: GTU4(reg,con)",
/* 250 */	"stmt: LEI2(reg,con)",
/* 251 */	"stmt: LEI4(reg,con)",
/* 252 */	"stmt: LEU2(reg,con)",
/* 253 */	"stmt: LEU4(reg,con)",
/* 254 */	"stmt: LTI2(CVUI2(reg),con)",
/* 255 */	"stmt: LTI2(reg,con)",
/* 256 */	"stmt: LTI4(reg,con)",
/* 257 */	"stmt: LTU2(CVUI2(reg),con)",
/* 258 */	"stmt: LTU2(reg,con)",
/* 259 */	"stmt: LTU4(reg,con)",
/* 260 */	"stmt: NEI2(CVUI2(reg),con0)",
/* 261 */	"stmt: NEI2(reg,con0)",
/* 262 */	"stmt: EQI2(CVUI2(reg),con0)",
/* 263 */	"stmt: EQI2(reg,con0)",
/* 264 */	"stmt: EQU2(reg,con0)",
/* 265 */	"stmt: EQU2(CVUI2(reg),con0)",
/* 266 */	"stmt: NEI2(reg,con)",
/* 267 */	"stmt: NEI4(reg,con)",
/* 268 */	"stmt: NEU2(reg,con0)",
/* 269 */	"stmt: NEU2(reg,con)",
/* 270 */	"stmt: NEU4(reg,con)",
/* 271 */	"stmt: NEI2(CVUI2(reg),con8bit)",
/* 272 */	"stmt: EQF4(reg,reg)",
/* 273 */	"stmt: GTF4(reg,reg)",
/* 274 */	"stmt: GEF4(reg,reg)",
/* 275 */	"stmt: LEF4(reg,reg)",
/* 276 */	"stmt: LTF4(reg,reg)",
/* 277 */	"stmt: NEF4(reg,reg)",
/* 278 */	"ar: ADDRGP2",
/* 279 */	"reg: CALLF4(ar)",
/* 280 */	"reg: CALLI2(ar)",
/* 281 */	"reg: CALLI4(ar)",
/* 282 */	"reg: CALLP2(ar)",
/* 283 */	"reg: CALLU2(ar)",
/* 284 */	"reg: CALLU4(ar)",
/* 285 */	"stmt: CALLV(ar)",
/* 286 */	"ar: reg",
/* 287 */	"ar: CNSTP2",
/* 288 */	"stmt: RETF4(reg)",
/* 289 */	"stmt: RETI2(reg)",
/* 290 */	"stmt: RETI4(reg)",
/* 291 */	"stmt: RETU2(reg)",
/* 292 */	"stmt: RETU4(reg)",
/* 293 */	"stmt: RETP2(reg)",
/* 294 */	"stmt: RETV(reg)",
/* 295 */	"stmt: ARGF4(reg)",
/* 296 */	"stmt: ARGI2(reg)",
/* 297 */	"stmt: ARGI4(reg)",
/* 298 */	"stmt: ARGP2(reg)",
/* 299 */	"stmt: ARGU2(reg)",
/* 300 */	"stmt: ARGU4(reg)",
/* 301 */	"stmt: ARGB(INDIRB(reg))",
/* 302 */	"stmt: ASGNB(reg,INDIRB(reg))",
/* 303 */	"stmt: IASMV",
};

static short _decode_stmt[] = {
	0,
	9,
	10,
	11,
	12,
	13,
	14,
	15,
	16,
	36,
	56,
	57,
	58,
	59,
	60,
	61,
	62,
	63,
	64,
	65,
	66,
	67,
	68,
	69,
	70,
	71,
	72,
	73,
	185,
	205,
	206,
	207,
	208,
	209,
	210,
	211,
	212,
	213,
	214,
	215,
	216,
	217,
	218,
	219,
	220,
	221,
	222,
	223,
	224,
	225,
	226,
	227,
	228,
	229,
	230,
	231,
	232,
	233,
	234,
	235,
	236,
	237,
	238,
	239,
	240,
	241,
	242,
	243,
	244,
	245,
	246,
	247,
	248,
	249,
	250,
	251,
	252,
	253,
	254,
	255,
	256,
	257,
	258,
	259,
	260,
	261,
	262,
	263,
	264,
	265,
	266,
	267,
	268,
	269,
	270,
	271,
	272,
	273,
	274,
	275,
	276,
	277,
	285,
	288,
	289,
	290,
	291,
	292,
	293,
	294,
	295,
	296,
	297,
	298,
	299,
	300,
	301,
	302,
	303,
};

static short _decode_reg[] = {
	0,
	1,
	2,
	3,
	4,
	5,
	6,
	7,
	8,
	34,
	35,
	47,
	48,
	49,
	50,
	51,
	52,
	53,
	54,
	55,
	74,
	75,
	76,
	77,
	78,
	79,
	80,
	81,
	82,
	83,
	84,
	85,
	86,
	87,
	88,
	89,
	90,
	91,
	92,
	93,
	94,
	95,
	96,
	97,
	98,
	99,
	100,
	101,
	102,
	103,
	104,
	105,
	106,
	107,
	108,
	109,
	110,
	111,
	112,
	113,
	114,
	115,
	116,
	117,
	118,
	119,
	120,
	121,
	122,
	123,
	124,
	125,
	126,
	127,
	128,
	129,
	130,
	131,
	132,
	133,
	134,
	135,
	136,
	137,
	138,
	139,
	140,
	141,
	142,
	143,
	144,
	145,
	146,
	147,
	148,
	149,
	150,
	151,
	152,
	153,
	154,
	155,
	156,
	157,
	158,
	159,
	160,
	161,
	162,
	163,
	164,
	165,
	166,
	167,
	168,
	169,
	170,
	171,
	172,
	173,
	174,
	175,
	176,
	177,
	178,
	179,
	180,
	181,
	182,
	183,
	184,
	186,
	187,
	188,
	189,
	190,
	191,
	192,
	193,
	194,
	195,
	196,
	197,
	198,
	199,
	200,
	201,
	202,
	203,
	204,
	279,
	280,
	281,
	282,
	283,
	284,
};

static short _decode_con0[] = {
	0,
	17,
	18,
	19,
};

static short _decode_con2[] = {
	0,
	20,
	21,
	22,
	23,
};

static short _decode_consm[] = {
	0,
	24,
	25,
	26,
};

static short _decode_con8bit[] = {
	0,
	27,
	28,
};

static short _decode_con[] = {
	0,
	29,
	30,
	31,
	32,
	33,
};

static short _decode_acon[] = {
	0,
	37,
	38,
};

static short _decode_addr[] = {
	0,
	39,
	40,
	41,
	42,
	43,
	45,
	46,
};

static short _decode_indaddr[] = {
	0,
	44,
};

static short _decode_ar[] = {
	0,
	278,
	286,
	287,
};

static int _rule(void *state, int goalnt) {
	if (goalnt < 1 || goalnt > 11)
		fatal("_rule", "Bad goal nonterminal %d\n", goalnt);
	if (!state)
		return 0;
	switch (goalnt) {
	case _stmt_NT:	return _decode_stmt[((struct _state *)state)->rule._stmt];
	case _reg_NT:	return _decode_reg[((struct _state *)state)->rule._reg];
	case _con0_NT:	return _decode_con0[((struct _state *)state)->rule._con0];
	case _con2_NT:	return _decode_con2[((struct _state *)state)->rule._con2];
	case _consm_NT:	return _decode_consm[((struct _state *)state)->rule._consm];
	case _con8bit_NT:	return _decode_con8bit[((struct _state *)state)->rule._con8bit];
	case _con_NT:	return _decode_con[((struct _state *)state)->rule._con];
	case _acon_NT:	return _decode_acon[((struct _state *)state)->rule._acon];
	case _addr_NT:	return _decode_addr[((struct _state *)state)->rule._addr];
	case _indaddr_NT:	return _decode_indaddr[((struct _state *)state)->rule._indaddr];
	case _ar_NT:	return _decode_ar[((struct _state *)state)->rule._ar];
	default:
		fatal("_rule", "Bad goal nonterminal %d\n", goalnt);
		return 0;
	}
}

static void _closure_reg(NODEPTR_TYPE, int);
static void _closure_con0(NODEPTR_TYPE, int);
static void _closure_con(NODEPTR_TYPE, int);
static void _closure_acon(NODEPTR_TYPE, int);
static void _closure_addr(NODEPTR_TYPE, int);

static void _closure_reg(NODEPTR_TYPE a, int c) {
	struct _state *p = STATE_LABEL(a);
	if (c + 0 < p->cost[_ar_NT]) {
		p->cost[_ar_NT] = c + 0;
		p->rule._ar = 2;
	}
	if (c + 0 < p->cost[_indaddr_NT]) {
		p->cost[_indaddr_NT] = c + 0;
		p->rule._indaddr = 1;
	}
	if (c + 0 < p->cost[_addr_NT]) {
		p->cost[_addr_NT] = c + 0;
		p->rule._addr = 5;
		_closure_addr(a, c + 0);
	}
	if (c + 0 < p->cost[_stmt_NT]) {
		p->cost[_stmt_NT] = c + 0;
		p->rule._stmt = 9;
	}
}

static void _closure_con0(NODEPTR_TYPE a, int c) {
	struct _state *p = STATE_LABEL(a);
	if (c + 1 < p->cost[_reg_NT]) {
		p->cost[_reg_NT] = c + 1;
		p->rule._reg = 17;
		_closure_reg(a, c + 1);
	}
}

static void _closure_con(NODEPTR_TYPE a, int c) {
	struct _state *p = STATE_LABEL(a);
	if (c + 0 < p->cost[_acon_NT]) {
		p->cost[_acon_NT] = c + 0;
		p->rule._acon = 1;
		_closure_acon(a, c + 0);
	}
}

static void _closure_acon(NODEPTR_TYPE a, int c) {
	struct _state *p = STATE_LABEL(a);
	if (c + 1 < p->cost[_reg_NT]) {
		p->cost[_reg_NT] = c + 1;
		p->rule._reg = 18;
		_closure_reg(a, c + 1);
	}
	if (c + 0 < p->cost[_addr_NT]) {
		p->cost[_addr_NT] = c + 0;
		p->rule._addr = 4;
		_closure_addr(a, c + 0);
	}
}

static void _closure_addr(NODEPTR_TYPE a, int c) {
	struct _state *p = STATE_LABEL(a);
	if (c + 2 < p->cost[_reg_NT]) {
		p->cost[_reg_NT] = c + 2;
		p->rule._reg = 19;
		_closure_reg(a, c + 2);
	}
}

static void _label(NODEPTR_TYPE a) {
	int c;
	struct _state *p;

	if (!a)
		fatal("_label", "Null tree\n", 0);
	STATE_LABEL(a) = p = allocate(sizeof *p, FUNC);
	p->rule._stmt = 0;
	p->cost[1] =
	p->cost[2] =
	p->cost[3] =
	p->cost[4] =
	p->cost[5] =
	p->cost[6] =
	p->cost[7] =
	p->cost[8] =
	p->cost[9] =
	p->cost[10] =
	p->cost[11] =
		0x7fff;
	switch (OP_LABEL(a)) {
	case 41: /* ARGB */
		_label(LEFT_CHILD(a));
		if (	/* stmt: ARGB(INDIRB(reg)) */
			LEFT_CHILD(a)->op == 73 /* INDIRB */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 116;
			}
		}
		break;
	case 57: /* ASGNB */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		if (	/* stmt: ASGNB(reg,INDIRB(reg)) */
			RIGHT_CHILD(a)->op == 73 /* INDIRB */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_reg_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 117;
			}
		}
		break;
	case 73: /* INDIRB */
		_label(LEFT_CHILD(a));
		break;
	case 88: /* IASMV */
		/* stmt: IASMV */
		if (0 + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = 0 + 0;
			p->rule._stmt = 118;
		}
		break;
	case 216: /* CALLV */
		_label(LEFT_CHILD(a));
		/* stmt: CALLV(ar) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_ar_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 102;
		}
		break;
	case 217: /* CALLB */
		break;
	case 233: /* LOADB */
		break;
	case 248: /* RETV */
		_label(LEFT_CHILD(a));
		/* stmt: RETV(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 109;
		}
		break;
	case 584: /* JUMPV */
		_label(LEFT_CHILD(a));
		/* stmt: JUMPV(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 30;
		}
		/* stmt: JUMPV(acon) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_acon_NT] + (1+wjrpixie);
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 31;
		}
		/* stmt: JUMPV(acon) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_acon_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 32;
		}
		break;
	case 600: /* LABELV */
		/* stmt: LABELV */
		if (0 + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = 0 + 0;
			p->rule._stmt = 29;
		}
		break;
	case 711: /* VREGP */
		break;
	case 1045: /* CNSTI1 */
		/* con: CNSTI1 */
		if (0 + 0 < p->cost[_con_NT]) {
			p->cost[_con_NT] = 0 + 0;
			p->rule._con = 1;
			_closure_con(a, 0 + 0);
		}
		break;
	case 1046: /* CNSTU1 */
		/* con: CNSTU1 */
		if (0 + 0 < p->cost[_con_NT]) {
			p->cost[_con_NT] = 0 + 0;
			p->rule._con = 2;
			_closure_con(a, 0 + 0);
		}
		break;
	case 1077: /* ASGNI1 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		if (	/* stmt: ASGNI1(VREGP,reg) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			c = ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 0;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 1;
			}
		}
		/* stmt: ASGNI1(addr,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 10;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 10;
		}
		/* stmt: ASGNI1(indaddr,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_indaddr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 11;
		}
		break;
	case 1078: /* ASGNU1 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		if (	/* stmt: ASGNU1(VREGP,reg) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			c = ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 0;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 2;
			}
		}
		/* stmt: ASGNU1(indaddr,acon) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_indaddr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_acon_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 12;
		}
		/* stmt: ASGNU1(addr,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 10;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 13;
		}
		if (	/* stmt: ASGNU1(indaddr,INDIRU1(indaddr)) */
			RIGHT_CHILD(a)->op == 1094 /* INDIRU1 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_indaddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_indaddr_NT] + 3;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 14;
			}
		}
		/* stmt: ASGNU1(indaddr,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_indaddr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 15;
		}
		if (	/* stmt: ASGNU1(indaddr,LOADU1(LOADU2(reg))) */
			RIGHT_CHILD(a)->op == 1254 && /* LOADU1 */
			LEFT_CHILD(RIGHT_CHILD(a))->op == 2278 /* LOADU2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_indaddr_NT] + ((struct _state *)(LEFT_CHILD(LEFT_CHILD(RIGHT_CHILD(a)))->x.state))->cost[_reg_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 16;
			}
		}
		/* stmt: ASGNU1(addr,acon) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_acon_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 17;
		}
		break;
	case 1093: /* INDIRI1 */
		_label(LEFT_CHILD(a));
		if (	/* reg: INDIRI1(VREGP) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			if (mayrecalc(a)) {
				struct _state *q = a->syms[RX]->u.t.cse->x.state;
				if (q->cost[_stmt_NT] == 0) {
					p->cost[_stmt_NT] = 0;
					p->rule._stmt = q->rule._stmt;
				}
				if (q->cost[_reg_NT] == 0) {
					p->cost[_reg_NT] = 0;
					p->rule._reg = q->rule._reg;
				}
				if (q->cost[_con0_NT] == 0) {
					p->cost[_con0_NT] = 0;
					p->rule._con0 = q->rule._con0;
				}
				if (q->cost[_con2_NT] == 0) {
					p->cost[_con2_NT] = 0;
					p->rule._con2 = q->rule._con2;
				}
				if (q->cost[_consm_NT] == 0) {
					p->cost[_consm_NT] = 0;
					p->rule._consm = q->rule._consm;
				}
				if (q->cost[_con8bit_NT] == 0) {
					p->cost[_con8bit_NT] = 0;
					p->rule._con8bit = q->rule._con8bit;
				}
				if (q->cost[_con_NT] == 0) {
					p->cost[_con_NT] = 0;
					p->rule._con = q->rule._con;
				}
				if (q->cost[_acon_NT] == 0) {
					p->cost[_acon_NT] = 0;
					p->rule._acon = q->rule._acon;
				}
				if (q->cost[_addr_NT] == 0) {
					p->cost[_addr_NT] = 0;
					p->rule._addr = q->rule._addr;
				}
				if (q->cost[_indaddr_NT] == 0) {
					p->cost[_indaddr_NT] = 0;
					p->rule._indaddr = q->rule._indaddr;
				}
				if (q->cost[_ar_NT] == 0) {
					p->cost[_ar_NT] = 0;
					p->rule._ar = q->rule._ar;
				}
			}
			c = 0;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 1;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: INDIRI1(indaddr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_indaddr_NT] + 0;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 20;
			_closure_reg(a, c + 0);
		}
		/* reg: INDIRI1(addr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 22;
			_closure_reg(a, c + 0);
		}
		break;
	case 1094: /* INDIRU1 */
		_label(LEFT_CHILD(a));
		if (	/* reg: INDIRU1(VREGP) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			if (mayrecalc(a)) {
				struct _state *q = a->syms[RX]->u.t.cse->x.state;
				if (q->cost[_stmt_NT] == 0) {
					p->cost[_stmt_NT] = 0;
					p->rule._stmt = q->rule._stmt;
				}
				if (q->cost[_reg_NT] == 0) {
					p->cost[_reg_NT] = 0;
					p->rule._reg = q->rule._reg;
				}
				if (q->cost[_con0_NT] == 0) {
					p->cost[_con0_NT] = 0;
					p->rule._con0 = q->rule._con0;
				}
				if (q->cost[_con2_NT] == 0) {
					p->cost[_con2_NT] = 0;
					p->rule._con2 = q->rule._con2;
				}
				if (q->cost[_consm_NT] == 0) {
					p->cost[_consm_NT] = 0;
					p->rule._consm = q->rule._consm;
				}
				if (q->cost[_con8bit_NT] == 0) {
					p->cost[_con8bit_NT] = 0;
					p->rule._con8bit = q->rule._con8bit;
				}
				if (q->cost[_con_NT] == 0) {
					p->cost[_con_NT] = 0;
					p->rule._con = q->rule._con;
				}
				if (q->cost[_acon_NT] == 0) {
					p->cost[_acon_NT] = 0;
					p->rule._acon = q->rule._acon;
				}
				if (q->cost[_addr_NT] == 0) {
					p->cost[_addr_NT] = 0;
					p->rule._addr = q->rule._addr;
				}
				if (q->cost[_indaddr_NT] == 0) {
					p->cost[_indaddr_NT] = 0;
					p->rule._indaddr = q->rule._indaddr;
				}
				if (q->cost[_ar_NT] == 0) {
					p->cost[_ar_NT] = 0;
					p->rule._ar = q->rule._ar;
				}
			}
			c = 0;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 2;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: INDIRU1(indaddr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_indaddr_NT] + 0;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 21;
			_closure_reg(a, c + 0);
		}
		/* reg: INDIRU1(addr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 23;
			_closure_reg(a, c + 0);
		}
		break;
	case 1157: /* CVII1 */
		break;
	case 1158: /* CVIU1 */
		break;
	case 1205: /* CVUI1 */
		break;
	case 1206: /* CVUU1 */
		break;
	case 1253: /* LOADI1 */
		_label(LEFT_CHILD(a));
		/* reg: LOADI1(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (move(a));
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 122;
			_closure_reg(a, c + 0);
		}
		break;
	case 1254: /* LOADU1 */
		_label(LEFT_CHILD(a));
		/* reg: LOADU1(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (move(a));
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 123;
			_closure_reg(a, c + 0);
		}
		break;
	case 2069: /* CNSTI2 */
		/* con0: CNSTI2 */
		c = (range(a,0,0));
		if (c + 0 < p->cost[_con0_NT]) {
			p->cost[_con0_NT] = c + 0;
			p->rule._con0 = 3;
			_closure_con0(a, c + 0);
		}
		/* con2: CNSTI2 */
		c = (range(a,2,2));
		if (c + 0 < p->cost[_con2_NT]) {
			p->cost[_con2_NT] = c + 0;
			p->rule._con2 = 2;
		}
		/* consm: CNSTI2 */
		c = (range (a,1,4));
		if (c + 0 < p->cost[_consm_NT]) {
			p->cost[_consm_NT] = c + 0;
			p->rule._consm = 2;
		}
		/* con8bit: CNSTI2 */
		c = (range (a,1,255));
		if (c + 0 < p->cost[_con8bit_NT]) {
			p->cost[_con8bit_NT] = c + 0;
			p->rule._con8bit = 1;
		}
		/* con: CNSTI2 */
		if (0 + 0 < p->cost[_con_NT]) {
			p->cost[_con_NT] = 0 + 0;
			p->rule._con = 3;
			_closure_con(a, 0 + 0);
		}
		break;
	case 2070: /* CNSTU2 */
		/* con0: CNSTU2 */
		c = (range(a,0,0));
		if (c + 0 < p->cost[_con0_NT]) {
			p->cost[_con0_NT] = c + 0;
			p->rule._con0 = 2;
			_closure_con0(a, c + 0);
		}
		/* con2: CNSTU2 */
		c = (range(a,2,2));
		if (c + 0 < p->cost[_con2_NT]) {
			p->cost[_con2_NT] = c + 0;
			p->rule._con2 = 1;
		}
		/* consm: CNSTU2 */
		c = (range (a,1,4));
		if (c + 0 < p->cost[_consm_NT]) {
			p->cost[_consm_NT] = c + 0;
			p->rule._consm = 1;
		}
		/* con8bit: CNSTU2 */
		c = (range (a,1,255));
		if (c + 0 < p->cost[_con8bit_NT]) {
			p->cost[_con8bit_NT] = c + 0;
			p->rule._con8bit = 2;
		}
		/* con: CNSTU2 */
		if (0 + 0 < p->cost[_con_NT]) {
			p->cost[_con_NT] = 0 + 0;
			p->rule._con = 4;
			_closure_con(a, 0 + 0);
		}
		break;
	case 2071: /* CNSTP2 */
		/* consm: CNSTP2 */
		c = (range (a,1,4));
		if (c + 0 < p->cost[_consm_NT]) {
			p->cost[_consm_NT] = c + 0;
			p->rule._consm = 3;
		}
		/* con: CNSTP2 */
		if (0 + 0 < p->cost[_con_NT]) {
			p->cost[_con_NT] = 0 + 0;
			p->rule._con = 5;
			_closure_con(a, 0 + 0);
		}
		/* ar: CNSTP2 */
		c = (range(a, 0, 0x0ffff));
		if (c + 0 < p->cost[_ar_NT]) {
			p->cost[_ar_NT] = c + 0;
			p->rule._ar = 3;
		}
		break;
	case 2085: /* ARGI2 */
		_label(LEFT_CHILD(a));
		/* stmt: ARGI2(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 111;
		}
		break;
	case 2086: /* ARGU2 */
		_label(LEFT_CHILD(a));
		/* stmt: ARGU2(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 114;
		}
		break;
	case 2087: /* ARGP2 */
		_label(LEFT_CHILD(a));
		/* stmt: ARGP2(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 113;
		}
		break;
	case 2101: /* ASGNI2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		if (	/* stmt: ASGNI2(VREGP,reg) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			c = ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 0;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 3;
			}
		}
		/* stmt: ASGNI2(addr,acon) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_acon_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 18;
		}
		/* stmt: ASGNI2(addr,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 10;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 19;
		}
		if (	/* stmt: ASGNI2(addr,LOADI2(reg)) */
			RIGHT_CHILD(a)->op == 2277 /* LOADI2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_reg_NT] + 10;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 20;
			}
		}
		if (	/* stmt: ASGNI2(addr,LOADI2(reg)) */
			RIGHT_CHILD(a)->op == 2277 /* LOADI2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_reg_NT] + 10;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 23;
			}
		}
		break;
	case 2102: /* ASGNU2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		if (	/* stmt: ASGNU2(VREGP,reg) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			c = ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 0;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 4;
			}
		}
		/* stmt: ASGNU2(addr,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 10;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 21;
		}
		if (	/* stmt: ASGNU2(addr,LOADU2(reg)) */
			RIGHT_CHILD(a)->op == 2278 /* LOADU2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_reg_NT] + 10;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 22;
			}
		}
		break;
	case 2103: /* ASGNP2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		if (	/* stmt: ASGNP2(VREGP,reg) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			c = ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 0;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 5;
			}
		}
		/* stmt: ASGNP2(addr,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 24;
		}
		/* stmt: ASGNP2(addr,acon) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_acon_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 25;
		}
		break;
	case 2117: /* INDIRI2 */
		_label(LEFT_CHILD(a));
		if (	/* reg: INDIRI2(VREGP) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			if (mayrecalc(a)) {
				struct _state *q = a->syms[RX]->u.t.cse->x.state;
				if (q->cost[_stmt_NT] == 0) {
					p->cost[_stmt_NT] = 0;
					p->rule._stmt = q->rule._stmt;
				}
				if (q->cost[_reg_NT] == 0) {
					p->cost[_reg_NT] = 0;
					p->rule._reg = q->rule._reg;
				}
				if (q->cost[_con0_NT] == 0) {
					p->cost[_con0_NT] = 0;
					p->rule._con0 = q->rule._con0;
				}
				if (q->cost[_con2_NT] == 0) {
					p->cost[_con2_NT] = 0;
					p->rule._con2 = q->rule._con2;
				}
				if (q->cost[_consm_NT] == 0) {
					p->cost[_consm_NT] = 0;
					p->rule._consm = q->rule._consm;
				}
				if (q->cost[_con8bit_NT] == 0) {
					p->cost[_con8bit_NT] = 0;
					p->rule._con8bit = q->rule._con8bit;
				}
				if (q->cost[_con_NT] == 0) {
					p->cost[_con_NT] = 0;
					p->rule._con = q->rule._con;
				}
				if (q->cost[_acon_NT] == 0) {
					p->cost[_acon_NT] = 0;
					p->rule._acon = q->rule._acon;
				}
				if (q->cost[_addr_NT] == 0) {
					p->cost[_addr_NT] = 0;
					p->rule._addr = q->rule._addr;
				}
				if (q->cost[_indaddr_NT] == 0) {
					p->cost[_indaddr_NT] = 0;
					p->rule._indaddr = q->rule._indaddr;
				}
				if (q->cost[_ar_NT] == 0) {
					p->cost[_ar_NT] = 0;
					p->rule._ar = q->rule._ar;
				}
			}
			c = 0;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 3;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: INDIRI2(addr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 24;
			_closure_reg(a, c + 0);
		}
		break;
	case 2118: /* INDIRU2 */
		_label(LEFT_CHILD(a));
		if (	/* reg: INDIRU2(VREGP) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			if (mayrecalc(a)) {
				struct _state *q = a->syms[RX]->u.t.cse->x.state;
				if (q->cost[_stmt_NT] == 0) {
					p->cost[_stmt_NT] = 0;
					p->rule._stmt = q->rule._stmt;
				}
				if (q->cost[_reg_NT] == 0) {
					p->cost[_reg_NT] = 0;
					p->rule._reg = q->rule._reg;
				}
				if (q->cost[_con0_NT] == 0) {
					p->cost[_con0_NT] = 0;
					p->rule._con0 = q->rule._con0;
				}
				if (q->cost[_con2_NT] == 0) {
					p->cost[_con2_NT] = 0;
					p->rule._con2 = q->rule._con2;
				}
				if (q->cost[_consm_NT] == 0) {
					p->cost[_consm_NT] = 0;
					p->rule._consm = q->rule._consm;
				}
				if (q->cost[_con8bit_NT] == 0) {
					p->cost[_con8bit_NT] = 0;
					p->rule._con8bit = q->rule._con8bit;
				}
				if (q->cost[_con_NT] == 0) {
					p->cost[_con_NT] = 0;
					p->rule._con = q->rule._con;
				}
				if (q->cost[_acon_NT] == 0) {
					p->cost[_acon_NT] = 0;
					p->rule._acon = q->rule._acon;
				}
				if (q->cost[_addr_NT] == 0) {
					p->cost[_addr_NT] = 0;
					p->rule._addr = q->rule._addr;
				}
				if (q->cost[_indaddr_NT] == 0) {
					p->cost[_indaddr_NT] = 0;
					p->rule._indaddr = q->rule._indaddr;
				}
				if (q->cost[_ar_NT] == 0) {
					p->cost[_ar_NT] = 0;
					p->rule._ar = q->rule._ar;
				}
			}
			c = 0;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 4;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: INDIRU2(addr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 25;
			_closure_reg(a, c + 0);
		}
		break;
	case 2119: /* INDIRP2 */
		_label(LEFT_CHILD(a));
		if (	/* reg: INDIRP2(VREGP) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			if (mayrecalc(a)) {
				struct _state *q = a->syms[RX]->u.t.cse->x.state;
				if (q->cost[_stmt_NT] == 0) {
					p->cost[_stmt_NT] = 0;
					p->rule._stmt = q->rule._stmt;
				}
				if (q->cost[_reg_NT] == 0) {
					p->cost[_reg_NT] = 0;
					p->rule._reg = q->rule._reg;
				}
				if (q->cost[_con0_NT] == 0) {
					p->cost[_con0_NT] = 0;
					p->rule._con0 = q->rule._con0;
				}
				if (q->cost[_con2_NT] == 0) {
					p->cost[_con2_NT] = 0;
					p->rule._con2 = q->rule._con2;
				}
				if (q->cost[_consm_NT] == 0) {
					p->cost[_consm_NT] = 0;
					p->rule._consm = q->rule._consm;
				}
				if (q->cost[_con8bit_NT] == 0) {
					p->cost[_con8bit_NT] = 0;
					p->rule._con8bit = q->rule._con8bit;
				}
				if (q->cost[_con_NT] == 0) {
					p->cost[_con_NT] = 0;
					p->rule._con = q->rule._con;
				}
				if (q->cost[_acon_NT] == 0) {
					p->cost[_acon_NT] = 0;
					p->rule._acon = q->rule._acon;
				}
				if (q->cost[_addr_NT] == 0) {
					p->cost[_addr_NT] = 0;
					p->rule._addr = q->rule._addr;
				}
				if (q->cost[_indaddr_NT] == 0) {
					p->cost[_indaddr_NT] = 0;
					p->rule._indaddr = q->rule._indaddr;
				}
				if (q->cost[_ar_NT] == 0) {
					p->cost[_ar_NT] = 0;
					p->rule._ar = q->rule._ar;
				}
			}
			c = 0;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 7;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: INDIRP2(addr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 26;
			_closure_reg(a, c + 0);
		}
		break;
	case 2165: /* CVFI2 */
		_label(LEFT_CHILD(a));
		/* reg: CVFI2(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (fp());
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 148;
			_closure_reg(a, c + 0);
		}
		break;
	case 2181: /* CVII2 */
		_label(LEFT_CHILD(a));
		if (	/* reg: CVII2(INDIRI1(addr)) */
			LEFT_CHILD(a)->op == 1093 /* INDIRI1 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_addr_NT] + 1;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 29;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: CVII2(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 137;
			_closure_reg(a, c + 0);
		}
		break;
	case 2182: /* CVIU2 */
		_label(LEFT_CHILD(a));
		/* reg: CVIU2(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 138;
			_closure_reg(a, c + 0);
		}
		break;
	case 2198: /* CVPU2 */
		break;
	case 2229: /* CVUI2 */
		_label(LEFT_CHILD(a));
		if (	/* reg: CVUI2(INDIRU1(addr)) */
			LEFT_CHILD(a)->op == 1094 /* INDIRU1 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_addr_NT] + 1;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 31;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: CVUI2(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 139;
			_closure_reg(a, c + 0);
		}
		break;
	case 2230: /* CVUU2 */
		_label(LEFT_CHILD(a));
		if (	/* reg: CVUU2(INDIRU1(addr)) */
			LEFT_CHILD(a)->op == 1094 /* INDIRU1 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_addr_NT] + 1;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 30;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: CVUU2(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 140;
			_closure_reg(a, c + 0);
		}
		break;
	case 2231: /* CVUP2 */
		break;
	case 2245: /* NEGI2 */
		_label(LEFT_CHILD(a));
		/* reg: NEGI2(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 120;
			_closure_reg(a, c + 0);
		}
		break;
	case 2261: /* CALLI2 */
		_label(LEFT_CHILD(a));
		/* reg: CALLI2(ar) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_ar_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 151;
			_closure_reg(a, c + 0);
		}
		break;
	case 2262: /* CALLU2 */
		_label(LEFT_CHILD(a));
		/* reg: CALLU2(ar) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_ar_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 154;
			_closure_reg(a, c + 0);
		}
		break;
	case 2263: /* CALLP2 */
		_label(LEFT_CHILD(a));
		/* reg: CALLP2(ar) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_ar_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 153;
			_closure_reg(a, c + 0);
		}
		break;
	case 2277: /* LOADI2 */
		_label(LEFT_CHILD(a));
		/* reg: LOADI2(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (move(a)+10);
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 124;
			_closure_reg(a, c + 0);
		}
		break;
	case 2278: /* LOADU2 */
		_label(LEFT_CHILD(a));
		if (	/* reg: LOADU2(SUBU2(reg,consm)) */
			LEFT_CHILD(a)->op == 2374 /* SUBU2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(LEFT_CHILD(a))->x.state))->cost[_consm_NT] + 0;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 125;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: LOADU2(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (move(a)+10);
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 126;
			_closure_reg(a, c + 0);
		}
		break;
	case 2279: /* LOADP2 */
		_label(LEFT_CHILD(a));
		/* reg: LOADP2(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (move(a)+1);
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 128;
			_closure_reg(a, c + 0);
		}
		break;
	case 2293: /* RETI2 */
		_label(LEFT_CHILD(a));
		/* stmt: RETI2(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 104;
		}
		break;
	case 2294: /* RETU2 */
		_label(LEFT_CHILD(a));
		/* stmt: RETU2(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 106;
		}
		break;
	case 2295: /* RETP2 */
		_label(LEFT_CHILD(a));
		/* stmt: RETP2(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 108;
		}
		break;
	case 2311: /* ADDRGP2 */
		/* acon: ADDRGP2 */
		if (0 + 0 < p->cost[_acon_NT]) {
			p->cost[_acon_NT] = 0 + 0;
			p->rule._acon = 2;
			_closure_acon(a, 0 + 0);
		}
		/* ar: ADDRGP2 */
		if (0 + 0 < p->cost[_ar_NT]) {
			p->cost[_ar_NT] = 0 + 0;
			p->rule._ar = 1;
		}
		break;
	case 2327: /* ADDRFP2 */
		/* addr: ADDRFP2 */
		if (0 + 0 < p->cost[_addr_NT]) {
			p->cost[_addr_NT] = 0 + 0;
			p->rule._addr = 6;
			_closure_addr(a, 0 + 0);
		}
		break;
	case 2343: /* ADDRLP2 */
		/* addr: ADDRLP2 */
		if (0 + 0 < p->cost[_addr_NT]) {
			p->cost[_addr_NT] = 0 + 0;
			p->rule._addr = 7;
			_closure_addr(a, 0 + 0);
		}
		break;
	case 2357: /* ADDI2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* addr: ADDI2(reg,acon) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_acon_NT] + 0;
		if (c + 0 < p->cost[_addr_NT]) {
			p->cost[_addr_NT] = c + 0;
			p->rule._addr = 1;
			_closure_addr(a, c + 0);
		}
		/* reg: ADDI2(reg,consm) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_consm_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 11;
			_closure_reg(a, c + 0);
		}
		if (	/* reg: ADDI2(reg,INDIRI2(addr)) */
			RIGHT_CHILD(a)->op == 2117 /* INDIRI2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_addr_NT] + 5;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 53;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: ADDI2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 10;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 54;
			_closure_reg(a, c + 0);
		}
		/* reg: ADDI2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 77;
			_closure_reg(a, c + 0);
		}
		if (	/* reg: ADDI2(CVUI2(reg),CVUI2(reg)) */
			LEFT_CHILD(a)->op == 2229 && /* CVUI2 */
			RIGHT_CHILD(a)->op == 2229 /* CVUI2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_reg_NT] + 5;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 78;
				_closure_reg(a, c + 0);
			}
		}
		break;
	case 2358: /* ADDU2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* addr: ADDU2(reg,acon) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_acon_NT] + 0;
		if (c + 0 < p->cost[_addr_NT]) {
			p->cost[_addr_NT] = c + 0;
			p->rule._addr = 2;
			_closure_addr(a, c + 0);
		}
		/* reg: ADDU2(reg,consm) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_consm_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 12;
			_closure_reg(a, c + 0);
		}
		/* reg: ADDU2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 58;
			_closure_reg(a, c + 0);
		}
		/* reg: ADDU2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 81;
			_closure_reg(a, c + 0);
		}
		break;
	case 2359: /* ADDP2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* addr: ADDP2(reg,acon) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_acon_NT] + 0;
		if (c + 0 < p->cost[_addr_NT]) {
			p->cost[_addr_NT] = c + 0;
			p->rule._addr = 3;
			_closure_addr(a, c + 0);
		}
		/* reg: ADDP2(reg,consm) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_consm_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 13;
			_closure_reg(a, c + 0);
		}
		if (	/* reg: ADDP2(reg,INDIRP2(addr)) */
			RIGHT_CHILD(a)->op == 2119 /* INDIRP2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_addr_NT] + 5;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 56;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: ADDP2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 10;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 57;
			_closure_reg(a, c + 0);
		}
		/* reg: ADDP2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 80;
			_closure_reg(a, c + 0);
		}
		break;
	case 2373: /* SUBI2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: SUBI2(reg,consm) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_consm_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 14;
			_closure_reg(a, c + 0);
		}
		/* reg: SUBI2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 72;
			_closure_reg(a, c + 0);
		}
		/* reg: SUBI2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 95;
			_closure_reg(a, c + 0);
		}
		break;
	case 2374: /* SUBU2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: SUBU2(reg,consm) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_consm_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 15;
			_closure_reg(a, c + 0);
		}
		/* reg: SUBU2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 75;
			_closure_reg(a, c + 0);
		}
		/* reg: SUBU2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 98;
			_closure_reg(a, c + 0);
		}
		break;
	case 2375: /* SUBP2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: SUBP2(reg,consm) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_consm_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 16;
			_closure_reg(a, c + 0);
		}
		/* reg: SUBP2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 74;
			_closure_reg(a, c + 0);
		}
		/* reg: SUBP2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 97;
			_closure_reg(a, c + 0);
		}
		break;
	case 2389: /* LSHI2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: LSHI2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 100;
			_closure_reg(a, c + 0);
		}
		/* reg: LSHI2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 108;
			_closure_reg(a, c + 0);
		}
		break;
	case 2390: /* LSHU2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: LSHU2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 102;
			_closure_reg(a, c + 0);
		}
		/* reg: LSHU2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 110;
			_closure_reg(a, c + 0);
		}
		break;
	case 2405: /* MODI2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: MODI2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 43;
			_closure_reg(a, c + 0);
		}
		break;
	case 2406: /* MODU2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: MODU2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 45;
			_closure_reg(a, c + 0);
		}
		break;
	case 2421: /* RSHI2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: RSHI2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 104;
			_closure_reg(a, c + 0);
		}
		/* reg: RSHI2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 112;
			_closure_reg(a, c + 0);
		}
		break;
	case 2422: /* RSHU2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: RSHU2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 106;
			_closure_reg(a, c + 0);
		}
		/* reg: RSHU2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 114;
			_closure_reg(a, c + 0);
		}
		break;
	case 2437: /* BANDI2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: BANDI2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 60;
			_closure_reg(a, c + 0);
		}
		/* reg: BANDI2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 83;
			_closure_reg(a, c + 0);
		}
		break;
	case 2438: /* BANDU2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: BANDU2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 66;
			_closure_reg(a, c + 0);
		}
		/* reg: BANDU2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 89;
			_closure_reg(a, c + 0);
		}
		break;
	case 2453: /* BCOMI2 */
		_label(LEFT_CHILD(a));
		/* reg: BCOMI2(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 116;
			_closure_reg(a, c + 0);
		}
		break;
	case 2454: /* BCOMU2 */
		_label(LEFT_CHILD(a));
		/* reg: BCOMU2(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 118;
			_closure_reg(a, c + 0);
		}
		break;
	case 2469: /* BORI2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: BORI2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 62;
			_closure_reg(a, c + 0);
		}
		/* reg: BORI2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 85;
			_closure_reg(a, c + 0);
		}
		break;
	case 2470: /* BORU2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: BORU2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 68;
			_closure_reg(a, c + 0);
		}
		/* reg: BORU2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 91;
			_closure_reg(a, c + 0);
		}
		break;
	case 2485: /* BXORI2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: BXORI2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 64;
			_closure_reg(a, c + 0);
		}
		/* reg: BXORI2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 0;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 87;
			_closure_reg(a, c + 0);
		}
		break;
	case 2486: /* BXORU2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: BXORU2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 70;
			_closure_reg(a, c + 0);
		}
		/* reg: BXORU2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 93;
			_closure_reg(a, c + 0);
		}
		break;
	case 2501: /* DIVI2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: DIVI2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 38;
			_closure_reg(a, c + 0);
		}
		break;
	case 2502: /* DIVU2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: DIVU2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 41;
			_closure_reg(a, c + 0);
		}
		break;
	case 2517: /* MULI2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: MULI2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 10;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 47;
			_closure_reg(a, c + 0);
		}
		/* reg: MULI2(con8bit,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_con8bit_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + (wjrMulInlineWeight);
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 48;
			_closure_reg(a, c + 0);
		}
		break;
	case 2518: /* MULU2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: MULU2(con8bit,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_con8bit_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + (wjrMulInlineWeight);
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 49;
			_closure_reg(a, c + 0);
		}
		/* reg: MULU2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 51;
			_closure_reg(a, c + 0);
		}
		break;
	case 2533: /* EQI2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: EQI2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 33;
		}
		if (	/* stmt: EQI2(CVUI2(reg),con8bit) */
			LEFT_CHILD(a)->op == 2229 /* CVUI2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con8bit_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 60;
			}
		}
		/* stmt: EQI2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 61;
		}
		if (	/* stmt: EQI2(CVUI2(reg),con0) */
			LEFT_CHILD(a)->op == 2229 /* CVUI2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con0_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 86;
			}
		}
		/* stmt: EQI2(reg,con0) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con0_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 87;
		}
		break;
	case 2534: /* EQU2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: EQU2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 34;
		}
		/* stmt: EQU2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 63;
		}
		/* stmt: EQU2(reg,con0) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con0_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 88;
		}
		if (	/* stmt: EQU2(CVUI2(reg),con0) */
			LEFT_CHILD(a)->op == 2229 /* CVUI2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con0_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 89;
			}
		}
		break;
	case 2549: /* GEI2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: GEI2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 35;
		}
		if (	/* stmt: GEI2(CVUI2(reg),con) */
			LEFT_CHILD(a)->op == 2229 /* CVUI2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 65;
			}
		}
		/* stmt: GEI2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 66;
		}
		break;
	case 2550: /* GEU2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: GEU2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 36;
		}
		/* stmt: GEU2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 68;
		}
		break;
	case 2565: /* GTI2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: GTI2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 37;
		}
		/* stmt: GTI2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 70;
		}
		break;
	case 2566: /* GTU2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: GTU2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 38;
		}
		/* stmt: GTU2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 72;
		}
		break;
	case 2581: /* LEI2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		if (	/* stmt: LEI2(CVUI2(reg),CVUI2(reg)) */
			LEFT_CHILD(a)->op == 2229 && /* CVUI2 */
			RIGHT_CHILD(a)->op == 2229 /* CVUI2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_reg_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 39;
			}
		}
		/* stmt: LEI2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 40;
		}
		/* stmt: LEI2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 74;
		}
		break;
	case 2582: /* LEU2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: LEU2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 41;
		}
		/* stmt: LEU2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 76;
		}
		break;
	case 2597: /* LTI2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		if (	/* stmt: LTI2(CVUI2(reg),CVUI2(reg)) */
			LEFT_CHILD(a)->op == 2229 && /* CVUI2 */
			RIGHT_CHILD(a)->op == 2229 /* CVUI2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_reg_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 42;
			}
		}
		/* stmt: LTI2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 43;
		}
		if (	/* stmt: LTI2(CVUI2(reg),con) */
			LEFT_CHILD(a)->op == 2229 /* CVUI2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 78;
			}
		}
		/* stmt: LTI2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 79;
		}
		break;
	case 2598: /* LTU2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: LTU2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 44;
		}
		if (	/* stmt: LTU2(CVUI2(reg),con) */
			LEFT_CHILD(a)->op == 2229 /* CVUI2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 81;
			}
		}
		/* stmt: LTU2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 82;
		}
		break;
	case 2613: /* NEI2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		if (	/* stmt: NEI2(CVUI2(reg),CVUI2(reg)) */
			LEFT_CHILD(a)->op == 2229 && /* CVUI2 */
			RIGHT_CHILD(a)->op == 2229 /* CVUI2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_reg_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 45;
			}
		}
		/* stmt: NEI2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 46;
		}
		if (	/* stmt: NEI2(CVUI2(reg),con0) */
			LEFT_CHILD(a)->op == 2229 /* CVUI2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con0_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 84;
			}
		}
		/* stmt: NEI2(reg,con0) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con0_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 85;
		}
		/* stmt: NEI2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 90;
		}
		if (	/* stmt: NEI2(CVUI2(reg),con8bit) */
			LEFT_CHILD(a)->op == 2229 /* CVUI2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con8bit_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 95;
			}
		}
		break;
	case 2614: /* NEU2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: NEU2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 47;
		}
		/* stmt: NEU2(reg,con0) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con0_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 92;
		}
		/* stmt: NEU2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 93;
		}
		break;
	case 4113: /* CNSTF4 */
		/* con0: CNSTF4 */
		c = (range(a,0,0));
		if (c + 0 < p->cost[_con0_NT]) {
			p->cost[_con0_NT] = c + 0;
			p->rule._con0 = 1;
			_closure_con0(a, c + 0);
		}
		break;
	case 4117: /* CNSTI4 */
		/* con2: CNSTI4 */
		c = (range(a,2,2));
		if (c + 0 < p->cost[_con2_NT]) {
			p->cost[_con2_NT] = c + 0;
			p->rule._con2 = 4;
		}
		/* reg: CNSTI4 */
		if (2 + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = 2 + 0;
			p->rule._reg = 9;
			_closure_reg(a, 2 + 0);
		}
		break;
	case 4118: /* CNSTU4 */
		/* con2: CNSTU4 */
		c = (range(a,2,2));
		if (c + 0 < p->cost[_con2_NT]) {
			p->cost[_con2_NT] = c + 0;
			p->rule._con2 = 3;
		}
		/* reg: CNSTU4 */
		if (2 + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = 2 + 0;
			p->rule._reg = 10;
			_closure_reg(a, 2 + 0);
		}
		break;
	case 4129: /* ARGF4 */
		_label(LEFT_CHILD(a));
		/* stmt: ARGF4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (fp());
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 110;
		}
		break;
	case 4133: /* ARGI4 */
		_label(LEFT_CHILD(a));
		/* stmt: ARGI4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 112;
		}
		break;
	case 4134: /* ARGU4 */
		_label(LEFT_CHILD(a));
		/* stmt: ARGU4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 115;
		}
		break;
	case 4145: /* ASGNF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		if (	/* stmt: ASGNF4(VREGP,reg) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			c = ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + (fp());
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 6;
			}
		}
		/* stmt: ASGNF4(addr,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + (fp());
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 28;
		}
		break;
	case 4149: /* ASGNI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		if (	/* stmt: ASGNI4(VREGP,reg) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			c = ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 0;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 7;
			}
		}
		/* stmt: ASGNI4(addr,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 26;
		}
		break;
	case 4150: /* ASGNU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		if (	/* stmt: ASGNU4(VREGP,reg) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			c = ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 0;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 8;
			}
		}
		/* stmt: ASGNU4(addr,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 27;
		}
		break;
	case 4161: /* INDIRF4 */
		_label(LEFT_CHILD(a));
		if (	/* reg: INDIRF4(VREGP) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			if (mayrecalc(a)) {
				struct _state *q = a->syms[RX]->u.t.cse->x.state;
				if (q->cost[_stmt_NT] == 0) {
					p->cost[_stmt_NT] = 0;
					p->rule._stmt = q->rule._stmt;
				}
				if (q->cost[_reg_NT] == 0) {
					p->cost[_reg_NT] = 0;
					p->rule._reg = q->rule._reg;
				}
				if (q->cost[_con0_NT] == 0) {
					p->cost[_con0_NT] = 0;
					p->rule._con0 = q->rule._con0;
				}
				if (q->cost[_con2_NT] == 0) {
					p->cost[_con2_NT] = 0;
					p->rule._con2 = q->rule._con2;
				}
				if (q->cost[_consm_NT] == 0) {
					p->cost[_consm_NT] = 0;
					p->rule._consm = q->rule._consm;
				}
				if (q->cost[_con8bit_NT] == 0) {
					p->cost[_con8bit_NT] = 0;
					p->rule._con8bit = q->rule._con8bit;
				}
				if (q->cost[_con_NT] == 0) {
					p->cost[_con_NT] = 0;
					p->rule._con = q->rule._con;
				}
				if (q->cost[_acon_NT] == 0) {
					p->cost[_acon_NT] = 0;
					p->rule._acon = q->rule._acon;
				}
				if (q->cost[_addr_NT] == 0) {
					p->cost[_addr_NT] = 0;
					p->rule._addr = q->rule._addr;
				}
				if (q->cost[_indaddr_NT] == 0) {
					p->cost[_indaddr_NT] = 0;
					p->rule._indaddr = q->rule._indaddr;
				}
				if (q->cost[_ar_NT] == 0) {
					p->cost[_ar_NT] = 0;
					p->rule._ar = q->rule._ar;
				}
			}
			c = (fp());
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 5;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: INDIRF4(addr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + (fp());
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 130;
			_closure_reg(a, c + 0);
		}
		break;
	case 4165: /* INDIRI4 */
		_label(LEFT_CHILD(a));
		if (	/* reg: INDIRI4(VREGP) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			if (mayrecalc(a)) {
				struct _state *q = a->syms[RX]->u.t.cse->x.state;
				if (q->cost[_stmt_NT] == 0) {
					p->cost[_stmt_NT] = 0;
					p->rule._stmt = q->rule._stmt;
				}
				if (q->cost[_reg_NT] == 0) {
					p->cost[_reg_NT] = 0;
					p->rule._reg = q->rule._reg;
				}
				if (q->cost[_con0_NT] == 0) {
					p->cost[_con0_NT] = 0;
					p->rule._con0 = q->rule._con0;
				}
				if (q->cost[_con2_NT] == 0) {
					p->cost[_con2_NT] = 0;
					p->rule._con2 = q->rule._con2;
				}
				if (q->cost[_consm_NT] == 0) {
					p->cost[_consm_NT] = 0;
					p->rule._consm = q->rule._consm;
				}
				if (q->cost[_con8bit_NT] == 0) {
					p->cost[_con8bit_NT] = 0;
					p->rule._con8bit = q->rule._con8bit;
				}
				if (q->cost[_con_NT] == 0) {
					p->cost[_con_NT] = 0;
					p->rule._con = q->rule._con;
				}
				if (q->cost[_acon_NT] == 0) {
					p->cost[_acon_NT] = 0;
					p->rule._acon = q->rule._acon;
				}
				if (q->cost[_addr_NT] == 0) {
					p->cost[_addr_NT] = 0;
					p->rule._addr = q->rule._addr;
				}
				if (q->cost[_indaddr_NT] == 0) {
					p->cost[_indaddr_NT] = 0;
					p->rule._indaddr = q->rule._indaddr;
				}
				if (q->cost[_ar_NT] == 0) {
					p->cost[_ar_NT] = 0;
					p->rule._ar = q->rule._ar;
				}
			}
			c = 0;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 6;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: INDIRI4(addr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 27;
			_closure_reg(a, c + 0);
		}
		break;
	case 4166: /* INDIRU4 */
		_label(LEFT_CHILD(a));
		if (	/* reg: INDIRU4(VREGP) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			if (mayrecalc(a)) {
				struct _state *q = a->syms[RX]->u.t.cse->x.state;
				if (q->cost[_stmt_NT] == 0) {
					p->cost[_stmt_NT] = 0;
					p->rule._stmt = q->rule._stmt;
				}
				if (q->cost[_reg_NT] == 0) {
					p->cost[_reg_NT] = 0;
					p->rule._reg = q->rule._reg;
				}
				if (q->cost[_con0_NT] == 0) {
					p->cost[_con0_NT] = 0;
					p->rule._con0 = q->rule._con0;
				}
				if (q->cost[_con2_NT] == 0) {
					p->cost[_con2_NT] = 0;
					p->rule._con2 = q->rule._con2;
				}
				if (q->cost[_consm_NT] == 0) {
					p->cost[_consm_NT] = 0;
					p->rule._consm = q->rule._consm;
				}
				if (q->cost[_con8bit_NT] == 0) {
					p->cost[_con8bit_NT] = 0;
					p->rule._con8bit = q->rule._con8bit;
				}
				if (q->cost[_con_NT] == 0) {
					p->cost[_con_NT] = 0;
					p->rule._con = q->rule._con;
				}
				if (q->cost[_acon_NT] == 0) {
					p->cost[_acon_NT] = 0;
					p->rule._acon = q->rule._acon;
				}
				if (q->cost[_addr_NT] == 0) {
					p->cost[_addr_NT] = 0;
					p->rule._addr = q->rule._addr;
				}
				if (q->cost[_indaddr_NT] == 0) {
					p->cost[_indaddr_NT] = 0;
					p->rule._indaddr = q->rule._indaddr;
				}
				if (q->cost[_ar_NT] == 0) {
					p->cost[_ar_NT] = 0;
					p->rule._ar = q->rule._ar;
				}
			}
			c = 0;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 8;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: INDIRU4(addr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 28;
			_closure_reg(a, c + 0);
		}
		break;
	case 4209: /* CVFF4 */
		_label(LEFT_CHILD(a));
		/* reg: CVFF4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (fp());
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 146;
			_closure_reg(a, c + 0);
		}
		break;
	case 4213: /* CVFI4 */
		_label(LEFT_CHILD(a));
		/* reg: CVFI4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (fp());
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 149;
			_closure_reg(a, c + 0);
		}
		break;
	case 4225: /* CVIF4 */
		_label(LEFT_CHILD(a));
		/* reg: CVIF4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (fp());
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 147;
			_closure_reg(a, c + 0);
		}
		break;
	case 4229: /* CVII4 */
		_label(LEFT_CHILD(a));
		if (	/* reg: CVII4(INDIRI1(addr)) */
			LEFT_CHILD(a)->op == 1093 /* INDIRI1 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_addr_NT] + 1;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 32;
				_closure_reg(a, c + 0);
			}
		}
		if (	/* reg: CVII4(INDIRI2(addr)) */
			LEFT_CHILD(a)->op == 2117 /* INDIRI2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_addr_NT] + 1;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 33;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: CVII4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 141;
			_closure_reg(a, c + 0);
		}
		break;
	case 4230: /* CVIU4 */
		_label(LEFT_CHILD(a));
		/* reg: CVIU4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 142;
			_closure_reg(a, c + 0);
		}
		if (	/* reg: CVIU4(CVUI2(INDIRU1(addr))) */
			LEFT_CHILD(a)->op == 2229 && /* CVUI2 */
			LEFT_CHILD(LEFT_CHILD(a))->op == 1094 /* INDIRU1 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(LEFT_CHILD(a)))->x.state))->cost[_addr_NT] + 1;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 143;
				_closure_reg(a, c + 0);
			}
		}
		break;
	case 4277: /* CVUI4 */
		_label(LEFT_CHILD(a));
		if (	/* reg: CVUI4(INDIRU1(addr)) */
			LEFT_CHILD(a)->op == 1094 /* INDIRU1 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_addr_NT] + 1;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 36;
				_closure_reg(a, c + 0);
			}
		}
		if (	/* reg: CVUI4(INDIRU2(addr)) */
			LEFT_CHILD(a)->op == 2118 /* INDIRU2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_addr_NT] + 1;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 37;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: CVUI4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 144;
			_closure_reg(a, c + 0);
		}
		break;
	case 4278: /* CVUU4 */
		_label(LEFT_CHILD(a));
		if (	/* reg: CVUU4(INDIRU1(addr)) */
			LEFT_CHILD(a)->op == 1094 /* INDIRU1 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_addr_NT] + 1;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 34;
				_closure_reg(a, c + 0);
			}
		}
		if (	/* reg: CVUU4(INDIRU2(addr)) */
			LEFT_CHILD(a)->op == 2118 /* INDIRU2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_addr_NT] + 1;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 35;
				_closure_reg(a, c + 0);
			}
		}
		/* reg: CVUU4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 145;
			_closure_reg(a, c + 0);
		}
		break;
	case 4289: /* NEGF4 */
		_label(LEFT_CHILD(a));
		/* reg: NEGF4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (fp());
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 136;
			_closure_reg(a, c + 0);
		}
		break;
	case 4293: /* NEGI4 */
		_label(LEFT_CHILD(a));
		/* reg: NEGI4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 121;
			_closure_reg(a, c + 0);
		}
		break;
	case 4305: /* CALLF4 */
		_label(LEFT_CHILD(a));
		/* reg: CALLF4(ar) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_ar_NT] + (fp());
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 150;
			_closure_reg(a, c + 0);
		}
		break;
	case 4309: /* CALLI4 */
		_label(LEFT_CHILD(a));
		/* reg: CALLI4(ar) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_ar_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 152;
			_closure_reg(a, c + 0);
		}
		break;
	case 4310: /* CALLU4 */
		_label(LEFT_CHILD(a));
		/* reg: CALLU4(ar) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_ar_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 155;
			_closure_reg(a, c + 0);
		}
		break;
	case 4321: /* LOADF4 */
		_label(LEFT_CHILD(a));
		/* reg: LOADF4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (fp()+move(a));
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 135;
			_closure_reg(a, c + 0);
		}
		break;
	case 4325: /* LOADI4 */
		_label(LEFT_CHILD(a));
		/* reg: LOADI4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (move(a)+1);
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 127;
			_closure_reg(a, c + 0);
		}
		break;
	case 4326: /* LOADU4 */
		_label(LEFT_CHILD(a));
		/* reg: LOADU4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (move(a)+1);
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 129;
			_closure_reg(a, c + 0);
		}
		break;
	case 4337: /* RETF4 */
		_label(LEFT_CHILD(a));
		/* stmt: RETF4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (fp());
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 103;
		}
		break;
	case 4341: /* RETI4 */
		_label(LEFT_CHILD(a));
		/* stmt: RETI4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 105;
		}
		break;
	case 4342: /* RETU4 */
		_label(LEFT_CHILD(a));
		/* stmt: RETU4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 107;
		}
		break;
	case 4401: /* ADDF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: ADDF4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + (fp());
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 131;
			_closure_reg(a, c + 0);
		}
		break;
	case 4405: /* ADDI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: ADDI4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 55;
			_closure_reg(a, c + 0);
		}
		/* reg: ADDI4(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 79;
			_closure_reg(a, c + 0);
		}
		break;
	case 4406: /* ADDU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: ADDU4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 59;
			_closure_reg(a, c + 0);
		}
		/* reg: ADDU4(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 82;
			_closure_reg(a, c + 0);
		}
		break;
	case 4417: /* SUBF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: SUBF4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + (fp());
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 134;
			_closure_reg(a, c + 0);
		}
		break;
	case 4421: /* SUBI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: SUBI4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 73;
			_closure_reg(a, c + 0);
		}
		/* reg: SUBI4(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 96;
			_closure_reg(a, c + 0);
		}
		break;
	case 4422: /* SUBU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: SUBU4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 76;
			_closure_reg(a, c + 0);
		}
		/* reg: SUBU4(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 99;
			_closure_reg(a, c + 0);
		}
		break;
	case 4437: /* LSHI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: LSHI4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 10;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 101;
			_closure_reg(a, c + 0);
		}
		/* reg: LSHI4(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 109;
			_closure_reg(a, c + 0);
		}
		break;
	case 4438: /* LSHU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: LSHU4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 10;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 103;
			_closure_reg(a, c + 0);
		}
		/* reg: LSHU4(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 111;
			_closure_reg(a, c + 0);
		}
		break;
	case 4453: /* MODI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: MODI4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 44;
			_closure_reg(a, c + 0);
		}
		break;
	case 4454: /* MODU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: MODU4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 46;
			_closure_reg(a, c + 0);
		}
		break;
	case 4469: /* RSHI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: RSHI4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 10;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 105;
			_closure_reg(a, c + 0);
		}
		/* reg: RSHI4(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 113;
			_closure_reg(a, c + 0);
		}
		break;
	case 4470: /* RSHU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: RSHU4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 10;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 107;
			_closure_reg(a, c + 0);
		}
		/* reg: RSHU4(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 115;
			_closure_reg(a, c + 0);
		}
		break;
	case 4485: /* BANDI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: BANDI4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 61;
			_closure_reg(a, c + 0);
		}
		/* reg: BANDI4(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 84;
			_closure_reg(a, c + 0);
		}
		break;
	case 4486: /* BANDU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: BANDU4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 67;
			_closure_reg(a, c + 0);
		}
		/* reg: BANDU4(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 90;
			_closure_reg(a, c + 0);
		}
		break;
	case 4501: /* BCOMI4 */
		_label(LEFT_CHILD(a));
		/* reg: BCOMI4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 117;
			_closure_reg(a, c + 0);
		}
		break;
	case 4502: /* BCOMU4 */
		_label(LEFT_CHILD(a));
		/* reg: BCOMU4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 119;
			_closure_reg(a, c + 0);
		}
		break;
	case 4517: /* BORI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: BORI4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 63;
			_closure_reg(a, c + 0);
		}
		/* reg: BORI4(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 86;
			_closure_reg(a, c + 0);
		}
		break;
	case 4518: /* BORU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: BORU4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 69;
			_closure_reg(a, c + 0);
		}
		/* reg: BORU4(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 92;
			_closure_reg(a, c + 0);
		}
		break;
	case 4533: /* BXORI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: BXORI4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 65;
			_closure_reg(a, c + 0);
		}
		/* reg: BXORI4(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 88;
			_closure_reg(a, c + 0);
		}
		break;
	case 4534: /* BXORU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: BXORU4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 71;
			_closure_reg(a, c + 0);
		}
		/* reg: BXORU4(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 94;
			_closure_reg(a, c + 0);
		}
		break;
	case 4545: /* DIVF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: DIVF4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + (fp());
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 132;
			_closure_reg(a, c + 0);
		}
		break;
	case 4549: /* DIVI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: DIVI4(reg,con2) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con2_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 39;
			_closure_reg(a, c + 0);
		}
		/* reg: DIVI4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 40;
			_closure_reg(a, c + 0);
		}
		break;
	case 4550: /* DIVU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: DIVU4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 42;
			_closure_reg(a, c + 0);
		}
		break;
	case 4561: /* MULF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: MULF4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + (fp());
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 133;
			_closure_reg(a, c + 0);
		}
		break;
	case 4565: /* MULI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: MULI4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 50;
			_closure_reg(a, c + 0);
		}
		break;
	case 4566: /* MULU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: MULU4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 4;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 52;
			_closure_reg(a, c + 0);
		}
		break;
	case 4577: /* EQF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: EQF4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + (fp());
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 96;
		}
		break;
	case 4581: /* EQI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: EQI4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 48;
		}
		/* stmt: EQI4(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 62;
		}
		break;
	case 4582: /* EQU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: EQU4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 49;
		}
		/* stmt: EQU4(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 64;
		}
		break;
	case 4593: /* GEF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: GEF4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + (fp()+10);
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 98;
		}
		break;
	case 4597: /* GEI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: GEI4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 50;
		}
		/* stmt: GEI4(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 67;
		}
		break;
	case 4598: /* GEU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: GEU4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 51;
		}
		/* stmt: GEU4(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 69;
		}
		break;
	case 4609: /* GTF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: GTF4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + (fp());
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 97;
		}
		break;
	case 4613: /* GTI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: GTI4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 52;
		}
		/* stmt: GTI4(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 71;
		}
		break;
	case 4614: /* GTU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: GTU4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 53;
		}
		/* stmt: GTU4(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 73;
		}
		break;
	case 4625: /* LEF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: LEF4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + (fp());
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 99;
		}
		break;
	case 4629: /* LEI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: LEI4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 54;
		}
		/* stmt: LEI4(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 75;
		}
		break;
	case 4630: /* LEU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: LEU4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 55;
		}
		/* stmt: LEU4(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 77;
		}
		break;
	case 4641: /* LTF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: LTF4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + (fp()+10);
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 100;
		}
		break;
	case 4645: /* LTI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: LTI4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 56;
		}
		/* stmt: LTI4(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 80;
		}
		break;
	case 4646: /* LTU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: LTU4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 57;
		}
		/* stmt: LTU4(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 83;
		}
		break;
	case 4657: /* NEF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: NEF4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + (fp());
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 101;
		}
		break;
	case 4661: /* NEI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: NEI4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 58;
		}
		/* stmt: NEI4(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 91;
		}
		break;
	case 4662: /* NEU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: NEU4(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 59;
		}
		/* stmt: NEU4(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 94;
		}
		break;
	default:
		fatal("_label", "Bad terminal %d\n", OP_LABEL(a));
	}
}

static void _kids(NODEPTR_TYPE p, int eruleno, NODEPTR_TYPE kids[]) {
	if (!p)
		fatal("_kids", "Null tree\n", 0);
	if (!kids)
		fatal("_kids", "Null kids\n", 0);
	switch (eruleno) {
	case 303: /* stmt: IASMV */
	case 287: /* ar: CNSTP2 */
	case 278: /* ar: ADDRGP2 */
	case 205: /* stmt: LABELV */
	case 46: /* addr: ADDRLP2 */
	case 45: /* addr: ADDRFP2 */
	case 38: /* acon: ADDRGP2 */
	case 35: /* reg: CNSTU4 */
	case 34: /* reg: CNSTI4 */
	case 33: /* con: CNSTP2 */
	case 32: /* con: CNSTU2 */
	case 31: /* con: CNSTI2 */
	case 30: /* con: CNSTU1 */
	case 29: /* con: CNSTI1 */
	case 28: /* con8bit: CNSTU2 */
	case 27: /* con8bit: CNSTI2 */
	case 26: /* consm: CNSTP2 */
	case 25: /* consm: CNSTI2 */
	case 24: /* consm: CNSTU2 */
	case 23: /* con2: CNSTI4 */
	case 22: /* con2: CNSTU4 */
	case 21: /* con2: CNSTI2 */
	case 20: /* con2: CNSTU2 */
	case 19: /* con0: CNSTI2 */
	case 18: /* con0: CNSTU2 */
	case 17: /* con0: CNSTF4 */
	case 8: /* reg: INDIRU4(VREGP) */
	case 7: /* reg: INDIRP2(VREGP) */
	case 6: /* reg: INDIRI4(VREGP) */
	case 5: /* reg: INDIRF4(VREGP) */
	case 4: /* reg: INDIRU2(VREGP) */
	case 3: /* reg: INDIRI2(VREGP) */
	case 2: /* reg: INDIRU1(VREGP) */
	case 1: /* reg: INDIRI1(VREGP) */
		break;
	case 16: /* stmt: ASGNU4(VREGP,reg) */
	case 15: /* stmt: ASGNI4(VREGP,reg) */
	case 14: /* stmt: ASGNF4(VREGP,reg) */
	case 13: /* stmt: ASGNP2(VREGP,reg) */
	case 12: /* stmt: ASGNU2(VREGP,reg) */
	case 11: /* stmt: ASGNI2(VREGP,reg) */
	case 10: /* stmt: ASGNU1(VREGP,reg) */
	case 9: /* stmt: ASGNI1(VREGP,reg) */
		kids[0] = RIGHT_CHILD(p);
		break;
	case 286: /* ar: reg */
	case 55: /* reg: addr */
	case 54: /* reg: acon */
	case 53: /* reg: con0 */
	case 44: /* indaddr: reg */
	case 43: /* addr: reg */
	case 42: /* addr: acon */
	case 37: /* acon: con */
	case 36: /* stmt: reg */
		kids[0] = p;
		break;
	case 277: /* stmt: NEF4(reg,reg) */
	case 276: /* stmt: LTF4(reg,reg) */
	case 275: /* stmt: LEF4(reg,reg) */
	case 274: /* stmt: GEF4(reg,reg) */
	case 273: /* stmt: GTF4(reg,reg) */
	case 272: /* stmt: EQF4(reg,reg) */
	case 270: /* stmt: NEU4(reg,con) */
	case 269: /* stmt: NEU2(reg,con) */
	case 268: /* stmt: NEU2(reg,con0) */
	case 267: /* stmt: NEI4(reg,con) */
	case 266: /* stmt: NEI2(reg,con) */
	case 264: /* stmt: EQU2(reg,con0) */
	case 263: /* stmt: EQI2(reg,con0) */
	case 261: /* stmt: NEI2(reg,con0) */
	case 259: /* stmt: LTU4(reg,con) */
	case 258: /* stmt: LTU2(reg,con) */
	case 256: /* stmt: LTI4(reg,con) */
	case 255: /* stmt: LTI2(reg,con) */
	case 253: /* stmt: LEU4(reg,con) */
	case 252: /* stmt: LEU2(reg,con) */
	case 251: /* stmt: LEI4(reg,con) */
	case 250: /* stmt: LEI2(reg,con) */
	case 249: /* stmt: GTU4(reg,con) */
	case 248: /* stmt: GTU2(reg,con) */
	case 247: /* stmt: GTI4(reg,con) */
	case 246: /* stmt: GTI2(reg,con) */
	case 245: /* stmt: GEU4(reg,con) */
	case 244: /* stmt: GEU2(reg,con) */
	case 243: /* stmt: GEI4(reg,con) */
	case 242: /* stmt: GEI2(reg,con) */
	case 240: /* stmt: EQU4(reg,con) */
	case 239: /* stmt: EQU2(reg,con) */
	case 238: /* stmt: EQI4(reg,con) */
	case 237: /* stmt: EQI2(reg,con) */
	case 235: /* stmt: NEU4(reg,reg) */
	case 234: /* stmt: NEI4(reg,reg) */
	case 233: /* stmt: LTU4(reg,reg) */
	case 232: /* stmt: LTI4(reg,reg) */
	case 231: /* stmt: LEU4(reg,reg) */
	case 230: /* stmt: LEI4(reg,reg) */
	case 229: /* stmt: GTU4(reg,reg) */
	case 228: /* stmt: GTI4(reg,reg) */
	case 227: /* stmt: GEU4(reg,reg) */
	case 226: /* stmt: GEI4(reg,reg) */
	case 225: /* stmt: EQU4(reg,reg) */
	case 224: /* stmt: EQI4(reg,reg) */
	case 223: /* stmt: NEU2(reg,reg) */
	case 222: /* stmt: NEI2(reg,reg) */
	case 220: /* stmt: LTU2(reg,reg) */
	case 219: /* stmt: LTI2(reg,reg) */
	case 217: /* stmt: LEU2(reg,reg) */
	case 216: /* stmt: LEI2(reg,reg) */
	case 214: /* stmt: GTU2(reg,reg) */
	case 213: /* stmt: GTI2(reg,reg) */
	case 212: /* stmt: GEU2(reg,reg) */
	case 211: /* stmt: GEI2(reg,reg) */
	case 210: /* stmt: EQU2(reg,reg) */
	case 209: /* stmt: EQI2(reg,reg) */
	case 189: /* reg: SUBF4(reg,reg) */
	case 188: /* reg: MULF4(reg,reg) */
	case 187: /* reg: DIVF4(reg,reg) */
	case 186: /* reg: ADDF4(reg,reg) */
	case 185: /* stmt: ASGNF4(addr,reg) */
	case 169: /* reg: RSHU4(reg,con) */
	case 168: /* reg: RSHU2(reg,con) */
	case 167: /* reg: RSHI4(reg,con) */
	case 166: /* reg: RSHI2(reg,con) */
	case 165: /* reg: LSHU4(reg,con) */
	case 164: /* reg: LSHU2(reg,con) */
	case 163: /* reg: LSHI4(reg,con) */
	case 162: /* reg: LSHI2(reg,con) */
	case 161: /* reg: RSHU4(reg,reg) */
	case 160: /* reg: RSHU2(reg,reg) */
	case 159: /* reg: RSHI4(reg,reg) */
	case 158: /* reg: RSHI2(reg,reg) */
	case 157: /* reg: LSHU4(reg,reg) */
	case 156: /* reg: LSHU2(reg,reg) */
	case 155: /* reg: LSHI4(reg,reg) */
	case 154: /* reg: LSHI2(reg,reg) */
	case 153: /* reg: SUBU4(reg,con) */
	case 152: /* reg: SUBU2(reg,con) */
	case 151: /* reg: SUBP2(reg,con) */
	case 150: /* reg: SUBI4(reg,con) */
	case 149: /* reg: SUBI2(reg,con) */
	case 148: /* reg: BXORU4(reg,con) */
	case 147: /* reg: BXORU2(reg,con) */
	case 146: /* reg: BORU4(reg,con) */
	case 145: /* reg: BORU2(reg,con) */
	case 144: /* reg: BANDU4(reg,con) */
	case 143: /* reg: BANDU2(reg,con) */
	case 142: /* reg: BXORI4(reg,con) */
	case 141: /* reg: BXORI2(reg,con) */
	case 140: /* reg: BORI4(reg,con) */
	case 139: /* reg: BORI2(reg,con) */
	case 138: /* reg: BANDI4(reg,con) */
	case 137: /* reg: BANDI2(reg,con) */
	case 136: /* reg: ADDU4(reg,con) */
	case 135: /* reg: ADDU2(reg,con) */
	case 134: /* reg: ADDP2(reg,con) */
	case 133: /* reg: ADDI4(reg,con) */
	case 131: /* reg: ADDI2(reg,con) */
	case 130: /* reg: SUBU4(reg,reg) */
	case 129: /* reg: SUBU2(reg,reg) */
	case 128: /* reg: SUBP2(reg,reg) */
	case 127: /* reg: SUBI4(reg,reg) */
	case 126: /* reg: SUBI2(reg,reg) */
	case 125: /* reg: BXORU4(reg,reg) */
	case 124: /* reg: BXORU2(reg,reg) */
	case 123: /* reg: BORU4(reg,reg) */
	case 122: /* reg: BORU2(reg,reg) */
	case 121: /* reg: BANDU4(reg,reg) */
	case 120: /* reg: BANDU2(reg,reg) */
	case 119: /* reg: BXORI4(reg,reg) */
	case 118: /* reg: BXORI2(reg,reg) */
	case 117: /* reg: BORI4(reg,reg) */
	case 116: /* reg: BORI2(reg,reg) */
	case 115: /* reg: BANDI4(reg,reg) */
	case 114: /* reg: BANDI2(reg,reg) */
	case 113: /* reg: ADDU4(reg,reg) */
	case 112: /* reg: ADDU2(reg,reg) */
	case 111: /* reg: ADDP2(reg,reg) */
	case 109: /* reg: ADDI4(reg,reg) */
	case 108: /* reg: ADDI2(reg,reg) */
	case 106: /* reg: MULU4(reg,reg) */
	case 105: /* reg: MULU2(reg,reg) */
	case 104: /* reg: MULI4(reg,reg) */
	case 103: /* reg: MULU2(con8bit,reg) */
	case 102: /* reg: MULI2(con8bit,reg) */
	case 101: /* reg: MULI2(reg,reg) */
	case 100: /* reg: MODU4(reg,reg) */
	case 99: /* reg: MODU2(reg,reg) */
	case 98: /* reg: MODI4(reg,reg) */
	case 97: /* reg: MODI2(reg,reg) */
	case 96: /* reg: DIVU4(reg,reg) */
	case 95: /* reg: DIVU2(reg,reg) */
	case 94: /* reg: DIVI4(reg,reg) */
	case 93: /* reg: DIVI4(reg,con2) */
	case 92: /* reg: DIVI2(reg,reg) */
	case 73: /* stmt: ASGNU4(addr,reg) */
	case 72: /* stmt: ASGNI4(addr,reg) */
	case 71: /* stmt: ASGNP2(addr,acon) */
	case 70: /* stmt: ASGNP2(addr,reg) */
	case 67: /* stmt: ASGNU2(addr,reg) */
	case 65: /* stmt: ASGNI2(addr,reg) */
	case 64: /* stmt: ASGNI2(addr,acon) */
	case 63: /* stmt: ASGNU1(addr,acon) */
	case 61: /* stmt: ASGNU1(indaddr,reg) */
	case 59: /* stmt: ASGNU1(addr,reg) */
	case 58: /* stmt: ASGNU1(indaddr,acon) */
	case 57: /* stmt: ASGNI1(indaddr,reg) */
	case 56: /* stmt: ASGNI1(addr,reg) */
	case 52: /* reg: SUBP2(reg,consm) */
	case 51: /* reg: SUBU2(reg,consm) */
	case 50: /* reg: SUBI2(reg,consm) */
	case 49: /* reg: ADDP2(reg,consm) */
	case 48: /* reg: ADDU2(reg,consm) */
	case 47: /* reg: ADDI2(reg,consm) */
	case 41: /* addr: ADDP2(reg,acon) */
	case 40: /* addr: ADDU2(reg,acon) */
	case 39: /* addr: ADDI2(reg,acon) */
		kids[0] = LEFT_CHILD(p);
		kids[1] = RIGHT_CHILD(p);
		break;
	case 302: /* stmt: ASGNB(reg,INDIRB(reg)) */
	case 110: /* reg: ADDP2(reg,INDIRP2(addr)) */
	case 107: /* reg: ADDI2(reg,INDIRI2(addr)) */
	case 69: /* stmt: ASGNI2(addr,LOADI2(reg)) */
	case 68: /* stmt: ASGNU2(addr,LOADU2(reg)) */
	case 66: /* stmt: ASGNI2(addr,LOADI2(reg)) */
	case 60: /* stmt: ASGNU1(indaddr,INDIRU1(indaddr)) */
		kids[0] = LEFT_CHILD(p);
		kids[1] = LEFT_CHILD(RIGHT_CHILD(p));
		break;
	case 62: /* stmt: ASGNU1(indaddr,LOADU1(LOADU2(reg))) */
		kids[0] = LEFT_CHILD(p);
		kids[1] = LEFT_CHILD(LEFT_CHILD(RIGHT_CHILD(p)));
		break;
	case 300: /* stmt: ARGU4(reg) */
	case 299: /* stmt: ARGU2(reg) */
	case 298: /* stmt: ARGP2(reg) */
	case 297: /* stmt: ARGI4(reg) */
	case 296: /* stmt: ARGI2(reg) */
	case 295: /* stmt: ARGF4(reg) */
	case 294: /* stmt: RETV(reg) */
	case 293: /* stmt: RETP2(reg) */
	case 292: /* stmt: RETU4(reg) */
	case 291: /* stmt: RETU2(reg) */
	case 290: /* stmt: RETI4(reg) */
	case 289: /* stmt: RETI2(reg) */
	case 288: /* stmt: RETF4(reg) */
	case 285: /* stmt: CALLV(ar) */
	case 284: /* reg: CALLU4(ar) */
	case 283: /* reg: CALLU2(ar) */
	case 282: /* reg: CALLP2(ar) */
	case 281: /* reg: CALLI4(ar) */
	case 280: /* reg: CALLI2(ar) */
	case 279: /* reg: CALLF4(ar) */
	case 208: /* stmt: JUMPV(acon) */
	case 207: /* stmt: JUMPV(acon) */
	case 206: /* stmt: JUMPV(reg) */
	case 204: /* reg: CVFI4(reg) */
	case 203: /* reg: CVFI2(reg) */
	case 202: /* reg: CVIF4(reg) */
	case 201: /* reg: CVFF4(reg) */
	case 200: /* reg: CVUU4(reg) */
	case 199: /* reg: CVUI4(reg) */
	case 197: /* reg: CVIU4(reg) */
	case 196: /* reg: CVII4(reg) */
	case 195: /* reg: CVUU2(reg) */
	case 194: /* reg: CVUI2(reg) */
	case 193: /* reg: CVIU2(reg) */
	case 192: /* reg: CVII2(reg) */
	case 191: /* reg: NEGF4(reg) */
	case 190: /* reg: LOADF4(reg) */
	case 184: /* reg: INDIRF4(addr) */
	case 183: /* reg: LOADU4(reg) */
	case 182: /* reg: LOADP2(reg) */
	case 181: /* reg: LOADI4(reg) */
	case 180: /* reg: LOADU2(reg) */
	case 178: /* reg: LOADI2(reg) */
	case 177: /* reg: LOADU1(reg) */
	case 176: /* reg: LOADI1(reg) */
	case 175: /* reg: NEGI4(reg) */
	case 174: /* reg: NEGI2(reg) */
	case 173: /* reg: BCOMU4(reg) */
	case 172: /* reg: BCOMU2(reg) */
	case 171: /* reg: BCOMI4(reg) */
	case 170: /* reg: BCOMI2(reg) */
	case 82: /* reg: INDIRU4(addr) */
	case 81: /* reg: INDIRI4(addr) */
	case 80: /* reg: INDIRP2(addr) */
	case 79: /* reg: INDIRU2(addr) */
	case 78: /* reg: INDIRI2(addr) */
	case 77: /* reg: INDIRU1(addr) */
	case 76: /* reg: INDIRI1(addr) */
	case 75: /* reg: INDIRU1(indaddr) */
	case 74: /* reg: INDIRI1(indaddr) */
		kids[0] = LEFT_CHILD(p);
		break;
	case 301: /* stmt: ARGB(INDIRB(reg)) */
	case 91: /* reg: CVUI4(INDIRU2(addr)) */
	case 90: /* reg: CVUI4(INDIRU1(addr)) */
	case 89: /* reg: CVUU4(INDIRU2(addr)) */
	case 88: /* reg: CVUU4(INDIRU1(addr)) */
	case 87: /* reg: CVII4(INDIRI2(addr)) */
	case 86: /* reg: CVII4(INDIRI1(addr)) */
	case 85: /* reg: CVUI2(INDIRU1(addr)) */
	case 84: /* reg: CVUU2(INDIRU1(addr)) */
	case 83: /* reg: CVII2(INDIRI1(addr)) */
		kids[0] = LEFT_CHILD(LEFT_CHILD(p));
		break;
	case 221: /* stmt: NEI2(CVUI2(reg),CVUI2(reg)) */
	case 218: /* stmt: LTI2(CVUI2(reg),CVUI2(reg)) */
	case 215: /* stmt: LEI2(CVUI2(reg),CVUI2(reg)) */
	case 132: /* reg: ADDI2(CVUI2(reg),CVUI2(reg)) */
		kids[0] = LEFT_CHILD(LEFT_CHILD(p));
		kids[1] = LEFT_CHILD(RIGHT_CHILD(p));
		break;
	case 179: /* reg: LOADU2(SUBU2(reg,consm)) */
		kids[0] = LEFT_CHILD(LEFT_CHILD(p));
		kids[1] = RIGHT_CHILD(LEFT_CHILD(p));
		break;
	case 198: /* reg: CVIU4(CVUI2(INDIRU1(addr))) */
		kids[0] = LEFT_CHILD(LEFT_CHILD(LEFT_CHILD(p)));
		break;
	case 271: /* stmt: NEI2(CVUI2(reg),con8bit) */
	case 265: /* stmt: EQU2(CVUI2(reg),con0) */
	case 262: /* stmt: EQI2(CVUI2(reg),con0) */
	case 260: /* stmt: NEI2(CVUI2(reg),con0) */
	case 257: /* stmt: LTU2(CVUI2(reg),con) */
	case 254: /* stmt: LTI2(CVUI2(reg),con) */
	case 241: /* stmt: GEI2(CVUI2(reg),con) */
	case 236: /* stmt: EQI2(CVUI2(reg),con8bit) */
		kids[0] = LEFT_CHILD(LEFT_CHILD(p));
		kids[1] = RIGHT_CHILD(p);
		break;
	default:
		fatal("_kids", "Bad rule number %d\n", eruleno);
	}
}

static void progend(void){
	print(	"\tinclude lcc1802epiloCX.inc\n");	//standard epilog
	if (wjrpixie){
		print(	"\tinclude lcc1802PXcode.inc\n");	//needed for 1861
	}
	if (wjrfloats){
		print("\tinclude LCC1802fp.inc\n"); //floating point code
	}
	print("\tinclude IO1802.inc\n");
	print("\tinclude LCC1802finale.inc\n");

}
static void progbeg(int argc, char *argv[]) {
        int i;
        time_t now;
        struct tm* ptmNow;
        static char rev[] = "$Version: 5.2 - XR18CX $";
 
        {
                union {
                        char c;
                        int i;
                } u;
                u.i = 0;
                u.c = 1;
                swap = ((int)(u.i == 1)) != IR->little_endian;
        }
        parseflags(argc, argv);
        for (i = 0; i < argc; i++){
        	//fprintf(stderr,"arg %d is %s\n",i,argv[i]);
                if (strstr(argv[i], "-pixie") != 0){ //accept combined args
                	fprintf(stderr,"long branch emulation\n");
                        wjrpixie = 1;
                }
                if (strstr(argv[i], "-pixie2") != 0){ //accept combined args
                	fprintf(stderr,"dedicated long branch register\n");
                        wjrpixie = 2;
                }
                if (strstr(argv[i], "-pixie3") != 0){ //accept combined args
                	fprintf(stderr,"dedicated long branch register\n");
                        wjrpixie = 3;
                }
                if (strstr(argv[i], "-mulcall") != 0){ //forces subroutine calls for multiply
                	fprintf(stderr,"subroutine calls for multiply\n");
                        wjrMulInlineWeight=15; //disallows inline multiply
                }
                if (strstr(argv[i], "-volatile") != 0){ //accept combined args
                	//fprintf(stderr,"going volatile\n");
                        wjrvolatile = 1;
                }
                if (strstr(argv[i], "-cpu1805") != 0){ //accept combined args
                	fprintf(stderr,"allowing regs 4&5 for variables\n");
                        wjrcpu1805 = 1;
                }
                if (strstr(argv[i], "-env=") != 0){ //accept combined args
                        wjrenv = strstr(argv[i], "-env=")+5;//point to the environment variable
                	fprintf(stderr,"environment specified %s\n",wjrenv);
                }
	}
	fprintf(stderr,"So Much Snow!\n");  //just so I know who's playing
        time(&now);
        ptmNow = localtime(&now);
        printf("; generated by lcc-xr18CX/PX-%d %s on %s\n", wjrpixie, rev, asctime(ptmNow));
        printf("SP:\tequ	%d ;stack pointer\n" "memAddr: equ	%d\n" "retAddr: equ	%d\n" //pass on reg definitions to assembler
        	"retVal:\tequ\t%d\n" "regArg1: equ	%d\n" "regArg2: equ	%d\n",
        	reg_sp_actual,REG_MEMADDR,REG_RETADDR,REG_RETVAL,REG_FIRST_ARG,REG_FIRST_ARG+1);
	if (wjrcpu1805){ //compiling for 1804/1805/1806
		print("\tcpu\t1805\n");
	}
	if (wjrenv){ //environment includes specified
		print("\tinclude %sprolog.inc\n",wjrenv);
	}
	if (wjrpixie==1){
		print("\tlisting off\n" "\tinclude lcc1802proloPX.inc\n" "\tlisting on\n");//include pixilated macro package but turn off the listing for now
	}else if (wjrpixie==2){
		print("\tlisting off\n" "\tinclude lcc1802proloPX2.inc\n" "\tlisting on\n");//include pixilated macros with branch assist
	}else if (wjrpixie==3){
		print("\tlisting off\n" "\tinclude lcc1802proloPX2.inc\n" "\tlisting on\n");//include pixilated macros with branch assist
	}else{
		print("\tlisting off\n" "\tinclude lcc1802proloCX.inc\n" "\tlisting on\n");//include standard macro package but turn off the listing for now
	}
		

        for (i = 0; i < NUM_IREGS; i++)
                ireg[i] = mkreg("%d", i, 1, IREG);  //makes a symbol for each of the integer regs
        ireg[reg_sp_actual]->x.name = "sp";		//renames the stack pointer to show as "sp"
        for (i = 0; i < NUM_IREGS; i += 2)
                lreg[i] = mkreg("L%d", i, 3, IREG); //makes one long-reg symbol for each ireg pair (0-1, 2-3 etc) intvar and inttmp still control the usage
        for (i = 0; i < NUM_IREGS; i += 2)
                freg2[i] = mkreg("L%d", i, 3, IREG); //(was FREG)makes one float-reg symbol for each ireg pair (0-1, 2-3 etc) intvar and inttmp still control the usage
        
        //for (i = 0; i < 31; i ++)
        //        freg2[i] = mkreg("F%d", i, 1, FREG); //makes symbols for 32 float regs 

        rp1p2 = mkreg("p1p2", REG_FIRST_ARG, 3, IREG);	//makes a symbol for the parameter register pair when used to hold a long.

        freg2w = mkwildcard(freg2);	//wildcards represent the set of registers for floats, ints, and longs
        iregw = mkwildcard(ireg);
        lregw = mkwildcard(lreg);
        tmask[IREG] = INTTMP; tmask[FREG] = INTTMP;	//tmask & vmask show what regs can be used for vars and temps
        vmask[IREG] = vmask[FREG] = INTVAR; //1802 register variables
        if (wjrvolatile){
        	//fprintf(stderr,"reduced register variables\n");
        	vmask[IREG] = vmask[FREG] =INTVAR & ~REGSVOLATILE; //eliminate 0&1
        }
        if (2==wjrpixie){
        	fprintf(stderr,"pixie branch assist\n");
        	vmask[IREG] = vmask[FREG] =INTVAR & ~REGSPIXIE2; //eliminate 7,0,1!
        }
        if (3==wjrpixie){
        	fprintf(stderr,"pixie branch assist, shuffled temps->vars\n");
        	vmask[IREG] = vmask[FREG] = PX3VARS;	//regs 9,8,6 for variables
        	tmask[IREG] = tmask[FREG] = PX3TMP;	//regs 11&10 only for temps
        }
        if (wjrcpu1805){
        	fprintf(stderr,"1805 register variables\n");
        	vmask[IREG] = vmask[FREG] | REGS1805; //allow 4&5
        }
        // fprintf(stderr,"Register mask for variables %X\n",vmask[IREG]);
        blkreg = mkreg(SZ_REG_FIRST_TEMP, REG_FIRST_TEMP, 7, IREG);	//which regs to use for block copies and moves
}
static Symbol rmap(int opk) {
        switch (optype(opk)) {
        case I: case U:
                return (opsize(opk) == 4) ? lregw : iregw;
        case P: case B:
                return iregw;
        case F:
                fp();
                return freg2w;
        default:
                return 0;
        }
}
static void target(Node p) {
	int sz = opsize(p->op);
        assert(p);
        switch (specific(p->op)) {
        case CALL+V:
                break;
        case CALL+F:
                setreg(p, freg2[REG_FIRST_ARG]);
                break;
        case CALL+I: case CALL+P: case CALL+U:
        	if (sz<4){
                	setreg(p, ireg[REG_RETVAL]);
                	break;
                } else{
			setreg(p,lreg[REG_FIRST_ARG]);
			break;          	

		}
        case RET+F:
                	//targeting is done in emit2
                break;
        case RET+I: case RET+U: case RET+P:
        	if (sz<4){
                	rtarget(p, 0, ireg[REG_RETVAL]);
                	break;
                } else{
                	//targeting is done in emit2
			break;          	

		}
                break;
        case ARG+F: case ARG+I: case ARG+P: case ARG+U: {
                static int ty0;
                int ty = optype(p->op);
                Symbol q;

                q = argreg(p->x.argno, p->syms[2]->u.c.v.i, ty, opsize(p->op), ty0);
                if (p->x.argno == 0)
                        ty0 = ty;
                if (q /*&& 					//wjr
                !(ty == F && q->x.regnode->set == IREG)*/)
                        rtarget(p, 0, q);
                break;
                }
         case CVI+F:	//wjr targetting conversion to float
        	//fprintf(stderr,"target selection for CV+I\n");
		setreg(p,lreg[REG_FIRST_TEMP]);
		//if (opsize((p->kids[0])->op)>2){
        	//	fprintf(stderr,"target is long\n");
			rtarget(p, 0, lreg[REG_FIRST_TEMP]);
		//} else{
        	//	fprintf(stderr,"target is short\n");
		//	rtarget(p, 0, lreg[REG_LAST_TEMP-1]);
		//}
		break;
        case CVF+I:	//wjr targetting conversion from float
        	//fprintf(stderr,"target selection for CV+F\n");
		setreg(p,lreg[REG_FIRST_TEMP]);
		rtarget(p, 0, lreg[REG_FIRST_TEMP]);
		break;
        case ASGN+B: rtarget(p->kids[1], 0, blkreg); break;
        case ARG+B:  rtarget(p->kids[0], 0, blkreg); break;
        case MUL+I: case MUL+U:
        case DIV+I: case MOD+I: 
        case DIV+U: case MOD+U:
        case DIV+F: case MUL+F: case ADD+F: case SUB+F:
        /* REVIEW: LSH, RSH? */
        	sz = opsize(p->op);
        	if (sz<4){
			assert(REG_FIRST_ARG+1 <= REG_LAST_ARG);
			setreg(p, ireg[REG_RETVAL]);
			rtarget(p, 0, ireg[REG_FIRST_ARG]);
			rtarget(p, 1, ireg[REG_FIRST_ARG+1]);
			break;
		}else{
			setreg(p,lreg[REG_FIRST_TEMP]);
			rtarget(p, 0, lreg[REG_FIRST_TEMP]);
			rtarget(p, 1, lreg[REG_LAST_TEMP-1]);
			break;
		}

        }
}
static void clobber(Node p) {
        assert(p);
        switch (specific(p->op)) {
        case CALL+F:
                spill(INTTMP | INTRET, IREG, p);
                //spill(FLTTMP,          FREG, p);
                break;
        case CALL+I: case CALL+P: case CALL+U:
                spill(INTTMP,          IREG, p);
                //spill(FLTTMP | FLTRET, FREG, p);
                break;
        case CALL+V:
                spill(INTTMP | INTRET, IREG, p);
                //spill(FLTTMP | FLTRET, FREG, p);
                break;
        }
}
extern void dumptree(Node);

static void emitmulcon(int c){ //going to multiply the value in R13 by the constant, result in R15
	int bits=c;
	print("	;inline multiplication by a constant %d\n",c);
	if (c&1){ //bottom bit set
		print("	cpy2 R15,R13\n");
	} else {
		print("	ld2z R15\n");
	}
	bits=bits>>1;
	while(bits!=0){
		print("	shl2I	R13,1\n");
		if (bits&1){
			print("	alu2 R15,R15,R13,add,adc\n");
		}
		bits=bits>>1;
	}
}
static void emit2(Node p) {
        int dst, n, src, sz, ty;
        int szkids0;
        static int ty0;
        Symbol q;
    	int op = specific(p->op); 
        switch (specific(p->op)) {
        //default: dumptree(p); fputc('\n',stderr); break; /* debugging only*/
        case MUL+I: case MUL+U: //handling multiplication by a small constant
        	assert(opsize(p->op)==2);
        	assert(generic(p->kids[0]->op)==CNST);
         	emitmulcon(p->kids[0]->syms[0]->u.c.v.i);
         	break;
        case RET+I: case RET+U: case RET+F: //trying to handle long returns
                ty = optype(p->op);
                sz = opsize(p->op);
                if (sz==4){ //long return value goes in rp1p2
              		print("\tcpy4 rp1p2,R%s\n",p->kids[0]->syms[2]->x.name);
                }
                break;
        case ARG+F: case ARG+I: case ARG+P: case ARG+U:
                ty = optype(p->op);
                sz = opsize(p->op);
                if (p->x.argno == 0)
                        ty0 = ty;
                q = argreg(p->x.argno, p->syms[2]->u.c.v.i, ty, sz, ty0);
                src = getregnum(p->x.kids[0]);
                if (q == NULL){
                        if (2==sz)
                        	print("\tst%d R%d,'O',sp,(%d+1); arg+f**\n",sz,src, p->syms[2]->u.c.v.i, "wl"[sz==4], src, p->syms[2]->u.c.v.i); //17-02-05 1802
                        else 
                        	print("\tst%d RL%d,'O',sp,(%d+1); arg+f**\n",sz,src, p->syms[2]->u.c.v.i);      //17-02-05 1806            
                }
                break;
        case ASGN+B:
		fprintf(stderr,"ASGN+B\n");
                dalign = salign = p->syms[1]->u.c.v.i; //not an issue for the 180x
                blkcopy(getregnum(p->x.kids[0]), 0,
                        getregnum(p->x.kids[1]), 0,
                        p->syms[0]->u.c.v.i, tmpregs);
                break;
        case ARG+B:
		fprintf(stderr,"ARG+B - should not occur\n");
                assert(0);

                break;
        case IASM+V:	//wjr jan 31 -3 lines
                asminline(p);
                break;
         case CVI+F:	//wjr april 10
		//fprintf(stderr,"CVI+F\n");
		//dumptree(p->kids[0]);
                ty = optype(p->op);
                sz = opsize(p->op);
                szkids0 = opsize((p->kids[0])->op);
                if (szkids0==2){
                	print("\tsext4 R%s; emit2:extend int to long for float conversion\n",p->syms['c' - 'a']->x.name);
                }
                print("\tCcall cvif4; emit2\n");
                //fprintf(stderr,"\t ty=%d,sz=%d,szkids0=%d\n ",ty,sz,szkids0);
                //print("\tst%d RL%d,'O',sp,(%d+1);\n\n",sz,src, p->syms[2]->u.c.v.i);  //17-02-05 1802                
		//fputs(p->syms['c' - 'a']->x.name, stderr);
                break;
       }
}
static Symbol argreg(int argno, int offset, int ty, int sz, int ty0) {
        assert((offset&1) == 0);
        //print(";argreg(argno=%d,offset=%d,ty=%d,sz=%d ",argno,offset,ty,sz);
        if (offset + sz > 2*NUM_ARG_REGS || !(ty == I || ty == U || ty == P||ty==F)) //wjr
                {/*print(" ret null\n");*/ return NULL;}
        else if (argno == 0 && sz == 4)
                {/*print(" ret p1p2\n");*/ return rp1p2;}
        else
                {/*print(" ret %d\n",ireg[(offset/2) + REG_FIRST_ARG]); */ return ireg[(offset/2) + REG_FIRST_ARG];}
}
static void doarg(Node p) {
        static int argno;
        int align;

        if (argoffset == 0)
                argno = 0;
        p->x.argno = argno++;
        align = p->syms[1]->u.c.v.i < 2 ? 2 : p->syms[1]->u.c.v.i;
        p->syms[2] = intconst(mkactual(align,
                p->syms[0]->u.c.v.i));
}
static void local(Symbol p) {
        if (askregvar(p, rmap(ttob(p->type))) == 0)
                mkauto(p);
}
static void gensaveregs(){  //routine to save registers extracted from function()
	int i;
	for (i = 0; i < NUM_IREGS; i++)	//now we save the int registers
                if (usedmask[IREG]&(1<<i)) {
                        print("\tpushr R%d\n", i);  //push the register
                }
        for (i = 20; i <= 30; i += 2)	//now we save the float registers
                if (usedmask[FREG]&(3<<i)) {
                        print("\tpushfloat RF%d\n", i);
                }
}
static void genrldregs(){ //reload any saved registers 
	int i;
        for (i = NUM_IREGS; i >=0 ; i--)	//now we reload the int registers
                if (usedmask[IREG]&(1<<i)) {
                        print("\tpopr R%d\n", i);  
                }
        for (i = 30; i >= 20; i -= 2)	//now we reload the float registers
                if (usedmask[FREG]&(3<<i)) {
                        print("\tpopfloat RF%d\n", i);
                }
}

static void function(Symbol f, Symbol caller[], Symbol callee[], int ncalls) {
        int i, saved, sizefsave, sizeisave, varargs;
        Symbol r, argregs[NUM_ARG_REGS];

        usedmask[0] = usedmask[1] = 0;
        freemask[0] = freemask[1] = ~(unsigned)0;
        offset = maxoffset = maxargoffset = 0;
        for (i = 0; callee[i]; i++) //find the last parameter
                ;
        varargs = variadic(f->type) //set the flag for variable arguments if the function is typed variadic 
                || i > 0 && strcmp(callee[i-1]->name, "va_alist") == 0; //or if the last argument is "va_alist"

	//this loop scans the arguments to assign offsets and	//
	//to see if they can stay in their registers or 	//
	//move to new registers or go on the stack		//
	for (i = 0; callee[i]; i++) {
                Symbol p = callee[i];
                Symbol q = caller[i];
                assert(q);
                offset = roundup(offset, q->type->align);	//even arguments that come in registers get an offset
                p->x.offset = q->x.offset = offset;
                p->x.name = q->x.name = stringd(offset);
                r = argreg(i, offset, optype(ttob(q->type)), q->type->size, optype(ttob(caller[0]->type)));
                if (i < NUM_ARG_REGS)
                        argregs[i] = r;
                offset = roundup(offset + q->type->size, 2);
                if (varargs)	//for variadic functions the arguments are auto
                        p->sclass = AUTO;
                else if (r && ncalls == 0 &&
                         !isstruct(q->type) && !p->addressed &&
                         !(isfloat(q->type) && r->x.regnode->set == IREG)) {
                        p->sclass = q->sclass = REGISTER;
                        askregvar(p, r);
                        assert(p->x.regnode && p->x.regnode->vbl == p);
                        q->x = p->x;
                        q->type = p->type;
                }
                else if (askregvar(p, rmap(ttob(p->type))) //this last case assigns a new register
                         && r != NULL
                         && (isint(p->type) || p->type == q->type)) {
                        assert(q->sclass != REGISTER);
                        p->sclass = q->sclass = REGISTER;
                        q->type = p->type;
                }
        }
        assert(!caller[i]);
        offset = 2; //wjr jan 8 allow for spot taken by saved return address
        gencode(caller, callee);  //while generating the dag tree, gencode will set offsets for locals,
        			//count calls in ncalls and mark what registers are used in usedmask
        usedmask[IREG] &= INT_CALLEE_SAVE;	//limit regs to be saved to those the callee is responsible for (basically the variables)
        usedmask[FREG] &= 0x00000000;		//not saving the float temps
        maxargoffset = roundup(maxargoffset, usedmask[FREG] ? 8 : 2);	//round up the arg area if floats are saved
        if (ncalls && maxargoffset < NUM_ARG_REGS*2)	//if we do calls, always leave at least enough room to save all the argument registers
                maxargoffset = NUM_ARG_REGS*2;
        //we now know we need maxargoffset bytes for outgoing arguments
        sizefsave = 4*bitcount(usedmask[FREG]);
        sizeisave = 2*bitcount(usedmask[IREG]);
        framesize = maxargoffset 	//the frame includes the outgoing argument area, 
                + sizefsave + sizeisave 	//the float and int reg save areas,
                + roundup(maxoffset, 2);       		// and the area for locals
 	print(";;function_start %s %t\n",f->x.name, f->type);
        printf("%s:\t\t;framesize=%d\n", f->x.name,framesize); //wjr june 27 2013
        if (framesize > 2) { 
        		if (0!=(usedmask[IREG]+usedmask[FREG])){  //if there are regs to save
        			if (roundup(maxoffset,2)>2){
        				print("\treserve %d; save room for local variables\n", roundup(maxoffset,2)-2);
        			}
				gensaveregs(); //save the registers
				if (maxargoffset>0) {
					print("\treserve %d; save room for outgoing arguments\n", maxargoffset);
				}
			} else {
				print("\treserve %d\n", framesize-2); //just reserve the stack frame 
			}
        }



        for (i = 0; i < NUM_ARG_REGS && callee[i]; i++) {  //this loop is all about assigning locations for the incoming parameter registers
                r = argregs[i];
                if (r && r->x.regnode != callee[i]->x.regnode) {
                        Symbol out = callee[i];
                        Symbol in  = caller[i];
                        int rn = r->x.regnode->number;
                        int rs = r->x.regnode->set;
                        int tyin = ttob(in->type);

                        assert(out && in && r && r->x.regnode);
                        assert(out->sclass != REGISTER || out->x.regnode);
                        if (out->sclass == REGISTER
                        && (isint(out->type) || out->type == in->type)) {	//this is about moving an arg register to another register
                                int outn = out->x.regnode->number;
                                /*Oct 12 eliminate refs to float regs - I don't use them
                                if (rs == FREG && tyin == F+sizeop(8))
                                        print("mov.d rf%d,rf%d\n", outn, rn);
                                else if (rs == FREG && tyin == F+sizeop(4))
                                        print("mov.s rf%d,rf%d\n", outn, rn);
                                else if (rs == IREG && tyin == F+sizeop(8))
                                        print("mtc1.d R%d,rf%d\n", rn,   outn);
                                else 
                                Oct 12 */
                                if (rs == IREG && tyin == F+sizeop(4))
                                        print("\tcpy4 RL%d,RL%d; halfbaked&floaty\n",   outn,rn);//print("mtc1 R%d,rf%d\n",   rn,   outn);
                                else if (rs == IREG && tyin == I+sizeop(4))
                                        print("\tcpy4 RL%d,RL%d; halfbaked\n",   outn,rn);
                                else if (rs == IREG && tyin == U+sizeop(4))
                                        print("\tcpy4 RL%d,RL%d; halfbaked\n",   outn,rn);
                                else
                                        print("\tcpy2 R%d,R%d; function(%d) 1\n",    outn, rn,tyin);
                        } else {
                                int off = in->x.offset + framesize;
                                /*oct 12 eliminate refs to FREG
                                if (rs == FREG && tyin == F+sizeop(8))
                                        print("pigs s.d rf%d,%d(sp)\n", rn, off); //cannot get here
                                else if (rs == FREG && tyin == F+sizeop(4))
                                        print("frogs s.s rf%d,%d(sp)\n", rn, off); //cannot get here
                                else 
                                Oct 12*/
                                {	//this is about saving arg registers in the caller's frame
                                        int i, n = (in->type->size + 1)/2;
                                        //print("hey hey rn=%d,n=%d,REG_LAST_ARG=%d\n",rn,n,REG_LAST_ARG);
                                        for (i = rn; i < rn+n && i <= REG_LAST_ARG; i++)
                                                print("\tst2 R%d,'O',sp,(%d+1); flag1 \n", i, off + (i-rn)*2);//17-02-05 1802
                                }
                        }
                }
        }
        if (varargs && callee[i-1]) { //this saves any remaining argument registers in the frame
                i = callee[i-1]->x.offset + callee[i-1]->type->size;
                for (i = roundup(i, 2)/2; i < NUM_ARG_REGS; i++)
                        print("\tst2 R%d,'O',sp,(%d+1); flag2\n", REG_FIRST_ARG + i, framesize + 2*i);//**see bottom** //17-02-05 1802
                }
        emitcode();
        if (framesize > 2) { 
        	if (0!=(usedmask[IREG]+usedmask[FREG])){  //if there are regs to restore
        		if (maxargoffset>0){
				print("\trelease %d; release room for outgoing arguments\n", maxargoffset);
			}
			genrldregs(); //reload the registers
			if (roundup(maxoffset,2)>2){
				print("\trelease %d; release room for local variables \n", roundup(maxoffset,2)-2);
			}
		} else {
			print("\trelease %d\n", framesize-2); //just release the stack frame 
		}
        }
/*
        if (0!=(usedmask[IREG]+usedmask[FREG])) //if there are regs to reload
        genrldregs(maxargoffset); //tell routine where to start reloading

        if (framesize > 2) { //wjr jan 8 virtual framesize allows for ret addr saved in call
                print("\trelease %d\n", framesize-2); //wjr jan 14 - release the stack frame 
        }
 */
        //print("\tghi 15 ;otherwise SCRT return may mess up R15.1\n"); //20-05-23 moved to cretn
        print("\tCretn\n\n");
 	print(";;function_end$$ %s\n",f->x.name);
}
static void defconst(int suffix, int size, Value v) {
        if (suffix == F && size == 4) {
                float f = v.d;
                print("\tdd 0x%x\n", *(unsigned *)&f);
        }        else if (suffix == P)
                print("\tdw %u\n", v.p);
        else if (size == 1)
                print(suffix==I ? "\tdb %d\n" : "\tdb %u\n", suffix == I ? v.i : v.u);
        else if (size == 2)
                print(suffix==I ? "\tdw %d\n" : "\tdw %u\n", suffix == I ? v.i : v.u);
        else if (size == 4)
                print(suffix==I ? "\tdd %d\n" : "\tdd %u\n", suffix == I ? v.i : v.u);
}
static void defaddress(Symbol p) {
        print("\tdw %s\n", p->x.name);
}
static void defstring(int n, char *str) {
        char *s;

        for (s = str; s < str + n; s++)
                print("\tdb %d\n", (*s)&0377);
}
static void export(Symbol p) {
        //print("\t;global %s\n", p->x.name);
}
static void import(Symbol p) {
        //if (!isfunc(p->type))
                //print("\t;global %s\n", p->x.name); /* good enough? */
}
static void defsymbol(Symbol p) {
        if (p->scope >= LOCAL && p->sclass == STATIC)
                p->x.name = stringf("L%d", genlabel(1));
        else if (p->generated)
                p->x.name = stringf("L%s", p->name);
        else if (p->scope == GLOBAL || p->sclass == EXTERN)
                p->x.name = stringf("_%s", p->name);
        else
                assert(p->scope != CONSTANTS || isint(p->type) || isptr(p->type)),
                p->x.name = p->name;
}
static void address(Symbol q, Symbol p, long n) {
        if (p->scope == GLOBAL
        || p->sclass == STATIC || p->sclass == EXTERN)
                q->x.name = stringf("%s%s%D", p->x.name,
                        n >= 0 ? "+" : "", n);
        else {
                assert(n <= INT_MAX && n >= INT_MIN);
                q->x.offset = p->x.offset + n;
                q->x.name = stringd(q->x.offset);
        }
}
static void global(Symbol p) {
	int dbsize;
        if (p->type->align > 1)
                print("\talign %d\n", p->type->align);
        print("%s:\n", p->x.name);
        if (p->u.seg == BSS){
        	if ((p->type->size)<1000) { //do small declares as one piece
                	printf("\tglobss %d; define global BSS\n", p->type->size);
                } 
                else{
                	for(dbsize=p->type->size;dbsize>1000;dbsize-=1000){//do it in 1000 byte chunks
                		printf("\tglobss %d; define global BSS\n", 1000);
                	}
                	if (dbsize>0) {	//emit remainder
                		printf("\tglobss %d; define global BSS\n", dbsize);
                	}
                }
	}
                	
}
static void segment(int n) {
	char * segnames[]={"N/A ","CODE","BSS ","DATA","CODE"}; //code, bss, initialized data, literals
	if (1==n || 4==n){ //compiler generated code or literals
	    print("\torgc\n"); //   reset the location counter to just after the jump
	} else{
	    if (2==n || 3==n){ //if this is compiler generated global data
	        print ("\torgd\n");
	    }
	}
        cseg = n;
}
static void space(int n) {
        if (cseg != BSS)
                print("\tdb %d dup(0) ;zerofill\n", n); //was print("\tds %d\n", n);
}
static void blkloop(int dreg, int doff, int sreg, int soff, int size, int tmps[]) { //probably not used
        int lab = genlabel(1);

        print("addi R%d,R%d,%d\n", sreg, sreg, size&~7);
        print("addi R%d,R%d,%d\n", tmps[2], dreg, size&~7);
        blkcopy(tmps[2], doff, sreg, soff, size&7, tmps);
        print("L%d:\n", lab);
        print("addi r%d,r%d,%d\n", sreg, sreg, -8);
        print("addi r%d,r%d,%d\n", tmps[2], tmps[2], -8);
        blkcopy(tmps[2], doff, sreg, soff, 8, tmps);
        print("cmp r%d,r%d\nbltu L%d\n", dreg, tmps[2], lab);
}
static void blkfetch(int size, int off, int reg, int tmp) {//probably not used
	fprintf(stderr,"blkfetch(size=%d,off=%d,reg=%d,tmp=%d,salign=%d\n",size,off,reg,tmp,salign);
        assert(size == 1 || size == 2);
        if (size == 1)
                print("lb r%d,%d(r%d)\n",  tmp, off, reg);
        else {
                //assert(salign >= size); //wjr why?
                print("lw r%d,%d(r%d)\n",   tmp, off, reg);
        }
}
static void blkstore(int size, int off, int reg, int tmp) {
        assert(size == 1 || size == 2);
        if (size == 1)
                print("sb r%d,%d(r%d);blkstore\\n",  tmp, off, reg);
        else {
                //assert(dalign >= size);//wjr why?
                print("blkstore sw r%d,%d(r%d);blkstore\n",  tmp, off, reg);
        }
}
static void stabinit(char *, int, char *[]);
static void stabline(Coordinate *);
static void stabsym(Symbol);

static char *currentfile;

static int bitcount(unsigned mask) {
        unsigned i, n = 0;

        for (i = 1; i; i <<= 1)
                if (mask&i)
                        n++;
        return n;
}

/* stabinit - initialize stab output */
static void stabinit(char *file, int argc, char *argv[]) {
/*        if (file) {
                print("file \"%s\"\n", file);
                currentfile = file;
        }
*/
}

/* stabline - emit stab entry for source coordinate *cp */
static void stabline(Coordinate *cp) {
/*        if (cp->file && cp->file != currentfile) {
                print("file \"%s\"\n", cp->file);
                currentfile = cp->file;
        }
        print("loc %d\n", cp->y);
*/
}

/* stabsym - output a stab entry for symbol p */
static void stabsym(Symbol p) {
/*        if (p == cfunc && IR->stabline)
                (*IR->stabline)(&p->src);
*/
}

static int fp() {
    if (!wjrfloats) {
    	//fprintf(stderr,"Floating point used\n");
    	wjrfloats=1;
    }
    return 10;
}

static int samesame(){
	fprintf(stderr,"same called\n");
	return 1;
}

Interface xr18CXIR = {	//each entry represents size, alignment, out-of-line literals needed
        1, 1, 0,  /* char */
        2, 1, 0,  /* short */
        2, 1, 0,  /* int */
        4, 4, 0,  /* long jan 20*/
        4, 4, 0,  /* long long  jan 20*/
        4, 4, 1,  /* float  jan 20*/
        4, 4, 1,  /* double  jan 20*/
        4, 4, 1,  /* long double  jan 20*/
        2, 1, 0,  /* pointer */
        0, 1, 0,  /* struct */
        0,  /* little_endian */
        1,  /* mulops_calls */
        0,  /* wants_callb */
        0,  /* wants_argb */
        1,  /* left_to_right */
        0,  /* wants_dag */
        1,  /* unsigned_char */
        address,
        blockbeg,
        blockend,
        defaddress,
        defconst,
        defstring,
        defsymbol,
        emit,
        export,
        function,
        gen,
        global,
        import,
        local,
        progbeg,
        progend,
        segment,
        space,
        0, 0, 0, stabinit, stabline, stabsym, 0,
        {
                2,      /* max_unaligned_load */
                rmap,
                blkfetch, blkstore, blkloop,
                _label,
                _rule,
                _nts,
                _kids,
                _string,
                _templates,
                _isinstruction,
                _ntname,
                emit2,
                doarg,
                target,
                clobber,

        }
};
static char rcsid[] =
	"XR18 Next Generation Optimized COMX Compliant - LCC backend for 1802/4/5/6 target";
//there is a peculiar problem with respect to longs and variadic functions
//if printf(char*,...) is called with printf("%l",(long)1) (or any variadic called with a 2 16 bit and a 32 bit argument,
//the 16 bit thing gets stored in a register and the 32 bit thing on the stack frame.
//However, the line flagged **see bottom** stores the 2nd argument register over top of the beginning of the 32 bit one.
//I've avoided that by making the alignment of longs=4 which will leave a 2 byte gap but that's pretty ugly
