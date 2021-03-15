#18-02-10 linear two pass scan - no recursion
#18-02-13 trying to include aligns with jumps.
import sys
sys.path.append('/lcc42/examples/branchanal/')
from opdefsNW import *
from opfuncsNW import *
asminfile = open(sys.argv[1],'r')
asmdata = asminfile.read().expandtabs()
asmlines = asmdata.split('\n')
print len(asmlines), 'lines read'
jumpcount=0;aligncount=0
repcount=0
progsize=0
i=0
tln=0;pln=0;mln=0 #temp label numbers for $$,+ -
labeldefs={}
labelrefs=[]
cfunc="" #current C function
def ireplace(old, new, text): #    Replace case insensitive
    index_l = text.lower().index(old.lower())
    return text[:index_l] + new + text[index_l + len(old):] 
def gotlabel(token):
    global tln,pln,mln
    #print "in gotlabel:",token
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
    #print "out gotlabel:",token,progsize

def fmtlabel(label):
    global tln,pln,mln
    if label.startswith("$$"):
        label+=str(tln)
    elif label.startswith("+"): #check for temporary label
        label+=str(pln)
    elif label.startswith("-"): #check for temporary label
        label+=str(mln)
    return label

def handlefunc(line): #line is a function definition  like ;$$function_start$$-_vis_includer=void function(void)
    global cfunc
    tokens=line.split('=') #separate name from prefix and signature
    #print "%d %X %s " % (i+1,progsize,line),tokens
    gotlabel(tokens[0])
    if tokens[0].startswith(";$$function_start$$-"):
        cfunc=tokens[0][21:]
        #print "A",tokens[0],tokens[0][21:]
    else:
        cfunc="n/a"
        #print "B",cfunc

for lnum,line in enumerate(asmlines):
    global cfunc
    aline=line.split(";")[0] #get rid of any comment portion
    tokens=aline.split();
    if tokens:
        #print "%d %X %s" % (i+1,progsize,aline)
        if (not aline.startswith(' ')) or tokens[0].endswith(':'): #if it has a label
            gotlabel(tokens[0])
            tokens=tokens[1:]           #get rid of it
        if tokens:
            if tokens[0].lower() in jumpdefs:
                jumpcount+=1
                labelrefs.append([i,progsize,tokens[0].lower(),fmtlabel(tokens[1].split(",")[0]),cfunc]) #added current C function
            elif tokens[0].lower()=="align":
                aligncount+=1
                labelrefs.append([i,progsize,tokens[0].lower(),tokens[1].split(",")[0],process(tokens,progsize),cfunc])
                #print "align ",i,progsize,tokens[0].lower(),tokens[1].split(",")[0],process(tokens,progsize)
            progsize+=process(tokens,progsize)
    else:
        if line.startswith(";$$function_"):
            handlefunc(line)
        if line.startswith(";$$function_start"):
            asmlines.insert(lnum+1,"   align 256 ;function align");

    i+=1;
print "pass 1 completed. ",len(labeldefs)," labels found. ",len(labelrefs)," jumps found."
#print labelrefs,labeldefs
adjb=0;adjl=0
repacount=0
i=0
for ref in labelrefs: # line index, location, branch op(or align), label referenced(or alignment)
    line=ref[0]; loc=ref[1]; brop=ref[2]; label=ref[3]; func=ref[4]
    adjamt=0
    if brop=="align":
        repacount+=1
        newsize=process([brop,label],loc)
        adjamt=newsize-ref[4]
        labelrefs[i][4]=newsize
        #print "*A*%4d %4x %s" %(line+1,loc,brop), label, ref[4],newsize, adjamt
    elif (loc+1)//256==labeldefs[label]//256:
        asmlines[line]=ireplace(brop,jumpreps[brop],asmlines[line])
        #print "%4d %4x (%s)%s %4x" %(line+1,loc,func,asmlines[line],labeldefs[label])
        repcount+=1
        adjamt=-1
    else:
        if brop.lower().startswith('l'):
            print "%4d %4x (%s)%s %4x" %(line+1,loc,func,asmlines[line],labeldefs[label])
    
    if not adjamt==0:
        for adjref in labelrefs: # line index, location, branch op, label referenced
            if adjref[0]>line: # for jumps further down the way
                if adjref[2]=="align":
                    pass #print "A%d %x "%(adjref[0],adjref[1]),adjref[2],adjref[3],adjref[4]
                adjref[1]+=adjamt #get adjusted
                adjb+=1
        for k, labloc in labeldefs.iteritems():
            if labloc>loc:
                labeldefs[k] +=adjamt
                adjl+=1
    i+=1
#print labeldefs
#print labelrefs
#print ">>>>>>>>>>>>>>>>>>>>"
#for k,labloc in sorted(labeldefs.iteritems(), key=lambda x: x[1]):
#    print "%04x %s" % (labloc,k)
#print "<<<<<<<<<<<<<<<<<<<<"
functions=dict()
for key, value in sorted(labeldefs.iteritems(), key=lambda x: x[1]): 
#    print("{} : {}".format(key, value))
    if key.startswith(";$$function_start$$-"):
        functions[key[20:]]=value
    elif key.startswith(";$$function_end$$-"):
        functions[key[18:]]=value-functions[key[18:]]
print functions
print repcount,"+",repacount," fixup cycles ",adjb," branch fixups ",adjl," label fixups"
asmoutfile=open(sys.argv[1].split('.')[0]+".basm",'w'); asmoutfile.truncate()
i=0
for line in asmlines:
    asmoutfile.write(line+'\n')
    i+=1
print i, 'lines written'
print "%d long jumps found, %d shortened" % (jumpcount,repcount)

