"""		sec=(BS.reservedSectors+
		(BS.fatCopies*BS.sectorsPerFAT)+
		((BS.rootDirectoryEntries*32)/512)+
		((currFile.currentCluster-2)*BS.sectorsPerCluster)+BS.hiddenSectors)
		
		+((currFile.currentPos/512) % BS.sectorsPerCluster);

		sec=(10+(fC*sPF)+((rDE*32)/512)+
		((cC-2)*sPC)+hS)
		
		+((cP/512) % sPC)
		
say reservedSectors=1, fatCopies=2,SectorsPerFat=32,rootDirectoryEntries
"""

def le(what): #convert little endian data to python int
	print "len(what)=",len(what)
	rc=0
	for b in reversed(range(len(what))):
		print b,what[b]
		rc=rc*256+ord(what[b])
	return rc
parameters={"bytesPerSector":(11,2), "sectorsPerCluster":(13,1)}
drive=r"\\.\D:" # Windows drive=r"\\.\PhysicalDrive1" # Windows
sector_size = 512
disk = file(drive,'rb')
#disk.seek(0)
x=disk.read(256)
for y in x:
	print format(ord(y),'02x'),
print
print le(x[11:13])
