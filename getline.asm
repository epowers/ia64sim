// Getline Sample Program
alloc r2=ar.pfs,0,1,2,0  // program requires one local and two output regs
st8 [r12]=r2            // save pfs on memory stack
mov r32=b0             // save return address in local reg stack
// display initial message
adds r33=0,r1          // pointer to format string in out1
adds r34=0,r0        // null pointer to variable list in out2
br.call.sptk.few b0=192 ;;             // call vprintf routine
// display prompt
adds r33=42,r1          // pointer to format string in out1
adds r34=0,r0        // null pointer to variable list in out2
br.call.sptk.few b0=176 ;;             // call vprintf routine
// read input
adds r33=128,r1          // pointer to input buffer in out1
adds r34=72,r0        // max input length in out2
br.call.sptk.few b0=128 ;;             // call getline routine
// print prologue
adds r33=49,r1          // pointer to format string in out1
adds r34=0,r0        // null pointer to variable list in out2
br.call.sptk.few b0=144 ;;             // call vprintf routine
// echo text
adds r33=128,r1          // pointer to format string in out1
adds r34=0,r0        // null pointer to variable list in out2
br.call.sptk.few b0=128 ;;             // call vprintf routine
// print eqilogue
adds r33=62,r1          // pointer to format string in out1
adds r34=0,r0        // null pointer to variable list in out2
br.call.sptk.few b0=112 ;;             // call vprintf routine
// check for q[uit]
adds r33=128,r1 ;;          //first input char
ld1 r2=[r33]   ;;         //load 1st char
cmp.eq p1,p2=113,r2 ;;     // check for 'q'
(p2) cmp.eq p1,p2=81,r2  // check for 'Q'
(p2) br.cond.sptk.few -112 ;;    //repeat prompt
// terminate program
add r33=0,r0          // set return code in out1
br.call.sptk.few b0=80 ;;             // call application exit
// getline routine
break.i 0x80002 ;;    // request getline service
br.ret.sptk.few b0 ;;
// vprintf routine
break.i 0x80001 ;;   // request vprintf service
br.ret.sptk.few b0 ;;
// application exit routine
break.m 0x80000 ;;
## End of Instructions
data1 10
data1 "Enter a word or phrase ['quit' to exit]"
data1 10
data1 0
data1 10
data1 "IA64>"
data1 0
data1 10
data1 "You said:  "
data1 0
data1 " "
data1 10
data1 0
   
