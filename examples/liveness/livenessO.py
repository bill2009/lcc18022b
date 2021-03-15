#Liveness Analysis for lcc1802 code optimization
#input is assembly code from lcc1802
#output is assembly code with comments stripped and a new line added whenever a variable dies allowing optimizations like:
#   cpy1 %1,%2
#   str1 %1,%3
#;D*E*A*D  %1
#=
#    str1 %2,%3
#this version is meant for beta use - possible pypy compilation 
from __future__ import print_function
import sys,time
#print ("Python Liveness analysis 9 - command line i/o")
retops=["cretn"]
callops=["ccall"]
ignores=["equ","listing","include","globss","db","dw","dd","reserve","release","seq",
         "pushf","pushm","pushl","pushr",
         "align","orgc","orgd","demote",
         "ldi","smi","bnz",
         "st2i","st1i","nop1806"]
horrorsU=["jumpv"]
loads=["ldad","ldi4","plo","phi","lda"] 
Ss=["zext","zext4","sext4"] #Sets
loads2=["ld1","ld2","lda2","ld4"] #SXs= Sets, possible index
ldns=["ldn1","ldn2","ldn2s"] #reg,reg
stores=["str1","str2"]
IUs=["str1i","str2i"] #immed op,reg used
Us=["str","ghi","ldn"] #reg used
UUs=["mvc2","mvcn2","blkcpy"] #reg used, reg used
UXs=["st2","st1","st4"] # reg and possible index
SUs=["cpy2","cpy1","shlr2r","alu2i","alu2rrs","negi2","negi4","cpy4"] #reg set, reg used
clobbers=["popr","ld2z","popf","popl"]
incdecs=["inc","Incr","incm","dec","decm","shl2i","shri2i","shru2i","shri4i","shl4i"]
cjumps1=["jneu2","jneu1","jeqi2","jnei2","jeqi4"] #two registers plus label
cjumps0=["jnzu2","jzu1","jzu2","jnzu1"] #one register and label
cjumps2=["jci2i","jci1i","jni2i","jnu2i","jcu2i"] #register,immediate,op, and label
cjumps3=["jcu1","jci2","jcu2","jci4","jcf4"] #two registers,op, and label
cjumps4=["jneu1i","jequ2i","jneu2i","jequ1i"] #reg, immed val, label
jumps=["lbr"];cjumps18=["lbnf"]
alus=["alu2","alu1","alu4"] #3 registers
pgm=list();label=list();opcode=list();uses=list();sets=list();refs=list();pops=list()
targets=dict() #branch targets
fname=sys.argv[1]
def loadpgm():
    fhand=open(fname+".oasm")
    for line in fhand:
        pgm.append(line.rstrip())
def parsepgm():
    functype="$$NOTSET"
    incode=False
    for n in range(len(pgm)):
        if pgm[n] !="" and pgm[n][0]!=';': #unless it's a full line comment or empty
            pgm[n]=pgm[n].split(';')[0] #strip off the trailing comment
        src=pgm[n]
        label.append("");opcode.append("");uses.append([]);sets.append([]);refs.append([]);pops.append([])
        if src.strip()=="":
            continue
        elif src.strip()=="orgc":
            incode=True
        elif src.strip()=="orgd":
            incode=False
        if True:
            src=src.split(";")[0]
            if src.strip()=="":
                if pgm[n].startswith(";;function_start"):                
                    functype=pgm[n].split(' ')[2] # isolate the function type ;;function_start _destroyHorizontalMissile void
                continue
            if src.find(":")>0:
                label[n]=src.split(":")[0]
                targets[label[n]]=n
                src=src.split(":")[1].strip()
                if src=="":
                    continue
            #here we have a non-blank/comment unlableled line
            toks=src.split()
            opcode[n]=toks[0].strip().lower()
            if opcode[n] in ignores:
                continue
            elif opcode[n] in retops:
                if not(functype.startswith("void")):
                    uses[n]=["R15"] # need to address possibility of longs being returned
                continue
            #print(n,pgm[n],src,toks)
            ops=toks[1].split(',')
            pops[n]=ops
            if opcode[n] in IUs: #immed, used reg
                uses[n]=[ops[1]]
            elif opcode[n] in Us: #used reg
                uses[n]=[ops[0]]
            elif opcode[n] in UUs: #used reg
                uses[n]=[ops[0],ops[1]]
            elif opcode[n] in UXs: #used reg,possible index
                uses[n]=[ops[0]]
                if ops[1]=="'o'": 
                    uses[n]=[ops[2]]
            elif opcode[n] in SUs:
                sets[n]=[ops[0]];uses[n]=[ops[1]]
            elif opcode[n] in alus:
                sets[n]=[ops[0]];uses[n]=[ops[1],ops[2]]
            elif opcode[n] in incdecs:
                sets[n]=[ops[0]];uses[n]=[ops[0]]
            elif opcode[n] in cjumps1:
                uses[n]=[ops[0],ops[1]]
                refs[n]=[ops[2]]
            elif opcode[n] in cjumps0:
                uses[n]=[ops[0]];refs[n]=[ops[1]]
            elif opcode[n] in cjumps4:
                uses[n]=[ops[0]];refs[n]=[ops[2]]
            elif opcode[n] in cjumps2:
                uses[n]=[ops[0]]; refs[n]=[ops[3]]
            elif opcode[n] in cjumps3:
                uses[n]=[ops[0],ops[1]]; refs[n]=[ops[3]]
            elif opcode[n] in jumps:
                refs[n]=[ops[0]]
            elif opcode[n] in clobbers:
                sets[n]=[ops[0]]
            elif opcode[n] in callops:
                sets[n]=["R15","R12","R13"]; uses[n]=["R12","R13"]
            elif opcode[n] in loads or opcode[n] in Ss:
                sets[n]=[ops[0]]
            elif opcode[n] in loads2:
                sets[n]=[ops[0]]
                if ops[1].lower()=="'o'": 
                    if ops[2]!='sp': #don't track sp liveness
                        uses[n]=[ops[2]]
            elif opcode[n] in ldns:
                sets[n]=[ops[0]];uses[n]=[ops[1]]
            elif opcode[n] in stores:
                uses[n]=[ops[0],ops[1]]
            elif opcode[n] in horrorsU:
                print ("horrible ", opcode[n])
                uses[n]=[ops[0]]                
            else:
                print ("ignoring",opcode[n]," on line ",n,pgm[n])
                #dummy=input("press enter to continue")
tbegin=time.time()        
loadpgm()
parsepgm()
tprocess=time.time() #begin heavy duty process
livein=dict()
liveout=dict()
labels=dict()
successors=list()
#print ("targets=",targets)

for n in range(len(pgm)-1):
    if opcode[n] in cjumps0+cjumps1+cjumps2+cjumps3+cjumps4+cjumps18:
        #print "**************CJUMP*************",n,refs[n]
        successors.append([n+1,targets[refs[n][0]]])
    elif opcode[n] in jumps:
        successors.append([targets[refs[n][0]]])
    else:
        successors.append([n+1])
successors.append([])
for n in range(len(pgm)):
    livein[n]=[]
    liveout[n]=[]


#print ("*********************")
for n in range(len(pgm)):
    for what in uses[n]:
        if what not in livein[n]:
            livein[n].append(what)
#print("***********")
for k in range(1,1000):
    changes1=0; changes2=0
    for n in range(len(pgm)):
        for succ in successors[n]:
            for what in livein[succ]:
                if what not in liveout[n]:
                    liveout[n].append(what)
                    changes1=changes1+1
                    
    for n in range(len(pgm)):
        for what in liveout[n]:
            if what not in sets[n] and what not in livein[n]:
                livein[n].append(what)
                changes2=changes2+1

    #print (k,"passes", changes1,"changes1, ", changes2,"changes2")
    if (changes1+changes2)==0:
        print(k,"passes", time.time()-tprocess," seconds analysis")
        break
opts=[0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0]

def match2(n,opcoden,deadopnumn,opcoden1,deadopnumn1):
    if opcode[n]==opcoden and pops[n][deadopnumn] not in liveout[n]:
        if opcode[n-1]==opcoden1 and pops[n-1][deadopnumn1]==pops[n][deadopnumn]:
            return True
    return False
def match3(n,opcoden,deadopnumn,opcoden1,deadopnumn1,opcoden2,deadopnumn2):
    if opcode[n]==opcoden and pops[n][deadopnumn] not in liveout[n]:
        if opcode[n-1]==opcoden1 and pops[n-1][deadopnumn1]==pops[n][deadopnumn]:
            if opcode[n-2]==opcoden2 and pops[n-2][deadopnumn2]==pops[n][deadopnumn]:
                return True
    return False

for n in range(10,min(len(pgm),10000)): #skip the first part, limit to 1000 lines

#    cpy1 %1,%2
#    str1 %1,%3
#;D*E*A*D  %1
#=
#    str1 %2,%3
    if opcode[n]=="str1" and pops[n][0] not in liveout[n]:
        if opcode[n-1]=="cpy1" and pops[n-1][0]==pops[n][0]:
            pgm[n]=pgm[n].replace(pops[n][0],pops[n-1][1])
            pgm[n-1]=""
            print("1bazinga",n,pgm[n-1:n+1])
            opts[1]=opts[1]+1

#    zExt %1 
#    incm %1,%3
#    str1 %1,%2
#;D*E*A*D  %1
#=
#    incm %1,%3
#    str1 %1,%2
#;D*E*A*D  %1
    if opcode[n]=="str1":
        if pops[n][0] not in liveout[n]:
            if (opcode[n-1]=="incm" or opcode[n-1]=="decm") and pops[n-1][0]==pops[n][0]:
                if opcode[n-2]=="zext"  and pops[n-2][0]==pops[n][0]:
                    pgm[n-2]=""
                    print("2bazinga",n,pgm[n-2:n+1])
                    opts[2]=opts[2]+1
#    ld1 %1,%2,%3,%4
#    zExt %1 
#    cpy2 %5,%1 
#;D*E*A*D  %1
#=
#    ld1 %5,%2,%3,%4
#    zExt %5 
    if opcode[n]=="cpy2" and pops[n][1] not in liveout[n]:
        if opcode[n-1]=="zext" and pops[n-1][0]==pops[n][1]:
            if opcode[n-2]=="ld1"  and pops[n-2][0]==pops[n][1]:
                pgm[n]=""
                pgm[n-1]=pgm[n-1].replace(pops[n][1],pops[n][0])
                pgm[n-2]=pgm[n-2].replace(pops[n][1],pops[n][0])
                print("3bazinga",n,pgm[n-2:n+1])
                opts[3]=opts[3]+1
#    zExt %1 
#    cpy1 %2,%1
#;D*E*A*D  %1
#=
#    cpy1 %2,%1
#;D*E*A*D  %1
    if opcode[n]=="cpy1":
        if pops[n][1] not in liveout[n]:
            if opcode[n-1]=="zext":
                if pops[n-1][0]==pops[n][1]:
                    pgm[n-1]=""
                    print("4bazinga",n,pgm[n-1:n+1])
                    opts[4]=opts[4]+1
#    cpy2 %1,%2 
#    cpy2 %3,%1 
#;D*E*A*D  %1
#=
#    cpy2 %3,%2 
    if opcode[n]=="cpy2": 
        #print(n+1,opcode[n],pops[n], liveout[n])
        if pops[n][1] not in liveout[n]:
            #print("5bingo",pops[n][1])
            if opcode[n-1]=="cpy2":
                if pops[n-1][0]==pops[n][1]:
                    pgm[n-1]=""
                    pgm[n]=pgm[n].replace(pops[n][1],pops[n-1][1])
                    print("5bazinga",n,pgm[n-1:n+1])
                    opts[5]=opts[5]+1
#    cpy1 %1,%2
#    zExt %1 
#    cpy1 %3,%1
#;D*E*A*D  %1
#=
#    cpy1 %3,%2
    if opcode[n]=="cpy1" and pops[n][1] not in liveout[n]:
        if opcode[n-1]=="zext" and pops[n-1][0]==pops[n][1]:
            if opcode[n-2]=="cpy1" and pops[n-2][0]==pops[n][1]:
                pgm[n-2]=""
                pgm[n-1]=""
                pgm[n]=pgm[n].replace(pops[n][1],pops[n-2][1])
                #print("6bazinga",n,pgm[n-2:n+1])
                opts[6]=opts[6]+1
#    cpy2 %1,%2 
#    jzU1 %1,%3
#;D*E*A*D  R11
#=
#    jzU1 %2,%3
#
#    cpy2 %1,%2 
#    jnzU1 %1,%3
#;D*E*A*D  R11
#=
#    jnzU1 %2,%3
    if (opcode[n]=="jzu1" or opcode[n]=="jnzu1") and pops[n][0] not in liveout[n]: 
        #print(n,"7bingo",pops[n][0])
        if opcode[n-1]=="cpy2":
            if pops[n-1][0]==pops[n][0]:
                pgm[n-1]=""
                pgm[n]=pgm[n].replace(pops[n][0],pops[n-1][1])
                print("7bazinga",n,pgm[n-1:n+1])
                opts[7]=opts[7]+1
#    ldaD %1,%2
#    st2 %1,%3,%4,%5
#;D*E*A*D  %1
#=
#    st2i %2,%3,%4,%5
    if opcode[n]=="st2" and pops[n][0] not in liveout[n]:
            if opcode[n-1]=="ldad":
                if pops[n-1][0]==pops[n][0]:
                    pgm[n-1]=""
                    pgm[n]=pgm[n].replace(pops[n][0],pops[n-1][1]).replace(opcode[n],"st2i")
                    print("8bazinga",n,pgm[n-1:n+1])
                    opts[8]=opts[8]+1
#    cpy2 %1,%2 
#    ldA2 %3,'O',%1,%5
#;D*E*A*D  %1
#=
#    ldA2 %3,'O',%2,%5
    if match2(n,"lda2",2,"cpy2",0):
        pgm[n-1]=""
        pgm[n]=pgm[n].replace(pops[n][2],pops[n-1][1])
        print("9bazinga",n,pgm[n-1:n+1])
        opts[9]=opts[9]+1
#    ld2z %1
#    st2 %1,%2,%3,%4
#;D*E*A*D  %1
#=
#    st2i 0,%2,%3,%4
    if match2(n,"st2",0,"ld2z",0):
        pgm[n-1]=""
        pgm[n]=pgm[n].replace(pops[n][0],"0").replace(opcode[n],"st2i")
        print("10bazinga",n,pgm[n-1:n+1])
        opts[10]=opts[10]+1
#    cpy2 %1,%2 
#    incm %1,%4
#    cpy1 %3,%1
#;D*E*A*D  %1
#=
#    cpy1 %3,%2
#    incm %3,%4
    if match3(n,"cpy1",1,"incm",0,"cpy2",0):
        pgm[n]=""
        pgm[n-1]=pgm[n-1].replace(pops[n-1][0],pops[n][0])
        pgm[n-2]=pgm[n-2].replace(pops[n-2][0],pops[n][0])
        print("11bazinga",n,pgm[n-2:n+1])
        opts[11]=opts[11]+1
#    cpy2 %1,%2
#;D*E*A*D  %2
#    incm %1,%3
#    str1I 0,%1
#;D*E*A*D  %1
#=
#    incm %2,%3
#    str1I 0,%2
#;D*E*A*D  %2
    if opcode[n]=="str1i":
        if opcode[n-1]=="incm" and pops[n-1][0]==pops[n][1]:
            if opcode[n-2]=="cpy2" and pops[n-2][1] not in liveout[n-1]:
                pgm[n-2]=""
                pgm[n-1]=pgm[n-1].replace(pops[n-1][0],pops[n-2][1])
                pgm[n]=pgm[n].replace(pops[n][1],pops[n-2][1])
                print("12bazinga",n,pgm[n-2:n+1])
                opts[12]=opts[12]+1
#    ldn1 %1,%2
#    jzU1 %1,%3
#;D*E*A*D  %1
#=
#    ldn   %2
#    lbz   %3
    if match2(n,"jzu1",0,"ldn1",0):
        pgm[n-1]="\tldn "+pops[n-1][1]
        pgm[n]="\tlbz "+pops[n][1]
        print("13bazinga",n,pgm[n-2:n+1])
        opts[13]=opts[13]+1
    if match2(n,"jnzu1",0,"ldn1",0):
        pgm[n-1]="\tldn "+pops[n-1][1]
        pgm[n]="\tlbnz "+pops[n][1]
        print("14bazinga",n,pgm[n-2:n+1])
        opts[14]=opts[14]+1
#    cpy2 %1,%2 
#    st2 %1,'O',sp,%3
#;D*E*A*D  %1
#=
#    st2 %2,'O',sp,%3
    if match2(n,"st2",0,"cpy2",0) and pops[n][2]=="sp":
        pgm[n-1]=""
        pgm[n]=pgm[n].replace(pops[n][0],pops[n-1][1])
        print("15bazinga",n,pgm[n-1:n+1])
        opts[15]=opts[15]+1
#    ldA2 %1,'O',sp,%2
#    str1I %3,%1
#;D*E*A*D  %1
#=
#    st1i  %3,'O',sp,%2
    if match2(n,"str1i",1,"lda2",0) and pops[n-1][2]=="sp":
        pgm[n-1]=""
        pgm[n]="\tst1i "+pops[n][0]+",'O',sp,"+pops[n-1][3]
        print("16bazinga",n,pgm[n-1:n+1])
        opts[16]=opts[16]+1

#    cpy2 %1,R15 
#    alu2 %2,%3,%1,add,adc
#;D*E*A*D  %4
#;D*E*A*D  %1
#=
#    alu2 %2,%3,R15,add,adc
#;D*E*A*D  %4
    if match2(n,"alu2",2,"cpy2",0):
        pgm[n-1]=""
        pgm[n]=pgm[n].replace(pops[n][2],pops[n-1][1])
        print("****************************17bazinga",n,pgm[n-1:n+1])
        opts[17]=opts[17]+1

print(sum(opts),":",opts[1:])

with open(fname+".Olasm", 'w') as f:
    for n in range(len(pgm)):
        l=pgm[n].split(';')[0]
        if l!="":
            print (pgm[n].split(';')[0],file=f) #(n,pgm[n] #,liveout[n] ,file=f)#," sets ",sets[n]," uses ",uses[n]
            for x in uses[n]:
                if x not in liveout[n] and x not in["R15","R12","R13"]:
                    print (";D*E*A*D ",x,file=f)
        else:
            if pgm[n]!="":
                print(pgm[n],file=f)
#print (time.time()-tbegin," seconds overall")



