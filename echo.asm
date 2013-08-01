// Getline Sample Program

// data segment
Introduction:
data1 10
data1 "Enter a word or phrase [`quit' to exit]"
data1 10
data1 0
prompt_str:
data1 10
data1 "IA64>"
data1 0
prologue:
data1 10
data1 "You said,  `"
data1 0
epilogue:
data1 "'"
data1 10
data1 0
buffer:
data8 0
data8 0
data8 0
data8 0
data8 0
data8 0
data8 0
data8 0
## End of Data   

// System Subroutines
exit:                    //System Exit Routine
break.m 0x80000

vprintf:                //System Vprintf Routine
break.i 0x80001      // request vprintf service
br.ret.sptk.few b0

getline:               //System Getline Routine
break.i 0x80002    // request getline service
br.ret.sptk.few b0

//Local Subroutines
strlen:                // return length of string
alloc r2=ar.pfs,1,2,0,0 //allocate one input and two local regs
// ld8 r33=[r32],8         // load 1st 8 bytes of string and inc pointer
czx1.l r34=r33          // find zero byte (if any)
adds r8=8,r0           // fake result for testing
br.ret.sptk.few b0

// External Entry Point
main:                  // application program entry
alloc r2=ar.pfs,0,1,2,0  // program requires one local and two output regs
st8 [r12]=r2            // save pfs on memory stack
mov r32=b0             // save return address in local reg stack
// display initial message
adds r33=Introduction,r1          // pointer to format string in out1
adds r34=0,r0        // null pointer to variable list in out2
br.call.sptk.few b0=vprintf             // call vprintf routine
// display prompt
prompt:
adds r33=prompt_str,r1          // pointer to format string in out1
adds r34=0,r0        // null pointer to variable list in out2
br.call.sptk.few b0=vprintf             // call vprintf routine
// read input
adds r33=buffer,r1          // pointer to input buffer in out1
adds r34=72,r0        // max input length in out2
br.call.sptk.few b0=getline             // call getline routine
// delete trailing nl char
adds r33=buffer,r1       // point to start of buffer
br.call.sptk.few b0=strlen  // get string length
adds r33=buffer,r8  ;;          // point past nl char
st1 [r33]=r0,-1            // delete nl char
// print prologue
adds r33=prologue,r1          // pointer to format string in out1
adds r34=0,r0        // null pointer to variable list in out2
br.call.sptk.few b0=vprintf              // call vprintf routine
// echo text
adds r33=buffer,r1          // pointer to format string in out1
adds r34=0,r0        // null pointer to variable list in out2
br.call.sptk.few b0=vprintf             // call vprintf routine
// print epilogue
adds r33=epilogue,r1          // pointer to format string in out1
adds r34=0,r0        // null pointer to variable list in out2
br.call.sptk.few b0=vprintf             // call vprintf routine
// check for q[uit]
adds r33=buffer,r1 ;;      //first input char
ld1 r2=[r33]   ;;         //load 1st char
cmp.eq p1,p2=113,r2 ;;    // check for 'q'
(p2) cmp.eq p1,p2=81,r2 ;; // check for 'Q'
(p2) br.cond.sptk.few prompt    //repeat prompt
// terminate program
add r33=0,r0          // set return code in out1
br.call.sptk.few b0=exit             // call application exit
## End of Instructions
