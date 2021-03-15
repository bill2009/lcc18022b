#define putc(c) putupcomx(c)
void putcomx(unsigned char c, unsigned int r13val){
	asm(" glo 12\n"
	    " lbr 0x2cd3\n");
}
void putupcomx(unsigned char c){
	if ('\n' == c){//newline
		putcomx('\n',0x3302);putcomx('\r',0x3302);
	} else if (c>='a' & c<='z'){//lower case
		putcomx(c-0x20,0x3302);
	} else
		putcomx(c,0x3302);

}
