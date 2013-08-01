alloc r3=ar.pfs,0,5,2,0
br.call.sptk.few b1=64
nop.f 4;;
ld8 r7=[r1],r5
nop.f 1;;
st8 [r1]=r7
nop.f 2;;
nop.f 3
alloc r5=ar.pfs,2,5,2,0
br.ret.sptk.few b1

