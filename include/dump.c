//dump.c header for simple hex/ascii dump
int isalpha(unsigned char c)
{
        return((c >='a' && c <='z') || (c >='A' && c <='Z'));
}
void dump(unsigned char * p, unsigned int len)
{
    int i,j;

    for (i=0;i<len/16;i++)
    {
        printf("%x  ",i*16);
        for (j=0;j<16;j++)
            printf("%cx ",p[i*16+j]);
        printf("  ");
        for (j=0;j<16;j++)
            printf("%c", isalpha(p[i*16+j]) ? p[i*16+j] : '.');
        printf("\n");
    }
}
