/*--------------------------------------------------------------------
This source distribution is placed in the public domain by its author,
Jason Papadopoulos. You may use it for any purpose, free of charge,
without having to notify anyone. I disclaim any responsibility for any
errors, inconsistencies, and inaccuracies with real-world hardware
implementations of the IA64 instruction set. If you build a mission-
critical system based only on this simulator and something goes wrong,
it's your own damned fault.

Optionally, please be nice and tell me if you find this source to be
useful. Again optionally, if you add to the functionality present here
please consider making those additions public too, so that others may 
benefit from your work.	
       				   --jasonp@glue.umd.edu  12/5/99
--------------------------------------------------------------------*/
/* Updated by Chuck Crayne <ccrayne@pacific.net> */

#include "ia64.h"
#include "execute.h"

/*------------------------------------------------------------------------*/
uint32 f_misc(qword *instr) {
   qword imm;

   if (BITS(instr[0], 27,33)==0) {   
         imm.q = BITS(instr[0],36,36) << 20 |
                 BITS(instr[0],6,25);
         iim = BITS(instr[0],6,25);
         faults |= BREAK_FAULT;
         trcins("break.f %d\n", LO(imm));
         return IA_OK;
   }
   else if (BITS(instr[0], 27,33)==1) {   
         imm.q = BITS(instr[0],36,36) << 20 |
                 BITS(instr[0],6,25);
         trcins("nop.f %d\n", LO(imm));
         return IA_OK;
   }
   else
      return UNIMPLEMENTED;
}

/*------------------------------------------------------------------------*/
uint32 f_compare(qword *instr) { return UNIMPLEMENTED; }
uint32 f_class(qword *instr) { return UNIMPLEMENTED; }
uint32 f_fma(qword *instr) { return UNIMPLEMENTED; }
uint32 f_fms(qword *instr) { return UNIMPLEMENTED; }
uint32 f_fnma(qword *instr) { return UNIMPLEMENTED; }
uint32 f_xma(qword *instr) { return UNIMPLEMENTED; }
