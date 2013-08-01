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

void i_unit_decode(qword *instr, char *dest);
void f_unit_decode(qword *instr, char *dest);
void b_unit_decode(qword *instr, char *dest);
void m_unit_decode(qword *instr, char *dest);
void x_unit_decode(qword *instr, char *dest);

void i_misc_decode(qword *instr, char *dest);
void i_deposit_decode(qword *instr, char *dest);
void i_shift_decode(qword *instr, char *dest);
void i_test_decode(qword *instr, char *dest);
void i_mmshift_decode(qword *instr, char *dest);
void i_alu_decode(qword *instr, char *dest);
void i_addi22_decode(qword *instr, char *dest);
void i_compare_decode(qword *instr, char *dest);
void m_mgmt_decode(qword *instr, char *dest);
void m_ldir_decode(qword *instr, char *dest);
void m_ldii_decode(qword *instr, char *dest);
void m_ldfr_decode(qword *instr, char *dest);
void m_ldfi_decode(qword *instr, char *dest);
void f_misc_decode(qword *instr, char *dest);
void f_compare_decode(qword *instr, char *dest);
void f_class_decode(qword *instr, char *dest);
void f_fma_decode(qword *instr, char *dest);
void f_fms_decode(qword *instr, char *dest);
void f_fnma_decode(qword *instr, char *dest);
void f_xma_decode(qword *instr, char *dest);
void b_branchr_decode(qword *instr, char *dest);
void b_callr_decode(qword *instr, char *dest);
void b_nop_decode(qword *instr, char *dest);
void b_branchi_decode(qword *instr, char *dest);
void b_calli_decode(qword *instr, char *dest);
void x_misc_decode(qword *instr, char *dest);
void x_movl_decode(qword *instr, char *dest);

#ifndef E0
#define E0 (mask & 0x1)
#define E1 (mask & 0x2)
#define E2 (mask & 0x4)
#endif

/*-------------------------------------------------------------------*/
void decode_instruction( bundle *vliw, uint32 mask, char *dest ) {

   uint32 template;
   uint32 ignored = TRUE;

   template = LO(vliw->lo_q) & 0x1f;

   slot[0].q = ( vliw->lo_q.q >> 5 ); HI(slot[0]) &= 0x1ff;

   slot[1].q = ( vliw->lo_q.q >> 46 ) | ( vliw->hi_q.q << 18 );
   HI(slot[1]) &= 0x1ff;

   slot[2].q = ( vliw->hi_q.q >> 23 ); HI(slot[2]) &= 0x1ff;

   if( (template&~1)==bbb) {
      if(E0) b_unit_decode(&slot[0], dest);
      if(E1) b_unit_decode(&slot[1], dest);
      if(E2) b_unit_decode(&slot[2], dest);
      return;
   }

   if(E0) m_unit_decode(&slot[0], dest);

   switch(template & ~1) {
      case mii:
         if(E1) i_unit_decode(&slot[1], dest);
         if(E2) i_unit_decode(&slot[2], dest);
         break;
      case misi:
         if(E1) i_unit_decode(&slot[1], dest);
         if(E1) sprintf( dest+strlen(dest), "  ;;" );
         if(E2) i_unit_decode(&slot[2], dest);
         break;
      case mx:
         if(E1) x_unit_decode(&slot[1], dest); 
         if(E2) dest[0] = 0;
         break;
      case mmi:
         if(E1) m_unit_decode(&slot[1], dest);
         if(E2) i_unit_decode(&slot[2], dest);
         break;
      case msmi:
         if(E0) sprintf( dest+strlen(dest), "  ;;" );
         if(E1) m_unit_decode(&slot[1], dest);
         if(E2) i_unit_decode(&slot[2], dest);
         break;
      case mfi:
         if(E1) f_unit_decode(&slot[1], dest);
         if(E2) i_unit_decode(&slot[2], dest);
         break;
      case mmf:
         if(E1) m_unit_decode(&slot[1], dest);
         if(E2) f_unit_decode(&slot[2], dest);
         break;
      case mib:
         if(E1) i_unit_decode(&slot[1], dest);
         if(E2) b_unit_decode(&slot[2], dest);
         break;
      case mbb:
         if(E1) b_unit_decode(&slot[1], dest);
         if(E2) b_unit_decode(&slot[2], dest);
         break;
      case mmb:
         if(E1) m_unit_decode(&slot[1], dest);
         if(E2) b_unit_decode(&slot[2], dest);
         break;
      case mfb:
         if(E1) f_unit_decode(&slot[1], dest);
         if(E2) b_unit_decode(&slot[2], dest);
         break;

      default:
         sprintf(dest, "invalid bundle");
         return;
   }
 
   if( E2 && (template&1) )
      sprintf( dest+strlen(dest), "  ;;" );
}

/*---------------------------------------------------------------------*/
void i_unit_decode(qword *instr, char *dest) {

   uint32 offset;
   uint32 pred = LO(instr[0]) & 63;

   if(pred && pred<10) 
      sprintf(dest, " (p%d) ", pred);
   else if(pred) 
      sprintf(dest, "(p%d) ", pred);
   else 
      sprintf(dest, "      ", pred);
   offset = strlen(dest);

   switch( HI(instr[0]) >> 5 ) {
      case 0: i_misc_decode(instr, dest+offset); return;
      case 4: i_deposit_decode(instr, dest+offset); return;
      case 5: if(!BITS(instr[0],34,35)) {
                 i_test_decode(instr, dest+offset); return;
              }
              i_shift_decode(instr, dest+offset); return;
      case 7: i_mmshift_decode(instr, dest+offset); return;
      case 8: i_alu_decode(instr, dest+offset); return;
      case 9: i_addi22_decode(instr, dest+offset); return;
      case 12:
      case 13:
      case 14: i_compare_decode(instr, dest+offset); return;
   }
}

/*---------------------------------------------------------------------*/
void m_unit_decode(qword *instr, char *dest) {

   uint32 offset;
   uint32 pred = LO(instr[0]) & 63;

   if(pred && pred<10) 
      sprintf(dest, " (p%d) ", pred);
   else if(pred) 
      sprintf(dest, "(p%d) ", pred);
   else 
      sprintf(dest, "      ", pred);
   offset = strlen(dest);

   switch( HI(instr[0]) >> 5 ) {
      case 0:
      case 1: m_mgmt_decode(instr, dest+offset); return;
      case 4: m_ldir_decode(instr, dest+offset); return;
      case 5: m_ldii_decode(instr, dest+offset); return;
      case 6: m_ldfr_decode(instr, dest+offset); return;
      case 7: m_ldfi_decode(instr, dest+offset); return;
      case 8: i_alu_decode(instr, dest+offset); return;
      case 9: i_addi22_decode(instr, dest+offset); return;
      case 12:
      case 13:
      case 14: i_compare_decode(instr, dest+offset); return;
   }
}

/*---------------------------------------------------------------------*/
void f_unit_decode(qword *instr, char *dest) {

   uint32 offset;
   uint32 pred = LO(instr[0]) & 63;

   if(pred && pred<10) 
      sprintf(dest, " (p%d) ", pred);
   else if(pred) 
      sprintf(dest, "(p%d) ", pred);
   else
      sprintf(dest, "      ", pred);
   offset = strlen(dest);

   switch( HI(instr[0]) >> 5 ) {
      case 0:
      case 1: f_misc_decode(instr, dest+offset); return;
      case 4: f_compare_decode(instr, dest+offset); return;
      case 5: f_class_decode(instr, dest+offset); return;
      case 8:
      case 9: f_fma_decode(instr, dest+offset); return;
      case 10:
      case 11: f_fms_decode(instr, dest+offset); return;
      case 12:
      case 13: f_fnma_decode(instr, dest+offset); return;
      case 14: f_xma_decode(instr, dest+offset); return;
   }
}


/*---------------------------------------------------------------------*/
void b_unit_decode(qword *instr, char *dest) {

   uint32 major_opcode = HI(instr[0]) >> 5;
   uint32 offset;
   uint32 pred = LO(instr[0]) & 63;

   if(major_opcode & 0x80) {
      sprintf(dest, "illegal instruction");
      return;
   }

   if(pred && pred<10) 
      sprintf(dest, " (p%d) ", pred);
   else if(pred) 
      sprintf(dest, "(p%d) ", pred);
   else 
      sprintf(dest, "      ", pred);
   offset = strlen(dest);

   switch( major_opcode ) {
      case 0: b_branchr_decode(instr, dest+offset); return;
      case 1: b_callr_decode(instr, dest+offset); return;
      case 2: b_nop_decode(instr, dest+offset); return;
      case 4: b_branchi_decode(instr, dest+offset); return;
      case 5: b_calli_decode(instr, dest+offset); return;
      case 3:
      case 6:
      case 7: sprintf(dest, "ignored"); return;
   }
}

/*---------------------------------------------------------------------*/
void x_unit_decode(qword *instr, char *dest) {

   uint32 offset;
   uint32 pred = LO(instr[0]) & 63;

   if(pred && pred<10) 
      sprintf(dest, " (p%d) ", pred);
   else if(pred) 
      sprintf(dest, "(p%d) ", pred);
   else 
      sprintf(dest, "      ", pred);
   offset = strlen(dest);

   switch( HI(instr[0]) >> 5 ) {
      case 0: x_misc_decode(instr, dest+offset); return;
      case 6: x_movl_decode(instr, dest+offset); return;
   }
}

/*----------------------------------------------------------------------*/
void i_compare_decode(qword *instr, char *dest) {

   uint32 p1, p2, r1, r2, pred;
   uint32 opcode;
   qword imm;

   pred = LO(instr[0])&63;
   p1 = BITS(instr[0],6,11);
   p2 = BITS(instr[0],27,32);
   r1 = BITS(instr[0],13,19);
   r2 = BITS(instr[0],20,26);

   if( p[pred]==1 && p1==p2 ) {
      sprintf(dest, "illegal instruction");
      return;
   }

   opcode = BITS(instr[0],33,40) << 1 |
            BITS(instr[0],12,12);
   imm.q = ((-(int64)(opcode>>3)) << 7 | r1) & 127;

   switch(opcode) {

      case 0xc*32+0*4+0*16+0*2+0:
         sprintf(dest,"cmp.lt p%d,p%d = r%d,r%d",p1,p2,r1,r2);
         return;            
      case 0xd*32+0*4+0*16+0*2+0:
         sprintf(dest,"cmp.ltu p%d,p%d = r%d,r%d",p1,p2,r1,r2);
         return;
      case 0xe*32+0*4+0*16+0*2+0:
         sprintf(dest,"cmp.eq p%d,p%d = r%d,r%d",p1,p2,r1,r2);
         return;
      case 0xc*32+0*4+0*16+0*2+1:
         sprintf(dest,"cmp.lt.unc p%d,p%d = r%d,r%d",p1,p2,r1,r2);
         return;
      case 0xd*32+0*4+0*16+0*2+1:
         sprintf(dest,"cmp.ltu.unc p%d,p%d = r%d,r%d",p1,p2,r1,r2);
         return;
      case 0xe*32+0*4+0*16+0*2+1:
         sprintf(dest,"cmp.eq.unc p%d,p%d = r%d,r%d",p1,p2,r1,r2);
         return;
      case 0xc*32+0*4+0*16+1*2+0:
         sprintf(dest,"cmp.eq.and p%d,p%d = r%d,r%d",p1,p2,r1,r2);
         return;
      case 0xd*32+0*4+0*16+1*2+0:
         sprintf(dest,"cmp.eq.or p%d,p%d = r%d,r%d",p1,p2,r1,r2);
         return;
      case 0xe*32+0*4+0*16+1*2+0:
         sprintf(dest,"cmp.eq.or.andcm p%d,p%d = r%d,r%d",p1,p2,r1,r2);
         return;
      case 0xc*32+0*4+0*16+1*2+1:
         sprintf(dest,"cmp.ne.and p%d,p%d = r%d,r%d",p1,p2,r1,r2);
         return;
      case 0xd*32+0*4+0*16+1*2+1:
         sprintf(dest,"cmp.ne.or p%d,p%d = r%d,r%d",p1,p2,r1,r2);
         return;
      case 0xe*32+0*4+0*16+1*2+1:
         sprintf(dest,"cmp.ne.or.andcm p%d,p%d = r%d,r%d",p1,p2,r1,r2);
         return;
      case 0xc*32+1*4+0*16+0*2+0:
         sprintf(dest,"cmp4.lt p%d,p%d = r%d,r%d",p1,p2,r1,r2);
         return;
      case 0xd*32+1*4+0*16+0*2+0:
         sprintf(dest,"cmp4.ltu p%d,p%d = r%d,r%d",p1,p2,r1,r2);
         return;
      case 0xe*32+1*4+0*16+0*2+0:
         sprintf(dest,"cmp4.eq p%d,p%d = r%d,r%d",p1,p2,r1,r2);
         return;
      case 0xc*32+1*4+0*16+0*2+1:
         sprintf(dest,"cmp4.lt.unc p%d,p%d = r%d,r%d",p1,p2,r1,r2);
         return;
      case 0xd*32+1*4+0*16+0*2+1:
         sprintf(dest,"cmp4.ltu.unc p%d,p%d = r%d,r%d",p1,p2,r1,r2);
         return;
      case 0xe*32+1*4+0*16+0*2+1:
         sprintf(dest,"cmp4.eq.unc p%d,p%d = r%d,r%d",p1,p2,r1,r2);
         return;
      case 0xc*32+1*4+0*16+1*2+0:
         sprintf(dest,"cmp4.eq.and p%d,p%d = r%d,r%d",p1,p2,r1,r2);
         return;
      case 0xd*32+1*4+0*16+1*2+0:
         sprintf(dest,"cmp4.eq.or p%d,p%d = r%d,r%d",p1,p2,r1,r2);
         return;
      case 0xe*32+1*4+0*16+1*2+0:
         sprintf(dest,"cmp4.eq.or.andcm p%d,p%d = r%d,r%d",p1,p2,r1,r2);
         return;
      case 0xc*32+1*4+0*16+1*2+1:
         sprintf(dest,"cmp4.ne.and p%d,p%d = r%d,r%d",p1,p2,r1,r2);
         return;
      case 0xd*32+1*4+0*16+1*2+1:
         sprintf(dest,"cmp4.ne.or p%d,p%d = r%d,r%d",p1,p2,r1,r2);
         return;
      case 0xe*32+1*4+0*16+1*2+1:
         sprintf(dest,"cmp4.ne.or.andcm p%d,p%d = r%d,r%d",p1,p2,r1,r2);
         return;
      case 0xc*32+0*4+1*16+0*2+0:
         sprintf(dest,"cmp.gt.and p%d,p%d = r0,r%d",p1,p2,r2);
         return;
      case 0xd*32+0*4+1*16+0*2+0:
         sprintf(dest,"cmp.gt.or p%d,p%d = r0,r%d",p1,p2,r2);
         return;
      case 0xe*32+0*4+1*16+0*2+0:
         sprintf(dest,"cmp.gt.or.andcm p%d,p%d = r0,r%d",p1,p2,r2);
         return;
      case 0xc*32+0*4+1*16+0*2+1:
         sprintf(dest,"cmp.le.and p%d,p%d = r0,r%d",p1,p2,r2);
         return;
      case 0xd*32+0*4+1*16+0*2+1:
         sprintf(dest,"cmp.le.or p%d,p%d = r0,r%d",p1,p2,r2);
         return;
      case 0xe*32+0*4+1*16+0*2+1:
         sprintf(dest,"cmp.le.or.andcm p%d,p%d = r0,r%d",p1,p2,r2);
         return;
      case 0xc*32+0*4+1*16+1*2+0:
         sprintf(dest,"cmp.ge.and p%d,p%d = r0,r%d",p1,p2,r2);
         return;
      case 0xd*32+0*4+1*16+1*2+0:
         sprintf(dest,"cmp.ge.or p%d,p%d = r0,r%d",p1,p2,r2);
         return;
      case 0xe*32+0*4+1*16+1*2+0:
         return;
      case 0xc*32+0*4+1*16+1*2+1:
         sprintf(dest,"cmp.lt.and p%d,p%d = r0,r%d",p1,p2,r2);
         return;
      case 0xd*32+0*4+1*16+1*2+1:
         sprintf(dest,"cmp.lt.or p%d,p%d = r0,r%d",p1,p2,r2);
         return;
      case 0xe*32+0*4+1*16+1*2+1:
         sprintf(dest,"cmp.lt.or.andcm p%d,p%d = r0,r%d",p1,p2,r2);
         return;
      case 0xc*32+1*4+1*16+0*2+0:
         sprintf(dest,"cmp4.gt.and p%d,p%d = r0,r%d",p1,p2,r2);
         return;
      case 0xd*32+1*4+1*16+0*2+0:
         sprintf(dest,"cmp4.gt.or p%d,p%d = r0,r%d",p1,p2,r2);
         return;
      case 0xe*32+1*4+1*16+0*2+0:
         sprintf(dest,"cmp4.gt.or.andcm p%d,p%d = r0,r%d",p1,p2,r2);
         return;
      case 0xc*32+1*4+1*16+0*2+1:
         sprintf(dest,"cmp4.le.and p%d,p%d = r0,r%d",p1,p2,r2);
         return;
      case 0xd*32+1*4+1*16+0*2+1:
         sprintf(dest,"cmp4.le.or p%d,p%d = r0,r%d",p1,p2,r2);
         return;
      case 0xe*32+1*4+1*16+0*2+1:
         sprintf(dest,"cmp4.le.or.andcm p%d,p%d = r0,r%d",p1,p2,r2);
         return;
      case 0xc*32+1*4+1*16+1*2+0:
         sprintf(dest,"cmp4.ge.and p%d,p%d = r0,r%d",p1,p2,r2);
         return;
      case 0xd*32+1*4+1*16+1*2+0:
         sprintf(dest,"cmp4.ge.or p%d,p%d = r0,r%d",p1,p2,r2);
         return;
      case 0xe*32+1*4+1*16+1*2+0:
         sprintf(dest,"cmp4.ge.or.andcm p%d,p%d = r0,r%d",p1,p2,r2);
         return;
      case 0xc*32+1*4+1*16+1*2+1:
         sprintf(dest,"cmp4.lt.and p%d,p%d = r0,r%d",p1,p2,r2);
         return;
      case 0xd*32+1*4+1*16+1*2+1:
         sprintf(dest,"cmp4.lt.or p%d,p%d = r0,r%d",p1,p2,r2);
         return;
      case 0xe*32+1*4+1*16+1*2+1:
         sprintf(dest,"cmp4.lt.or.andcm p%d,p%d = r0,r%d",p1,p2,r2);
         return;
      case 0xc*32+2*4+0*16+0*2+0:
      case 0xc*32+2*4+1*16+0*2+0:
         sprintf(dest,"cmp.lt p%d,p%d = %d,r%d",p1,p2,LO(imm),r2);
         return;
      case 0xd*32+2*4+0*16+0*2+0:
      case 0xd*32+2*4+1*16+0*2+0:
         sprintf(dest,"cmp.ltu p%d,p%d = %d,r%d",p1,p2,LO(imm),r2);
         return;
      case 0xe*32+2*4+0*16+0*2+0:
      case 0xe*32+2*4+1*16+0*2+0:
         sprintf(dest,"cmp.eq p%d,p%d = %d,r%d",p1,p2,LO(imm),r2);
         return;
      case 0xc*32+2*4+0*16+0*2+1:
      case 0xc*32+2*4+1*16+0*2+1:
         sprintf(dest,"cmp.lt.unc p%d,p%d = %d,r%d",p1,p2,LO(imm),r2);
         return;
      case 0xd*32+2*4+0*16+0*2+1:
      case 0xd*32+2*4+1*16+0*2+1:
         sprintf(dest,"cmp.ltu.unc p%d,p%d = %d,r%d",p1,p2,LO(imm),r2);
         return;
      case 0xe*32+2*4+0*16+0*2+1:
      case 0xe*32+2*4+1*16+0*2+1:
         sprintf(dest,"cmp.eq.unc p%d,p%d = %d,r%d",p1,p2,LO(imm),r2);
         return;
      case 0xc*32+2*4+0*16+1*2+0:
      case 0xc*32+2*4+1*16+1*2+0:
         sprintf(dest,"cmp.eq.and p%d,p%d = %d,r%d",p1,p2,LO(imm),r2);
         return;
      case 0xd*32+2*4+0*16+1*2+0:
      case 0xd*32+2*4+1*16+1*2+0:
         sprintf(dest,"cmp.eq.or p%d,p%d = %d,r%d",p1,p2,LO(imm),r2);
         return;
      case 0xe*32+2*4+0*16+1*2+0:
      case 0xe*32+2*4+1*16+1*2+0:
         sprintf(dest,"cmp.eq.or.andcm p%d,p%d = %d,r%d",p1,p2,LO(imm),r2);
         return;
      case 0xc*32+2*4+0*16+1*2+1:
      case 0xc*32+2*4+1*16+1*2+1:
         sprintf(dest,"cmp.ne.and p%d,p%d = %d,r%d",p1,p2,LO(imm),r2);
         return;
      case 0xd*32+2*4+0*16+1*2+1:
      case 0xd*32+2*4+1*16+1*2+1:
         sprintf(dest,"cmp.ne.or p%d,p%d = %d,r%d",p1,p2,LO(imm),r2);
         return;
      case 0xe*32+2*4+0*16+1*2+1:
      case 0xe*32+2*4+1*16+1*2+1:
         sprintf(dest,"cmp.ne.or.andcm p%d,p%d = %d,r%d",p1,p2,LO(imm),r2);
         return;
      case 0xc*32+3*4+0*16+0*2+0:
      case 0xc*32+3*4+1*16+0*2+0:
         sprintf(dest,"cmp4.lt p%d,p%d = %d,r%d",p1,p2,LO(imm),r2);
         return;
      case 0xd*32+3*4+0*16+0*2+0:
      case 0xd*32+3*4+1*16+0*2+0:
         sprintf(dest,"cmp4.ltu p%d,p%d = %d,r%d",p1,p2,LO(imm),r2);
         return;
      case 0xe*32+3*4+0*16+0*2+0:
      case 0xe*32+3*4+1*16+0*2+0:
         sprintf(dest,"cmp4.eq p%d,p%d = %d,r%d",p1,p2,LO(imm),r2);
         return;
      case 0xc*32+3*4+0*16+0*2+1:
      case 0xc*32+3*4+1*16+0*2+1:
         sprintf(dest,"cmp4.lt.unc p%d,p%d = %d,r%d",p1,p2,LO(imm),r2);
         return;
      case 0xd*32+3*4+0*16+0*2+1:
      case 0xd*32+3*4+1*16+0*2+1:
         sprintf(dest,"cmp4.ltu.unc p%d,p%d = %d,r%d",p1,p2,LO(imm),r2);
         return;
      case 0xe*32+3*4+0*16+0*2+1:
      case 0xe*32+3*4+1*16+0*2+1:
         sprintf(dest,"cmp4.eq.unc p%d,p%d = %d,r%d",p1,p2,LO(imm),r2);
         return;
      case 0xc*32+3*4+0*16+1*2+0:
      case 0xc*32+3*4+1*16+1*2+0:
         sprintf(dest,"cmp4.eq.and p%d,p%d = %d,r%d",p1,p2,LO(imm),r2);
         return;
      case 0xd*32+3*4+0*16+1*2+0:
      case 0xd*32+3*4+1*16+1*2+0:
         sprintf(dest,"cmp4.eq.or p%d,p%d = %d,r%d",p1,p2,LO(imm),r2);
         return;
      case 0xe*32+3*4+0*16+1*2+0:
      case 0xe*32+3*4+1*16+1*2+0:
         sprintf(dest,"cmp4.eq.or.andcm p%d,p%d = %d,r%d",p1,p2,LO(imm),r2);
         return;
      case 0xc*32+3*4+0*16+1*2+1:
      case 0xc*32+3*4+1*16+1*2+1:
         sprintf(dest,"cmp4.ne.and p%d,p%d = %d,r%d",p1,p2,LO(imm),r2);
         return;
      case 0xd*32+3*4+0*16+1*2+1:
      case 0xd*32+3*4+1*16+1*2+1:
         sprintf(dest,"cmp4.ne.or p%d,p%d = %d,r%d",p1,p2,LO(imm),r2);
         return;
      case 0xe*32+3*4+0*16+1*2+1:
      case 0xe*32+3*4+1*16+1*2+1:
         sprintf(dest,"cmp4.ne.or.andcm p%d,p%d = %d,r%d",p1,p2,LO(imm),r2);
         return;
   }

   sprintf(dest, "illegal instruction");
}            

/*----------------------------------------------------------------------*/
void i_alu_decode(qword *instr, char *dest) { 

   uint32 rs1, rs2, rd;
   uint32 opcode;
   uint32 i, temp;
   qword imm;

   rs2 = BITS(instr[0],20,26);  
   rs1 = BITS(instr[0],13,19);
   rd = BITS(instr[0],6,12);

   opcode = BITS(instr[0],27,35);

   switch(opcode >> 6) { 

      case 2*2+0:
         imm.q = (-(uint64)BITS(instr[0],36,36)) << 13;
         imm.q |= (uint64)( rs1 | (opcode & 0x3f) << 7 );
         sprintf(dest,"add r%d = %d, r%d", rd,LO(imm),rs2 );
         return;
      case 3*2+0:
         imm.q = (-(uint64)BITS(instr[0],36,36)) << 13;
         imm.q |= (uint64)( rs1 | (opcode & 0x3f) << 7 );
         sprintf(dest,"addp4 r%d = %d, r%d", rd,LO(imm),rs2 );
         return;
   }
   switch(opcode) { 

      case 0:
         sprintf(dest,"add r%d = r%d, r%d", rd,rs1,rs2 );
         return;
      case 1:
         sprintf(dest,"add r%d = r%d, r%d, 1", rd,rs1,rs2 );
         return;
      case 1*4+1:
         sprintf(dest,"sub r%d = r%d, r%d", rd,rs1,rs2 );
         return;
      case 1*4+0:
         sprintf(dest,"sub r%d = r%d, r%d, 1", rd,rs1,rs2 );
         return;
      case 2*4+0:
         sprintf(dest,"addp4 r%d = r%d, r%d", rd,rs1,rs2 );
         return;
      case 3*4+0:
         sprintf(dest,"and r%d = r%d, r%d", rd,rs1,rs2 );
         return;
      case 3*4+1:
         sprintf(dest,"andcm r%d = r%d, r%d", rd,rs1,rs2 );
         return;
      case 3*4+2:
         sprintf(dest,"or r%d = r%d, r%d", rd,rs1,rs2 );
         return;
      case 3*4+3:
         sprintf(dest,"xor r%d = r%d, r%d", rd,rs1,rs2 );
         return;
      case 4*4+0:
      case 4*4+1:
      case 4*4+2:
      case 4*4+3:
         sprintf(dest,"shladd r%d = r%d, %d, r%d", rd,rs1,opcode&3,rs2 );
         return;
      case 6*4+0:
      case 6*4+1:
      case 6*4+2:
      case 6*4+3:
         sprintf(dest,"shladdp4 r%d = r%d, %d, r%d", rd,rs1,opcode&3,rs2 );
         return;
      case 9*4+1:
         imm.q = (-(uint64)BITS(instr[0],36,36)) << 7 | rs1;
         sprintf(dest,"sub r%d = %d, r%d", rd,LO(imm),rs2 );
         return;
      case 0xb*4+0:
         imm.q = (-(uint64)BITS(instr[0],36,36)) << 7 | rs1;
         sprintf(dest,"and r%d = %d, r%d", rd,LO(imm),rs2 );
         return;
      case 0xb*4+1:
         imm.q = (-(uint64)BITS(instr[0],36,36)) << 7 | rs1;
         sprintf(dest,"andcm r%d = %d, r%d", rd,LO(imm),rs2 );
         return;
      case 0xb*4+2:
         imm.q = (-(uint64)BITS(instr[0],36,36)) << 7 | rs1;
         return;
      case 0xb*4+3:
         imm.q = (-(uint64)BITS(instr[0],36,36)) << 7 | rs1;
         sprintf(dest,"xor r%d = %d, r%d", rd,LO(imm),rs2 );
         return;
      case 1*128+0*64+0*4+0:
         sprintf(dest,"padd1 r%d = r%d, r%d", rd,rs1,rs2 );
         return;
      case 1*128+0*64+0*4+1:
         sprintf(dest,"padd1.sss r%d = r%d, r%d", rd,rs1,rs2 );
         return;
      case 1*128+0*64+0*4+2:
         sprintf(dest,"padd1.uuu r%d = r%d, r%d", rd,rs1,rs2 );
         return;
      case 1*128+0*64+0*4+3:
         sprintf(dest,"padd1.uus r%d = r%d, r%d", rd,rs1,rs2 );
         return;
      case 1*128+0*64+1*4+0:
         sprintf(dest,"psub1 r%d = r%d, r%d", rd,rs1,rs2 );
         return;
      case 1*128+0*64+1*4+1:
         sprintf(dest,"psub1.sss r%d = r%d, r%d", rd,rs1,rs2 );
         return;
      case 1*128+0*64+1*4+2:
         sprintf(dest,"psub1.uuu r%d = r%d, r%d", rd,rs1,rs2 );
         return;
      case 1*128+0*64+1*4+3:
         sprintf(dest,"psub1.uus r%d = r%d, r%d", rd,rs1,rs2 );
         return;
      case 1*128+0*64+2*4+2:
         sprintf(dest,"pavg1 r%d = r%d, r%d", rd,rs1,rs2 );
         return;
      case 1*128+0*64+2*4+3:
         sprintf(dest,"pavg1.raz r%d = r%d, r%d", rd,rs1,rs2 );
         return;
      case 1*128+0*64+3*4+2:
         sprintf(dest,"pavgsub1 r%d = r%d, r%d", rd,rs1,rs2 );
         return;
      case 1*128+0*64+9*4+0:
         sprintf(dest,"pcmp1.eq r%d = r%d, r%d", rd,rs1,rs2 );
         return;
      case 1*128+0*64+9*4+1:
         sprintf(dest,"pcmp1.gt r%d = r%d, r%d", rd,rs1,rs2 );
         return;
      case 1*128+1*64+0*4+0:
         sprintf(dest,"padd2 r%d = r%d, r%d", rd,rs1,rs2 );
         return;
      case 1*128+1*64+0*4+1:
         sprintf(dest,"padd2.sss r%d = r%d, r%d", rd,rs1,rs2 );
         return;
      case 1*128+1*64+0*4+2:
         sprintf(dest,"padd2.uuu r%d = r%d, r%d", rd,rs1,rs2 );
         return;
      case 1*128+1*64+0*4+3:
         sprintf(dest,"padd2.uus r%d = r%d, r%d", rd,rs1,rs2 );
         return;
      case 1*128+1*64+1*4+0:
         sprintf(dest,"psub2 r%d = r%d, r%d", rd,rs1,rs2 );
         return;
      case 1*128+1*64+1*4+1:
         sprintf(dest,"psub2.sss r%d = r%d, r%d", rd,rs1,rs2 );
         return;
      case 1*128+1*64+1*4+2:
         sprintf(dest,"psub2.uuu r%d = r%d, r%d", rd,rs1,rs2 );
         return;
      case 1*128+1*64+1*4+3:
         sprintf(dest,"psub2.uus r%d = r%d, r%d", rd,rs1,rs2 );
         return;
      case 1*128+1*64+9*4+0:
         sprintf(dest,"pcmp2.eq r%d = r%d, r%d", rd,rs1,rs2 );
         return;
      case 1*128+1*64+9*4+1:
         sprintf(dest,"pcmp2.gt r%d = r%d, r%d", rd,rs1,rs2 );
         return;
      case 1*128+1*64+2*4+2:
         sprintf(dest,"pavg2 r%d = r%d, r%d", rd,rs1,rs2 );
         return;
      case 1*128+1*64+2*4+3:
         sprintf(dest,"pavg2.raz r%d = r%d, r%d", rd,rs1,rs2 );
         return;
      case 1*128+1*64+3*4+2:
         sprintf(dest,"pavgsub2 r%d = r%d, r%d", rd,rs1,rs2 );
         return;
      case 1*128+1*64+4*4+0:
      case 1*128+1*64+4*4+1:
      case 1*128+1*64+4*4+2:
      case 1*128+1*64+4*4+3:
         sprintf(dest,"pshladd2 r%d = r%d, %d, r%d", rd,rs1,opcode&3,rs2 );
         return;
      case 1*128+1*64+6*4+0:
      case 1*128+1*64+6*4+1:
      case 1*128+1*64+6*4+2:
      case 1*128+1*64+6*4+3:
         sprintf(dest,"pshradd2 r%d = r%d, %d, r%d", rd,rs1,opcode&3,rs2 );
         return;
      case 1*128+1*8+9*4+0:
         sprintf(dest,"pcmp2.eq r%d = r%d, r%d", rd,rs1,rs2 );
         return;
      case 1*128+1*8+9*4+1:
         sprintf(dest,"pcmp2.gt r%d = r%d, r%d", rd,rs1,rs2 );
         return;
      case 2*128+0*64+0*4+0:
         sprintf(dest,"padd4 r%d = r%d, r%d", rd,rs1,rs2 );
         return;
      case 2*128+0*64+1*4+0:
         sprintf(dest,"psub4 r%d = r%d, r%d", rd,rs1,rs2 );
         return;
      case 2*128+0*64+9*4+0:
         sprintf(dest,"pcmp4.eq r%d = r%d, r%d", rd,rs1,rs2 );
         return;
      case 2*128+0*64+9*4+1:
         sprintf(dest,"pcmp4.gt r%d = r%d, r%d", rd,rs1,rs2 );
         return;
   }


   sprintf(dest, "illegal instruction");
}               

/*----------------------------------------------------------------------*/
void i_addi22_decode(qword *instr, char *dest) {

   uint32 rs2, rd;
   uint32 opcode;
   qword imm;

   rs2 = BITS(instr[0],20,21);
   rd = BITS(instr[0],6,12);

   imm.q = (-(uint64)BITS(instr[0],36,36) << 21 |
                 BITS(instr[0],13,19) |
                 BITS(instr[0],22,35) << 7 ); 
   sprintf(dest,"addl r%d = %d, r%d", rd,LO(imm),rs2 );
}

/*----------------------------------------------------------------------*/
void i_misc_decode(qword *instr, char *dest) {

   uint32 opcode, rs2, rs1, rd;
   uint32 i;
   qword imm, mask;

   opcode = BITS(instr[0],27,35);
   rs1 = BITS(instr[0],13,19);
   rd = BITS(instr[0],6,12);

   switch(opcode) {

      case 0*64+0:
         imm.q = BITS(instr[0],36,36) << 20 |
                 BITS(instr[0],6,25);
         sprintf(dest,"break.i %d", LO(imm));
         return;
      case 0*64+1:
         imm.q = BITS(instr[0],36,36) << 20 |
                 BITS(instr[0],6,25);
         sprintf(dest,"nop.i %d", LO(imm));
         return;
      case 0*64+0x31:
         rs1 &= 7;
         sprintf(dest,"mov r%d = b%d", rd,rs1);
         return;
      case 0*64+0x30:   
         sprintf(dest,"mov r%d = ip", rd);
         return;
      case 0*64+0x33:   
         sprintf(dest,"mov r%d = pr", rd);
         return;
      case 0*64+0x2a:
         rs2 = BITS(instr[0],20,26);
         sprintf(dest,"mov.i ar%d = r%d", rs2, rs1);
         return;
      case 0*64+0x0a:
         rs2 = BITS(instr[0],20,26);
         imm.q = -((int64)BITS(instr[0],36,36)) << 7 | rs1;
         sprintf(dest,"mov.i ar%d = %d", rs2, LO(imm));
         return;
      case 0*64+0x32:
         rs2 = BITS(instr[0],20,26);
         sprintf(dest,"mov.i r%d = ar%d", rd, rs2);
         return;
      case 0*64+0x10:
         rs2 = BITS(instr[0],20,26);
         sprintf(dest,"zxt1 r%d = r%d", rd, rs2);
         return;
      case 0*64+0x11:
         rs2 = BITS(instr[0],20,26);
         sprintf(dest,"zxt2 r%d = r%d", rd, rs2);
         return;
      case 0*64+0x12:
         rs2 = BITS(instr[0],20,26);
         sprintf(dest,"zxt4 r%d = r%d", rd, rs2);
         return;
      case 0*64+0x14:
         rs2 = BITS(instr[0],20,26);
         sprintf(dest,"sxt1 r%d = r%d", rd, rs2);
         return;
      case 0*64+0x15:
         rs2 = BITS(instr[0],20,26);
         sprintf(dest,"sxt2 r%d = r%d", rd, rs2);
         return;
      case 0*64+0x16:
         rs2 = BITS(instr[0],20,26);
         sprintf(dest,"sxt4 r%d = r%d", rd, rs2);
         return;
      case 0*64+0x18:
         rs2 = BITS(instr[0],20,26);
         sprintf(dest,"czx1.l r%d = r%d", rd, rs2);
         return;
      case 0*64+0x19:
         rs2 = BITS(instr[0],20,26);
         sprintf(dest,"czx2.l r%d = r%d", rd, rs2);
         return;
      case 0*64+0x1c:
         rs2 = BITS(instr[0],20,26);
         sprintf(dest,"czx1.r r%d = r%d", rd, rs2);
         return;
      case 0*64+0x1d:
         rs2 = BITS(instr[0],20,26);
         sprintf(dest,"czx2.r r%d = r%d", rd, rs2);
         return;
      case 1*64+0:
         imm.q = -((int64)BITS(instr[0],36,36)) << 24 |
              BITS(instr[0],20,32) << 11 | rd << 4;
         sprintf(dest,"chk.s.i r%d, %d", rs1,LO(imm));
         return;
      case 7*64+0:
         rd &= 7;
         if( !BITS(instr[0],22,22) ) {
            sprintf(dest,"mov b%d = r%d", rd,rs1);
            return;
         }
         else {
            sprintf(dest,"mov.ret b%d = r%d", rd,rs1);
            return;
         }

      case 3*64+0:
         imm.q = -((int64)BITS(instr[0],36,36)) << 16 |
                    BITS(instr[0],24,31) << 8 | rd << 1;
         sprintf(dest,"mov pr = r%d,%d", rs1, LO(imm));
         return;
      case 2*64+0:
         imm.q = -((int64)BITS(instr[0],36,36)) << 43 |
                    (uint64)BITS(instr[0],6,32) << 16;
         sprintf(dest,"mov pr.rot = 0x%08x%08x", HI(imm),LO(imm));
         return;
   }

   sprintf(dest, "illegal instruction");
}

/*----------------------------------------------------------------------*/
void i_mmshift_decode(qword *instr, char *dest) {

   uint32 rs1, rs2, rd;
   uint32 opcode;
   uint32 shifts[]={0,7,15,16};
   uint32 i, t0;

   rs2 = BITS(instr[0],20,26); 
   rs1 = BITS(instr[0],13,19);
   rd = BITS(instr[0],6,12);

   opcode = BITS(instr[0],28,36);

   switch(opcode) {

      case 0*256+1*32+0*16+0*64+3+0*4: 
      case 0*256+1*32+0*16+0*64+3+1*4: 
      case 0*256+1*32+0*16+0*64+3+2*4: 
      case 0*256+1*32+0*16+0*64+3+3*4:
         sprintf(dest,"pmpyshr2 r%d = r%d,r%d,%d", rd,rs1,rs2,
                        shifts[(opcode>>2)&3] );
         return;
      case 0*256+1*32+0*16+0*64+1+0*4:
      case 0*256+1*32+0*16+0*64+1+1*4: 
      case 0*256+1*32+0*16+0*64+1+2*4: 
      case 0*256+1*32+0*16+0*64+1+3*4:
         sprintf(dest,"pmpyshr2.u r%d = r%d,r%d,%d", rd,rs1,rs2,
                        shifts[(opcode>>2)&3] );
         return;
      case 0*256+1*32+0*16+2*64+1+3*4:
         sprintf(dest,"pmpy2.r r%d = r%d,r%d", rd,rs1,rs2 );
         return;
      case 0*256+1*32+0*16+2*64+3+3*4:
         sprintf(dest,"pmpy2.l r%d = r%d,r%d", rd,rs1,rs2 );
         return;
      case 0*256+0*32+0*16+2*64+0+2*4:
         sprintf(dest,"mix1.r r%d = r%d,r%d", rd,rs1,rs2 );
         return;
      case 0*256+1*32+0*16+2*64+0+2*4:
         sprintf(dest,"mix2.r r%d = r%d,r%d", rd,rs1,rs2 );
         return;
      case 1*256+0*32+0*16+2*64+0+2*4:
         sprintf(dest,"mix4.r r%d = r%d,r%d", rd,rs1,rs2 );
         return;
      case 0*256+0*32+0*16+2*64+2+2*4:
         sprintf(dest,"mix1.l r%d = r%d,r%d", rd,rs1,rs2 );
         return;
      case 0*256+1*32+0*16+2*64+2+2*4:
         sprintf(dest,"mix2.l r%d = r%d,r%d", rd,rs1,rs2 );
         return;
      case 1*256+0*32+0*16+2*64+2+2*4:
         sprintf(dest,"mix4.l r%d = r%d,r%d", rd,rs1,rs2 );
         return;
      case 0*256+1*32+0*16+2*64+0+0*4:
         sprintf(dest,"pack2.uss r%d = r%d,r%d", rd,rs1,rs2 );
         return;
      case 0*256+1*32+0*16+2*64+2+0*4:
         sprintf(dest,"pack2.sss r%d = r%d,r%d", rd,rs1,rs2 );
         return;
      case 1*256+0*32+0*16+2*64+2+0*4:
         sprintf(dest,"pack4.sss r%d = r%d,r%d", rd,rs1,rs2 );
         return;
      case 0*256+0*32+0*16+2*64+0+1*4:
         sprintf(dest,"unpack1.h r%d = r%d,r%d", rd,rs1,rs2 );
         return;
      case 0*256+1*32+0*16+2*64+0+1*4:
         sprintf(dest,"unpack2.h r%d = r%d,r%d", rd,rs1,rs2 );
         return;
      case 1*256+0*32+0*16+2*64+0+1*4:
         sprintf(dest,"unpack4.h r%d = r%d,r%d", rd,rs1,rs2 );
         return;
      case 0*256+0*32+0*16+2*64+2+1*4:
         sprintf(dest,"unpack1.l r%d = r%d,r%d", rd,rs1,rs2 );
         return;
      case 0*256+1*32+0*16+2*64+2+1*4:
         sprintf(dest,"unpack2.l r%d = r%d,r%d", rd,rs1,rs2 );
         return;
      case 1*256+0*32+0*16+2*64+2+1*4:
         sprintf(dest,"unpack4.l r%d = r%d,r%d", rd,rs1,rs2 );
         return;
      case 0*256+0*32+0*16+2*64+1+0*4:
         sprintf(dest,"pmin1.u r%d = r%d,r%d", rd,rs1,rs2 );
         return;
      case 0*256+0*32+0*16+2*64+1+1*4:
         sprintf(dest,"pmax1.u r%d = r%d,r%d", rd,rs1,rs2 );
         return;
      case 0*256+1*32+0*16+2*64+3+0*4:
         sprintf(dest,"pmin2 r%d = r%d,r%d", rd,rs1,rs2 );
         return;
      case 0*256+1*32+0*16+2*64+3+1*4:
         sprintf(dest,"pmax2 r%d = r%d,r%d", rd,rs1,rs2 );
         return;
      case 0*256+0*32+0*16+2*64+3+2*4:
         sprintf(dest,"psad1 r%d = r%d,r%d", rd,rs1,rs2 );
         return;
      case 0*256+0*32+0*16+3*64+2+2*4:
         switch(rs2&15) {
            case 0:
               sprintf(dest,"mux1 r%d = r%d,@brcast", rd,rs1);
               return;
            case 8:
               sprintf(dest,"mux1 r%d = r%d,@mix", rd,rs1);
               return;
            case 9:
               sprintf(dest,"mux1 r%d = r%d,@shuf", rd,rs1);
               return;
            case 10:
               sprintf(dest,"mux1 r%d = r%d,@alt", rd,rs1);
               return;
            case 11:
               sprintf(dest,"mux1 r%d = r%d,@rev", rd,rs1);
               return;
            default:
               sprintf(dest, "illegal instruction");
         }

      case 0*256+1*32+0*16+3*64+2+2*4:
         rs2 = BITS(instr[0],20,27);
         sprintf(dest,"mux2 r%d = r%d,0x%02x", rd,rs1,rs2);
         return;
      case 0*256+1*32+0*16+0*64+2+0*4:
         sprintf(dest,"pshr2 r%d = r%d,r%d", rd,rs2,rs1 );
         return;
      case 1*256+0*32+0*16+0*64+2+0*4:
         sprintf(dest,"pshr4 r%d = r%d,r%d", rd,rs2,rs1 );
         return;
      case 1*256+1*32+0*16+0*64+2+0*4:
         sprintf(dest,"shr r%d = r%d,r%d", rd,rs2,rs1 );
         return;
      case 0*256+1*32+0*16+0*64+0+0*4:
         sprintf(dest,"pshr2.u r%d = r%d,r%d", rd,rs2,rs1 );
         return;
      case 1*256+0*32+0*16+0*64+0+0*4:
         sprintf(dest,"pshr4.u r%d = r%d,r%d", rd,rs2,rs1 );
         return;
      case 1*256+1*32+0*16+0*64+0+0*4:
         sprintf(dest,"shr.u r%d = r%d,r%d", rd,rs2,rs1 );
         return;
      case 0*256+1*32+0*16+1*64+3+0*4:
         sprintf(dest,"pshr2 r%d = r%d,%d", rd,rs2,rs1>>1 );
         return;
      case 1*256+0*32+0*16+1*64+3+0*4:
         sprintf(dest,"pshr4 r%d = r%d,%d", rd,rs2,rs1>>1 );
         return;
      case 0*256+1*32+0*16+1*64+1+0*4:
         sprintf(dest,"pshr2.u r%d = r%d,%d", rd,rs2,rs1>>1 );
         return;
      case 1*256+0*32+0*16+1*64+1+0*4:
         sprintf(dest,"pshr4.u r%d = r%d,%d", rd,rs2,rs1>>1 );
         return;
      case 0*256+1*32+0*16+0*64+0+1*4:
         sprintf(dest,"pshl2 r%d = r%d,r%d", rd,rs1,rs2 );
         return;
      case 1*256+0*32+0*16+0*64+0+1*4:
         sprintf(dest,"pshl4 r%d = r%d,r%d", rd,rs1,rs2 );
         return;
      case 1*256+1*32+0*16+0*64+0+1*4:
         sprintf(dest,"shl r%d = r%d,r%d", rd,rs1,rs2 );
         return;
      case 0*256+1*32+0*16+1*64+1+2*4:
         return;
   }

   sprintf(dest, "illegal instruction");
}

/*----------------------------------------------------------------------*/
void i_shift_decode(qword *instr, char *dest) {

   uint32 rs1, rs2, rd;
   uint32 opcode, count, pos;
   qword temp;

   rs2 = BITS(instr[0],20,26);  
   rs1 = BITS(instr[0],13,19);
   rd = BITS(instr[0],6,12);

   opcode = BITS(instr[0],33,35);
   count = BITS(instr[0],27,32);
   pos = ~BITS(instr[0],20,25);
   switch(opcode) {

      case 3*2+0:
         sprintf(dest,"shrp r%d = r%d,r%d,%d", rd,rs1,rs2,count );
         return;
      case 1*2+0:
         if( rs1 & 1 ) {
            rs1 >>= 1; count++;
            sprintf(dest,"extr.u r%d = r%d,%d,%d", rd,rs2,rs1,count );
            return;
         }
         else {
            rs1 >>= 1; count++;
            sprintf(dest,"extr r%d = r%d,%d,%d", rd,rs2,rs1,count );
            return;
         }

      case 1*2+1:
         if( rs2 >> 6 ) {
            temp.q = -((int64)BITS(instr[0],36,36)) << 7 | rs1;
            rs2 &= 63; count++;
            sprintf(dest,"dep.z r%d = %d,%d,%d", rd,LO(temp),rs2,count );
            return;
         }
         else {
            pos &= 63; count++;
            sprintf(dest,"dep.z r%d = r%d,%d,%d", rd,rs1,pos,count );
            return;
         }

      case 3*2+1:
         temp.q = -(int64)BITS(instr[0],36,36);
         rs1 >>= 1; count++;
         sprintf(dest,"dep r%d = %d,r%d,%d,%d", rd,LO(temp),rs2,rs1,count );
         return;
   }

   sprintf(dest, "illegal instruction");
}

/*----------------------------------------------------------------------*/
void i_deposit_decode(qword *instr, char *dest) {

   uint32 rs1, rs2, rd;
   uint32 len, pos;
   qword temp;

   pos = BITS(instr[0],31,36);  
   len = BITS(instr[0],27,30)+1;  
   rs2 = BITS(instr[0],20,26);  
   rs1 = BITS(instr[0],13,19);
   rd = BITS(instr[0],6,12);

   sprintf(dest,"dep r%d = r%d,r%d,%d,%d", rd,rs1,rs2,pos,len );
}

/*--------------------------------------------------------------------*/
void i_test_decode(qword *instr, char *dest) { 

   uint32 p1, p2, r1, pred;
   uint32 opcode, pos;
   qword imm;

   pred = LO(instr[0]) & 63;
   p1 = BITS(instr[0],6,11);
   p2 = BITS(instr[0],27,32);
   r1 = BITS(instr[0],20,26);
   pos = BITS(instr[0],14,19);

   if( p1==p2 ) {
      sprintf(dest, "illegal instruction");
      return;
   }

   opcode = BITS(instr[0],33,36) << 2 |
            BITS(instr[0],12,13);

   switch(opcode) {

      case 0*8+0*4+0*32+0*2+0:
         sprintf(dest,"tbit.z p%d,p%d = r%d,%d",p1,p2,r1,pos);
         return;
      case 0*8+0*4+0*32+0*2+1:
         sprintf(dest,"tbit.z.unc p%d,p%d = r%d,%d",p1,p2,r1,pos);
         return;
      case 0*8+0*4+1*32+0*2+0:
         sprintf(dest,"tbit.z.and p%d,p%d = r%d,%d",p1,p2,r1,pos);
         return;
      case 0*8+0*4+1*32+0*2+1:
         sprintf(dest,"tbit.nz.and p%d,p%d = r%d,%d",p1,p2,r1,pos);
         return;
      case 0*8+1*4+0*32+0*2+0:
         sprintf(dest,"tbit.z.or p%d,p%d = r%d,%d",p1,p2,r1,pos);
         return;
      case 0*8+1*4+0*32+0*2+1:
         sprintf(dest,"tbit.nz.or p%d,p%d = r%d,%d",p1,p2,r1,pos);
         return;
      case 0*8+1*4+1*32+0*2+0:
         sprintf(dest,"tbit.z.or.andcm p%d,p%d = r%d,%d",p1,p2,r1,pos);
         return;
      case 0*8+1*4+1*32+0*2+1:
         sprintf(dest,"tbit.nz.or.andcm p%d,p%d = r%d,%d",p1,p2,r1,pos);
         return;
      case 0*8+0*4+0*32+1*2+0:
         sprintf(dest,"tnat.z p%d,p%d = r%d",p1,p2,r1);
         return;
      case 0*8+0*4+0*32+1*2+1:
         sprintf(dest,"tnat.z.unc p%d,p%d = r%d",p1,p2,r1);
         return;
      case 0*8+0*4+1*32+1*2+0:
         sprintf(dest,"tnat.z.and p%d,p%d = r%d",p1,p2,r1);
         return;
      case 0*8+0*4+1*32+1*2+1:
         sprintf(dest,"tnat.nz.and p%d,p%d = r%d",p1,p2,r1);
         return;
      case 0*8+1*4+0*32+1*2+0:
         sprintf(dest,"tnat.z.or p%d,p%d = r%d",p1,p2,r1);
         return;
      case 0*8+1*4+0*32+1*2+1:
         sprintf(dest,"tnat.nz.or p%d,p%d = r%d",p1,p2,r1);
         return;
      case 0*8+1*4+1*32+1*2+0:
         sprintf(dest,"tnat.z.or.andcm p%d,p%d = r%d",p1,p2,r1);
         return;
      case 0*8+1*4+1*32+1*2+1:
         sprintf(dest,"tnat.nz.or.andcm p%d,p%d = r%d",p1,p2,r1);
         return;
   }

   sprintf(dest, "illegal instruction");
}


char prefetch[2][6] = {".few", ".many"};
char bwh_imm[4][6] = {".sptk", ".spnt", ".dptk", ".dpnt"};
char bwh_reg[8][6] = {"", ".sptk", "", ".spnt",
                    "", ".dptk", "", ".dpnt"};
char dealloc[2][6] = {"", ".clr"};

/*--------------------------------------------------------------------*/
void b_nop_decode(qword *instr, char *dest) {

   uint32 opcode;
   qword imm;

   imm.q = BITS(instr[0],36,36) << 20 |
           BITS(instr[0],6,25);

   sprintf(dest,"nop.b %d", LO(imm));
}

/*--------------------------------------------------------------------*/
void b_branchr_decode(qword *instr, char *dest) {

   uint32 opcode, b1, pred;
   qword imm;
               
   opcode = BITS(instr[0],27,32) << 3 |
            BITS(instr[0],6,8);
   b1 = BITS(instr[0],13,15);
   pred = LO(instr[0]) & 63;

   switch(opcode) {
      case 0 ... 7:
          imm.q = BITS(instr[0],36,36) << 20 |
              BITS(instr[0],6,25);
          sprintf(dest,"break.b %d", LO(imm));
          return;
            
     case 0x20*8+0:
         sprintf(dest,"br.cond%s%s%s b%d",bwh_imm[BITS(instr[0],33,34)],
                                       prefetch[BITS(instr[0],12,12)],
                                       dealloc[BITS(instr[0],35,35)], b1 );
         return;
      case 0x20*8+1:
         sprintf(dest, "unimplemented");
         return;
      case 0x21*8+4:
         sprintf(dest,"br.ret%s%s%s b%d",bwh_imm[BITS(instr[0],33,34)],
                                       prefetch[BITS(instr[0],12,12)],
                                       dealloc[BITS(instr[0],35,35)], b1 );
         return;
   }

   sprintf(dest, "illegal instruction");
}

/*--------------------------------------------------------------------*/
void b_branchi_decode(qword *instr, char *dest) {

   uint32 opcode, pred;
   qword imm;
                
   opcode = BITS(instr[0],6,8);
   pred = LO(instr[0]) & 63;
   imm.q = -((int64)BITS(instr[0],36,36)) << 24 |
              BITS(instr[0],13,32) << 4;

   switch(opcode) {

      case 0:
         sprintf(dest,"br.cond%s%s%s %d",bwh_imm[BITS(instr[0],33,34)],
                                       prefetch[BITS(instr[0],12,12)],
                                       dealloc[BITS(instr[0],35,35)],
                                       LO(imm));
         return;
      case 2:
         sprintf(dest,"br.wexit%s%s%s %d",bwh_imm[BITS(instr[0],33,34)],
                                       prefetch[BITS(instr[0],12,12)],
                                       dealloc[BITS(instr[0],35,35)],
                                       LO(imm));
         return;
      case 3:
         sprintf(dest,"br.wtop%s%s%s %d",bwh_imm[BITS(instr[0],33,34)],
                                       prefetch[BITS(instr[0],12,12)],
                                       dealloc[BITS(instr[0],35,35)],
                                       LO(imm));
         return;
      case 5:
         sprintf(dest,"br.cloop%s%s%s %d",bwh_imm[BITS(instr[0],33,34)],
                                       prefetch[BITS(instr[0],12,12)],
                                       dealloc[BITS(instr[0],35,35)],
                                       LO(imm));
         return;
      case 6:
         sprintf(dest,"br.cexit%s%s%s %d",bwh_imm[BITS(instr[0],33,34)],
                                       prefetch[BITS(instr[0],12,12)],
                                       dealloc[BITS(instr[0],35,35)],
                                       LO(imm));
         return;
      case 7:
         sprintf(dest,"br.ctop%s%s%s %d",bwh_imm[BITS(instr[0],33,34)],
                                       prefetch[BITS(instr[0],12,12)],
                                       dealloc[BITS(instr[0],35,35)],
                                       LO(imm));
         return;
   }

   sprintf(dest, "illegal instruction");
}

/*--------------------------------------------------------------------*/
void b_calli_decode(qword *instr, char *dest) { 

   uint32 b1, pred;
   qword imm;
                
   pred = LO(instr[0]) & 63;
   b1 = BITS(instr[0],6,8);
   imm.q = -((int64)BITS(instr[0],36,36)) << 24 |
              BITS(instr[0],13,32) << 4;

   sprintf(dest,"br.call%s%s%s b%d = %d",bwh_imm[BITS(instr[0],33,34)],
                                      prefetch[BITS(instr[0],12,12)],
                                      dealloc[BITS(instr[0],35,35)],
                                      b1, LO(imm) );
}

/*--------------------------------------------------------------------*/
void b_callr_decode(qword *instr, char *dest) { 

   uint32 b1, b2, pred;
   qword imm;
                
   pred = LO(instr[0]) & 63;
   b1 = BITS(instr[0],6,8);
   b2 = BITS(instr[0],13,15);

   sprintf(dest,"br.call%s%s%s b%d = b%d", bwh_reg[BITS(instr[0],32,34)],
                                      prefetch[BITS(instr[0],12,12)],
                                      dealloc[BITS(instr[0],35,35)],
                                      b1, b2 );
}

/*----------------------------------------------------------------------*/

char ldhint[4][8]={"",".nt1","",".nta"};
char sthint[4][8]={"","","",".nta"};

/*----------------------------------------------------------------------*/
void m_ldir_decode(qword *instr, char *dest) {

   uint32 rd, rs1, rs2, opcode, status;

   rd = BITS(instr[0],6,12);
   rs1 = BITS(instr[0],13,19);
   rs2 = BITS(instr[0],20,26);
   opcode = BITS(instr[0],30,36)<<1 + BITS(instr[0],27,27);

   switch(opcode) {

      case 0*2:
         sprintf(dest,"ld1%s r%d = [r%d]", ldhint[BITS(instr[0],28,29)],
                                      rd,rs2);
         return;
      case 1*2:
         sprintf(dest,"ld2%s r%d = [r%d]", ldhint[BITS(instr[0],28,29)],
                                      rd,rs2);
         return;
      case 2*2:
         sprintf(dest,"ld4%s r%d = [r%d]", ldhint[BITS(instr[0],28,29)],
                                      rd,rs2);
         return;
      case 3*2:
         sprintf(dest,"ld8%s r%d = [r%d]", ldhint[BITS(instr[0],28,29)],
                                      rd,rs2);
         return;
      case 4*2:
         sprintf(dest,"ld1.s%s r%d = [r%d]", ldhint[BITS(instr[0],28,29)],
                                      rd,rs2);
         return;
      case 5*2:
         sprintf(dest,"ld2.s%s r%d = [r%d]", ldhint[BITS(instr[0],28,29)],
                                      rd,rs2);
         return;
      case 6*2:
         sprintf(dest,"ld4.s%s r%d = [r%d]", ldhint[BITS(instr[0],28,29)],
                                      rd,rs2);
         return;
      case 7*2:
         sprintf(dest,"ld8.s%s r%d = [r%d]", ldhint[BITS(instr[0],28,29)],
                                      rd,rs2);
         return;
      case 1*128+0*2:
         sprintf(dest,"ld1%s r%d = [r%d], r%d", ldhint[BITS(instr[0],28,29)],
                                      rd,rs2,rs1);
         return;
      case 1*128+1*2:
         sprintf(dest,"ld2%s r%d = [r%d], r%d", ldhint[BITS(instr[0],28,29)],
                                      rd,rs2,rs1);
         return;
      case 1*128+2*2:
         sprintf(dest,"ld4%s r%d = [r%d], r%d", ldhint[BITS(instr[0],28,29)],
                                      rd,rs2,rs1);
         return;
      case 1*128+3*2:
         sprintf(dest,"ld8%s r%d = [r%d], r%d", ldhint[BITS(instr[0],28,29)],
                                      rd,rs2,rs1);
         return;
      case 1*128+4*2:
         sprintf(dest,"ld1.s%s r%d = [r%d], r%d", ldhint[BITS(instr[0],28,29)],
                                      rd,rs2,rs1);
         return;
      case 1*128+5*2:
         sprintf(dest,"ld2.s%s r%d = [r%d], r%d", ldhint[BITS(instr[0],28,29)],
                                      rd,rs2,rs1);
         return;
      case 1*128+6*2:
         sprintf(dest,"ld4.s%s r%d = [r%d], r%d", ldhint[BITS(instr[0],28,29)],
                                      rd,rs2,rs1);
         return;
      case 1*128+7*2:
         sprintf(dest,"ld8.s%s r%d = [r%d], r%d", ldhint[BITS(instr[0],28,29)],
                                      rd,rs2,rs1);
         return;
      case 0x30*2:
         sprintf(dest,"st1%s [r%d] = r%d", sthint[BITS(instr[0],28,29)],
                                      rs2,rs1);
         return;
      case 0x31*2:
         sprintf(dest,"st2%s [r%d] = r%d", sthint[BITS(instr[0],28,29)],
                                      rs2,rs1);
         return;
      case 0x32*2:
         sprintf(dest,"st4%s [r%d] = r%d", sthint[BITS(instr[0],28,29)],
                                      rs2,rs1);
         return;
      case 0x33*2:
         sprintf(dest,"st8%s [r%d] = r%d", sthint[BITS(instr[0],28,29)],
                                      rs2,rs1);
         return;
      default:
         sprintf(dest,"unimplemented");
   }
}

/*----------------------------------------------------------------------*/
void m_ldii_decode(qword *instr, char *dest){

   uint32 r1, r2, r3, opcode, status;
   int64 imm;

   opcode = BITS(instr[0],30,35)<<1;
     r3 = BITS(instr[0],20,26);
   if(opcode > 11*2) {                  /* st. instructions */
     imm = -((int64)BITS(instr[0],36,36)) << 8 | BITS(instr[0],6,12) | BITS(instr[0],27,27)<<7;
     r2 = BITS(instr[0],13,19);
     }
   else {                               /* ld. instructions */
     imm = -((int64)BITS(instr[0],36,36)) << 8 | BITS(instr[0],13,19) | BITS(instr[0],27,27)<<7;
     r1 = BITS(instr[0],6,12);
     }

   switch(opcode) {

      case 0*2:
         sprintf(dest,"ld1%s r%d = [r%d],%d", ldhint[BITS(instr[0],28,29)],
                                      r1,r3,imm);
         return;
      case 1*2:
         sprintf(dest,"ld2%s r%d = [r%d],%d", ldhint[BITS(instr[0],28,29)],
                                      r1,r3,imm);
         return;
      case 2*2:
         sprintf(dest,"ld4%s r%d = [r%d],%d", ldhint[BITS(instr[0],28,29)],
                                      r1,r3,imm);
         return;
      case 3*2:
         sprintf(dest,"ld8%s r%d = [r%d],%d", ldhint[BITS(instr[0],28,29)],
                                      r1,r3,imm);
         return;
      case 4*2:
         sprintf(dest,"ld1.s%s r%d = [r%d],%d", ldhint[BITS(instr[0],28,29)],
                                      r1,r3,imm);
         return;
      case 5*2:
         sprintf(dest,"ld2.s%s r%d = [r%d],%d", ldhint[BITS(instr[0],28,29)],
                                      r1,r3,imm);
         return;
      case 6*2:
         sprintf(dest,"ld4.s%s r%d = [r%d],%d", ldhint[BITS(instr[0],28,29)],
                                      r1,r3,imm);
         return;
      case 7*2:
         sprintf(dest,"ld8.s%s r%d = [r%d],%d", ldhint[BITS(instr[0],28,29)],
                                      r1,r3,imm);
         return;
      case 0x30*2:
         sprintf(dest,"st1%s [r%d] = r%d,%d", sthint[BITS(instr[0],28,29)],
                                      r3,r2,imm);
         return;
      case 0x31*2:
         sprintf(dest,"st2%s [r%d] = r%d,%d", sthint[BITS(instr[0],28,29)],
                                      r3,r2,imm);
         return;
      case 0x32*2:
         sprintf(dest,"st4%s [r%d] = r%d,%d", sthint[BITS(instr[0],28,29)],
                                      r3,r2,imm);
         return;
      case 0x33*2:
         sprintf(dest,"st8%s [r%d] = r%d,%d", sthint[BITS(instr[0],28,29)],
                                      r3,r2,imm);
         return;
      default:
         sprintf(dest,"unimplemented");
   }
}


/*----------------------------------------------------------------------*/
void m_mgmt_decode(qword *instr, char *dest) {

   uint32 sof, soi, sol, sor, rd, rs1, opcode;
   uint64 imm;

   opcode = BITS(instr[0],33,40) &~8;

   switch(opcode) {

      case 1*16+1:
         rs1 = BITS(instr[0],13,19);
         imm = -((int64)BITS(instr[0],36,36)) << 24 |
              BITS(instr[0],20,32) << 11 | BITS(instr[0],6,12) << 4;
         sprintf(dest,"chk.s.m r%d, %d\n", rs1,imm);
         return;
         
      case 1*16+6:
         rd = BITS(instr[0],6,12);
         soi = cfm.q & 0x7f;
         sof = BITS(instr[0],13,19);
         sol = BITS(instr[0],20,26);
         sor = BITS(instr[0],27,30) << 3;
         sprintf(dest,"alloc r%d = ar.pfs, {%d},%d,%d", rd,
                                     sol, sof-sol, sor );
         return;
      case 0:
         imm = -(int64)BITS(instr[0],36,36)<<20 |
                       BITS(instr[0],6,25);
         if( BITS(instr[0],27,40)==0 ) {
            sprintf(dest,"break.m %d", (uint32)imm);
            return;
         }
         if( BITS(instr[0],27,40)==1 ) {
            sprintf(dest,"nop.m %d", (uint32)imm);
            return;
         }

      default:
         sprintf(dest,"unimplemented");
   }            
}


/*----------------------------------------------------------------------*/
void f_misc_decode(qword *instr, char *dest) {

   qword imm;

   if (BITS(instr[0], 27,35)==0) {   
         imm.q = BITS(instr[0],36,36) << 20 |
                 BITS(instr[0],6,25);
         sprintf(dest,"break.f %d", LO(imm));
         return;
   }
   else if (BITS(instr[0], 27,35)==1) {   
         imm.q = BITS(instr[0],36,36) << 20 |
                 BITS(instr[0],6,25);
         sprintf(dest,"nop.f %d", LO(imm));
         return;
   }
   else
      sprintf(dest,"unimplemented");
}

/*----------------------------------------------------------------------*/
void m_ldfr_decode(qword *instr, char *dest){sprintf(dest,"unimplemented");}
void m_ldfi_decode(qword *instr, char *dest){sprintf(dest,"unimplemented");}
void f_compare_decode(qword *instr, char *dest){sprintf(dest,"unimplemented");}
void f_class_decode(qword *instr, char *dest){sprintf(dest,"unimplemented");}
void f_fma_decode(qword *instr, char *dest){sprintf(dest,"unimplemented");}
void f_fms_decode(qword *instr, char *dest){sprintf(dest,"unimplemented");}
void f_fnma_decode(qword *instr, char *dest){sprintf(dest,"unimplemented");}
void f_xma_decode(qword *instr, char *dest){sprintf(dest,"unimplemented");}
void x_misc_decode(qword *instr, char *dest){sprintf(dest,"unimplemented");}
void x_movl_decode(qword *instr, char *dest){sprintf(dest,"unimplemented");}

