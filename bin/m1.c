
// Register declration enumeration here
enum {R_L0=0, R_L1=1, R_L2=2, R_L3=3, R_B=4, R_A=5, R_C=6};
enum {R_F0=0, R_F1=1, R_F2=2, R_F3=3, R_F4=4, R_F5=5, R_F6=6, R_F7=7,
      R_F8=8, R_F9=9, R_F10=10, R_F11=11, R_F12=12, R_F13=13, R_F14=14, R_F15=15};

#include "c.h"

#define NODEPTR_TYPE Node
#define OP_LABEL(p) ((p)->op)
#define LEFT_CHILD(p) ((p)->kids[0])
#define RIGHT_CHILD(p) ((p)->kids[1])
#define STATE_LABEL(p) ((p)->x.state)

// Declare local routines to be used by IR record here
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
static int memop(Node);
static int flip_memop(Node);
static int isfloat4(Node);
static int isfloat8(Node);
static int sametree(Node,Node);
static int isfptr(Node n, int iftrue, int iffalse);

// Local vars here

static Symbol longreg[32];
static Symbol intreg[32];
static Symbol fltreg[32];

static int used_longs;
static int used_floats;
static int ever_used_floats;

static Symbol longwldcrd;
static Symbol intwldcrd;
static Symbol cwldcrd;
static Symbol fltwldcrd;

static int current_seg;

static double_ptr = 0;
/*
generated at Sun Jan 24 11:24:47 2021
by $Id: lburg.c,v 2.10 2002/03/08 18:45:21 drh Exp $
*/
static void _kids(NODEPTR_TYPE, int, NODEPTR_TYPE[]);
static void _label(NODEPTR_TYPE);
static int _rule(void*, int);

#define _stmt_NT 1
#define _reg8_NT 2
#define _reg_NT 3
#define _reg32_NT 4
#define _regf64_NT 5
#define _con_NT 6
#define _c0_NT 7
#define _c1_NT 8
#define _c2_NT 9
#define _con1_NT 10
#define _con2_NT 11
#define _con4_NT 12
#define _creg_NT 13
#define _dref16_NT 14
#define _dref8_NT 15
#define _faddr_NT 16
#define _mem_NT 17
#define _vaddr_NT 18
#define _addr_NT 19
#define _ar_NT 20

static char *_ntname[] = {
	0,
	"stmt",
	"reg8",
	"reg",
	"reg32",
	"regf64",
	"con",
	"c0",
	"c1",
	"c2",
	"con1",
	"con2",
	"con4",
	"creg",
	"dref16",
	"dref8",
	"faddr",
	"mem",
	"vaddr",
	"addr",
	"ar",
	0
};

struct _state {
	short cost[21];
	struct {
		unsigned int _stmt:9;
		unsigned int _reg8:6;
		unsigned int _reg:7;
		unsigned int _reg32:6;
		unsigned int _regf64:6;
		unsigned int _con:3;
		unsigned int _c0:2;
		unsigned int _c1:2;
		unsigned int _c2:2;
		unsigned int _con1:2;
		unsigned int _con2:2;
		unsigned int _con4:2;
		unsigned int _creg:1;
		unsigned int _dref16:4;
		unsigned int _dref8:3;
		unsigned int _faddr:3;
		unsigned int _mem:3;
		unsigned int _vaddr:1;
		unsigned int _addr:2;
		unsigned int _ar:2;
	} rule;
};

static short _nts_0[] = { _reg8_NT, 0 };
static short _nts_1[] = { _reg_NT, 0 };
static short _nts_2[] = { _reg32_NT, 0 };
static short _nts_3[] = { _regf64_NT, 0 };
static short _nts_4[] = { 0 };
static short _nts_5[] = { _con_NT, 0 };
static short _nts_6[] = { _con1_NT, 0 };
static short _nts_7[] = { _con2_NT, 0 };
static short _nts_8[] = { _con4_NT, 0 };
static short _nts_9[] = { _c0_NT, 0 };
static short _nts_10[] = { _c1_NT, 0 };
static short _nts_11[] = { _c2_NT, 0 };
static short _nts_12[] = { _reg_NT, _reg_NT, 0 };
static short _nts_13[] = { _reg_NT, _con_NT, 0 };
static short _nts_14[] = { _reg_NT, _dref16_NT, 0 };
static short _nts_15[] = { _reg_NT, _c1_NT, _reg_NT, 0 };
static short _nts_16[] = { _reg8_NT, _reg8_NT, 0 };
static short _nts_17[] = { _reg8_NT, _con_NT, 0 };
static short _nts_18[] = { _reg8_NT, _dref8_NT, 0 };
static short _nts_19[] = { _reg32_NT, _reg32_NT, 0 };
static short _nts_20[] = { _reg32_NT, _faddr_NT, 0 };
static short _nts_21[] = { _regf64_NT, _regf64_NT, 0 };
static short _nts_22[] = { _regf64_NT, _faddr_NT, 0 };
static short _nts_23[] = { _reg8_NT, _dref16_NT, 0 };
static short _nts_24[] = { _reg32_NT, _con_NT, 0 };
static short _nts_25[] = { _faddr_NT, 0 };
static short _nts_26[] = { _reg_NT, _c1_NT, 0 };
static short _nts_27[] = { _reg_NT, _c2_NT, 0 };
static short _nts_28[] = { _reg_NT, _creg_NT, 0 };
static short _nts_29[] = { _reg32_NT, _creg_NT, 0 };
static short _nts_30[] = { _reg32_NT, _reg_NT, 0 };
static short _nts_31[] = { _reg_NT, _con_NT, _c0_NT, 0 };
static short _nts_32[] = { _reg8_NT, _con_NT, _c0_NT, 0 };
static short _nts_33[] = { _mem_NT, _mem_NT, 0 };
static short _nts_34[] = { _reg_NT, _c0_NT, 0 };
static short _nts_35[] = { _con_NT, _reg_NT, 0 };
static short _nts_36[] = { _dref16_NT, _reg_NT, 0 };
static short _nts_37[] = { _con_NT, _reg8_NT, 0 };
static short _nts_38[] = { _dref8_NT, _reg8_NT, 0 };
static short _nts_39[] = { _faddr_NT, _reg32_NT, 0 };
static short _nts_40[] = { _faddr_NT, _regf64_NT, 0 };
static short _nts_41[] = { _addr_NT, 0 };
static short _nts_42[] = { _dref16_NT, 0 };
static short _nts_43[] = { _vaddr_NT, 0 };
static short _nts_44[] = { _ar_NT, 0 };
static short _nts_45[] = { _ar_NT, _reg_NT, 0 };
static short _nts_46[] = { _reg_NT, _reg8_NT, 0 };
static short _nts_47[] = { _addr_NT, _reg8_NT, 0 };
static short _nts_48[] = { _addr_NT, _reg_NT, 0 };
static short _nts_49[] = { _reg_NT, _reg32_NT, 0 };
static short _nts_50[] = { _faddr_NT, _faddr_NT, 0 };
static short _nts_51[] = { _reg_NT, _regf64_NT, 0 };
static short _nts_52[] = { _faddr_NT, _mem_NT, _mem_NT, 0 };

static short *_nts[] = {
	0,	/* 0 */
	_nts_0,	/* 1 */
	_nts_1,	/* 2 */
	_nts_2,	/* 3 */
	_nts_0,	/* 4 */
	_nts_1,	/* 5 */
	_nts_2,	/* 6 */
	_nts_1,	/* 7 */
	_nts_2,	/* 8 */
	_nts_1,	/* 9 */
	_nts_2,	/* 10 */
	_nts_1,	/* 11 */
	_nts_2,	/* 12 */
	_nts_2,	/* 13 */
	_nts_3,	/* 14 */
	_nts_3,	/* 15 */
	_nts_4,	/* 16 */
	_nts_4,	/* 17 */
	_nts_4,	/* 18 */
	_nts_4,	/* 19 */
	_nts_4,	/* 20 */
	_nts_4,	/* 21 */
	_nts_4,	/* 22 */
	_nts_4,	/* 23 */
	_nts_4,	/* 24 */
	_nts_0,	/* 25 */
	_nts_0,	/* 26 */
	_nts_1,	/* 27 */
	_nts_1,	/* 28 */
	_nts_1,	/* 29 */
	_nts_2,	/* 30 */
	_nts_2,	/* 31 */
	_nts_3,	/* 32 */
	_nts_3,	/* 33 */
	_nts_3,	/* 34 */
	_nts_3,	/* 35 */
	_nts_2,	/* 36 */
	_nts_1,	/* 37 */
	_nts_2,	/* 38 */
	_nts_1,	/* 39 */
	_nts_3,	/* 40 */
	_nts_3,	/* 41 */
	_nts_3,	/* 42 */
	_nts_3,	/* 43 */
	_nts_2,	/* 44 */
	_nts_1,	/* 45 */
	_nts_2,	/* 46 */
	_nts_1,	/* 47 */
	_nts_1,	/* 48 */
	_nts_1,	/* 49 */
	_nts_0,	/* 50 */
	_nts_0,	/* 51 */
	_nts_0,	/* 52 */
	_nts_2,	/* 53 */
	_nts_0,	/* 54 */
	_nts_2,	/* 55 */
	_nts_2,	/* 56 */
	_nts_1,	/* 57 */
	_nts_2,	/* 58 */
	_nts_1,	/* 59 */
	_nts_0,	/* 60 */
	_nts_2,	/* 61 */
	_nts_0,	/* 62 */
	_nts_2,	/* 63 */
	_nts_1,	/* 64 */
	_nts_1,	/* 65 */
	_nts_1,	/* 66 */
	_nts_1,	/* 67 */
	_nts_5,	/* 68 */
	_nts_5,	/* 69 */
	_nts_4,	/* 70 */
	_nts_4,	/* 71 */
	_nts_4,	/* 72 */
	_nts_4,	/* 73 */
	_nts_4,	/* 74 */
	_nts_4,	/* 75 */
	_nts_4,	/* 76 */
	_nts_4,	/* 77 */
	_nts_4,	/* 78 */
	_nts_4,	/* 79 */
	_nts_4,	/* 80 */
	_nts_4,	/* 81 */
	_nts_4,	/* 82 */
	_nts_6,	/* 83 */
	_nts_7,	/* 84 */
	_nts_8,	/* 85 */
	_nts_9,	/* 86 */
	_nts_10,	/* 87 */
	_nts_11,	/* 88 */
	_nts_6,	/* 89 */
	_nts_7,	/* 90 */
	_nts_5,	/* 91 */
	_nts_12,	/* 92 */
	_nts_13,	/* 93 */
	_nts_14,	/* 94 */
	_nts_12,	/* 95 */
	_nts_13,	/* 96 */
	_nts_14,	/* 97 */
	_nts_13,	/* 98 */
	_nts_12,	/* 99 */
	_nts_15,	/* 100 */
	_nts_15,	/* 101 */
	_nts_16,	/* 102 */
	_nts_17,	/* 103 */
	_nts_17,	/* 104 */
	_nts_18,	/* 105 */
	_nts_18,	/* 106 */
	_nts_16,	/* 107 */
	_nts_17,	/* 108 */
	_nts_18,	/* 109 */
	_nts_18,	/* 110 */
	_nts_19,	/* 111 */
	_nts_20,	/* 112 */
	_nts_19,	/* 113 */
	_nts_20,	/* 114 */
	_nts_21,	/* 115 */
	_nts_22,	/* 116 */
	_nts_21,	/* 117 */
	_nts_22,	/* 118 */
	_nts_12,	/* 119 */
	_nts_13,	/* 120 */
	_nts_14,	/* 121 */
	_nts_12,	/* 122 */
	_nts_13,	/* 123 */
	_nts_14,	/* 124 */
	_nts_12,	/* 125 */
	_nts_13,	/* 126 */
	_nts_14,	/* 127 */
	_nts_16,	/* 128 */
	_nts_17,	/* 129 */
	_nts_23,	/* 130 */
	_nts_16,	/* 131 */
	_nts_17,	/* 132 */
	_nts_23,	/* 133 */
	_nts_19,	/* 134 */
	_nts_20,	/* 135 */
	_nts_19,	/* 136 */
	_nts_20,	/* 137 */
	_nts_21,	/* 138 */
	_nts_22,	/* 139 */
	_nts_21,	/* 140 */
	_nts_22,	/* 141 */
	_nts_12,	/* 142 */
	_nts_13,	/* 143 */
	_nts_14,	/* 144 */
	_nts_12,	/* 145 */
	_nts_13,	/* 146 */
	_nts_14,	/* 147 */
	_nts_16,	/* 148 */
	_nts_16,	/* 149 */
	_nts_16,	/* 150 */
	_nts_17,	/* 151 */
	_nts_17,	/* 152 */
	_nts_17,	/* 153 */
	_nts_23,	/* 154 */
	_nts_16,	/* 155 */
	_nts_17,	/* 156 */
	_nts_23,	/* 157 */
	_nts_19,	/* 158 */
	_nts_20,	/* 159 */
	_nts_19,	/* 160 */
	_nts_20,	/* 161 */
	_nts_24,	/* 162 */
	_nts_1,	/* 163 */
	_nts_1,	/* 164 */
	_nts_12,	/* 165 */
	_nts_13,	/* 166 */
	_nts_14,	/* 167 */
	_nts_12,	/* 168 */
	_nts_13,	/* 169 */
	_nts_14,	/* 170 */
	_nts_16,	/* 171 */
	_nts_17,	/* 172 */
	_nts_23,	/* 173 */
	_nts_16,	/* 174 */
	_nts_17,	/* 175 */
	_nts_23,	/* 176 */
	_nts_2,	/* 177 */
	_nts_2,	/* 178 */
	_nts_19,	/* 179 */
	_nts_20,	/* 180 */
	_nts_19,	/* 181 */
	_nts_20,	/* 182 */
	_nts_12,	/* 183 */
	_nts_12,	/* 184 */
	_nts_19,	/* 185 */
	_nts_20,	/* 186 */
	_nts_19,	/* 187 */
	_nts_20,	/* 188 */
	_nts_12,	/* 189 */
	_nts_12,	/* 190 */
	_nts_19,	/* 191 */
	_nts_20,	/* 192 */
	_nts_19,	/* 193 */
	_nts_20,	/* 194 */
	_nts_21,	/* 195 */
	_nts_22,	/* 196 */
	_nts_21,	/* 197 */
	_nts_22,	/* 198 */
	_nts_12,	/* 199 */
	_nts_12,	/* 200 */
	_nts_19,	/* 201 */
	_nts_20,	/* 202 */
	_nts_19,	/* 203 */
	_nts_20,	/* 204 */
	_nts_21,	/* 205 */
	_nts_22,	/* 206 */
	_nts_21,	/* 207 */
	_nts_22,	/* 208 */
	_nts_12,	/* 209 */
	_nts_12,	/* 210 */
	_nts_19,	/* 211 */
	_nts_20,	/* 212 */
	_nts_19,	/* 213 */
	_nts_20,	/* 214 */
	_nts_1,	/* 215 */
	_nts_2,	/* 216 */
	_nts_25,	/* 217 */
	_nts_3,	/* 218 */
	_nts_25,	/* 219 */
	_nts_3,	/* 220 */
	_nts_25,	/* 221 */
	_nts_26,	/* 222 */
	_nts_27,	/* 223 */
	_nts_28,	/* 224 */
	_nts_12,	/* 225 */
	_nts_26,	/* 226 */
	_nts_27,	/* 227 */
	_nts_28,	/* 228 */
	_nts_12,	/* 229 */
	_nts_29,	/* 230 */
	_nts_30,	/* 231 */
	_nts_29,	/* 232 */
	_nts_30,	/* 233 */
	_nts_26,	/* 234 */
	_nts_27,	/* 235 */
	_nts_28,	/* 236 */
	_nts_12,	/* 237 */
	_nts_26,	/* 238 */
	_nts_27,	/* 239 */
	_nts_28,	/* 240 */
	_nts_12,	/* 241 */
	_nts_29,	/* 242 */
	_nts_30,	/* 243 */
	_nts_29,	/* 244 */
	_nts_30,	/* 245 */
	_nts_12,	/* 246 */
	_nts_13,	/* 247 */
	_nts_14,	/* 248 */
	_nts_31,	/* 249 */
	_nts_12,	/* 250 */
	_nts_13,	/* 251 */
	_nts_14,	/* 252 */
	_nts_31,	/* 253 */
	_nts_16,	/* 254 */
	_nts_16,	/* 255 */
	_nts_17,	/* 256 */
	_nts_17,	/* 257 */
	_nts_17,	/* 258 */
	_nts_18,	/* 259 */
	_nts_18,	/* 260 */
	_nts_16,	/* 261 */
	_nts_32,	/* 262 */
	_nts_32,	/* 263 */
	_nts_32,	/* 264 */
	_nts_16,	/* 265 */
	_nts_17,	/* 266 */
	_nts_17,	/* 267 */
	_nts_18,	/* 268 */
	_nts_18,	/* 269 */
	_nts_16,	/* 270 */
	_nts_32,	/* 271 */
	_nts_19,	/* 272 */
	_nts_20,	/* 273 */
	_nts_33,	/* 274 */
	_nts_19,	/* 275 */
	_nts_20,	/* 276 */
	_nts_33,	/* 277 */
	_nts_21,	/* 278 */
	_nts_22,	/* 279 */
	_nts_33,	/* 280 */
	_nts_21,	/* 281 */
	_nts_22,	/* 282 */
	_nts_33,	/* 283 */
	_nts_12,	/* 284 */
	_nts_13,	/* 285 */
	_nts_34,	/* 286 */
	_nts_14,	/* 287 */
	_nts_31,	/* 288 */
	_nts_12,	/* 289 */
	_nts_13,	/* 290 */
	_nts_34,	/* 291 */
	_nts_14,	/* 292 */
	_nts_31,	/* 293 */
	_nts_16,	/* 294 */
	_nts_16,	/* 295 */
	_nts_17,	/* 296 */
	_nts_17,	/* 297 */
	_nts_17,	/* 298 */
	_nts_18,	/* 299 */
	_nts_18,	/* 300 */
	_nts_16,	/* 301 */
	_nts_32,	/* 302 */
	_nts_32,	/* 303 */
	_nts_32,	/* 304 */
	_nts_16,	/* 305 */
	_nts_17,	/* 306 */
	_nts_17,	/* 307 */
	_nts_18,	/* 308 */
	_nts_18,	/* 309 */
	_nts_16,	/* 310 */
	_nts_32,	/* 311 */
	_nts_19,	/* 312 */
	_nts_20,	/* 313 */
	_nts_33,	/* 314 */
	_nts_19,	/* 315 */
	_nts_20,	/* 316 */
	_nts_33,	/* 317 */
	_nts_21,	/* 318 */
	_nts_22,	/* 319 */
	_nts_33,	/* 320 */
	_nts_21,	/* 321 */
	_nts_22,	/* 322 */
	_nts_33,	/* 323 */
	_nts_12,	/* 324 */
	_nts_13,	/* 325 */
	_nts_14,	/* 326 */
	_nts_12,	/* 327 */
	_nts_13,	/* 328 */
	_nts_14,	/* 329 */
	_nts_16,	/* 330 */
	_nts_16,	/* 331 */
	_nts_17,	/* 332 */
	_nts_17,	/* 333 */
	_nts_17,	/* 334 */
	_nts_18,	/* 335 */
	_nts_18,	/* 336 */
	_nts_16,	/* 337 */
	_nts_16,	/* 338 */
	_nts_17,	/* 339 */
	_nts_18,	/* 340 */
	_nts_18,	/* 341 */
	_nts_16,	/* 342 */
	_nts_19,	/* 343 */
	_nts_20,	/* 344 */
	_nts_33,	/* 345 */
	_nts_19,	/* 346 */
	_nts_20,	/* 347 */
	_nts_33,	/* 348 */
	_nts_21,	/* 349 */
	_nts_22,	/* 350 */
	_nts_33,	/* 351 */
	_nts_21,	/* 352 */
	_nts_22,	/* 353 */
	_nts_33,	/* 354 */
	_nts_12,	/* 355 */
	_nts_13,	/* 356 */
	_nts_14,	/* 357 */
	_nts_12,	/* 358 */
	_nts_13,	/* 359 */
	_nts_14,	/* 360 */
	_nts_16,	/* 361 */
	_nts_16,	/* 362 */
	_nts_17,	/* 363 */
	_nts_17,	/* 364 */
	_nts_18,	/* 365 */
	_nts_18,	/* 366 */
	_nts_16,	/* 367 */
	_nts_16,	/* 368 */
	_nts_17,	/* 369 */
	_nts_18,	/* 370 */
	_nts_18,	/* 371 */
	_nts_16,	/* 372 */
	_nts_19,	/* 373 */
	_nts_20,	/* 374 */
	_nts_33,	/* 375 */
	_nts_19,	/* 376 */
	_nts_20,	/* 377 */
	_nts_33,	/* 378 */
	_nts_21,	/* 379 */
	_nts_22,	/* 380 */
	_nts_33,	/* 381 */
	_nts_21,	/* 382 */
	_nts_22,	/* 383 */
	_nts_33,	/* 384 */
	_nts_12,	/* 385 */
	_nts_35,	/* 386 */
	_nts_36,	/* 387 */
	_nts_12,	/* 388 */
	_nts_35,	/* 389 */
	_nts_36,	/* 390 */
	_nts_16,	/* 391 */
	_nts_16,	/* 392 */
	_nts_37,	/* 393 */
	_nts_37,	/* 394 */
	_nts_38,	/* 395 */
	_nts_38,	/* 396 */
	_nts_16,	/* 397 */
	_nts_16,	/* 398 */
	_nts_37,	/* 399 */
	_nts_38,	/* 400 */
	_nts_38,	/* 401 */
	_nts_16,	/* 402 */
	_nts_19,	/* 403 */
	_nts_39,	/* 404 */
	_nts_33,	/* 405 */
	_nts_19,	/* 406 */
	_nts_39,	/* 407 */
	_nts_33,	/* 408 */
	_nts_21,	/* 409 */
	_nts_40,	/* 410 */
	_nts_33,	/* 411 */
	_nts_21,	/* 412 */
	_nts_40,	/* 413 */
	_nts_33,	/* 414 */
	_nts_12,	/* 415 */
	_nts_35,	/* 416 */
	_nts_36,	/* 417 */
	_nts_12,	/* 418 */
	_nts_35,	/* 419 */
	_nts_36,	/* 420 */
	_nts_16,	/* 421 */
	_nts_16,	/* 422 */
	_nts_37,	/* 423 */
	_nts_37,	/* 424 */
	_nts_38,	/* 425 */
	_nts_38,	/* 426 */
	_nts_16,	/* 427 */
	_nts_16,	/* 428 */
	_nts_37,	/* 429 */
	_nts_38,	/* 430 */
	_nts_38,	/* 431 */
	_nts_16,	/* 432 */
	_nts_19,	/* 433 */
	_nts_39,	/* 434 */
	_nts_33,	/* 435 */
	_nts_19,	/* 436 */
	_nts_39,	/* 437 */
	_nts_33,	/* 438 */
	_nts_21,	/* 439 */
	_nts_40,	/* 440 */
	_nts_33,	/* 441 */
	_nts_21,	/* 442 */
	_nts_40,	/* 443 */
	_nts_33,	/* 444 */
	_nts_4,	/* 445 */
	_nts_4,	/* 446 */
	_nts_4,	/* 447 */
	_nts_4,	/* 448 */
	_nts_13,	/* 449 */
	_nts_41,	/* 450 */
	_nts_41,	/* 451 */
	_nts_41,	/* 452 */
	_nts_42,	/* 453 */
	_nts_42,	/* 454 */
	_nts_42,	/* 455 */
	_nts_1,	/* 456 */
	_nts_1,	/* 457 */
	_nts_1,	/* 458 */
	_nts_41,	/* 459 */
	_nts_41,	/* 460 */
	_nts_1,	/* 461 */
	_nts_1,	/* 462 */
	_nts_25,	/* 463 */
	_nts_43,	/* 464 */
	_nts_1,	/* 465 */
	_nts_4,	/* 466 */
	_nts_4,	/* 467 */
	_nts_4,	/* 468 */
	_nts_4,	/* 469 */
	_nts_1,	/* 470 */
	_nts_1,	/* 471 */
	_nts_41,	/* 472 */
	_nts_41,	/* 473 */
	_nts_1,	/* 474 */
	_nts_1,	/* 475 */
	_nts_41,	/* 476 */
	_nts_41,	/* 477 */
	_nts_1,	/* 478 */
	_nts_41,	/* 479 */
	_nts_1,	/* 480 */
	_nts_41,	/* 481 */
	_nts_1,	/* 482 */
	_nts_41,	/* 483 */
	_nts_1,	/* 484 */
	_nts_25,	/* 485 */
	_nts_1,	/* 486 */
	_nts_25,	/* 487 */
	_nts_1,	/* 488 */
	_nts_25,	/* 489 */
	_nts_1,	/* 490 */
	_nts_25,	/* 491 */
	_nts_1,	/* 492 */
	_nts_4,	/* 493 */
	_nts_44,	/* 494 */
	_nts_44,	/* 495 */
	_nts_44,	/* 496 */
	_nts_44,	/* 497 */
	_nts_44,	/* 498 */
	_nts_44,	/* 499 */
	_nts_44,	/* 500 */
	_nts_44,	/* 501 */
	_nts_45,	/* 502 */
	_nts_44,	/* 503 */
	_nts_44,	/* 504 */
	_nts_44,	/* 505 */
	_nts_44,	/* 506 */
	_nts_1,	/* 507 */
	_nts_1,	/* 508 */
	_nts_1,	/* 509 */
	_nts_2,	/* 510 */
	_nts_2,	/* 511 */
	_nts_3,	/* 512 */
	_nts_3,	/* 513 */
	_nts_1,	/* 514 */
	_nts_1,	/* 515 */
	_nts_1,	/* 516 */
	_nts_1,	/* 517 */
	_nts_2,	/* 518 */
	_nts_2,	/* 519 */
	_nts_3,	/* 520 */
	_nts_3,	/* 521 */
	_nts_44,	/* 522 */
	_nts_25,	/* 523 */
	_nts_25,	/* 524 */
	_nts_25,	/* 525 */
	_nts_25,	/* 526 */
	_nts_2,	/* 527 */
	_nts_3,	/* 528 */
	_nts_46,	/* 529 */
	_nts_47,	/* 530 */
	_nts_46,	/* 531 */
	_nts_47,	/* 532 */
	_nts_12,	/* 533 */
	_nts_48,	/* 534 */
	_nts_12,	/* 535 */
	_nts_48,	/* 536 */
	_nts_12,	/* 537 */
	_nts_48,	/* 538 */
	_nts_12,	/* 539 */
	_nts_1,	/* 540 */
	_nts_49,	/* 541 */
	_nts_39,	/* 542 */
	_nts_50,	/* 543 */
	_nts_49,	/* 544 */
	_nts_39,	/* 545 */
	_nts_50,	/* 546 */
	_nts_51,	/* 547 */
	_nts_40,	/* 548 */
	_nts_50,	/* 549 */
	_nts_51,	/* 550 */
	_nts_40,	/* 551 */
	_nts_50,	/* 552 */
	_nts_52,	/* 553 */
	_nts_52,	/* 554 */
	_nts_52,	/* 555 */
	_nts_52,	/* 556 */
	_nts_52,	/* 557 */
	_nts_52,	/* 558 */
	_nts_52,	/* 559 */
	_nts_52,	/* 560 */
	_nts_52,	/* 561 */
	_nts_52,	/* 562 */
	_nts_52,	/* 563 */
	_nts_52,	/* 564 */
	_nts_52,	/* 565 */
	_nts_52,	/* 566 */
	_nts_52,	/* 567 */
	_nts_52,	/* 568 */
	_nts_52,	/* 569 */
	_nts_52,	/* 570 */
	_nts_52,	/* 571 */
	_nts_52,	/* 572 */
	_nts_52,	/* 573 */
	_nts_52,	/* 574 */
	_nts_52,	/* 575 */
	_nts_52,	/* 576 */
	_nts_52,	/* 577 */
	_nts_52,	/* 578 */
	_nts_52,	/* 579 */
	_nts_52,	/* 580 */
	_nts_52,	/* 581 */
	_nts_52,	/* 582 */
	_nts_52,	/* 583 */
	_nts_52,	/* 584 */
	_nts_52,	/* 585 */
	_nts_52,	/* 586 */
	_nts_52,	/* 587 */
	_nts_52,	/* 588 */
	_nts_52,	/* 589 */
	_nts_52,	/* 590 */
	_nts_52,	/* 591 */
	_nts_52,	/* 592 */
	_nts_52,	/* 593 */
	_nts_52,	/* 594 */
	_nts_52,	/* 595 */
	_nts_52,	/* 596 */
	_nts_52,	/* 597 */
	_nts_52,	/* 598 */
	_nts_52,	/* 599 */
	_nts_52,	/* 600 */
	_nts_52,	/* 601 */
	_nts_52,	/* 602 */
	_nts_52,	/* 603 */
	_nts_52,	/* 604 */
	_nts_52,	/* 605 */
	_nts_52,	/* 606 */
	_nts_52,	/* 607 */
	_nts_52,	/* 608 */
	_nts_52,	/* 609 */
	_nts_52,	/* 610 */
};

static char *_templates[] = {
/* 0 */	0,
/* 1 */	"\tcopy\t%c,%0\n",	/* reg8: LOADI1(reg8) */
/* 2 */	"\tcopy\t%c,%0\n",	/* reg8: LOADI1(reg) */
/* 3 */	"\tld.8\t%c,3+%0\n",	/* reg8: LOADI1(reg32) */
/* 4 */	"\tcopy\t%c,%0\n",	/* reg8: LOADU1(reg8) */
/* 5 */	"\tcopy\t%c,%0\n",	/* reg8: LOADU1(reg) */
/* 6 */	"\tld.8\t%c,3+%0\n",	/* reg8: LOADU1(reg32) */
/* 7 */	"\tcopy\t%c,%0\n",	/* reg: LOADI2(reg) */
/* 8 */	"\tld.16\t%c,2+%0\n",	/* reg: LOADI2(reg32) */
/* 9 */	"\tcopy\t%c,%0\n",	/* reg: LOADU2(reg) */
/* 10 */	"\tld.16\t%c,2+%0\n",	/* reg: LOADU2(reg32) */
/* 11 */	"\tcopy\t%c,%0\n",	/* reg: LOADP2(reg) */
/* 12 */	"\tCOPY32(%c,%0)\n",	/* reg32: LOADI4(reg32) */
/* 13 */	"\tCOPY32(%c,%0)\n",	/* reg32: LOADU4(reg32) */
/* 14 */	"\tCOPY32(%c,%0)\n",	/* regf64: LOADF4(regf64) */
/* 15 */	"\tCOPY64(%c,%0)\n",	/* regf64: LOADF8(regf64) */
/* 16 */	"# read register\n",	/* reg8: INDIRI1(VREGP) */
/* 17 */	"# read register\n",	/* reg8: INDIRU1(VREGP) */
/* 18 */	"# read register\n",	/* reg: INDIRI2(VREGP) */
/* 19 */	"# read register\n",	/* reg: INDIRU2(VREGP) */
/* 20 */	"# read register\n",	/* reg: INDIRP2(VREGP) */
/* 21 */	"# read register\n",	/* reg32: INDIRI4(VREGP) */
/* 22 */	"# read register\n",	/* reg32: INDIRU4(VREGP) */
/* 23 */	"# read register\n",	/* regf64: INDIRF4(VREGP) */
/* 24 */	"# read register\n",	/* regf64: INDIRF8(VREGP) */
/* 25 */	"# write register\n",	/* stmt: ASGNI1(VREGP,reg8) */
/* 26 */	"# write register\n",	/* stmt: ASGNU1(VREGP,reg8) */
/* 27 */	"# write register\n",	/* stmt: ASGNI2(VREGP,reg) */
/* 28 */	"# write register\n",	/* stmt: ASGNU2(VREGP,reg) */
/* 29 */	"# write register\n",	/* stmt: ASGNP2(VREGP,reg) */
/* 30 */	"# write register\n",	/* stmt: ASGNI4(VREGP,reg32) */
/* 31 */	"# write register\n",	/* stmt: ASGNU4(VREGP,reg32) */
/* 32 */	"# write register\n",	/* stmt: ASGNF4(VREGP,regf64) */
/* 33 */	"# write register\n",	/* stmt: ASGNF8(VREGP,regf64) */
/* 34 */	"\tCVTF8F4(%c,%0)\n",	/* regf64: CVFF4(regf64) */
/* 35 */	"\tCVTF4F8(%c,%0)\n",	/* regf64: CVFF8(regf64) */
/* 36 */	"\tCVTI4F4(%c,%0)\n",	/* regf64: CVIF4(reg32) */
/* 37 */	"\tCVTI2F4(%c,%0)\n",	/* regf64: CVIF4(reg) */
/* 38 */	"\tCVTI4F8(%c,%0)\n",	/* regf64: CVIF8(reg32) */
/* 39 */	"\tCVTI2F8(%c,%0)\n",	/* regf64: CVIF8(reg) */
/* 40 */	"\tCVTF8I4(%c,%0)\n",	/* reg32: CVFI4(regf64) */
/* 41 */	"\tCVTF4I4(%c,%0)\n",	/* reg32: CVFI4(regf64) */
/* 42 */	"\tCVTF8I2(%c,%0)\n",	/* reg: CVFI2(regf64) */
/* 43 */	"\tCVTF4I2(%c,%0)\n",	/* reg: CVFI2(regf64) */
/* 44 */	"\tld.8\t%c,3+%0\n",	/* reg8: CVII1(reg32) */
/* 45 */	"\tcopy\t%c,%0\n",	/* reg8: CVII1(reg) */
/* 46 */	"\tld.8\t%c,3+%0\n",	/* reg8: CVUI1(reg32) */
/* 47 */	"\tcopy\t%c,%0\n",	/* reg8: CVUI1(reg) */
/* 48 */	"?\tcopy\t%c,%0\n",	/* reg8: LOADU1(LOADU2(CVII2(reg))) */
/* 49 */	"?\tcopy\t%c,%0\n",	/* reg8: LOADI1(CVUI2(reg)) */
/* 50 */	"?\tcopy\t%c,%0\n",	/* reg8: LOADU1(LOADU2(CVII2(reg8))) */
/* 51 */	"?\tcopy\t%c,%0\n",	/* reg8: LOADI1(CVUI2(reg8)) */
/* 52 */	"?\tcopy\t%c,%0\n\tsex\t%c\n",	/* reg: CVII2(reg8) */
/* 53 */	"\tld.16\t%c,2+%0\n",	/* reg: CVII2(reg32) */
/* 54 */	"?\tcopy\t%c,%0\n\tand.16\ta,0xff\n",	/* reg: CVUI2(reg8) */
/* 55 */	"\tld.16\t%c,2+%0\n",	/* reg: CVUI2(reg32) */
/* 56 */	"\tld.8\t%c,3+%0\n",	/* reg8: CVIU1(reg32) */
/* 57 */	"\tcopy\t%c,%0\n",	/* reg8: CVIU1(reg) */
/* 58 */	"\tld.8\t%c,3+%0\n",	/* reg8: CVUU1(reg32) */
/* 59 */	"\tcopy\t%c,%0\n",	/* reg8: CVUU1(reg) */
/* 60 */	"?\tcopy\t%c,%0\n\tsex\t%c,0xff\n",	/* reg: CVIU2(reg8) */
/* 61 */	"\tld.16\t%c,2+%0\n",	/* reg: CVIU2(reg32) */
/* 62 */	"?\tcopy\t%c,%0\n\tand.16\t%c,0xff\n",	/* reg: CVUU2(reg8) */
/* 63 */	"\tld.16\t%c,2+%0\n",	/* reg: CVUU2(reg32) */
/* 64 */	"\tCVTUU4(%c,%0)\n",	/* reg32: CVUU4(reg) */
/* 65 */	"\tCVTUU4(%c,%0)\n",	/* reg32: CVIU4(reg) */
/* 66 */	"\tCVTUU4(%c,%0)\n",	/* reg32: CVUI4(reg) */
/* 67 */	"\tCVTII4(%c,%0)\n",	/* reg32: CVII4(reg) */
/* 68 */	"\tld.16\t%c,%0\n",	/* reg: con */
/* 69 */	"\tld.8\t%c,%0\n",	/* reg8: con */
/* 70 */	"0",	/* c0: CNSTI2 */
/* 71 */	"1",	/* c1: CNSTI2 */
/* 72 */	"2",	/* c2: CNSTI2 */
/* 73 */	"0",	/* c0: CNSTU2 */
/* 74 */	"1",	/* c1: CNSTU2 */
/* 75 */	"2",	/* c2: CNSTU2 */
/* 76 */	"%a",	/* con1: CNSTI1 */
/* 77 */	"%a",	/* con1: CNSTU1 */
/* 78 */	"%a",	/* con2: CNSTI2 */
/* 79 */	"%a",	/* con2: CNSTU2 */
/* 80 */	"%a",	/* con4: CNSTI4 */
/* 81 */	"%a",	/* con4: CNSTU4 */
/* 82 */	"%a",	/* con4: CNSTP2 */
/* 83 */	"%0",	/* con: con1 */
/* 84 */	"%0",	/* con: con2 */
/* 85 */	"%0",	/* con: con4 */
/* 86 */	"%0",	/* con: c0 */
/* 87 */	"%0",	/* con: c1 */
/* 88 */	"%0",	/* con: c2 */
/* 89 */	"\tld.8\t%c,%0\n",	/* reg: con1 */
/* 90 */	"\tld.16\t%c,%0\n",	/* reg: con2 */
/* 91 */	"\tld.16\tc,%0\n",	/* creg: con */
/* 92 */	"?\tcopy\t%c,%0\n\tadd.16\t%c,%1\n",	/* reg: ADDI2(reg,reg) */
/* 93 */	"?\tcopy\t%c,%0\n\tadd.16\t%c,%1\n",	/* reg: ADDI2(reg,con) */
/* 94 */	"?\tcopy\t%c,%0\n\tadd.16\t%c,%1\n",	/* reg: ADDI2(reg,dref16) */
/* 95 */	"?\tcopy\t%c,%0\n\tadd.16\t%c,%1\n",	/* reg: ADDU2(reg,reg) */
/* 96 */	"?\tcopy\t%c,%0\n\tadd.16\t%c,%1\n",	/* reg: ADDU2(reg,con) */
/* 97 */	"?\tcopy\t%c,%0\n\tadd.16\t%c,%1\n",	/* reg: ADDU2(reg,dref16) */
/* 98 */	"\tlea\t%c,%1(%0)\n",	/* reg: ADDP2(reg,con) */
/* 99 */	"\tsh0add\t%c,%0,%1\n",	/* reg: ADDP2(reg,reg) */
/* 100 */	"\tsh1add\t%c,%2,%0\n",	/* reg: ADDP2(LSHI2(reg,c1),reg) */
/* 101 */	"\tsh1add\t%c,%2,%0\n",	/* reg: ADDP2(LSHU2(reg,c1),reg) */
/* 102 */	"?\tcopy\t%c,%0\n\tadd.8\t%c,%1\n",	/* reg8: ADDI2(reg8,reg8) */
/* 103 */	"?\tcopy\t%c,%0\n\tadd.8\t%c,%1\n",	/* reg8: ADDI2(reg8,con) */
/* 104 */	"?\tcopy\t%c,%0\n\tadd.8\t%c,%1\n",	/* reg8: ADDI2(CVII2(reg8),con) */
/* 105 */	"?\tcopy\t%c,%0\n\tadd.8\t%c,%1\n",	/* reg8: ADDI2(reg8,CVII2(dref8)) */
/* 106 */	"?\tcopy\t%c,%0\n\tadd.8\t%c,%1\n",	/* reg8: ADDI2(CVII2(reg8),CVII2(dref8)) */
/* 107 */	"?\tcopy\t%c,%0\n\tadd.8\t%c,%1\n",	/* reg8: ADDU2(reg8,reg8) */
/* 108 */	"?\tcopy\t%c,%0\n\tadd.8\t%c,%1\n",	/* reg8: ADDU2(reg8,con) */
/* 109 */	"?\tcopy\t%c,%0\n\tadd.8\t%c,%1\n",	/* reg8: ADDU2(reg8,CVUU2(dref8)) */
/* 110 */	"?\tcopy\t%c,%0\n\tadd.8\t%c,%1\n",	/* reg8: ADDU2(CVUU2(reg8),CVUU2(dref8)) */
/* 111 */	"?\tCOPY32(%c,%0)\n\tADD32(%c,%1)\n",	/* reg32: ADDI4(reg32,reg32) */
/* 112 */	"?\tCOPY32(%c,%0)\n\tADD32(%c,%1)\n",	/* reg32: ADDI4(reg32,INDIRI4(faddr)) */
/* 113 */	"?\tCOPY32(%c,%0)\n\tADD32(%c,%1)\n",	/* reg32: ADDU4(reg32,reg32) */
/* 114 */	"?\tCOPY32(%c,%0)\n\tADD32(%c,%1)\n",	/* reg32: ADDU4(reg32,INDIRU4(faddr)) */
/* 115 */	"?\tCOPY32(%c,%0)\n\tFADD32(%c,%1)\n",	/* regf64: ADDF4(regf64,regf64) */
/* 116 */	"?\tCOPY32(%c,%0)\n\tFADD32(%c,%1)\n",	/* regf64: ADDF4(regf64,INDIRF4(faddr)) */
/* 117 */	"?\tCOPY64(%c,%0)\n\tFADD64(%c,%1)\n",	/* regf64: ADDF8(regf64,regf64) */
/* 118 */	"?\tCOPY64(%c,%0)\n\tFADD64(%c,%1)\n",	/* regf64: ADDF8(regf64,INDIRF8(faddr)) */
/* 119 */	"?\tcopy\t%c,%0\n\tsub.16\t%c,%1\n",	/* reg: SUBI2(reg,reg) */
/* 120 */	"?\tcopy\t%c,%0\n\tsub.16\t%c,%1\n",	/* reg: SUBI2(reg,con) */
/* 121 */	"?\tcopy\t%c,%0\n\tsub.16\t%c,%1\n",	/* reg: SUBI2(reg,dref16) */
/* 122 */	"?\tcopy\t%c,%0\n\tsub.16\t%c,%1\n",	/* reg: SUBU2(reg,reg) */
/* 123 */	"?\tcopy\t%c,%0\n\tsub.16\t%c,%1\n",	/* reg: SUBU2(reg,con) */
/* 124 */	"?\tcopy\t%c,%0\n\tsub.16\t%c,%1\n",	/* reg: SUBU2(reg,dref16) */
/* 125 */	"?\tcopy\t%c,%0\n\tsub.16\t%c,%1\n",	/* reg: SUBP2(reg,reg) */
/* 126 */	"?\tcopy\t%c,%0\n\tsub.16\t%c,%1\n",	/* reg: SUBP2(reg,con) */
/* 127 */	"?\tcopy\t%c,%0\n\tsub.16\t%c,%1\n",	/* reg: SUBP2(reg,dref16) */
/* 128 */	"?\tcopy\t%c,%0\n\tsub.8\t%c,%1\n",	/* reg8: SUBI2(reg8,reg8) */
/* 129 */	"?\tcopy\t%c,%0\n\tsub.8\t%c,%1\n",	/* reg8: SUBI2(reg8,con) */
/* 130 */	"?\tcopy\t%c,%0\n\tsub.8\t%c,%1\n",	/* reg8: SUBI2(reg8,CVII2(dref16)) */
/* 131 */	"?\tcopy\t%c,%0\n\tsub.8\t%c,%1\n",	/* reg8: SUBU2(reg8,reg8) */
/* 132 */	"?\tcopy\t%c,%0\n\tsub.8\t%c,%1\n",	/* reg8: SUBU2(reg8,con) */
/* 133 */	"?\tcopy\t%c,%0\n\tsub.8\t%c,%1\n",	/* reg8: SUBU2(reg8,CVUU2(dref16)) */
/* 134 */	"?\tCOPY32(%c,%0)\n\tSUB32(%c,%1)\n",	/* reg32: SUBI4(reg32,reg32) */
/* 135 */	"?\tCOPY32(%c,%0)\n\tSUB32(%c,%1)\n",	/* reg32: SUBI4(reg32,INDIRI4(faddr)) */
/* 136 */	"?\tCOPY32(%c,%0)\n\tSUB32(%c,%1)\n",	/* reg32: SUBU4(reg32,reg32) */
/* 137 */	"?\tCOPY32(%c,%0)\n\tSUB32(%c,%1)\n",	/* reg32: SUBU4(reg32,INDIRU4(faddr)) */
/* 138 */	"?\tCOPY32(%c,%0)\n\tFSUB32(%c,%1)\n",	/* regf64: SUBF4(regf64,regf64) */
/* 139 */	"?\tCOPY32(%c,%0)\n\tFSUB32(%c,%1)\n",	/* regf64: SUBF4(regf64,INDIRF4(faddr)) */
/* 140 */	"?\tCOPY64(%c,%0)\n\tFSUB64(%c,%1)\n",	/* regf64: SUBF8(regf64,regf64) */
/* 141 */	"?\tCOPY64(%c,%0)\n\tFSUB64(%c,%1)\n",	/* regf64: SUBF8(regf64,INDIRF8(faddr)) */
/* 142 */	"?\tcopy\t%c,%0\n\tand.16\t%c,%1\n",	/* reg: BANDI2(reg,reg) */
/* 143 */	"?\tcopy\t%c,%0\n\tand.16\t%c,%1\n",	/* reg: BANDI2(reg,con) */
/* 144 */	"?\tcopy\t%c,%0\n\tand.16\t%c,%1\n",	/* reg: BANDI2(reg,dref16) */
/* 145 */	"?\tcopy\t%c,%0\n\tand.16\t%c,%1\n",	/* reg: BANDU2(reg,reg) */
/* 146 */	"?\tcopy\t%c,%0\n\tand.16\t%c,%1\n",	/* reg: BANDU2(reg,con) */
/* 147 */	"?\tcopy\t%c,%0\n\tand.16\t%c,%1\n",	/* reg: BANDU2(reg,dref16) */
/* 148 */	"?\tcopy\t%c,%0\n\tand.8\t%c,%1\n",	/* reg8: BANDI2(reg8,reg8) */
/* 149 */	"?\tcopy\t%c,%0\n\tand.8\t%c,%1\n",	/* reg8: BANDI2(CVII2(reg8),reg8) */
/* 150 */	"?\tcopy\t%c,%0\n\tand.8\t%c,%1\n",	/* reg8: BANDI2(CVUI2(reg8),reg8) */
/* 151 */	"?\tcopy\t%c,%0\n\tand.8\t%c,%1\n",	/* reg8: BANDI2(reg8,con) */
/* 152 */	"?\tcopy\t%c,%0\n\tand.8\t%c,%1\n",	/* reg8: BANDI2(CVII2(reg8),con) */
/* 153 */	"?\tcopy\t%c,%0\n\tand.8\t%c,%1\n",	/* reg8: BANDI2(CVUI2(reg8),con) */
/* 154 */	"?\tcopy\t%c,%0\n\tand.8\t%c,%1\n",	/* reg8: BANDI2(reg8,CVII2(dref16)) */
/* 155 */	"?\tcopy\t%c,%0\n\tand.8\t%c,%1\n",	/* reg8: BANDU2(reg8,reg8) */
/* 156 */	"?\tcopy\t%c,%0\n\tand.8\t%c,%1\n",	/* reg8: BANDU2(reg8,con) */
/* 157 */	"?\tcopy\t%c,%0\n\tand.8\t%c,%1\n",	/* reg8: BANDU2(reg8,CVUU2(dref16)) */
/* 158 */	"?\tCOPY32(%c,%0)\n\tAND32(%c,%1)\n",	/* reg32: BANDI4(reg32,reg32) */
/* 159 */	"?\tCOPY32(%c,%0)\n\tAND32(%c,%1)\n",	/* reg32: BANDI4(reg32,INDIRI4(faddr)) */
/* 160 */	"?\tCOPY32(%c,%0)\n\tAND32(%c,%1)\n",	/* reg32: BANDU4(reg32,reg32) */
/* 161 */	"?\tCOPY32(%c,%0)\n\tAND32(%c,%1)\n",	/* reg32: BANDU4(reg32,INDIRU4(faddr)) */
/* 162 */	"\tld.16\ta,0\n\tst.16\t%c,a\n\tld.16\ta,2+%0\n\tand.16\ta,%1\n\tst.16\t2+%c,a\n",	/* reg32: BANDU4(reg32,con) */
/* 163 */	"ld.16\ta,-1\n\txor.16\ta,b\n",	/* reg: BCOMI2(reg) */
/* 164 */	"ld.16\ta,-1\n\txor.16\ta,b\n",	/* reg: BCOMU2(reg) */
/* 165 */	"?\tcopy\t%c,%0\n\tor.16\t%c,%1\n",	/* reg: BORI2(reg,reg) */
/* 166 */	"?\tcopy\t%c,%0\n\tor.16\t%c,%1\n",	/* reg: BORI2(reg,con) */
/* 167 */	"?\tcopy\t%c,%0\n\tor.16\t%c,%1\n",	/* reg: BORI2(reg,dref16) */
/* 168 */	"?\tcopy\t%c,%0\n\tor.16\t%c,%1\n",	/* reg: BORU2(reg,reg) */
/* 169 */	"?\tcopy\t%c,%0\n\tor.16\t%c,%1\n",	/* reg: BORU2(reg,con) */
/* 170 */	"?\tcopy\t%c,%0\n\tor.16\t%c,%1\n",	/* reg: BORU2(reg,dref16) */
/* 171 */	"?\tcopy\t%c,%0\n\tor.8\t%c,%1\n",	/* reg8: BORI2(reg8,reg8) */
/* 172 */	"?\tcopy\t%c,%0\n\tor.8\t%c,%1\n",	/* reg8: BORI2(reg8,con) */
/* 173 */	"?\tcopy\t%c,%0\n\tor.8\t%c,%1\n",	/* reg8: BORI2(reg8,CVII2(dref16)) */
/* 174 */	"?\tcopy\t%c,%0\n\tor.8\t%c,%1\n",	/* reg8: BORU2(reg8,reg8) */
/* 175 */	"?\tcopy\t%c,%0\n\tor.8\t%c,%1\n",	/* reg8: BORU2(reg8,con) */
/* 176 */	"?\tcopy\t%c,%0\n\tor.8\t%c,%1\n",	/* reg8: BORU2(reg8,CVUU2(dref16)) */
/* 177 */	"?\tCOPY32(%c,%0)\n\tBCOM32(%c)\n",	/* reg32: BCOMI4(reg32) */
/* 178 */	"?\tCOPY32(%c,%0)\n\tBCOM32(%c)\n",	/* reg32: BCOMU4(reg32) */
/* 179 */	"?\tCOPY32(%c,%0)\n\tOR32(%c,%1)\n",	/* reg32: BORI4(reg32,reg32) */
/* 180 */	"?\tCOPY32(%c,%0)\n\tOR32(%c,%1)\n",	/* reg32: BORI4(reg32,INDIRI4(faddr)) */
/* 181 */	"?\tCOPY32(%c,%0)\n\tOR32(%c,%1)\n",	/* reg32: BORU4(reg32,reg32) */
/* 182 */	"?\tCOPY32(%c,%0)\n\tOR32(%c,%1)\n",	/* reg32: BORU4(reg32,INDIRU4(faddr)) */
/* 183 */	"?\tcopy\t%c,%0\n\txor.16\t%c,%1\n",	/* reg: BXORI2(reg,reg) */
/* 184 */	"?\tcopy\t%c,%0\n\txor.16\t%c,%1\n",	/* reg: BXORU2(reg,reg) */
/* 185 */	"?\tCOPY32(%c,%0)\n\tXOR32(%c,%1)\n",	/* reg32: BXORI4(reg32,reg32) */
/* 186 */	"?\tCOPY32(%c,%0)\n\tXOR32(%c,%1)\n",	/* reg32: BXORI4(reg32,INDIRI4(faddr)) */
/* 187 */	"?\tCOPY32(%c,%0)\n\tXOR32(%c,%1)\n",	/* reg32: BXORU4(reg32,reg32) */
/* 188 */	"?\tCOPY32(%c,%0)\n\tXOR32(%c,%1)\n",	/* reg32: BXORU4(reg32,INDIRU4(faddr)) */
/* 189 */	"?\tcopy\t%c,%0\n\tcall\t$muli16\n",	/* reg: MULI2(reg,reg) */
/* 190 */	"?\tcopy\t%c,%0\n\tcall\t$mulu16\n",	/* reg: MULU2(reg,reg) */
/* 191 */	"?\tCOPY32(%c,%0)\n\tMULI32(%c,%1)\n",	/* reg32: MULI4(reg32,reg32) */
/* 192 */	"?\tCOPY32(%c,%0)\n\tMULI32(%c,%1)\n",	/* reg32: MULI4(reg32,INDIRI4(faddr)) */
/* 193 */	"?\tCOPY32(%c,%0)\n\tMULU32(%c,%1)\n",	/* reg32: MULU4(reg32,reg32) */
/* 194 */	"?\tCOPY32(%c,%0)\n\tMULU32(%c,%1)\n",	/* reg32: MULU4(reg32,INDIRU4(faddr)) */
/* 195 */	"?\tCOPY32(%c,%0)\n\tFMUL32(%c,%1)\n",	/* regf64: MULF4(regf64,regf64) */
/* 196 */	"?\tCOPY32(%c,%0)\n\tFMUL32(%c,%1)\n",	/* regf64: MULF4(regf64,INDIRF4(faddr)) */
/* 197 */	"?\tCOPY64(%c,%0)\n\tFMUL64(%c,%1)\n",	/* regf64: MULF8(regf64,regf64) */
/* 198 */	"?\tCOPY64(%c,%0)\n\tFMUL64(%c,%1)\n",	/* regf64: MULF8(regf64,INDIRF8(faddr)) */
/* 199 */	"?\tcopy\t%c,%0\n\tcall\t$divi16\n",	/* reg: DIVI2(reg,reg) */
/* 200 */	"?\tcopy\t%c,%0\n\tcall\t$divu16\n",	/* reg: DIVU2(reg,reg) */
/* 201 */	"?\tCOPY32(%c,%0)\n\tDIVI32(%c,%1)\n",	/* reg32: DIVI4(reg32,reg32) */
/* 202 */	"?\tCOPY32(%c,%0)\n\tDIVI32(%c,%1)\n",	/* reg32: DIVI4(reg32,INDIRI4(faddr)) */
/* 203 */	"?\tCOPY32(%c,%0)\n\tDIVU32(%c,%1)\n",	/* reg32: DIVU4(reg32,reg32) */
/* 204 */	"?\tCOPY32(%c,%0)\n\tDIVU32(%c,%1)\n",	/* reg32: DIVU4(reg32,INDIRU4(faddr)) */
/* 205 */	"?\tCOPY32(%c,%0)\n\tFDIV32(%c,%1)\n",	/* regf64: DIVF4(regf64,regf64) */
/* 206 */	"?\tCOPY32(%c,%0)\n\tFDIV32(%c,%1)\n",	/* regf64: DIVF4(regf64,INDIRF4(faddr)) */
/* 207 */	"?\tCOPY64(%c,%0)\n\tFDIV64(%c,%1)\n",	/* regf64: DIVF8(regf64,regf64) */
/* 208 */	"?\tCOPY64(%c,%0)\n\tFDIV64(%c,%1)\n",	/* regf64: DIVF8(regf64,INDIRF8(faddr)) */
/* 209 */	"?\tcopy\t%c,%0\n\tcall\t$modi16\n",	/* reg: MODI2(reg,reg) */
/* 210 */	"?\tcopy\t%c,%0\n\tcall\t$modu16\n",	/* reg: MODU2(reg,reg) */
/* 211 */	"?\tCOPY32(%c,%0)\n\tMODI32(%c,%1)\n",	/* reg32: MODI4(reg32,reg32) */
/* 212 */	"?\tCOPY32(%c,%0)\n\tMODI32(%c,%1)\n",	/* reg32: MODI4(reg32,INDIRI4(faddr)) */
/* 213 */	"?\tCOPY32(%c,%0)\n\tMODU32(%c,%1)\n",	/* reg32: MODU4(reg32,reg32) */
/* 214 */	"?\tCOPY32(%c,%0)\n\tMODU32(%c,%1)\n",	/* reg32: MODU4(reg32,INDIRU4(faddr)) */
/* 215 */	"\tld.16\t%c,0\n\tsub.16\t%c,%0\n",	/* reg: NEGI2(reg) */
/* 216 */	"\tNEGI32(%c,%0)\n",	/* reg32: NEGI4(reg32) */
/* 217 */	"\tNEGI32(%c,%0)\n",	/* reg32: NEGI4(INDIRI4(faddr)) */
/* 218 */	"\tFNEG32(%c,%0)\n",	/* regf64: NEGF4(regf64) */
/* 219 */	"\tFNEG32(%c,%0)\n",	/* regf64: NEGF4(INDIRF4(faddr)) */
/* 220 */	"\tFNEG64(%c,%0)\n",	/* regf64: NEGF8(regf64) */
/* 221 */	"\tFNEG64(%c,%0)\n",	/* regf64: NEGF8(INDIRF8(faddr)) */
/* 222 */	"?\tcopy\t%c,%0\n\tshl.16\t%c\n",	/* reg: LSHI2(reg,c1) */
/* 223 */	"?\tcopy\t%c,%0\n\tshl.16\t%c\n\tshl.16\t%c\n",	/* reg: LSHI2(reg,c2) */
/* 224 */	"?\tcopy\t%c,%0\n\tvshl.16\t%c\n",	/* reg: LSHI2(reg,creg) */
/* 225 */	"?\tcopy\t%c,%0\n\tcopy\tc,%1\n\tvshl.16\t%c\n",	/* reg: LSHI2(reg,reg) */
/* 226 */	"?\tcopy\t%c,%0\n\tshl.16\t%c\n",	/* reg: LSHU2(reg,c1) */
/* 227 */	"?\tcopy\t%c,%0\n\tshl.16\t%c\n\tshl.16\t%c\n",	/* reg: LSHU2(reg,c2) */
/* 228 */	"?\tcopy\t%c,%0\n\tvshl.16\t%c\n",	/* reg: LSHU2(reg,creg) */
/* 229 */	"?\tcopy\t%c,%0\n\tcopy\tc,%1\n\tvshl.16\t%c\n",	/* reg: LSHU2(reg,reg) */
/* 230 */	"?\tCOPY32(%c,%0)\n\tLSH32(%c)\n",	/* reg32: LSHI4(reg32,creg) */
/* 231 */	"?\tCOPY32(%c,%0)\n\tcopy\tc,%1\n\tLSH32(%c)\n",	/* reg32: LSHI4(reg32,reg) */
/* 232 */	"?\tCOPY32(%c,%0)\n\tLSH32(%c)\n",	/* reg32: LSHU4(reg32,creg) */
/* 233 */	"?\tCOPY32(%c,%0)\n\tcopy\tc,%1\n\tLSH32(%c)\n",	/* reg32: LSHU4(reg32,reg) */
/* 234 */	"?\tcopy\t%c,%0\n\tshr.16\t%c\n",	/* reg: RSHI2(reg,c1) */
/* 235 */	"?\tcopy\t%c,%0\n\tshr.16\t%c\n\tshr.16\t%c\n",	/* reg: RSHI2(reg,c2) */
/* 236 */	"?\tcopy\t%c,%0\n\tvshr.16\t%c\n",	/* reg: RSHI2(reg,creg) */
/* 237 */	"?\tcopy\t%c,%0\n\tcopy\tc,%1\n\tvshr.16\t%c\n",	/* reg: RSHI2(reg,reg) */
/* 238 */	"?\tcopy\t%c,%0\n\tshr.16\t%c\n",	/* reg: RSHU2(reg,c1) */
/* 239 */	"?\tcopy\t%c,%0\n\tshr.16\t%c\n\tshr.16\t%c\n",	/* reg: RSHU2(reg,c2) */
/* 240 */	"?\tcopy\t%c,%0\n\tvshr.16\t%c\n",	/* reg: RSHU2(reg,creg) */
/* 241 */	"?\tcopy\t%c,%0\n\tcopy\tc,%1\n\tvshr.16\t%c\n",	/* reg: RSHU2(reg,reg) */
/* 242 */	"?\tCOPY32(%c,%0)\n\tRSH32(%c)\n",	/* reg32: RSHI4(reg32,creg) */
/* 243 */	"?\tCOPY32(%c,%0)\n\tcopy\tc,%1\n\tRSH32(%c)\n",	/* reg32: RSHI4(reg32,reg) */
/* 244 */	"?\tCOPY32(%c,%0)\n\tRSH32(%c)\n",	/* reg32: RSHU4(reg32,creg) */
/* 245 */	"?\tCOPY32(%c,%0)\n\tcopy\tc,%1\n\tRSH32(%c)\n",	/* reg32: RSHU4(reg32,reg) */
/* 246 */	"\tcmpb.eq.16\t%0,%1,%a\n",	/* stmt: EQI2(reg,reg) */
/* 247 */	"\tcmpb.eq.16\t%0,%1,%a\n",	/* stmt: EQI2(reg,con) */
/* 248 */	"\tcmpb.eq.16\t%0,%1,%a\n",	/* stmt: EQI2(reg,dref16) */
/* 249 */	"\tbclr.16\t%0,%1,%a\n",	/* stmt: EQI2(BANDI2(reg,con),c0) */
/* 250 */	"\tcmpb.eq.16\t%0,%1,%a\n",	/* stmt: EQU2(reg,reg) */
/* 251 */	"\tcmpb.eq.16\t%0,%1,%a\n",	/* stmt: EQU2(reg,con) */
/* 252 */	"\tcmpb.eq.16\t%0,%1,%a\n",	/* stmt: EQU2(reg,dref16) */
/* 253 */	"\tbclr.16\t%0,%1,%a\n",	/* stmt: EQU2(BANDU2(reg,con),c0) */
/* 254 */	"\tcmpb.eq.8\t%0,%1,%a\n",	/* stmt: EQI2(reg8,reg8) */
/* 255 */	"\tcmpb.eq.8\t%0,%1,%a\n",	/* stmt: EQI2(CVII2(reg8),CVII2(reg8)) */
/* 256 */	"\tcmpb.eq.8\t%0,%1,%a\n",	/* stmt: EQI2(reg8,con) */
/* 257 */	"\tcmpb.eq.8\t%0,%1,%a\n",	/* stmt: EQI2(CVII2(reg8),con) */
/* 258 */	"\tcmpb.eq.8\t%0,%1,%a\n",	/* stmt: EQI2(CVUI2(reg8),con) */
/* 259 */	"\tcmpb.eq.8\t%0,%1,%a\n",	/* stmt: EQI2(reg8,CVII2(dref8)) */
/* 260 */	"\tcmpb.eq.8\t%0,%1,%a\n",	/* stmt: EQI2(CVII2(reg8),CVII2(dref8)) */
/* 261 */	"\tcmpb.eq.8\t%0,%1,%a\n",	/* stmt: EQI2(reg8,CVII2(reg8)) */
/* 262 */	"\tbclr.8\t%0,%1,%a\n",	/* stmt: EQI2(BANDI2(reg8,con),c0) */
/* 263 */	"\tbclr.8\t%0,%1,%a\n",	/* stmt: EQI2(BANDI2(CVII2(reg8),con),c0) */
/* 264 */	"\tbclr.8\t%0,%1,%a\n",	/* stmt: EQI2(BANDI2(CVUI2(reg8),con),c0) */
/* 265 */	"\tcmpb.eq.8\t%0,%1,%a\n",	/* stmt: EQU2(reg8,reg8) */
/* 266 */	"\tcmpb.eq.8\t%0,%1,%a\n",	/* stmt: EQU2(reg8,con) */
/* 267 */	"\tcmpb.eq.8\t%0,%1,%a\n",	/* stmt: EQU2(CVUU2(reg8),con) */
/* 268 */	"\tcmpb.eq.8\t%0,%1,%a\n",	/* stmt: EQU2(reg8,CVUU2(dref8)) */
/* 269 */	"\tcmpb.eq.8\t%0,%1,%a\n",	/* stmt: EQU2(CVUU2(reg8),CVUU2(dref8)) */
/* 270 */	"\tcmpb.eq.8\t%0,%1,%a\n",	/* stmt: EQU2(reg8,CVUU2(reg8)) */
/* 271 */	"\tbclr.8\t%0,%1,%a\n",	/* stmt: EQU2(BANDU2(reg8,con),c0) */
/* 272 */	"\tCMPBEQ32(%0,%1,%a)\n",	/* stmt: EQI4(reg32,reg32) */
/* 273 */	"\tCMPBEQ32(%0,%1,%a)\n",	/* stmt: EQI4(reg32,INDIRI4(faddr)) */
/* 274 */	"\tCMPBEQ32(%0,%1,%a)\n",	/* stmt: EQI4(mem,mem) */
/* 275 */	"\tCMPBEQ32(%0,%1,%a)\n",	/* stmt: EQU4(reg32,reg32) */
/* 276 */	"\tCMPBEQ32(%0,%1,%a)\n",	/* stmt: EQU4(reg32,INDIRU4(faddr)) */
/* 277 */	"\tCMPBEQ32(%0,%1,%a)\n",	/* stmt: EQU4(mem,mem) */
/* 278 */	"\tCMPBEQF32(%0,%1,%a)\n",	/* stmt: EQF4(regf64,regf64) */
/* 279 */	"\tCMPBEQF32(%0,%1,%a)\n",	/* stmt: EQF4(regf64,INDIRF4(faddr)) */
/* 280 */	"\tCMPBEQF32(%0,%1,%a)\n",	/* stmt: EQF4(mem,mem) */
/* 281 */	"\tCMPBEQF64(%0,%1,%a)\n",	/* stmt: EQF8(regf64,regf64) */
/* 282 */	"\tCMPBEQF64(%0,%1,%a)\n",	/* stmt: EQF8(regf64,INDIRF8(faddr)) */
/* 283 */	"\tCMPBEQF64(%0,%1,%a)\n",	/* stmt: EQF8(mem,mem) */
/* 284 */	"\tcmpb.ne.16\t%0,%1,%a\n",	/* stmt: NEI2(reg,reg) */
/* 285 */	"\tcmpb.ne.16\t%0,%1,%a\n",	/* stmt: NEI2(reg,con) */
/* 286 */	"\tcmpb.ne.16\t%0,%1,%a\n",	/* stmt: NEI2(reg,c0) */
/* 287 */	"\tcmpb.ne.16\t%0,%1,%a\n",	/* stmt: NEI2(reg,dref16) */
/* 288 */	"\tbset.16\t%0,%1,%a\n",	/* stmt: NEI2(BANDI2(reg,con),c0) */
/* 289 */	"\tcmpb.ne.16\t%0,%1,%a\n",	/* stmt: NEU2(reg,reg) */
/* 290 */	"\tcmpb.ne.16\t%0,%1,%a\n",	/* stmt: NEU2(reg,con) */
/* 291 */	"\tcmpb.ne.16\t%0,%1,%a\n",	/* stmt: NEU2(reg,c0) */
/* 292 */	"\tcmpb.ne.16\t%0,%1,%a\n",	/* stmt: NEU2(reg,dref16) */
/* 293 */	"\tbset.16\t%0,%1,%a\n",	/* stmt: NEU2(BANDI2(reg,con),c0) */
/* 294 */	"\tcmpb.ne.8\t%0,%1,%a\n",	/* stmt: NEI2(reg8,reg8) */
/* 295 */	"\tcmpb.ne.8\t%0,%1,%a\n",	/* stmt: NEI2(CVII2(reg8),CVII2(reg8)) */
/* 296 */	"\tcmpb.ne.8\t%0,%1,%a\n",	/* stmt: NEI2(reg8,con) */
/* 297 */	"\tcmpb.ne.8\t%0,%1,%a\n",	/* stmt: NEI2(CVII2(reg8),con) */
/* 298 */	"\tcmpb.ne.8\t%0,%1,%a\n",	/* stmt: NEI2(CVUI2(reg8),con) */
/* 299 */	"\tcmpb.ne.8\t%0,%1,%a\n",	/* stmt: NEI2(reg8,CVII2(dref8)) */
/* 300 */	"\tcmpb.ne.8\t%0,%1,%a\n",	/* stmt: NEI2(CVII2(reg8),CVII2(dref8)) */
/* 301 */	"\tcmpb.ne.8\t%0,%1,%a\n",	/* stmt: NEI2(reg8,CVII2(reg8)) */
/* 302 */	"\tbset.8\t%0,%1,%a\n",	/* stmt: NEI2(BANDI2(reg8,con),c0) */
/* 303 */	"\tbset.8\t%0,%1,%a\n",	/* stmt: NEI2(BANDI2(CVII2(reg8),con),c0) */
/* 304 */	"\tbset.8\t%0,%1,%a\n",	/* stmt: NEI2(BANDI2(CVUI2(reg8),con),c0) */
/* 305 */	"\tcmpb.ne.8\t%0,%1,%a\n",	/* stmt: NEU2(reg8,reg8) */
/* 306 */	"\tcmpb.ne.8\t%0,%1,%a\n",	/* stmt: NEU2(reg8,con) */
/* 307 */	"\tcmpb.ne.8\t%0,%1,%a\n",	/* stmt: NEU2(CVUU2(reg8),con) */
/* 308 */	"\tcmpb.ne.8\t%0,%1,%a\n",	/* stmt: NEU2(reg8,CVUU2(dref8)) */
/* 309 */	"\tcmpb.ne.8\t%0,%1,%a\n",	/* stmt: NEU2(CVUU2(reg8),CVUU2(dref8)) */
/* 310 */	"\tcmpb.ne.8\t%0,%1,%a\n",	/* stmt: NEU2(reg8,CVUU2(reg8)) */
/* 311 */	"\tbset.8\t%0,%1,%a\n",	/* stmt: NEU2(BANDU2(reg8,con),c0) */
/* 312 */	"\tCMPBNE32(%0,%1,%a)\n",	/* stmt: NEI4(reg32,reg32) */
/* 313 */	"\tCMPBNE32(%0,%1,%a)\n",	/* stmt: NEI4(reg32,INDIRU4(faddr)) */
/* 314 */	"\tCMPBNE32(%0,%1,%a)\n",	/* stmt: NEI4(mem,mem) */
/* 315 */	"\tCMPBNE32(%0,%1,%a)\n",	/* stmt: NEU4(reg32,reg32) */
/* 316 */	"\tCMPBNE32(%0,%1,%a)\n",	/* stmt: NEU4(reg32,INDIRU4(faddr)) */
/* 317 */	"\tCMPBNE32(%0,%1,%a)\n",	/* stmt: NEU4(mem,mem) */
/* 318 */	"\tCMPBNEF32(%0,%1,%a)\n",	/* stmt: NEF4(regf64,regf64) */
/* 319 */	"\tCMPBNEF32(%0,%1,%a)\n",	/* stmt: NEF4(regf64,INDIRF4(faddr)) */
/* 320 */	"\tCMPBNEF32(%0,%1,%a)\n",	/* stmt: NEF4(mem,mem) */
/* 321 */	"\tCMPBNEF64(%0,%1,%a)\n",	/* stmt: NEF8(regf64,regf64) */
/* 322 */	"\tCMPBNEF64(%0,%1,%a)\n",	/* stmt: NEF8(regf64,INDIRF8(faddr)) */
/* 323 */	"\tCMPBNEF64(%0,%1,%a)\n",	/* stmt: NEF8(mem,mem) */
/* 324 */	"\tcmpb.lt.16\t%0,%1,%a\n",	/* stmt: LTI2(reg,reg) */
/* 325 */	"\tcmpb.lt.16\t%0,%1,%a\n",	/* stmt: LTI2(reg,con) */
/* 326 */	"\tcmpb.lt.16\t%0,%1,%a\n",	/* stmt: LTI2(reg,dref16) */
/* 327 */	"\tcmp.16\t%0,%1\n\tbr.ltu\t%a\n",	/* stmt: LTU2(reg,reg) */
/* 328 */	"\tcmp.16\t%0,%1\n\tbr.ltu\t%a\n",	/* stmt: LTU2(reg,con) */
/* 329 */	"\tcmp.16\t%0,%1\n\tbr.ltu\t%a\n",	/* stmt: LTU2(reg,dref16) */
/* 330 */	"\tcmpb.lt.8\t%0,%1,%a\n",	/* stmt: LTI2(reg8,reg8) */
/* 331 */	"\tcmpb.lt.8\t%0,%1,%a\n",	/* stmt: LTI2(CVII2(reg8),CVII2(reg8)) */
/* 332 */	"\tcmpb.lt.8\t%0,%1,%a\n",	/* stmt: LTI2(reg8,con) */
/* 333 */	"\tcmpb.lt.8\t%0,%1,%a\n",	/* stmt: LTI2(CVII2(reg8),con) */
/* 334 */	"\tcmpb.lt.8\t%0,%1,%a\n",	/* stmt: LTI2(CVUI2(reg8),con) */
/* 335 */	"\tcmpb.lt.8\t%0,%1,%a\n",	/* stmt: LTI2(reg8,CVII2(dref8)) */
/* 336 */	"\tcmpb.lt.8\t%0,%1,%a\n",	/* stmt: LTI2(CVII2(reg8),CVII2(dref8)) */
/* 337 */	"\tcmpb.lt.8\t%0,%1,%a\n",	/* stmt: LTI2(reg8,CVII2(reg8)) */
/* 338 */	"\tcmp.8\t%0,%1\n\tbr.ltu\t%a\n",	/* stmt: LTU2(reg8,reg8) */
/* 339 */	"\tcmp.8\t%0,%1\n\tbr.ltu\t%a\n",	/* stmt: LTU2(reg8,con) */
/* 340 */	"\tcmp.8\t%0,%1\n\tbr.ltu\t%a\n",	/* stmt: LTU2(reg8,CVUU2(dref8)) */
/* 341 */	"\tcmp.8\t%0,%1\n\tbr.ltu\t%a\n",	/* stmt: LTU2(CVUU2(reg8),CVUU2(dref8)) */
/* 342 */	"\tcmp.8\t%0,%1\n\tbr.ltu\t%a\n",	/* stmt: LTU2(reg8,CVUU2(reg8)) */
/* 343 */	"\tCMPBLT32(%0,%1,%a)\n",	/* stmt: LTI4(reg32,reg32) */
/* 344 */	"\tCMPBLT32(%0,%1,%a)\n",	/* stmt: LTI4(reg32,INDIRI4(faddr)) */
/* 345 */	"\tCMPBLT32(%0,%1,%a)\n",	/* stmt: LTI4(mem,mem) */
/* 346 */	"\tCMPBLTU32(%0,%1,%a)\n",	/* stmt: LTU4(reg32,reg32) */
/* 347 */	"\tCMPBLTU32(%0,%1,%a)\n",	/* stmt: LTU4(reg32,INDIRU4(faddr)) */
/* 348 */	"\tCMPBLTU32(%0,%1,%a)\n",	/* stmt: LTU4(mem,mem) */
/* 349 */	"\tCMPBLTF32(%0,%1,%a)\n",	/* stmt: LTF4(regf64,regf64) */
/* 350 */	"\tCMPBLTF32(%0,%1,%a)\n",	/* stmt: LTF4(regf64,INDIRF4(faddr)) */
/* 351 */	"\tCMPBLTF32(%0,%1,%a)\n",	/* stmt: LTF4(mem,mem) */
/* 352 */	"\tCMPBLTF32(%0,%1,%a)\n",	/* stmt: LTF8(regf64,regf64) */
/* 353 */	"\tCMPBLTF64(%0,%1,%a)\n",	/* stmt: LTF8(regf64,INDIRF8(faddr)) */
/* 354 */	"\tCMPBLTF64(%0,%1,%a)\n",	/* stmt: LTF8(mem,mem) */
/* 355 */	"\tcmpb.le.16\t%0,%1,%a\n",	/* stmt: LEI2(reg,reg) */
/* 356 */	"\tcmpb.le.16\t%0,%1,%a\n",	/* stmt: LEI2(reg,con) */
/* 357 */	"\tcmpb.le.16\t%0,%1,%a\n",	/* stmt: LEI2(reg,dref16) */
/* 358 */	"\tcmp.16\t%0,%1\n\tbr.leu\t%a\n",	/* stmt: LEU2(reg,reg) */
/* 359 */	"\tcmp.16\t%0,%1\n\tbr.leu\t%a\n",	/* stmt: LEU2(reg,con) */
/* 360 */	"\tcmp.16\t%0,%1\n\tbr.leu\t%a\n",	/* stmt: LEU2(reg,dref16) */
/* 361 */	"\tcmpb.le.8\t%0,%1,%a\n",	/* stmt: LEI2(reg8,reg8) */
/* 362 */	"\tcmpb.le.8\t%0,%1,%a\n",	/* stmt: LEI2(CVII2(reg8),CVII2(reg8)) */
/* 363 */	"\tcmpb.le.8\t%0,%1,%a\n",	/* stmt: LEI2(reg8,con) */
/* 364 */	"\tcmpb.le.8\t%0,%1,%a\n",	/* stmt: LEI2(CVII2(reg8),con) */
/* 365 */	"\tcmpb.le.8\t%0,%1,%a\n",	/* stmt: LEI2(reg8,CVII2(dref8)) */
/* 366 */	"\tcmpb.le.8\t%0,%1,%a\n",	/* stmt: LEI2(CVII2(reg8),CVII2(dref8)) */
/* 367 */	"\tcmpb.le.8\t%0,%1,%a\n",	/* stmt: LEI2(reg8,CVII2(reg8)) */
/* 368 */	"\tcmp.8\t%0,%1\n\tbr.leu\t%a\n",	/* stmt: LEU2(reg8,reg8) */
/* 369 */	"\tcmp.8\t%0,%1\n\tbr.leu\t%a\n",	/* stmt: LEU2(reg8,con) */
/* 370 */	"\tcmp.8\t%0,%1\n\tbr.leu\t%a\n",	/* stmt: LEU2(reg8,CVUU2(dref8)) */
/* 371 */	"\tcmp.8\t%0,%1\n\tbr.leu\t%a\n",	/* stmt: LEU2(CVUU2(reg8),CVUU2(dref8)) */
/* 372 */	"\tcmp.8\t%0,%1\n\tbr.leu\t%a\n",	/* stmt: LEU2(reg8,CVUU2(reg8)) */
/* 373 */	"\tCMPBLE32(%0,%1,%a)\n",	/* stmt: LEI4(reg32,reg32) */
/* 374 */	"\tCMPBLE32(%0,%1,%a)\n",	/* stmt: LEI4(reg32,INDIRI4(faddr)) */
/* 375 */	"\tCMPBLE32(%0,%1,%a)\n",	/* stmt: LEI4(mem,mem) */
/* 376 */	"\tCMPBLEU32(%0,%1,%a)\n",	/* stmt: LEU4(reg32,reg32) */
/* 377 */	"\tCMPBLEU32(%0,%1,%a)\n",	/* stmt: LEU4(reg32,INDIRU4(faddr)) */
/* 378 */	"\tCMPBLEU32(%0,%1,%a)\n",	/* stmt: LEU4(mem,mem) */
/* 379 */	"\tCMPBLEF32(%0,%1,%a)\n",	/* stmt: LEF4(regf64,regf64) */
/* 380 */	"\tCMPBLEF32(%0,%1,%a)\n",	/* stmt: LEF4(regf64,INDIRF4(faddr)) */
/* 381 */	"\tCMPBLEF32(%0,%1,%a)\n",	/* stmt: LEF4(mem,mem) */
/* 382 */	"\tCMPBLEF64(%0,%1,%a)\n",	/* stmt: LEF8(regf64,regf64) */
/* 383 */	"\tCMPBLEF64(%0,%1,%a)\n",	/* stmt: LEF8(regf64,INDIRF8(faddr)) */
/* 384 */	"\tCMPBLEF64(%0,%1,%a)\n",	/* stmt: LEF8(mem,mem) */
/* 385 */	"\tcmpb.lt.16\t%1,%0,%a\n",	/* stmt: GTI2(reg,reg) */
/* 386 */	"\tcmpb.lt.16\t%1,%0,%a\n",	/* stmt: GTI2(con,reg) */
/* 387 */	"\tcmpb.lt.16\t%1,%0,%a\n",	/* stmt: GTI2(dref16,reg) */
/* 388 */	"\tcmp.16\t%1,%0\n\tbr.ltu\t%a\n",	/* stmt: GTU2(reg,reg) */
/* 389 */	"\tcmp.16\t%1,%0\n\tbr.ltu\t%a\n",	/* stmt: GTU2(con,reg) */
/* 390 */	"\tcmp.16\t%1,%0\n\tbr.ltu\t%a\n",	/* stmt: GTU2(dref16,reg) */
/* 391 */	"\tcmpb.lt.8\t%1,%0,%a\n",	/* stmt: GTI2(reg8,reg8) */
/* 392 */	"\tcmpb.lt.8\t%1,%0,%a\n",	/* stmt: GTI2(CVII2(reg8),CVII2(reg8)) */
/* 393 */	"\tcmpb.lt.8\t%1,%0,%a\n",	/* stmt: GTI2(con,reg8) */
/* 394 */	"\tcmpb.lt.8\t%1,%0,%a\n",	/* stmt: GTI2(con,CVII2(reg8)) */
/* 395 */	"\tcmpb.lt.8\t%1,%0,%a\n",	/* stmt: GTI2(CVII2(dref8),reg8) */
/* 396 */	"\tcmpb.lt.8\t%1,%0,%a\n",	/* stmt: GTI2(CVII2(dref8),CVII2(reg8)) */
/* 397 */	"\tcmpb.lt.8\t%1,%0,%a\n",	/* stmt: GTI2(CVII2(reg8),reg8) */
/* 398 */	"\tcmp.8\t%1,%0\n\tbr.ltu\t%a\n",	/* stmt: GTU2(reg8,reg8) */
/* 399 */	"\tcmp.8\t%1,%0\n\tbr.ltu\t%a\n",	/* stmt: GTU2(con,reg8) */
/* 400 */	"\tcmp.8\t%1,%0\n\tbr.ltu\t%a\n",	/* stmt: GTU2(CVUU2(dref8),reg8) */
/* 401 */	"\tcmp.8\t%1,%0\n\tbr.ltu\t%a\n",	/* stmt: GTU2(CVUU2(dref8),CVUU2(reg8)) */
/* 402 */	"\tcmp.8\t%1,%0\n\tbr.ltu\t%a\n",	/* stmt: GTU2(CVUU2(reg8),reg8) */
/* 403 */	"\tCMPBLT32(%1,%0,%a)\n",	/* stmt: GTI4(reg32,reg32) */
/* 404 */	"\tCMPBLT32(%1,%0,%a)\n",	/* stmt: GTI4(INDIRI4(faddr),reg32) */
/* 405 */	"\tCMPBLT32(%1,%0,%a)\n",	/* stmt: GTI4(mem,mem) */
/* 406 */	"\tCMPBLTU32(%1,%0,%a)\n",	/* stmt: GTU4(reg32,reg32) */
/* 407 */	"\tCMPBLTU32(%1,%0,%a)\n",	/* stmt: GTU4(INDIRU4(faddr),reg32) */
/* 408 */	"\tCMPBLTU32(%1,%0,%a)\n",	/* stmt: GTU4(mem,mem) */
/* 409 */	"\tCMPBLTF32(%1,%0,%a)\n",	/* stmt: GTF4(regf64,regf64) */
/* 410 */	"\tCMPBLTF32(%1,%0,%a)\n",	/* stmt: GTF4(INDIRF4(faddr),regf64) */
/* 411 */	"\tCMPBLTF32(%1,%0,%a)\n",	/* stmt: GTF4(mem,mem) */
/* 412 */	"\tCMPBLTF64(%1,%0,%a)\n",	/* stmt: GTF8(regf64,regf64) */
/* 413 */	"\tCMPBLTF64(%1,%0,%a)\n",	/* stmt: GTF8(INDIRF8(faddr),regf64) */
/* 414 */	"\tCMPBLTF64(%1,%0,%a)\n",	/* stmt: GTF8(mem,mem) */
/* 415 */	"\tcmpb.le.16\t%1,%0,%a\n",	/* stmt: GEI2(reg,reg) */
/* 416 */	"\tcmpb.le.16\t%1,%0,%a\n",	/* stmt: GEI2(con,reg) */
/* 417 */	"\tcmpb.le.16\t%1,%0,%a\n",	/* stmt: GEI2(dref16,reg) */
/* 418 */	"\tcmp.16\t%1,%0\n\tbr.leu\t%a\n",	/* stmt: GEU2(reg,reg) */
/* 419 */	"\tcmp.16\t%1,%0\n\tbr.leu\t%a\n",	/* stmt: GEU2(con,reg) */
/* 420 */	"\tcmp.16\t%1,%0\n\tbr.leu\t%a\n",	/* stmt: GEU2(dref16,reg) */
/* 421 */	"\tcmpb.le.8\t%1,%0,%a\n",	/* stmt: GEI2(reg8,reg8) */
/* 422 */	"\tcmpb.le.8\t%1,%0,%a\n",	/* stmt: GEI2(CVII2(reg8),CVII2(reg8)) */
/* 423 */	"\tcmpb.le.8\t%1,%0,%a\n",	/* stmt: GEI2(con,reg8) */
/* 424 */	"\tcmpb.le.8\t%1,%0,%a\n",	/* stmt: GEI2(con,CVII2(reg8)) */
/* 425 */	"\tcmpb.le.8\t%1,%0,%a\n",	/* stmt: GEI2(CVII2(dref8),reg8) */
/* 426 */	"\tcmpb.le.8\t%1,%0,%a\n",	/* stmt: GEI2(CVII2(dref8),CVII2(reg8)) */
/* 427 */	"\tcmpb.le.8\t%1,%0,%a\n",	/* stmt: GEI2(CVII2(reg8),reg8) */
/* 428 */	"\tcmp.8\t%1,%0\n\tbr.leu\t%a\n",	/* stmt: GEU2(reg8,reg8) */
/* 429 */	"\tcmp.8\t%1,%0\n\tbr.leu\t%a\n",	/* stmt: GEU2(con,reg8) */
/* 430 */	"\tcmp.8\t%1,%0\n\tbr.leu\t%a\n",	/* stmt: GEU2(CVUU2(dref8),reg8) */
/* 431 */	"\tcmp.8\t%1,%0\n\tbr.leu\t%a\n",	/* stmt: GEU2(CVUU2(dref8),CVUU2(reg8)) */
/* 432 */	"\tcmp.8\t%1,%0\n\tbr.leu\t%a\n",	/* stmt: GEU2(CVUU2(reg8),reg8) */
/* 433 */	"\tCMPBLE32(%1,%0,%a)\n",	/* stmt: GEI4(reg32,reg32) */
/* 434 */	"\tCMPBLE32(%1,%0,%a)\n",	/* stmt: GEI4(INDIRI4(faddr),reg32) */
/* 435 */	"\tCMPBLE32(%1,%0,%a)\n",	/* stmt: GEI4(mem,mem) */
/* 436 */	"\tCMPBLEU32(%1,%0,%a)\n",	/* stmt: GEU4(reg32,reg32) */
/* 437 */	"\tCMPBLEU32(%1,%0,%a)\n",	/* stmt: GEU4(INDIRU4(faddr),reg32) */
/* 438 */	"\tCMPBLEU32(%1,%0,%a)\n",	/* stmt: GEU4(mem,mem) */
/* 439 */	"\tCMPBLEF32(%1,%0,%a)\n",	/* stmt: GEF4(regf64,regf64) */
/* 440 */	"\tCMPBLEF32(%1,%0,%a)\n",	/* stmt: GEF4(INDIRF4(faddr),regf64) */
/* 441 */	"\tCMPBLEF32(%1,%0,%a)\n",	/* stmt: GEF4(mem,mem) */
/* 442 */	"\tCMPBLEF64(%1,%0,%a)\n",	/* stmt: GEF8(regf64,regf64) */
/* 443 */	"\tCMPBLEF64(%1,%0,%a)\n",	/* stmt: GEF8(INDIRF8(faddr),regf64) */
/* 444 */	"\tCMPBLEF64(%1,%0,%a)\n",	/* stmt: GEF8(mem,mem) */
/* 445 */	"%a-$global$(dp)",	/* faddr: ADDRGP2 */
/* 446 */	"%a(dp)",	/* faddr: ADDRGP2 */
/* 447 */	"%a+%F(sp)",	/* faddr: ADDRLP2 */
/* 448 */	"%a+4+%F(sp)",	/* faddr: ADDRFP2 */
/* 449 */	"%1(%0)",	/* vaddr: ADDP2(reg,con) */
/* 450 */	"%0",	/* dref16: INDIRP2(addr) */
/* 451 */	"%0",	/* dref16: INDIRI2(addr) */
/* 452 */	"%0",	/* dref16: INDIRU2(addr) */
/* 453 */	"%0",	/* dref16: LOADU2(dref16) */
/* 454 */	"%0",	/* dref16: LOADI2(dref16) */
/* 455 */	"%0",	/* dref16: LOADP2(dref16) */
/* 456 */	"0(%0)",	/* dref16: INDIRP2(reg) */
/* 457 */	"0(%0)",	/* dref16: INDIRI2(reg) */
/* 458 */	"0(%0)",	/* dref16: INDIRU2(reg) */
/* 459 */	"%0",	/* dref8: INDIRI1(addr) */
/* 460 */	"%0",	/* dref8: INDIRU1(addr) */
/* 461 */	"0(%0)",	/* dref8: INDIRI1(reg) */
/* 462 */	"0(%0)",	/* dref8: INDIRU1(reg) */
/* 463 */	"%0",	/* addr: faddr */
/* 464 */	"%0",	/* addr: vaddr */
/* 465 */	"",	/* stmt: reg */
/* 466 */	"%a:\n",	/* stmt: LABELV */
/* 467 */	"\tld.16\t%c,%a\n",	/* reg: ADDRGP2 */
/* 468 */	"\tlea\t%c,%a+%F(sp)\n",	/* reg: ADDRLP2 */
/* 469 */	"\tlea\t%c,%a+4+%F(sp)\n",	/* reg: ADDRFP2 */
/* 470 */	"\tld.8\t%c,0(%0)\n",	/* reg8: INDIRI1(reg) */
/* 471 */	"\tld.8\t%c,0(%0)\n",	/* reg: INDIRI1(reg) */
/* 472 */	"\tld.8\t%c,%0\n",	/* reg8: INDIRI1(addr) */
/* 473 */	"\tld.8\t%c,%0\n",	/* reg: INDIRI1(addr) */
/* 474 */	"\tld.8\t%c,0(%0)\n",	/* reg8: INDIRU1(reg) */
/* 475 */	"\tld.8\t%c,0(%0)\n",	/* reg: INDIRU1(reg) */
/* 476 */	"\tld.8\t%c,%0\n",	/* reg8: INDIRU1(addr) */
/* 477 */	"\tld.8\t%c,%0\n",	/* reg: INDIRU1(addr) */
/* 478 */	"\tld.16\t%c,0(%0)\n",	/* reg: INDIRI2(reg) */
/* 479 */	"\tld.16\t%c,%0\n",	/* reg: INDIRI2(addr) */
/* 480 */	"\tld.16\t%c,0(%0)\n",	/* reg: INDIRU2(reg) */
/* 481 */	"\tld.16\t%c,%0\n",	/* reg: INDIRU2(addr) */
/* 482 */	"\tld.16\t%c,0(%0)\n",	/* reg: INDIRP2(reg) */
/* 483 */	"\tld.16\t%c,%0\n",	/* reg: INDIRP2(addr) */
/* 484 */	"\tCOPY32(%c,0(%0))\n",	/* reg32: INDIRI4(reg) */
/* 485 */	"\tCOPY32(%c,%0)\n",	/* reg32: INDIRI4(faddr) */
/* 486 */	"\tCOPY32(%c,0(%0))\n",	/* reg32: INDIRU4(reg) */
/* 487 */	"\tCOPY32(%c,%0)\n",	/* reg32: INDIRU4(faddr) */
/* 488 */	"\tCOPY32(%c,0(%0))\n",	/* regf64: INDIRF4(reg) */
/* 489 */	"\tCOPY32(%c,%0)\n",	/* regf64: INDIRF4(faddr) */
/* 490 */	"\tCOPY64(%c,0(%0))\n",	/* regf64: INDIRF8(reg) */
/* 491 */	"\tCOPY64(%c,%0)\n",	/* regf64: INDIRF8(faddr) */
/* 492 */	"%0",	/* ar: reg */
/* 493 */	"%a",	/* ar: ADDRGP2 */
/* 494 */	"\tcall\t%0\n",	/* reg: CALLI2(ar) */
/* 495 */	"\tcall\t%0\n",	/* reg: CALLU2(ar) */
/* 496 */	"\tcall\t%0\n",	/* reg: CALLP2(ar) */
/* 497 */	"\tcall\t%0\n\tst.16\t2+%c,a\n\tst.16\t%c,b\n",	/* reg32: CALLI4(ar) */
/* 498 */	"\tcall\t%0\n",	/* reg: CALLI4(ar) */
/* 499 */	"\tcall\t%0\n\tst.16\t2+%c,a\n\tst.16\t%c,b\n",	/* reg32: CALLU4(ar) */
/* 500 */	"\tcall\t%0\n",	/* reg: CALLU4(ar) */
/* 501 */	"\tcall\t%0\n",	/* stmt: CALLV(ar) */
/* 502 */	"\tcall\t%0\n",	/* stmt: CALLB(ar,reg) */
/* 503 */	"\tcall\t%0\n\tst.16\t2+%c,a\n\tst.16\t%c,b\n",	/* regf64: CALLF4(ar) */
/* 504 */	"\tcall\t%0 ; bogus? \n",	/* reg: CALLF4(ar) */
/* 505 */	"\tlea\ta,%c\n\tcall\t%0\n",	/* regf64: CALLF8(ar) */
/* 506 */	"\tlea\ta,%c\n\tcall\t%0\n",	/* stmt: CALLF8(ar) */
/* 507 */	"# rtarget should have handled it\n",	/* stmt: RETI2(reg) */
/* 508 */	"# rtarget should have handled it\n",	/* stmt: RETU2(reg) */
/* 509 */	"# rtarget should have handled it\n",	/* stmt: RETP2(reg) */
/* 510 */	"# Let emit2 fix up\n",	/* stmt: RETI4(reg32) */
/* 511 */	"# Let emit2 fix up\n",	/* stmt: RETU4(reg32) */
/* 512 */	"# Let emit2 fix up\n",	/* stmt: RETF4(regf64) */
/* 513 */	"# Let emit2 fix up\n",	/* stmt: RETF8(regf64) */
/* 514 */	"# ret\n",	/* stmt: RETV(reg) */
/* 515 */	"# Let emit2 handle args\n",	/* stmt: ARGP2(reg) */
/* 516 */	"# Let emit2 handle args\n",	/* stmt: ARGI2(reg) */
/* 517 */	"# Let emit2 handle args\n",	/* stmt: ARGU2(reg) */
/* 518 */	"# Let emit2 handle args\n",	/* stmt: ARGI4(reg32) */
/* 519 */	"# Let emit2 handle args\n",	/* stmt: ARGU4(reg32) */
/* 520 */	"# Let emit2 handle args\n",	/* stmt: ARGF4(regf64) */
/* 521 */	"# Let emit2 handle args\n",	/* stmt: ARGF8(regf64) */
/* 522 */	"\tbr\t%0\n",	/* stmt: JUMPV(ar) */
/* 523 */	"%0",	/* mem: INDIRI4(faddr) */
/* 524 */	"%0",	/* mem: INDIRU4(faddr) */
/* 525 */	"%0",	/* mem: INDIRF4(faddr) */
/* 526 */	"%0",	/* mem: INDIRF8(faddr) */
/* 527 */	"%0",	/* mem: reg32 */
/* 528 */	"%0",	/* mem: regf64 */
/* 529 */	"\tst.8\t0(%0),%1\n",	/* stmt: ASGNI1(reg,reg8) */
/* 530 */	"\tst.8\t%0,%1\n",	/* stmt: ASGNI1(addr,reg8) */
/* 531 */	"\tst.8\t0(%0),%1\n",	/* stmt: ASGNU1(reg,reg8) */
/* 532 */	"\tst.8\t%0,%1\n",	/* stmt: ASGNU1(addr,reg8) */
/* 533 */	"\tst.16\t0(%0),%1\n",	/* stmt: ASGNI2(reg,reg) */
/* 534 */	"\tst.16\t%0,%1\n",	/* stmt: ASGNI2(addr,reg) */
/* 535 */	"\tst.16\t0(%0),%1\n",	/* stmt: ASGNU2(reg,reg) */
/* 536 */	"\tst.16\t%0,%1\n",	/* stmt: ASGNU2(addr,reg) */
/* 537 */	"\tst.16\t0(%0),%1\n",	/* stmt: ASGNP2(reg,reg) */
/* 538 */	"\tst.16\t%0,%1\n",	/* stmt: ASGNP2(addr,reg) */
/* 539 */	"\tld.16\tc,%a\n\tmemcopy\n",	/* stmt: ASGNB(reg,INDIRB(reg)) */
/* 540 */	"# let emit2 handle %0\n",	/* stmt: ARGB(INDIRB(reg)) */
/* 541 */	"\tCOPY32(0(%0),%1)\n",	/* stmt: ASGNI4(reg,reg32) */
/* 542 */	"\tCOPY32(%0,%1)\n",	/* stmt: ASGNI4(faddr,reg32) */
/* 543 */	"\tCOPY32(%0,%1)\n",	/* stmt: ASGNI4(faddr,INDIRI4(faddr)) */
/* 544 */	"\tCOPY32(0(%0),%1)\n",	/* stmt: ASGNU4(reg,reg32) */
/* 545 */	"\tCOPY32(%0,%1)\n",	/* stmt: ASGNU4(faddr,reg32) */
/* 546 */	"\tCOPY32(%0,%1)\n",	/* stmt: ASGNU4(faddr,INDIRU4(faddr)) */
/* 547 */	"\tCOPY32(0(%0),%1)\n",	/* stmt: ASGNF4(reg,regf64) */
/* 548 */	"\tCOPY32(%0,%1)\n",	/* stmt: ASGNF4(faddr,regf64) */
/* 549 */	"\tCOPY32(%0,%1)\n",	/* stmt: ASGNF4(faddr,INDIRF4(faddr)) */
/* 550 */	"\tCOPY64(0(%0),%1)\n",	/* stmt: ASGNF8(reg,regf64) */
/* 551 */	"\tCOPY64(%0,%1)\n",	/* stmt: ASGNF8(faddr,regf64) */
/* 552 */	"\tCOPY64(%0,%1)\n",	/* stmt: ASGNF8(faddr,INDIRF8(faddr)) */
/* 553 */	"\tADD32(%1,%2)\n",	/* stmt: ASGNI4(faddr,ADDI4(mem,mem)) */
/* 554 */	"\tADD32(%2,%1)\n",	/* stmt: ASGNI4(faddr,ADDI4(mem,mem)) */
/* 555 */	"\tADD32(%1,%2)\n",	/* stmt: ASGNU4(faddr,ADDI4(mem,mem)) */
/* 556 */	"\tADD32(%2,%1)\n",	/* stmt: ASGNU4(faddr,ADDI4(mem,mem)) */
/* 557 */	"\tFADD32(%1,%2)\n",	/* stmt: ASGNF4(faddr,ADDF4(mem,mem)) */
/* 558 */	"\tFADD32(%2,%1)\n",	/* stmt: ASGNF4(faddr,ADDF4(mem,mem)) */
/* 559 */	"\tFADD64(%1,%2)\n",	/* stmt: ASGNF8(faddr,ADDF8(mem,mem)) */
/* 560 */	"\tFADD64(%2,%1)\n",	/* stmt: ASGNF8(faddr,ADDF8(mem,mem)) */
/* 561 */	"\tCOPY32(%0,%1)\n\tADD32(%0,%2)\n",	/* stmt: ASGNI4(faddr,ADDI4(mem,mem)) */
/* 562 */	"\tCOPY32(%0,%1)\n\tADD32(%0,%2)\n",	/* stmt: ASGNU4(faddr,ADDU4(mem,mem)) */
/* 563 */	"\tCOPY32(%0,%1)\n\tFADD32(%0,%2)\n",	/* stmt: ASGNF4(faddr,ADDF4(mem,mem)) */
/* 564 */	"\tCOPY64(%0,%1)\n\tFADD64(%0,%2)\n",	/* stmt: ASGNF8(faddr,ADDF8(mem,mem)) */
/* 565 */	"\tSUB32(%1,%2)\n",	/* stmt: ASGNI4(faddr,SUBI4(mem,mem)) */
/* 566 */	"\tSUB32(%1,%2)\n",	/* stmt: ASGNU4(faddr,SUBI4(mem,mem)) */
/* 567 */	"\tFSUB32(%1,%2)\n",	/* stmt: ASGNF4(faddr,SUBF4(mem,mem)) */
/* 568 */	"\tFSUB64(%1,%2)\n",	/* stmt: ASGNF8(faddr,SUBF8(mem,mem)) */
/* 569 */	"\tCOPY32(%0,%1)\n\tSUB32(%0,%2)\n",	/* stmt: ASGNI4(faddr,SUBI4(mem,mem)) */
/* 570 */	"\tCOPY32(%0,%1)\n\tSUB32(%0,%2)\n",	/* stmt: ASGNU4(faddr,SUBU4(mem,mem)) */
/* 571 */	"\tCOPY32(%0,%1)\n\tFSUB32(%0,%2)\n",	/* stmt: ASGNF4(faddr,SUBF4(mem,mem)) */
/* 572 */	"\tCOPY64(%0,%1)\n\tFSUB64(%0,%2)\n",	/* stmt: ASGNF8(faddr,SUBF8(mem,mem)) */
/* 573 */	"\tMULI32(%1,%2)\n",	/* stmt: ASGNI4(faddr,MULI4(mem,mem)) */
/* 574 */	"\tMULI32(%2,%1)\n",	/* stmt: ASGNI4(faddr,MULI4(mem,mem)) */
/* 575 */	"\tMULU32(%1,%2)\n",	/* stmt: ASGNU4(faddr,MULI4(mem,mem)) */
/* 576 */	"\tMULU32(%2,%1)\n",	/* stmt: ASGNU4(faddr,MULI4(mem,mem)) */
/* 577 */	"\tFMUL32(%1,%2)\n",	/* stmt: ASGNF4(faddr,MULF4(mem,mem)) */
/* 578 */	"\tFMUL32(%2,%1)\n",	/* stmt: ASGNF4(faddr,MULF4(mem,mem)) */
/* 579 */	"\tFMUL64(%1,%2)\n",	/* stmt: ASGNF8(faddr,MULF8(mem,mem)) */
/* 580 */	"\tFMUL64(%2,%1)\n",	/* stmt: ASGNF8(faddr,MULF8(mem,mem)) */
/* 581 */	"\tCOPY32(%0,%1)\n\tMULI32(%0,%2)\n",	/* stmt: ASGNI4(faddr,MULI4(mem,mem)) */
/* 582 */	"\tCOPY32(%0,%1)\n\tMULU32(%0,%2)\n",	/* stmt: ASGNU4(faddr,MULU4(mem,mem)) */
/* 583 */	"\tCOPY32(%0,%1)\n\tFMUL32(%0,%2)\n",	/* stmt: ASGNF4(faddr,MULF4(mem,mem)) */
/* 584 */	"\tCOPY64(%0,%1)\n\tFMUL64(%0,%2)\n",	/* stmt: ASGNF8(faddr,MULF8(mem,mem)) */
/* 585 */	"\tDIVI32(%1,%2)\n",	/* stmt: ASGNI4(faddr,DIVI4(mem,mem)) */
/* 586 */	"\tDIVU32(%1,%2)\n",	/* stmt: ASGNU4(faddr,DIVI4(mem,mem)) */
/* 587 */	"\tFDIV32(%1,%2)\n",	/* stmt: ASGNF4(faddr,DIVF4(mem,mem)) */
/* 588 */	"\tFDIV64(%1,%2)\n",	/* stmt: ASGNF8(faddr,DIVF8(mem,mem)) */
/* 589 */	"\tCOPY32(%0,%1)\n\tDIVI32(%0,%2)\n",	/* stmt: ASGNI4(faddr,DIVI4(mem,mem)) */
/* 590 */	"\tCOPY32(%0,%1)\n\tDIVU32(%0,%2)\n",	/* stmt: ASGNU4(faddr,DIVU4(mem,mem)) */
/* 591 */	"\tCOPY32(%0,%1)\n\tFDIV32(%0,%2)\n",	/* stmt: ASGNF4(faddr,DIVF4(mem,mem)) */
/* 592 */	"\tCOPY64(%0,%1)\n\tFDIV64(%0,%2)\n",	/* stmt: ASGNF8(faddr,DIVF8(mem,mem)) */
/* 593 */	"\tAND32(%1,%2)\n",	/* stmt: ASGNI4(faddr,BANDI4(mem,mem)) */
/* 594 */	"\tAND32(%2,%1)\n",	/* stmt: ASGNI4(faddr,BANDI4(mem,mem)) */
/* 595 */	"\tAND32(%1,%2)\n",	/* stmt: ASGNU4(faddr,BANDU4(mem,mem)) */
/* 596 */	"\tAND32(%2,%1)\n",	/* stmt: ASGNU4(faddr,BANDU4(mem,mem)) */
/* 597 */	"\tCOPY32(%0,%1)\n\tAND32(%0,%2)\n",	/* stmt: ASGNI4(faddr,BANDI4(mem,mem)) */
/* 598 */	"\tCOPY32(%0,%1)\n\tAND32(%0,%2)\n",	/* stmt: ASGNI4(faddr,BANDU4(mem,mem)) */
/* 599 */	"\tOR32(%1,%2)\n",	/* stmt: ASGNI4(faddr,BORI4(mem,mem)) */
/* 600 */	"\tOR32(%2,%1)\n",	/* stmt: ASGNI4(faddr,BORI4(mem,mem)) */
/* 601 */	"\tOR32(%1,%2)\n",	/* stmt: ASGNU4(faddr,BORU4(mem,mem)) */
/* 602 */	"\tOR32(%2,%1)\n",	/* stmt: ASGNU4(faddr,BORU4(mem,mem)) */
/* 603 */	"\tCOPY32(%0,%1)\n\tOR32(%0,%2)\n",	/* stmt: ASGNI4(faddr,BORI4(mem,mem)) */
/* 604 */	"\tCOPY32(%0,%1)\n\tOR32(%0,%2)\n",	/* stmt: ASGNI4(faddr,BORU4(mem,mem)) */
/* 605 */	"\tXOR32(%1,%2)\n",	/* stmt: ASGNI4(faddr,BXORI4(mem,mem)) */
/* 606 */	"\tXOR32(%2,%1)\n",	/* stmt: ASGNI4(faddr,BXORI4(mem,mem)) */
/* 607 */	"\tXOR32(%1,%2)\n",	/* stmt: ASGNU4(faddr,BXORU4(mem,mem)) */
/* 608 */	"\tXOR32(%2,%1)\n",	/* stmt: ASGNU4(faddr,BXORU4(mem,mem)) */
/* 609 */	"\tCOPY32(%0,%1)\n\tXOR32(%0,%2)\n",	/* stmt: ASGNI4(faddr,BXORI4(mem,mem)) */
/* 610 */	"\tCOPY32(%0,%1)\n\tXOR32(%0,%2)\n",	/* stmt: ASGNI4(faddr,BXORU4(mem,mem)) */
};

static char _isinstruction[] = {
/* 0 */	0,
/* 1 */	1,	/* \tcopy\t%c,%0\n */
/* 2 */	1,	/* \tcopy\t%c,%0\n */
/* 3 */	1,	/* \tld.8\t%c,3+%0\n */
/* 4 */	1,	/* \tcopy\t%c,%0\n */
/* 5 */	1,	/* \tcopy\t%c,%0\n */
/* 6 */	1,	/* \tld.8\t%c,3+%0\n */
/* 7 */	1,	/* \tcopy\t%c,%0\n */
/* 8 */	1,	/* \tld.16\t%c,2+%0\n */
/* 9 */	1,	/* \tcopy\t%c,%0\n */
/* 10 */	1,	/* \tld.16\t%c,2+%0\n */
/* 11 */	1,	/* \tcopy\t%c,%0\n */
/* 12 */	1,	/* \tCOPY32(%c,%0)\n */
/* 13 */	1,	/* \tCOPY32(%c,%0)\n */
/* 14 */	1,	/* \tCOPY32(%c,%0)\n */
/* 15 */	1,	/* \tCOPY64(%c,%0)\n */
/* 16 */	1,	/* # read register\n */
/* 17 */	1,	/* # read register\n */
/* 18 */	1,	/* # read register\n */
/* 19 */	1,	/* # read register\n */
/* 20 */	1,	/* # read register\n */
/* 21 */	1,	/* # read register\n */
/* 22 */	1,	/* # read register\n */
/* 23 */	1,	/* # read register\n */
/* 24 */	1,	/* # read register\n */
/* 25 */	1,	/* # write register\n */
/* 26 */	1,	/* # write register\n */
/* 27 */	1,	/* # write register\n */
/* 28 */	1,	/* # write register\n */
/* 29 */	1,	/* # write register\n */
/* 30 */	1,	/* # write register\n */
/* 31 */	1,	/* # write register\n */
/* 32 */	1,	/* # write register\n */
/* 33 */	1,	/* # write register\n */
/* 34 */	1,	/* \tCVTF8F4(%c,%0)\n */
/* 35 */	1,	/* \tCVTF4F8(%c,%0)\n */
/* 36 */	1,	/* \tCVTI4F4(%c,%0)\n */
/* 37 */	1,	/* \tCVTI2F4(%c,%0)\n */
/* 38 */	1,	/* \tCVTI4F8(%c,%0)\n */
/* 39 */	1,	/* \tCVTI2F8(%c,%0)\n */
/* 40 */	1,	/* \tCVTF8I4(%c,%0)\n */
/* 41 */	1,	/* \tCVTF4I4(%c,%0)\n */
/* 42 */	1,	/* \tCVTF8I2(%c,%0)\n */
/* 43 */	1,	/* \tCVTF4I2(%c,%0)\n */
/* 44 */	1,	/* \tld.8\t%c,3+%0\n */
/* 45 */	1,	/* \tcopy\t%c,%0\n */
/* 46 */	1,	/* \tld.8\t%c,3+%0\n */
/* 47 */	1,	/* \tcopy\t%c,%0\n */
/* 48 */	1,	/* ?\tcopy\t%c,%0\n */
/* 49 */	1,	/* ?\tcopy\t%c,%0\n */
/* 50 */	1,	/* ?\tcopy\t%c,%0\n */
/* 51 */	1,	/* ?\tcopy\t%c,%0\n */
/* 52 */	1,	/* ?\tcopy\t%c,%0\n\tsex\t%c\n */
/* 53 */	1,	/* \tld.16\t%c,2+%0\n */
/* 54 */	1,	/* ?\tcopy\t%c,%0\n\tand.16\ta,0xff\n */
/* 55 */	1,	/* \tld.16\t%c,2+%0\n */
/* 56 */	1,	/* \tld.8\t%c,3+%0\n */
/* 57 */	1,	/* \tcopy\t%c,%0\n */
/* 58 */	1,	/* \tld.8\t%c,3+%0\n */
/* 59 */	1,	/* \tcopy\t%c,%0\n */
/* 60 */	1,	/* ?\tcopy\t%c,%0\n\tsex\t%c,0xff\n */
/* 61 */	1,	/* \tld.16\t%c,2+%0\n */
/* 62 */	1,	/* ?\tcopy\t%c,%0\n\tand.16\t%c,0xff\n */
/* 63 */	1,	/* \tld.16\t%c,2+%0\n */
/* 64 */	1,	/* \tCVTUU4(%c,%0)\n */
/* 65 */	1,	/* \tCVTUU4(%c,%0)\n */
/* 66 */	1,	/* \tCVTUU4(%c,%0)\n */
/* 67 */	1,	/* \tCVTII4(%c,%0)\n */
/* 68 */	1,	/* \tld.16\t%c,%0\n */
/* 69 */	1,	/* \tld.8\t%c,%0\n */
/* 70 */	0,	/* 0 */
/* 71 */	0,	/* 1 */
/* 72 */	0,	/* 2 */
/* 73 */	0,	/* 0 */
/* 74 */	0,	/* 1 */
/* 75 */	0,	/* 2 */
/* 76 */	0,	/* %a */
/* 77 */	0,	/* %a */
/* 78 */	0,	/* %a */
/* 79 */	0,	/* %a */
/* 80 */	0,	/* %a */
/* 81 */	0,	/* %a */
/* 82 */	0,	/* %a */
/* 83 */	0,	/* %0 */
/* 84 */	0,	/* %0 */
/* 85 */	0,	/* %0 */
/* 86 */	0,	/* %0 */
/* 87 */	0,	/* %0 */
/* 88 */	0,	/* %0 */
/* 89 */	1,	/* \tld.8\t%c,%0\n */
/* 90 */	1,	/* \tld.16\t%c,%0\n */
/* 91 */	1,	/* \tld.16\tc,%0\n */
/* 92 */	1,	/* ?\tcopy\t%c,%0\n\tadd.16\t%c,%1\n */
/* 93 */	1,	/* ?\tcopy\t%c,%0\n\tadd.16\t%c,%1\n */
/* 94 */	1,	/* ?\tcopy\t%c,%0\n\tadd.16\t%c,%1\n */
/* 95 */	1,	/* ?\tcopy\t%c,%0\n\tadd.16\t%c,%1\n */
/* 96 */	1,	/* ?\tcopy\t%c,%0\n\tadd.16\t%c,%1\n */
/* 97 */	1,	/* ?\tcopy\t%c,%0\n\tadd.16\t%c,%1\n */
/* 98 */	1,	/* \tlea\t%c,%1(%0)\n */
/* 99 */	1,	/* \tsh0add\t%c,%0,%1\n */
/* 100 */	1,	/* \tsh1add\t%c,%2,%0\n */
/* 101 */	1,	/* \tsh1add\t%c,%2,%0\n */
/* 102 */	1,	/* ?\tcopy\t%c,%0\n\tadd.8\t%c,%1\n */
/* 103 */	1,	/* ?\tcopy\t%c,%0\n\tadd.8\t%c,%1\n */
/* 104 */	1,	/* ?\tcopy\t%c,%0\n\tadd.8\t%c,%1\n */
/* 105 */	1,	/* ?\tcopy\t%c,%0\n\tadd.8\t%c,%1\n */
/* 106 */	1,	/* ?\tcopy\t%c,%0\n\tadd.8\t%c,%1\n */
/* 107 */	1,	/* ?\tcopy\t%c,%0\n\tadd.8\t%c,%1\n */
/* 108 */	1,	/* ?\tcopy\t%c,%0\n\tadd.8\t%c,%1\n */
/* 109 */	1,	/* ?\tcopy\t%c,%0\n\tadd.8\t%c,%1\n */
/* 110 */	1,	/* ?\tcopy\t%c,%0\n\tadd.8\t%c,%1\n */
/* 111 */	1,	/* ?\tCOPY32(%c,%0)\n\tADD32(%c,%1)\n */
/* 112 */	1,	/* ?\tCOPY32(%c,%0)\n\tADD32(%c,%1)\n */
/* 113 */	1,	/* ?\tCOPY32(%c,%0)\n\tADD32(%c,%1)\n */
/* 114 */	1,	/* ?\tCOPY32(%c,%0)\n\tADD32(%c,%1)\n */
/* 115 */	1,	/* ?\tCOPY32(%c,%0)\n\tFADD32(%c,%1)\n */
/* 116 */	1,	/* ?\tCOPY32(%c,%0)\n\tFADD32(%c,%1)\n */
/* 117 */	1,	/* ?\tCOPY64(%c,%0)\n\tFADD64(%c,%1)\n */
/* 118 */	1,	/* ?\tCOPY64(%c,%0)\n\tFADD64(%c,%1)\n */
/* 119 */	1,	/* ?\tcopy\t%c,%0\n\tsub.16\t%c,%1\n */
/* 120 */	1,	/* ?\tcopy\t%c,%0\n\tsub.16\t%c,%1\n */
/* 121 */	1,	/* ?\tcopy\t%c,%0\n\tsub.16\t%c,%1\n */
/* 122 */	1,	/* ?\tcopy\t%c,%0\n\tsub.16\t%c,%1\n */
/* 123 */	1,	/* ?\tcopy\t%c,%0\n\tsub.16\t%c,%1\n */
/* 124 */	1,	/* ?\tcopy\t%c,%0\n\tsub.16\t%c,%1\n */
/* 125 */	1,	/* ?\tcopy\t%c,%0\n\tsub.16\t%c,%1\n */
/* 126 */	1,	/* ?\tcopy\t%c,%0\n\tsub.16\t%c,%1\n */
/* 127 */	1,	/* ?\tcopy\t%c,%0\n\tsub.16\t%c,%1\n */
/* 128 */	1,	/* ?\tcopy\t%c,%0\n\tsub.8\t%c,%1\n */
/* 129 */	1,	/* ?\tcopy\t%c,%0\n\tsub.8\t%c,%1\n */
/* 130 */	1,	/* ?\tcopy\t%c,%0\n\tsub.8\t%c,%1\n */
/* 131 */	1,	/* ?\tcopy\t%c,%0\n\tsub.8\t%c,%1\n */
/* 132 */	1,	/* ?\tcopy\t%c,%0\n\tsub.8\t%c,%1\n */
/* 133 */	1,	/* ?\tcopy\t%c,%0\n\tsub.8\t%c,%1\n */
/* 134 */	1,	/* ?\tCOPY32(%c,%0)\n\tSUB32(%c,%1)\n */
/* 135 */	1,	/* ?\tCOPY32(%c,%0)\n\tSUB32(%c,%1)\n */
/* 136 */	1,	/* ?\tCOPY32(%c,%0)\n\tSUB32(%c,%1)\n */
/* 137 */	1,	/* ?\tCOPY32(%c,%0)\n\tSUB32(%c,%1)\n */
/* 138 */	1,	/* ?\tCOPY32(%c,%0)\n\tFSUB32(%c,%1)\n */
/* 139 */	1,	/* ?\tCOPY32(%c,%0)\n\tFSUB32(%c,%1)\n */
/* 140 */	1,	/* ?\tCOPY64(%c,%0)\n\tFSUB64(%c,%1)\n */
/* 141 */	1,	/* ?\tCOPY64(%c,%0)\n\tFSUB64(%c,%1)\n */
/* 142 */	1,	/* ?\tcopy\t%c,%0\n\tand.16\t%c,%1\n */
/* 143 */	1,	/* ?\tcopy\t%c,%0\n\tand.16\t%c,%1\n */
/* 144 */	1,	/* ?\tcopy\t%c,%0\n\tand.16\t%c,%1\n */
/* 145 */	1,	/* ?\tcopy\t%c,%0\n\tand.16\t%c,%1\n */
/* 146 */	1,	/* ?\tcopy\t%c,%0\n\tand.16\t%c,%1\n */
/* 147 */	1,	/* ?\tcopy\t%c,%0\n\tand.16\t%c,%1\n */
/* 148 */	1,	/* ?\tcopy\t%c,%0\n\tand.8\t%c,%1\n */
/* 149 */	1,	/* ?\tcopy\t%c,%0\n\tand.8\t%c,%1\n */
/* 150 */	1,	/* ?\tcopy\t%c,%0\n\tand.8\t%c,%1\n */
/* 151 */	1,	/* ?\tcopy\t%c,%0\n\tand.8\t%c,%1\n */
/* 152 */	1,	/* ?\tcopy\t%c,%0\n\tand.8\t%c,%1\n */
/* 153 */	1,	/* ?\tcopy\t%c,%0\n\tand.8\t%c,%1\n */
/* 154 */	1,	/* ?\tcopy\t%c,%0\n\tand.8\t%c,%1\n */
/* 155 */	1,	/* ?\tcopy\t%c,%0\n\tand.8\t%c,%1\n */
/* 156 */	1,	/* ?\tcopy\t%c,%0\n\tand.8\t%c,%1\n */
/* 157 */	1,	/* ?\tcopy\t%c,%0\n\tand.8\t%c,%1\n */
/* 158 */	1,	/* ?\tCOPY32(%c,%0)\n\tAND32(%c,%1)\n */
/* 159 */	1,	/* ?\tCOPY32(%c,%0)\n\tAND32(%c,%1)\n */
/* 160 */	1,	/* ?\tCOPY32(%c,%0)\n\tAND32(%c,%1)\n */
/* 161 */	1,	/* ?\tCOPY32(%c,%0)\n\tAND32(%c,%1)\n */
/* 162 */	1,	/* \tld.16\ta,0\n\tst.16\t%c,a\n\tld.16\ta,2+%0\n\tand.16\ta,%1\n\tst.16\t2+%c,a\n */
/* 163 */	1,	/* ld.16\ta,-1\n\txor.16\ta,b\n */
/* 164 */	1,	/* ld.16\ta,-1\n\txor.16\ta,b\n */
/* 165 */	1,	/* ?\tcopy\t%c,%0\n\tor.16\t%c,%1\n */
/* 166 */	1,	/* ?\tcopy\t%c,%0\n\tor.16\t%c,%1\n */
/* 167 */	1,	/* ?\tcopy\t%c,%0\n\tor.16\t%c,%1\n */
/* 168 */	1,	/* ?\tcopy\t%c,%0\n\tor.16\t%c,%1\n */
/* 169 */	1,	/* ?\tcopy\t%c,%0\n\tor.16\t%c,%1\n */
/* 170 */	1,	/* ?\tcopy\t%c,%0\n\tor.16\t%c,%1\n */
/* 171 */	1,	/* ?\tcopy\t%c,%0\n\tor.8\t%c,%1\n */
/* 172 */	1,	/* ?\tcopy\t%c,%0\n\tor.8\t%c,%1\n */
/* 173 */	1,	/* ?\tcopy\t%c,%0\n\tor.8\t%c,%1\n */
/* 174 */	1,	/* ?\tcopy\t%c,%0\n\tor.8\t%c,%1\n */
/* 175 */	1,	/* ?\tcopy\t%c,%0\n\tor.8\t%c,%1\n */
/* 176 */	1,	/* ?\tcopy\t%c,%0\n\tor.8\t%c,%1\n */
/* 177 */	1,	/* ?\tCOPY32(%c,%0)\n\tBCOM32(%c)\n */
/* 178 */	1,	/* ?\tCOPY32(%c,%0)\n\tBCOM32(%c)\n */
/* 179 */	1,	/* ?\tCOPY32(%c,%0)\n\tOR32(%c,%1)\n */
/* 180 */	1,	/* ?\tCOPY32(%c,%0)\n\tOR32(%c,%1)\n */
/* 181 */	1,	/* ?\tCOPY32(%c,%0)\n\tOR32(%c,%1)\n */
/* 182 */	1,	/* ?\tCOPY32(%c,%0)\n\tOR32(%c,%1)\n */
/* 183 */	1,	/* ?\tcopy\t%c,%0\n\txor.16\t%c,%1\n */
/* 184 */	1,	/* ?\tcopy\t%c,%0\n\txor.16\t%c,%1\n */
/* 185 */	1,	/* ?\tCOPY32(%c,%0)\n\tXOR32(%c,%1)\n */
/* 186 */	1,	/* ?\tCOPY32(%c,%0)\n\tXOR32(%c,%1)\n */
/* 187 */	1,	/* ?\tCOPY32(%c,%0)\n\tXOR32(%c,%1)\n */
/* 188 */	1,	/* ?\tCOPY32(%c,%0)\n\tXOR32(%c,%1)\n */
/* 189 */	1,	/* ?\tcopy\t%c,%0\n\tcall\t$muli16\n */
/* 190 */	1,	/* ?\tcopy\t%c,%0\n\tcall\t$mulu16\n */
/* 191 */	1,	/* ?\tCOPY32(%c,%0)\n\tMULI32(%c,%1)\n */
/* 192 */	1,	/* ?\tCOPY32(%c,%0)\n\tMULI32(%c,%1)\n */
/* 193 */	1,	/* ?\tCOPY32(%c,%0)\n\tMULU32(%c,%1)\n */
/* 194 */	1,	/* ?\tCOPY32(%c,%0)\n\tMULU32(%c,%1)\n */
/* 195 */	1,	/* ?\tCOPY32(%c,%0)\n\tFMUL32(%c,%1)\n */
/* 196 */	1,	/* ?\tCOPY32(%c,%0)\n\tFMUL32(%c,%1)\n */
/* 197 */	1,	/* ?\tCOPY64(%c,%0)\n\tFMUL64(%c,%1)\n */
/* 198 */	1,	/* ?\tCOPY64(%c,%0)\n\tFMUL64(%c,%1)\n */
/* 199 */	1,	/* ?\tcopy\t%c,%0\n\tcall\t$divi16\n */
/* 200 */	1,	/* ?\tcopy\t%c,%0\n\tcall\t$divu16\n */
/* 201 */	1,	/* ?\tCOPY32(%c,%0)\n\tDIVI32(%c,%1)\n */
/* 202 */	1,	/* ?\tCOPY32(%c,%0)\n\tDIVI32(%c,%1)\n */
/* 203 */	1,	/* ?\tCOPY32(%c,%0)\n\tDIVU32(%c,%1)\n */
/* 204 */	1,	/* ?\tCOPY32(%c,%0)\n\tDIVU32(%c,%1)\n */
/* 205 */	1,	/* ?\tCOPY32(%c,%0)\n\tFDIV32(%c,%1)\n */
/* 206 */	1,	/* ?\tCOPY32(%c,%0)\n\tFDIV32(%c,%1)\n */
/* 207 */	1,	/* ?\tCOPY64(%c,%0)\n\tFDIV64(%c,%1)\n */
/* 208 */	1,	/* ?\tCOPY64(%c,%0)\n\tFDIV64(%c,%1)\n */
/* 209 */	1,	/* ?\tcopy\t%c,%0\n\tcall\t$modi16\n */
/* 210 */	1,	/* ?\tcopy\t%c,%0\n\tcall\t$modu16\n */
/* 211 */	1,	/* ?\tCOPY32(%c,%0)\n\tMODI32(%c,%1)\n */
/* 212 */	1,	/* ?\tCOPY32(%c,%0)\n\tMODI32(%c,%1)\n */
/* 213 */	1,	/* ?\tCOPY32(%c,%0)\n\tMODU32(%c,%1)\n */
/* 214 */	1,	/* ?\tCOPY32(%c,%0)\n\tMODU32(%c,%1)\n */
/* 215 */	1,	/* \tld.16\t%c,0\n\tsub.16\t%c,%0\n */
/* 216 */	1,	/* \tNEGI32(%c,%0)\n */
/* 217 */	1,	/* \tNEGI32(%c,%0)\n */
/* 218 */	1,	/* \tFNEG32(%c,%0)\n */
/* 219 */	1,	/* \tFNEG32(%c,%0)\n */
/* 220 */	1,	/* \tFNEG64(%c,%0)\n */
/* 221 */	1,	/* \tFNEG64(%c,%0)\n */
/* 222 */	1,	/* ?\tcopy\t%c,%0\n\tshl.16\t%c\n */
/* 223 */	1,	/* ?\tcopy\t%c,%0\n\tshl.16\t%c\n\tshl.16\t%c\n */
/* 224 */	1,	/* ?\tcopy\t%c,%0\n\tvshl.16\t%c\n */
/* 225 */	1,	/* ?\tcopy\t%c,%0\n\tcopy\tc,%1\n\tvshl.16\t%c\n */
/* 226 */	1,	/* ?\tcopy\t%c,%0\n\tshl.16\t%c\n */
/* 227 */	1,	/* ?\tcopy\t%c,%0\n\tshl.16\t%c\n\tshl.16\t%c\n */
/* 228 */	1,	/* ?\tcopy\t%c,%0\n\tvshl.16\t%c\n */
/* 229 */	1,	/* ?\tcopy\t%c,%0\n\tcopy\tc,%1\n\tvshl.16\t%c\n */
/* 230 */	1,	/* ?\tCOPY32(%c,%0)\n\tLSH32(%c)\n */
/* 231 */	1,	/* ?\tCOPY32(%c,%0)\n\tcopy\tc,%1\n\tLSH32(%c)\n */
/* 232 */	1,	/* ?\tCOPY32(%c,%0)\n\tLSH32(%c)\n */
/* 233 */	1,	/* ?\tCOPY32(%c,%0)\n\tcopy\tc,%1\n\tLSH32(%c)\n */
/* 234 */	1,	/* ?\tcopy\t%c,%0\n\tshr.16\t%c\n */
/* 235 */	1,	/* ?\tcopy\t%c,%0\n\tshr.16\t%c\n\tshr.16\t%c\n */
/* 236 */	1,	/* ?\tcopy\t%c,%0\n\tvshr.16\t%c\n */
/* 237 */	1,	/* ?\tcopy\t%c,%0\n\tcopy\tc,%1\n\tvshr.16\t%c\n */
/* 238 */	1,	/* ?\tcopy\t%c,%0\n\tshr.16\t%c\n */
/* 239 */	1,	/* ?\tcopy\t%c,%0\n\tshr.16\t%c\n\tshr.16\t%c\n */
/* 240 */	1,	/* ?\tcopy\t%c,%0\n\tvshr.16\t%c\n */
/* 241 */	1,	/* ?\tcopy\t%c,%0\n\tcopy\tc,%1\n\tvshr.16\t%c\n */
/* 242 */	1,	/* ?\tCOPY32(%c,%0)\n\tRSH32(%c)\n */
/* 243 */	1,	/* ?\tCOPY32(%c,%0)\n\tcopy\tc,%1\n\tRSH32(%c)\n */
/* 244 */	1,	/* ?\tCOPY32(%c,%0)\n\tRSH32(%c)\n */
/* 245 */	1,	/* ?\tCOPY32(%c,%0)\n\tcopy\tc,%1\n\tRSH32(%c)\n */
/* 246 */	1,	/* \tcmpb.eq.16\t%0,%1,%a\n */
/* 247 */	1,	/* \tcmpb.eq.16\t%0,%1,%a\n */
/* 248 */	1,	/* \tcmpb.eq.16\t%0,%1,%a\n */
/* 249 */	1,	/* \tbclr.16\t%0,%1,%a\n */
/* 250 */	1,	/* \tcmpb.eq.16\t%0,%1,%a\n */
/* 251 */	1,	/* \tcmpb.eq.16\t%0,%1,%a\n */
/* 252 */	1,	/* \tcmpb.eq.16\t%0,%1,%a\n */
/* 253 */	1,	/* \tbclr.16\t%0,%1,%a\n */
/* 254 */	1,	/* \tcmpb.eq.8\t%0,%1,%a\n */
/* 255 */	1,	/* \tcmpb.eq.8\t%0,%1,%a\n */
/* 256 */	1,	/* \tcmpb.eq.8\t%0,%1,%a\n */
/* 257 */	1,	/* \tcmpb.eq.8\t%0,%1,%a\n */
/* 258 */	1,	/* \tcmpb.eq.8\t%0,%1,%a\n */
/* 259 */	1,	/* \tcmpb.eq.8\t%0,%1,%a\n */
/* 260 */	1,	/* \tcmpb.eq.8\t%0,%1,%a\n */
/* 261 */	1,	/* \tcmpb.eq.8\t%0,%1,%a\n */
/* 262 */	1,	/* \tbclr.8\t%0,%1,%a\n */
/* 263 */	1,	/* \tbclr.8\t%0,%1,%a\n */
/* 264 */	1,	/* \tbclr.8\t%0,%1,%a\n */
/* 265 */	1,	/* \tcmpb.eq.8\t%0,%1,%a\n */
/* 266 */	1,	/* \tcmpb.eq.8\t%0,%1,%a\n */
/* 267 */	1,	/* \tcmpb.eq.8\t%0,%1,%a\n */
/* 268 */	1,	/* \tcmpb.eq.8\t%0,%1,%a\n */
/* 269 */	1,	/* \tcmpb.eq.8\t%0,%1,%a\n */
/* 270 */	1,	/* \tcmpb.eq.8\t%0,%1,%a\n */
/* 271 */	1,	/* \tbclr.8\t%0,%1,%a\n */
/* 272 */	1,	/* \tCMPBEQ32(%0,%1,%a)\n */
/* 273 */	1,	/* \tCMPBEQ32(%0,%1,%a)\n */
/* 274 */	1,	/* \tCMPBEQ32(%0,%1,%a)\n */
/* 275 */	1,	/* \tCMPBEQ32(%0,%1,%a)\n */
/* 276 */	1,	/* \tCMPBEQ32(%0,%1,%a)\n */
/* 277 */	1,	/* \tCMPBEQ32(%0,%1,%a)\n */
/* 278 */	1,	/* \tCMPBEQF32(%0,%1,%a)\n */
/* 279 */	1,	/* \tCMPBEQF32(%0,%1,%a)\n */
/* 280 */	1,	/* \tCMPBEQF32(%0,%1,%a)\n */
/* 281 */	1,	/* \tCMPBEQF64(%0,%1,%a)\n */
/* 282 */	1,	/* \tCMPBEQF64(%0,%1,%a)\n */
/* 283 */	1,	/* \tCMPBEQF64(%0,%1,%a)\n */
/* 284 */	1,	/* \tcmpb.ne.16\t%0,%1,%a\n */
/* 285 */	1,	/* \tcmpb.ne.16\t%0,%1,%a\n */
/* 286 */	1,	/* \tcmpb.ne.16\t%0,%1,%a\n */
/* 287 */	1,	/* \tcmpb.ne.16\t%0,%1,%a\n */
/* 288 */	1,	/* \tbset.16\t%0,%1,%a\n */
/* 289 */	1,	/* \tcmpb.ne.16\t%0,%1,%a\n */
/* 290 */	1,	/* \tcmpb.ne.16\t%0,%1,%a\n */
/* 291 */	1,	/* \tcmpb.ne.16\t%0,%1,%a\n */
/* 292 */	1,	/* \tcmpb.ne.16\t%0,%1,%a\n */
/* 293 */	1,	/* \tbset.16\t%0,%1,%a\n */
/* 294 */	1,	/* \tcmpb.ne.8\t%0,%1,%a\n */
/* 295 */	1,	/* \tcmpb.ne.8\t%0,%1,%a\n */
/* 296 */	1,	/* \tcmpb.ne.8\t%0,%1,%a\n */
/* 297 */	1,	/* \tcmpb.ne.8\t%0,%1,%a\n */
/* 298 */	1,	/* \tcmpb.ne.8\t%0,%1,%a\n */
/* 299 */	1,	/* \tcmpb.ne.8\t%0,%1,%a\n */
/* 300 */	1,	/* \tcmpb.ne.8\t%0,%1,%a\n */
/* 301 */	1,	/* \tcmpb.ne.8\t%0,%1,%a\n */
/* 302 */	1,	/* \tbset.8\t%0,%1,%a\n */
/* 303 */	1,	/* \tbset.8\t%0,%1,%a\n */
/* 304 */	1,	/* \tbset.8\t%0,%1,%a\n */
/* 305 */	1,	/* \tcmpb.ne.8\t%0,%1,%a\n */
/* 306 */	1,	/* \tcmpb.ne.8\t%0,%1,%a\n */
/* 307 */	1,	/* \tcmpb.ne.8\t%0,%1,%a\n */
/* 308 */	1,	/* \tcmpb.ne.8\t%0,%1,%a\n */
/* 309 */	1,	/* \tcmpb.ne.8\t%0,%1,%a\n */
/* 310 */	1,	/* \tcmpb.ne.8\t%0,%1,%a\n */
/* 311 */	1,	/* \tbset.8\t%0,%1,%a\n */
/* 312 */	1,	/* \tCMPBNE32(%0,%1,%a)\n */
/* 313 */	1,	/* \tCMPBNE32(%0,%1,%a)\n */
/* 314 */	1,	/* \tCMPBNE32(%0,%1,%a)\n */
/* 315 */	1,	/* \tCMPBNE32(%0,%1,%a)\n */
/* 316 */	1,	/* \tCMPBNE32(%0,%1,%a)\n */
/* 317 */	1,	/* \tCMPBNE32(%0,%1,%a)\n */
/* 318 */	1,	/* \tCMPBNEF32(%0,%1,%a)\n */
/* 319 */	1,	/* \tCMPBNEF32(%0,%1,%a)\n */
/* 320 */	1,	/* \tCMPBNEF32(%0,%1,%a)\n */
/* 321 */	1,	/* \tCMPBNEF64(%0,%1,%a)\n */
/* 322 */	1,	/* \tCMPBNEF64(%0,%1,%a)\n */
/* 323 */	1,	/* \tCMPBNEF64(%0,%1,%a)\n */
/* 324 */	1,	/* \tcmpb.lt.16\t%0,%1,%a\n */
/* 325 */	1,	/* \tcmpb.lt.16\t%0,%1,%a\n */
/* 326 */	1,	/* \tcmpb.lt.16\t%0,%1,%a\n */
/* 327 */	1,	/* \tcmp.16\t%0,%1\n\tbr.ltu\t%a\n */
/* 328 */	1,	/* \tcmp.16\t%0,%1\n\tbr.ltu\t%a\n */
/* 329 */	1,	/* \tcmp.16\t%0,%1\n\tbr.ltu\t%a\n */
/* 330 */	1,	/* \tcmpb.lt.8\t%0,%1,%a\n */
/* 331 */	1,	/* \tcmpb.lt.8\t%0,%1,%a\n */
/* 332 */	1,	/* \tcmpb.lt.8\t%0,%1,%a\n */
/* 333 */	1,	/* \tcmpb.lt.8\t%0,%1,%a\n */
/* 334 */	1,	/* \tcmpb.lt.8\t%0,%1,%a\n */
/* 335 */	1,	/* \tcmpb.lt.8\t%0,%1,%a\n */
/* 336 */	1,	/* \tcmpb.lt.8\t%0,%1,%a\n */
/* 337 */	1,	/* \tcmpb.lt.8\t%0,%1,%a\n */
/* 338 */	1,	/* \tcmp.8\t%0,%1\n\tbr.ltu\t%a\n */
/* 339 */	1,	/* \tcmp.8\t%0,%1\n\tbr.ltu\t%a\n */
/* 340 */	1,	/* \tcmp.8\t%0,%1\n\tbr.ltu\t%a\n */
/* 341 */	1,	/* \tcmp.8\t%0,%1\n\tbr.ltu\t%a\n */
/* 342 */	1,	/* \tcmp.8\t%0,%1\n\tbr.ltu\t%a\n */
/* 343 */	1,	/* \tCMPBLT32(%0,%1,%a)\n */
/* 344 */	1,	/* \tCMPBLT32(%0,%1,%a)\n */
/* 345 */	1,	/* \tCMPBLT32(%0,%1,%a)\n */
/* 346 */	1,	/* \tCMPBLTU32(%0,%1,%a)\n */
/* 347 */	1,	/* \tCMPBLTU32(%0,%1,%a)\n */
/* 348 */	1,	/* \tCMPBLTU32(%0,%1,%a)\n */
/* 349 */	1,	/* \tCMPBLTF32(%0,%1,%a)\n */
/* 350 */	1,	/* \tCMPBLTF32(%0,%1,%a)\n */
/* 351 */	1,	/* \tCMPBLTF32(%0,%1,%a)\n */
/* 352 */	1,	/* \tCMPBLTF32(%0,%1,%a)\n */
/* 353 */	1,	/* \tCMPBLTF64(%0,%1,%a)\n */
/* 354 */	1,	/* \tCMPBLTF64(%0,%1,%a)\n */
/* 355 */	1,	/* \tcmpb.le.16\t%0,%1,%a\n */
/* 356 */	1,	/* \tcmpb.le.16\t%0,%1,%a\n */
/* 357 */	1,	/* \tcmpb.le.16\t%0,%1,%a\n */
/* 358 */	1,	/* \tcmp.16\t%0,%1\n\tbr.leu\t%a\n */
/* 359 */	1,	/* \tcmp.16\t%0,%1\n\tbr.leu\t%a\n */
/* 360 */	1,	/* \tcmp.16\t%0,%1\n\tbr.leu\t%a\n */
/* 361 */	1,	/* \tcmpb.le.8\t%0,%1,%a\n */
/* 362 */	1,	/* \tcmpb.le.8\t%0,%1,%a\n */
/* 363 */	1,	/* \tcmpb.le.8\t%0,%1,%a\n */
/* 364 */	1,	/* \tcmpb.le.8\t%0,%1,%a\n */
/* 365 */	1,	/* \tcmpb.le.8\t%0,%1,%a\n */
/* 366 */	1,	/* \tcmpb.le.8\t%0,%1,%a\n */
/* 367 */	1,	/* \tcmpb.le.8\t%0,%1,%a\n */
/* 368 */	1,	/* \tcmp.8\t%0,%1\n\tbr.leu\t%a\n */
/* 369 */	1,	/* \tcmp.8\t%0,%1\n\tbr.leu\t%a\n */
/* 370 */	1,	/* \tcmp.8\t%0,%1\n\tbr.leu\t%a\n */
/* 371 */	1,	/* \tcmp.8\t%0,%1\n\tbr.leu\t%a\n */
/* 372 */	1,	/* \tcmp.8\t%0,%1\n\tbr.leu\t%a\n */
/* 373 */	1,	/* \tCMPBLE32(%0,%1,%a)\n */
/* 374 */	1,	/* \tCMPBLE32(%0,%1,%a)\n */
/* 375 */	1,	/* \tCMPBLE32(%0,%1,%a)\n */
/* 376 */	1,	/* \tCMPBLEU32(%0,%1,%a)\n */
/* 377 */	1,	/* \tCMPBLEU32(%0,%1,%a)\n */
/* 378 */	1,	/* \tCMPBLEU32(%0,%1,%a)\n */
/* 379 */	1,	/* \tCMPBLEF32(%0,%1,%a)\n */
/* 380 */	1,	/* \tCMPBLEF32(%0,%1,%a)\n */
/* 381 */	1,	/* \tCMPBLEF32(%0,%1,%a)\n */
/* 382 */	1,	/* \tCMPBLEF64(%0,%1,%a)\n */
/* 383 */	1,	/* \tCMPBLEF64(%0,%1,%a)\n */
/* 384 */	1,	/* \tCMPBLEF64(%0,%1,%a)\n */
/* 385 */	1,	/* \tcmpb.lt.16\t%1,%0,%a\n */
/* 386 */	1,	/* \tcmpb.lt.16\t%1,%0,%a\n */
/* 387 */	1,	/* \tcmpb.lt.16\t%1,%0,%a\n */
/* 388 */	1,	/* \tcmp.16\t%1,%0\n\tbr.ltu\t%a\n */
/* 389 */	1,	/* \tcmp.16\t%1,%0\n\tbr.ltu\t%a\n */
/* 390 */	1,	/* \tcmp.16\t%1,%0\n\tbr.ltu\t%a\n */
/* 391 */	1,	/* \tcmpb.lt.8\t%1,%0,%a\n */
/* 392 */	1,	/* \tcmpb.lt.8\t%1,%0,%a\n */
/* 393 */	1,	/* \tcmpb.lt.8\t%1,%0,%a\n */
/* 394 */	1,	/* \tcmpb.lt.8\t%1,%0,%a\n */
/* 395 */	1,	/* \tcmpb.lt.8\t%1,%0,%a\n */
/* 396 */	1,	/* \tcmpb.lt.8\t%1,%0,%a\n */
/* 397 */	1,	/* \tcmpb.lt.8\t%1,%0,%a\n */
/* 398 */	1,	/* \tcmp.8\t%1,%0\n\tbr.ltu\t%a\n */
/* 399 */	1,	/* \tcmp.8\t%1,%0\n\tbr.ltu\t%a\n */
/* 400 */	1,	/* \tcmp.8\t%1,%0\n\tbr.ltu\t%a\n */
/* 401 */	1,	/* \tcmp.8\t%1,%0\n\tbr.ltu\t%a\n */
/* 402 */	1,	/* \tcmp.8\t%1,%0\n\tbr.ltu\t%a\n */
/* 403 */	1,	/* \tCMPBLT32(%1,%0,%a)\n */
/* 404 */	1,	/* \tCMPBLT32(%1,%0,%a)\n */
/* 405 */	1,	/* \tCMPBLT32(%1,%0,%a)\n */
/* 406 */	1,	/* \tCMPBLTU32(%1,%0,%a)\n */
/* 407 */	1,	/* \tCMPBLTU32(%1,%0,%a)\n */
/* 408 */	1,	/* \tCMPBLTU32(%1,%0,%a)\n */
/* 409 */	1,	/* \tCMPBLTF32(%1,%0,%a)\n */
/* 410 */	1,	/* \tCMPBLTF32(%1,%0,%a)\n */
/* 411 */	1,	/* \tCMPBLTF32(%1,%0,%a)\n */
/* 412 */	1,	/* \tCMPBLTF64(%1,%0,%a)\n */
/* 413 */	1,	/* \tCMPBLTF64(%1,%0,%a)\n */
/* 414 */	1,	/* \tCMPBLTF64(%1,%0,%a)\n */
/* 415 */	1,	/* \tcmpb.le.16\t%1,%0,%a\n */
/* 416 */	1,	/* \tcmpb.le.16\t%1,%0,%a\n */
/* 417 */	1,	/* \tcmpb.le.16\t%1,%0,%a\n */
/* 418 */	1,	/* \tcmp.16\t%1,%0\n\tbr.leu\t%a\n */
/* 419 */	1,	/* \tcmp.16\t%1,%0\n\tbr.leu\t%a\n */
/* 420 */	1,	/* \tcmp.16\t%1,%0\n\tbr.leu\t%a\n */
/* 421 */	1,	/* \tcmpb.le.8\t%1,%0,%a\n */
/* 422 */	1,	/* \tcmpb.le.8\t%1,%0,%a\n */
/* 423 */	1,	/* \tcmpb.le.8\t%1,%0,%a\n */
/* 424 */	1,	/* \tcmpb.le.8\t%1,%0,%a\n */
/* 425 */	1,	/* \tcmpb.le.8\t%1,%0,%a\n */
/* 426 */	1,	/* \tcmpb.le.8\t%1,%0,%a\n */
/* 427 */	1,	/* \tcmpb.le.8\t%1,%0,%a\n */
/* 428 */	1,	/* \tcmp.8\t%1,%0\n\tbr.leu\t%a\n */
/* 429 */	1,	/* \tcmp.8\t%1,%0\n\tbr.leu\t%a\n */
/* 430 */	1,	/* \tcmp.8\t%1,%0\n\tbr.leu\t%a\n */
/* 431 */	1,	/* \tcmp.8\t%1,%0\n\tbr.leu\t%a\n */
/* 432 */	1,	/* \tcmp.8\t%1,%0\n\tbr.leu\t%a\n */
/* 433 */	1,	/* \tCMPBLE32(%1,%0,%a)\n */
/* 434 */	1,	/* \tCMPBLE32(%1,%0,%a)\n */
/* 435 */	1,	/* \tCMPBLE32(%1,%0,%a)\n */
/* 436 */	1,	/* \tCMPBLEU32(%1,%0,%a)\n */
/* 437 */	1,	/* \tCMPBLEU32(%1,%0,%a)\n */
/* 438 */	1,	/* \tCMPBLEU32(%1,%0,%a)\n */
/* 439 */	1,	/* \tCMPBLEF32(%1,%0,%a)\n */
/* 440 */	1,	/* \tCMPBLEF32(%1,%0,%a)\n */
/* 441 */	1,	/* \tCMPBLEF32(%1,%0,%a)\n */
/* 442 */	1,	/* \tCMPBLEF64(%1,%0,%a)\n */
/* 443 */	1,	/* \tCMPBLEF64(%1,%0,%a)\n */
/* 444 */	1,	/* \tCMPBLEF64(%1,%0,%a)\n */
/* 445 */	0,	/* %a-$global$(dp) */
/* 446 */	0,	/* %a(dp) */
/* 447 */	0,	/* %a+%F(sp) */
/* 448 */	0,	/* %a+4+%F(sp) */
/* 449 */	0,	/* %1(%0) */
/* 450 */	0,	/* %0 */
/* 451 */	0,	/* %0 */
/* 452 */	0,	/* %0 */
/* 453 */	0,	/* %0 */
/* 454 */	0,	/* %0 */
/* 455 */	0,	/* %0 */
/* 456 */	0,	/* 0(%0) */
/* 457 */	0,	/* 0(%0) */
/* 458 */	0,	/* 0(%0) */
/* 459 */	0,	/* %0 */
/* 460 */	0,	/* %0 */
/* 461 */	0,	/* 0(%0) */
/* 462 */	0,	/* 0(%0) */
/* 463 */	0,	/* %0 */
/* 464 */	0,	/* %0 */
/* 465 */	0,	/*  */
/* 466 */	1,	/* %a:\n */
/* 467 */	1,	/* \tld.16\t%c,%a\n */
/* 468 */	1,	/* \tlea\t%c,%a+%F(sp)\n */
/* 469 */	1,	/* \tlea\t%c,%a+4+%F(sp)\n */
/* 470 */	1,	/* \tld.8\t%c,0(%0)\n */
/* 471 */	1,	/* \tld.8\t%c,0(%0)\n */
/* 472 */	1,	/* \tld.8\t%c,%0\n */
/* 473 */	1,	/* \tld.8\t%c,%0\n */
/* 474 */	1,	/* \tld.8\t%c,0(%0)\n */
/* 475 */	1,	/* \tld.8\t%c,0(%0)\n */
/* 476 */	1,	/* \tld.8\t%c,%0\n */
/* 477 */	1,	/* \tld.8\t%c,%0\n */
/* 478 */	1,	/* \tld.16\t%c,0(%0)\n */
/* 479 */	1,	/* \tld.16\t%c,%0\n */
/* 480 */	1,	/* \tld.16\t%c,0(%0)\n */
/* 481 */	1,	/* \tld.16\t%c,%0\n */
/* 482 */	1,	/* \tld.16\t%c,0(%0)\n */
/* 483 */	1,	/* \tld.16\t%c,%0\n */
/* 484 */	1,	/* \tCOPY32(%c,0(%0))\n */
/* 485 */	1,	/* \tCOPY32(%c,%0)\n */
/* 486 */	1,	/* \tCOPY32(%c,0(%0))\n */
/* 487 */	1,	/* \tCOPY32(%c,%0)\n */
/* 488 */	1,	/* \tCOPY32(%c,0(%0))\n */
/* 489 */	1,	/* \tCOPY32(%c,%0)\n */
/* 490 */	1,	/* \tCOPY64(%c,0(%0))\n */
/* 491 */	1,	/* \tCOPY64(%c,%0)\n */
/* 492 */	0,	/* %0 */
/* 493 */	0,	/* %a */
/* 494 */	1,	/* \tcall\t%0\n */
/* 495 */	1,	/* \tcall\t%0\n */
/* 496 */	1,	/* \tcall\t%0\n */
/* 497 */	1,	/* \tcall\t%0\n\tst.16\t2+%c,a\n\tst.16\t%c,b\n */
/* 498 */	1,	/* \tcall\t%0\n */
/* 499 */	1,	/* \tcall\t%0\n\tst.16\t2+%c,a\n\tst.16\t%c,b\n */
/* 500 */	1,	/* \tcall\t%0\n */
/* 501 */	1,	/* \tcall\t%0\n */
/* 502 */	1,	/* \tcall\t%0\n */
/* 503 */	1,	/* \tcall\t%0\n\tst.16\t2+%c,a\n\tst.16\t%c,b\n */
/* 504 */	1,	/* \tcall\t%0 ; bogus? \n */
/* 505 */	1,	/* \tlea\ta,%c\n\tcall\t%0\n */
/* 506 */	1,	/* \tlea\ta,%c\n\tcall\t%0\n */
/* 507 */	1,	/* # rtarget should have handled it\n */
/* 508 */	1,	/* # rtarget should have handled it\n */
/* 509 */	1,	/* # rtarget should have handled it\n */
/* 510 */	1,	/* # Let emit2 fix up\n */
/* 511 */	1,	/* # Let emit2 fix up\n */
/* 512 */	1,	/* # Let emit2 fix up\n */
/* 513 */	1,	/* # Let emit2 fix up\n */
/* 514 */	1,	/* # ret\n */
/* 515 */	1,	/* # Let emit2 handle args\n */
/* 516 */	1,	/* # Let emit2 handle args\n */
/* 517 */	1,	/* # Let emit2 handle args\n */
/* 518 */	1,	/* # Let emit2 handle args\n */
/* 519 */	1,	/* # Let emit2 handle args\n */
/* 520 */	1,	/* # Let emit2 handle args\n */
/* 521 */	1,	/* # Let emit2 handle args\n */
/* 522 */	1,	/* \tbr\t%0\n */
/* 523 */	0,	/* %0 */
/* 524 */	0,	/* %0 */
/* 525 */	0,	/* %0 */
/* 526 */	0,	/* %0 */
/* 527 */	0,	/* %0 */
/* 528 */	0,	/* %0 */
/* 529 */	1,	/* \tst.8\t0(%0),%1\n */
/* 530 */	1,	/* \tst.8\t%0,%1\n */
/* 531 */	1,	/* \tst.8\t0(%0),%1\n */
/* 532 */	1,	/* \tst.8\t%0,%1\n */
/* 533 */	1,	/* \tst.16\t0(%0),%1\n */
/* 534 */	1,	/* \tst.16\t%0,%1\n */
/* 535 */	1,	/* \tst.16\t0(%0),%1\n */
/* 536 */	1,	/* \tst.16\t%0,%1\n */
/* 537 */	1,	/* \tst.16\t0(%0),%1\n */
/* 538 */	1,	/* \tst.16\t%0,%1\n */
/* 539 */	1,	/* \tld.16\tc,%a\n\tmemcopy\n */
/* 540 */	1,	/* # let emit2 handle %0\n */
/* 541 */	1,	/* \tCOPY32(0(%0),%1)\n */
/* 542 */	1,	/* \tCOPY32(%0,%1)\n */
/* 543 */	1,	/* \tCOPY32(%0,%1)\n */
/* 544 */	1,	/* \tCOPY32(0(%0),%1)\n */
/* 545 */	1,	/* \tCOPY32(%0,%1)\n */
/* 546 */	1,	/* \tCOPY32(%0,%1)\n */
/* 547 */	1,	/* \tCOPY32(0(%0),%1)\n */
/* 548 */	1,	/* \tCOPY32(%0,%1)\n */
/* 549 */	1,	/* \tCOPY32(%0,%1)\n */
/* 550 */	1,	/* \tCOPY64(0(%0),%1)\n */
/* 551 */	1,	/* \tCOPY64(%0,%1)\n */
/* 552 */	1,	/* \tCOPY64(%0,%1)\n */
/* 553 */	1,	/* \tADD32(%1,%2)\n */
/* 554 */	1,	/* \tADD32(%2,%1)\n */
/* 555 */	1,	/* \tADD32(%1,%2)\n */
/* 556 */	1,	/* \tADD32(%2,%1)\n */
/* 557 */	1,	/* \tFADD32(%1,%2)\n */
/* 558 */	1,	/* \tFADD32(%2,%1)\n */
/* 559 */	1,	/* \tFADD64(%1,%2)\n */
/* 560 */	1,	/* \tFADD64(%2,%1)\n */
/* 561 */	1,	/* \tCOPY32(%0,%1)\n\tADD32(%0,%2)\n */
/* 562 */	1,	/* \tCOPY32(%0,%1)\n\tADD32(%0,%2)\n */
/* 563 */	1,	/* \tCOPY32(%0,%1)\n\tFADD32(%0,%2)\n */
/* 564 */	1,	/* \tCOPY64(%0,%1)\n\tFADD64(%0,%2)\n */
/* 565 */	1,	/* \tSUB32(%1,%2)\n */
/* 566 */	1,	/* \tSUB32(%1,%2)\n */
/* 567 */	1,	/* \tFSUB32(%1,%2)\n */
/* 568 */	1,	/* \tFSUB64(%1,%2)\n */
/* 569 */	1,	/* \tCOPY32(%0,%1)\n\tSUB32(%0,%2)\n */
/* 570 */	1,	/* \tCOPY32(%0,%1)\n\tSUB32(%0,%2)\n */
/* 571 */	1,	/* \tCOPY32(%0,%1)\n\tFSUB32(%0,%2)\n */
/* 572 */	1,	/* \tCOPY64(%0,%1)\n\tFSUB64(%0,%2)\n */
/* 573 */	1,	/* \tMULI32(%1,%2)\n */
/* 574 */	1,	/* \tMULI32(%2,%1)\n */
/* 575 */	1,	/* \tMULU32(%1,%2)\n */
/* 576 */	1,	/* \tMULU32(%2,%1)\n */
/* 577 */	1,	/* \tFMUL32(%1,%2)\n */
/* 578 */	1,	/* \tFMUL32(%2,%1)\n */
/* 579 */	1,	/* \tFMUL64(%1,%2)\n */
/* 580 */	1,	/* \tFMUL64(%2,%1)\n */
/* 581 */	1,	/* \tCOPY32(%0,%1)\n\tMULI32(%0,%2)\n */
/* 582 */	1,	/* \tCOPY32(%0,%1)\n\tMULU32(%0,%2)\n */
/* 583 */	1,	/* \tCOPY32(%0,%1)\n\tFMUL32(%0,%2)\n */
/* 584 */	1,	/* \tCOPY64(%0,%1)\n\tFMUL64(%0,%2)\n */
/* 585 */	1,	/* \tDIVI32(%1,%2)\n */
/* 586 */	1,	/* \tDIVU32(%1,%2)\n */
/* 587 */	1,	/* \tFDIV32(%1,%2)\n */
/* 588 */	1,	/* \tFDIV64(%1,%2)\n */
/* 589 */	1,	/* \tCOPY32(%0,%1)\n\tDIVI32(%0,%2)\n */
/* 590 */	1,	/* \tCOPY32(%0,%1)\n\tDIVU32(%0,%2)\n */
/* 591 */	1,	/* \tCOPY32(%0,%1)\n\tFDIV32(%0,%2)\n */
/* 592 */	1,	/* \tCOPY64(%0,%1)\n\tFDIV64(%0,%2)\n */
/* 593 */	1,	/* \tAND32(%1,%2)\n */
/* 594 */	1,	/* \tAND32(%2,%1)\n */
/* 595 */	1,	/* \tAND32(%1,%2)\n */
/* 596 */	1,	/* \tAND32(%2,%1)\n */
/* 597 */	1,	/* \tCOPY32(%0,%1)\n\tAND32(%0,%2)\n */
/* 598 */	1,	/* \tCOPY32(%0,%1)\n\tAND32(%0,%2)\n */
/* 599 */	1,	/* \tOR32(%1,%2)\n */
/* 600 */	1,	/* \tOR32(%2,%1)\n */
/* 601 */	1,	/* \tOR32(%1,%2)\n */
/* 602 */	1,	/* \tOR32(%2,%1)\n */
/* 603 */	1,	/* \tCOPY32(%0,%1)\n\tOR32(%0,%2)\n */
/* 604 */	1,	/* \tCOPY32(%0,%1)\n\tOR32(%0,%2)\n */
/* 605 */	1,	/* \tXOR32(%1,%2)\n */
/* 606 */	1,	/* \tXOR32(%2,%1)\n */
/* 607 */	1,	/* \tXOR32(%1,%2)\n */
/* 608 */	1,	/* \tXOR32(%2,%1)\n */
/* 609 */	1,	/* \tCOPY32(%0,%1)\n\tXOR32(%0,%2)\n */
/* 610 */	1,	/* \tCOPY32(%0,%1)\n\tXOR32(%0,%2)\n */
};

static char *_string[] = {
/* 0 */	0,
/* 1 */	"reg8: LOADI1(reg8)",
/* 2 */	"reg8: LOADI1(reg)",
/* 3 */	"reg8: LOADI1(reg32)",
/* 4 */	"reg8: LOADU1(reg8)",
/* 5 */	"reg8: LOADU1(reg)",
/* 6 */	"reg8: LOADU1(reg32)",
/* 7 */	"reg: LOADI2(reg)",
/* 8 */	"reg: LOADI2(reg32)",
/* 9 */	"reg: LOADU2(reg)",
/* 10 */	"reg: LOADU2(reg32)",
/* 11 */	"reg: LOADP2(reg)",
/* 12 */	"reg32: LOADI4(reg32)",
/* 13 */	"reg32: LOADU4(reg32)",
/* 14 */	"regf64: LOADF4(regf64)",
/* 15 */	"regf64: LOADF8(regf64)",
/* 16 */	"reg8: INDIRI1(VREGP)",
/* 17 */	"reg8: INDIRU1(VREGP)",
/* 18 */	"reg: INDIRI2(VREGP)",
/* 19 */	"reg: INDIRU2(VREGP)",
/* 20 */	"reg: INDIRP2(VREGP)",
/* 21 */	"reg32: INDIRI4(VREGP)",
/* 22 */	"reg32: INDIRU4(VREGP)",
/* 23 */	"regf64: INDIRF4(VREGP)",
/* 24 */	"regf64: INDIRF8(VREGP)",
/* 25 */	"stmt: ASGNI1(VREGP,reg8)",
/* 26 */	"stmt: ASGNU1(VREGP,reg8)",
/* 27 */	"stmt: ASGNI2(VREGP,reg)",
/* 28 */	"stmt: ASGNU2(VREGP,reg)",
/* 29 */	"stmt: ASGNP2(VREGP,reg)",
/* 30 */	"stmt: ASGNI4(VREGP,reg32)",
/* 31 */	"stmt: ASGNU4(VREGP,reg32)",
/* 32 */	"stmt: ASGNF4(VREGP,regf64)",
/* 33 */	"stmt: ASGNF8(VREGP,regf64)",
/* 34 */	"regf64: CVFF4(regf64)",
/* 35 */	"regf64: CVFF8(regf64)",
/* 36 */	"regf64: CVIF4(reg32)",
/* 37 */	"regf64: CVIF4(reg)",
/* 38 */	"regf64: CVIF8(reg32)",
/* 39 */	"regf64: CVIF8(reg)",
/* 40 */	"reg32: CVFI4(regf64)",
/* 41 */	"reg32: CVFI4(regf64)",
/* 42 */	"reg: CVFI2(regf64)",
/* 43 */	"reg: CVFI2(regf64)",
/* 44 */	"reg8: CVII1(reg32)",
/* 45 */	"reg8: CVII1(reg)",
/* 46 */	"reg8: CVUI1(reg32)",
/* 47 */	"reg8: CVUI1(reg)",
/* 48 */	"reg8: LOADU1(LOADU2(CVII2(reg)))",
/* 49 */	"reg8: LOADI1(CVUI2(reg))",
/* 50 */	"reg8: LOADU1(LOADU2(CVII2(reg8)))",
/* 51 */	"reg8: LOADI1(CVUI2(reg8))",
/* 52 */	"reg: CVII2(reg8)",
/* 53 */	"reg: CVII2(reg32)",
/* 54 */	"reg: CVUI2(reg8)",
/* 55 */	"reg: CVUI2(reg32)",
/* 56 */	"reg8: CVIU1(reg32)",
/* 57 */	"reg8: CVIU1(reg)",
/* 58 */	"reg8: CVUU1(reg32)",
/* 59 */	"reg8: CVUU1(reg)",
/* 60 */	"reg: CVIU2(reg8)",
/* 61 */	"reg: CVIU2(reg32)",
/* 62 */	"reg: CVUU2(reg8)",
/* 63 */	"reg: CVUU2(reg32)",
/* 64 */	"reg32: CVUU4(reg)",
/* 65 */	"reg32: CVIU4(reg)",
/* 66 */	"reg32: CVUI4(reg)",
/* 67 */	"reg32: CVII4(reg)",
/* 68 */	"reg: con",
/* 69 */	"reg8: con",
/* 70 */	"c0: CNSTI2",
/* 71 */	"c1: CNSTI2",
/* 72 */	"c2: CNSTI2",
/* 73 */	"c0: CNSTU2",
/* 74 */	"c1: CNSTU2",
/* 75 */	"c2: CNSTU2",
/* 76 */	"con1: CNSTI1",
/* 77 */	"con1: CNSTU1",
/* 78 */	"con2: CNSTI2",
/* 79 */	"con2: CNSTU2",
/* 80 */	"con4: CNSTI4",
/* 81 */	"con4: CNSTU4",
/* 82 */	"con4: CNSTP2",
/* 83 */	"con: con1",
/* 84 */	"con: con2",
/* 85 */	"con: con4",
/* 86 */	"con: c0",
/* 87 */	"con: c1",
/* 88 */	"con: c2",
/* 89 */	"reg: con1",
/* 90 */	"reg: con2",
/* 91 */	"creg: con",
/* 92 */	"reg: ADDI2(reg,reg)",
/* 93 */	"reg: ADDI2(reg,con)",
/* 94 */	"reg: ADDI2(reg,dref16)",
/* 95 */	"reg: ADDU2(reg,reg)",
/* 96 */	"reg: ADDU2(reg,con)",
/* 97 */	"reg: ADDU2(reg,dref16)",
/* 98 */	"reg: ADDP2(reg,con)",
/* 99 */	"reg: ADDP2(reg,reg)",
/* 100 */	"reg: ADDP2(LSHI2(reg,c1),reg)",
/* 101 */	"reg: ADDP2(LSHU2(reg,c1),reg)",
/* 102 */	"reg8: ADDI2(reg8,reg8)",
/* 103 */	"reg8: ADDI2(reg8,con)",
/* 104 */	"reg8: ADDI2(CVII2(reg8),con)",
/* 105 */	"reg8: ADDI2(reg8,CVII2(dref8))",
/* 106 */	"reg8: ADDI2(CVII2(reg8),CVII2(dref8))",
/* 107 */	"reg8: ADDU2(reg8,reg8)",
/* 108 */	"reg8: ADDU2(reg8,con)",
/* 109 */	"reg8: ADDU2(reg8,CVUU2(dref8))",
/* 110 */	"reg8: ADDU2(CVUU2(reg8),CVUU2(dref8))",
/* 111 */	"reg32: ADDI4(reg32,reg32)",
/* 112 */	"reg32: ADDI4(reg32,INDIRI4(faddr))",
/* 113 */	"reg32: ADDU4(reg32,reg32)",
/* 114 */	"reg32: ADDU4(reg32,INDIRU4(faddr))",
/* 115 */	"regf64: ADDF4(regf64,regf64)",
/* 116 */	"regf64: ADDF4(regf64,INDIRF4(faddr))",
/* 117 */	"regf64: ADDF8(regf64,regf64)",
/* 118 */	"regf64: ADDF8(regf64,INDIRF8(faddr))",
/* 119 */	"reg: SUBI2(reg,reg)",
/* 120 */	"reg: SUBI2(reg,con)",
/* 121 */	"reg: SUBI2(reg,dref16)",
/* 122 */	"reg: SUBU2(reg,reg)",
/* 123 */	"reg: SUBU2(reg,con)",
/* 124 */	"reg: SUBU2(reg,dref16)",
/* 125 */	"reg: SUBP2(reg,reg)",
/* 126 */	"reg: SUBP2(reg,con)",
/* 127 */	"reg: SUBP2(reg,dref16)",
/* 128 */	"reg8: SUBI2(reg8,reg8)",
/* 129 */	"reg8: SUBI2(reg8,con)",
/* 130 */	"reg8: SUBI2(reg8,CVII2(dref16))",
/* 131 */	"reg8: SUBU2(reg8,reg8)",
/* 132 */	"reg8: SUBU2(reg8,con)",
/* 133 */	"reg8: SUBU2(reg8,CVUU2(dref16))",
/* 134 */	"reg32: SUBI4(reg32,reg32)",
/* 135 */	"reg32: SUBI4(reg32,INDIRI4(faddr))",
/* 136 */	"reg32: SUBU4(reg32,reg32)",
/* 137 */	"reg32: SUBU4(reg32,INDIRU4(faddr))",
/* 138 */	"regf64: SUBF4(regf64,regf64)",
/* 139 */	"regf64: SUBF4(regf64,INDIRF4(faddr))",
/* 140 */	"regf64: SUBF8(regf64,regf64)",
/* 141 */	"regf64: SUBF8(regf64,INDIRF8(faddr))",
/* 142 */	"reg: BANDI2(reg,reg)",
/* 143 */	"reg: BANDI2(reg,con)",
/* 144 */	"reg: BANDI2(reg,dref16)",
/* 145 */	"reg: BANDU2(reg,reg)",
/* 146 */	"reg: BANDU2(reg,con)",
/* 147 */	"reg: BANDU2(reg,dref16)",
/* 148 */	"reg8: BANDI2(reg8,reg8)",
/* 149 */	"reg8: BANDI2(CVII2(reg8),reg8)",
/* 150 */	"reg8: BANDI2(CVUI2(reg8),reg8)",
/* 151 */	"reg8: BANDI2(reg8,con)",
/* 152 */	"reg8: BANDI2(CVII2(reg8),con)",
/* 153 */	"reg8: BANDI2(CVUI2(reg8),con)",
/* 154 */	"reg8: BANDI2(reg8,CVII2(dref16))",
/* 155 */	"reg8: BANDU2(reg8,reg8)",
/* 156 */	"reg8: BANDU2(reg8,con)",
/* 157 */	"reg8: BANDU2(reg8,CVUU2(dref16))",
/* 158 */	"reg32: BANDI4(reg32,reg32)",
/* 159 */	"reg32: BANDI4(reg32,INDIRI4(faddr))",
/* 160 */	"reg32: BANDU4(reg32,reg32)",
/* 161 */	"reg32: BANDU4(reg32,INDIRU4(faddr))",
/* 162 */	"reg32: BANDU4(reg32,con)",
/* 163 */	"reg: BCOMI2(reg)",
/* 164 */	"reg: BCOMU2(reg)",
/* 165 */	"reg: BORI2(reg,reg)",
/* 166 */	"reg: BORI2(reg,con)",
/* 167 */	"reg: BORI2(reg,dref16)",
/* 168 */	"reg: BORU2(reg,reg)",
/* 169 */	"reg: BORU2(reg,con)",
/* 170 */	"reg: BORU2(reg,dref16)",
/* 171 */	"reg8: BORI2(reg8,reg8)",
/* 172 */	"reg8: BORI2(reg8,con)",
/* 173 */	"reg8: BORI2(reg8,CVII2(dref16))",
/* 174 */	"reg8: BORU2(reg8,reg8)",
/* 175 */	"reg8: BORU2(reg8,con)",
/* 176 */	"reg8: BORU2(reg8,CVUU2(dref16))",
/* 177 */	"reg32: BCOMI4(reg32)",
/* 178 */	"reg32: BCOMU4(reg32)",
/* 179 */	"reg32: BORI4(reg32,reg32)",
/* 180 */	"reg32: BORI4(reg32,INDIRI4(faddr))",
/* 181 */	"reg32: BORU4(reg32,reg32)",
/* 182 */	"reg32: BORU4(reg32,INDIRU4(faddr))",
/* 183 */	"reg: BXORI2(reg,reg)",
/* 184 */	"reg: BXORU2(reg,reg)",
/* 185 */	"reg32: BXORI4(reg32,reg32)",
/* 186 */	"reg32: BXORI4(reg32,INDIRI4(faddr))",
/* 187 */	"reg32: BXORU4(reg32,reg32)",
/* 188 */	"reg32: BXORU4(reg32,INDIRU4(faddr))",
/* 189 */	"reg: MULI2(reg,reg)",
/* 190 */	"reg: MULU2(reg,reg)",
/* 191 */	"reg32: MULI4(reg32,reg32)",
/* 192 */	"reg32: MULI4(reg32,INDIRI4(faddr))",
/* 193 */	"reg32: MULU4(reg32,reg32)",
/* 194 */	"reg32: MULU4(reg32,INDIRU4(faddr))",
/* 195 */	"regf64: MULF4(regf64,regf64)",
/* 196 */	"regf64: MULF4(regf64,INDIRF4(faddr))",
/* 197 */	"regf64: MULF8(regf64,regf64)",
/* 198 */	"regf64: MULF8(regf64,INDIRF8(faddr))",
/* 199 */	"reg: DIVI2(reg,reg)",
/* 200 */	"reg: DIVU2(reg,reg)",
/* 201 */	"reg32: DIVI4(reg32,reg32)",
/* 202 */	"reg32: DIVI4(reg32,INDIRI4(faddr))",
/* 203 */	"reg32: DIVU4(reg32,reg32)",
/* 204 */	"reg32: DIVU4(reg32,INDIRU4(faddr))",
/* 205 */	"regf64: DIVF4(regf64,regf64)",
/* 206 */	"regf64: DIVF4(regf64,INDIRF4(faddr))",
/* 207 */	"regf64: DIVF8(regf64,regf64)",
/* 208 */	"regf64: DIVF8(regf64,INDIRF8(faddr))",
/* 209 */	"reg: MODI2(reg,reg)",
/* 210 */	"reg: MODU2(reg,reg)",
/* 211 */	"reg32: MODI4(reg32,reg32)",
/* 212 */	"reg32: MODI4(reg32,INDIRI4(faddr))",
/* 213 */	"reg32: MODU4(reg32,reg32)",
/* 214 */	"reg32: MODU4(reg32,INDIRU4(faddr))",
/* 215 */	"reg: NEGI2(reg)",
/* 216 */	"reg32: NEGI4(reg32)",
/* 217 */	"reg32: NEGI4(INDIRI4(faddr))",
/* 218 */	"regf64: NEGF4(regf64)",
/* 219 */	"regf64: NEGF4(INDIRF4(faddr))",
/* 220 */	"regf64: NEGF8(regf64)",
/* 221 */	"regf64: NEGF8(INDIRF8(faddr))",
/* 222 */	"reg: LSHI2(reg,c1)",
/* 223 */	"reg: LSHI2(reg,c2)",
/* 224 */	"reg: LSHI2(reg,creg)",
/* 225 */	"reg: LSHI2(reg,reg)",
/* 226 */	"reg: LSHU2(reg,c1)",
/* 227 */	"reg: LSHU2(reg,c2)",
/* 228 */	"reg: LSHU2(reg,creg)",
/* 229 */	"reg: LSHU2(reg,reg)",
/* 230 */	"reg32: LSHI4(reg32,creg)",
/* 231 */	"reg32: LSHI4(reg32,reg)",
/* 232 */	"reg32: LSHU4(reg32,creg)",
/* 233 */	"reg32: LSHU4(reg32,reg)",
/* 234 */	"reg: RSHI2(reg,c1)",
/* 235 */	"reg: RSHI2(reg,c2)",
/* 236 */	"reg: RSHI2(reg,creg)",
/* 237 */	"reg: RSHI2(reg,reg)",
/* 238 */	"reg: RSHU2(reg,c1)",
/* 239 */	"reg: RSHU2(reg,c2)",
/* 240 */	"reg: RSHU2(reg,creg)",
/* 241 */	"reg: RSHU2(reg,reg)",
/* 242 */	"reg32: RSHI4(reg32,creg)",
/* 243 */	"reg32: RSHI4(reg32,reg)",
/* 244 */	"reg32: RSHU4(reg32,creg)",
/* 245 */	"reg32: RSHU4(reg32,reg)",
/* 246 */	"stmt: EQI2(reg,reg)",
/* 247 */	"stmt: EQI2(reg,con)",
/* 248 */	"stmt: EQI2(reg,dref16)",
/* 249 */	"stmt: EQI2(BANDI2(reg,con),c0)",
/* 250 */	"stmt: EQU2(reg,reg)",
/* 251 */	"stmt: EQU2(reg,con)",
/* 252 */	"stmt: EQU2(reg,dref16)",
/* 253 */	"stmt: EQU2(BANDU2(reg,con),c0)",
/* 254 */	"stmt: EQI2(reg8,reg8)",
/* 255 */	"stmt: EQI2(CVII2(reg8),CVII2(reg8))",
/* 256 */	"stmt: EQI2(reg8,con)",
/* 257 */	"stmt: EQI2(CVII2(reg8),con)",
/* 258 */	"stmt: EQI2(CVUI2(reg8),con)",
/* 259 */	"stmt: EQI2(reg8,CVII2(dref8))",
/* 260 */	"stmt: EQI2(CVII2(reg8),CVII2(dref8))",
/* 261 */	"stmt: EQI2(reg8,CVII2(reg8))",
/* 262 */	"stmt: EQI2(BANDI2(reg8,con),c0)",
/* 263 */	"stmt: EQI2(BANDI2(CVII2(reg8),con),c0)",
/* 264 */	"stmt: EQI2(BANDI2(CVUI2(reg8),con),c0)",
/* 265 */	"stmt: EQU2(reg8,reg8)",
/* 266 */	"stmt: EQU2(reg8,con)",
/* 267 */	"stmt: EQU2(CVUU2(reg8),con)",
/* 268 */	"stmt: EQU2(reg8,CVUU2(dref8))",
/* 269 */	"stmt: EQU2(CVUU2(reg8),CVUU2(dref8))",
/* 270 */	"stmt: EQU2(reg8,CVUU2(reg8))",
/* 271 */	"stmt: EQU2(BANDU2(reg8,con),c0)",
/* 272 */	"stmt: EQI4(reg32,reg32)",
/* 273 */	"stmt: EQI4(reg32,INDIRI4(faddr))",
/* 274 */	"stmt: EQI4(mem,mem)",
/* 275 */	"stmt: EQU4(reg32,reg32)",
/* 276 */	"stmt: EQU4(reg32,INDIRU4(faddr))",
/* 277 */	"stmt: EQU4(mem,mem)",
/* 278 */	"stmt: EQF4(regf64,regf64)",
/* 279 */	"stmt: EQF4(regf64,INDIRF4(faddr))",
/* 280 */	"stmt: EQF4(mem,mem)",
/* 281 */	"stmt: EQF8(regf64,regf64)",
/* 282 */	"stmt: EQF8(regf64,INDIRF8(faddr))",
/* 283 */	"stmt: EQF8(mem,mem)",
/* 284 */	"stmt: NEI2(reg,reg)",
/* 285 */	"stmt: NEI2(reg,con)",
/* 286 */	"stmt: NEI2(reg,c0)",
/* 287 */	"stmt: NEI2(reg,dref16)",
/* 288 */	"stmt: NEI2(BANDI2(reg,con),c0)",
/* 289 */	"stmt: NEU2(reg,reg)",
/* 290 */	"stmt: NEU2(reg,con)",
/* 291 */	"stmt: NEU2(reg,c0)",
/* 292 */	"stmt: NEU2(reg,dref16)",
/* 293 */	"stmt: NEU2(BANDI2(reg,con),c0)",
/* 294 */	"stmt: NEI2(reg8,reg8)",
/* 295 */	"stmt: NEI2(CVII2(reg8),CVII2(reg8))",
/* 296 */	"stmt: NEI2(reg8,con)",
/* 297 */	"stmt: NEI2(CVII2(reg8),con)",
/* 298 */	"stmt: NEI2(CVUI2(reg8),con)",
/* 299 */	"stmt: NEI2(reg8,CVII2(dref8))",
/* 300 */	"stmt: NEI2(CVII2(reg8),CVII2(dref8))",
/* 301 */	"stmt: NEI2(reg8,CVII2(reg8))",
/* 302 */	"stmt: NEI2(BANDI2(reg8,con),c0)",
/* 303 */	"stmt: NEI2(BANDI2(CVII2(reg8),con),c0)",
/* 304 */	"stmt: NEI2(BANDI2(CVUI2(reg8),con),c0)",
/* 305 */	"stmt: NEU2(reg8,reg8)",
/* 306 */	"stmt: NEU2(reg8,con)",
/* 307 */	"stmt: NEU2(CVUU2(reg8),con)",
/* 308 */	"stmt: NEU2(reg8,CVUU2(dref8))",
/* 309 */	"stmt: NEU2(CVUU2(reg8),CVUU2(dref8))",
/* 310 */	"stmt: NEU2(reg8,CVUU2(reg8))",
/* 311 */	"stmt: NEU2(BANDU2(reg8,con),c0)",
/* 312 */	"stmt: NEI4(reg32,reg32)",
/* 313 */	"stmt: NEI4(reg32,INDIRU4(faddr))",
/* 314 */	"stmt: NEI4(mem,mem)",
/* 315 */	"stmt: NEU4(reg32,reg32)",
/* 316 */	"stmt: NEU4(reg32,INDIRU4(faddr))",
/* 317 */	"stmt: NEU4(mem,mem)",
/* 318 */	"stmt: NEF4(regf64,regf64)",
/* 319 */	"stmt: NEF4(regf64,INDIRF4(faddr))",
/* 320 */	"stmt: NEF4(mem,mem)",
/* 321 */	"stmt: NEF8(regf64,regf64)",
/* 322 */	"stmt: NEF8(regf64,INDIRF8(faddr))",
/* 323 */	"stmt: NEF8(mem,mem)",
/* 324 */	"stmt: LTI2(reg,reg)",
/* 325 */	"stmt: LTI2(reg,con)",
/* 326 */	"stmt: LTI2(reg,dref16)",
/* 327 */	"stmt: LTU2(reg,reg)",
/* 328 */	"stmt: LTU2(reg,con)",
/* 329 */	"stmt: LTU2(reg,dref16)",
/* 330 */	"stmt: LTI2(reg8,reg8)",
/* 331 */	"stmt: LTI2(CVII2(reg8),CVII2(reg8))",
/* 332 */	"stmt: LTI2(reg8,con)",
/* 333 */	"stmt: LTI2(CVII2(reg8),con)",
/* 334 */	"stmt: LTI2(CVUI2(reg8),con)",
/* 335 */	"stmt: LTI2(reg8,CVII2(dref8))",
/* 336 */	"stmt: LTI2(CVII2(reg8),CVII2(dref8))",
/* 337 */	"stmt: LTI2(reg8,CVII2(reg8))",
/* 338 */	"stmt: LTU2(reg8,reg8)",
/* 339 */	"stmt: LTU2(reg8,con)",
/* 340 */	"stmt: LTU2(reg8,CVUU2(dref8))",
/* 341 */	"stmt: LTU2(CVUU2(reg8),CVUU2(dref8))",
/* 342 */	"stmt: LTU2(reg8,CVUU2(reg8))",
/* 343 */	"stmt: LTI4(reg32,reg32)",
/* 344 */	"stmt: LTI4(reg32,INDIRI4(faddr))",
/* 345 */	"stmt: LTI4(mem,mem)",
/* 346 */	"stmt: LTU4(reg32,reg32)",
/* 347 */	"stmt: LTU4(reg32,INDIRU4(faddr))",
/* 348 */	"stmt: LTU4(mem,mem)",
/* 349 */	"stmt: LTF4(regf64,regf64)",
/* 350 */	"stmt: LTF4(regf64,INDIRF4(faddr))",
/* 351 */	"stmt: LTF4(mem,mem)",
/* 352 */	"stmt: LTF8(regf64,regf64)",
/* 353 */	"stmt: LTF8(regf64,INDIRF8(faddr))",
/* 354 */	"stmt: LTF8(mem,mem)",
/* 355 */	"stmt: LEI2(reg,reg)",
/* 356 */	"stmt: LEI2(reg,con)",
/* 357 */	"stmt: LEI2(reg,dref16)",
/* 358 */	"stmt: LEU2(reg,reg)",
/* 359 */	"stmt: LEU2(reg,con)",
/* 360 */	"stmt: LEU2(reg,dref16)",
/* 361 */	"stmt: LEI2(reg8,reg8)",
/* 362 */	"stmt: LEI2(CVII2(reg8),CVII2(reg8))",
/* 363 */	"stmt: LEI2(reg8,con)",
/* 364 */	"stmt: LEI2(CVII2(reg8),con)",
/* 365 */	"stmt: LEI2(reg8,CVII2(dref8))",
/* 366 */	"stmt: LEI2(CVII2(reg8),CVII2(dref8))",
/* 367 */	"stmt: LEI2(reg8,CVII2(reg8))",
/* 368 */	"stmt: LEU2(reg8,reg8)",
/* 369 */	"stmt: LEU2(reg8,con)",
/* 370 */	"stmt: LEU2(reg8,CVUU2(dref8))",
/* 371 */	"stmt: LEU2(CVUU2(reg8),CVUU2(dref8))",
/* 372 */	"stmt: LEU2(reg8,CVUU2(reg8))",
/* 373 */	"stmt: LEI4(reg32,reg32)",
/* 374 */	"stmt: LEI4(reg32,INDIRI4(faddr))",
/* 375 */	"stmt: LEI4(mem,mem)",
/* 376 */	"stmt: LEU4(reg32,reg32)",
/* 377 */	"stmt: LEU4(reg32,INDIRU4(faddr))",
/* 378 */	"stmt: LEU4(mem,mem)",
/* 379 */	"stmt: LEF4(regf64,regf64)",
/* 380 */	"stmt: LEF4(regf64,INDIRF4(faddr))",
/* 381 */	"stmt: LEF4(mem,mem)",
/* 382 */	"stmt: LEF8(regf64,regf64)",
/* 383 */	"stmt: LEF8(regf64,INDIRF8(faddr))",
/* 384 */	"stmt: LEF8(mem,mem)",
/* 385 */	"stmt: GTI2(reg,reg)",
/* 386 */	"stmt: GTI2(con,reg)",
/* 387 */	"stmt: GTI2(dref16,reg)",
/* 388 */	"stmt: GTU2(reg,reg)",
/* 389 */	"stmt: GTU2(con,reg)",
/* 390 */	"stmt: GTU2(dref16,reg)",
/* 391 */	"stmt: GTI2(reg8,reg8)",
/* 392 */	"stmt: GTI2(CVII2(reg8),CVII2(reg8))",
/* 393 */	"stmt: GTI2(con,reg8)",
/* 394 */	"stmt: GTI2(con,CVII2(reg8))",
/* 395 */	"stmt: GTI2(CVII2(dref8),reg8)",
/* 396 */	"stmt: GTI2(CVII2(dref8),CVII2(reg8))",
/* 397 */	"stmt: GTI2(CVII2(reg8),reg8)",
/* 398 */	"stmt: GTU2(reg8,reg8)",
/* 399 */	"stmt: GTU2(con,reg8)",
/* 400 */	"stmt: GTU2(CVUU2(dref8),reg8)",
/* 401 */	"stmt: GTU2(CVUU2(dref8),CVUU2(reg8))",
/* 402 */	"stmt: GTU2(CVUU2(reg8),reg8)",
/* 403 */	"stmt: GTI4(reg32,reg32)",
/* 404 */	"stmt: GTI4(INDIRI4(faddr),reg32)",
/* 405 */	"stmt: GTI4(mem,mem)",
/* 406 */	"stmt: GTU4(reg32,reg32)",
/* 407 */	"stmt: GTU4(INDIRU4(faddr),reg32)",
/* 408 */	"stmt: GTU4(mem,mem)",
/* 409 */	"stmt: GTF4(regf64,regf64)",
/* 410 */	"stmt: GTF4(INDIRF4(faddr),regf64)",
/* 411 */	"stmt: GTF4(mem,mem)",
/* 412 */	"stmt: GTF8(regf64,regf64)",
/* 413 */	"stmt: GTF8(INDIRF8(faddr),regf64)",
/* 414 */	"stmt: GTF8(mem,mem)",
/* 415 */	"stmt: GEI2(reg,reg)",
/* 416 */	"stmt: GEI2(con,reg)",
/* 417 */	"stmt: GEI2(dref16,reg)",
/* 418 */	"stmt: GEU2(reg,reg)",
/* 419 */	"stmt: GEU2(con,reg)",
/* 420 */	"stmt: GEU2(dref16,reg)",
/* 421 */	"stmt: GEI2(reg8,reg8)",
/* 422 */	"stmt: GEI2(CVII2(reg8),CVII2(reg8))",
/* 423 */	"stmt: GEI2(con,reg8)",
/* 424 */	"stmt: GEI2(con,CVII2(reg8))",
/* 425 */	"stmt: GEI2(CVII2(dref8),reg8)",
/* 426 */	"stmt: GEI2(CVII2(dref8),CVII2(reg8))",
/* 427 */	"stmt: GEI2(CVII2(reg8),reg8)",
/* 428 */	"stmt: GEU2(reg8,reg8)",
/* 429 */	"stmt: GEU2(con,reg8)",
/* 430 */	"stmt: GEU2(CVUU2(dref8),reg8)",
/* 431 */	"stmt: GEU2(CVUU2(dref8),CVUU2(reg8))",
/* 432 */	"stmt: GEU2(CVUU2(reg8),reg8)",
/* 433 */	"stmt: GEI4(reg32,reg32)",
/* 434 */	"stmt: GEI4(INDIRI4(faddr),reg32)",
/* 435 */	"stmt: GEI4(mem,mem)",
/* 436 */	"stmt: GEU4(reg32,reg32)",
/* 437 */	"stmt: GEU4(INDIRU4(faddr),reg32)",
/* 438 */	"stmt: GEU4(mem,mem)",
/* 439 */	"stmt: GEF4(regf64,regf64)",
/* 440 */	"stmt: GEF4(INDIRF4(faddr),regf64)",
/* 441 */	"stmt: GEF4(mem,mem)",
/* 442 */	"stmt: GEF8(regf64,regf64)",
/* 443 */	"stmt: GEF8(INDIRF8(faddr),regf64)",
/* 444 */	"stmt: GEF8(mem,mem)",
/* 445 */	"faddr: ADDRGP2",
/* 446 */	"faddr: ADDRGP2",
/* 447 */	"faddr: ADDRLP2",
/* 448 */	"faddr: ADDRFP2",
/* 449 */	"vaddr: ADDP2(reg,con)",
/* 450 */	"dref16: INDIRP2(addr)",
/* 451 */	"dref16: INDIRI2(addr)",
/* 452 */	"dref16: INDIRU2(addr)",
/* 453 */	"dref16: LOADU2(dref16)",
/* 454 */	"dref16: LOADI2(dref16)",
/* 455 */	"dref16: LOADP2(dref16)",
/* 456 */	"dref16: INDIRP2(reg)",
/* 457 */	"dref16: INDIRI2(reg)",
/* 458 */	"dref16: INDIRU2(reg)",
/* 459 */	"dref8: INDIRI1(addr)",
/* 460 */	"dref8: INDIRU1(addr)",
/* 461 */	"dref8: INDIRI1(reg)",
/* 462 */	"dref8: INDIRU1(reg)",
/* 463 */	"addr: faddr",
/* 464 */	"addr: vaddr",
/* 465 */	"stmt: reg",
/* 466 */	"stmt: LABELV",
/* 467 */	"reg: ADDRGP2",
/* 468 */	"reg: ADDRLP2",
/* 469 */	"reg: ADDRFP2",
/* 470 */	"reg8: INDIRI1(reg)",
/* 471 */	"reg: INDIRI1(reg)",
/* 472 */	"reg8: INDIRI1(addr)",
/* 473 */	"reg: INDIRI1(addr)",
/* 474 */	"reg8: INDIRU1(reg)",
/* 475 */	"reg: INDIRU1(reg)",
/* 476 */	"reg8: INDIRU1(addr)",
/* 477 */	"reg: INDIRU1(addr)",
/* 478 */	"reg: INDIRI2(reg)",
/* 479 */	"reg: INDIRI2(addr)",
/* 480 */	"reg: INDIRU2(reg)",
/* 481 */	"reg: INDIRU2(addr)",
/* 482 */	"reg: INDIRP2(reg)",
/* 483 */	"reg: INDIRP2(addr)",
/* 484 */	"reg32: INDIRI4(reg)",
/* 485 */	"reg32: INDIRI4(faddr)",
/* 486 */	"reg32: INDIRU4(reg)",
/* 487 */	"reg32: INDIRU4(faddr)",
/* 488 */	"regf64: INDIRF4(reg)",
/* 489 */	"regf64: INDIRF4(faddr)",
/* 490 */	"regf64: INDIRF8(reg)",
/* 491 */	"regf64: INDIRF8(faddr)",
/* 492 */	"ar: reg",
/* 493 */	"ar: ADDRGP2",
/* 494 */	"reg: CALLI2(ar)",
/* 495 */	"reg: CALLU2(ar)",
/* 496 */	"reg: CALLP2(ar)",
/* 497 */	"reg32: CALLI4(ar)",
/* 498 */	"reg: CALLI4(ar)",
/* 499 */	"reg32: CALLU4(ar)",
/* 500 */	"reg: CALLU4(ar)",
/* 501 */	"stmt: CALLV(ar)",
/* 502 */	"stmt: CALLB(ar,reg)",
/* 503 */	"regf64: CALLF4(ar)",
/* 504 */	"reg: CALLF4(ar)",
/* 505 */	"regf64: CALLF8(ar)",
/* 506 */	"stmt: CALLF8(ar)",
/* 507 */	"stmt: RETI2(reg)",
/* 508 */	"stmt: RETU2(reg)",
/* 509 */	"stmt: RETP2(reg)",
/* 510 */	"stmt: RETI4(reg32)",
/* 511 */	"stmt: RETU4(reg32)",
/* 512 */	"stmt: RETF4(regf64)",
/* 513 */	"stmt: RETF8(regf64)",
/* 514 */	"stmt: RETV(reg)",
/* 515 */	"stmt: ARGP2(reg)",
/* 516 */	"stmt: ARGI2(reg)",
/* 517 */	"stmt: ARGU2(reg)",
/* 518 */	"stmt: ARGI4(reg32)",
/* 519 */	"stmt: ARGU4(reg32)",
/* 520 */	"stmt: ARGF4(regf64)",
/* 521 */	"stmt: ARGF8(regf64)",
/* 522 */	"stmt: JUMPV(ar)",
/* 523 */	"mem: INDIRI4(faddr)",
/* 524 */	"mem: INDIRU4(faddr)",
/* 525 */	"mem: INDIRF4(faddr)",
/* 526 */	"mem: INDIRF8(faddr)",
/* 527 */	"mem: reg32",
/* 528 */	"mem: regf64",
/* 529 */	"stmt: ASGNI1(reg,reg8)",
/* 530 */	"stmt: ASGNI1(addr,reg8)",
/* 531 */	"stmt: ASGNU1(reg,reg8)",
/* 532 */	"stmt: ASGNU1(addr,reg8)",
/* 533 */	"stmt: ASGNI2(reg,reg)",
/* 534 */	"stmt: ASGNI2(addr,reg)",
/* 535 */	"stmt: ASGNU2(reg,reg)",
/* 536 */	"stmt: ASGNU2(addr,reg)",
/* 537 */	"stmt: ASGNP2(reg,reg)",
/* 538 */	"stmt: ASGNP2(addr,reg)",
/* 539 */	"stmt: ASGNB(reg,INDIRB(reg))",
/* 540 */	"stmt: ARGB(INDIRB(reg))",
/* 541 */	"stmt: ASGNI4(reg,reg32)",
/* 542 */	"stmt: ASGNI4(faddr,reg32)",
/* 543 */	"stmt: ASGNI4(faddr,INDIRI4(faddr))",
/* 544 */	"stmt: ASGNU4(reg,reg32)",
/* 545 */	"stmt: ASGNU4(faddr,reg32)",
/* 546 */	"stmt: ASGNU4(faddr,INDIRU4(faddr))",
/* 547 */	"stmt: ASGNF4(reg,regf64)",
/* 548 */	"stmt: ASGNF4(faddr,regf64)",
/* 549 */	"stmt: ASGNF4(faddr,INDIRF4(faddr))",
/* 550 */	"stmt: ASGNF8(reg,regf64)",
/* 551 */	"stmt: ASGNF8(faddr,regf64)",
/* 552 */	"stmt: ASGNF8(faddr,INDIRF8(faddr))",
/* 553 */	"stmt: ASGNI4(faddr,ADDI4(mem,mem))",
/* 554 */	"stmt: ASGNI4(faddr,ADDI4(mem,mem))",
/* 555 */	"stmt: ASGNU4(faddr,ADDI4(mem,mem))",
/* 556 */	"stmt: ASGNU4(faddr,ADDI4(mem,mem))",
/* 557 */	"stmt: ASGNF4(faddr,ADDF4(mem,mem))",
/* 558 */	"stmt: ASGNF4(faddr,ADDF4(mem,mem))",
/* 559 */	"stmt: ASGNF8(faddr,ADDF8(mem,mem))",
/* 560 */	"stmt: ASGNF8(faddr,ADDF8(mem,mem))",
/* 561 */	"stmt: ASGNI4(faddr,ADDI4(mem,mem))",
/* 562 */	"stmt: ASGNU4(faddr,ADDU4(mem,mem))",
/* 563 */	"stmt: ASGNF4(faddr,ADDF4(mem,mem))",
/* 564 */	"stmt: ASGNF8(faddr,ADDF8(mem,mem))",
/* 565 */	"stmt: ASGNI4(faddr,SUBI4(mem,mem))",
/* 566 */	"stmt: ASGNU4(faddr,SUBI4(mem,mem))",
/* 567 */	"stmt: ASGNF4(faddr,SUBF4(mem,mem))",
/* 568 */	"stmt: ASGNF8(faddr,SUBF8(mem,mem))",
/* 569 */	"stmt: ASGNI4(faddr,SUBI4(mem,mem))",
/* 570 */	"stmt: ASGNU4(faddr,SUBU4(mem,mem))",
/* 571 */	"stmt: ASGNF4(faddr,SUBF4(mem,mem))",
/* 572 */	"stmt: ASGNF8(faddr,SUBF8(mem,mem))",
/* 573 */	"stmt: ASGNI4(faddr,MULI4(mem,mem))",
/* 574 */	"stmt: ASGNI4(faddr,MULI4(mem,mem))",
/* 575 */	"stmt: ASGNU4(faddr,MULI4(mem,mem))",
/* 576 */	"stmt: ASGNU4(faddr,MULI4(mem,mem))",
/* 577 */	"stmt: ASGNF4(faddr,MULF4(mem,mem))",
/* 578 */	"stmt: ASGNF4(faddr,MULF4(mem,mem))",
/* 579 */	"stmt: ASGNF8(faddr,MULF8(mem,mem))",
/* 580 */	"stmt: ASGNF8(faddr,MULF8(mem,mem))",
/* 581 */	"stmt: ASGNI4(faddr,MULI4(mem,mem))",
/* 582 */	"stmt: ASGNU4(faddr,MULU4(mem,mem))",
/* 583 */	"stmt: ASGNF4(faddr,MULF4(mem,mem))",
/* 584 */	"stmt: ASGNF8(faddr,MULF8(mem,mem))",
/* 585 */	"stmt: ASGNI4(faddr,DIVI4(mem,mem))",
/* 586 */	"stmt: ASGNU4(faddr,DIVI4(mem,mem))",
/* 587 */	"stmt: ASGNF4(faddr,DIVF4(mem,mem))",
/* 588 */	"stmt: ASGNF8(faddr,DIVF8(mem,mem))",
/* 589 */	"stmt: ASGNI4(faddr,DIVI4(mem,mem))",
/* 590 */	"stmt: ASGNU4(faddr,DIVU4(mem,mem))",
/* 591 */	"stmt: ASGNF4(faddr,DIVF4(mem,mem))",
/* 592 */	"stmt: ASGNF8(faddr,DIVF8(mem,mem))",
/* 593 */	"stmt: ASGNI4(faddr,BANDI4(mem,mem))",
/* 594 */	"stmt: ASGNI4(faddr,BANDI4(mem,mem))",
/* 595 */	"stmt: ASGNU4(faddr,BANDU4(mem,mem))",
/* 596 */	"stmt: ASGNU4(faddr,BANDU4(mem,mem))",
/* 597 */	"stmt: ASGNI4(faddr,BANDI4(mem,mem))",
/* 598 */	"stmt: ASGNI4(faddr,BANDU4(mem,mem))",
/* 599 */	"stmt: ASGNI4(faddr,BORI4(mem,mem))",
/* 600 */	"stmt: ASGNI4(faddr,BORI4(mem,mem))",
/* 601 */	"stmt: ASGNU4(faddr,BORU4(mem,mem))",
/* 602 */	"stmt: ASGNU4(faddr,BORU4(mem,mem))",
/* 603 */	"stmt: ASGNI4(faddr,BORI4(mem,mem))",
/* 604 */	"stmt: ASGNI4(faddr,BORU4(mem,mem))",
/* 605 */	"stmt: ASGNI4(faddr,BXORI4(mem,mem))",
/* 606 */	"stmt: ASGNI4(faddr,BXORI4(mem,mem))",
/* 607 */	"stmt: ASGNU4(faddr,BXORU4(mem,mem))",
/* 608 */	"stmt: ASGNU4(faddr,BXORU4(mem,mem))",
/* 609 */	"stmt: ASGNI4(faddr,BXORI4(mem,mem))",
/* 610 */	"stmt: ASGNI4(faddr,BXORU4(mem,mem))",
};

static short _decode_stmt[] = {
	0,
	25,
	26,
	27,
	28,
	29,
	30,
	31,
	32,
	33,
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
	278,
	279,
	280,
	281,
	282,
	283,
	284,
	285,
	286,
	287,
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
	304,
	305,
	306,
	307,
	308,
	309,
	310,
	311,
	312,
	313,
	314,
	315,
	316,
	317,
	318,
	319,
	320,
	321,
	322,
	323,
	324,
	325,
	326,
	327,
	328,
	329,
	330,
	331,
	332,
	333,
	334,
	335,
	336,
	337,
	338,
	339,
	340,
	341,
	342,
	343,
	344,
	345,
	346,
	347,
	348,
	349,
	350,
	351,
	352,
	353,
	354,
	355,
	356,
	357,
	358,
	359,
	360,
	361,
	362,
	363,
	364,
	365,
	366,
	367,
	368,
	369,
	370,
	371,
	372,
	373,
	374,
	375,
	376,
	377,
	378,
	379,
	380,
	381,
	382,
	383,
	384,
	385,
	386,
	387,
	388,
	389,
	390,
	391,
	392,
	393,
	394,
	395,
	396,
	397,
	398,
	399,
	400,
	401,
	402,
	403,
	404,
	405,
	406,
	407,
	408,
	409,
	410,
	411,
	412,
	413,
	414,
	415,
	416,
	417,
	418,
	419,
	420,
	421,
	422,
	423,
	424,
	425,
	426,
	427,
	428,
	429,
	430,
	431,
	432,
	433,
	434,
	435,
	436,
	437,
	438,
	439,
	440,
	441,
	442,
	443,
	444,
	465,
	466,
	501,
	502,
	506,
	507,
	508,
	509,
	510,
	511,
	512,
	513,
	514,
	515,
	516,
	517,
	518,
	519,
	520,
	521,
	522,
	529,
	530,
	531,
	532,
	533,
	534,
	535,
	536,
	537,
	538,
	539,
	540,
	541,
	542,
	543,
	544,
	545,
	546,
	547,
	548,
	549,
	550,
	551,
	552,
	553,
	554,
	555,
	556,
	557,
	558,
	559,
	560,
	561,
	562,
	563,
	564,
	565,
	566,
	567,
	568,
	569,
	570,
	571,
	572,
	573,
	574,
	575,
	576,
	577,
	578,
	579,
	580,
	581,
	582,
	583,
	584,
	585,
	586,
	587,
	588,
	589,
	590,
	591,
	592,
	593,
	594,
	595,
	596,
	597,
	598,
	599,
	600,
	601,
	602,
	603,
	604,
	605,
	606,
	607,
	608,
	609,
	610,
};

static short _decode_reg8[] = {
	0,
	1,
	2,
	3,
	4,
	5,
	6,
	16,
	17,
	44,
	45,
	46,
	47,
	48,
	49,
	50,
	51,
	56,
	57,
	58,
	59,
	69,
	102,
	103,
	104,
	105,
	106,
	107,
	108,
	109,
	110,
	128,
	129,
	130,
	131,
	132,
	133,
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
	171,
	172,
	173,
	174,
	175,
	176,
	470,
	472,
	474,
	476,
};

static short _decode_reg[] = {
	0,
	7,
	8,
	9,
	10,
	11,
	18,
	19,
	20,
	42,
	43,
	52,
	53,
	54,
	55,
	60,
	61,
	62,
	63,
	68,
	89,
	90,
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
	119,
	120,
	121,
	122,
	123,
	124,
	125,
	126,
	127,
	142,
	143,
	144,
	145,
	146,
	147,
	163,
	164,
	165,
	166,
	167,
	168,
	169,
	170,
	183,
	184,
	189,
	190,
	199,
	200,
	209,
	210,
	215,
	222,
	223,
	224,
	225,
	226,
	227,
	228,
	229,
	234,
	235,
	236,
	237,
	238,
	239,
	240,
	241,
	467,
	468,
	469,
	471,
	473,
	475,
	477,
	478,
	479,
	480,
	481,
	482,
	483,
	494,
	495,
	496,
	498,
	500,
	504,
};

static short _decode_reg32[] = {
	0,
	12,
	13,
	21,
	22,
	40,
	41,
	64,
	65,
	66,
	67,
	111,
	112,
	113,
	114,
	134,
	135,
	136,
	137,
	158,
	159,
	160,
	161,
	162,
	177,
	178,
	179,
	180,
	181,
	182,
	185,
	186,
	187,
	188,
	191,
	192,
	193,
	194,
	201,
	202,
	203,
	204,
	211,
	212,
	213,
	214,
	216,
	217,
	230,
	231,
	232,
	233,
	242,
	243,
	244,
	245,
	484,
	485,
	486,
	487,
	497,
	499,
};

static short _decode_regf64[] = {
	0,
	14,
	15,
	23,
	24,
	34,
	35,
	36,
	37,
	38,
	39,
	115,
	116,
	117,
	118,
	138,
	139,
	140,
	141,
	195,
	196,
	197,
	198,
	205,
	206,
	207,
	208,
	218,
	219,
	220,
	221,
	488,
	489,
	490,
	491,
	503,
	505,
};

static short _decode_con[] = {
	0,
	83,
	84,
	85,
	86,
	87,
	88,
};

static short _decode_c0[] = {
	0,
	70,
	73,
};

static short _decode_c1[] = {
	0,
	71,
	74,
};

static short _decode_c2[] = {
	0,
	72,
	75,
};

static short _decode_con1[] = {
	0,
	76,
	77,
};

static short _decode_con2[] = {
	0,
	78,
	79,
};

static short _decode_con4[] = {
	0,
	80,
	81,
	82,
};

static short _decode_creg[] = {
	0,
	91,
};

static short _decode_dref16[] = {
	0,
	450,
	451,
	452,
	453,
	454,
	455,
	456,
	457,
	458,
};

static short _decode_dref8[] = {
	0,
	459,
	460,
	461,
	462,
};

static short _decode_faddr[] = {
	0,
	445,
	446,
	447,
	448,
};

static short _decode_mem[] = {
	0,
	523,
	524,
	525,
	526,
	527,
	528,
};

static short _decode_vaddr[] = {
	0,
	449,
};

static short _decode_addr[] = {
	0,
	463,
	464,
};

static short _decode_ar[] = {
	0,
	492,
	493,
};

static int _rule(void *state, int goalnt) {
	if (goalnt < 1 || goalnt > 20)
		fatal("_rule", "Bad goal nonterminal %d\n", goalnt);
	if (!state)
		return 0;
	switch (goalnt) {
	case _stmt_NT:	return _decode_stmt[((struct _state *)state)->rule._stmt];
	case _reg8_NT:	return _decode_reg8[((struct _state *)state)->rule._reg8];
	case _reg_NT:	return _decode_reg[((struct _state *)state)->rule._reg];
	case _reg32_NT:	return _decode_reg32[((struct _state *)state)->rule._reg32];
	case _regf64_NT:	return _decode_regf64[((struct _state *)state)->rule._regf64];
	case _con_NT:	return _decode_con[((struct _state *)state)->rule._con];
	case _c0_NT:	return _decode_c0[((struct _state *)state)->rule._c0];
	case _c1_NT:	return _decode_c1[((struct _state *)state)->rule._c1];
	case _c2_NT:	return _decode_c2[((struct _state *)state)->rule._c2];
	case _con1_NT:	return _decode_con1[((struct _state *)state)->rule._con1];
	case _con2_NT:	return _decode_con2[((struct _state *)state)->rule._con2];
	case _con4_NT:	return _decode_con4[((struct _state *)state)->rule._con4];
	case _creg_NT:	return _decode_creg[((struct _state *)state)->rule._creg];
	case _dref16_NT:	return _decode_dref16[((struct _state *)state)->rule._dref16];
	case _dref8_NT:	return _decode_dref8[((struct _state *)state)->rule._dref8];
	case _faddr_NT:	return _decode_faddr[((struct _state *)state)->rule._faddr];
	case _mem_NT:	return _decode_mem[((struct _state *)state)->rule._mem];
	case _vaddr_NT:	return _decode_vaddr[((struct _state *)state)->rule._vaddr];
	case _addr_NT:	return _decode_addr[((struct _state *)state)->rule._addr];
	case _ar_NT:	return _decode_ar[((struct _state *)state)->rule._ar];
	default:
		fatal("_rule", "Bad goal nonterminal %d\n", goalnt);
		return 0;
	}
}

static void _closure_reg(NODEPTR_TYPE, int);
static void _closure_reg32(NODEPTR_TYPE, int);
static void _closure_regf64(NODEPTR_TYPE, int);
static void _closure_con(NODEPTR_TYPE, int);
static void _closure_c0(NODEPTR_TYPE, int);
static void _closure_c1(NODEPTR_TYPE, int);
static void _closure_c2(NODEPTR_TYPE, int);
static void _closure_con1(NODEPTR_TYPE, int);
static void _closure_con2(NODEPTR_TYPE, int);
static void _closure_con4(NODEPTR_TYPE, int);
static void _closure_faddr(NODEPTR_TYPE, int);
static void _closure_vaddr(NODEPTR_TYPE, int);

static void _closure_reg(NODEPTR_TYPE a, int c) {
	struct _state *p = STATE_LABEL(a);
	if (c + 0 < p->cost[_ar_NT]) {
		p->cost[_ar_NT] = c + 0;
		p->rule._ar = 1;
	}
	if (c + 0 < p->cost[_stmt_NT]) {
		p->cost[_stmt_NT] = c + 0;
		p->rule._stmt = 209;
	}
}

static void _closure_reg32(NODEPTR_TYPE a, int c) {
	struct _state *p = STATE_LABEL(a);
	if (c + 0 < p->cost[_mem_NT]) {
		p->cost[_mem_NT] = c + 0;
		p->rule._mem = 5;
	}
}

static void _closure_regf64(NODEPTR_TYPE a, int c) {
	struct _state *p = STATE_LABEL(a);
	if (c + 0 < p->cost[_mem_NT]) {
		p->cost[_mem_NT] = c + 0;
		p->rule._mem = 6;
	}
}

static void _closure_con(NODEPTR_TYPE a, int c) {
	struct _state *p = STATE_LABEL(a);
	if (c + 1 < p->cost[_creg_NT]) {
		p->cost[_creg_NT] = c + 1;
		p->rule._creg = 1;
	}
	if (c + 1 < p->cost[_reg8_NT]) {
		p->cost[_reg8_NT] = c + 1;
		p->rule._reg8 = 21;
	}
	if (c + 1 < p->cost[_reg_NT]) {
		p->cost[_reg_NT] = c + 1;
		p->rule._reg = 19;
		_closure_reg(a, c + 1);
	}
}

static void _closure_c0(NODEPTR_TYPE a, int c) {
	struct _state *p = STATE_LABEL(a);
	if (c + 0 < p->cost[_con_NT]) {
		p->cost[_con_NT] = c + 0;
		p->rule._con = 4;
		_closure_con(a, c + 0);
	}
}

static void _closure_c1(NODEPTR_TYPE a, int c) {
	struct _state *p = STATE_LABEL(a);
	if (c + 0 < p->cost[_con_NT]) {
		p->cost[_con_NT] = c + 0;
		p->rule._con = 5;
		_closure_con(a, c + 0);
	}
}

static void _closure_c2(NODEPTR_TYPE a, int c) {
	struct _state *p = STATE_LABEL(a);
	if (c + 0 < p->cost[_con_NT]) {
		p->cost[_con_NT] = c + 0;
		p->rule._con = 6;
		_closure_con(a, c + 0);
	}
}

static void _closure_con1(NODEPTR_TYPE a, int c) {
	struct _state *p = STATE_LABEL(a);
	if (c + 1 < p->cost[_reg_NT]) {
		p->cost[_reg_NT] = c + 1;
		p->rule._reg = 20;
		_closure_reg(a, c + 1);
	}
	if (c + 0 < p->cost[_con_NT]) {
		p->cost[_con_NT] = c + 0;
		p->rule._con = 1;
		_closure_con(a, c + 0);
	}
}

static void _closure_con2(NODEPTR_TYPE a, int c) {
	struct _state *p = STATE_LABEL(a);
	if (c + 1 < p->cost[_reg_NT]) {
		p->cost[_reg_NT] = c + 1;
		p->rule._reg = 21;
		_closure_reg(a, c + 1);
	}
	if (c + 0 < p->cost[_con_NT]) {
		p->cost[_con_NT] = c + 0;
		p->rule._con = 2;
		_closure_con(a, c + 0);
	}
}

static void _closure_con4(NODEPTR_TYPE a, int c) {
	struct _state *p = STATE_LABEL(a);
	if (c + 0 < p->cost[_con_NT]) {
		p->cost[_con_NT] = c + 0;
		p->rule._con = 3;
		_closure_con(a, c + 0);
	}
}

static void _closure_faddr(NODEPTR_TYPE a, int c) {
	struct _state *p = STATE_LABEL(a);
	if (c + 0 < p->cost[_addr_NT]) {
		p->cost[_addr_NT] = c + 0;
		p->rule._addr = 1;
	}
}

static void _closure_vaddr(NODEPTR_TYPE a, int c) {
	struct _state *p = STATE_LABEL(a);
	if (c + 0 < p->cost[_addr_NT]) {
		p->cost[_addr_NT] = c + 0;
		p->rule._addr = 2;
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
	p->cost[12] =
	p->cost[13] =
	p->cost[14] =
	p->cost[15] =
	p->cost[16] =
	p->cost[17] =
	p->cost[18] =
	p->cost[19] =
	p->cost[20] =
		0x7fff;
	switch (OP_LABEL(a)) {
	case 41: /* ARGB */
		_label(LEFT_CHILD(a));
		if (	/* stmt: ARGB(INDIRB(reg)) */
			LEFT_CHILD(a)->op == 73 /* INDIRB */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg_NT] + 0;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 241;
			}
		}
		break;
	case 57: /* ASGNB */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		if (	/* stmt: ASGNB(reg,INDIRB(reg)) */
			RIGHT_CHILD(a)->op == 73 /* INDIRB */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_reg_NT] + 0;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 240;
			}
		}
		break;
	case 73: /* INDIRB */
		_label(LEFT_CHILD(a));
		break;
	case 216: /* CALLV */
		_label(LEFT_CHILD(a));
		/* stmt: CALLV(ar) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_ar_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 211;
		}
		break;
	case 217: /* CALLB */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: CALLB(ar,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_ar_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 212;
		}
		break;
	case 233: /* LOADB */
		break;
	case 248: /* RETV */
		_label(LEFT_CHILD(a));
		/* stmt: RETV(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 221;
		}
		break;
	case 584: /* JUMPV */
		_label(LEFT_CHILD(a));
		/* stmt: JUMPV(ar) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_ar_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 229;
		}
		break;
	case 600: /* LABELV */
		/* stmt: LABELV */
		if (0 + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = 0 + 0;
			p->rule._stmt = 210;
		}
		break;
	case 711: /* VREGP */
		break;
	case 1045: /* CNSTI1 */
		/* con1: CNSTI1 */
		if (0 + 0 < p->cost[_con1_NT]) {
			p->cost[_con1_NT] = 0 + 0;
			p->rule._con1 = 1;
			_closure_con1(a, 0 + 0);
		}
		break;
	case 1046: /* CNSTU1 */
		/* con1: CNSTU1 */
		if (0 + 0 < p->cost[_con1_NT]) {
			p->cost[_con1_NT] = 0 + 0;
			p->rule._con1 = 2;
			_closure_con1(a, 0 + 0);
		}
		break;
	case 1077: /* ASGNI1 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		if (	/* stmt: ASGNI1(VREGP,reg8) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			c = ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg8_NT] + 0;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 1;
			}
		}
		/* stmt: ASGNI1(reg,reg8) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg8_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 230;
		}
		/* stmt: ASGNI1(addr,reg8) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg8_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 231;
		}
		break;
	case 1078: /* ASGNU1 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		if (	/* stmt: ASGNU1(VREGP,reg8) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			c = ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg8_NT] + 0;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 2;
			}
		}
		/* stmt: ASGNU1(reg,reg8) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg8_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 232;
		}
		/* stmt: ASGNU1(addr,reg8) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg8_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 233;
		}
		break;
	case 1093: /* INDIRI1 */
		_label(LEFT_CHILD(a));
		if (	/* reg8: INDIRI1(VREGP) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			if (mayrecalc(a)) {
				struct _state *q = a->syms[RX]->u.t.cse->x.state;
				if (q->cost[_stmt_NT] == 0) {
					p->cost[_stmt_NT] = 0;
					p->rule._stmt = q->rule._stmt;
				}
				if (q->cost[_reg8_NT] == 0) {
					p->cost[_reg8_NT] = 0;
					p->rule._reg8 = q->rule._reg8;
				}
				if (q->cost[_reg_NT] == 0) {
					p->cost[_reg_NT] = 0;
					p->rule._reg = q->rule._reg;
				}
				if (q->cost[_reg32_NT] == 0) {
					p->cost[_reg32_NT] = 0;
					p->rule._reg32 = q->rule._reg32;
				}
				if (q->cost[_regf64_NT] == 0) {
					p->cost[_regf64_NT] = 0;
					p->rule._regf64 = q->rule._regf64;
				}
				if (q->cost[_con_NT] == 0) {
					p->cost[_con_NT] = 0;
					p->rule._con = q->rule._con;
				}
				if (q->cost[_c0_NT] == 0) {
					p->cost[_c0_NT] = 0;
					p->rule._c0 = q->rule._c0;
				}
				if (q->cost[_c1_NT] == 0) {
					p->cost[_c1_NT] = 0;
					p->rule._c1 = q->rule._c1;
				}
				if (q->cost[_c2_NT] == 0) {
					p->cost[_c2_NT] = 0;
					p->rule._c2 = q->rule._c2;
				}
				if (q->cost[_con1_NT] == 0) {
					p->cost[_con1_NT] = 0;
					p->rule._con1 = q->rule._con1;
				}
				if (q->cost[_con2_NT] == 0) {
					p->cost[_con2_NT] = 0;
					p->rule._con2 = q->rule._con2;
				}
				if (q->cost[_con4_NT] == 0) {
					p->cost[_con4_NT] = 0;
					p->rule._con4 = q->rule._con4;
				}
				if (q->cost[_creg_NT] == 0) {
					p->cost[_creg_NT] = 0;
					p->rule._creg = q->rule._creg;
				}
				if (q->cost[_dref16_NT] == 0) {
					p->cost[_dref16_NT] = 0;
					p->rule._dref16 = q->rule._dref16;
				}
				if (q->cost[_dref8_NT] == 0) {
					p->cost[_dref8_NT] = 0;
					p->rule._dref8 = q->rule._dref8;
				}
				if (q->cost[_faddr_NT] == 0) {
					p->cost[_faddr_NT] = 0;
					p->rule._faddr = q->rule._faddr;
				}
				if (q->cost[_mem_NT] == 0) {
					p->cost[_mem_NT] = 0;
					p->rule._mem = q->rule._mem;
				}
				if (q->cost[_vaddr_NT] == 0) {
					p->cost[_vaddr_NT] = 0;
					p->rule._vaddr = q->rule._vaddr;
				}
				if (q->cost[_addr_NT] == 0) {
					p->cost[_addr_NT] = 0;
					p->rule._addr = q->rule._addr;
				}
				if (q->cost[_ar_NT] == 0) {
					p->cost[_ar_NT] = 0;
					p->rule._ar = q->rule._ar;
				}
			}
			c = 0;
			if (c + 0 < p->cost[_reg8_NT]) {
				p->cost[_reg8_NT] = c + 0;
				p->rule._reg8 = 7;
			}
		}
		/* dref8: INDIRI1(addr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + 0;
		if (c + 0 < p->cost[_dref8_NT]) {
			p->cost[_dref8_NT] = c + 0;
			p->rule._dref8 = 1;
		}
		/* dref8: INDIRI1(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 0;
		if (c + 0 < p->cost[_dref8_NT]) {
			p->cost[_dref8_NT] = c + 0;
			p->rule._dref8 = 3;
		}
		/* reg8: INDIRI1(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg8_NT]) {
			p->cost[_reg8_NT] = c + 0;
			p->rule._reg8 = 53;
		}
		/* reg: INDIRI1(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 83;
			_closure_reg(a, c + 0);
		}
		/* reg8: INDIRI1(addr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + 1;
		if (c + 0 < p->cost[_reg8_NT]) {
			p->cost[_reg8_NT] = c + 0;
			p->rule._reg8 = 54;
		}
		/* reg: INDIRI1(addr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 84;
			_closure_reg(a, c + 0);
		}
		break;
	case 1094: /* INDIRU1 */
		_label(LEFT_CHILD(a));
		if (	/* reg8: INDIRU1(VREGP) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			if (mayrecalc(a)) {
				struct _state *q = a->syms[RX]->u.t.cse->x.state;
				if (q->cost[_stmt_NT] == 0) {
					p->cost[_stmt_NT] = 0;
					p->rule._stmt = q->rule._stmt;
				}
				if (q->cost[_reg8_NT] == 0) {
					p->cost[_reg8_NT] = 0;
					p->rule._reg8 = q->rule._reg8;
				}
				if (q->cost[_reg_NT] == 0) {
					p->cost[_reg_NT] = 0;
					p->rule._reg = q->rule._reg;
				}
				if (q->cost[_reg32_NT] == 0) {
					p->cost[_reg32_NT] = 0;
					p->rule._reg32 = q->rule._reg32;
				}
				if (q->cost[_regf64_NT] == 0) {
					p->cost[_regf64_NT] = 0;
					p->rule._regf64 = q->rule._regf64;
				}
				if (q->cost[_con_NT] == 0) {
					p->cost[_con_NT] = 0;
					p->rule._con = q->rule._con;
				}
				if (q->cost[_c0_NT] == 0) {
					p->cost[_c0_NT] = 0;
					p->rule._c0 = q->rule._c0;
				}
				if (q->cost[_c1_NT] == 0) {
					p->cost[_c1_NT] = 0;
					p->rule._c1 = q->rule._c1;
				}
				if (q->cost[_c2_NT] == 0) {
					p->cost[_c2_NT] = 0;
					p->rule._c2 = q->rule._c2;
				}
				if (q->cost[_con1_NT] == 0) {
					p->cost[_con1_NT] = 0;
					p->rule._con1 = q->rule._con1;
				}
				if (q->cost[_con2_NT] == 0) {
					p->cost[_con2_NT] = 0;
					p->rule._con2 = q->rule._con2;
				}
				if (q->cost[_con4_NT] == 0) {
					p->cost[_con4_NT] = 0;
					p->rule._con4 = q->rule._con4;
				}
				if (q->cost[_creg_NT] == 0) {
					p->cost[_creg_NT] = 0;
					p->rule._creg = q->rule._creg;
				}
				if (q->cost[_dref16_NT] == 0) {
					p->cost[_dref16_NT] = 0;
					p->rule._dref16 = q->rule._dref16;
				}
				if (q->cost[_dref8_NT] == 0) {
					p->cost[_dref8_NT] = 0;
					p->rule._dref8 = q->rule._dref8;
				}
				if (q->cost[_faddr_NT] == 0) {
					p->cost[_faddr_NT] = 0;
					p->rule._faddr = q->rule._faddr;
				}
				if (q->cost[_mem_NT] == 0) {
					p->cost[_mem_NT] = 0;
					p->rule._mem = q->rule._mem;
				}
				if (q->cost[_vaddr_NT] == 0) {
					p->cost[_vaddr_NT] = 0;
					p->rule._vaddr = q->rule._vaddr;
				}
				if (q->cost[_addr_NT] == 0) {
					p->cost[_addr_NT] = 0;
					p->rule._addr = q->rule._addr;
				}
				if (q->cost[_ar_NT] == 0) {
					p->cost[_ar_NT] = 0;
					p->rule._ar = q->rule._ar;
				}
			}
			c = 0;
			if (c + 0 < p->cost[_reg8_NT]) {
				p->cost[_reg8_NT] = c + 0;
				p->rule._reg8 = 8;
			}
		}
		/* dref8: INDIRU1(addr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + 0;
		if (c + 0 < p->cost[_dref8_NT]) {
			p->cost[_dref8_NT] = c + 0;
			p->rule._dref8 = 2;
		}
		/* dref8: INDIRU1(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 0;
		if (c + 0 < p->cost[_dref8_NT]) {
			p->cost[_dref8_NT] = c + 0;
			p->rule._dref8 = 4;
		}
		/* reg8: INDIRU1(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg8_NT]) {
			p->cost[_reg8_NT] = c + 0;
			p->rule._reg8 = 55;
		}
		/* reg: INDIRU1(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 85;
			_closure_reg(a, c + 0);
		}
		/* reg8: INDIRU1(addr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + 1;
		if (c + 0 < p->cost[_reg8_NT]) {
			p->cost[_reg8_NT] = c + 0;
			p->rule._reg8 = 56;
		}
		/* reg: INDIRU1(addr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 86;
			_closure_reg(a, c + 0);
		}
		break;
	case 1157: /* CVII1 */
		_label(LEFT_CHILD(a));
		/* reg8: CVII1(reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + 1;
		if (c + 0 < p->cost[_reg8_NT]) {
			p->cost[_reg8_NT] = c + 0;
			p->rule._reg8 = 9;
		}
		/* reg8: CVII1(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (move(a));
		if (c + 0 < p->cost[_reg8_NT]) {
			p->cost[_reg8_NT] = c + 0;
			p->rule._reg8 = 10;
		}
		break;
	case 1158: /* CVIU1 */
		_label(LEFT_CHILD(a));
		/* reg8: CVIU1(reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + 1;
		if (c + 0 < p->cost[_reg8_NT]) {
			p->cost[_reg8_NT] = c + 0;
			p->rule._reg8 = 17;
		}
		/* reg8: CVIU1(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (move(a));
		if (c + 0 < p->cost[_reg8_NT]) {
			p->cost[_reg8_NT] = c + 0;
			p->rule._reg8 = 18;
		}
		break;
	case 1205: /* CVUI1 */
		_label(LEFT_CHILD(a));
		/* reg8: CVUI1(reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + 1;
		if (c + 0 < p->cost[_reg8_NT]) {
			p->cost[_reg8_NT] = c + 0;
			p->rule._reg8 = 11;
		}
		/* reg8: CVUI1(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (move(a));
		if (c + 0 < p->cost[_reg8_NT]) {
			p->cost[_reg8_NT] = c + 0;
			p->rule._reg8 = 12;
		}
		break;
	case 1206: /* CVUU1 */
		_label(LEFT_CHILD(a));
		/* reg8: CVUU1(reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + 1;
		if (c + 0 < p->cost[_reg8_NT]) {
			p->cost[_reg8_NT] = c + 0;
			p->rule._reg8 = 19;
		}
		/* reg8: CVUU1(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (move(a));
		if (c + 0 < p->cost[_reg8_NT]) {
			p->cost[_reg8_NT] = c + 0;
			p->rule._reg8 = 20;
		}
		break;
	case 1253: /* LOADI1 */
		_label(LEFT_CHILD(a));
		/* reg8: LOADI1(reg8) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + (move(a));
		if (c + 0 < p->cost[_reg8_NT]) {
			p->cost[_reg8_NT] = c + 0;
			p->rule._reg8 = 1;
		}
		/* reg8: LOADI1(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (move(a));
		if (c + 0 < p->cost[_reg8_NT]) {
			p->cost[_reg8_NT] = c + 0;
			p->rule._reg8 = 2;
		}
		/* reg8: LOADI1(reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + 1;
		if (c + 0 < p->cost[_reg8_NT]) {
			p->cost[_reg8_NT] = c + 0;
			p->rule._reg8 = 3;
		}
		if (	/* reg8: LOADI1(CVUI2(reg)) */
			LEFT_CHILD(a)->op == 2229 /* CVUI2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg_NT] + 1;
			if (c + 0 < p->cost[_reg8_NT]) {
				p->cost[_reg8_NT] = c + 0;
				p->rule._reg8 = 14;
			}
		}
		if (	/* reg8: LOADI1(CVUI2(reg8)) */
			LEFT_CHILD(a)->op == 2229 /* CVUI2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg8_NT] + 1;
			if (c + 0 < p->cost[_reg8_NT]) {
				p->cost[_reg8_NT] = c + 0;
				p->rule._reg8 = 16;
			}
		}
		break;
	case 1254: /* LOADU1 */
		_label(LEFT_CHILD(a));
		/* reg8: LOADU1(reg8) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + (move(a));
		if (c + 0 < p->cost[_reg8_NT]) {
			p->cost[_reg8_NT] = c + 0;
			p->rule._reg8 = 4;
		}
		/* reg8: LOADU1(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (move(a));
		if (c + 0 < p->cost[_reg8_NT]) {
			p->cost[_reg8_NT] = c + 0;
			p->rule._reg8 = 5;
		}
		/* reg8: LOADU1(reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + 1;
		if (c + 0 < p->cost[_reg8_NT]) {
			p->cost[_reg8_NT] = c + 0;
			p->rule._reg8 = 6;
		}
		if (	/* reg8: LOADU1(LOADU2(CVII2(reg))) */
			LEFT_CHILD(a)->op == 2278 && /* LOADU2 */
			LEFT_CHILD(LEFT_CHILD(a))->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(LEFT_CHILD(a)))->x.state))->cost[_reg_NT] + 1;
			if (c + 0 < p->cost[_reg8_NT]) {
				p->cost[_reg8_NT] = c + 0;
				p->rule._reg8 = 13;
			}
		}
		if (	/* reg8: LOADU1(LOADU2(CVII2(reg8))) */
			LEFT_CHILD(a)->op == 2278 && /* LOADU2 */
			LEFT_CHILD(LEFT_CHILD(a))->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(LEFT_CHILD(a)))->x.state))->cost[_reg8_NT] + 1;
			if (c + 0 < p->cost[_reg8_NT]) {
				p->cost[_reg8_NT] = c + 0;
				p->rule._reg8 = 15;
			}
		}
		break;
	case 2069: /* CNSTI2 */
		/* c0: CNSTI2 */
		c = (range(a,0,0));
		if (c + 0 < p->cost[_c0_NT]) {
			p->cost[_c0_NT] = c + 0;
			p->rule._c0 = 1;
			_closure_c0(a, c + 0);
		}
		/* c1: CNSTI2 */
		c = (range(a,1,1));
		if (c + 0 < p->cost[_c1_NT]) {
			p->cost[_c1_NT] = c + 0;
			p->rule._c1 = 1;
			_closure_c1(a, c + 0);
		}
		/* c2: CNSTI2 */
		c = (range(a,2,2));
		if (c + 0 < p->cost[_c2_NT]) {
			p->cost[_c2_NT] = c + 0;
			p->rule._c2 = 1;
			_closure_c2(a, c + 0);
		}
		/* con2: CNSTI2 */
		if (0 + 0 < p->cost[_con2_NT]) {
			p->cost[_con2_NT] = 0 + 0;
			p->rule._con2 = 1;
			_closure_con2(a, 0 + 0);
		}
		break;
	case 2070: /* CNSTU2 */
		/* c0: CNSTU2 */
		c = (range(a,0,0));
		if (c + 0 < p->cost[_c0_NT]) {
			p->cost[_c0_NT] = c + 0;
			p->rule._c0 = 2;
			_closure_c0(a, c + 0);
		}
		/* c1: CNSTU2 */
		c = (range(a,1,1));
		if (c + 0 < p->cost[_c1_NT]) {
			p->cost[_c1_NT] = c + 0;
			p->rule._c1 = 2;
			_closure_c1(a, c + 0);
		}
		/* c2: CNSTU2 */
		c = (range(a,2,2));
		if (c + 0 < p->cost[_c2_NT]) {
			p->cost[_c2_NT] = c + 0;
			p->rule._c2 = 2;
			_closure_c2(a, c + 0);
		}
		/* con2: CNSTU2 */
		if (0 + 0 < p->cost[_con2_NT]) {
			p->cost[_con2_NT] = 0 + 0;
			p->rule._con2 = 2;
			_closure_con2(a, 0 + 0);
		}
		break;
	case 2071: /* CNSTP2 */
		/* con4: CNSTP2 */
		if (0 + 0 < p->cost[_con4_NT]) {
			p->cost[_con4_NT] = 0 + 0;
			p->rule._con4 = 3;
			_closure_con4(a, 0 + 0);
		}
		break;
	case 2085: /* ARGI2 */
		_label(LEFT_CHILD(a));
		/* stmt: ARGI2(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 223;
		}
		break;
	case 2086: /* ARGU2 */
		_label(LEFT_CHILD(a));
		/* stmt: ARGU2(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 224;
		}
		break;
	case 2087: /* ARGP2 */
		_label(LEFT_CHILD(a));
		/* stmt: ARGP2(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 222;
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
		/* stmt: ASGNI2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 234;
		}
		/* stmt: ASGNI2(addr,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 235;
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
		/* stmt: ASGNU2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 236;
		}
		/* stmt: ASGNU2(addr,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 237;
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
		/* stmt: ASGNP2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 238;
		}
		/* stmt: ASGNP2(addr,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 239;
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
				if (q->cost[_reg8_NT] == 0) {
					p->cost[_reg8_NT] = 0;
					p->rule._reg8 = q->rule._reg8;
				}
				if (q->cost[_reg_NT] == 0) {
					p->cost[_reg_NT] = 0;
					p->rule._reg = q->rule._reg;
				}
				if (q->cost[_reg32_NT] == 0) {
					p->cost[_reg32_NT] = 0;
					p->rule._reg32 = q->rule._reg32;
				}
				if (q->cost[_regf64_NT] == 0) {
					p->cost[_regf64_NT] = 0;
					p->rule._regf64 = q->rule._regf64;
				}
				if (q->cost[_con_NT] == 0) {
					p->cost[_con_NT] = 0;
					p->rule._con = q->rule._con;
				}
				if (q->cost[_c0_NT] == 0) {
					p->cost[_c0_NT] = 0;
					p->rule._c0 = q->rule._c0;
				}
				if (q->cost[_c1_NT] == 0) {
					p->cost[_c1_NT] = 0;
					p->rule._c1 = q->rule._c1;
				}
				if (q->cost[_c2_NT] == 0) {
					p->cost[_c2_NT] = 0;
					p->rule._c2 = q->rule._c2;
				}
				if (q->cost[_con1_NT] == 0) {
					p->cost[_con1_NT] = 0;
					p->rule._con1 = q->rule._con1;
				}
				if (q->cost[_con2_NT] == 0) {
					p->cost[_con2_NT] = 0;
					p->rule._con2 = q->rule._con2;
				}
				if (q->cost[_con4_NT] == 0) {
					p->cost[_con4_NT] = 0;
					p->rule._con4 = q->rule._con4;
				}
				if (q->cost[_creg_NT] == 0) {
					p->cost[_creg_NT] = 0;
					p->rule._creg = q->rule._creg;
				}
				if (q->cost[_dref16_NT] == 0) {
					p->cost[_dref16_NT] = 0;
					p->rule._dref16 = q->rule._dref16;
				}
				if (q->cost[_dref8_NT] == 0) {
					p->cost[_dref8_NT] = 0;
					p->rule._dref8 = q->rule._dref8;
				}
				if (q->cost[_faddr_NT] == 0) {
					p->cost[_faddr_NT] = 0;
					p->rule._faddr = q->rule._faddr;
				}
				if (q->cost[_mem_NT] == 0) {
					p->cost[_mem_NT] = 0;
					p->rule._mem = q->rule._mem;
				}
				if (q->cost[_vaddr_NT] == 0) {
					p->cost[_vaddr_NT] = 0;
					p->rule._vaddr = q->rule._vaddr;
				}
				if (q->cost[_addr_NT] == 0) {
					p->cost[_addr_NT] = 0;
					p->rule._addr = q->rule._addr;
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
		/* dref16: INDIRI2(addr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + 0;
		if (c + 0 < p->cost[_dref16_NT]) {
			p->cost[_dref16_NT] = c + 0;
			p->rule._dref16 = 2;
		}
		/* dref16: INDIRI2(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 0;
		if (c + 0 < p->cost[_dref16_NT]) {
			p->cost[_dref16_NT] = c + 0;
			p->rule._dref16 = 8;
		}
		/* reg: INDIRI2(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 87;
			_closure_reg(a, c + 0);
		}
		/* reg: INDIRI2(addr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 88;
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
				if (q->cost[_reg8_NT] == 0) {
					p->cost[_reg8_NT] = 0;
					p->rule._reg8 = q->rule._reg8;
				}
				if (q->cost[_reg_NT] == 0) {
					p->cost[_reg_NT] = 0;
					p->rule._reg = q->rule._reg;
				}
				if (q->cost[_reg32_NT] == 0) {
					p->cost[_reg32_NT] = 0;
					p->rule._reg32 = q->rule._reg32;
				}
				if (q->cost[_regf64_NT] == 0) {
					p->cost[_regf64_NT] = 0;
					p->rule._regf64 = q->rule._regf64;
				}
				if (q->cost[_con_NT] == 0) {
					p->cost[_con_NT] = 0;
					p->rule._con = q->rule._con;
				}
				if (q->cost[_c0_NT] == 0) {
					p->cost[_c0_NT] = 0;
					p->rule._c0 = q->rule._c0;
				}
				if (q->cost[_c1_NT] == 0) {
					p->cost[_c1_NT] = 0;
					p->rule._c1 = q->rule._c1;
				}
				if (q->cost[_c2_NT] == 0) {
					p->cost[_c2_NT] = 0;
					p->rule._c2 = q->rule._c2;
				}
				if (q->cost[_con1_NT] == 0) {
					p->cost[_con1_NT] = 0;
					p->rule._con1 = q->rule._con1;
				}
				if (q->cost[_con2_NT] == 0) {
					p->cost[_con2_NT] = 0;
					p->rule._con2 = q->rule._con2;
				}
				if (q->cost[_con4_NT] == 0) {
					p->cost[_con4_NT] = 0;
					p->rule._con4 = q->rule._con4;
				}
				if (q->cost[_creg_NT] == 0) {
					p->cost[_creg_NT] = 0;
					p->rule._creg = q->rule._creg;
				}
				if (q->cost[_dref16_NT] == 0) {
					p->cost[_dref16_NT] = 0;
					p->rule._dref16 = q->rule._dref16;
				}
				if (q->cost[_dref8_NT] == 0) {
					p->cost[_dref8_NT] = 0;
					p->rule._dref8 = q->rule._dref8;
				}
				if (q->cost[_faddr_NT] == 0) {
					p->cost[_faddr_NT] = 0;
					p->rule._faddr = q->rule._faddr;
				}
				if (q->cost[_mem_NT] == 0) {
					p->cost[_mem_NT] = 0;
					p->rule._mem = q->rule._mem;
				}
				if (q->cost[_vaddr_NT] == 0) {
					p->cost[_vaddr_NT] = 0;
					p->rule._vaddr = q->rule._vaddr;
				}
				if (q->cost[_addr_NT] == 0) {
					p->cost[_addr_NT] = 0;
					p->rule._addr = q->rule._addr;
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
		/* dref16: INDIRU2(addr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + 0;
		if (c + 0 < p->cost[_dref16_NT]) {
			p->cost[_dref16_NT] = c + 0;
			p->rule._dref16 = 3;
		}
		/* dref16: INDIRU2(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 0;
		if (c + 0 < p->cost[_dref16_NT]) {
			p->cost[_dref16_NT] = c + 0;
			p->rule._dref16 = 9;
		}
		/* reg: INDIRU2(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 89;
			_closure_reg(a, c + 0);
		}
		/* reg: INDIRU2(addr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 90;
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
				if (q->cost[_reg8_NT] == 0) {
					p->cost[_reg8_NT] = 0;
					p->rule._reg8 = q->rule._reg8;
				}
				if (q->cost[_reg_NT] == 0) {
					p->cost[_reg_NT] = 0;
					p->rule._reg = q->rule._reg;
				}
				if (q->cost[_reg32_NT] == 0) {
					p->cost[_reg32_NT] = 0;
					p->rule._reg32 = q->rule._reg32;
				}
				if (q->cost[_regf64_NT] == 0) {
					p->cost[_regf64_NT] = 0;
					p->rule._regf64 = q->rule._regf64;
				}
				if (q->cost[_con_NT] == 0) {
					p->cost[_con_NT] = 0;
					p->rule._con = q->rule._con;
				}
				if (q->cost[_c0_NT] == 0) {
					p->cost[_c0_NT] = 0;
					p->rule._c0 = q->rule._c0;
				}
				if (q->cost[_c1_NT] == 0) {
					p->cost[_c1_NT] = 0;
					p->rule._c1 = q->rule._c1;
				}
				if (q->cost[_c2_NT] == 0) {
					p->cost[_c2_NT] = 0;
					p->rule._c2 = q->rule._c2;
				}
				if (q->cost[_con1_NT] == 0) {
					p->cost[_con1_NT] = 0;
					p->rule._con1 = q->rule._con1;
				}
				if (q->cost[_con2_NT] == 0) {
					p->cost[_con2_NT] = 0;
					p->rule._con2 = q->rule._con2;
				}
				if (q->cost[_con4_NT] == 0) {
					p->cost[_con4_NT] = 0;
					p->rule._con4 = q->rule._con4;
				}
				if (q->cost[_creg_NT] == 0) {
					p->cost[_creg_NT] = 0;
					p->rule._creg = q->rule._creg;
				}
				if (q->cost[_dref16_NT] == 0) {
					p->cost[_dref16_NT] = 0;
					p->rule._dref16 = q->rule._dref16;
				}
				if (q->cost[_dref8_NT] == 0) {
					p->cost[_dref8_NT] = 0;
					p->rule._dref8 = q->rule._dref8;
				}
				if (q->cost[_faddr_NT] == 0) {
					p->cost[_faddr_NT] = 0;
					p->rule._faddr = q->rule._faddr;
				}
				if (q->cost[_mem_NT] == 0) {
					p->cost[_mem_NT] = 0;
					p->rule._mem = q->rule._mem;
				}
				if (q->cost[_vaddr_NT] == 0) {
					p->cost[_vaddr_NT] = 0;
					p->rule._vaddr = q->rule._vaddr;
				}
				if (q->cost[_addr_NT] == 0) {
					p->cost[_addr_NT] = 0;
					p->rule._addr = q->rule._addr;
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
		/* dref16: INDIRP2(addr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + 0;
		if (c + 0 < p->cost[_dref16_NT]) {
			p->cost[_dref16_NT] = c + 0;
			p->rule._dref16 = 1;
		}
		/* dref16: INDIRP2(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 0;
		if (c + 0 < p->cost[_dref16_NT]) {
			p->cost[_dref16_NT] = c + 0;
			p->rule._dref16 = 7;
		}
		/* reg: INDIRP2(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 91;
			_closure_reg(a, c + 0);
		}
		/* reg: INDIRP2(addr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_addr_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 92;
			_closure_reg(a, c + 0);
		}
		break;
	case 2165: /* CVFI2 */
		_label(LEFT_CHILD(a));
		/* reg: CVFI2(regf64) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + (isfloat8(a));
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 9;
			_closure_reg(a, c + 0);
		}
		/* reg: CVFI2(regf64) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + (isfloat4(a));
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 10;
			_closure_reg(a, c + 0);
		}
		break;
	case 2181: /* CVII2 */
		_label(LEFT_CHILD(a));
		/* reg: CVII2(reg8) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 11;
			_closure_reg(a, c + 0);
		}
		/* reg: CVII2(reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 12;
			_closure_reg(a, c + 0);
		}
		break;
	case 2182: /* CVIU2 */
		_label(LEFT_CHILD(a));
		/* reg: CVIU2(reg8) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 15;
			_closure_reg(a, c + 0);
		}
		/* reg: CVIU2(reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 16;
			_closure_reg(a, c + 0);
		}
		break;
	case 2198: /* CVPU2 */
		break;
	case 2229: /* CVUI2 */
		_label(LEFT_CHILD(a));
		/* reg: CVUI2(reg8) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 13;
			_closure_reg(a, c + 0);
		}
		/* reg: CVUI2(reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 14;
			_closure_reg(a, c + 0);
		}
		break;
	case 2230: /* CVUU2 */
		_label(LEFT_CHILD(a));
		/* reg: CVUU2(reg8) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 17;
			_closure_reg(a, c + 0);
		}
		/* reg: CVUU2(reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 18;
			_closure_reg(a, c + 0);
		}
		break;
	case 2231: /* CVUP2 */
		break;
	case 2245: /* NEGI2 */
		_label(LEFT_CHILD(a));
		/* reg: NEGI2(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 5;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 63;
			_closure_reg(a, c + 0);
		}
		break;
	case 2261: /* CALLI2 */
		_label(LEFT_CHILD(a));
		/* reg: CALLI2(ar) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_ar_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 93;
			_closure_reg(a, c + 0);
		}
		break;
	case 2262: /* CALLU2 */
		_label(LEFT_CHILD(a));
		/* reg: CALLU2(ar) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_ar_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 94;
			_closure_reg(a, c + 0);
		}
		break;
	case 2263: /* CALLP2 */
		_label(LEFT_CHILD(a));
		/* reg: CALLP2(ar) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_ar_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 95;
			_closure_reg(a, c + 0);
		}
		break;
	case 2277: /* LOADI2 */
		_label(LEFT_CHILD(a));
		/* reg: LOADI2(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (move(a));
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 1;
			_closure_reg(a, c + 0);
		}
		/* reg: LOADI2(reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 2;
			_closure_reg(a, c + 0);
		}
		/* dref16: LOADI2(dref16) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_dref16_NT] + 0;
		if (c + 0 < p->cost[_dref16_NT]) {
			p->cost[_dref16_NT] = c + 0;
			p->rule._dref16 = 5;
		}
		break;
	case 2278: /* LOADU2 */
		_label(LEFT_CHILD(a));
		/* reg: LOADU2(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (move(a));
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 3;
			_closure_reg(a, c + 0);
		}
		/* reg: LOADU2(reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 4;
			_closure_reg(a, c + 0);
		}
		/* dref16: LOADU2(dref16) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_dref16_NT] + 0;
		if (c + 0 < p->cost[_dref16_NT]) {
			p->cost[_dref16_NT] = c + 0;
			p->rule._dref16 = 4;
		}
		break;
	case 2279: /* LOADP2 */
		_label(LEFT_CHILD(a));
		/* reg: LOADP2(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (move(a));
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 5;
			_closure_reg(a, c + 0);
		}
		/* dref16: LOADP2(dref16) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_dref16_NT] + 0;
		if (c + 0 < p->cost[_dref16_NT]) {
			p->cost[_dref16_NT] = c + 0;
			p->rule._dref16 = 6;
		}
		break;
	case 2293: /* RETI2 */
		_label(LEFT_CHILD(a));
		/* stmt: RETI2(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 214;
		}
		break;
	case 2294: /* RETU2 */
		_label(LEFT_CHILD(a));
		/* stmt: RETU2(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 215;
		}
		break;
	case 2295: /* RETP2 */
		_label(LEFT_CHILD(a));
		/* stmt: RETP2(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 216;
		}
		break;
	case 2311: /* ADDRGP2 */
		/* faddr: ADDRGP2 */
		c = (LBURG_MAX);
		if (c + 0 < p->cost[_faddr_NT]) {
			p->cost[_faddr_NT] = c + 0;
			p->rule._faddr = 1;
			_closure_faddr(a, c + 0);
		}
		/* faddr: ADDRGP2 */
		if (0 + 0 < p->cost[_faddr_NT]) {
			p->cost[_faddr_NT] = 0 + 0;
			p->rule._faddr = 2;
			_closure_faddr(a, 0 + 0);
		}
		/* reg: ADDRGP2 */
		if (1 + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = 1 + 0;
			p->rule._reg = 80;
			_closure_reg(a, 1 + 0);
		}
		/* ar: ADDRGP2 */
		if (0 + 0 < p->cost[_ar_NT]) {
			p->cost[_ar_NT] = 0 + 0;
			p->rule._ar = 2;
		}
		break;
	case 2327: /* ADDRFP2 */
		/* faddr: ADDRFP2 */
		if (0 + 0 < p->cost[_faddr_NT]) {
			p->cost[_faddr_NT] = 0 + 0;
			p->rule._faddr = 4;
			_closure_faddr(a, 0 + 0);
		}
		/* reg: ADDRFP2 */
		if (1 + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = 1 + 0;
			p->rule._reg = 82;
			_closure_reg(a, 1 + 0);
		}
		break;
	case 2343: /* ADDRLP2 */
		/* faddr: ADDRLP2 */
		if (0 + 0 < p->cost[_faddr_NT]) {
			p->cost[_faddr_NT] = 0 + 0;
			p->rule._faddr = 3;
			_closure_faddr(a, 0 + 0);
		}
		/* reg: ADDRLP2 */
		if (1 + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = 1 + 0;
			p->rule._reg = 81;
			_closure_reg(a, 1 + 0);
		}
		break;
	case 2357: /* ADDI2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: ADDI2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 22;
			_closure_reg(a, c + 0);
		}
		/* reg: ADDI2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 23;
			_closure_reg(a, c + 0);
		}
		/* reg: ADDI2(reg,dref16) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_dref16_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 24;
			_closure_reg(a, c + 0);
		}
		/* reg8: ADDI2(reg8,reg8) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg8_NT] + 2;
		if (c + 0 < p->cost[_reg8_NT]) {
			p->cost[_reg8_NT] = c + 0;
			p->rule._reg8 = 22;
		}
		/* reg8: ADDI2(reg8,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_reg8_NT]) {
			p->cost[_reg8_NT] = c + 0;
			p->rule._reg8 = 23;
		}
		if (	/* reg8: ADDI2(CVII2(reg8),con) */
			LEFT_CHILD(a)->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
			if (c + 0 < p->cost[_reg8_NT]) {
				p->cost[_reg8_NT] = c + 0;
				p->rule._reg8 = 24;
			}
		}
		if (	/* reg8: ADDI2(reg8,CVII2(dref8)) */
			RIGHT_CHILD(a)->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_dref8_NT] + 2;
			if (c + 0 < p->cost[_reg8_NT]) {
				p->cost[_reg8_NT] = c + 0;
				p->rule._reg8 = 25;
			}
		}
		if (	/* reg8: ADDI2(CVII2(reg8),CVII2(dref8)) */
			LEFT_CHILD(a)->op == 2181 && /* CVII2 */
			RIGHT_CHILD(a)->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_dref8_NT] + 2;
			if (c + 0 < p->cost[_reg8_NT]) {
				p->cost[_reg8_NT] = c + 0;
				p->rule._reg8 = 26;
			}
		}
		break;
	case 2358: /* ADDU2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: ADDU2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 25;
			_closure_reg(a, c + 0);
		}
		/* reg: ADDU2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 26;
			_closure_reg(a, c + 0);
		}
		/* reg: ADDU2(reg,dref16) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_dref16_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 27;
			_closure_reg(a, c + 0);
		}
		/* reg8: ADDU2(reg8,reg8) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg8_NT] + 2;
		if (c + 0 < p->cost[_reg8_NT]) {
			p->cost[_reg8_NT] = c + 0;
			p->rule._reg8 = 27;
		}
		/* reg8: ADDU2(reg8,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_reg8_NT]) {
			p->cost[_reg8_NT] = c + 0;
			p->rule._reg8 = 28;
		}
		if (	/* reg8: ADDU2(reg8,CVUU2(dref8)) */
			RIGHT_CHILD(a)->op == 2230 /* CVUU2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_dref8_NT] + 2;
			if (c + 0 < p->cost[_reg8_NT]) {
				p->cost[_reg8_NT] = c + 0;
				p->rule._reg8 = 29;
			}
		}
		if (	/* reg8: ADDU2(CVUU2(reg8),CVUU2(dref8)) */
			LEFT_CHILD(a)->op == 2230 && /* CVUU2 */
			RIGHT_CHILD(a)->op == 2230 /* CVUU2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_dref8_NT] + 2;
			if (c + 0 < p->cost[_reg8_NT]) {
				p->cost[_reg8_NT] = c + 0;
				p->rule._reg8 = 30;
			}
		}
		break;
	case 2359: /* ADDP2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: ADDP2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 28;
			_closure_reg(a, c + 0);
		}
		/* reg: ADDP2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 29;
			_closure_reg(a, c + 0);
		}
		if (	/* reg: ADDP2(LSHI2(reg,c1),reg) */
			LEFT_CHILD(a)->op == 2389 /* LSHI2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(LEFT_CHILD(a))->x.state))->cost[_c1_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 30;
				_closure_reg(a, c + 0);
			}
		}
		if (	/* reg: ADDP2(LSHU2(reg,c1),reg) */
			LEFT_CHILD(a)->op == 2390 /* LSHU2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(LEFT_CHILD(a))->x.state))->cost[_c1_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
			if (c + 0 < p->cost[_reg_NT]) {
				p->cost[_reg_NT] = c + 0;
				p->rule._reg = 31;
				_closure_reg(a, c + 0);
			}
		}
		/* vaddr: ADDP2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 0;
		if (c + 0 < p->cost[_vaddr_NT]) {
			p->cost[_vaddr_NT] = c + 0;
			p->rule._vaddr = 1;
			_closure_vaddr(a, c + 0);
		}
		break;
	case 2373: /* SUBI2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: SUBI2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 32;
			_closure_reg(a, c + 0);
		}
		/* reg: SUBI2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 33;
			_closure_reg(a, c + 0);
		}
		/* reg: SUBI2(reg,dref16) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_dref16_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 34;
			_closure_reg(a, c + 0);
		}
		/* reg8: SUBI2(reg8,reg8) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg8_NT] + 2;
		if (c + 0 < p->cost[_reg8_NT]) {
			p->cost[_reg8_NT] = c + 0;
			p->rule._reg8 = 31;
		}
		/* reg8: SUBI2(reg8,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_reg8_NT]) {
			p->cost[_reg8_NT] = c + 0;
			p->rule._reg8 = 32;
		}
		if (	/* reg8: SUBI2(reg8,CVII2(dref16)) */
			RIGHT_CHILD(a)->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_dref16_NT] + 2;
			if (c + 0 < p->cost[_reg8_NT]) {
				p->cost[_reg8_NT] = c + 0;
				p->rule._reg8 = 33;
			}
		}
		break;
	case 2374: /* SUBU2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: SUBU2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 35;
			_closure_reg(a, c + 0);
		}
		/* reg: SUBU2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 36;
			_closure_reg(a, c + 0);
		}
		/* reg: SUBU2(reg,dref16) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_dref16_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 37;
			_closure_reg(a, c + 0);
		}
		/* reg8: SUBU2(reg8,reg8) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg8_NT] + 2;
		if (c + 0 < p->cost[_reg8_NT]) {
			p->cost[_reg8_NT] = c + 0;
			p->rule._reg8 = 34;
		}
		/* reg8: SUBU2(reg8,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_reg8_NT]) {
			p->cost[_reg8_NT] = c + 0;
			p->rule._reg8 = 35;
		}
		if (	/* reg8: SUBU2(reg8,CVUU2(dref16)) */
			RIGHT_CHILD(a)->op == 2230 /* CVUU2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_dref16_NT] + 2;
			if (c + 0 < p->cost[_reg8_NT]) {
				p->cost[_reg8_NT] = c + 0;
				p->rule._reg8 = 36;
			}
		}
		break;
	case 2375: /* SUBP2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: SUBP2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 38;
			_closure_reg(a, c + 0);
		}
		/* reg: SUBP2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 39;
			_closure_reg(a, c + 0);
		}
		/* reg: SUBP2(reg,dref16) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_dref16_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 40;
			_closure_reg(a, c + 0);
		}
		break;
	case 2389: /* LSHI2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: LSHI2(reg,c1) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_c1_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 64;
			_closure_reg(a, c + 0);
		}
		/* reg: LSHI2(reg,c2) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_c2_NT] + 3;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 65;
			_closure_reg(a, c + 0);
		}
		/* reg: LSHI2(reg,creg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_creg_NT] + 4;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 66;
			_closure_reg(a, c + 0);
		}
		/* reg: LSHI2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 4;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 67;
			_closure_reg(a, c + 0);
		}
		break;
	case 2390: /* LSHU2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: LSHU2(reg,c1) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_c1_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 68;
			_closure_reg(a, c + 0);
		}
		/* reg: LSHU2(reg,c2) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_c2_NT] + 3;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 69;
			_closure_reg(a, c + 0);
		}
		/* reg: LSHU2(reg,creg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_creg_NT] + 4;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 70;
			_closure_reg(a, c + 0);
		}
		/* reg: LSHU2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 4;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 71;
			_closure_reg(a, c + 0);
		}
		break;
	case 2405: /* MODI2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: MODI2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 61;
			_closure_reg(a, c + 0);
		}
		break;
	case 2406: /* MODU2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: MODU2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 62;
			_closure_reg(a, c + 0);
		}
		break;
	case 2421: /* RSHI2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: RSHI2(reg,c1) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_c1_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 72;
			_closure_reg(a, c + 0);
		}
		/* reg: RSHI2(reg,c2) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_c2_NT] + 3;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 73;
			_closure_reg(a, c + 0);
		}
		/* reg: RSHI2(reg,creg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_creg_NT] + 4;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 74;
			_closure_reg(a, c + 0);
		}
		/* reg: RSHI2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 4;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 75;
			_closure_reg(a, c + 0);
		}
		break;
	case 2422: /* RSHU2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: RSHU2(reg,c1) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_c1_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 76;
			_closure_reg(a, c + 0);
		}
		/* reg: RSHU2(reg,c2) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_c2_NT] + 3;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 77;
			_closure_reg(a, c + 0);
		}
		/* reg: RSHU2(reg,creg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_creg_NT] + 4;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 78;
			_closure_reg(a, c + 0);
		}
		/* reg: RSHU2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 4;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 79;
			_closure_reg(a, c + 0);
		}
		break;
	case 2437: /* BANDI2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: BANDI2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 41;
			_closure_reg(a, c + 0);
		}
		/* reg: BANDI2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 42;
			_closure_reg(a, c + 0);
		}
		/* reg: BANDI2(reg,dref16) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_dref16_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 43;
			_closure_reg(a, c + 0);
		}
		/* reg8: BANDI2(reg8,reg8) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg8_NT] + 2;
		if (c + 0 < p->cost[_reg8_NT]) {
			p->cost[_reg8_NT] = c + 0;
			p->rule._reg8 = 37;
		}
		if (	/* reg8: BANDI2(CVII2(reg8),reg8) */
			LEFT_CHILD(a)->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg8_NT] + 2;
			if (c + 0 < p->cost[_reg8_NT]) {
				p->cost[_reg8_NT] = c + 0;
				p->rule._reg8 = 38;
			}
		}
		if (	/* reg8: BANDI2(CVUI2(reg8),reg8) */
			LEFT_CHILD(a)->op == 2229 /* CVUI2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg8_NT] + 2;
			if (c + 0 < p->cost[_reg8_NT]) {
				p->cost[_reg8_NT] = c + 0;
				p->rule._reg8 = 39;
			}
		}
		/* reg8: BANDI2(reg8,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_reg8_NT]) {
			p->cost[_reg8_NT] = c + 0;
			p->rule._reg8 = 40;
		}
		if (	/* reg8: BANDI2(CVII2(reg8),con) */
			LEFT_CHILD(a)->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
			if (c + 0 < p->cost[_reg8_NT]) {
				p->cost[_reg8_NT] = c + 0;
				p->rule._reg8 = 41;
			}
		}
		if (	/* reg8: BANDI2(CVUI2(reg8),con) */
			LEFT_CHILD(a)->op == 2229 /* CVUI2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
			if (c + 0 < p->cost[_reg8_NT]) {
				p->cost[_reg8_NT] = c + 0;
				p->rule._reg8 = 42;
			}
		}
		if (	/* reg8: BANDI2(reg8,CVII2(dref16)) */
			RIGHT_CHILD(a)->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_dref16_NT] + 2;
			if (c + 0 < p->cost[_reg8_NT]) {
				p->cost[_reg8_NT] = c + 0;
				p->rule._reg8 = 43;
			}
		}
		break;
	case 2438: /* BANDU2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: BANDU2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 44;
			_closure_reg(a, c + 0);
		}
		/* reg: BANDU2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 45;
			_closure_reg(a, c + 0);
		}
		/* reg: BANDU2(reg,dref16) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_dref16_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 46;
			_closure_reg(a, c + 0);
		}
		/* reg8: BANDU2(reg8,reg8) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg8_NT] + 2;
		if (c + 0 < p->cost[_reg8_NT]) {
			p->cost[_reg8_NT] = c + 0;
			p->rule._reg8 = 44;
		}
		/* reg8: BANDU2(reg8,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_reg8_NT]) {
			p->cost[_reg8_NT] = c + 0;
			p->rule._reg8 = 45;
		}
		if (	/* reg8: BANDU2(reg8,CVUU2(dref16)) */
			RIGHT_CHILD(a)->op == 2230 /* CVUU2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_dref16_NT] + 2;
			if (c + 0 < p->cost[_reg8_NT]) {
				p->cost[_reg8_NT] = c + 0;
				p->rule._reg8 = 46;
			}
		}
		break;
	case 2453: /* BCOMI2 */
		_label(LEFT_CHILD(a));
		/* reg: BCOMI2(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 47;
			_closure_reg(a, c + 0);
		}
		break;
	case 2454: /* BCOMU2 */
		_label(LEFT_CHILD(a));
		/* reg: BCOMU2(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 48;
			_closure_reg(a, c + 0);
		}
		break;
	case 2469: /* BORI2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: BORI2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 49;
			_closure_reg(a, c + 0);
		}
		/* reg: BORI2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 50;
			_closure_reg(a, c + 0);
		}
		/* reg: BORI2(reg,dref16) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_dref16_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 51;
			_closure_reg(a, c + 0);
		}
		/* reg8: BORI2(reg8,reg8) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg8_NT] + 2;
		if (c + 0 < p->cost[_reg8_NT]) {
			p->cost[_reg8_NT] = c + 0;
			p->rule._reg8 = 47;
		}
		/* reg8: BORI2(reg8,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_reg8_NT]) {
			p->cost[_reg8_NT] = c + 0;
			p->rule._reg8 = 48;
		}
		if (	/* reg8: BORI2(reg8,CVII2(dref16)) */
			RIGHT_CHILD(a)->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_dref16_NT] + 2;
			if (c + 0 < p->cost[_reg8_NT]) {
				p->cost[_reg8_NT] = c + 0;
				p->rule._reg8 = 49;
			}
		}
		break;
	case 2470: /* BORU2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: BORU2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 52;
			_closure_reg(a, c + 0);
		}
		/* reg: BORU2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 53;
			_closure_reg(a, c + 0);
		}
		/* reg: BORU2(reg,dref16) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_dref16_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 54;
			_closure_reg(a, c + 0);
		}
		/* reg8: BORU2(reg8,reg8) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg8_NT] + 2;
		if (c + 0 < p->cost[_reg8_NT]) {
			p->cost[_reg8_NT] = c + 0;
			p->rule._reg8 = 50;
		}
		/* reg8: BORU2(reg8,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_reg8_NT]) {
			p->cost[_reg8_NT] = c + 0;
			p->rule._reg8 = 51;
		}
		if (	/* reg8: BORU2(reg8,CVUU2(dref16)) */
			RIGHT_CHILD(a)->op == 2230 /* CVUU2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_dref16_NT] + 2;
			if (c + 0 < p->cost[_reg8_NT]) {
				p->cost[_reg8_NT] = c + 0;
				p->rule._reg8 = 52;
			}
		}
		break;
	case 2485: /* BXORI2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: BXORI2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 55;
			_closure_reg(a, c + 0);
		}
		break;
	case 2486: /* BXORU2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: BXORU2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 56;
			_closure_reg(a, c + 0);
		}
		break;
	case 2501: /* DIVI2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: DIVI2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 59;
			_closure_reg(a, c + 0);
		}
		break;
	case 2502: /* DIVU2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: DIVU2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 60;
			_closure_reg(a, c + 0);
		}
		break;
	case 2517: /* MULI2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: MULI2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 4;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 57;
			_closure_reg(a, c + 0);
		}
		break;
	case 2518: /* MULU2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg: MULU2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 4;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 58;
			_closure_reg(a, c + 0);
		}
		break;
	case 2533: /* EQI2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: EQI2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 10;
		}
		/* stmt: EQI2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 11;
		}
		/* stmt: EQI2(reg,dref16) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_dref16_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 12;
		}
		if (	/* stmt: EQI2(BANDI2(reg,con),c0) */
			LEFT_CHILD(a)->op == 2437 /* BANDI2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(LEFT_CHILD(a))->x.state))->cost[_con_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_c0_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 13;
			}
		}
		/* stmt: EQI2(reg8,reg8) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg8_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 18;
		}
		if (	/* stmt: EQI2(CVII2(reg8),CVII2(reg8)) */
			LEFT_CHILD(a)->op == 2181 && /* CVII2 */
			RIGHT_CHILD(a)->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_reg8_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 19;
			}
		}
		/* stmt: EQI2(reg8,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 20;
		}
		if (	/* stmt: EQI2(CVII2(reg8),con) */
			LEFT_CHILD(a)->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 21;
			}
		}
		if (	/* stmt: EQI2(CVUI2(reg8),con) */
			LEFT_CHILD(a)->op == 2229 /* CVUI2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 22;
			}
		}
		if (	/* stmt: EQI2(reg8,CVII2(dref8)) */
			RIGHT_CHILD(a)->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_dref8_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 23;
			}
		}
		if (	/* stmt: EQI2(CVII2(reg8),CVII2(dref8)) */
			LEFT_CHILD(a)->op == 2181 && /* CVII2 */
			RIGHT_CHILD(a)->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_dref8_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 24;
			}
		}
		if (	/* stmt: EQI2(reg8,CVII2(reg8)) */
			RIGHT_CHILD(a)->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_reg8_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 25;
			}
		}
		if (	/* stmt: EQI2(BANDI2(reg8,con),c0) */
			LEFT_CHILD(a)->op == 2437 /* BANDI2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(LEFT_CHILD(a))->x.state))->cost[_con_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_c0_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 26;
			}
		}
		if (	/* stmt: EQI2(BANDI2(CVII2(reg8),con),c0) */
			LEFT_CHILD(a)->op == 2437 && /* BANDI2 */
			LEFT_CHILD(LEFT_CHILD(a))->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(LEFT_CHILD(a)))->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(LEFT_CHILD(a))->x.state))->cost[_con_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_c0_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 27;
			}
		}
		if (	/* stmt: EQI2(BANDI2(CVUI2(reg8),con),c0) */
			LEFT_CHILD(a)->op == 2437 && /* BANDI2 */
			LEFT_CHILD(LEFT_CHILD(a))->op == 2229 /* CVUI2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(LEFT_CHILD(a)))->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(LEFT_CHILD(a))->x.state))->cost[_con_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_c0_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 28;
			}
		}
		break;
	case 2534: /* EQU2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: EQU2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 14;
		}
		/* stmt: EQU2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 15;
		}
		/* stmt: EQU2(reg,dref16) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_dref16_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 16;
		}
		if (	/* stmt: EQU2(BANDU2(reg,con),c0) */
			LEFT_CHILD(a)->op == 2438 /* BANDU2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(LEFT_CHILD(a))->x.state))->cost[_con_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_c0_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 17;
			}
		}
		/* stmt: EQU2(reg8,reg8) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg8_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 29;
		}
		/* stmt: EQU2(reg8,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 30;
		}
		if (	/* stmt: EQU2(CVUU2(reg8),con) */
			LEFT_CHILD(a)->op == 2230 /* CVUU2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 31;
			}
		}
		if (	/* stmt: EQU2(reg8,CVUU2(dref8)) */
			RIGHT_CHILD(a)->op == 2230 /* CVUU2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_dref8_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 32;
			}
		}
		if (	/* stmt: EQU2(CVUU2(reg8),CVUU2(dref8)) */
			LEFT_CHILD(a)->op == 2230 && /* CVUU2 */
			RIGHT_CHILD(a)->op == 2230 /* CVUU2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_dref8_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 33;
			}
		}
		if (	/* stmt: EQU2(reg8,CVUU2(reg8)) */
			RIGHT_CHILD(a)->op == 2230 /* CVUU2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_reg8_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 34;
			}
		}
		if (	/* stmt: EQU2(BANDU2(reg8,con),c0) */
			LEFT_CHILD(a)->op == 2438 /* BANDU2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(LEFT_CHILD(a))->x.state))->cost[_con_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_c0_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 35;
			}
		}
		break;
	case 2549: /* GEI2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: GEI2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 179;
		}
		/* stmt: GEI2(con,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_con_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 180;
		}
		/* stmt: GEI2(dref16,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_dref16_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 181;
		}
		/* stmt: GEI2(reg8,reg8) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg8_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 185;
		}
		if (	/* stmt: GEI2(CVII2(reg8),CVII2(reg8)) */
			LEFT_CHILD(a)->op == 2181 && /* CVII2 */
			RIGHT_CHILD(a)->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_reg8_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 186;
			}
		}
		/* stmt: GEI2(con,reg8) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_con_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg8_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 187;
		}
		if (	/* stmt: GEI2(con,CVII2(reg8)) */
			RIGHT_CHILD(a)->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_con_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_reg8_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 188;
			}
		}
		if (	/* stmt: GEI2(CVII2(dref8),reg8) */
			LEFT_CHILD(a)->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_dref8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg8_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 189;
			}
		}
		if (	/* stmt: GEI2(CVII2(dref8),CVII2(reg8)) */
			LEFT_CHILD(a)->op == 2181 && /* CVII2 */
			RIGHT_CHILD(a)->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_dref8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_reg8_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 190;
			}
		}
		if (	/* stmt: GEI2(CVII2(reg8),reg8) */
			LEFT_CHILD(a)->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg8_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 191;
			}
		}
		break;
	case 2550: /* GEU2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: GEU2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 182;
		}
		/* stmt: GEU2(con,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_con_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 183;
		}
		/* stmt: GEU2(dref16,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_dref16_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 184;
		}
		/* stmt: GEU2(reg8,reg8) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg8_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 192;
		}
		/* stmt: GEU2(con,reg8) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_con_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg8_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 193;
		}
		if (	/* stmt: GEU2(CVUU2(dref8),reg8) */
			LEFT_CHILD(a)->op == 2230 /* CVUU2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_dref8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg8_NT] + 2;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 194;
			}
		}
		if (	/* stmt: GEU2(CVUU2(dref8),CVUU2(reg8)) */
			LEFT_CHILD(a)->op == 2230 && /* CVUU2 */
			RIGHT_CHILD(a)->op == 2230 /* CVUU2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_dref8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_reg8_NT] + 2;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 195;
			}
		}
		if (	/* stmt: GEU2(CVUU2(reg8),reg8) */
			LEFT_CHILD(a)->op == 2230 /* CVUU2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg8_NT] + 2;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 196;
			}
		}
		break;
	case 2565: /* GTI2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: GTI2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 149;
		}
		/* stmt: GTI2(con,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_con_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 150;
		}
		/* stmt: GTI2(dref16,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_dref16_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 151;
		}
		/* stmt: GTI2(reg8,reg8) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg8_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 155;
		}
		if (	/* stmt: GTI2(CVII2(reg8),CVII2(reg8)) */
			LEFT_CHILD(a)->op == 2181 && /* CVII2 */
			RIGHT_CHILD(a)->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_reg8_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 156;
			}
		}
		/* stmt: GTI2(con,reg8) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_con_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg8_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 157;
		}
		if (	/* stmt: GTI2(con,CVII2(reg8)) */
			RIGHT_CHILD(a)->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_con_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_reg8_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 158;
			}
		}
		if (	/* stmt: GTI2(CVII2(dref8),reg8) */
			LEFT_CHILD(a)->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_dref8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg8_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 159;
			}
		}
		if (	/* stmt: GTI2(CVII2(dref8),CVII2(reg8)) */
			LEFT_CHILD(a)->op == 2181 && /* CVII2 */
			RIGHT_CHILD(a)->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_dref8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_reg8_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 160;
			}
		}
		if (	/* stmt: GTI2(CVII2(reg8),reg8) */
			LEFT_CHILD(a)->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg8_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 161;
			}
		}
		break;
	case 2566: /* GTU2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: GTU2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 152;
		}
		/* stmt: GTU2(con,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_con_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 153;
		}
		/* stmt: GTU2(dref16,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_dref16_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 154;
		}
		/* stmt: GTU2(reg8,reg8) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg8_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 162;
		}
		/* stmt: GTU2(con,reg8) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_con_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg8_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 163;
		}
		if (	/* stmt: GTU2(CVUU2(dref8),reg8) */
			LEFT_CHILD(a)->op == 2230 /* CVUU2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_dref8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg8_NT] + 2;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 164;
			}
		}
		if (	/* stmt: GTU2(CVUU2(dref8),CVUU2(reg8)) */
			LEFT_CHILD(a)->op == 2230 && /* CVUU2 */
			RIGHT_CHILD(a)->op == 2230 /* CVUU2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_dref8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_reg8_NT] + 2;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 165;
			}
		}
		if (	/* stmt: GTU2(CVUU2(reg8),reg8) */
			LEFT_CHILD(a)->op == 2230 /* CVUU2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg8_NT] + 2;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 166;
			}
		}
		break;
	case 2581: /* LEI2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: LEI2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 119;
		}
		/* stmt: LEI2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 120;
		}
		/* stmt: LEI2(reg,dref16) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_dref16_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 121;
		}
		/* stmt: LEI2(reg8,reg8) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg8_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 125;
		}
		if (	/* stmt: LEI2(CVII2(reg8),CVII2(reg8)) */
			LEFT_CHILD(a)->op == 2181 && /* CVII2 */
			RIGHT_CHILD(a)->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_reg8_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 126;
			}
		}
		/* stmt: LEI2(reg8,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 127;
		}
		if (	/* stmt: LEI2(CVII2(reg8),con) */
			LEFT_CHILD(a)->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 128;
			}
		}
		if (	/* stmt: LEI2(reg8,CVII2(dref8)) */
			RIGHT_CHILD(a)->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_dref8_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 129;
			}
		}
		if (	/* stmt: LEI2(CVII2(reg8),CVII2(dref8)) */
			LEFT_CHILD(a)->op == 2181 && /* CVII2 */
			RIGHT_CHILD(a)->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_dref8_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 130;
			}
		}
		if (	/* stmt: LEI2(reg8,CVII2(reg8)) */
			RIGHT_CHILD(a)->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_reg8_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 131;
			}
		}
		break;
	case 2582: /* LEU2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: LEU2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 122;
		}
		/* stmt: LEU2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 123;
		}
		/* stmt: LEU2(reg,dref16) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_dref16_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 124;
		}
		/* stmt: LEU2(reg8,reg8) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg8_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 132;
		}
		/* stmt: LEU2(reg8,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 133;
		}
		if (	/* stmt: LEU2(reg8,CVUU2(dref8)) */
			RIGHT_CHILD(a)->op == 2230 /* CVUU2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_dref8_NT] + 2;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 134;
			}
		}
		if (	/* stmt: LEU2(CVUU2(reg8),CVUU2(dref8)) */
			LEFT_CHILD(a)->op == 2230 && /* CVUU2 */
			RIGHT_CHILD(a)->op == 2230 /* CVUU2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_dref8_NT] + 2;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 135;
			}
		}
		if (	/* stmt: LEU2(reg8,CVUU2(reg8)) */
			RIGHT_CHILD(a)->op == 2230 /* CVUU2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_reg8_NT] + 2;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 136;
			}
		}
		break;
	case 2597: /* LTI2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: LTI2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 88;
		}
		/* stmt: LTI2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 89;
		}
		/* stmt: LTI2(reg,dref16) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_dref16_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 90;
		}
		/* stmt: LTI2(reg8,reg8) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg8_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 94;
		}
		if (	/* stmt: LTI2(CVII2(reg8),CVII2(reg8)) */
			LEFT_CHILD(a)->op == 2181 && /* CVII2 */
			RIGHT_CHILD(a)->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_reg8_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 95;
			}
		}
		/* stmt: LTI2(reg8,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 96;
		}
		if (	/* stmt: LTI2(CVII2(reg8),con) */
			LEFT_CHILD(a)->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 97;
			}
		}
		if (	/* stmt: LTI2(CVUI2(reg8),con) */
			LEFT_CHILD(a)->op == 2229 /* CVUI2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 98;
			}
		}
		if (	/* stmt: LTI2(reg8,CVII2(dref8)) */
			RIGHT_CHILD(a)->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_dref8_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 99;
			}
		}
		if (	/* stmt: LTI2(CVII2(reg8),CVII2(dref8)) */
			LEFT_CHILD(a)->op == 2181 && /* CVII2 */
			RIGHT_CHILD(a)->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_dref8_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 100;
			}
		}
		if (	/* stmt: LTI2(reg8,CVII2(reg8)) */
			RIGHT_CHILD(a)->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_reg8_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 101;
			}
		}
		break;
	case 2598: /* LTU2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: LTU2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 91;
		}
		/* stmt: LTU2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 92;
		}
		/* stmt: LTU2(reg,dref16) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_dref16_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 93;
		}
		/* stmt: LTU2(reg8,reg8) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg8_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 102;
		}
		/* stmt: LTU2(reg8,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 2;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 103;
		}
		if (	/* stmt: LTU2(reg8,CVUU2(dref8)) */
			RIGHT_CHILD(a)->op == 2230 /* CVUU2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_dref8_NT] + 2;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 104;
			}
		}
		if (	/* stmt: LTU2(CVUU2(reg8),CVUU2(dref8)) */
			LEFT_CHILD(a)->op == 2230 && /* CVUU2 */
			RIGHT_CHILD(a)->op == 2230 /* CVUU2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_dref8_NT] + 2;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 105;
			}
		}
		if (	/* stmt: LTU2(reg8,CVUU2(reg8)) */
			RIGHT_CHILD(a)->op == 2230 /* CVUU2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_reg8_NT] + 2;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 106;
			}
		}
		break;
	case 2613: /* NEI2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: NEI2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 48;
		}
		/* stmt: NEI2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 49;
		}
		/* stmt: NEI2(reg,c0) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_c0_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 50;
		}
		/* stmt: NEI2(reg,dref16) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_dref16_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 51;
		}
		if (	/* stmt: NEI2(BANDI2(reg,con),c0) */
			LEFT_CHILD(a)->op == 2437 /* BANDI2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(LEFT_CHILD(a))->x.state))->cost[_con_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_c0_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 52;
			}
		}
		/* stmt: NEI2(reg8,reg8) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg8_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 58;
		}
		if (	/* stmt: NEI2(CVII2(reg8),CVII2(reg8)) */
			LEFT_CHILD(a)->op == 2181 && /* CVII2 */
			RIGHT_CHILD(a)->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_reg8_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 59;
			}
		}
		/* stmt: NEI2(reg8,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 60;
		}
		if (	/* stmt: NEI2(CVII2(reg8),con) */
			LEFT_CHILD(a)->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 61;
			}
		}
		if (	/* stmt: NEI2(CVUI2(reg8),con) */
			LEFT_CHILD(a)->op == 2229 /* CVUI2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 62;
			}
		}
		if (	/* stmt: NEI2(reg8,CVII2(dref8)) */
			RIGHT_CHILD(a)->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_dref8_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 63;
			}
		}
		if (	/* stmt: NEI2(CVII2(reg8),CVII2(dref8)) */
			LEFT_CHILD(a)->op == 2181 && /* CVII2 */
			RIGHT_CHILD(a)->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_dref8_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 64;
			}
		}
		if (	/* stmt: NEI2(reg8,CVII2(reg8)) */
			RIGHT_CHILD(a)->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_reg8_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 65;
			}
		}
		if (	/* stmt: NEI2(BANDI2(reg8,con),c0) */
			LEFT_CHILD(a)->op == 2437 /* BANDI2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(LEFT_CHILD(a))->x.state))->cost[_con_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_c0_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 66;
			}
		}
		if (	/* stmt: NEI2(BANDI2(CVII2(reg8),con),c0) */
			LEFT_CHILD(a)->op == 2437 && /* BANDI2 */
			LEFT_CHILD(LEFT_CHILD(a))->op == 2181 /* CVII2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(LEFT_CHILD(a)))->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(LEFT_CHILD(a))->x.state))->cost[_con_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_c0_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 67;
			}
		}
		if (	/* stmt: NEI2(BANDI2(CVUI2(reg8),con),c0) */
			LEFT_CHILD(a)->op == 2437 && /* BANDI2 */
			LEFT_CHILD(LEFT_CHILD(a))->op == 2229 /* CVUI2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(LEFT_CHILD(a)))->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(LEFT_CHILD(a))->x.state))->cost[_con_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_c0_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 68;
			}
		}
		break;
	case 2614: /* NEU2 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: NEU2(reg,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 53;
		}
		/* stmt: NEU2(reg,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 54;
		}
		/* stmt: NEU2(reg,c0) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_c0_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 55;
		}
		/* stmt: NEU2(reg,dref16) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_dref16_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 56;
		}
		if (	/* stmt: NEU2(BANDI2(reg,con),c0) */
			LEFT_CHILD(a)->op == 2437 /* BANDI2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(LEFT_CHILD(a))->x.state))->cost[_con_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_c0_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 57;
			}
		}
		/* stmt: NEU2(reg8,reg8) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg8_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 69;
		}
		/* stmt: NEU2(reg8,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 70;
		}
		if (	/* stmt: NEU2(CVUU2(reg8),con) */
			LEFT_CHILD(a)->op == 2230 /* CVUU2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 71;
			}
		}
		if (	/* stmt: NEU2(reg8,CVUU2(dref8)) */
			RIGHT_CHILD(a)->op == 2230 /* CVUU2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_dref8_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 72;
			}
		}
		if (	/* stmt: NEU2(CVUU2(reg8),CVUU2(dref8)) */
			LEFT_CHILD(a)->op == 2230 && /* CVUU2 */
			RIGHT_CHILD(a)->op == 2230 /* CVUU2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_dref8_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 73;
			}
		}
		if (	/* stmt: NEU2(reg8,CVUU2(reg8)) */
			RIGHT_CHILD(a)->op == 2230 /* CVUU2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg8_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_reg8_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 74;
			}
		}
		if (	/* stmt: NEU2(BANDU2(reg8,con),c0) */
			LEFT_CHILD(a)->op == 2438 /* BANDU2 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_reg8_NT] + ((struct _state *)(RIGHT_CHILD(LEFT_CHILD(a))->x.state))->cost[_con_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_c0_NT] + 1;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 75;
			}
		}
		break;
	case 4113: /* CNSTF4 */
		break;
	case 4117: /* CNSTI4 */
		/* con4: CNSTI4 */
		if (0 + 0 < p->cost[_con4_NT]) {
			p->cost[_con4_NT] = 0 + 0;
			p->rule._con4 = 1;
			_closure_con4(a, 0 + 0);
		}
		break;
	case 4118: /* CNSTU4 */
		/* con4: CNSTU4 */
		if (0 + 0 < p->cost[_con4_NT]) {
			p->cost[_con4_NT] = 0 + 0;
			p->rule._con4 = 2;
			_closure_con4(a, 0 + 0);
		}
		break;
	case 4129: /* ARGF4 */
		_label(LEFT_CHILD(a));
		/* stmt: ARGF4(regf64) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 227;
		}
		break;
	case 4133: /* ARGI4 */
		_label(LEFT_CHILD(a));
		/* stmt: ARGI4(reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 225;
		}
		break;
	case 4134: /* ARGU4 */
		_label(LEFT_CHILD(a));
		/* stmt: ARGU4(reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 226;
		}
		break;
	case 4145: /* ASGNF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		if (	/* stmt: ASGNF4(VREGP,regf64) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			c = ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_regf64_NT] + 0;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 8;
			}
		}
		/* stmt: ASGNF4(reg,regf64) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_regf64_NT] + 4;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 248;
		}
		/* stmt: ASGNF4(faddr,regf64) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_regf64_NT] + 4;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 249;
		}
		if (	/* stmt: ASGNF4(faddr,INDIRF4(faddr)) */
			RIGHT_CHILD(a)->op == 4161 /* INDIRF4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 4;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 250;
			}
		}
		if (	/* stmt: ASGNF4(faddr,ADDF4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4401 /* ADDF4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + (memop(a));
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 258;
			}
		}
		if (	/* stmt: ASGNF4(faddr,ADDF4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4401 /* ADDF4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + (flip_memop(a));
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 259;
			}
		}
		if (	/* stmt: ASGNF4(faddr,ADDF4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4401 /* ADDF4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + 4;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 264;
			}
		}
		if (	/* stmt: ASGNF4(faddr,SUBF4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4417 /* SUBF4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + (memop(a));
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 268;
			}
		}
		if (	/* stmt: ASGNF4(faddr,SUBF4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4417 /* SUBF4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + 4;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 272;
			}
		}
		if (	/* stmt: ASGNF4(faddr,MULF4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4561 /* MULF4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + (memop(a));
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 278;
			}
		}
		if (	/* stmt: ASGNF4(faddr,MULF4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4561 /* MULF4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + (flip_memop(a));
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 279;
			}
		}
		if (	/* stmt: ASGNF4(faddr,MULF4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4561 /* MULF4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + 4;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 284;
			}
		}
		if (	/* stmt: ASGNF4(faddr,DIVF4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4545 /* DIVF4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + (memop(a));
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 288;
			}
		}
		if (	/* stmt: ASGNF4(faddr,DIVF4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4545 /* DIVF4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + 4;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 292;
			}
		}
		break;
	case 4149: /* ASGNI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		if (	/* stmt: ASGNI4(VREGP,reg32) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			c = ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg32_NT] + 0;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 6;
			}
		}
		/* stmt: ASGNI4(reg,reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg32_NT] + 4;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 242;
		}
		/* stmt: ASGNI4(faddr,reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg32_NT] + 4;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 243;
		}
		if (	/* stmt: ASGNI4(faddr,INDIRI4(faddr)) */
			RIGHT_CHILD(a)->op == 4165 /* INDIRI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 4;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 244;
			}
		}
		if (	/* stmt: ASGNI4(faddr,ADDI4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4405 /* ADDI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + (memop(a));
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 254;
			}
		}
		if (	/* stmt: ASGNI4(faddr,ADDI4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4405 /* ADDI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + (flip_memop(a));
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 255;
			}
		}
		if (	/* stmt: ASGNI4(faddr,ADDI4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4405 /* ADDI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + 4;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 262;
			}
		}
		if (	/* stmt: ASGNI4(faddr,SUBI4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4421 /* SUBI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + (memop(a));
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 266;
			}
		}
		if (	/* stmt: ASGNI4(faddr,SUBI4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4421 /* SUBI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + 4;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 270;
			}
		}
		if (	/* stmt: ASGNI4(faddr,MULI4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4565 /* MULI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + (memop(a));
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 274;
			}
		}
		if (	/* stmt: ASGNI4(faddr,MULI4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4565 /* MULI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + (flip_memop(a));
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 275;
			}
		}
		if (	/* stmt: ASGNI4(faddr,MULI4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4565 /* MULI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + 4;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 282;
			}
		}
		if (	/* stmt: ASGNI4(faddr,DIVI4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4549 /* DIVI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + (memop(a));
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 286;
			}
		}
		if (	/* stmt: ASGNI4(faddr,DIVI4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4549 /* DIVI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + 4;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 290;
			}
		}
		if (	/* stmt: ASGNI4(faddr,BANDI4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4485 /* BANDI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + (memop(a));
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 294;
			}
		}
		if (	/* stmt: ASGNI4(faddr,BANDI4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4485 /* BANDI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + (flip_memop(a));
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 295;
			}
		}
		if (	/* stmt: ASGNI4(faddr,BANDI4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4485 /* BANDI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + 4;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 298;
			}
		}
		if (	/* stmt: ASGNI4(faddr,BANDU4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4486 /* BANDU4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + 4;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 299;
			}
		}
		if (	/* stmt: ASGNI4(faddr,BORI4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4517 /* BORI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + (memop(a));
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 300;
			}
		}
		if (	/* stmt: ASGNI4(faddr,BORI4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4517 /* BORI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + (flip_memop(a));
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 301;
			}
		}
		if (	/* stmt: ASGNI4(faddr,BORI4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4517 /* BORI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + 4;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 304;
			}
		}
		if (	/* stmt: ASGNI4(faddr,BORU4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4518 /* BORU4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + 4;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 305;
			}
		}
		if (	/* stmt: ASGNI4(faddr,BXORI4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4533 /* BXORI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + (memop(a));
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 306;
			}
		}
		if (	/* stmt: ASGNI4(faddr,BXORI4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4533 /* BXORI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + (flip_memop(a));
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 307;
			}
		}
		if (	/* stmt: ASGNI4(faddr,BXORI4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4533 /* BXORI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + 4;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 310;
			}
		}
		if (	/* stmt: ASGNI4(faddr,BXORU4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4534 /* BXORU4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + 4;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 311;
			}
		}
		break;
	case 4150: /* ASGNU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		if (	/* stmt: ASGNU4(VREGP,reg32) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			c = ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg32_NT] + 0;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 7;
			}
		}
		/* stmt: ASGNU4(reg,reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg32_NT] + 4;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 245;
		}
		/* stmt: ASGNU4(faddr,reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg32_NT] + 4;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 246;
		}
		if (	/* stmt: ASGNU4(faddr,INDIRU4(faddr)) */
			RIGHT_CHILD(a)->op == 4166 /* INDIRU4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 4;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 247;
			}
		}
		if (	/* stmt: ASGNU4(faddr,ADDI4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4405 /* ADDI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + (memop(a));
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 256;
			}
		}
		if (	/* stmt: ASGNU4(faddr,ADDI4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4405 /* ADDI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + (flip_memop(a));
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 257;
			}
		}
		if (	/* stmt: ASGNU4(faddr,ADDU4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4406 /* ADDU4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + 4;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 263;
			}
		}
		if (	/* stmt: ASGNU4(faddr,SUBI4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4421 /* SUBI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + (memop(a));
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 267;
			}
		}
		if (	/* stmt: ASGNU4(faddr,SUBU4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4422 /* SUBU4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + 4;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 271;
			}
		}
		if (	/* stmt: ASGNU4(faddr,MULI4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4565 /* MULI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + (memop(a));
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 276;
			}
		}
		if (	/* stmt: ASGNU4(faddr,MULI4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4565 /* MULI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + (flip_memop(a));
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 277;
			}
		}
		if (	/* stmt: ASGNU4(faddr,MULU4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4566 /* MULU4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + 4;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 283;
			}
		}
		if (	/* stmt: ASGNU4(faddr,DIVI4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4549 /* DIVI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + (memop(a));
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 287;
			}
		}
		if (	/* stmt: ASGNU4(faddr,DIVU4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4550 /* DIVU4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + 4;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 291;
			}
		}
		if (	/* stmt: ASGNU4(faddr,BANDU4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4486 /* BANDU4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + (memop(a));
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 296;
			}
		}
		if (	/* stmt: ASGNU4(faddr,BANDU4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4486 /* BANDU4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + (flip_memop(a));
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 297;
			}
		}
		if (	/* stmt: ASGNU4(faddr,BORU4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4518 /* BORU4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + (memop(a));
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 302;
			}
		}
		if (	/* stmt: ASGNU4(faddr,BORU4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4518 /* BORU4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + (flip_memop(a));
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 303;
			}
		}
		if (	/* stmt: ASGNU4(faddr,BXORU4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4534 /* BXORU4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + (memop(a));
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 308;
			}
		}
		if (	/* stmt: ASGNU4(faddr,BXORU4(mem,mem)) */
			RIGHT_CHILD(a)->op == 4534 /* BXORU4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + (flip_memop(a));
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 309;
			}
		}
		break;
	case 4161: /* INDIRF4 */
		_label(LEFT_CHILD(a));
		if (	/* regf64: INDIRF4(VREGP) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			if (mayrecalc(a)) {
				struct _state *q = a->syms[RX]->u.t.cse->x.state;
				if (q->cost[_stmt_NT] == 0) {
					p->cost[_stmt_NT] = 0;
					p->rule._stmt = q->rule._stmt;
				}
				if (q->cost[_reg8_NT] == 0) {
					p->cost[_reg8_NT] = 0;
					p->rule._reg8 = q->rule._reg8;
				}
				if (q->cost[_reg_NT] == 0) {
					p->cost[_reg_NT] = 0;
					p->rule._reg = q->rule._reg;
				}
				if (q->cost[_reg32_NT] == 0) {
					p->cost[_reg32_NT] = 0;
					p->rule._reg32 = q->rule._reg32;
				}
				if (q->cost[_regf64_NT] == 0) {
					p->cost[_regf64_NT] = 0;
					p->rule._regf64 = q->rule._regf64;
				}
				if (q->cost[_con_NT] == 0) {
					p->cost[_con_NT] = 0;
					p->rule._con = q->rule._con;
				}
				if (q->cost[_c0_NT] == 0) {
					p->cost[_c0_NT] = 0;
					p->rule._c0 = q->rule._c0;
				}
				if (q->cost[_c1_NT] == 0) {
					p->cost[_c1_NT] = 0;
					p->rule._c1 = q->rule._c1;
				}
				if (q->cost[_c2_NT] == 0) {
					p->cost[_c2_NT] = 0;
					p->rule._c2 = q->rule._c2;
				}
				if (q->cost[_con1_NT] == 0) {
					p->cost[_con1_NT] = 0;
					p->rule._con1 = q->rule._con1;
				}
				if (q->cost[_con2_NT] == 0) {
					p->cost[_con2_NT] = 0;
					p->rule._con2 = q->rule._con2;
				}
				if (q->cost[_con4_NT] == 0) {
					p->cost[_con4_NT] = 0;
					p->rule._con4 = q->rule._con4;
				}
				if (q->cost[_creg_NT] == 0) {
					p->cost[_creg_NT] = 0;
					p->rule._creg = q->rule._creg;
				}
				if (q->cost[_dref16_NT] == 0) {
					p->cost[_dref16_NT] = 0;
					p->rule._dref16 = q->rule._dref16;
				}
				if (q->cost[_dref8_NT] == 0) {
					p->cost[_dref8_NT] = 0;
					p->rule._dref8 = q->rule._dref8;
				}
				if (q->cost[_faddr_NT] == 0) {
					p->cost[_faddr_NT] = 0;
					p->rule._faddr = q->rule._faddr;
				}
				if (q->cost[_mem_NT] == 0) {
					p->cost[_mem_NT] = 0;
					p->rule._mem = q->rule._mem;
				}
				if (q->cost[_vaddr_NT] == 0) {
					p->cost[_vaddr_NT] = 0;
					p->rule._vaddr = q->rule._vaddr;
				}
				if (q->cost[_addr_NT] == 0) {
					p->cost[_addr_NT] = 0;
					p->rule._addr = q->rule._addr;
				}
				if (q->cost[_ar_NT] == 0) {
					p->cost[_ar_NT] = 0;
					p->rule._ar = q->rule._ar;
				}
			}
			c = 0;
			if (c + 0 < p->cost[_regf64_NT]) {
				p->cost[_regf64_NT] = c + 0;
				p->rule._regf64 = 3;
				_closure_regf64(a, c + 0);
			}
		}
		/* regf64: INDIRF4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 4;
		if (c + 0 < p->cost[_regf64_NT]) {
			p->cost[_regf64_NT] = c + 0;
			p->rule._regf64 = 31;
			_closure_regf64(a, c + 0);
		}
		/* regf64: INDIRF4(faddr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + 4;
		if (c + 0 < p->cost[_regf64_NT]) {
			p->cost[_regf64_NT] = c + 0;
			p->rule._regf64 = 32;
			_closure_regf64(a, c + 0);
		}
		/* mem: INDIRF4(faddr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + 0;
		if (c + 0 < p->cost[_mem_NT]) {
			p->cost[_mem_NT] = c + 0;
			p->rule._mem = 3;
		}
		break;
	case 4165: /* INDIRI4 */
		_label(LEFT_CHILD(a));
		if (	/* reg32: INDIRI4(VREGP) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			if (mayrecalc(a)) {
				struct _state *q = a->syms[RX]->u.t.cse->x.state;
				if (q->cost[_stmt_NT] == 0) {
					p->cost[_stmt_NT] = 0;
					p->rule._stmt = q->rule._stmt;
				}
				if (q->cost[_reg8_NT] == 0) {
					p->cost[_reg8_NT] = 0;
					p->rule._reg8 = q->rule._reg8;
				}
				if (q->cost[_reg_NT] == 0) {
					p->cost[_reg_NT] = 0;
					p->rule._reg = q->rule._reg;
				}
				if (q->cost[_reg32_NT] == 0) {
					p->cost[_reg32_NT] = 0;
					p->rule._reg32 = q->rule._reg32;
				}
				if (q->cost[_regf64_NT] == 0) {
					p->cost[_regf64_NT] = 0;
					p->rule._regf64 = q->rule._regf64;
				}
				if (q->cost[_con_NT] == 0) {
					p->cost[_con_NT] = 0;
					p->rule._con = q->rule._con;
				}
				if (q->cost[_c0_NT] == 0) {
					p->cost[_c0_NT] = 0;
					p->rule._c0 = q->rule._c0;
				}
				if (q->cost[_c1_NT] == 0) {
					p->cost[_c1_NT] = 0;
					p->rule._c1 = q->rule._c1;
				}
				if (q->cost[_c2_NT] == 0) {
					p->cost[_c2_NT] = 0;
					p->rule._c2 = q->rule._c2;
				}
				if (q->cost[_con1_NT] == 0) {
					p->cost[_con1_NT] = 0;
					p->rule._con1 = q->rule._con1;
				}
				if (q->cost[_con2_NT] == 0) {
					p->cost[_con2_NT] = 0;
					p->rule._con2 = q->rule._con2;
				}
				if (q->cost[_con4_NT] == 0) {
					p->cost[_con4_NT] = 0;
					p->rule._con4 = q->rule._con4;
				}
				if (q->cost[_creg_NT] == 0) {
					p->cost[_creg_NT] = 0;
					p->rule._creg = q->rule._creg;
				}
				if (q->cost[_dref16_NT] == 0) {
					p->cost[_dref16_NT] = 0;
					p->rule._dref16 = q->rule._dref16;
				}
				if (q->cost[_dref8_NT] == 0) {
					p->cost[_dref8_NT] = 0;
					p->rule._dref8 = q->rule._dref8;
				}
				if (q->cost[_faddr_NT] == 0) {
					p->cost[_faddr_NT] = 0;
					p->rule._faddr = q->rule._faddr;
				}
				if (q->cost[_mem_NT] == 0) {
					p->cost[_mem_NT] = 0;
					p->rule._mem = q->rule._mem;
				}
				if (q->cost[_vaddr_NT] == 0) {
					p->cost[_vaddr_NT] = 0;
					p->rule._vaddr = q->rule._vaddr;
				}
				if (q->cost[_addr_NT] == 0) {
					p->cost[_addr_NT] = 0;
					p->rule._addr = q->rule._addr;
				}
				if (q->cost[_ar_NT] == 0) {
					p->cost[_ar_NT] = 0;
					p->rule._ar = q->rule._ar;
				}
			}
			c = 0;
			if (c + 0 < p->cost[_reg32_NT]) {
				p->cost[_reg32_NT] = c + 0;
				p->rule._reg32 = 3;
				_closure_reg32(a, c + 0);
			}
		}
		/* reg32: INDIRI4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 4;
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 56;
			_closure_reg32(a, c + 0);
		}
		/* reg32: INDIRI4(faddr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + 4;
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 57;
			_closure_reg32(a, c + 0);
		}
		/* mem: INDIRI4(faddr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + 0;
		if (c + 0 < p->cost[_mem_NT]) {
			p->cost[_mem_NT] = c + 0;
			p->rule._mem = 1;
		}
		break;
	case 4166: /* INDIRU4 */
		_label(LEFT_CHILD(a));
		if (	/* reg32: INDIRU4(VREGP) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			if (mayrecalc(a)) {
				struct _state *q = a->syms[RX]->u.t.cse->x.state;
				if (q->cost[_stmt_NT] == 0) {
					p->cost[_stmt_NT] = 0;
					p->rule._stmt = q->rule._stmt;
				}
				if (q->cost[_reg8_NT] == 0) {
					p->cost[_reg8_NT] = 0;
					p->rule._reg8 = q->rule._reg8;
				}
				if (q->cost[_reg_NT] == 0) {
					p->cost[_reg_NT] = 0;
					p->rule._reg = q->rule._reg;
				}
				if (q->cost[_reg32_NT] == 0) {
					p->cost[_reg32_NT] = 0;
					p->rule._reg32 = q->rule._reg32;
				}
				if (q->cost[_regf64_NT] == 0) {
					p->cost[_regf64_NT] = 0;
					p->rule._regf64 = q->rule._regf64;
				}
				if (q->cost[_con_NT] == 0) {
					p->cost[_con_NT] = 0;
					p->rule._con = q->rule._con;
				}
				if (q->cost[_c0_NT] == 0) {
					p->cost[_c0_NT] = 0;
					p->rule._c0 = q->rule._c0;
				}
				if (q->cost[_c1_NT] == 0) {
					p->cost[_c1_NT] = 0;
					p->rule._c1 = q->rule._c1;
				}
				if (q->cost[_c2_NT] == 0) {
					p->cost[_c2_NT] = 0;
					p->rule._c2 = q->rule._c2;
				}
				if (q->cost[_con1_NT] == 0) {
					p->cost[_con1_NT] = 0;
					p->rule._con1 = q->rule._con1;
				}
				if (q->cost[_con2_NT] == 0) {
					p->cost[_con2_NT] = 0;
					p->rule._con2 = q->rule._con2;
				}
				if (q->cost[_con4_NT] == 0) {
					p->cost[_con4_NT] = 0;
					p->rule._con4 = q->rule._con4;
				}
				if (q->cost[_creg_NT] == 0) {
					p->cost[_creg_NT] = 0;
					p->rule._creg = q->rule._creg;
				}
				if (q->cost[_dref16_NT] == 0) {
					p->cost[_dref16_NT] = 0;
					p->rule._dref16 = q->rule._dref16;
				}
				if (q->cost[_dref8_NT] == 0) {
					p->cost[_dref8_NT] = 0;
					p->rule._dref8 = q->rule._dref8;
				}
				if (q->cost[_faddr_NT] == 0) {
					p->cost[_faddr_NT] = 0;
					p->rule._faddr = q->rule._faddr;
				}
				if (q->cost[_mem_NT] == 0) {
					p->cost[_mem_NT] = 0;
					p->rule._mem = q->rule._mem;
				}
				if (q->cost[_vaddr_NT] == 0) {
					p->cost[_vaddr_NT] = 0;
					p->rule._vaddr = q->rule._vaddr;
				}
				if (q->cost[_addr_NT] == 0) {
					p->cost[_addr_NT] = 0;
					p->rule._addr = q->rule._addr;
				}
				if (q->cost[_ar_NT] == 0) {
					p->cost[_ar_NT] = 0;
					p->rule._ar = q->rule._ar;
				}
			}
			c = 0;
			if (c + 0 < p->cost[_reg32_NT]) {
				p->cost[_reg32_NT] = c + 0;
				p->rule._reg32 = 4;
				_closure_reg32(a, c + 0);
			}
		}
		/* reg32: INDIRU4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 4;
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 58;
			_closure_reg32(a, c + 0);
		}
		/* reg32: INDIRU4(faddr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + 4;
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 59;
			_closure_reg32(a, c + 0);
		}
		/* mem: INDIRU4(faddr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + 0;
		if (c + 0 < p->cost[_mem_NT]) {
			p->cost[_mem_NT] = c + 0;
			p->rule._mem = 2;
		}
		break;
	case 4209: /* CVFF4 */
		_label(LEFT_CHILD(a));
		/* regf64: CVFF4(regf64) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + 4;
		if (c + 0 < p->cost[_regf64_NT]) {
			p->cost[_regf64_NT] = c + 0;
			p->rule._regf64 = 5;
			_closure_regf64(a, c + 0);
		}
		break;
	case 4213: /* CVFI4 */
		_label(LEFT_CHILD(a));
		/* reg32: CVFI4(regf64) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + (isfloat8(a));
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 5;
			_closure_reg32(a, c + 0);
		}
		/* reg32: CVFI4(regf64) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + (isfloat4(a));
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 6;
			_closure_reg32(a, c + 0);
		}
		break;
	case 4225: /* CVIF4 */
		_label(LEFT_CHILD(a));
		/* regf64: CVIF4(reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + 4;
		if (c + 0 < p->cost[_regf64_NT]) {
			p->cost[_regf64_NT] = c + 0;
			p->rule._regf64 = 7;
			_closure_regf64(a, c + 0);
		}
		/* regf64: CVIF4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 4;
		if (c + 0 < p->cost[_regf64_NT]) {
			p->cost[_regf64_NT] = c + 0;
			p->rule._regf64 = 8;
			_closure_regf64(a, c + 0);
		}
		break;
	case 4229: /* CVII4 */
		_label(LEFT_CHILD(a));
		/* reg32: CVII4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (move(a));
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 10;
			_closure_reg32(a, c + 0);
		}
		break;
	case 4230: /* CVIU4 */
		_label(LEFT_CHILD(a));
		/* reg32: CVIU4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (move(a));
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 8;
			_closure_reg32(a, c + 0);
		}
		break;
	case 4277: /* CVUI4 */
		_label(LEFT_CHILD(a));
		/* reg32: CVUI4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (move(a));
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 9;
			_closure_reg32(a, c + 0);
		}
		break;
	case 4278: /* CVUU4 */
		_label(LEFT_CHILD(a));
		/* reg32: CVUU4(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + (move(a));
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 7;
			_closure_reg32(a, c + 0);
		}
		break;
	case 4289: /* NEGF4 */
		_label(LEFT_CHILD(a));
		/* regf64: NEGF4(regf64) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + 5;
		if (c + 0 < p->cost[_regf64_NT]) {
			p->cost[_regf64_NT] = c + 0;
			p->rule._regf64 = 27;
			_closure_regf64(a, c + 0);
		}
		if (	/* regf64: NEGF4(INDIRF4(faddr)) */
			LEFT_CHILD(a)->op == 4161 /* INDIRF4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_faddr_NT] + 5;
			if (c + 0 < p->cost[_regf64_NT]) {
				p->cost[_regf64_NT] = c + 0;
				p->rule._regf64 = 28;
				_closure_regf64(a, c + 0);
			}
		}
		break;
	case 4293: /* NEGI4 */
		_label(LEFT_CHILD(a));
		/* reg32: NEGI4(reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + 5;
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 46;
			_closure_reg32(a, c + 0);
		}
		if (	/* reg32: NEGI4(INDIRI4(faddr)) */
			LEFT_CHILD(a)->op == 4165 /* INDIRI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_faddr_NT] + 5;
			if (c + 0 < p->cost[_reg32_NT]) {
				p->cost[_reg32_NT] = c + 0;
				p->rule._reg32 = 47;
				_closure_reg32(a, c + 0);
			}
		}
		break;
	case 4305: /* CALLF4 */
		_label(LEFT_CHILD(a));
		/* regf64: CALLF4(ar) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_ar_NT] + 3;
		if (c + 0 < p->cost[_regf64_NT]) {
			p->cost[_regf64_NT] = c + 0;
			p->rule._regf64 = 35;
			_closure_regf64(a, c + 0);
		}
		/* reg: CALLF4(ar) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_ar_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 98;
			_closure_reg(a, c + 0);
		}
		break;
	case 4309: /* CALLI4 */
		_label(LEFT_CHILD(a));
		/* reg32: CALLI4(ar) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_ar_NT] + 3;
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 60;
			_closure_reg32(a, c + 0);
		}
		/* reg: CALLI4(ar) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_ar_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 96;
			_closure_reg(a, c + 0);
		}
		break;
	case 4310: /* CALLU4 */
		_label(LEFT_CHILD(a));
		/* reg32: CALLU4(ar) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_ar_NT] + 3;
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 61;
			_closure_reg32(a, c + 0);
		}
		/* reg: CALLU4(ar) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_ar_NT] + 1;
		if (c + 0 < p->cost[_reg_NT]) {
			p->cost[_reg_NT] = c + 0;
			p->rule._reg = 97;
			_closure_reg(a, c + 0);
		}
		break;
	case 4321: /* LOADF4 */
		_label(LEFT_CHILD(a));
		/* regf64: LOADF4(regf64) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + (move(a));
		if (c + 0 < p->cost[_regf64_NT]) {
			p->cost[_regf64_NT] = c + 0;
			p->rule._regf64 = 1;
			_closure_regf64(a, c + 0);
		}
		break;
	case 4325: /* LOADI4 */
		_label(LEFT_CHILD(a));
		/* reg32: LOADI4(reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + (move(a));
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 1;
			_closure_reg32(a, c + 0);
		}
		break;
	case 4326: /* LOADU4 */
		_label(LEFT_CHILD(a));
		/* reg32: LOADU4(reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + (move(a));
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 2;
			_closure_reg32(a, c + 0);
		}
		break;
	case 4337: /* RETF4 */
		_label(LEFT_CHILD(a));
		/* stmt: RETF4(regf64) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 219;
		}
		break;
	case 4341: /* RETI4 */
		_label(LEFT_CHILD(a));
		/* stmt: RETI4(reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 217;
		}
		break;
	case 4342: /* RETU4 */
		_label(LEFT_CHILD(a));
		/* stmt: RETU4(reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 218;
		}
		break;
	case 4401: /* ADDF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* regf64: ADDF4(regf64,regf64) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_regf64_NT] + 5;
		if (c + 0 < p->cost[_regf64_NT]) {
			p->cost[_regf64_NT] = c + 0;
			p->rule._regf64 = 11;
			_closure_regf64(a, c + 0);
		}
		if (	/* regf64: ADDF4(regf64,INDIRF4(faddr)) */
			RIGHT_CHILD(a)->op == 4161 /* INDIRF4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 5;
			if (c + 0 < p->cost[_regf64_NT]) {
				p->cost[_regf64_NT] = c + 0;
				p->rule._regf64 = 12;
				_closure_regf64(a, c + 0);
			}
		}
		break;
	case 4405: /* ADDI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg32: ADDI4(reg32,reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg32_NT] + 5;
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 11;
			_closure_reg32(a, c + 0);
		}
		if (	/* reg32: ADDI4(reg32,INDIRI4(faddr)) */
			RIGHT_CHILD(a)->op == 4165 /* INDIRI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 5;
			if (c + 0 < p->cost[_reg32_NT]) {
				p->cost[_reg32_NT] = c + 0;
				p->rule._reg32 = 12;
				_closure_reg32(a, c + 0);
			}
		}
		break;
	case 4406: /* ADDU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg32: ADDU4(reg32,reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg32_NT] + 5;
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 13;
			_closure_reg32(a, c + 0);
		}
		if (	/* reg32: ADDU4(reg32,INDIRU4(faddr)) */
			RIGHT_CHILD(a)->op == 4166 /* INDIRU4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 5;
			if (c + 0 < p->cost[_reg32_NT]) {
				p->cost[_reg32_NT] = c + 0;
				p->rule._reg32 = 14;
				_closure_reg32(a, c + 0);
			}
		}
		break;
	case 4417: /* SUBF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* regf64: SUBF4(regf64,regf64) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_regf64_NT] + 5;
		if (c + 0 < p->cost[_regf64_NT]) {
			p->cost[_regf64_NT] = c + 0;
			p->rule._regf64 = 15;
			_closure_regf64(a, c + 0);
		}
		if (	/* regf64: SUBF4(regf64,INDIRF4(faddr)) */
			RIGHT_CHILD(a)->op == 4161 /* INDIRF4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 5;
			if (c + 0 < p->cost[_regf64_NT]) {
				p->cost[_regf64_NT] = c + 0;
				p->rule._regf64 = 16;
				_closure_regf64(a, c + 0);
			}
		}
		break;
	case 4421: /* SUBI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg32: SUBI4(reg32,reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg32_NT] + 5;
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 15;
			_closure_reg32(a, c + 0);
		}
		if (	/* reg32: SUBI4(reg32,INDIRI4(faddr)) */
			RIGHT_CHILD(a)->op == 4165 /* INDIRI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 5;
			if (c + 0 < p->cost[_reg32_NT]) {
				p->cost[_reg32_NT] = c + 0;
				p->rule._reg32 = 16;
				_closure_reg32(a, c + 0);
			}
		}
		break;
	case 4422: /* SUBU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg32: SUBU4(reg32,reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg32_NT] + 5;
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 17;
			_closure_reg32(a, c + 0);
		}
		if (	/* reg32: SUBU4(reg32,INDIRU4(faddr)) */
			RIGHT_CHILD(a)->op == 4166 /* INDIRU4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 2;
			if (c + 0 < p->cost[_reg32_NT]) {
				p->cost[_reg32_NT] = c + 0;
				p->rule._reg32 = 18;
				_closure_reg32(a, c + 0);
			}
		}
		break;
	case 4437: /* LSHI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg32: LSHI4(reg32,creg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_creg_NT] + 6;
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 48;
			_closure_reg32(a, c + 0);
		}
		/* reg32: LSHI4(reg32,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 6;
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 49;
			_closure_reg32(a, c + 0);
		}
		break;
	case 4438: /* LSHU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg32: LSHU4(reg32,creg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_creg_NT] + 6;
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 50;
			_closure_reg32(a, c + 0);
		}
		/* reg32: LSHU4(reg32,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 6;
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 51;
			_closure_reg32(a, c + 0);
		}
		break;
	case 4453: /* MODI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg32: MODI4(reg32,reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg32_NT] + 8;
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 42;
			_closure_reg32(a, c + 0);
		}
		if (	/* reg32: MODI4(reg32,INDIRI4(faddr)) */
			RIGHT_CHILD(a)->op == 4165 /* INDIRI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 8;
			if (c + 0 < p->cost[_reg32_NT]) {
				p->cost[_reg32_NT] = c + 0;
				p->rule._reg32 = 43;
				_closure_reg32(a, c + 0);
			}
		}
		break;
	case 4454: /* MODU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg32: MODU4(reg32,reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg32_NT] + 8;
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 44;
			_closure_reg32(a, c + 0);
		}
		if (	/* reg32: MODU4(reg32,INDIRU4(faddr)) */
			RIGHT_CHILD(a)->op == 4166 /* INDIRU4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 8;
			if (c + 0 < p->cost[_reg32_NT]) {
				p->cost[_reg32_NT] = c + 0;
				p->rule._reg32 = 45;
				_closure_reg32(a, c + 0);
			}
		}
		break;
	case 4469: /* RSHI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg32: RSHI4(reg32,creg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_creg_NT] + 6;
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 52;
			_closure_reg32(a, c + 0);
		}
		/* reg32: RSHI4(reg32,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 6;
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 53;
			_closure_reg32(a, c + 0);
		}
		break;
	case 4470: /* RSHU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg32: RSHU4(reg32,creg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_creg_NT] + 6;
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 54;
			_closure_reg32(a, c + 0);
		}
		/* reg32: RSHU4(reg32,reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg_NT] + 6;
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 55;
			_closure_reg32(a, c + 0);
		}
		break;
	case 4485: /* BANDI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg32: BANDI4(reg32,reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg32_NT] + 5;
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 19;
			_closure_reg32(a, c + 0);
		}
		if (	/* reg32: BANDI4(reg32,INDIRI4(faddr)) */
			RIGHT_CHILD(a)->op == 4165 /* INDIRI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 5;
			if (c + 0 < p->cost[_reg32_NT]) {
				p->cost[_reg32_NT] = c + 0;
				p->rule._reg32 = 20;
				_closure_reg32(a, c + 0);
			}
		}
		break;
	case 4486: /* BANDU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg32: BANDU4(reg32,reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg32_NT] + 5;
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 21;
			_closure_reg32(a, c + 0);
		}
		if (	/* reg32: BANDU4(reg32,INDIRU4(faddr)) */
			RIGHT_CHILD(a)->op == 4166 /* INDIRU4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 5;
			if (c + 0 < p->cost[_reg32_NT]) {
				p->cost[_reg32_NT] = c + 0;
				p->rule._reg32 = 22;
				_closure_reg32(a, c + 0);
			}
		}
		/* reg32: BANDU4(reg32,con) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_con_NT] + 5;
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 23;
			_closure_reg32(a, c + 0);
		}
		break;
	case 4501: /* BCOMI4 */
		_label(LEFT_CHILD(a));
		/* reg32: BCOMI4(reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + 2;
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 24;
			_closure_reg32(a, c + 0);
		}
		break;
	case 4502: /* BCOMU4 */
		_label(LEFT_CHILD(a));
		/* reg32: BCOMU4(reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + 2;
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 25;
			_closure_reg32(a, c + 0);
		}
		break;
	case 4517: /* BORI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg32: BORI4(reg32,reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg32_NT] + 5;
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 26;
			_closure_reg32(a, c + 0);
		}
		if (	/* reg32: BORI4(reg32,INDIRI4(faddr)) */
			RIGHT_CHILD(a)->op == 4165 /* INDIRI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 5;
			if (c + 0 < p->cost[_reg32_NT]) {
				p->cost[_reg32_NT] = c + 0;
				p->rule._reg32 = 27;
				_closure_reg32(a, c + 0);
			}
		}
		break;
	case 4518: /* BORU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg32: BORU4(reg32,reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg32_NT] + 5;
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 28;
			_closure_reg32(a, c + 0);
		}
		if (	/* reg32: BORU4(reg32,INDIRU4(faddr)) */
			RIGHT_CHILD(a)->op == 4166 /* INDIRU4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 5;
			if (c + 0 < p->cost[_reg32_NT]) {
				p->cost[_reg32_NT] = c + 0;
				p->rule._reg32 = 29;
				_closure_reg32(a, c + 0);
			}
		}
		break;
	case 4533: /* BXORI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg32: BXORI4(reg32,reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg32_NT] + 2;
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 30;
			_closure_reg32(a, c + 0);
		}
		if (	/* reg32: BXORI4(reg32,INDIRI4(faddr)) */
			RIGHT_CHILD(a)->op == 4165 /* INDIRI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 2;
			if (c + 0 < p->cost[_reg32_NT]) {
				p->cost[_reg32_NT] = c + 0;
				p->rule._reg32 = 31;
				_closure_reg32(a, c + 0);
			}
		}
		break;
	case 4534: /* BXORU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg32: BXORU4(reg32,reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg32_NT] + 2;
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 32;
			_closure_reg32(a, c + 0);
		}
		if (	/* reg32: BXORU4(reg32,INDIRU4(faddr)) */
			RIGHT_CHILD(a)->op == 4166 /* INDIRU4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 2;
			if (c + 0 < p->cost[_reg32_NT]) {
				p->cost[_reg32_NT] = c + 0;
				p->rule._reg32 = 33;
				_closure_reg32(a, c + 0);
			}
		}
		break;
	case 4545: /* DIVF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* regf64: DIVF4(regf64,regf64) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_regf64_NT] + 8;
		if (c + 0 < p->cost[_regf64_NT]) {
			p->cost[_regf64_NT] = c + 0;
			p->rule._regf64 = 23;
			_closure_regf64(a, c + 0);
		}
		if (	/* regf64: DIVF4(regf64,INDIRF4(faddr)) */
			RIGHT_CHILD(a)->op == 4161 /* INDIRF4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 8;
			if (c + 0 < p->cost[_regf64_NT]) {
				p->cost[_regf64_NT] = c + 0;
				p->rule._regf64 = 24;
				_closure_regf64(a, c + 0);
			}
		}
		break;
	case 4549: /* DIVI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg32: DIVI4(reg32,reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg32_NT] + 8;
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 38;
			_closure_reg32(a, c + 0);
		}
		if (	/* reg32: DIVI4(reg32,INDIRI4(faddr)) */
			RIGHT_CHILD(a)->op == 4165 /* INDIRI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 8;
			if (c + 0 < p->cost[_reg32_NT]) {
				p->cost[_reg32_NT] = c + 0;
				p->rule._reg32 = 39;
				_closure_reg32(a, c + 0);
			}
		}
		break;
	case 4550: /* DIVU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg32: DIVU4(reg32,reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg32_NT] + 8;
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 40;
			_closure_reg32(a, c + 0);
		}
		if (	/* reg32: DIVU4(reg32,INDIRU4(faddr)) */
			RIGHT_CHILD(a)->op == 4166 /* INDIRU4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 8;
			if (c + 0 < p->cost[_reg32_NT]) {
				p->cost[_reg32_NT] = c + 0;
				p->rule._reg32 = 41;
				_closure_reg32(a, c + 0);
			}
		}
		break;
	case 4561: /* MULF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* regf64: MULF4(regf64,regf64) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_regf64_NT] + 8;
		if (c + 0 < p->cost[_regf64_NT]) {
			p->cost[_regf64_NT] = c + 0;
			p->rule._regf64 = 19;
			_closure_regf64(a, c + 0);
		}
		if (	/* regf64: MULF4(regf64,INDIRF4(faddr)) */
			RIGHT_CHILD(a)->op == 4161 /* INDIRF4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 8;
			if (c + 0 < p->cost[_regf64_NT]) {
				p->cost[_regf64_NT] = c + 0;
				p->rule._regf64 = 20;
				_closure_regf64(a, c + 0);
			}
		}
		break;
	case 4565: /* MULI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg32: MULI4(reg32,reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg32_NT] + 8;
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 34;
			_closure_reg32(a, c + 0);
		}
		if (	/* reg32: MULI4(reg32,INDIRI4(faddr)) */
			RIGHT_CHILD(a)->op == 4165 /* INDIRI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 8;
			if (c + 0 < p->cost[_reg32_NT]) {
				p->cost[_reg32_NT] = c + 0;
				p->rule._reg32 = 35;
				_closure_reg32(a, c + 0);
			}
		}
		break;
	case 4566: /* MULU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* reg32: MULU4(reg32,reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg32_NT] + 8;
		if (c + 0 < p->cost[_reg32_NT]) {
			p->cost[_reg32_NT] = c + 0;
			p->rule._reg32 = 36;
			_closure_reg32(a, c + 0);
		}
		if (	/* reg32: MULU4(reg32,INDIRU4(faddr)) */
			RIGHT_CHILD(a)->op == 4166 /* INDIRU4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 8;
			if (c + 0 < p->cost[_reg32_NT]) {
				p->cost[_reg32_NT] = c + 0;
				p->rule._reg32 = 37;
				_closure_reg32(a, c + 0);
			}
		}
		break;
	case 4577: /* EQF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: EQF4(regf64,regf64) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_regf64_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 42;
		}
		if (	/* stmt: EQF4(regf64,INDIRF4(faddr)) */
			RIGHT_CHILD(a)->op == 4161 /* INDIRF4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 5;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 43;
			}
		}
		/* stmt: EQF4(mem,mem) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_mem_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 44;
		}
		break;
	case 4581: /* EQI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: EQI4(reg32,reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg32_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 36;
		}
		if (	/* stmt: EQI4(reg32,INDIRI4(faddr)) */
			RIGHT_CHILD(a)->op == 4165 /* INDIRI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 5;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 37;
			}
		}
		/* stmt: EQI4(mem,mem) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_mem_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 38;
		}
		break;
	case 4582: /* EQU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: EQU4(reg32,reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg32_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 39;
		}
		if (	/* stmt: EQU4(reg32,INDIRU4(faddr)) */
			RIGHT_CHILD(a)->op == 4166 /* INDIRU4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 5;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 40;
			}
		}
		/* stmt: EQU4(mem,mem) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_mem_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 41;
		}
		break;
	case 4593: /* GEF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: GEF4(regf64,regf64) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_regf64_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 203;
		}
		if (	/* stmt: GEF4(INDIRF4(faddr),regf64) */
			LEFT_CHILD(a)->op == 4161 /* INDIRF4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_faddr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_regf64_NT] + 5;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 204;
			}
		}
		/* stmt: GEF4(mem,mem) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_mem_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 205;
		}
		break;
	case 4597: /* GEI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: GEI4(reg32,reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg32_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 197;
		}
		if (	/* stmt: GEI4(INDIRI4(faddr),reg32) */
			LEFT_CHILD(a)->op == 4165 /* INDIRI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_faddr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg32_NT] + 5;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 198;
			}
		}
		/* stmt: GEI4(mem,mem) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_mem_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 199;
		}
		break;
	case 4598: /* GEU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: GEU4(reg32,reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg32_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 200;
		}
		if (	/* stmt: GEU4(INDIRU4(faddr),reg32) */
			LEFT_CHILD(a)->op == 4166 /* INDIRU4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_faddr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg32_NT] + 5;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 201;
			}
		}
		/* stmt: GEU4(mem,mem) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_mem_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 202;
		}
		break;
	case 4609: /* GTF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: GTF4(regf64,regf64) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_regf64_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 173;
		}
		if (	/* stmt: GTF4(INDIRF4(faddr),regf64) */
			LEFT_CHILD(a)->op == 4161 /* INDIRF4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_faddr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_regf64_NT] + 5;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 174;
			}
		}
		/* stmt: GTF4(mem,mem) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_mem_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 175;
		}
		break;
	case 4613: /* GTI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: GTI4(reg32,reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg32_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 167;
		}
		if (	/* stmt: GTI4(INDIRI4(faddr),reg32) */
			LEFT_CHILD(a)->op == 4165 /* INDIRI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_faddr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg32_NT] + 5;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 168;
			}
		}
		/* stmt: GTI4(mem,mem) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_mem_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 169;
		}
		break;
	case 4614: /* GTU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: GTU4(reg32,reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg32_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 170;
		}
		if (	/* stmt: GTU4(INDIRU4(faddr),reg32) */
			LEFT_CHILD(a)->op == 4166 /* INDIRU4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_faddr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg32_NT] + 5;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 171;
			}
		}
		/* stmt: GTU4(mem,mem) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_mem_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 172;
		}
		break;
	case 4625: /* LEF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: LEF4(regf64,regf64) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_regf64_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 143;
		}
		if (	/* stmt: LEF4(regf64,INDIRF4(faddr)) */
			RIGHT_CHILD(a)->op == 4161 /* INDIRF4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 5;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 144;
			}
		}
		/* stmt: LEF4(mem,mem) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_mem_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 145;
		}
		break;
	case 4629: /* LEI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: LEI4(reg32,reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg32_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 137;
		}
		if (	/* stmt: LEI4(reg32,INDIRI4(faddr)) */
			RIGHT_CHILD(a)->op == 4165 /* INDIRI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 5;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 138;
			}
		}
		/* stmt: LEI4(mem,mem) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_mem_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 139;
		}
		break;
	case 4630: /* LEU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: LEU4(reg32,reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg32_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 140;
		}
		if (	/* stmt: LEU4(reg32,INDIRU4(faddr)) */
			RIGHT_CHILD(a)->op == 4166 /* INDIRU4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 5;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 141;
			}
		}
		/* stmt: LEU4(mem,mem) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_mem_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 142;
		}
		break;
	case 4641: /* LTF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: LTF4(regf64,regf64) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_regf64_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 113;
		}
		if (	/* stmt: LTF4(regf64,INDIRF4(faddr)) */
			RIGHT_CHILD(a)->op == 4161 /* INDIRF4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 5;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 114;
			}
		}
		/* stmt: LTF4(mem,mem) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_mem_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 115;
		}
		break;
	case 4645: /* LTI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: LTI4(reg32,reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg32_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 107;
		}
		if (	/* stmt: LTI4(reg32,INDIRI4(faddr)) */
			RIGHT_CHILD(a)->op == 4165 /* INDIRI4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 5;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 108;
			}
		}
		/* stmt: LTI4(mem,mem) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_mem_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 109;
		}
		break;
	case 4646: /* LTU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: LTU4(reg32,reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg32_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 110;
		}
		if (	/* stmt: LTU4(reg32,INDIRU4(faddr)) */
			RIGHT_CHILD(a)->op == 4166 /* INDIRU4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 5;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 111;
			}
		}
		/* stmt: LTU4(mem,mem) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_mem_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 112;
		}
		break;
	case 4657: /* NEF4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: NEF4(regf64,regf64) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_regf64_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 82;
		}
		if (	/* stmt: NEF4(regf64,INDIRF4(faddr)) */
			RIGHT_CHILD(a)->op == 4161 /* INDIRF4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 5;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 83;
			}
		}
		/* stmt: NEF4(mem,mem) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_mem_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 84;
		}
		break;
	case 4661: /* NEI4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: NEI4(reg32,reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg32_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 76;
		}
		if (	/* stmt: NEI4(reg32,INDIRU4(faddr)) */
			RIGHT_CHILD(a)->op == 4166 /* INDIRU4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 5;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 77;
			}
		}
		/* stmt: NEI4(mem,mem) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_mem_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 78;
		}
		break;
	case 4662: /* NEU4 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: NEU4(reg32,reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_reg32_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 79;
		}
		if (	/* stmt: NEU4(reg32,INDIRU4(faddr)) */
			RIGHT_CHILD(a)->op == 4166 /* INDIRU4 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 5;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 80;
			}
		}
		/* stmt: NEU4(mem,mem) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_mem_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 81;
		}
		break;
	case 8209: /* CNSTF8 */
		break;
	case 8225: /* ARGF8 */
		_label(LEFT_CHILD(a));
		/* stmt: ARGF8(regf64) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 228;
		}
		break;
	case 8241: /* ASGNF8 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		if (	/* stmt: ASGNF8(VREGP,regf64) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			c = ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_regf64_NT] + 0;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 9;
			}
		}
		/* stmt: ASGNF8(reg,regf64) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_regf64_NT] + 4;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 251;
		}
		/* stmt: ASGNF8(faddr,regf64) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_regf64_NT] + 4;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 252;
		}
		if (	/* stmt: ASGNF8(faddr,INDIRF8(faddr)) */
			RIGHT_CHILD(a)->op == 8257 /* INDIRF8 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 4;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 253;
			}
		}
		if (	/* stmt: ASGNF8(faddr,ADDF8(mem,mem)) */
			RIGHT_CHILD(a)->op == 8497 /* ADDF8 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + (memop(a));
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 260;
			}
		}
		if (	/* stmt: ASGNF8(faddr,ADDF8(mem,mem)) */
			RIGHT_CHILD(a)->op == 8497 /* ADDF8 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + (flip_memop(a));
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 261;
			}
		}
		if (	/* stmt: ASGNF8(faddr,ADDF8(mem,mem)) */
			RIGHT_CHILD(a)->op == 8497 /* ADDF8 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + 4;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 265;
			}
		}
		if (	/* stmt: ASGNF8(faddr,SUBF8(mem,mem)) */
			RIGHT_CHILD(a)->op == 8513 /* SUBF8 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + (memop(a));
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 269;
			}
		}
		if (	/* stmt: ASGNF8(faddr,SUBF8(mem,mem)) */
			RIGHT_CHILD(a)->op == 8513 /* SUBF8 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + 4;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 273;
			}
		}
		if (	/* stmt: ASGNF8(faddr,MULF8(mem,mem)) */
			RIGHT_CHILD(a)->op == 8657 /* MULF8 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + (memop(a));
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 280;
			}
		}
		if (	/* stmt: ASGNF8(faddr,MULF8(mem,mem)) */
			RIGHT_CHILD(a)->op == 8657 /* MULF8 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + (flip_memop(a));
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 281;
			}
		}
		if (	/* stmt: ASGNF8(faddr,MULF8(mem,mem)) */
			RIGHT_CHILD(a)->op == 8657 /* MULF8 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + 4;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 285;
			}
		}
		if (	/* stmt: ASGNF8(faddr,DIVF8(mem,mem)) */
			RIGHT_CHILD(a)->op == 8641 /* DIVF8 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + (memop(a));
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 289;
			}
		}
		if (	/* stmt: ASGNF8(faddr,DIVF8(mem,mem)) */
			RIGHT_CHILD(a)->op == 8641 /* DIVF8 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_mem_NT] + 4;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 293;
			}
		}
		break;
	case 8257: /* INDIRF8 */
		_label(LEFT_CHILD(a));
		if (	/* regf64: INDIRF8(VREGP) */
			LEFT_CHILD(a)->op == 711 /* VREGP */
		) {
			if (mayrecalc(a)) {
				struct _state *q = a->syms[RX]->u.t.cse->x.state;
				if (q->cost[_stmt_NT] == 0) {
					p->cost[_stmt_NT] = 0;
					p->rule._stmt = q->rule._stmt;
				}
				if (q->cost[_reg8_NT] == 0) {
					p->cost[_reg8_NT] = 0;
					p->rule._reg8 = q->rule._reg8;
				}
				if (q->cost[_reg_NT] == 0) {
					p->cost[_reg_NT] = 0;
					p->rule._reg = q->rule._reg;
				}
				if (q->cost[_reg32_NT] == 0) {
					p->cost[_reg32_NT] = 0;
					p->rule._reg32 = q->rule._reg32;
				}
				if (q->cost[_regf64_NT] == 0) {
					p->cost[_regf64_NT] = 0;
					p->rule._regf64 = q->rule._regf64;
				}
				if (q->cost[_con_NT] == 0) {
					p->cost[_con_NT] = 0;
					p->rule._con = q->rule._con;
				}
				if (q->cost[_c0_NT] == 0) {
					p->cost[_c0_NT] = 0;
					p->rule._c0 = q->rule._c0;
				}
				if (q->cost[_c1_NT] == 0) {
					p->cost[_c1_NT] = 0;
					p->rule._c1 = q->rule._c1;
				}
				if (q->cost[_c2_NT] == 0) {
					p->cost[_c2_NT] = 0;
					p->rule._c2 = q->rule._c2;
				}
				if (q->cost[_con1_NT] == 0) {
					p->cost[_con1_NT] = 0;
					p->rule._con1 = q->rule._con1;
				}
				if (q->cost[_con2_NT] == 0) {
					p->cost[_con2_NT] = 0;
					p->rule._con2 = q->rule._con2;
				}
				if (q->cost[_con4_NT] == 0) {
					p->cost[_con4_NT] = 0;
					p->rule._con4 = q->rule._con4;
				}
				if (q->cost[_creg_NT] == 0) {
					p->cost[_creg_NT] = 0;
					p->rule._creg = q->rule._creg;
				}
				if (q->cost[_dref16_NT] == 0) {
					p->cost[_dref16_NT] = 0;
					p->rule._dref16 = q->rule._dref16;
				}
				if (q->cost[_dref8_NT] == 0) {
					p->cost[_dref8_NT] = 0;
					p->rule._dref8 = q->rule._dref8;
				}
				if (q->cost[_faddr_NT] == 0) {
					p->cost[_faddr_NT] = 0;
					p->rule._faddr = q->rule._faddr;
				}
				if (q->cost[_mem_NT] == 0) {
					p->cost[_mem_NT] = 0;
					p->rule._mem = q->rule._mem;
				}
				if (q->cost[_vaddr_NT] == 0) {
					p->cost[_vaddr_NT] = 0;
					p->rule._vaddr = q->rule._vaddr;
				}
				if (q->cost[_addr_NT] == 0) {
					p->cost[_addr_NT] = 0;
					p->rule._addr = q->rule._addr;
				}
				if (q->cost[_ar_NT] == 0) {
					p->cost[_ar_NT] = 0;
					p->rule._ar = q->rule._ar;
				}
			}
			c = 0;
			if (c + 0 < p->cost[_regf64_NT]) {
				p->cost[_regf64_NT] = c + 0;
				p->rule._regf64 = 4;
				_closure_regf64(a, c + 0);
			}
		}
		/* regf64: INDIRF8(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 4;
		if (c + 0 < p->cost[_regf64_NT]) {
			p->cost[_regf64_NT] = c + 0;
			p->rule._regf64 = 33;
			_closure_regf64(a, c + 0);
		}
		/* regf64: INDIRF8(faddr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + 4;
		if (c + 0 < p->cost[_regf64_NT]) {
			p->cost[_regf64_NT] = c + 0;
			p->rule._regf64 = 34;
			_closure_regf64(a, c + 0);
		}
		/* mem: INDIRF8(faddr) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_faddr_NT] + 0;
		if (c + 0 < p->cost[_mem_NT]) {
			p->cost[_mem_NT] = c + 0;
			p->rule._mem = 4;
		}
		break;
	case 8305: /* CVFF8 */
		_label(LEFT_CHILD(a));
		/* regf64: CVFF8(regf64) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + 4;
		if (c + 0 < p->cost[_regf64_NT]) {
			p->cost[_regf64_NT] = c + 0;
			p->rule._regf64 = 6;
			_closure_regf64(a, c + 0);
		}
		break;
	case 8321: /* CVIF8 */
		_label(LEFT_CHILD(a));
		/* regf64: CVIF8(reg32) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg32_NT] + 4;
		if (c + 0 < p->cost[_regf64_NT]) {
			p->cost[_regf64_NT] = c + 0;
			p->rule._regf64 = 9;
			_closure_regf64(a, c + 0);
		}
		/* regf64: CVIF8(reg) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_reg_NT] + 4;
		if (c + 0 < p->cost[_regf64_NT]) {
			p->cost[_regf64_NT] = c + 0;
			p->rule._regf64 = 10;
			_closure_regf64(a, c + 0);
		}
		break;
	case 8385: /* NEGF8 */
		_label(LEFT_CHILD(a));
		/* regf64: NEGF8(regf64) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + 5;
		if (c + 0 < p->cost[_regf64_NT]) {
			p->cost[_regf64_NT] = c + 0;
			p->rule._regf64 = 29;
			_closure_regf64(a, c + 0);
		}
		if (	/* regf64: NEGF8(INDIRF8(faddr)) */
			LEFT_CHILD(a)->op == 8257 /* INDIRF8 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_faddr_NT] + 5;
			if (c + 0 < p->cost[_regf64_NT]) {
				p->cost[_regf64_NT] = c + 0;
				p->rule._regf64 = 30;
				_closure_regf64(a, c + 0);
			}
		}
		break;
	case 8401: /* CALLF8 */
		_label(LEFT_CHILD(a));
		/* regf64: CALLF8(ar) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_ar_NT] + 1;
		if (c + 0 < p->cost[_regf64_NT]) {
			p->cost[_regf64_NT] = c + 0;
			p->rule._regf64 = 36;
			_closure_regf64(a, c + 0);
		}
		/* stmt: CALLF8(ar) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_ar_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 213;
		}
		break;
	case 8417: /* LOADF8 */
		_label(LEFT_CHILD(a));
		/* regf64: LOADF8(regf64) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + (move(a));
		if (c + 0 < p->cost[_regf64_NT]) {
			p->cost[_regf64_NT] = c + 0;
			p->rule._regf64 = 2;
			_closure_regf64(a, c + 0);
		}
		break;
	case 8433: /* RETF8 */
		_label(LEFT_CHILD(a));
		/* stmt: RETF8(regf64) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + 1;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 220;
		}
		break;
	case 8497: /* ADDF8 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* regf64: ADDF8(regf64,regf64) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_regf64_NT] + 5;
		if (c + 0 < p->cost[_regf64_NT]) {
			p->cost[_regf64_NT] = c + 0;
			p->rule._regf64 = 13;
			_closure_regf64(a, c + 0);
		}
		if (	/* regf64: ADDF8(regf64,INDIRF8(faddr)) */
			RIGHT_CHILD(a)->op == 8257 /* INDIRF8 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 5;
			if (c + 0 < p->cost[_regf64_NT]) {
				p->cost[_regf64_NT] = c + 0;
				p->rule._regf64 = 14;
				_closure_regf64(a, c + 0);
			}
		}
		break;
	case 8513: /* SUBF8 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* regf64: SUBF8(regf64,regf64) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_regf64_NT] + 5;
		if (c + 0 < p->cost[_regf64_NT]) {
			p->cost[_regf64_NT] = c + 0;
			p->rule._regf64 = 17;
			_closure_regf64(a, c + 0);
		}
		if (	/* regf64: SUBF8(regf64,INDIRF8(faddr)) */
			RIGHT_CHILD(a)->op == 8257 /* INDIRF8 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 5;
			if (c + 0 < p->cost[_regf64_NT]) {
				p->cost[_regf64_NT] = c + 0;
				p->rule._regf64 = 18;
				_closure_regf64(a, c + 0);
			}
		}
		break;
	case 8641: /* DIVF8 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* regf64: DIVF8(regf64,regf64) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_regf64_NT] + 8;
		if (c + 0 < p->cost[_regf64_NT]) {
			p->cost[_regf64_NT] = c + 0;
			p->rule._regf64 = 25;
			_closure_regf64(a, c + 0);
		}
		if (	/* regf64: DIVF8(regf64,INDIRF8(faddr)) */
			RIGHT_CHILD(a)->op == 8257 /* INDIRF8 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 8;
			if (c + 0 < p->cost[_regf64_NT]) {
				p->cost[_regf64_NT] = c + 0;
				p->rule._regf64 = 26;
				_closure_regf64(a, c + 0);
			}
		}
		break;
	case 8657: /* MULF8 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* regf64: MULF8(regf64,regf64) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_regf64_NT] + 8;
		if (c + 0 < p->cost[_regf64_NT]) {
			p->cost[_regf64_NT] = c + 0;
			p->rule._regf64 = 21;
			_closure_regf64(a, c + 0);
		}
		if (	/* regf64: MULF8(regf64,INDIRF8(faddr)) */
			RIGHT_CHILD(a)->op == 8257 /* INDIRF8 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 8;
			if (c + 0 < p->cost[_regf64_NT]) {
				p->cost[_regf64_NT] = c + 0;
				p->rule._regf64 = 22;
				_closure_regf64(a, c + 0);
			}
		}
		break;
	case 8673: /* EQF8 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: EQF8(regf64,regf64) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_regf64_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 45;
		}
		if (	/* stmt: EQF8(regf64,INDIRF8(faddr)) */
			RIGHT_CHILD(a)->op == 8257 /* INDIRF8 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 5;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 46;
			}
		}
		/* stmt: EQF8(mem,mem) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_mem_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 47;
		}
		break;
	case 8689: /* GEF8 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: GEF8(regf64,regf64) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_regf64_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 206;
		}
		if (	/* stmt: GEF8(INDIRF8(faddr),regf64) */
			LEFT_CHILD(a)->op == 8257 /* INDIRF8 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_faddr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_regf64_NT] + 5;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 207;
			}
		}
		/* stmt: GEF8(mem,mem) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_mem_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 208;
		}
		break;
	case 8705: /* GTF8 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: GTF8(regf64,regf64) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_regf64_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 176;
		}
		if (	/* stmt: GTF8(INDIRF8(faddr),regf64) */
			LEFT_CHILD(a)->op == 8257 /* INDIRF8 */
		) {
			c = ((struct _state *)(LEFT_CHILD(LEFT_CHILD(a))->x.state))->cost[_faddr_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_regf64_NT] + 5;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 177;
			}
		}
		/* stmt: GTF8(mem,mem) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_mem_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 178;
		}
		break;
	case 8721: /* LEF8 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: LEF8(regf64,regf64) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_regf64_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 146;
		}
		if (	/* stmt: LEF8(regf64,INDIRF8(faddr)) */
			RIGHT_CHILD(a)->op == 8257 /* INDIRF8 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 5;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 147;
			}
		}
		/* stmt: LEF8(mem,mem) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_mem_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 148;
		}
		break;
	case 8737: /* LTF8 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: LTF8(regf64,regf64) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_regf64_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 116;
		}
		if (	/* stmt: LTF8(regf64,INDIRF8(faddr)) */
			RIGHT_CHILD(a)->op == 8257 /* INDIRF8 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 5;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 117;
			}
		}
		/* stmt: LTF8(mem,mem) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_mem_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 118;
		}
		break;
	case 8753: /* NEF8 */
		_label(LEFT_CHILD(a));
		_label(RIGHT_CHILD(a));
		/* stmt: NEF8(regf64,regf64) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_regf64_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 85;
		}
		if (	/* stmt: NEF8(regf64,INDIRF8(faddr)) */
			RIGHT_CHILD(a)->op == 8257 /* INDIRF8 */
		) {
			c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_regf64_NT] + ((struct _state *)(LEFT_CHILD(RIGHT_CHILD(a))->x.state))->cost[_faddr_NT] + 5;
			if (c + 0 < p->cost[_stmt_NT]) {
				p->cost[_stmt_NT] = c + 0;
				p->rule._stmt = 86;
			}
		}
		/* stmt: NEF8(mem,mem) */
		c = ((struct _state *)(LEFT_CHILD(a)->x.state))->cost[_mem_NT] + ((struct _state *)(RIGHT_CHILD(a)->x.state))->cost[_mem_NT] + 5;
		if (c + 0 < p->cost[_stmt_NT]) {
			p->cost[_stmt_NT] = c + 0;
			p->rule._stmt = 87;
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
	case 526: /* mem: INDIRF8(faddr) */
	case 525: /* mem: INDIRF4(faddr) */
	case 524: /* mem: INDIRU4(faddr) */
	case 523: /* mem: INDIRI4(faddr) */
	case 522: /* stmt: JUMPV(ar) */
	case 521: /* stmt: ARGF8(regf64) */
	case 520: /* stmt: ARGF4(regf64) */
	case 519: /* stmt: ARGU4(reg32) */
	case 518: /* stmt: ARGI4(reg32) */
	case 517: /* stmt: ARGU2(reg) */
	case 516: /* stmt: ARGI2(reg) */
	case 515: /* stmt: ARGP2(reg) */
	case 514: /* stmt: RETV(reg) */
	case 513: /* stmt: RETF8(regf64) */
	case 512: /* stmt: RETF4(regf64) */
	case 511: /* stmt: RETU4(reg32) */
	case 510: /* stmt: RETI4(reg32) */
	case 509: /* stmt: RETP2(reg) */
	case 508: /* stmt: RETU2(reg) */
	case 507: /* stmt: RETI2(reg) */
	case 506: /* stmt: CALLF8(ar) */
	case 505: /* regf64: CALLF8(ar) */
	case 504: /* reg: CALLF4(ar) */
	case 503: /* regf64: CALLF4(ar) */
	case 501: /* stmt: CALLV(ar) */
	case 500: /* reg: CALLU4(ar) */
	case 499: /* reg32: CALLU4(ar) */
	case 498: /* reg: CALLI4(ar) */
	case 497: /* reg32: CALLI4(ar) */
	case 496: /* reg: CALLP2(ar) */
	case 495: /* reg: CALLU2(ar) */
	case 494: /* reg: CALLI2(ar) */
	case 491: /* regf64: INDIRF8(faddr) */
	case 490: /* regf64: INDIRF8(reg) */
	case 489: /* regf64: INDIRF4(faddr) */
	case 488: /* regf64: INDIRF4(reg) */
	case 487: /* reg32: INDIRU4(faddr) */
	case 486: /* reg32: INDIRU4(reg) */
	case 485: /* reg32: INDIRI4(faddr) */
	case 484: /* reg32: INDIRI4(reg) */
	case 483: /* reg: INDIRP2(addr) */
	case 482: /* reg: INDIRP2(reg) */
	case 481: /* reg: INDIRU2(addr) */
	case 480: /* reg: INDIRU2(reg) */
	case 479: /* reg: INDIRI2(addr) */
	case 478: /* reg: INDIRI2(reg) */
	case 477: /* reg: INDIRU1(addr) */
	case 476: /* reg8: INDIRU1(addr) */
	case 475: /* reg: INDIRU1(reg) */
	case 474: /* reg8: INDIRU1(reg) */
	case 473: /* reg: INDIRI1(addr) */
	case 472: /* reg8: INDIRI1(addr) */
	case 471: /* reg: INDIRI1(reg) */
	case 470: /* reg8: INDIRI1(reg) */
	case 462: /* dref8: INDIRU1(reg) */
	case 461: /* dref8: INDIRI1(reg) */
	case 460: /* dref8: INDIRU1(addr) */
	case 459: /* dref8: INDIRI1(addr) */
	case 458: /* dref16: INDIRU2(reg) */
	case 457: /* dref16: INDIRI2(reg) */
	case 456: /* dref16: INDIRP2(reg) */
	case 455: /* dref16: LOADP2(dref16) */
	case 454: /* dref16: LOADI2(dref16) */
	case 453: /* dref16: LOADU2(dref16) */
	case 452: /* dref16: INDIRU2(addr) */
	case 451: /* dref16: INDIRI2(addr) */
	case 450: /* dref16: INDIRP2(addr) */
	case 220: /* regf64: NEGF8(regf64) */
	case 218: /* regf64: NEGF4(regf64) */
	case 216: /* reg32: NEGI4(reg32) */
	case 215: /* reg: NEGI2(reg) */
	case 178: /* reg32: BCOMU4(reg32) */
	case 177: /* reg32: BCOMI4(reg32) */
	case 164: /* reg: BCOMU2(reg) */
	case 163: /* reg: BCOMI2(reg) */
	case 67: /* reg32: CVII4(reg) */
	case 66: /* reg32: CVUI4(reg) */
	case 65: /* reg32: CVIU4(reg) */
	case 64: /* reg32: CVUU4(reg) */
	case 63: /* reg: CVUU2(reg32) */
	case 62: /* reg: CVUU2(reg8) */
	case 61: /* reg: CVIU2(reg32) */
	case 60: /* reg: CVIU2(reg8) */
	case 59: /* reg8: CVUU1(reg) */
	case 58: /* reg8: CVUU1(reg32) */
	case 57: /* reg8: CVIU1(reg) */
	case 56: /* reg8: CVIU1(reg32) */
	case 55: /* reg: CVUI2(reg32) */
	case 54: /* reg: CVUI2(reg8) */
	case 53: /* reg: CVII2(reg32) */
	case 52: /* reg: CVII2(reg8) */
	case 47: /* reg8: CVUI1(reg) */
	case 46: /* reg8: CVUI1(reg32) */
	case 45: /* reg8: CVII1(reg) */
	case 44: /* reg8: CVII1(reg32) */
	case 43: /* reg: CVFI2(regf64) */
	case 42: /* reg: CVFI2(regf64) */
	case 41: /* reg32: CVFI4(regf64) */
	case 40: /* reg32: CVFI4(regf64) */
	case 39: /* regf64: CVIF8(reg) */
	case 38: /* regf64: CVIF8(reg32) */
	case 37: /* regf64: CVIF4(reg) */
	case 36: /* regf64: CVIF4(reg32) */
	case 35: /* regf64: CVFF8(regf64) */
	case 34: /* regf64: CVFF4(regf64) */
	case 15: /* regf64: LOADF8(regf64) */
	case 14: /* regf64: LOADF4(regf64) */
	case 13: /* reg32: LOADU4(reg32) */
	case 12: /* reg32: LOADI4(reg32) */
	case 11: /* reg: LOADP2(reg) */
	case 10: /* reg: LOADU2(reg32) */
	case 9: /* reg: LOADU2(reg) */
	case 8: /* reg: LOADI2(reg32) */
	case 7: /* reg: LOADI2(reg) */
	case 6: /* reg8: LOADU1(reg32) */
	case 5: /* reg8: LOADU1(reg) */
	case 4: /* reg8: LOADU1(reg8) */
	case 3: /* reg8: LOADI1(reg32) */
	case 2: /* reg8: LOADI1(reg) */
	case 1: /* reg8: LOADI1(reg8) */
		kids[0] = LEFT_CHILD(p);
		break;
	case 493: /* ar: ADDRGP2 */
	case 469: /* reg: ADDRFP2 */
	case 468: /* reg: ADDRLP2 */
	case 467: /* reg: ADDRGP2 */
	case 466: /* stmt: LABELV */
	case 448: /* faddr: ADDRFP2 */
	case 447: /* faddr: ADDRLP2 */
	case 446: /* faddr: ADDRGP2 */
	case 445: /* faddr: ADDRGP2 */
	case 82: /* con4: CNSTP2 */
	case 81: /* con4: CNSTU4 */
	case 80: /* con4: CNSTI4 */
	case 79: /* con2: CNSTU2 */
	case 78: /* con2: CNSTI2 */
	case 77: /* con1: CNSTU1 */
	case 76: /* con1: CNSTI1 */
	case 75: /* c2: CNSTU2 */
	case 74: /* c1: CNSTU2 */
	case 73: /* c0: CNSTU2 */
	case 72: /* c2: CNSTI2 */
	case 71: /* c1: CNSTI2 */
	case 70: /* c0: CNSTI2 */
	case 24: /* regf64: INDIRF8(VREGP) */
	case 23: /* regf64: INDIRF4(VREGP) */
	case 22: /* reg32: INDIRU4(VREGP) */
	case 21: /* reg32: INDIRI4(VREGP) */
	case 20: /* reg: INDIRP2(VREGP) */
	case 19: /* reg: INDIRU2(VREGP) */
	case 18: /* reg: INDIRI2(VREGP) */
	case 17: /* reg8: INDIRU1(VREGP) */
	case 16: /* reg8: INDIRI1(VREGP) */
		break;
	case 33: /* stmt: ASGNF8(VREGP,regf64) */
	case 32: /* stmt: ASGNF4(VREGP,regf64) */
	case 31: /* stmt: ASGNU4(VREGP,reg32) */
	case 30: /* stmt: ASGNI4(VREGP,reg32) */
	case 29: /* stmt: ASGNP2(VREGP,reg) */
	case 28: /* stmt: ASGNU2(VREGP,reg) */
	case 27: /* stmt: ASGNI2(VREGP,reg) */
	case 26: /* stmt: ASGNU1(VREGP,reg8) */
	case 25: /* stmt: ASGNI1(VREGP,reg8) */
		kids[0] = RIGHT_CHILD(p);
		break;
	case 50: /* reg8: LOADU1(LOADU2(CVII2(reg8))) */
	case 48: /* reg8: LOADU1(LOADU2(CVII2(reg))) */
		kids[0] = LEFT_CHILD(LEFT_CHILD(LEFT_CHILD(p)));
		break;
	case 540: /* stmt: ARGB(INDIRB(reg)) */
	case 221: /* regf64: NEGF8(INDIRF8(faddr)) */
	case 219: /* regf64: NEGF4(INDIRF4(faddr)) */
	case 217: /* reg32: NEGI4(INDIRI4(faddr)) */
	case 51: /* reg8: LOADI1(CVUI2(reg8)) */
	case 49: /* reg8: LOADI1(CVUI2(reg)) */
		kids[0] = LEFT_CHILD(LEFT_CHILD(p));
		break;
	case 528: /* mem: regf64 */
	case 527: /* mem: reg32 */
	case 492: /* ar: reg */
	case 465: /* stmt: reg */
	case 464: /* addr: vaddr */
	case 463: /* addr: faddr */
	case 91: /* creg: con */
	case 90: /* reg: con2 */
	case 89: /* reg: con1 */
	case 88: /* con: c2 */
	case 87: /* con: c1 */
	case 86: /* con: c0 */
	case 85: /* con: con4 */
	case 84: /* con: con2 */
	case 83: /* con: con1 */
	case 69: /* reg8: con */
	case 68: /* reg: con */
		kids[0] = p;
		break;
	case 551: /* stmt: ASGNF8(faddr,regf64) */
	case 550: /* stmt: ASGNF8(reg,regf64) */
	case 548: /* stmt: ASGNF4(faddr,regf64) */
	case 547: /* stmt: ASGNF4(reg,regf64) */
	case 545: /* stmt: ASGNU4(faddr,reg32) */
	case 544: /* stmt: ASGNU4(reg,reg32) */
	case 542: /* stmt: ASGNI4(faddr,reg32) */
	case 541: /* stmt: ASGNI4(reg,reg32) */
	case 538: /* stmt: ASGNP2(addr,reg) */
	case 537: /* stmt: ASGNP2(reg,reg) */
	case 536: /* stmt: ASGNU2(addr,reg) */
	case 535: /* stmt: ASGNU2(reg,reg) */
	case 534: /* stmt: ASGNI2(addr,reg) */
	case 533: /* stmt: ASGNI2(reg,reg) */
	case 532: /* stmt: ASGNU1(addr,reg8) */
	case 531: /* stmt: ASGNU1(reg,reg8) */
	case 530: /* stmt: ASGNI1(addr,reg8) */
	case 529: /* stmt: ASGNI1(reg,reg8) */
	case 502: /* stmt: CALLB(ar,reg) */
	case 449: /* vaddr: ADDP2(reg,con) */
	case 444: /* stmt: GEF8(mem,mem) */
	case 442: /* stmt: GEF8(regf64,regf64) */
	case 441: /* stmt: GEF4(mem,mem) */
	case 439: /* stmt: GEF4(regf64,regf64) */
	case 438: /* stmt: GEU4(mem,mem) */
	case 436: /* stmt: GEU4(reg32,reg32) */
	case 435: /* stmt: GEI4(mem,mem) */
	case 433: /* stmt: GEI4(reg32,reg32) */
	case 429: /* stmt: GEU2(con,reg8) */
	case 428: /* stmt: GEU2(reg8,reg8) */
	case 423: /* stmt: GEI2(con,reg8) */
	case 421: /* stmt: GEI2(reg8,reg8) */
	case 420: /* stmt: GEU2(dref16,reg) */
	case 419: /* stmt: GEU2(con,reg) */
	case 418: /* stmt: GEU2(reg,reg) */
	case 417: /* stmt: GEI2(dref16,reg) */
	case 416: /* stmt: GEI2(con,reg) */
	case 415: /* stmt: GEI2(reg,reg) */
	case 414: /* stmt: GTF8(mem,mem) */
	case 412: /* stmt: GTF8(regf64,regf64) */
	case 411: /* stmt: GTF4(mem,mem) */
	case 409: /* stmt: GTF4(regf64,regf64) */
	case 408: /* stmt: GTU4(mem,mem) */
	case 406: /* stmt: GTU4(reg32,reg32) */
	case 405: /* stmt: GTI4(mem,mem) */
	case 403: /* stmt: GTI4(reg32,reg32) */
	case 399: /* stmt: GTU2(con,reg8) */
	case 398: /* stmt: GTU2(reg8,reg8) */
	case 393: /* stmt: GTI2(con,reg8) */
	case 391: /* stmt: GTI2(reg8,reg8) */
	case 390: /* stmt: GTU2(dref16,reg) */
	case 389: /* stmt: GTU2(con,reg) */
	case 388: /* stmt: GTU2(reg,reg) */
	case 387: /* stmt: GTI2(dref16,reg) */
	case 386: /* stmt: GTI2(con,reg) */
	case 385: /* stmt: GTI2(reg,reg) */
	case 384: /* stmt: LEF8(mem,mem) */
	case 382: /* stmt: LEF8(regf64,regf64) */
	case 381: /* stmt: LEF4(mem,mem) */
	case 379: /* stmt: LEF4(regf64,regf64) */
	case 378: /* stmt: LEU4(mem,mem) */
	case 376: /* stmt: LEU4(reg32,reg32) */
	case 375: /* stmt: LEI4(mem,mem) */
	case 373: /* stmt: LEI4(reg32,reg32) */
	case 369: /* stmt: LEU2(reg8,con) */
	case 368: /* stmt: LEU2(reg8,reg8) */
	case 363: /* stmt: LEI2(reg8,con) */
	case 361: /* stmt: LEI2(reg8,reg8) */
	case 360: /* stmt: LEU2(reg,dref16) */
	case 359: /* stmt: LEU2(reg,con) */
	case 358: /* stmt: LEU2(reg,reg) */
	case 357: /* stmt: LEI2(reg,dref16) */
	case 356: /* stmt: LEI2(reg,con) */
	case 355: /* stmt: LEI2(reg,reg) */
	case 354: /* stmt: LTF8(mem,mem) */
	case 352: /* stmt: LTF8(regf64,regf64) */
	case 351: /* stmt: LTF4(mem,mem) */
	case 349: /* stmt: LTF4(regf64,regf64) */
	case 348: /* stmt: LTU4(mem,mem) */
	case 346: /* stmt: LTU4(reg32,reg32) */
	case 345: /* stmt: LTI4(mem,mem) */
	case 343: /* stmt: LTI4(reg32,reg32) */
	case 339: /* stmt: LTU2(reg8,con) */
	case 338: /* stmt: LTU2(reg8,reg8) */
	case 332: /* stmt: LTI2(reg8,con) */
	case 330: /* stmt: LTI2(reg8,reg8) */
	case 329: /* stmt: LTU2(reg,dref16) */
	case 328: /* stmt: LTU2(reg,con) */
	case 327: /* stmt: LTU2(reg,reg) */
	case 326: /* stmt: LTI2(reg,dref16) */
	case 325: /* stmt: LTI2(reg,con) */
	case 324: /* stmt: LTI2(reg,reg) */
	case 323: /* stmt: NEF8(mem,mem) */
	case 321: /* stmt: NEF8(regf64,regf64) */
	case 320: /* stmt: NEF4(mem,mem) */
	case 318: /* stmt: NEF4(regf64,regf64) */
	case 317: /* stmt: NEU4(mem,mem) */
	case 315: /* stmt: NEU4(reg32,reg32) */
	case 314: /* stmt: NEI4(mem,mem) */
	case 312: /* stmt: NEI4(reg32,reg32) */
	case 306: /* stmt: NEU2(reg8,con) */
	case 305: /* stmt: NEU2(reg8,reg8) */
	case 296: /* stmt: NEI2(reg8,con) */
	case 294: /* stmt: NEI2(reg8,reg8) */
	case 292: /* stmt: NEU2(reg,dref16) */
	case 291: /* stmt: NEU2(reg,c0) */
	case 290: /* stmt: NEU2(reg,con) */
	case 289: /* stmt: NEU2(reg,reg) */
	case 287: /* stmt: NEI2(reg,dref16) */
	case 286: /* stmt: NEI2(reg,c0) */
	case 285: /* stmt: NEI2(reg,con) */
	case 284: /* stmt: NEI2(reg,reg) */
	case 283: /* stmt: EQF8(mem,mem) */
	case 281: /* stmt: EQF8(regf64,regf64) */
	case 280: /* stmt: EQF4(mem,mem) */
	case 278: /* stmt: EQF4(regf64,regf64) */
	case 277: /* stmt: EQU4(mem,mem) */
	case 275: /* stmt: EQU4(reg32,reg32) */
	case 274: /* stmt: EQI4(mem,mem) */
	case 272: /* stmt: EQI4(reg32,reg32) */
	case 266: /* stmt: EQU2(reg8,con) */
	case 265: /* stmt: EQU2(reg8,reg8) */
	case 256: /* stmt: EQI2(reg8,con) */
	case 254: /* stmt: EQI2(reg8,reg8) */
	case 252: /* stmt: EQU2(reg,dref16) */
	case 251: /* stmt: EQU2(reg,con) */
	case 250: /* stmt: EQU2(reg,reg) */
	case 248: /* stmt: EQI2(reg,dref16) */
	case 247: /* stmt: EQI2(reg,con) */
	case 246: /* stmt: EQI2(reg,reg) */
	case 245: /* reg32: RSHU4(reg32,reg) */
	case 244: /* reg32: RSHU4(reg32,creg) */
	case 243: /* reg32: RSHI4(reg32,reg) */
	case 242: /* reg32: RSHI4(reg32,creg) */
	case 241: /* reg: RSHU2(reg,reg) */
	case 240: /* reg: RSHU2(reg,creg) */
	case 239: /* reg: RSHU2(reg,c2) */
	case 238: /* reg: RSHU2(reg,c1) */
	case 237: /* reg: RSHI2(reg,reg) */
	case 236: /* reg: RSHI2(reg,creg) */
	case 235: /* reg: RSHI2(reg,c2) */
	case 234: /* reg: RSHI2(reg,c1) */
	case 233: /* reg32: LSHU4(reg32,reg) */
	case 232: /* reg32: LSHU4(reg32,creg) */
	case 231: /* reg32: LSHI4(reg32,reg) */
	case 230: /* reg32: LSHI4(reg32,creg) */
	case 229: /* reg: LSHU2(reg,reg) */
	case 228: /* reg: LSHU2(reg,creg) */
	case 227: /* reg: LSHU2(reg,c2) */
	case 226: /* reg: LSHU2(reg,c1) */
	case 225: /* reg: LSHI2(reg,reg) */
	case 224: /* reg: LSHI2(reg,creg) */
	case 223: /* reg: LSHI2(reg,c2) */
	case 222: /* reg: LSHI2(reg,c1) */
	case 213: /* reg32: MODU4(reg32,reg32) */
	case 211: /* reg32: MODI4(reg32,reg32) */
	case 210: /* reg: MODU2(reg,reg) */
	case 209: /* reg: MODI2(reg,reg) */
	case 207: /* regf64: DIVF8(regf64,regf64) */
	case 205: /* regf64: DIVF4(regf64,regf64) */
	case 203: /* reg32: DIVU4(reg32,reg32) */
	case 201: /* reg32: DIVI4(reg32,reg32) */
	case 200: /* reg: DIVU2(reg,reg) */
	case 199: /* reg: DIVI2(reg,reg) */
	case 197: /* regf64: MULF8(regf64,regf64) */
	case 195: /* regf64: MULF4(regf64,regf64) */
	case 193: /* reg32: MULU4(reg32,reg32) */
	case 191: /* reg32: MULI4(reg32,reg32) */
	case 190: /* reg: MULU2(reg,reg) */
	case 189: /* reg: MULI2(reg,reg) */
	case 187: /* reg32: BXORU4(reg32,reg32) */
	case 185: /* reg32: BXORI4(reg32,reg32) */
	case 184: /* reg: BXORU2(reg,reg) */
	case 183: /* reg: BXORI2(reg,reg) */
	case 181: /* reg32: BORU4(reg32,reg32) */
	case 179: /* reg32: BORI4(reg32,reg32) */
	case 175: /* reg8: BORU2(reg8,con) */
	case 174: /* reg8: BORU2(reg8,reg8) */
	case 172: /* reg8: BORI2(reg8,con) */
	case 171: /* reg8: BORI2(reg8,reg8) */
	case 170: /* reg: BORU2(reg,dref16) */
	case 169: /* reg: BORU2(reg,con) */
	case 168: /* reg: BORU2(reg,reg) */
	case 167: /* reg: BORI2(reg,dref16) */
	case 166: /* reg: BORI2(reg,con) */
	case 165: /* reg: BORI2(reg,reg) */
	case 162: /* reg32: BANDU4(reg32,con) */
	case 160: /* reg32: BANDU4(reg32,reg32) */
	case 158: /* reg32: BANDI4(reg32,reg32) */
	case 156: /* reg8: BANDU2(reg8,con) */
	case 155: /* reg8: BANDU2(reg8,reg8) */
	case 151: /* reg8: BANDI2(reg8,con) */
	case 148: /* reg8: BANDI2(reg8,reg8) */
	case 147: /* reg: BANDU2(reg,dref16) */
	case 146: /* reg: BANDU2(reg,con) */
	case 145: /* reg: BANDU2(reg,reg) */
	case 144: /* reg: BANDI2(reg,dref16) */
	case 143: /* reg: BANDI2(reg,con) */
	case 142: /* reg: BANDI2(reg,reg) */
	case 140: /* regf64: SUBF8(regf64,regf64) */
	case 138: /* regf64: SUBF4(regf64,regf64) */
	case 136: /* reg32: SUBU4(reg32,reg32) */
	case 134: /* reg32: SUBI4(reg32,reg32) */
	case 132: /* reg8: SUBU2(reg8,con) */
	case 131: /* reg8: SUBU2(reg8,reg8) */
	case 129: /* reg8: SUBI2(reg8,con) */
	case 128: /* reg8: SUBI2(reg8,reg8) */
	case 127: /* reg: SUBP2(reg,dref16) */
	case 126: /* reg: SUBP2(reg,con) */
	case 125: /* reg: SUBP2(reg,reg) */
	case 124: /* reg: SUBU2(reg,dref16) */
	case 123: /* reg: SUBU2(reg,con) */
	case 122: /* reg: SUBU2(reg,reg) */
	case 121: /* reg: SUBI2(reg,dref16) */
	case 120: /* reg: SUBI2(reg,con) */
	case 119: /* reg: SUBI2(reg,reg) */
	case 117: /* regf64: ADDF8(regf64,regf64) */
	case 115: /* regf64: ADDF4(regf64,regf64) */
	case 113: /* reg32: ADDU4(reg32,reg32) */
	case 111: /* reg32: ADDI4(reg32,reg32) */
	case 108: /* reg8: ADDU2(reg8,con) */
	case 107: /* reg8: ADDU2(reg8,reg8) */
	case 103: /* reg8: ADDI2(reg8,con) */
	case 102: /* reg8: ADDI2(reg8,reg8) */
	case 99: /* reg: ADDP2(reg,reg) */
	case 98: /* reg: ADDP2(reg,con) */
	case 97: /* reg: ADDU2(reg,dref16) */
	case 96: /* reg: ADDU2(reg,con) */
	case 95: /* reg: ADDU2(reg,reg) */
	case 94: /* reg: ADDI2(reg,dref16) */
	case 93: /* reg: ADDI2(reg,con) */
	case 92: /* reg: ADDI2(reg,reg) */
		kids[0] = LEFT_CHILD(p);
		kids[1] = RIGHT_CHILD(p);
		break;
	case 311: /* stmt: NEU2(BANDU2(reg8,con),c0) */
	case 302: /* stmt: NEI2(BANDI2(reg8,con),c0) */
	case 293: /* stmt: NEU2(BANDI2(reg,con),c0) */
	case 288: /* stmt: NEI2(BANDI2(reg,con),c0) */
	case 271: /* stmt: EQU2(BANDU2(reg8,con),c0) */
	case 262: /* stmt: EQI2(BANDI2(reg8,con),c0) */
	case 253: /* stmt: EQU2(BANDU2(reg,con),c0) */
	case 249: /* stmt: EQI2(BANDI2(reg,con),c0) */
	case 101: /* reg: ADDP2(LSHU2(reg,c1),reg) */
	case 100: /* reg: ADDP2(LSHI2(reg,c1),reg) */
		kids[0] = LEFT_CHILD(LEFT_CHILD(p));
		kids[1] = RIGHT_CHILD(LEFT_CHILD(p));
		kids[2] = RIGHT_CHILD(p);
		break;
	case 443: /* stmt: GEF8(INDIRF8(faddr),regf64) */
	case 440: /* stmt: GEF4(INDIRF4(faddr),regf64) */
	case 437: /* stmt: GEU4(INDIRU4(faddr),reg32) */
	case 434: /* stmt: GEI4(INDIRI4(faddr),reg32) */
	case 432: /* stmt: GEU2(CVUU2(reg8),reg8) */
	case 430: /* stmt: GEU2(CVUU2(dref8),reg8) */
	case 427: /* stmt: GEI2(CVII2(reg8),reg8) */
	case 425: /* stmt: GEI2(CVII2(dref8),reg8) */
	case 413: /* stmt: GTF8(INDIRF8(faddr),regf64) */
	case 410: /* stmt: GTF4(INDIRF4(faddr),regf64) */
	case 407: /* stmt: GTU4(INDIRU4(faddr),reg32) */
	case 404: /* stmt: GTI4(INDIRI4(faddr),reg32) */
	case 402: /* stmt: GTU2(CVUU2(reg8),reg8) */
	case 400: /* stmt: GTU2(CVUU2(dref8),reg8) */
	case 397: /* stmt: GTI2(CVII2(reg8),reg8) */
	case 395: /* stmt: GTI2(CVII2(dref8),reg8) */
	case 364: /* stmt: LEI2(CVII2(reg8),con) */
	case 334: /* stmt: LTI2(CVUI2(reg8),con) */
	case 333: /* stmt: LTI2(CVII2(reg8),con) */
	case 307: /* stmt: NEU2(CVUU2(reg8),con) */
	case 298: /* stmt: NEI2(CVUI2(reg8),con) */
	case 297: /* stmt: NEI2(CVII2(reg8),con) */
	case 267: /* stmt: EQU2(CVUU2(reg8),con) */
	case 258: /* stmt: EQI2(CVUI2(reg8),con) */
	case 257: /* stmt: EQI2(CVII2(reg8),con) */
	case 153: /* reg8: BANDI2(CVUI2(reg8),con) */
	case 152: /* reg8: BANDI2(CVII2(reg8),con) */
	case 150: /* reg8: BANDI2(CVUI2(reg8),reg8) */
	case 149: /* reg8: BANDI2(CVII2(reg8),reg8) */
	case 104: /* reg8: ADDI2(CVII2(reg8),con) */
		kids[0] = LEFT_CHILD(LEFT_CHILD(p));
		kids[1] = RIGHT_CHILD(p);
		break;
	case 552: /* stmt: ASGNF8(faddr,INDIRF8(faddr)) */
	case 549: /* stmt: ASGNF4(faddr,INDIRF4(faddr)) */
	case 546: /* stmt: ASGNU4(faddr,INDIRU4(faddr)) */
	case 543: /* stmt: ASGNI4(faddr,INDIRI4(faddr)) */
	case 539: /* stmt: ASGNB(reg,INDIRB(reg)) */
	case 424: /* stmt: GEI2(con,CVII2(reg8)) */
	case 394: /* stmt: GTI2(con,CVII2(reg8)) */
	case 383: /* stmt: LEF8(regf64,INDIRF8(faddr)) */
	case 380: /* stmt: LEF4(regf64,INDIRF4(faddr)) */
	case 377: /* stmt: LEU4(reg32,INDIRU4(faddr)) */
	case 374: /* stmt: LEI4(reg32,INDIRI4(faddr)) */
	case 372: /* stmt: LEU2(reg8,CVUU2(reg8)) */
	case 370: /* stmt: LEU2(reg8,CVUU2(dref8)) */
	case 367: /* stmt: LEI2(reg8,CVII2(reg8)) */
	case 365: /* stmt: LEI2(reg8,CVII2(dref8)) */
	case 353: /* stmt: LTF8(regf64,INDIRF8(faddr)) */
	case 350: /* stmt: LTF4(regf64,INDIRF4(faddr)) */
	case 347: /* stmt: LTU4(reg32,INDIRU4(faddr)) */
	case 344: /* stmt: LTI4(reg32,INDIRI4(faddr)) */
	case 342: /* stmt: LTU2(reg8,CVUU2(reg8)) */
	case 340: /* stmt: LTU2(reg8,CVUU2(dref8)) */
	case 337: /* stmt: LTI2(reg8,CVII2(reg8)) */
	case 335: /* stmt: LTI2(reg8,CVII2(dref8)) */
	case 322: /* stmt: NEF8(regf64,INDIRF8(faddr)) */
	case 319: /* stmt: NEF4(regf64,INDIRF4(faddr)) */
	case 316: /* stmt: NEU4(reg32,INDIRU4(faddr)) */
	case 313: /* stmt: NEI4(reg32,INDIRU4(faddr)) */
	case 310: /* stmt: NEU2(reg8,CVUU2(reg8)) */
	case 308: /* stmt: NEU2(reg8,CVUU2(dref8)) */
	case 301: /* stmt: NEI2(reg8,CVII2(reg8)) */
	case 299: /* stmt: NEI2(reg8,CVII2(dref8)) */
	case 282: /* stmt: EQF8(regf64,INDIRF8(faddr)) */
	case 279: /* stmt: EQF4(regf64,INDIRF4(faddr)) */
	case 276: /* stmt: EQU4(reg32,INDIRU4(faddr)) */
	case 273: /* stmt: EQI4(reg32,INDIRI4(faddr)) */
	case 270: /* stmt: EQU2(reg8,CVUU2(reg8)) */
	case 268: /* stmt: EQU2(reg8,CVUU2(dref8)) */
	case 261: /* stmt: EQI2(reg8,CVII2(reg8)) */
	case 259: /* stmt: EQI2(reg8,CVII2(dref8)) */
	case 214: /* reg32: MODU4(reg32,INDIRU4(faddr)) */
	case 212: /* reg32: MODI4(reg32,INDIRI4(faddr)) */
	case 208: /* regf64: DIVF8(regf64,INDIRF8(faddr)) */
	case 206: /* regf64: DIVF4(regf64,INDIRF4(faddr)) */
	case 204: /* reg32: DIVU4(reg32,INDIRU4(faddr)) */
	case 202: /* reg32: DIVI4(reg32,INDIRI4(faddr)) */
	case 198: /* regf64: MULF8(regf64,INDIRF8(faddr)) */
	case 196: /* regf64: MULF4(regf64,INDIRF4(faddr)) */
	case 194: /* reg32: MULU4(reg32,INDIRU4(faddr)) */
	case 192: /* reg32: MULI4(reg32,INDIRI4(faddr)) */
	case 188: /* reg32: BXORU4(reg32,INDIRU4(faddr)) */
	case 186: /* reg32: BXORI4(reg32,INDIRI4(faddr)) */
	case 182: /* reg32: BORU4(reg32,INDIRU4(faddr)) */
	case 180: /* reg32: BORI4(reg32,INDIRI4(faddr)) */
	case 176: /* reg8: BORU2(reg8,CVUU2(dref16)) */
	case 173: /* reg8: BORI2(reg8,CVII2(dref16)) */
	case 161: /* reg32: BANDU4(reg32,INDIRU4(faddr)) */
	case 159: /* reg32: BANDI4(reg32,INDIRI4(faddr)) */
	case 157: /* reg8: BANDU2(reg8,CVUU2(dref16)) */
	case 154: /* reg8: BANDI2(reg8,CVII2(dref16)) */
	case 141: /* regf64: SUBF8(regf64,INDIRF8(faddr)) */
	case 139: /* regf64: SUBF4(regf64,INDIRF4(faddr)) */
	case 137: /* reg32: SUBU4(reg32,INDIRU4(faddr)) */
	case 135: /* reg32: SUBI4(reg32,INDIRI4(faddr)) */
	case 133: /* reg8: SUBU2(reg8,CVUU2(dref16)) */
	case 130: /* reg8: SUBI2(reg8,CVII2(dref16)) */
	case 118: /* regf64: ADDF8(regf64,INDIRF8(faddr)) */
	case 116: /* regf64: ADDF4(regf64,INDIRF4(faddr)) */
	case 114: /* reg32: ADDU4(reg32,INDIRU4(faddr)) */
	case 112: /* reg32: ADDI4(reg32,INDIRI4(faddr)) */
	case 109: /* reg8: ADDU2(reg8,CVUU2(dref8)) */
	case 105: /* reg8: ADDI2(reg8,CVII2(dref8)) */
		kids[0] = LEFT_CHILD(p);
		kids[1] = LEFT_CHILD(RIGHT_CHILD(p));
		break;
	case 431: /* stmt: GEU2(CVUU2(dref8),CVUU2(reg8)) */
	case 426: /* stmt: GEI2(CVII2(dref8),CVII2(reg8)) */
	case 422: /* stmt: GEI2(CVII2(reg8),CVII2(reg8)) */
	case 401: /* stmt: GTU2(CVUU2(dref8),CVUU2(reg8)) */
	case 396: /* stmt: GTI2(CVII2(dref8),CVII2(reg8)) */
	case 392: /* stmt: GTI2(CVII2(reg8),CVII2(reg8)) */
	case 371: /* stmt: LEU2(CVUU2(reg8),CVUU2(dref8)) */
	case 366: /* stmt: LEI2(CVII2(reg8),CVII2(dref8)) */
	case 362: /* stmt: LEI2(CVII2(reg8),CVII2(reg8)) */
	case 341: /* stmt: LTU2(CVUU2(reg8),CVUU2(dref8)) */
	case 336: /* stmt: LTI2(CVII2(reg8),CVII2(dref8)) */
	case 331: /* stmt: LTI2(CVII2(reg8),CVII2(reg8)) */
	case 309: /* stmt: NEU2(CVUU2(reg8),CVUU2(dref8)) */
	case 300: /* stmt: NEI2(CVII2(reg8),CVII2(dref8)) */
	case 295: /* stmt: NEI2(CVII2(reg8),CVII2(reg8)) */
	case 269: /* stmt: EQU2(CVUU2(reg8),CVUU2(dref8)) */
	case 260: /* stmt: EQI2(CVII2(reg8),CVII2(dref8)) */
	case 255: /* stmt: EQI2(CVII2(reg8),CVII2(reg8)) */
	case 110: /* reg8: ADDU2(CVUU2(reg8),CVUU2(dref8)) */
	case 106: /* reg8: ADDI2(CVII2(reg8),CVII2(dref8)) */
		kids[0] = LEFT_CHILD(LEFT_CHILD(p));
		kids[1] = LEFT_CHILD(RIGHT_CHILD(p));
		break;
	case 304: /* stmt: NEI2(BANDI2(CVUI2(reg8),con),c0) */
	case 303: /* stmt: NEI2(BANDI2(CVII2(reg8),con),c0) */
	case 264: /* stmt: EQI2(BANDI2(CVUI2(reg8),con),c0) */
	case 263: /* stmt: EQI2(BANDI2(CVII2(reg8),con),c0) */
		kids[0] = LEFT_CHILD(LEFT_CHILD(LEFT_CHILD(p)));
		kids[1] = RIGHT_CHILD(LEFT_CHILD(p));
		kids[2] = RIGHT_CHILD(p);
		break;
	case 610: /* stmt: ASGNI4(faddr,BXORU4(mem,mem)) */
	case 609: /* stmt: ASGNI4(faddr,BXORI4(mem,mem)) */
	case 608: /* stmt: ASGNU4(faddr,BXORU4(mem,mem)) */
	case 607: /* stmt: ASGNU4(faddr,BXORU4(mem,mem)) */
	case 606: /* stmt: ASGNI4(faddr,BXORI4(mem,mem)) */
	case 605: /* stmt: ASGNI4(faddr,BXORI4(mem,mem)) */
	case 604: /* stmt: ASGNI4(faddr,BORU4(mem,mem)) */
	case 603: /* stmt: ASGNI4(faddr,BORI4(mem,mem)) */
	case 602: /* stmt: ASGNU4(faddr,BORU4(mem,mem)) */
	case 601: /* stmt: ASGNU4(faddr,BORU4(mem,mem)) */
	case 600: /* stmt: ASGNI4(faddr,BORI4(mem,mem)) */
	case 599: /* stmt: ASGNI4(faddr,BORI4(mem,mem)) */
	case 598: /* stmt: ASGNI4(faddr,BANDU4(mem,mem)) */
	case 597: /* stmt: ASGNI4(faddr,BANDI4(mem,mem)) */
	case 596: /* stmt: ASGNU4(faddr,BANDU4(mem,mem)) */
	case 595: /* stmt: ASGNU4(faddr,BANDU4(mem,mem)) */
	case 594: /* stmt: ASGNI4(faddr,BANDI4(mem,mem)) */
	case 593: /* stmt: ASGNI4(faddr,BANDI4(mem,mem)) */
	case 592: /* stmt: ASGNF8(faddr,DIVF8(mem,mem)) */
	case 591: /* stmt: ASGNF4(faddr,DIVF4(mem,mem)) */
	case 590: /* stmt: ASGNU4(faddr,DIVU4(mem,mem)) */
	case 589: /* stmt: ASGNI4(faddr,DIVI4(mem,mem)) */
	case 588: /* stmt: ASGNF8(faddr,DIVF8(mem,mem)) */
	case 587: /* stmt: ASGNF4(faddr,DIVF4(mem,mem)) */
	case 586: /* stmt: ASGNU4(faddr,DIVI4(mem,mem)) */
	case 585: /* stmt: ASGNI4(faddr,DIVI4(mem,mem)) */
	case 584: /* stmt: ASGNF8(faddr,MULF8(mem,mem)) */
	case 583: /* stmt: ASGNF4(faddr,MULF4(mem,mem)) */
	case 582: /* stmt: ASGNU4(faddr,MULU4(mem,mem)) */
	case 581: /* stmt: ASGNI4(faddr,MULI4(mem,mem)) */
	case 580: /* stmt: ASGNF8(faddr,MULF8(mem,mem)) */
	case 579: /* stmt: ASGNF8(faddr,MULF8(mem,mem)) */
	case 578: /* stmt: ASGNF4(faddr,MULF4(mem,mem)) */
	case 577: /* stmt: ASGNF4(faddr,MULF4(mem,mem)) */
	case 576: /* stmt: ASGNU4(faddr,MULI4(mem,mem)) */
	case 575: /* stmt: ASGNU4(faddr,MULI4(mem,mem)) */
	case 574: /* stmt: ASGNI4(faddr,MULI4(mem,mem)) */
	case 573: /* stmt: ASGNI4(faddr,MULI4(mem,mem)) */
	case 572: /* stmt: ASGNF8(faddr,SUBF8(mem,mem)) */
	case 571: /* stmt: ASGNF4(faddr,SUBF4(mem,mem)) */
	case 570: /* stmt: ASGNU4(faddr,SUBU4(mem,mem)) */
	case 569: /* stmt: ASGNI4(faddr,SUBI4(mem,mem)) */
	case 568: /* stmt: ASGNF8(faddr,SUBF8(mem,mem)) */
	case 567: /* stmt: ASGNF4(faddr,SUBF4(mem,mem)) */
	case 566: /* stmt: ASGNU4(faddr,SUBI4(mem,mem)) */
	case 565: /* stmt: ASGNI4(faddr,SUBI4(mem,mem)) */
	case 564: /* stmt: ASGNF8(faddr,ADDF8(mem,mem)) */
	case 563: /* stmt: ASGNF4(faddr,ADDF4(mem,mem)) */
	case 562: /* stmt: ASGNU4(faddr,ADDU4(mem,mem)) */
	case 561: /* stmt: ASGNI4(faddr,ADDI4(mem,mem)) */
	case 560: /* stmt: ASGNF8(faddr,ADDF8(mem,mem)) */
	case 559: /* stmt: ASGNF8(faddr,ADDF8(mem,mem)) */
	case 558: /* stmt: ASGNF4(faddr,ADDF4(mem,mem)) */
	case 557: /* stmt: ASGNF4(faddr,ADDF4(mem,mem)) */
	case 556: /* stmt: ASGNU4(faddr,ADDI4(mem,mem)) */
	case 555: /* stmt: ASGNU4(faddr,ADDI4(mem,mem)) */
	case 554: /* stmt: ASGNI4(faddr,ADDI4(mem,mem)) */
	case 553: /* stmt: ASGNI4(faddr,ADDI4(mem,mem)) */
		kids[0] = LEFT_CHILD(p);
		kids[1] = LEFT_CHILD(RIGHT_CHILD(p));
		kids[2] = RIGHT_CHILD(RIGHT_CHILD(p));
		break;
	default:
		fatal("_kids", "Bad rule number %d\n", eruleno);
	}
}


// Emitters
static void progbeg(int argc, char *argv[]) {
    {
	union {
	    char c;
	    int i;
	} u;
	u.i = 0;
	u.c = 1;
	swap = ((int)(u.i == 1)) != IR->little_endian;
    }
    parseflags(argc,argv);

    // Long reg symbols
    longreg[R_L0] = mkreg("L0",R_L0,1,IREG);
    longreg[R_L1] = mkreg("L1",R_L1,1,IREG);
    longreg[R_L2] = mkreg("L2",R_L2,1,IREG);
    longreg[R_L3] = mkreg("L3",R_L3,1,IREG);

    // Reg symbols
    intreg[R_A] = mkreg("a",R_A,1,IREG);
    intreg[R_B] = mkreg("b",R_B,1,IREG);
    intreg[R_C] = mkreg("c",R_C,1,IREG);

    // Float symbols
    fltreg[R_F0] = mkreg("F0",R_F0,1,FREG);
    fltreg[R_F1] = mkreg("F1",R_F1,1,FREG);
    fltreg[R_F2] = mkreg("F2",R_F2,1,FREG);
    fltreg[R_F3] = mkreg("F3",R_F3,1,FREG);
    fltreg[R_F4] = mkreg("F4",R_F4,1,FREG);
    fltreg[R_F5] = mkreg("F5",R_F5,1,FREG);
    fltreg[R_F6] = mkreg("F6",R_F6,1,FREG);
    fltreg[R_F7] = mkreg("F7",R_F7,1,FREG);
    fltreg[R_F8] = mkreg("F8",R_F8,1,FREG);
    fltreg[R_F9] = mkreg("F9",R_F9,1,FREG);
    fltreg[R_F10] = mkreg("F10",R_F10,1,FREG);
    fltreg[R_F11] = mkreg("F11",R_F11,1,FREG);
    fltreg[R_F12] = mkreg("F12",R_F12,1,FREG);
    fltreg[R_F13] = mkreg("F13",R_F13,1,FREG);
    fltreg[R_F14] = mkreg("F14",R_F14,1,FREG);
    fltreg[R_F15] = mkreg("F15",R_F15,1,FREG);

    // Set up sets
    longwldcrd = mkwildcard(longreg);
    intwldcrd = mkwildcard(intreg);
    fltwldcrd = mkwildcard(fltreg);

    // Set up temp regs
    tmask[IREG] = (1<<R_L0) | (1<<R_L1) | (1<<R_L2) | (1<<R_L3) | 
		  (1<<R_A)  | (1<<R_B);
    // Set up register temps - none in our case
    vmask[IREG] = 0;

    // FP regs.
    tmask[FREG] = (1<<R_F0) | (1<<R_F1) | (1<<R_F2) | (1<<R_F3) |
                  (1<<R_F4) | (1<<R_F5) | (1<<R_F6) | (1<<R_F7) |
                  (1<<R_F8) | (1<<R_F9) | (1<<R_F10) | (1<<R_F11);
    // Set up float register temps
    vmask[FREG] = 0;
    
    print(";	Magic-1 assembly file, generated by lcc 4.2\n");
    print("\n");
/* 
 * FIXME - enable this when we have a linker
    print("	.extern	$global$\n");
*/
    
    current_seg = 0;
}

static Symbol rmap(int opk) {
    switch (optype(opk)) {
	case B:
	case P:
	    return intwldcrd;
	case I:
	case U:
	    if (opsize(opk) <= 2) {
	        return intwldcrd; 
	    } else {
		return longwldcrd;
	    }
	case F:
	    return fltwldcrd;
	default:
	    return 0;
    }
}

static void segment(int n) {
    if (n==current_seg)
	return;
    if (n == CODE)
	print("\t.cseg\n");
    else if (n == LIT)
#if 1 // combine lit with dseg
	print("\t.dseg\n");
#else
	print("\t.lit\n");
#endif
    else if (n == DATA)
	print("\t.dseg\n");
    else if (n == BSS)
	print("\t.bss\n");
    else
	print("\tERROR - unknown segment %d\n",n);
    current_seg = n;
}

// NOTE - remove these when I have a real linker.  It will take care of
// defining these symbols.
static void progend(void) {
    if (ever_used_floats) {
	// Emit dummy ref to bring in floating point support code
	print("\t.dseg\n");
	printf("\t.extern\t__fp_hook\n");
	print("\t.defw\t__fp_hook\n");
    }
    print("\t.end\n");
}

int iscvt(Node p) {
    return ((generic(p->op)==CVI) || (generic(p->op)==CVU));
}

static void target(Node p) {
    assert(p);

    if (optype(p->op) == F) {
	if (!used_floats) {
	    printf("; used_floats -> TRUE\n");
            ever_used_floats = used_floats = 1;
	}
    } else if (opsize(p->op) > 2) {
	if (!used_longs) {
	   printf("; used_longs -> TRUE\n");
           used_longs = 1;
	}
    }
    
    switch (specific(p->op)) {


	case BCOM+I:
	case BCOM+U:
	case NEG+I:
	case NEG+U:
		/* going to set a=0, and then a-b */
	    if (opsize(p->op)<=2) {
		rtarget(p,0,intreg[R_B]);
		setreg(p,intreg[R_A]);
		if (iscvt(p->kids[0])) {
		    rtarget(p->kids[0],0,intreg[R_A]);
		}
	    }
	    break;

	case ADD+I:
	case ADD+U:
	case SUB+I:
	case SUB+U:
	case SUB+P:
	case BOR+I:
	case BOR+U:
	case BAND+I:
	case BAND+U:
	case BXOR+I:
	case BXOR+U:
	    if (opsize(p->op)<=2) {
	        rtarget(p,0,intreg[R_A]);
		if (iscvt(p->kids[0])) {
		    rtarget(p->kids[0],0,intreg[R_A]);
		}
	        setreg(p,intreg[R_A]);
	    }
	    break;
	case ADD+P:
	    break;
	case LSH+I:
	case LSH+U:
	case RSH+I:
	case RSH+U:
	// COPY32 must preserve B
	//   ... why?  is this true?  verify.  FIXME
	    if (opsize(p->op)<=2) {
	        rtarget(p,0,intreg[R_A]);
		if (iscvt(p->kids[0])) {
		    rtarget(p->kids[0],0,intreg[R_A]);
		}
	        setreg(p,intreg[R_A]);
	    }
	    rtarget(p,1,intreg[R_B]);
	    break;
	
	case MUL+I:
	case MUL+U:
	case DIV+I:
	case DIV+U:
	case MOD+I:
	case MOD+U:
	    if (opsize(p->op)<=2) {
		rtarget(p,0,intreg[R_A]);
		if (iscvt(p->kids[0])) {
		    rtarget(p->kids[0],0,intreg[R_A]);
		}
		rtarget(p,1,intreg[R_B]);
		setreg(p,intreg[R_A]);
	    } 
	    break;

	case CALL+I:
	case CALL+U:
	case CALL+P:
	    if (opsize(p->op)<=2) {
   	        setreg(p,intreg[R_A]);
	    }
	    break;

	case CALL+B:
	    // Yes, this is correct (I think). We need the pointer to the return
	    // area to show up in A.  FIXME - verify what happens if the
	    // caller treats this function as a procedure and doesn't use the
	    // result.  Is some space allocated anyway?
	    rtarget(p,1,intreg[R_A]);
	    break;

	case EQ+I:
	case EQ+U:
	case NE+I:
	case NE+U:
	case LE+I:
	case LE+U:
	case LT+I:
	case LT+U:
	    if (opsize(p->op) <= 2) {
		rtarget(p,0,intreg[R_A]);
		if (iscvt(p->kids[0])) {
		    rtarget(p->kids[0],0,intreg[R_A]);
		}
		rtarget(p,1,intreg[R_B]);
	    } 
	    break;

	    // Swapping operands for these to normalize to LE & LT
	case GT+I:
	case GT+U:
        case GE+I:
	case GE+U:
	    if (opsize(p->op) <= 2) {
		rtarget(p,1,intreg[R_A]);
		if (iscvt(p->kids[1])) {
		    rtarget(p->kids[1],0,intreg[R_A]);
		}
		rtarget(p,0,intreg[R_B]);
	    }
	    break;
	
	case RET+I:
	case RET+U:
	case RET+P:
	    if (opsize(p->op) <= 2) {
		rtarget(p,0,intreg[R_A]);
	    } else {
	        rtarget(p,0,longreg[R_L0]);
 	    }
	    break;

	case RET+F:
	    rtarget(p,0,fltreg[R_F0]);
	    if (opsize(p->op) == 8) {
		double_ptr = 1;
	    }
	    break;

	case CVU+U:
	case CVU+I:
            // No unsigned extension for B, so must leave in A for <= 2; mem is either because do not need sratch reg
	    if ((opsize(p->op) == 2) && (opsize(p->kids[0]->op) == 1)) {
		setreg(p,intreg[R_A]);
		rtarget(p,0,intreg[R_A]);
	    }
	    break;

	case CVF+I:
	    if (opsize(p->op) == 2) {
		// Converting float to 16-bit int/unsigned
		setreg(p,intreg[R_A]);
	    }
	    break;
	case CVI+F:
	    if (opsize(p->kids[0]->op)==2) {
		// Converting 16-bit int/unsigned to float
		rtarget(p,0,intreg[R_A]);
	    }
	    break;
        
	case CVI+U:
        case CVI+I:
            // When doing int -> long, pass in in A to because need logical ops that only work on A
	    if ((opsize(p->op) == 4) && (opsize(p->kids[0]->op) == 2)) {
		rtarget(p,0,intreg[R_A]);
	    }
	    break;

	case ARG+B:
	    // Make sure pointer to source is in B so we can easily memcpy
	    rtarget(p->kids[0],0,intreg[R_B]);
	    break;

	case ASGN+B:
	    // Dst addr in A, src addr in B.
	    rtarget(p,0,intreg[R_A]);
	    rtarget(p->kids[1],0,intreg[R_B]);
	    break;

        case INDIR+I:
        case INDIR+U:
        case INDIR+F:
	    // Src addr in B for easier memcpy
	    if (opsize(p->op) > 2) {
		rtarget(p,0,intreg[R_B]);
	    }
	    break;
    }
}

// Only real registers can be clobbered.  If we've already done a setreg
// for the result, make sure we don't clobber it here.
// Shouldn't we clobber for 16-bit MUL/DIV?
static void clobber(Node p) {
    assert(p);
    switch(specific(p->op)) {
	case MUL+I:
	case MUL+U:
	case DIV+I:
	case DIV+U:
	case MOD+I:
	case MOD+U:
	case CALL+I:
	case CALL+P:
	case CALL+U:
	// Would normally kill both, but if we're returning result
	// in A we've already done a setreg on it so don't spill.
	    if (opsize(p->op > 2)) { 
		spill( (1<<R_A) | (1<<R_B) , IREG , p );
	    } else {
		spill( (1<<R_B) , IREG , p );
	    }
	    break;
	case ASGN+B:
	case CALL+V:
        case ARG+B:
	case CALL+F:
	case CVI+F:
	case EQ+F:
	case NE+F:
	case LE+F:
	case LT+F:
	case GT+F:
	case GE+F:
	// always spill both A & B
	    spill( (1<<R_B) | (1<<R_A) ,IREG,p);
	    break;
	case CALL+B:
	// I don't really understand this.  I want to show that
	// both A and B are killed, but if I spill A here I get the
	// spill assert as if A had been setreg'd - but I don't.
	    spill( (1<<R_B) , IREG , p);
	    break;
	case CVF+I:
	    // We've done setreg on A, kill B
	    spill( (1<<R_B) , IREG , p);
	    break;
	default:
	// spill everything for generic long operation.
	    if (opsize(p->op) > 2) {
		spill((1<<R_A)|(1<<R_B),IREG,p);
	    }
	    break;
    }
}


int isfloat8(Node p) {
        assert(p);
	assert(p->kids[0]);
	if (opsize(p->kids[0]->op)==8)
	    return 3;
	else
	    return LBURG_MAX;
}

int isfloat4(Node p) {
        assert(p);
	assert(p->kids[0]);
	if (opsize(p->kids[0]->op)==4)
	    return 3;
	else
	    return LBURG_MAX;
}


// Looks for a a = b <op> a cases
int flip_memop(Node p) {
        assert(p);
        assert(generic(p->op) == ASGN);
        assert(p->kids[0]);
        assert(p->kids[1]);
	if (generic(p->kids[1]->kids[1]->op) == INDIR
        && sametree(p->kids[0], p->kids[1]->kids[1]->kids[0])) {
                return 3; 
	} else
                return LBURG_MAX;
}

// Looks for a <op>= b or a = a <op> b cases
int memop(Node p) {
        assert(p);
        assert(generic(p->op) == ASGN);
        assert(p->kids[0]);
        assert(p->kids[1]);
        if (generic(p->kids[1]->kids[0]->op) == INDIR
        && sametree(p->kids[0], p->kids[1]->kids[0]->kids[0]))
                return 3;
        else
                return LBURG_MAX;
}
int sametree(Node p, Node q) {
        return p == NULL && q == NULL
        || p && q && p->op == q->op && p->syms[0] == q->syms[0]
                && sametree(p->kids[0], q->kids[0])
                && sametree(p->kids[1], q->kids[1]);
}

int isfptr(Node n, int iftrue, int iffalse) {
   if (!n->syms[0]->generated && isfunc(n->syms[0]->type))
       return iftrue;
   else
       return iffalse;
}

static void emit2(Node p) {
    int op = specific(p->op); 

    switch( op ) {
       case RET+I:
       case RET+U:
       case RET+F:
           if (opsize(p->op) == 4) {
              print("\tld.16\ta,2+%s\n",p->kids[0]->syms[2]->x.name);
              print("\tld.16\tb,%s\n",p->kids[0]->syms[2]->x.name);
           } else if (opsize(p->op)==8) {
	      print("\tld.16\ta,%d+%d(sp)\n",double_ptr,framesize);
	      print("\tCOPY64(0(a),%s)\n",fltreg[R_F0]->x.name);
	   }
           break;
       case ARG+F: 
       case ARG+P: 
       case ARG+I: 
       case ARG+U: 
	   if (opsize(p->op) <= 2) {
	       print("\tst.16\t%d(sp),%s\n",p->syms[2]->u.c.v.i,p->kids[0]->syms[2]->x.name); 
	   } else if (opsize(p->op) == 4) {
	       print("\tCOPY32(%d(sp),%s)\n",p->syms[2]->u.c.v.i,p->kids[0]->syms[2]->x.name); 
	   } else {
	       print("\tCOPY64(%d(sp),%s)\n",p->syms[2]->u.c.v.i,p->kids[0]->syms[2]->x.name); 
	   }
           break;
	
        case ARG+B:
	       print("\tld.16\tc,%d\n\tlea\ta,%d(sp)\n\tmemcopy\n",p->syms[0]->u.c.v.i,p->syms[2]->u.c.v.i);
	   break;

    }
}

static void doarg(Node p) {
    static int argno;
    if (argoffset==0) {
	argoffset = 2;
	argno = 0;
    }
    p->x.argno=argno++;
    p->syms[2] = intconst(mkactual(2,p->syms[0]->u.c.v.i));
}

// Block operators not needed
static void blkfetch(int k, int off, int reg, int tmp) {}
static void blkstore(int k, int off, int reg, int tmp) {}
static void blkloop(int dreg, int doff, int sreg, int soff,int size, int tmps[]) {}

static void local(Symbol p) {
#if 0 
    // FIXME - why am I doing this?   The x86 had to differentiate, but
    // I don't.
    if (isfloat(p->type)) {
	p->sclass = AUTO;
    }
#endif
    if (askregvar(p,(*IR->x.rmap)(ttob(p->type)))==0) {
	mkauto(p);
    }
}


// FIXME - I appears as if I'm allocating an additional 2 bytes for
// the outgoing argument build area.  Not a big problem, but why?
// I may be confused by doarg().  I intend that starting point of
// 2 to be to skip over the frame pointer.  Perhaps I should redo my
// address math.
static void function(Symbol f, Symbol caller[], Symbol callee[], int n) {
    int i;

    used_longs = used_floats = 0;

    double_ptr = 0;

    print("%s:\n",f->x.name);
    usedmask[0] = usedmask[1] = 0;
    freemask[0] = freemask[1] = ~(unsigned)0;

    offset = 0;
    for (i=0; callee[i]; i++) {
	Symbol p = callee[i];
	Symbol q = caller[i];
	assert(q);
	p->x.offset = q->x.offset = offset; /* + A_T0_STORE_SIZE; */
	p->x.name = q->x.name = stringf("%d",p->x.offset);
	p->sclass = q->sclass = AUTO;
	offset += roundup(q->type->size,2);
    }
    assert(caller[i] == 0);
    maxoffset = offset = 0;
    maxargoffset = 0;

    // Generate code
    gencode(caller,callee);

    // Allocate space for any pseudo regs we used
    if (used_longs) {
        for (i=R_L0;i<=R_L3;i++) {
	    if (usedmask[0] & (1<<i)) {
	        maxoffset+=4;
	        longreg[i]->x.offset = -maxoffset;
	    }
        }
    }
    if (used_floats) {
        for (i=R_F0;i<=R_F15;i++) {
	    if (usedmask[1] & (1<<i)) {
	        maxoffset+=8;
	        fltreg[i]->x.offset = -maxoffset;
	    }
        }
    }

    if (double_ptr) {
	maxoffset += 2;
	double_ptr = -maxoffset;
    }

    // Now, set the frame size


    framesize = maxoffset + maxargoffset;
    if (framesize > 0) {
	// Allow for FP created by ENTER instruction
	framesize += 2;
    }

    // Rewrite names of used long and float regs now that we know framesize
    if (used_longs) {
        for (i=R_L0;i<=R_L3;i++) {
	    if (usedmask[0] & (1<<i)) {
	        longreg[i]->x.name = stringf("%d(sp)",longreg[i]->x.offset+framesize);
		printf("; equating L%d to %d(sp)\n",i,longreg[i]->x.offset+framesize);
	    }
        }
    }
    if (used_floats) {
        for (i=R_F0;i<=R_F15;i++) {
	    if (usedmask[1] & (1<<i)) {
	        fltreg[i]->x.name = stringf("%d(sp)",fltreg[i]->x.offset+framesize);
		printf("; equating F%d to %d(sp)\n",i,fltreg[i]->x.offset+framesize);
	    }
        }
    }


    // Gen entry code
    if (framesize > 0) {
        print("\tenter\t%d\n",framesize-2);
    }
    if (isstruct(freturn(f->type))) {
	print("\tst.16\t-2+%d(sp),a\n",framesize);
    }
    if (double_ptr) {
	print("\tst.16\t%d+%d(sp),a\n",double_ptr,framesize);
    }
    emitcode();
    if (framesize > 0) {
        print("\tleave\n"); 
    }
    printf("\tret\n");
    printf("\n");
}

static void defsymbol(Symbol p) {
    if (p->scope >= LOCAL && p->sclass == STATIC)
	p->x.name = stringf("L%d", genlabel(1));
    else if (p->generated)
	p->x.name = stringf("L%s", p->name);
    else if (p->scope == GLOBAL || p->sclass == EXTERN)
	p->x.name = stringf("_%s",p->name);
    else if (p->scope == CONSTANTS
	    && (isint(p->type) || isptr(p->type))
	    && p->name[0] == '0' && p->name[1] == 'x')
	p->x.name = stringf("0%sH", &p->name[2]);
    else
	p->x.name = p->name;
}

static void address(Symbol q, Symbol p, long n) {
        if (p->scope == GLOBAL
        || p->sclass == STATIC || p->sclass == EXTERN)
                q->x.name = stringf("%s%s%D",
                        p->x.name, n >= 0 ? "+" : "", n);
        else {
                assert(n <= INT_MAX && n >= INT_MIN);
                q->x.offset = p->x.offset + n;
                q->x.name = stringd(q->x.offset);
        }
}

static void defconst(int suffix, int size, Value v) {
        if (suffix == I && size == 1)
                print("	.defb 0x%x\n",   v.u & 0xff);
        else if (suffix == I && size == 2)
                print("	.defw 0x%x\n",   v.i & 0xffff);
        else if (suffix == U && size == 1)
                print("	.defb 0x%x\n", v.u & 0xff);
        else if (suffix == U && size == 2)
                print("	.defw 0x%x\n",   v.i & 0xffff);
        else if (suffix == P && size == 2)
                print("	.defw 0x%x\n", v.u & 0xffff);
        else if (suffix == F && size == 4) {
                float f = (float)v.d;
		unsigned short *p = (unsigned short*)&f;
                print("	.defw 0x%x\n", p[1]);
                print("	.defw 0x%x\n", p[0]);
	} else if (suffix == F && size == 8) {
	        double d = (double)v.d;
		unsigned short *f = (unsigned short*)&d;
                print("	.defw 0x%x\n", f[3]);
                print("	.defw 0x%x\n", f[2]);
                print("	.defw 0x%x\n", f[1]);
                print("	.defw 0x%x\n", f[0]);
        } else if (suffix == I && size == 4) {
                print("	.defw 0x%x\n",   (v.i>>16) & 0xffff);
                print("	.defw 0x%x\n",   v.i & 0xffff);
        } else if (suffix == U && size == 4) {
                print("	.defw 0x%x\n",   (v.u>>16) & 0xffff);
                print("	.defw 0x%x\n",   v.u & 0xffff);
	}
        else assert(0);
}

static void defaddress(Symbol p) {
        print("	.defw %s\n", p->x.name);
}

static void defstring(int n, char *str) {
        char *s;
        for (s = str; s < str + n; s++)
                print("	.defb %d\n", (*s)&0377);
}

static void export(Symbol p) {
    print("\t.global %s\n", p->x.name);
}

static void import(Symbol p) {
    print("\t.extern %s\n", p->x.name);
}

static void global(Symbol p) {
        assert(p->type->align == 1);
        print("%s:\n", p->x.name);
        if (p->u.seg == BSS)
                print("	.defs %d\n", p->type->size);
}

static void space(int n) {
        if (current_seg != BSS)
                print("	.defs %d\n", n);
}


Interface m1IR = {
        1, 1, 0,  /* char */
        2, 1, 0,  /* short */
        2, 1, 0,  /* int */
        4, 1, 1,  /* long */
        4, 1, 1,  /* long long */
        4, 1, 1,  /* float */
        8, 1, 1,  /* double */
        8, 1, 1,  /* long double */
        2, 1, 0,  /* T * */
        0, 1, 0,  /* struct */
        0,        /* little_endian */
        0,        /* mulops_calls */
        1,        /* wants_callb */
        1,        /* wants_argb */
        1,        /* left_to_right */
        0,        /* wants_dag */
        0,        /* unsigned_char */
/*wjr	8,	   byte width */
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
        0, 0, 0, 0, 0, 0, 0,
        {1, rmap,
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
static char rcsid[] = "$Id: m1.md,v 1.15 2006/10/11 01:05:49 buzbee Exp buzbee $";



