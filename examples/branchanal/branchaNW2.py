#updated Feb 2018 for NW compiler, clean stack
import sys
from opdefsNW import *
from opfuncsNW import *
#print opsizes, jumpdefs
#print 'Number of arguments:', len(sys.argv), 'arguments.'
#print 'Argument List:', str(sys.argv)
asminfile = open(sys.argv[1],'r')
asmdata = asminfile.read().expandtabs()
asmlines = asmdata.split('\n')
print len(asmlines), 'lines read'

def jumper(i,op,tokens,pc):
	jumpcounts[op]+=1
	return jumpdefs[op]

labeldefs={}
def stolabel(label,address):
    labeldefs[label]=address;
    #print label,address

def scanner(beginline,beginpc,target):
    print "scanner begins at line %d with PC %d and target %s\n" %(beginline,beginpc,target)
    progsize=beginpc;
    i=beginline
    for line in asmlines[beginline:]:
    	if line:
		print '%3d %4X %s' % (i,progsize,asmlines[i])
	aline=line.split(";")[0] #get rid of any comment portion
	#print "!aline=",aline
        tokens=aline.split();
        if tokens:
		if not aline.startswith(' '): #if it has to be a label
			if not tokens[0].endswith(':'): #make sure it ends with a colon
				tokens[0]=tokens[0]+':'
		#print "!tokens=",tokens
		if tokens[0].endswith(':'):
			stolabel(tokens[0][:-1],progsize);
			print '%4X %s' %(progsize,tokens[0])
			if tokens[0][:-1]==target:
				return progsize
			else:
				tokens=tokens[1:]
		if tokens:
			if tokens[0].lower() in jumpdefs:
				print "Oh Boy"
				progsize+=jumper(i,tokens[0].lower(),tokens,progsize)
			else:
				progsize+=process(tokens,progsize)
	i+=1;
                
    return -1

print 'scanner sez ',scanner(0,0,'**end**')

asmoutfile=open(sys.argv[1]+"O.asm",'w'); asmoutfile.truncate()
i=0
for line in asmlines:
	asmoutfile.write(line+'\n')
	i+=1
print i, 'lines written'
print jumpcounts
print len(labeldefs),labeldefs
