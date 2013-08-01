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

        /* All the IA64 integer compare instructions. Un-
           fortunately, there are so many obscure combinations
           of qualifying predicates, register hazards, and 
	   parallel comparisons that the most straightforward 
	   way to code them up is to treat all 72 variations 
	   separately. */

/*----------------------------------------------------------------------*/
uint32 i_compare(qword *instr) {

   uint32 p1, p2, r1, r2, pred;
   uint32 opcode;
   qword imm;

   pred = LO(instr[0])&63;
   p1 = BITS(instr[0],6,11);
   p2 = BITS(instr[0],27,32);
   r1 = BITS(instr[0],13,19); /* imm7b */
   r2 = BITS(instr[0],20,26);

   check_reg(p1 | PRD_REG);
   check_reg(p2 | PRD_REG);
   if(r2) check_reg(r2 | ALU_REG);

   if( p[pred]==1 && p1==p2 )
      return ILLEGAL_INSTR;

   opcode = BITS(instr[0],33,40) << 1 |
            BITS(instr[0],12,12);
   imm.q = ((-(int64)(opcode>>3)) << 7 | r1) & 127;

   switch(opcode) {

      case 0xc*32+0*4+0*16+0*2+0:
         trcins("cmp.lt p%d,p%d = r%d,r%d\n",p1,p2,r1,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) check_reg(r1 | ALU_REG);
         if( (int64)(r[r1].dat.q) < (int64)(r[r2].dat.q) ) {
            p[p1] = 1; p[p2] = 0;
         }
         else {
            p[p1] = 0; p[p2] = 1;
         }
         if( r[r1].nat | r[r2].nat ) p[p1] = p[p2] = 0;
         if(p1) add_hazard(p1 | PRD_REG);
         if(p2) add_hazard(p2 | PRD_REG);
         p[0] = 1; return IA_OK;
            
      case 0xd*32+0*4+0*16+0*2+0:
         trcins("cmp.ltu p%d,p%d = r%d,r%d\n",p1,p2,r1,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) check_reg(r1 | ALU_REG);
         if( (uint64)(r[r1].dat.q) < (uint64)(r[r2].dat.q) ) {
            p[p1] = 1; p[p2] = 0;
         }
         else {
            p[p1] = 0; p[p2] = 1;
         }
         if( r[r1].nat | r[r2].nat ) p[p1] = p[p2] = 0;
         if(p1) add_hazard(p1 | PRD_REG);
         if(p2) add_hazard(p2 | PRD_REG);
         p[0] = 1; return IA_OK;
            
      case 0xe*32+0*4+0*16+0*2+0:
         trcins("cmp.eq p%d,p%d = r%d,r%d\n",p1,p2,r1,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) check_reg(r1 | ALU_REG);
         if( r[r1].dat.q == r[r2].dat.q ) {
            p[p1] = 1; p[p2] = 0;
         }
         else {
            p[p1] = 0; p[p2] = 1;
         }
         if( r[r1].nat | r[r2].nat ) p[p1] = p[p2] = 0;
         if(p1) add_hazard(p1 | PRD_REG);
         if(p2) add_hazard(p2 | PRD_REG);
         p[0] = 1; return IA_OK;
            
      case 0xc*32+0*4+0*16+0*2+1:
         trcins("cmp.lt.unc p%d,p%d = r%d,r%d\n",p1,p2,r1,r2);
         if(r1) check_reg(r1 | ALU_REG);
         p[p1] = p[p2] = 0;
         if(p1) add_hazard(p1 | PRD_REG);
         if(p2) add_hazard(p2 | PRD_REG);
         if( p[pred]==0 ) {
             p[0] = 1; return IA_OK;
          }
         if( (int64)(r[r1].dat.q) < (int64)(r[r2].dat.q) ) {
            p[p1] = 1; p[p2] = 0;
         }
         else {
            p[p1] = 0; p[p2] = 1;
         }
         if( r[r1].nat | r[r2].nat ) p[p1] = p[p2] = 0;
         p[0] = 1; return IA_OK;
            
      case 0xd*32+0*4+0*16+0*2+1:
         trcins("cmp.ltu.unc p%d,p%d = r%d,r%d\n",p1,p2,r1,r2);
         if(r1) check_reg(r1 | ALU_REG);
         p[p1] = p[p2] = 0;
         if(p1) add_hazard(p1 | PRD_REG);
         if(p2) add_hazard(p2 | PRD_REG);
         if( p[pred]==0 ) {
             p[0] = 1; return IA_OK;
          }
         if( (uint64)(r[r1].dat.q) < (uint64)(r[r2].dat.q) ) {
            p[p1] = 1; p[p2] = 0;
         }
         else {
            p[p1] = 0; p[p2] = 1;
         }
         if( r[r1].nat | r[r2].nat ) p[p1] = p[p2] = 0;
         p[0] = 1; return IA_OK;
            
      case 0xe*32+0*4+0*16+0*2+1:
         trcins("cmp.eq.unc p%d,p%d = r%d,r%d\n",p1,p2,r1,r2);
         if(r1) check_reg(r1 | ALU_REG);
         p[p1] = p[p2] = 0;
         if(p1) add_hazard(p1 | PRD_REG);
         if(p2) add_hazard(p2 | PRD_REG);
         if( p[pred]==0 ) {
             p[0] = 1; return IA_OK;
          }
         if( r[r1].dat.q == r[r2].dat.q ) {
            p[p1] = 1; p[p2] = 0;
         }
         else {
            p[p1] = 0; p[p2] = 1;
         }
         if( r[r1].nat | r[r2].nat ) p[p1] = p[p2] = 0;
         p[0] = 1; return IA_OK;
            
      case 0xc*32+0*4+0*16+1*2+0:
         trcins("cmp.eq.and p%d,p%d = r%d,r%d\n",p1,p2,r1,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) check_reg(r1 | ALU_REG);
         if( (r[r1].dat.q!=r[r2].dat.q) ||
             (r[r1].nat|r[r2].nat) ) {
            p[p1] = p[p2] = 0;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xd*32+0*4+0*16+1*2+0:
         trcins("cmp.eq.or p%d,p%d = r%d,r%d\n",p1,p2,r1,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) check_reg(r1 | ALU_REG);
         if( r[r1].dat.q == r[r2].dat.q ) {
            p[p1] = p[p2] = 1;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xe*32+0*4+0*16+1*2+0:
         trcins("cmp.eq.or.andcm p%d,p%d = r%d,r%d\n",p1,p2,r1,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) check_reg(r1 | ALU_REG);
         if( r[r1].dat.q == r[r2].dat.q ) {
            p[p1] = 1; p[p2] = 0;
            if(p1) add_hazard(p1 | PRD_REG);
//special case hack            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xc*32+0*4+0*16+1*2+1:
         trcins("cmp.ne.and p%d,p%d = r%d,r%d\n",p1,p2,r1,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) check_reg(r1 | ALU_REG);
         if( (r[r1].dat.q==r[r2].dat.q) ||
             (r[r1].nat|r[r2].nat) ) {
            p[p1] = p[p2] = 0;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xd*32+0*4+0*16+1*2+1:
         trcins("cmp.ne.or p%d,p%d = r%d,r%d\n",p1,p2,r1,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) check_reg(r1 | ALU_REG);
         if( r[r1].dat.q != r[r2].dat.q ) {
            p[p1] = p[p2] = 1;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xe*32+0*4+0*16+1*2+1:
         trcins("cmp.ne.or.andcm p%d,p%d = r%d,r%d\n",p1,p2,r1,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) check_reg(r1 | ALU_REG);
         if( r[r1].dat.q != r[r2].dat.q ) {
            p[p1] = 1; p[p2] = 0;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xc*32+1*4+0*16+0*2+0:
         trcins("cmp4.lt p%d,p%d = r%d,r%d\n",p1,p2,r1,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) check_reg(r1 | ALU_REG);
         if( (int32)LO(r[r1].dat) < (int32)LO(r[r2].dat) ) {
            p[p1] = 1; p[p2] = 0;
         }
         else {
            p[p1] = 0; p[p2] = 1;
         }
         if( r[r1].nat | r[r2].nat ) p[p1] = p[p2] = 0;
         if(p1) add_hazard(p1 | PRD_REG);
         if(p2) add_hazard(p2 | PRD_REG);
         p[0] = 1; return IA_OK;
            
      case 0xd*32+1*4+0*16+0*2+0:
         trcins("cmp4.ltu p%d,p%d = r%d,r%d\n",p1,p2,r1,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) check_reg(r1 | ALU_REG);
         if( (uint32)LO(r[r1].dat) < (uint32)LO(r[r2].dat) ) {
            p[p1] = 1; p[p2] = 0;
         }
         else {
            p[p1] = 0; p[p2] = 1;
         }
         if( r[r1].nat | r[r2].nat ) p[p1] = p[p2] = 0;
         if(p1) add_hazard(p1 | PRD_REG);
         if(p2) add_hazard(p2 | PRD_REG);
         p[0] = 1; return IA_OK;
            
      case 0xe*32+1*4+0*16+0*2+0:
         trcins("cmp4.eq p%d,p%d = r%d,r%d\n",p1,p2,r1,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) check_reg(r1 | ALU_REG);
         if( LO(r[r1].dat) == LO(r[r2].dat) ) {
            p[p1] = 1; p[p2] = 0;
         }
         else {
            p[p1] = 0; p[p2] = 1;
         }
         if( r[r1].nat | r[r2].nat ) p[p1] = p[p2] = 0;
         if(p1) add_hazard(p1 | PRD_REG);
         if(p2) add_hazard(p2 | PRD_REG);
         p[0] = 1; return IA_OK;
            
      case 0xc*32+1*4+0*16+0*2+1:
         trcins("cmp4.lt.unc p%d,p%d = r%d,r%d\n",p1,p2,r1,r2);
         if(r1) check_reg(r1 | ALU_REG);
         p[p1] = p[p2] = 0;
         if(p1) add_hazard(p1 | PRD_REG);
         if(p2) add_hazard(p2 | PRD_REG);
         if( p[pred]==0 ) {
             p[0] = 1; return IA_OK;
          }
         if( (int32)LO(r[r1].dat) < (int32)LO(r[r2].dat) ) {
            p[p1] = 1; p[p2] = 0;
         }
         else {
            p[p1] = 0; p[p2] = 1;
         }
         if( r[r1].nat | r[r2].nat ) p[p1] = p[p2] = 0;
         p[0] = 1; return IA_OK;
            
      case 0xd*32+1*4+0*16+0*2+1:
         trcins("cmp4.ltu.unc p%d,p%d = r%d,r%d\n",p1,p2,r1,r2);
         if(r1) check_reg(r1 | ALU_REG);
         p[p1] = p[p2] = 0;
         if(p1) add_hazard(p1 | PRD_REG);
         if(p2) add_hazard(p2 | PRD_REG);
         if( p[pred]==0 ) {
             p[0] = 1; return IA_OK;
          }
         if( (uint32)LO(r[r1].dat) < (uint32)LO(r[r2].dat) ) {
            p[p1] = 1; p[p2] = 0;
         }
         else {
            p[p1] = 0; p[p2] = 1;
         }
         if( r[r1].nat | r[r2].nat ) p[p1] = p[p2] = 0;
         p[0] = 1; return IA_OK;
            
      case 0xe*32+1*4+0*16+0*2+1:
         trcins("cmp4.eq.unc p%d,p%d = r%d,r%d\n",p1,p2,r1,r2);
         if(r1) check_reg(r1 | ALU_REG);
         p[p1] = p[p2] = 0;
         if(p1) add_hazard(p1 | PRD_REG);
         if(p2) add_hazard(p2 | PRD_REG);
         if( p[pred]==0 ) {
             p[0] = 1; return IA_OK;
          }
         if( LO(r[r1].dat) == LO(r[r2].dat) ) {
            p[p1] = 1; p[p2] = 0;
         }
         else {
            p[p1] = 0; p[p2] = 1;
         }
         if( r[r1].nat | r[r2].nat ) p[p1] = p[p2] = 0;
         p[0] = 1; return IA_OK;
            
      case 0xc*32+1*4+0*16+1*2+0:
         trcins("cmp4.eq.and p%d,p%d = r%d,r%d\n",p1,p2,r1,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) check_reg(r1 | ALU_REG);
         if( (LO(r[r1].dat)!=LO(r[r2].dat)) ||
             (r[r1].nat|r[r2].nat) ) {
            p[p1] = p[p2] = 0;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xd*32+1*4+0*16+1*2+0:
         trcins("cmp4.eq.or p%d,p%d = r%d,r%d\n",p1,p2,r1,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) check_reg(r1 | ALU_REG);
         if( LO(r[r1].dat) == LO(r[r2].dat) ) {
            p[p1] = p[p2] = 1;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xe*32+1*4+0*16+1*2+0:
         trcins("cmp4.eq.or.andcm p%d,p%d = r%d,r%d\n",p1,p2,r1,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) check_reg(r1 | ALU_REG);
         if( LO(r[r1].dat) == LO(r[r2].dat) ) {
            p[p1] = 1; p[p2] = 0;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xc*32+1*4+0*16+1*2+1:
         trcins("cmp4.ne.and p%d,p%d = r%d,r%d\n",p1,p2,r1,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) check_reg(r1 | ALU_REG);
         if( (LO(r[r1].dat)==LO(r[r2].dat)) ||
             (r[r1].nat|r[r2].nat) ) {
            p[p1] = p[p2] = 0;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xd*32+1*4+0*16+1*2+1:
         trcins("cmp4.ne.or p%d,p%d = r%d,r%d\n",p1,p2,r1,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) check_reg(r1 | ALU_REG);
         if( LO(r[r1].dat) != LO(r[r2].dat) ) {
            p[p1] = p[p2] = 1;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xe*32+1*4+0*16+1*2+1:
         trcins("cmp4.ne.or.andcm p%d,p%d = r%d,r%d\n",p1,p2,r1,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) check_reg(r1 | ALU_REG);
         if( LO(r[r1].dat) != LO(r[r2].dat) ) {
            p[p1] = 1; p[p2] = 0;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xc*32+0*4+1*16+0*2+0:
         trcins("cmp.gt.and p%d,p%d = r0,r%d\n",p1,p2,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) return ILLEGAL_INSTR;
         if( ((int64)(0)<=(int64)r[r2].dat.q) || r[r2].nat ) {
            p[p1] = p[p2] = 0;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xd*32+0*4+1*16+0*2+0:
         trcins("cmp.gt.or p%d,p%d = r0,r%d\n",p1,p2,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) return ILLEGAL_INSTR;
         if( (int64)(0)>(int64)r[r2].dat.q ) {
            p[p1] = p[p2] = 1;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xe*32+0*4+1*16+0*2+0:
         trcins("cmp.gt.or.andcm p%d,p%d = r0,r%d\n",p1,p2,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) return ILLEGAL_INSTR;
         if( (int64)(0)>(int64)r[r2].dat.q ) {
            p[p1] = 1; p[p2] = 0;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xc*32+0*4+1*16+0*2+1:
         trcins("cmp.le.and p%d,p%d = r0,r%d\n",p1,p2,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) return ILLEGAL_INSTR;
         if( ( (int64)(0)>(int64)r[r2].dat.q) || r[r2].nat ) {
            p[p1] = p[p2] = 0;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xd*32+0*4+1*16+0*2+1:
         trcins("cmp.le.or p%d,p%d = r0,r%d\n",p1,p2,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) return ILLEGAL_INSTR;
         if( (int64)(0)<=(int64)r[r2].dat.q ) {
            p[p1] = p[p2] = 1;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xe*32+0*4+1*16+0*2+1:
         trcins("cmp.le.or.andcm p%d,p%d = r0,r%d\n",p1,p2,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) return ILLEGAL_INSTR;
         if( (int64)(0)<=(int64)r[r2].dat.q ) {
            p[p1] = 1; p[p2] = 0;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xc*32+0*4+1*16+1*2+0:
         trcins("cmp.ge.and p%d,p%d = r0,r%d\n",p1,p2,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) return ILLEGAL_INSTR;
         if( ((int64)(0)<(int64)r[r2].dat.q) || r[r2].nat ) {
            p[p1] = p[p2] = 0;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xd*32+0*4+1*16+1*2+0:
         trcins("cmp.ge.or p%d,p%d = r0,r%d\n",p1,p2,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) return ILLEGAL_INSTR;
         if( (int64)(0)>=(int64)r[r2].dat.q ) {
            p[p1] = p[p2] = 1;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xe*32+0*4+1*16+1*2+0:
         trcins("cmp.ge.or.andcm p%d,p%d = r0,r%d\n",p1,p2,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) return ILLEGAL_INSTR;
         if( (int64)(0)>=(int64)r[r2].dat.q ) {
            p[p1] = 1; p[p2] = 0;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xc*32+0*4+1*16+1*2+1:
         trcins("cmp.lt.and p%d,p%d = r0,r%d\n",p1,p2,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) return ILLEGAL_INSTR;
         if( ((int64)(0)>=(int64)r[r2].dat.q) || r[r2].nat ) {
            p[p1] = p[p2] = 0;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xd*32+0*4+1*16+1*2+1:
         trcins("cmp.lt.or p%d,p%d = r0,r%d\n",p1,p2,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) return ILLEGAL_INSTR;
         if( (int64)(0)<(int64)r[r2].dat.q ) {
            p[p1] = p[p2] = 1;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xe*32+0*4+1*16+1*2+1:
         trcins("cmp.lt.or.andcm p%d,p%d = r0,r%d\n",p1,p2,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) return ILLEGAL_INSTR;
         if( (int64)(0)<(int64)r[r2].dat.q ) {
            p[p1] = 1; p[p2] = 0;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xc*32+1*4+1*16+0*2+0:
         trcins("cmp4.gt.and p%d,p%d = r0,r%d\n",p1,p2,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) return ILLEGAL_INSTR;
         if( ((int32)(0)<=(int32)LO(r[r2].dat)) || r[r2].nat ) {
            p[p1] = p[p2] = 0;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xd*32+1*4+1*16+0*2+0:
         trcins("cmp4.gt.or p%d,p%d = r0,r%d\n",p1,p2,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) return ILLEGAL_INSTR;
         if( (int32)(0)>(int32)LO(r[r2].dat) ) {
            p[p1] = p[p2] = 1;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xe*32+1*4+1*16+0*2+0:
         trcins("cmp4.gt.or.andcm p%d,p%d = r0,r%d\n",p1,p2,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) return ILLEGAL_INSTR;
         if( (int32)(0)>(int32)LO(r[r2].dat) ) {
            p[p1] = 1; p[p2] = 0;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xc*32+1*4+1*16+0*2+1:
         trcins("cmp4.le.and p%d,p%d = r0,r%d\n",p1,p2,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) return ILLEGAL_INSTR;
         if( ((int32)(0)>(int32)LO(r[r2].dat)) || r[r2].nat ) {
            p[p1] = p[p2] = 0;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xd*32+1*4+1*16+0*2+1:
         trcins("cmp4.le.or p%d,p%d = r0,r%d\n",p1,p2,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) return ILLEGAL_INSTR;
         if( (int32)(0)<=(int32)LO(r[r2].dat) ) {
            p[p1] = p[p2] = 1;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xe*32+1*4+1*16+0*2+1:
         trcins("cmp4.le.or.andcm p%d,p%d = r0,r%d\n",p1,p2,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) return ILLEGAL_INSTR;
         if( (int32)(0)<=(int32)LO(r[r2].dat) ) {
            p[p1] = 1; p[p2] = 0;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xc*32+1*4+1*16+1*2+0:
         trcins("cmp4.ge.and p%d,p%d = r0,r%d\n",p1,p2,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) return ILLEGAL_INSTR;
         if( ((int32)(0)<(int32)LO(r[r2].dat)) || r[r2].nat ) {
            p[p1] = p[p2] = 0;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xd*32+1*4+1*16+1*2+0:
         trcins("cmp4.ge.or p%d,p%d = r0,r%d\n",p1,p2,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) return ILLEGAL_INSTR;
         if( (int32)(0)>=(int32)LO(r[r2].dat) ) {
            p[p1] = p[p2] = 1;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xe*32+1*4+1*16+1*2+0:
         trcins("cmp4.ge.or.andcm p%d,p%d = r0,r%d\n",p1,p2,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) return ILLEGAL_INSTR;
         if( (int32)(0)>=(int32)LO(r[r2].dat) ) {
            p[p1] = 1; p[p2] = 0;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xc*32+1*4+1*16+1*2+1:
         trcins("cmp4.lt.and p%d,p%d = r0,r%d\n",p1,p2,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) return ILLEGAL_INSTR;
         if( ((int32)(0)>=(int32)LO(r[r2].dat)) || r[r2].nat ) {
            p[p1] = p[p2] = 0;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xd*32+1*4+1*16+1*2+1:
         trcins("cmp4.lt.or p%d,p%d = r0,r%d\n",p1,p2,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) return ILLEGAL_INSTR;
         if( (int32)(0)<(int32)LO(r[r2].dat) ) {
            p[p1] = p[p2] = 1;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xe*32+1*4+1*16+1*2+1:
         trcins("cmp4.lt.or.andcm p%d,p%d = r0,r%d\n",p1,p2,r2);
         if( p[pred]==0 ) return IA_OK;
         if(r1) return ILLEGAL_INSTR;
         if( (int32)(0)<(int32)LO(r[r2].dat) ) {
            p[p1] = 1; p[p2] = 0;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xc*32+2*4+0*16+0*2+0:
      case 0xc*32+2*4+1*16+0*2+0:
         trcins("cmp.lt p%d,p%d = %d,r%d\n",p1,p2,LO(imm),r2);
         if( p[pred]==0 ) return IA_OK;
         if( (int64)(imm.q) < (int64)(r[r2].dat.q) ) {
            p[p1] = 1; p[p2] = 0;
         }
         else {
            p[p1] = 0; p[p2] = 1;
         }
         if( r[r2].nat ) p[p1] = p[p2] = 0;
         if(p1) add_hazard(p1 | PRD_REG);
         if(p2) add_hazard(p2 | PRD_REG);
         p[0] = 1; return IA_OK;
            
      case 0xd*32+2*4+0*16+0*2+0:
      case 0xd*32+2*4+1*16+0*2+0:
         trcins("cmp.ltu p%d,p%d = %d,r%d\n",p1,p2,LO(imm),r2);
         if( p[pred]==0 ) return IA_OK;
         if( (uint64)(imm.q) < (uint64)(r[r2].dat.q) ) {
            p[p1] = 1; p[p2] = 0;
         }
         else {
            p[p1] = 0; p[p2] = 1;
         }
         if( r[r2].nat ) p[p1] = p[p2] = 0;
         if(p1) add_hazard(p1 | PRD_REG);
         if(p2) add_hazard(p2 | PRD_REG);
         p[0] = 1; return IA_OK;
            
      case 0xe*32+2*4+0*16+0*2+0:
      case 0xe*32+2*4+1*16+0*2+0:
         trcins("cmp.eq p%d,p%d = %d,r%d\n",p1,p2,LO(imm),r2);
         if( p[pred]==0 ) return IA_OK;
         if( imm.q == r[r2].dat.q ) {
            p[p1] = 1; p[p2] = 0;
         }
         else {
            p[p1] = 0; p[p2] = 1;
         }
         if( r[r2].nat ) p[p1] = p[p2] = 0;
         if(p1) add_hazard(p1 | PRD_REG);
         if(p2) add_hazard(p2 | PRD_REG);
         p[0] = 1; return IA_OK;
            
      case 0xc*32+2*4+0*16+0*2+1:
      case 0xc*32+2*4+1*16+0*2+1:
         trcins("cmp.lt.unc p%d,p%d = %d,r%d\n",p1,p2,LO(imm),r2);
         p[p1] = p[p2] = 0;
         if(p1) add_hazard(p1 | PRD_REG);
         if(p2) add_hazard(p2 | PRD_REG);
         if( p[pred]==0 ) {
             p[0] = 1; return IA_OK;
          }
         if( (int64)(imm.q) < (int64)(r[r2].dat.q) ) {
            p[p1] = 1; p[p2] = 0;
         }
         else {
            p[p1] = 0; p[p2] = 1;
         }
         if( r[r2].nat ) p[p1] = p[p2] = 0;
         p[0] = 1; return IA_OK;
            
      case 0xd*32+2*4+0*16+0*2+1:
      case 0xd*32+2*4+1*16+0*2+1:
         trcins("cmp.ltu.unc p%d,p%d = %d,r%d\n",p1,p2,LO(imm),r2);
         p[p1] = p[p2] = 0;
         if(p1) add_hazard(p1 | PRD_REG);
         if(p2) add_hazard(p2 | PRD_REG);
         if( p[pred]==0 ) {
             p[0] = 1; return IA_OK;
          }
         if( (uint64)(imm.q) < (uint64)(r[r2].dat.q) ) {
            p[p1] = 1; p[p2] = 0;
         }
         else {
            p[p1] = 0; p[p2] = 1;
         }
         if( r[r2].nat ) p[p1] = p[p2] = 0;
         p[0] = 1; return IA_OK;
            
      case 0xe*32+2*4+0*16+0*2+1:
      case 0xe*32+2*4+1*16+0*2+1:
         trcins("cmp.eq.unc p%d,p%d = %d,r%d\n",p1,p2,LO(imm),r2);
         p[p1] = p[p2] = 0;
         if(p1) add_hazard(p1 | PRD_REG);
         if(p2) add_hazard(p2 | PRD_REG);
         if( p[pred]==0 ) {
             p[0] = 1; return IA_OK;
          }
         if( imm.q == r[r2].dat.q ) {
            p[p1] = 1; p[p2] = 0;
         }
         else {
            p[p1] = 0; p[p2] = 1;
         }
         if( r[r2].nat ) p[p1] = p[p2] = 0;
         p[0] = 1; return IA_OK;
            
      case 0xc*32+2*4+0*16+1*2+0:
      case 0xc*32+2*4+1*16+1*2+0:
         trcins("cmp.eq.and p%d,p%d = %d,r%d\n",p1,p2,LO(imm),r2);
         if( p[pred]==0 ) return IA_OK;
         if( (imm.q!=r[r2].dat.q) || r[r2].nat ) {
            p[p1] = p[p2] = 0;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xd*32+2*4+0*16+1*2+0:
      case 0xd*32+2*4+1*16+1*2+0:
         trcins("cmp.eq.or p%d,p%d = %d,r%d\n",p1,p2,LO(imm),r2);
         if( p[pred]==0 ) return IA_OK;
         if( imm.q == r[r2].dat.q ) {
            p[p1] = p[p2] = 1;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xe*32+2*4+0*16+1*2+0:
      case 0xe*32+2*4+1*16+1*2+0:
         trcins("cmp.eq.or.andcm p%d,p%d = %d,r%d\n",p1,p2,LO(imm),r2);
         if( p[pred]==0 ) return IA_OK;
         if( imm.q == r[r2].dat.q ) {
            p[p1] = 1; p[p2] = 0;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xc*32+2*4+0*16+1*2+1:
      case 0xc*32+2*4+1*16+1*2+1:
         trcins("cmp.ne.and p%d,p%d = %d,r%d\n",p1,p2,LO(imm),r2);
         if( p[pred]==0 ) return IA_OK;
         if( (imm.q==r[r2].dat.q) || r[r2].nat ) {
            p[p1] = p[p2] = 0;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xd*32+2*4+0*16+1*2+1:
      case 0xd*32+2*4+1*16+1*2+1:
         trcins("cmp.ne.or p%d,p%d = %d,r%d\n",p1,p2,LO(imm),r2);
         if( p[pred]==0 ) return IA_OK;
         if( imm.q != r[r2].dat.q ) {
            p[p1] = p[p2] = 1;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xe*32+2*4+0*16+1*2+1:
      case 0xe*32+2*4+1*16+1*2+1:
         trcins("cmp.ne.or.andcm p%d,p%d = %d,r%d\n",p1,p2,LO(imm),r2);
         if( p[pred]==0 ) return IA_OK;
         if( imm.q != r[r2].dat.q ) {
            p[p1] = 1; p[p2] = 0;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xc*32+3*4+0*16+0*2+0:
      case 0xc*32+3*4+1*16+0*2+0:
         trcins("cmp4.lt p%d,p%d = %d,r%d\n",p1,p2,LO(imm),r2);
         if( p[pred]==0 ) return IA_OK;
         if( (int32)LO(imm) < (int32)LO(r[r2].dat) ) {
            p[p1] = 1; p[p2] = 0;
         }
         else {
            p[p1] = 0; p[p2] = 1;
         }
         if( r[r2].nat ) p[p1] = p[p2] = 0;
         if(p1) add_hazard(p1 | PRD_REG);
         if(p2) add_hazard(p2 | PRD_REG);
         p[0] = 1; return IA_OK;
            
      case 0xd*32+3*4+0*16+0*2+0:
      case 0xd*32+3*4+1*16+0*2+0:
         trcins("cmp4.ltu p%d,p%d = %d,r%d\n",p1,p2,LO(imm),r2);
         if( p[pred]==0 ) return IA_OK;
         if( (uint32)LO(imm) < (uint32)LO(r[r2].dat) ) {
            p[p1] = 1; p[p2] = 0;
         }
         else {
            p[p1] = 0; p[p2] = 1;
         }
         if( r[r2].nat ) p[p1] = p[p2] = 0;
         if(p1) add_hazard(p1 | PRD_REG);
         if(p2) add_hazard(p2 | PRD_REG);
         p[0] = 1; return IA_OK;
            
      case 0xe*32+3*4+0*16+0*2+0:
      case 0xe*32+3*4+1*16+0*2+0:
         trcins("cmp4.eq p%d,p%d = %d,r%d\n",p1,p2,LO(imm),r2);
         if( p[pred]==0 ) return IA_OK;
         if( LO(imm) == LO(r[r2].dat) ) {
            p[p1] = 1; p[p2] = 0;
         }
         else {
            p[p1] = 0; p[p2] = 1;
         }
         if( r[r2].nat ) p[p1] = p[p2] = 0;
         if(p1) add_hazard(p1 | PRD_REG);
         if(p2) add_hazard(p2 | PRD_REG);
         p[0] = 1; return IA_OK;
            
      case 0xc*32+3*4+0*16+0*2+1:
      case 0xc*32+3*4+1*16+0*2+1:
         trcins("cmp4.lt.unc p%d,p%d = %d,r%d\n",p1,p2,LO(imm),r2);
         p[p1] = p[p2] = 0;
         if(p1) add_hazard(p1 | PRD_REG);
         if(p2) add_hazard(p2 | PRD_REG);
         if( p[pred]==0 ) {
             p[0] = 1; return IA_OK;
          }
         if( (int32)LO(imm) < (int32)LO(r[r2].dat) ) {
            p[p1] = 1; p[p2] = 0;
         }
         else {
            p[p1] = 0; p[p2] = 1;
         }
         if( r[r2].nat ) p[p1] = p[p2] = 0;
         p[0] = 1; return IA_OK;
            
      case 0xd*32+3*4+0*16+0*2+1:
      case 0xd*32+3*4+1*16+0*2+1:
         trcins("cmp4.ltu.unc p%d,p%d = %d,r%d\n",p1,p2,LO(imm),r2);
         p[p1] = p[p2] = 0;
         if(p1) add_hazard(p1 | PRD_REG);
         if(p2) add_hazard(p2 | PRD_REG);
         if( p[pred]==0 ) {
             p[0] = 1; return IA_OK;
          }
         if( (uint32)LO(imm) < (uint32)LO(r[r2].dat) ) {
            p[p1] = 1; p[p2] = 0;
         }
         else {
            p[p1] = 0; p[p2] = 1;
         }
         if( r[r2].nat ) p[p1] = p[p2] = 0;
         p[0] = 1; return IA_OK;
            
      case 0xe*32+3*4+0*16+0*2+1:
      case 0xe*32+3*4+1*16+0*2+1:
         trcins("cmp4.eq.unc p%d,p%d = %d,r%d\n",p1,p2,LO(imm),r2);
         p[p1] = p[p2] = 0;
         if(p1) add_hazard(p1 | PRD_REG);
         if(p2) add_hazard(p2 | PRD_REG);
         if( p[pred]==0 ) {
             p[0] = 1; return IA_OK;
          }
         if( LO(imm) == LO(r[r2].dat) ) {
            p[p1] = 1; p[p2] = 0;
         }
         else {
            p[p1] = 0; p[p2] = 1;
         }
         if( r[r2].nat ) p[p1] = p[p2] = 0;
         p[0] = 1; return IA_OK;
            
      case 0xc*32+3*4+0*16+1*2+0:
      case 0xc*32+3*4+1*16+1*2+0:
         trcins("cmp4.eq.and p%d,p%d = %d,r%d\n",p1,p2,LO(imm),r2);
         if( p[pred]==0 ) return IA_OK;
         if( (LO(imm)!=LO(r[r2].dat)) || r[r2].nat ) {
            p[p1] = p[p2] = 0;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xd*32+3*4+0*16+1*2+0:
      case 0xd*32+3*4+1*16+1*2+0:
         trcins("cmp4.eq.or p%d,p%d = %d,r%d\n",p1,p2,LO(imm),r2);
         if( p[pred]==0 ) return IA_OK;
         if( LO(imm) == LO(r[r2].dat) ) {
            p[p1] = p[p2] = 1;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xe*32+3*4+0*16+1*2+0:
      case 0xe*32+3*4+1*16+1*2+0:
         trcins("cmp4.eq.or.andcm p%d,p%d = %d,r%d\n",p1,p2,LO(imm),r2);
         if( p[pred]==0 ) return IA_OK;
         if( LO(imm) == LO(r[r2].dat) ) {
            p[p1] = 1; p[p2] = 0;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xc*32+3*4+0*16+1*2+1:
      case 0xc*32+3*4+1*16+1*2+1:
         trcins("cmp4.ne.and p%d,p%d = %d,r%d\n",p1,p2,LO(imm),r2);
         if( p[pred]==0 ) return IA_OK;
         if( (LO(imm)==LO(r[r2].dat)) || r[r2].nat ) {
            p[p1] = p[p2] = 0;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xd*32+3*4+0*16+1*2+1:
      case 0xd*32+3*4+1*16+1*2+1:
         trcins("cmp4.ne.or p%d,p%d = %d,r%d\n",p1,p2,LO(imm),r2);
         if( p[pred]==0 ) return IA_OK;
         if( LO(imm) != LO(r[r2].dat) ) {
            p[p1] = p[p2] = 1;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0xe*32+3*4+0*16+1*2+1:
      case 0xe*32+3*4+1*16+1*2+1:
         trcins("cmp4.ne.or.andcm p%d,p%d = %d,r%d\n",p1,p2,LO(imm),r2);
         if( p[pred]==0 ) return IA_OK;
         if( LO(imm) != LO(r[r2].dat) ) {
            p[p1] = 1; p[p2] = 0;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
   }

   return ILLEGAL_INSTR;
}            


