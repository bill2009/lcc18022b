#copt peephole analyzer rewritten in python
#input is optimization prototypes as below
#   cpy1 %1,%2
#   str1 %1,%3
#;D*E*A*D  %1
#=
#    str1 %2,%3
#this version is meant for beta use - possible pypy compilation 
from __future__ import print_function
import sys,time
print ("Peephole analyzer in Python")
fname=sys.argv[1]
patterns=list()
spat=list()
def loadpgm():
    fhand=open(fname+".oasm")
    for line in fhand:
        pgm.append(line.rstrip())
def loadpats(): # load the patterns
    global patterns,spat
    patdata=open("patterns.opt").read()
    patterns=patdata.split('\n\n')
    for pat in patterns:
        spat.append(pat.split('=\n'))
loadpats()
#print(patterns)
for pat in spat:
    print("replace{\n"+pat[0]+"}\nwith{\n"+pat[1]+"\n}")
    print()
    