import re
print "Python Liveness analysis V - regex for labels - no assumed liveness"
retops=["cretn"]
callops=["ccall"]
ignores=["equ","listing","include","globss","db","dw","reserve","release","seq",
         "pushf","pushm","pushl","pushr",
         "align","orgc","orgd","demote",
         "ldi","smi","bnz",
         "zext","zext4","st2i"]
horrors=["jumpv"]
loads=["ldad","ld2","lda2","ld1","ldn","plo","phi","ldi4"]
ldas=["lda"]
ldns=["ldn1","ldn2"]
stores=["str1","str2"]
IUs=["str1i"] #immed op,reg used
Us=["st2","st1","str"] #reg used
clobbers=["popr","ld2z","popf","popl"]
incdecs=["inc","Incr","incm","dec","decm","shl2i","shri2i","shru2i"]
cjumps1=["jneu2","jneu1","jeqi2","jnei2"] #two registers plus label
cjumps0=["jnzu2","jzu1","jzu2","jnzu1"] #one register and label
cjumps2=["jci2i","jci1i","jni2i","jnu2i","jcu2i"] #register,immediate,op, and label
cjumps3=["jcu1","jci2","jcu2"] #two registers,op, and label
cjumps4=["jneu1i","jequ2i","jneu2i","jequ1i"] #reg, immed val, label
jumps=["lbr"];cjumps18=["lbnf"]
copies=["cpy2","cpy1","shlr2r"]
alus=["alu2","alu1","alu4"] #3 registers
aluis=["alu2i"] #2 regs
alurrss=["alu2rrs"]
pgm=list();label=list();opcode=list();uses=list();sets=list();refs=list()
targets=dict() #branch targets
def loadpgm():
    fname="reduced_full_lcc1802_combo"#"test2"
    #fname=raw_input("program to analyze:")
    if fname=="":
        fname="reduced_full_lcc1802_combo"
    fname=fname+".asm"
    fhand=open(fname)
    for line in fhand:
        pgm.append(line.rstrip())
def parsepgm():
    functype="$$NOTSET"
    incode=False
    for n in range(len(pgm)):
        src=pgm[n]
        label.append("");opcode.append("");uses.append([]);sets.append([]);refs.append([])
        if src.strip()=="":
            continue
        elif src.strip()=="orgc":
            incode=True
        elif src.strip()=="orgd":
            incode=False
        if True:
            src=src.split(";")[0]
            if src.strip()=="":
                #print pgm[n]
                if pgm[n].startswith(";;function_start"):                
                    functype=pgm[n].split(' ')[2] # isolate the function type ;;function_start _destroyHorizontalMissile void
                    #print functype,"**",pgm[n]
                continue
            if src.find(":")>0:
                label[n]=src.split(":")[0]
                #print "label[n]:",label[n]
                targets[label[n]]=n
                src=src.split(":")[1].strip()
                if src=="":
                    continue
            #here we have a non-blank/comment unlableled line
            #print src
            toks=src.split()
            opcode[n]=toks[0].strip().lower()
            #print "**OPCODE=",opcode[n]
            if opcode[n] in ignores:
                continue
            elif opcode[n] in retops:
                if not(functype.startswith("void")):
                    #print "------------------ line ",n,pgm[n]," return from ",functype
                    uses[n]=["R15"] # need to address possibility of longs being returned
                continue

            ops=toks[1].split(',')
            if opcode[n] in IUs: #immed, used reg
                uses[n]=[ops[1]]
            elif opcode[n] in Us: #used reg
                uses[n]=[ops[0]]
            elif opcode[n] in copies or opcode[n] in alurrss or (opcode[n] in aluis):
                sets[n]=[ops[0]];uses[n]=[ops[1]]
            elif opcode[n] in ldas:
                uses[n]=[ops[0]]
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
            elif opcode[n] in loads:
                sets[n]=[ops[0]]
            elif opcode[n] in ldns:
                sets[n]=[ops[0]];uses[n]=[ops[1]]
            elif opcode[n] in stores:
                uses[n]=[ops[0],ops[1]]
            elif opcode[n] in horrors:
                print "horrible ", opcode[n]
            else:
                print "ignoring",opcode[n]," on line ",n,pgm[n]
                dummy=raw_input("press enter to continue")
        
loadpgm()
parsepgm()

livein=dict()
liveout=dict()
labels=dict()
successors=list()
print "targets=",targets

for n in range(len(pgm)-1):
    if opcode[n] in cjumps0+cjumps1+cjumps2+cjumps3+cjumps4+cjumps18:
        print "**************CJUMP*************",n,refs[n]
        successors.append([n+1,targets[refs[n][0]]])
    elif opcode[n] in jumps:
        successors.append([targets[refs[n][0]]])
    else:
        successors.append([n+1])
    #print "successor of ",n," is ",successors[n]
successors.append([])
for n in range(len(successors)):
    if successors[n]!=[n+1]:
        print "successor(s) to line ",n,":", successors[n]
for n in range(len(pgm)):
    livein[n]=[]
    #if opcode[n]=="cretn":
    #    liveout[n]=["rp1p2","r15"]
    #else:
    liveout[n]=[]


print "*********************"
for k in range(1,1000):
    changes=0
    for n in range(len(pgm)):
        for succ in successors[n]:
            for what in livein[succ]:
                if what not in liveout[n]:
                    liveout[n].append(what)
                    changes=changes+1
                    
    for n in range(len(pgm)):
        for what in uses[n]:
            if what not in livein[n]:
                livein[n].append(what)
                changes=changes+1

    for n in range(len(pgm)):
        for what in liveout[n]:
            if what not in sets[n] and what not in livein[n]:
                livein[n].append(what)
                changes=changes+1

    print k,"passes", changes,"changes"
    if changes==0:
        break
for n in range(len(pgm)):
    print n,pgm[n],liveout[n] #," sets ",sets[n]," uses ",uses[n]

