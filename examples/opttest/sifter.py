from __future__ import print_function
import sys,time
fname=sys.argv[1]
ops=set()
fhand=open(fname)
for line in fhand:
    for op in line.split(' '):
        if op!="" and (op[0]>='A') and (op[0]<='Z'):
            ops.add(op)
for op in ops:
    if op[-1]=='1':
        print(op)    