#include <nstdint.h>
#include <olduino.h>
#include <nstdlib.h>
#include <hspi2.h>
#include "tinyFAT.h"
#include "mmc.h"
#define nofloats
uint16_t tF_readLn(char *st, int bufSize)
{
	uint32_t sec;
		sec=((uint32_t)BS.reservedSectors+
		((uint32_t)BS.fatCopies*(uint32_t)BS.sectorsPerFAT)+
		(((uint32_t)BS.rootDirectoryEntries*32)/512)+
		(((uint32_t)currFile.currentCluster-2)*(uint32_t)BS.sectorsPerCluster)+BS.hiddenSectors)+(((uint32_t)currFile.currentPos/512) % (uint32_t)BS.sectorsPerCluster);
	return ERROR_WRONG_FILEMODE;
}
void main(){}
//#include <olduino.c>
//#include <nstdlib.c>
//#include "mmc.c"
//#include "hspi2.c"
/*
sec=BS.reservedSectors
    +BS.fatCopies*BS.sectorsPerFAT
    +BS.rootDirectoryEntries/16
    +(currFile.currentCluster-2)*BS.sectorsPerCluster
    +BS.hiddenSectors
    +(currFile.currentPos/512) % BS.sectorsPerCluster;


		sec=(BS.reservedSectors+
		(BS.fatCopies*BS.sectorsPerFAT)+
		((BS.rootDirectoryEntries*32)/512)+
		((currFile.currentCluster-2)*BS.sectorsPerCluster)+BS.hiddenSectors)+((currFile.currentPos/512) % BS.sectorsPerCluster);
 */