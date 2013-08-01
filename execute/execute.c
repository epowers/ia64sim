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

/* instruction decode for IA64 */

#include "ia64.h"
#include "execute.h"

#define E0 (mask & 0x1)
#define E1 (mask & 0x2)
#define E2 (mask & 0x4)

/*-------------------------------------------------------------------*/
uint32 execute_instruction( bundle *vliw, uint32 mask ) {

   uint32 template;
   uint32 status;

   status = IA_OK;
   template = LO(vliw->lo_q) & 0x1f;

   slot[0].q = ( vliw->lo_q.q >> 5 ); HI(slot[0]) &= 0x1ff;

   slot[1].q = ( vliw->lo_q.q >> 46 ) | ( vliw->hi_q.q << 18 );
   HI(slot[1]) &= 0x1ff;

   slot[2].q = ( vliw->hi_q.q >> 23 ); HI(slot[2]) &= 0x1ff;

   if( (template&~1)==bbb) {
      if(E0) status = b_unit(&slot[0]);
      if(!status && E1) status = b_unit(&slot[1]);
      if(!status && E2) status = b_unit(&slot[2]);
      if( (template&1) && !status && E2) stop();
      return status;
   }

   if(E0) status = m_unit(&slot[0]);

   switch(template & ~1) {
      case mii:
         if(!status && E1) status = i_unit(&slot[1]);
         if(!status && E2) status = i_unit(&slot[2]);
         break;
      case misi:
         if(!status && E1) {
            status = i_unit(&slot[1]);
            stop();
            }
         if(!status && E2) status = i_unit(&slot[2]);
         break;
      case mx:
         if(!status && E1) status = x_unit(&slot[1]); 
         break;
      case mmi:
         if(!status && E1) status = m_unit(&slot[1]);
         if(!status && E2) status = i_unit(&slot[2]);
         break;
      case msmi:
         if(!status && E0) stop();
         if(!status && E1) status = m_unit(&slot[1]);
         if(!status && E2) status = i_unit(&slot[2]);
         break;
      case mfi:
         if(!status && E1) status = f_unit(&slot[1]);
         if(!status && E2) status = i_unit(&slot[2]);
         break;
      case mmf:
         if(!status && E1) status = m_unit(&slot[1]);
         if(!status && E2) status = f_unit(&slot[2]);
         break;
      case mib:
         if(!status && E1) status = i_unit(&slot[1]);
         if(!status && E2) status = b_unit(&slot[2]);
         break;
      case mbb:
         if(!status && E1) status = b_unit(&slot[1]);
         if(!status && E2) status = b_unit(&slot[2]);
         break;
      case mmb:
         if(!status && E1) status = m_unit(&slot[1]);
         if(!status && E2) status = b_unit(&slot[2]);
         break;
      case mfb:
         if(!status && E1) status = f_unit(&slot[1]);
         if(!status && E2) status = b_unit(&slot[2]);
         break;

      default:
         return INVALID_BUNDLE;
   }

   if( (template&1) && !status && E2) stop();

   return status;
}

/*---------------------------------------------------------------------*/
void stop(void) {

   /* Represents a barrier to multiple issue; all instructions
      issued before this call must complete before further
      instructions are issued.

      Since this is only an ISA-level simulator, the only practical
      effect of a stop is that register hazards for the next
      instruction are impossible (except for r0, f0, and f1) */
      
 
   num_reg_written = 3;

}

/*---------------------------------------------------------------------*/
uint32 i_unit(qword *instr) {

   uint32 pred = LO(instr[0])&63;

   check_reg(pred | PRD_REG);

   switch( HI(instr[0]) >> 5 ) {
      case 0: if( p[pred]==0 ) return IA_OK; return i_misc(instr);
      case 4: if( p[pred]==0 ) return IA_OK; return i_deposit(instr);
      case 5: if(!BITS(instr[0],34,35)) return i_test(instr);
              if( p[pred]==0 ) return IA_OK; return i_shift(instr);
      case 7: if( p[pred]==0 ) return IA_OK; return i_mmshift(instr);
      case 8: if( p[pred]==0 ) return IA_OK; return i_alu(instr);
      case 9: if( p[pred]==0 ) return IA_OK; return i_addi22(instr);
      case 12:
      case 13:
      case 14: return i_compare(instr);
      default: return ILLEGAL_INSTR;
   }
}

/*---------------------------------------------------------------------*/
uint32 m_unit(qword *instr) {

   uint32 pred = LO(instr[0])&63;

   check_reg(pred | PRD_REG);

   switch( HI(instr[0]) >> 5 ) {
      case 0:
      case 1: if( p[pred]==0 ) return IA_OK; return m_mgmt(instr);
      case 4: if( p[pred]==0 ) return IA_OK; return m_ldir(instr);
      case 5: if( p[pred]==0 ) return IA_OK; return m_ldii(instr);
      case 6: if( p[pred]==0 ) return IA_OK; return m_ldfr(instr);
      case 7: if( p[pred]==0 ) return IA_OK; return m_ldfi(instr);
      case 8: if( p[pred]==0 ) return IA_OK; return i_alu(instr);
      case 9: if( p[pred]==0 ) return IA_OK; return i_addi22(instr);
      case 12:
      case 13:
      case 14: return i_compare(instr);
      default: return ILLEGAL_INSTR;
   }
}

/*---------------------------------------------------------------------*/
uint32 f_unit(qword *instr) {

   uint32 pred = LO(instr[0])&63;

   check_reg(pred | PRD_REG);

   if( p[pred]==0 ) return IA_OK;

   switch( HI(instr[0]) >> 5 ) {
      case 0:
      case 1: return f_misc(instr);
      case 4: return f_compare(instr);
      case 5: return f_class(instr);
      case 8:
      case 9: return f_fma(instr);
      case 10:
      case 11: return f_fms(instr);
      case 12:
      case 13: return f_fnma(instr);
      case 14: return f_xma(instr);
      default: return ILLEGAL_INSTR;
   }
}


/*---------------------------------------------------------------------*/
uint32 b_unit(qword *instr) {

   uint32 major_opcode = HI(instr[0]) >> 5;
   uint32 opcode_ext = (BITS(instr[0], 27,33));
   
   if(major_opcode & 0x80) return ILLEGAL_INSTR;

   switch( major_opcode ) {
      case 0: 
         switch(opcode_ext) {
         case 0: return b_break(instr); /* break instruction */
         case 4: return UNIMPLEMENTED; /* clrrb */
         case 5: return UNIMPLEMENTED; /* clrrb.pr */
         case 32: return b_branchr(instr); /* indirect branch */
         case 33: return b_branchr(instr); /* indirect branch */
         default: return ILLEGAL_INSTR;
         }
      case 1: return b_callr(instr); /* indirect call */
      case 2: return b_nop(instr);  /* nop */
      case 4: return b_branchi(instr); /* ip relative or counted branch */
      case 5: return b_calli(instr); /* ip relative call */
      case 3:
      case 6:
      case 7: return ILLEGAL_INSTR; /* not defined in ADAG */
      default: return ILLEGAL_INSTR;
   }
}

/*---------------------------------------------------------------------*/
uint32 x_unit(qword *instr) {

   uint32 pred = LO(instr[0])&63;

   check_reg(pred | PRD_REG);

   if( p[pred]==0 ) return IA_OK;

   switch( HI(instr[0]) >> 5 ) {
      case 0: return x_misc(instr);
      case 6: return x_movl(instr);
      default: return ILLEGAL_INSTR;
   }
}
