typedef unsigned long uint16_t;
typedef unsigned int uint8_t;
/*uint8_t sub(int p1, int p2, int p3, int p4, int p5){
	if(p3+p4)
		p1=0;
	return p1+p2;
}

*/
void main(){
	char * where; uint8_t n=1;
	//unsigned char j=1,k=1,l;
	//l=1;
	while(n>0){
		n--;}
}
//livemule.c:10: compiler error in _label--Bad terminal 1333
/* fatal - issue fatal error message and exit */
/*
int fatal(const char *name, const char *fmt, int n) {
	print("\n");
	errcnt = -1;
	error("compiler error in %s--", name);
	fprint(stderr, fmt, n);
	exit(EXIT_FAILURE);
	return 0;
}
*/
/* emitlabel - emit label function */
/*static void emitlabel(Term terms, Nonterm start, int ntnumber) {
	int i;
	Term p;

	print("static void %Plabel(NODEPTR_TYPE a) {\n%1int c;\n"
"%1struct %Pstate *p;\n\n"
"%1if (!a)\n%2fatal(\"%Plabel\", \"Null tree\\n\", 0);\n");
	print("%1STATE_LABEL(a) = p = allocate(sizeof *p, FUNC);\n"
"%1p->rule._stmt = 0;\n");
	for (i = 1; i <= ntnumber; i++)
		print("%1p->cost[%d] =\n", i);
	print("%20x7fff;\n%1switch (OP_LABEL(a)) {\n");
	for (p = terms; p; p = p->link)
		emitcase(p, ntnumber);
	print("%1default:\n"
"%2fatal(\"%Plabel\", \"Bad terminal(lburg) %%d\\n\", OP_LABEL(a));\n%1}\n}\n\n");
}
*/
/*
export main
segment text
function main type=void function(void) sclass=auto scope=GLOBAL flags=0 ref=0.000000 ncalls=0
blockbeg off=0
local n type=unsigned int sclass=register scope=LOCAL flags=0 offset=0 ref=11.000000
local where type=pointer to char sclass=auto scope=LOCAL flags=0 offset=2 ref=0.000000
blockbeg off=4
blockend off=4
blockend off=4
;void main(){
livemule.c:10.11:
;	char * where; uint8_t n=1;
livemule.c:11.25:
 2. ADDRLP2 n
3. CNSTU2 1
1' ASGNU2 #2 #3 2 1
 2:
;	while(n>0){}
livemule.c:14.11:
livemule.c:14.12:
 3:
livemule.c:14.7:
 3. ADDRLP2 n
2. INDIRU2 #3
4. CNSTU2 0
1' NEU2 #2 #4 2
;}
livemule.c:15.0:
 1:
maxoff=4

progend
*/
