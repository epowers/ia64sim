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
/* Modified by Chuck Crayne <ccrayne@pacific.net> */

#include "ia64.h"
#include "execute.h"

extern char prefetch[2][6];
extern char bwh_imm[4][6];
extern char bwh_reg[8][6];
extern char dealloc[2][6];


/*--------------------------------------------------------------------*/
uint32 b_nop(qword *instr) {

   uint32 opcode;
   qword imm;

   opcode = BITS(instr[0],27,32);
   imm.q = BITS(instr[0],36,36) << 20 |
           BITS(instr[0],6,25);

   trcins("nop.b %d\n", LO(imm));

   /* According to the manual, if the opcode is 2
      execute a nop, else do nothing. */

   return IA_OK;
}


/*--------------------------------------------------------------------*/
uint32 b_break(qword *instr) {

   uint32 opcode;
   qword imm;

   opcode = BITS(instr[0],27,32);
   imm.q = BITS(instr[0],36,36) << 20 |
           BITS(instr[0],6,25);
           iim = 0;
           faults |= BREAK_FAULT;           

   trcins("break.b %d\n", LO(imm));

   return IA_OK;
}


/*--------------------------------------------------------------------*/
uint32 b_branchr(qword *instr) {


   uint32 opcode, b1, pred, status;
                
   opcode = BITS(instr[0],27,32) << 3 |
            BITS(instr[0],6,8);
   b1 = BITS(instr[0],13,15);
   pred = LO(instr[0]) & 63;


   /* branches are different from everybody else for dependency
      purposes; they ignore most RAW & WAR hazards from non-branches,
      and (by definition) a taken branch ends an application group.
      However, the five loop control branch types may read and write
      LC, EC, the RRBs, and PR[63], even if the branch is not taken,
      which creates a potential hazard if the loop setup instructions
      are in the same group with the the loop control branch, or if
      the loop control branch and the loop closing branch are in the
      same group.
            
       */

   check_reg(PC_REG);

   switch(opcode) {
      case 0x20*8+0:
         trcins("br.cond%s%s%s b%d\n", prefetch[BITS(instr[0],12,12)],
                                       bwh_imm[BITS(instr[0],33,34)],
                                       dealloc[BITS(instr[0],35,35)], b1 );
         if(p[pred]) {
            ip.q = b[b1].q & ~15;
            stop();
         }
         return IA_OK;
   
      case 0x20*8+1:
         trcins("br.ia%s%s%s b%d\n", prefetch[BITS(instr[0],12,12)],
                                     bwh_imm[BITS(instr[0],33,34)],
                                     dealloc[BITS(instr[0],35,35)], b1 );
         return UNIMPLEMENTED;

      case 0x21*8+4:
         trcins("br.ret%s%s%s b%d\n", prefetch[BITS(instr[0],12,12)],
                                      bwh_imm[BITS(instr[0],33,34)],
                                      dealloc[BITS(instr[0],35,35)], b1);
         if(p[pred]) {
            ip.q = b[b1].q;
            add_hazard(PC_REG);
            cfm.q = ar[AR_PFS].q & (((uint64)(1) << 37) - 1);
            ar[AR_EC].q = (uint64)BITS(ar[AR_PFS],52,57); 
            status = pop_stack_frame();
            stop();
            return status;
         }
         return IA_OK;
   }

   return ILLEGAL_INSTR;
}

/*--------------------------------------------------------------------*/
uint32 b_branchi(qword *instr) {


   uint32 opcode, pred;
   qword imm;
                
   opcode = BITS(instr[0],6,8);
   pred = LO(instr[0]) & 63;
   imm.q = -((int64)BITS(instr[0],36,36)) << 24 |
              BITS(instr[0],13,32) << 4;

   check_reg(PC_REG);

   switch(opcode) {

      case 0:
         trcins("br.cond%s%s%s %d\n", prefetch[BITS(instr[0],12,12)],
                                      bwh_imm[BITS(instr[0],33,34)],
                                      dealloc[BITS(instr[0],35,35)],
                                      LO(imm));
         if(p[pred]) {
            ip.q += imm.q;
            stop();
         }
         return IA_OK;

      case 2:
         trcins("br.wexit%s%s%s %d\n", prefetch[BITS(instr[0],12,12)],
                                       bwh_imm[BITS(instr[0],33,34)],
                                       dealloc[BITS(instr[0],35,35)],
                                       LO(imm));
         if(p[pred]) {
            p[63] = 1;
            rotate_registers();
            return IA_OK;
         }
         if(ar[AR_EC].q>1) {
            ar[AR_EC].q -= 1;
            add_hazard(AR_EC | APP_REG);
            p[63] = 0;
            rotate_registers();
            return IA_OK;
         }
         if(ar[AR_EC].q==1) {
            ar[AR_EC].q -= 1;
            p[63] = 0;
            ip.q += imm.q;
            stop();
            rotate_registers();
            return IA_OK;
         }
         else {
            p[63] = 0;
            ip.q += imm.q;
            stop();
            return IA_OK;
         }

      case 3:
         trcins("br.wtop%s%s%s %d\n", prefetch[BITS(instr[0],12,12)],
                                      bwh_imm[BITS(instr[0],33,34)],
                                      dealloc[BITS(instr[0],35,35)],
                                      LO(imm));
         if(p[pred]) {
            ip.q += imm.q;
            stop();
            p[63] = 0;
            rotate_registers();
            return IA_OK;
         }
         if(ar[AR_EC].q>1) {
            ar[AR_EC].q -= 1;
            ip.q += imm.q;
            stop();
            p[63] = 0;
            rotate_registers();
            return IA_OK;
         }
         if(ar[AR_EC].q==1) {
            ar[AR_EC].q -= 1;
            add_hazard(AR_EC | APP_REG);
            p[63] = 0;
            rotate_registers();
            return IA_OK;
         }
         else {
            p[63] = 0;
            add_hazard(63 | PRD_REG);
            return IA_OK;
         }

      case 5:
         trcins("br.cloop%s%s%s %d\n", prefetch[BITS(instr[0],12,12)],
                                       bwh_imm[BITS(instr[0],33,34)],
                                       dealloc[BITS(instr[0],35,35)],
                                       LO(imm));
         if(pred)
            return ILLEGAL_INSTR;
         if(ar[AR_LC].q) {
            ar[AR_LC].q -= 1;
            ip.q += imm.q;
            stop();
         }
         return IA_OK;

      case 6:
         trcins("br.cexit%s%s%s %d\n", prefetch[BITS(instr[0],12,12)],
                                       bwh_imm[BITS(instr[0],33,34)],
                                       dealloc[BITS(instr[0],35,35)],
                                       LO(imm));
         if(pred)
            return ILLEGAL_INSTR;
         if(ar[AR_LC].q) {
            ar[AR_LC].q -= 1;
            add_hazard(AR_LC | APP_REG);
            p[63] = 1;
            rotate_registers();
            return IA_OK;
         }
         if(ar[AR_EC].q>1) {
            ar[AR_EC].q -= 1;
            add_hazard(AR_EC | APP_REG);
            p[63] = 0;
            rotate_registers();
            return IA_OK;
         }
         if(ar[AR_EC].q==1) {
            ar[AR_EC].q -= 1;
            p[63] = 0;
            ip.q += imm.q;
            stop();
            rotate_registers();
            return IA_OK;
         }
         else {
            p[63] = 0;
            ip.q += imm.q;
            stop();
            return IA_OK;
         }

      case 7:
         trcins("br.ctop%s%s%s %d\n", prefetch[BITS(instr[0],12,12)],
                                      bwh_imm[BITS(instr[0],33,34)],
                                      dealloc[BITS(instr[0],35,35)],
                                      LO(imm));
         if(pred)
            return ILLEGAL_INSTR;
         if(ar[AR_LC].q) {
            ar[AR_LC].q -= 1;
            ip.q += imm.q;
            stop();
            p[63] = 1;
            rotate_registers();
            return IA_OK;
         }
         if(ar[AR_EC].q>1) {
            ar[AR_EC].q -= 1;
            ip.q += imm.q;
            stop();
            p[63] = 0;
            rotate_registers();
            return IA_OK;
         }
         if(ar[AR_EC].q==1) {
            ar[AR_EC].q -= 1;
            add_hazard(AR_EC | APP_REG);
            p[63] = 0;
            rotate_registers();
            return IA_OK;
         }
         else {
            p[63] = 0;
            add_hazard(63 | PRD_REG);
            return IA_OK;
         }

   }

   return ILLEGAL_INSTR;
}

/*--------------------------------------------------------------------*/
uint32 b_calli(qword *instr) { 

   uint32 b1, pred;
   qword imm;
                
   pred = LO(instr[0]) & 63;
   b1 = BITS(instr[0],6,8);
   imm.q = -((int64)BITS(instr[0],36,36)) << 24 |
              BITS(instr[0],13,32) << 4;
   
   check_reg(PC_REG);

   trcins("br.call%s%s%s b%d = %d\n", prefetch[BITS(instr[0],12,12)],
                                      bwh_imm[BITS(instr[0],33,34)],
                                      dealloc[BITS(instr[0],35,35)],
                                      b1, LO(imm) );
   if(p[pred]) {
      b[b1].q = ip.q + 16;
      ip.q += imm.q;
      stop();
      ar[AR_PFS].q = cfm.q & (((uint64)(1) << 37) - 1);
      ar[AR_PFS].q |= (uint64)(ar[AR_EC].q & 63) << 52;
      push_stack_frame();
   }

   return IA_OK;
}

/*--------------------------------------------------------------------*/
uint32 b_callr(qword *instr) { 

   uint32 b1, b2, pred;
   qword imm;
                
   pred = LO(instr[0]) & 63;
   b1 = BITS(instr[0],6,8);
   b2 = BITS(instr[0],13,15);

   check_reg(PC_REG);

   trcins("br.call%s%s%s b%d = b%d\n", prefetch[BITS(instr[0],12,12)],
                                      bwh_reg[BITS(instr[0],32,34)],
                                      dealloc[BITS(instr[0],35,35)],
                                      b1, b2 );
   if(p[pred]) {
      b[b1].q = ip.q + 16;
      ip.q = b[b2].q;
      stop();
      ar[AR_PFS].q = cfm.q & (((uint64)(1) << 37) - 1);
      ar[AR_PFS].q |= (uint64)(ar[AR_EC].q & 63) << 52;
      push_stack_frame();
   }

   return IA_OK;
}
