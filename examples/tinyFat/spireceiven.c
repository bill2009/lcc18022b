unsigned char spirec(){return 0;}
void spiReceiveNa(unsigned char * where, unsigned int n){
	while(n>0){
		*where++=spiRec();
		n--;
	}
}
void spiReceiveNb(unsigned char * where, unsigned int n){
	while(n>0){
		*where=spiRec();
		where++;
		n--;
	}
}

void main(){}
