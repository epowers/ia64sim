// Hello.asm program for IA64 architecture
// Included "dll" Routines

// vprintf
vprintf:
break.i 0x80001 ;;
br.ret.sptk.few b0

// application exit routine
exit:                // program exit subroutine
break.m 0x80000 ;;   // request program exit service

// main routine
main:                   // initial program entry point
alloc r2=ar.pfs,0,1,2,0 // program requires one local and two output regs
st8 [r12]=r2            // save pfs on memory stack
mov r32=b0             // save return address in local reg stack
add r33=r1,r0          // pointer to format string in out1
adds r34=16,r1        // pointer to variable list in out2
br.call.sptk.few b0=32 ;;             // call vprintf routine
add r33=0,r0          // set return code in out1
br.call.sptk.few b0=48 ;;             // call application exit

## short data segment follows
data1 10             // new line char
data1 "Hello Rock %u"  //vprintf format string
data1 10             // new line char
data1 0              // end of string 
data4 3              // data for vprintf
