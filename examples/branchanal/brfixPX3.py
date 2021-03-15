#21-2-25 experiment at macro level
#overall flow:
#   read in the .oasm file with macros intact: asmlines[]
#   pseudo-assemble each line: asmlines[i]
#       calculate the address: progsize using the fixed full size for jumps, likely size for variable elements(aligns,pagefits)
#       remember each label: labeldefs[label]=progsize
#       remember each jump/align/pagefit:   labelrefs.append([i,progsize,branch-op,label/boundary/amount]) later the item size gets added
#   now, for each of the labelrefs: 
#       if it's a jump, see if it can be shortened and flag it.
#   write out the file
import sys
from opdefsPX3 import *  #opsizes[op]=size, jumpdefs, jumpreps
from opfuncsPX import *
asminfile = open(sys.argv[1],'r')
asmdata = asminfile.read().expandtabs()
asmlines = asmdata.split('\n')
print (len(asmlines), 'lines read')
jumpcount=0;aligncount=0
skipcount=0; skipneeded=0
repcount=0
progsize=0
i=0
tln=0;pln=0;mln=0 #temp label numbers for $$,+ -
labeldefs={}
labelrefs=[]
def ireplace(old, new, text): #    Replace case insensitive
    index_l = text.lower().index(old.lower())
    return text[:index_l] + new + text[index_l + len(old):] 
def gotlabel(token):
    global tln,pln,mln
    label=token.split(':')[0]
    if label.startswith("$$"):
        label+=str(tln)
    elif label.startswith("+"): #check for temporary label
        label+=str(pln)
        pln+=1
    elif label.startswith("-"): #check for temporary label
        mln+=1
        label+=str(mln)
    else:
        tln+=1
    labeldefs[label]=progsize

def fmtlabel(label):
    global tln,pln,mln
    if label.startswith("$$"):
        label+=str(tln)
    elif label.startswith("+"): #check for temporary label
        label+=str(pln)
    elif label.startswith("-"): #check for temporary label
        label+=str(mln)
    return label

for i,line in enumerate(asmlines):
    #print line
    aline=line.split(";")[0] #get rid of any comment portion
    tokens=aline.split();
    if tokens:  #If there's anything left
        print "%d %X %s" % (i+1,progsize,aline)
        if (not aline.startswith(' ')) or tokens[0].endswith(':'): #if it has a label
            gotlabel(tokens[0])
            tokens=tokens[1:]           #get rid of it
        if tokens:
            thisop=tokens[0].lower()
            if thisop in jumpdefs:
                jumpcount+=1
                labelrefs.append([i,progsize,tokens[0].lower(),fmtlabel(tokens[1].split(",")[labpos[thisop]])])
            elif thisop=="align":
                aligncount+=1
                labelrefs.append([i,progsize,thisop,tokens[1].split(",")[0],process(tokens,progsize)])
                #print "align ",i,progsize,thisop,tokens[1].split(",")[0],process(tokens,progsize)
            elif tokens[0].lower()=='pagefit':
                skipcount+=1
                needed=process(tokens,progsize)
                labelrefs.append([i,progsize,thisop,tokens[1].split(",")[0],needed]) #21-2-17
                skipneeded+=needed
            progsize+=process(tokens,progsize)

    i+=1;
print "pass 1 completed. ",len(labeldefs)," labels found. ",len(labelrefs)," references found."
#print labelrefs,labeldefs
adjb=0;adjl=0
repacount=0
i=0
for ref in labelrefs: # line index, location, branch op(or align), label referenced(or alignment), old size(?)
    line=ref[0]; loc=ref[1]; brop=ref[2]; label=ref[3]
    adjamt=0
    print "line,brop,label= ",line,brop,label #21-2-17
    if brop in jumpdefs:
        print line,asmlines[line],loc,label,labeldefs #21-2-17
        print line,asmlines[line],loc,labeldefs[label]
        if (loc+1)//256==labeldefs[label]//256:
            asmlines[line]=ireplace(brop,jumpreps[brop],asmlines[line])
            print "%4d %4x %s %4x" %(line+1,loc,asmlines[line],labeldefs[label])
            repcount+=1
            adjamt=0   #not adjusting size
    i+=1
#print labeldefs
#for k,labloc in sorted(labeldefs.iteritems(), key=lambda x: x[1]):
#    print "%04x %s" % (labloc,k)
#for key, value in sorted(labeldefs.iteritems(), key=lambda x: x[1]): 
#    print("{} : {}".format(key, value))
print repcount,"+",repacount," fixup cycles ",adjb," branch fixups ",adjl," label fixups"
asmoutfile=open(sys.argv[1].split('.')[0]+".basm",'w'); asmoutfile.truncate()
i=1
for line in asmlines:
    asmoutfile.write(line+'\n')
    i+=1
print i, 'lines written'
print "%d long jumps found, %d shortened" % (jumpcount,repcount)
print "%d long skips found, %d bytes added" % (skipcount,skipneeded)
