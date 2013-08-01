// Hello.asm program for IA64 architecture

// Data Segment
// Data String
string:
data1 10             // new line char
data1 "Hello Rock %u"  //vprintf format string
data1 10             // new line char
data1 0              // end of string
// Data Variable
var: 
data4 3              // data for vprintf
## End of Data Segment
// Inline DLL Routines
vprintf:            // vprintf subroutine
break.i 0x80001   // request vprintf service
br.ret.sptk.few b0
// application exit routine
exit:                // program exit subroutine
break.m 0x80000   // request program exit service
//Main routine
main:                   // initial program entry point
alloc r2=ar.pfs,0,1,2,0 // program requires one local and two output regs
st8 [r12]=r2            // save pfs on memory stack
mov r32=b0             // save return address in local reg stack
adds r33=string,r1          // pointer to format string in out1
adds r34=var,r1        // pointer to variable list in out2
br.call.sptk.few b0=vprintf             // call vprintf routine
add r33=0,r0          // set return code in out1
br.call.sptk.few b0=exit             // call application exit
## End of Instructions



