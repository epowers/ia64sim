// Getline Sample Program

// data segment
.sdata
Introduction: stringz "\nEnter a word or phrase [`quit' to exit]\n"
prompt_str: stringz "\nEcho>"
prologue: stringz "\nYou said,  `"
epilogue: stringz "'\n"
coda: stringz  "Program terminated by user request"
data8 0 // Align buffer to enable 8 byte compare operations
string "***"
buffer: string "     " 
data8 0
data8 0
data8 0
data8 0
data8 0
data8 0
data8 0

// text segment
.text
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
//----------------------------------------------------------------
// IA-64 String Length Subroutine
//       Entry:   r32 -> string
//       Returns: r8 = string length
//       Local:   r33 = 8 bytes from string
//                r34 = index to null byte
//                r35 = adjusted start of string
//                r36 = alignment count (bytes)
//                r37 = shift count (bytes)
//                r38 = scratch register
//                r39 = scratch register
//                p6 = set if loop not done
//                p7 = set if loop done
//                p8 = set if alignment needed
//                p9 = set if alignment ok
//----------------------------------------------------------------
strlen:                // return length of string
      alloc r2=ar.pfs,1,7,0,0 //allocate register stack
      adds r35=8,r32       //remember adjusted start of string
      and r36=7,r32        //check alignment
      and r39=~7,r32      //allign address
      ld8 r33=[r39],8      //get the first load started
      mov pr=r0,0x40       //clear 1st time switch for load 
      cmp.eq p9,p8=r36,r0  //set p9 & clear p8 if already aligned
      mov r32=r39          //set up starting address for loop
 (p8) sub r37=8,r36        //length of partial string
 (p8) shl r38=r36,3        //shift value in bits
 (p8) shr r39=r33,r38      //right align string bytes
 (p8) czx1.r r8=r39       //check for zero byte
 (p8) cmp.lt p7,p6=r8,r37 //zero byte within alignment range?
 (p7) br.ret.sptk.few b0  //yes - return length in r8
str_nxt:               // top of loop
 (p6) ld8 r33=[r32],8      // load 8 bytes of string and inc pointer
  czx1.r r34=r33       // find zero byte (if any)
  cmp.eq p6,p7=8,r34   //did we find it yet?
 (p6) br.cond.sptk.few str_nxt    //not yet
  add r33=r32,r34      // end of string
  sub r8=r33,r35       // length
br.ret.sptk.few b0   // return to caller
//----------------------------------------------------------------

// External Entry Point
main::                  // application program entry
alloc r2=ar.pfs,0,1,2,0  // program requires one local and two output regs
st8 [r12]=r2            // save pfs on memory stack
mov r32=b0             // save return address in local reg stack
// display initial message
addl r33=@gprel(Introduction),r1          // pointer to format string in out1
adds r34=0,r0        // null pointer to variable list in out2
br.call.sptk.few b0=vprintf             // call vprintf routine
// display prompt
prompt:
addl r33=@gprel(prompt_str),r1          // pointer to format string in out1
adds r34=0,r0        // null pointer to variable list in out2
br.call.sptk.few b0=vprintf             // call vprintf routine
// read input
addl r33=@gprel(buffer),r1          // pointer to input buffer in out1
adds r34=72,r0        // max input length in out2
br.call.sptk.few b0=getline             // call getline routine
// delete trailing nl char
addl r33=@gprel(buffer),r1       // point to start of buffer
br.call.sptk.few b0=strlen  // get string length
addl r33=@gprel(buffer-1),r1   //adjusted start of string
add r34=r33,r8              // end of string-1
st1 [r34]=r0            // delete nl char
// print prologue
addl r33=@gprel(prologue),r1          // pointer to format string in out1
adds r34=0,r0        // null pointer to variable list in out2
br.call.sptk.few b0=vprintf              // call vprintf routine
// echo text
addl r33=@gprel(buffer),r1          // pointer to format string in out1
adds r34=0,r0        // null pointer to variable list in out2
br.call.sptk.few b0=vprintf             // call vprintf routine
// print epilogue
addl r33=@gprel(epilogue),r1          // pointer to format string in out1
adds r34=0,r0        // null pointer to variable list in out2
br.call.sptk.few b0=vprintf             // call vprintf routine
// check for q[uit]
addl r33=@gprel(buffer),r1 ;;      //first input char
ld1 r2=[r33]   ;;         //load 1st char
cmp.eq p1,p2=113,r2 ;;    // check for 'q'
(p2) cmp.eq p1,p2=81,r2 ;; // check for 'Q'
(p2) br.cond.sptk.few prompt    //repeat prompt
// terminate program
addl r33=@gprel(coda),r1          // pointer to format string in out1
adds r34=0,r0        // null pointer to variable list in out2
br.call.sptk.few b0=vprintf             // call vprintf routine
add r33=0,r0          // set return code in out1
br.call.sptk.few b0=exit             // call application exit
.entry main
