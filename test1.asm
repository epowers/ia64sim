add                 r1=r2,r3
xor r4 = r10,r11
(p2) add r1=r2,r3 ;;
nop.b  12
nop.f 23
sub r1=-12,    r2	//this is a comment
sub r1=12,    r2	//this is a comment
cmp.eq p1,p0=r1, r2
(p1) br.cond.sptk.few -16 ;;
