// Echo Sample Program

// data segment
.sdata
Introduction: stringz "\nEnter a word or phrase [`quit' to exit]\n"
prompt_str: stringz "\nEcho>"
prologue: stringz "\nYou said,  `"
epilogue: stringz "'\n"
coda: stringz  "Program terminated by user request"
quit_str: stringz "quit"
buffer: data8 0 // Align buffer to enable 8 byte compare operations
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
      mov pr=r0,0xc0       //clear 1st time switches
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
// IA-64 String Compare Subroutine
//       Entry:   r32 -> string1
//                r33 -> string2
//       Returns: r8 = <0 if string1 is less than string2
//                      0 if string1 is equal to string2
//                     >0 if string1 is greater than string2
//       Local:   r32 <-> r39 rotating registers for loop unrolling
//                r40 = working copy of string1 ptr
//                r41 = working copy of string2 ptr
//                p6 = string1 exhausted
//                p7 = string2 exhausted
//                p8 = strings not equal
//                p9 = strings same length
//----------------------------------------------------------------
strcmp:
     alloc r2=ar.pfs,2,8,0,8 //allocate register stack
     mov r40=r32             //string1 pointer
     mov r41=r33             //string2 pointer
     mov ar.ec=2             //set prolog count
     mov pr=r0,0x3c0         //clear exit conditions
     mov pr.rot=1<<16        //enable pipeline
     mov r8=r0               //assume strings are equal
strcmp1:
     ld1.s r32=[r40],1       //load char from string1
     ld1.s r36=[r41],1       //load char from string2
 (p18) chk.s r34,strcbad     //check load to r32
 (p18) chk.s r38,strcbad     //check load to r36
 (p18) cmp.ne p8,p17=r34,r38 //check strings equal
 (p18) cmp.eq.or.andcm p6,p17=r34,r0  //check end of string1
 (p18) cmp.eq.or.andcm p7,p17=r38,r0  //check end of string2
 (p17) br.wtop.sptk strcmp1  //loop if no exit condition set
 (p6)  cmp.eq p9,p8=r39,r0   //check both strings exhausted
 (p9)  br.ret.sptk.few b0    //return strings equal if so
 (p8)  cmp.gt p7,p6=r35,r39  //else set inequality predicates
 (p6)  mov r8=-1             //string1 is less
 (p7)  mov r8=1              //string1 is greater
    br.ret.sptk.few b0       //return to caller
strcbad:
  break 0x20000              //application interrupt
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
// check for quit
addl r33=@gprel(buffer),r1       //response buffer
addl r34=@gprel(quit_str),r1     //"quit"
br.call.sptk.few b0=strcmp       //compare response to "quit"
cmp.eq p1,p2=r8,r0 ;;    // check for match
(p2) br.cond.sptk.few prompt    //repeat prompt
// terminate program
addl r33=@gprel(coda),r1          // pointer to format string in out1
adds r34=0,r0        // null pointer to variable list in out2
br.call.sptk.few b0=vprintf             // call vprintf routine
add r33=0,r0          // set return code in out1
br.call.sptk.few b0=exit             // call application exit
.entry main
