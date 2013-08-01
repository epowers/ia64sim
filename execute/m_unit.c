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


extern char ldhint[4][8];
extern char sthint[4][8];

/*---------------------------------------------------------------------*/
uint32 m_ldir(qword *instr) {

   uint32 rd, rs1, rs2, opcode, status;
   uint64 addr;

   rd = BITS(instr[0],6,12);
   rs1 = BITS(instr[0],13,19);
   rs2 = BITS(instr[0],20,26);
   opcode = BITS(instr[0],30,36)<<1 | BITS(instr[0],27,27);

   if(rs2) check_reg(rs2 | ALU_REG);

   switch(opcode) {

      case 0*2:
         trcins("ld1%s r%d = [r%d]\n", ldhint[BITS(instr[0],28,29)],
                                      rd,rs2);
         check_reg(rd | ALU_REG);
         add_hazard(rd | ALU_REG);
         addr = r[rs2].dat.q;
         if(r[rs1].nat) return NAT_CONSUMPTION;
         if( (status=load_mem((uint8 *)(&r[rd].dat.q),&addr,1)) )
            return status;
         r[rd].nat = 0;
         return IA_OK;

      case 1*2:
         trcins("ld2%s r%d = [r%d]\n", ldhint[BITS(instr[0],28,29)],
                                      rd,rs2);
         check_reg(rd | ALU_REG);
         add_hazard(rd | ALU_REG);
         addr = r[rs2].dat.q;
         if(r[rs1].nat) return NAT_CONSUMPTION;
         if( (status=load_mem((uint8 *)(&r[rd].dat.q),&addr,2)) )
            return status;
         r[rd].nat = 0;
         return IA_OK;

      case 2*2:
         trcins("ld4%s r%d = [r%d]\n", ldhint[BITS(instr[0],28,29)],
                                      rd,rs2);
         check_reg(rd | ALU_REG);
         add_hazard(rd | ALU_REG);
         addr = r[rs2].dat.q;
         if(r[rs1].nat) return NAT_CONSUMPTION;
         if( (status=load_mem((uint8 *)(&r[rd].dat.q),&addr,4)) )
            return status;
         r[rd].nat = 0;
         return IA_OK;

      case 3*2:
         trcins("ld8%s r%d = [r%d]\n", ldhint[BITS(instr[0],28,29)],
                                      rd,rs2);
         check_reg(rd | ALU_REG);
         add_hazard(rd | ALU_REG);
         addr = r[rs2].dat.q;
         if(r[rs1].nat) return NAT_CONSUMPTION;
         if( (status=load_mem((uint8 *)(&r[rd].dat.q),&addr,8)) )
            return status;
         r[rd].nat = 0;
         return IA_OK;

      case 4*2:
         trcins("ld1.s%s r%d = [r%d]\n", ldhint[BITS(instr[0],28,29)],
                                      rd,rs2);
         check_reg(rd | ALU_REG);
         add_hazard(rd | ALU_REG);
         addr = r[rs2].dat.q;
         r[rd].nat = r[rs2].nat | (load_mem((uint8 *)(&r[rd].dat.q),&addr,1)!=IA_OK);
         return IA_OK;

      case 5*2:
         trcins("ld2.s%s r%d = [r%d]\n", ldhint[BITS(instr[0],28,29)],
                                      rd,rs2);
         check_reg(rd | ALU_REG);
         add_hazard(rd | ALU_REG);
         addr = r[rs2].dat.q;
         r[rd].nat = r[rs2].nat | (load_mem((uint8 *)(&r[rd].dat.q),&addr,2)!=IA_OK);
         return IA_OK;

      case 6*2:
         trcins("ld4.s%s r%d = [r%d]\n", ldhint[BITS(instr[0],28,29)],
                                      rd,rs2);
         check_reg(rd | ALU_REG);
         add_hazard(rd | ALU_REG);
         addr = r[rs2].dat.q;
         r[rd].nat = r[rs2].nat | (load_mem((uint8 *)(&r[rd].dat.q),&addr,4)!=IA_OK);
         return IA_OK;

      case 7*2:
         trcins("ld8.s%s r%d = [r%d]\n", ldhint[BITS(instr[0],28,29)],
                                      rd,rs2);
         check_reg(rd | ALU_REG);
         add_hazard(rd | ALU_REG);
         addr = r[rs2].dat.q;
         r[rd].nat = r[rs2].nat | (load_mem((uint8 *)(&r[rd].dat.q),&addr,8)!=IA_OK);
         return IA_OK;

      case 1*128+0*2:
         trcins("ld1%s r%d = [r%d], r%d\n", ldhint[BITS(instr[0],28,29)],
                                      rd,rs2,rs1);
         check_reg(rd | ALU_REG);
         add_hazard(rd | ALU_REG);
         add_hazard(rs2 | ALU_REG);
         if(rs1) check_reg(rs1 | ALU_REG);
         if(rd==rs2) return ILLEGAL_INSTR;
         if(r[rs1].nat || r[rs2].nat) return NAT_CONSUMPTION;
         addr = r[rs2].dat.q;
         if( (status=load_mem((uint8 *)(&r[rd].dat.q),&addr,1)) )
            return status;
         r[rd].dat.q += r[rs1].dat.q;
         r[rd].nat = 0;
         return IA_OK;

      case 1*128+1*2:
         trcins("ld2%s r%d = [r%d], r%d\n", ldhint[BITS(instr[0],28,29)],
                                      rd,rs2,rs1);
         check_reg(rd | ALU_REG);
         add_hazard(rd | ALU_REG);
         add_hazard(rs2 | ALU_REG);
         if(rs1) check_reg(rs1 | ALU_REG);
         if(rd==rs2) return ILLEGAL_INSTR;
         if(r[rs1].nat || r[rs2].nat) return NAT_CONSUMPTION;
         addr = r[rs2].dat.q;
         if( (status=load_mem((uint8 *)(&r[rd].dat.q),&addr,2)) )
            return status;
         r[rd].dat.q += r[rs1].dat.q;
         r[rd].nat = 0;
         return IA_OK;

      case 1*128+2*2:
         trcins("ld4%s r%d = [r%d], r%d\n", ldhint[BITS(instr[0],28,29)],
                                      rd,rs2,rs1);
         check_reg(rd | ALU_REG);
         add_hazard(rd | ALU_REG);
         add_hazard(rs2 | ALU_REG);
         if(rs1) check_reg(rs1 | ALU_REG);
         if(rd==rs2) return ILLEGAL_INSTR;
         if(r[rs1].nat || r[rs2].nat) return NAT_CONSUMPTION;
         addr = r[rs2].dat.q;
         if( (status=load_mem((uint8 *)(&r[rd].dat.q),&addr,4)) )
            return status;
         r[rd].dat.q += r[rs1].dat.q;
         r[rd].nat = 0;
         return IA_OK;

      case 1*128+3*2:
         trcins("ld8%s r%d = [r%d], r%d\n", ldhint[BITS(instr[0],28,29)],
                                      rd,rs2,rs1);
         check_reg(rd | ALU_REG);
         add_hazard(rd | ALU_REG);
         add_hazard(rs2 | ALU_REG);
         if(rs1) check_reg(rs1 | ALU_REG);
         if(rd==rs2) return ILLEGAL_INSTR;
         if(r[rs1].nat || r[rs2].nat) return NAT_CONSUMPTION;
         addr = r[rs2].dat.q;
         if( (status=load_mem((uint8 *)(&r[rd].dat.q),&addr,8)) )
            return status;
         r[rd].dat.q += r[rs1].dat.q;
         r[rd].nat = 0;
         return IA_OK;

      case 1*128+4*2:
         trcins("ld1.s%s r%d = [r%d], r%d\n", ldhint[BITS(instr[0],28,29)],
                                      rd,rs2,rs1);
         check_reg(rd | ALU_REG);
         add_hazard(rd | ALU_REG);
         add_hazard(rs2 | ALU_REG);
         if(rs1) check_reg(rs1 | ALU_REG);
         if(rd==rs2) return ILLEGAL_INSTR;
         addr = r[rs2].dat.q;
         r[rs2].nat |= r[rs1].nat;
         r[rd].nat = r[rs2].nat |
              (load_mem((uint8 *)(&r[rd].dat.q),&addr,1)!=IA_OK);
         r[rd].dat.q += r[rs1].dat.q;
         return IA_OK;

      case 1*128+5*2:
         trcins("ld2.s%s r%d = [r%d], r%d\n", ldhint[BITS(instr[0],28,29)],
                                      rd,rs2,rs1);
         check_reg(rd | ALU_REG);
         add_hazard(rd | ALU_REG);
         add_hazard(rs2 | ALU_REG);
         if(rs1) check_reg(rs1 | ALU_REG);
         if(rd==rs2) return ILLEGAL_INSTR;
         addr = r[rs2].dat.q;
         r[rs2].nat |= r[rs1].nat;
         r[rd].nat = r[rs2].nat |
              (load_mem((uint8 *)(&r[rd].dat.q),&addr,2)!=IA_OK);
         return IA_OK;

      case 1*128+6*2:
         trcins("ld4.s%s r%d = [r%d], r%d\n", ldhint[BITS(instr[0],28,29)],
                                      rd,rs2,rs1);
         check_reg(rd | ALU_REG);
         add_hazard(rd | ALU_REG);
         add_hazard(rs2 | ALU_REG);
         if(rs1) check_reg(rs1 | ALU_REG);
         if(rd==rs2) return ILLEGAL_INSTR;
         addr = r[rs2].dat.q;
         r[rs2].nat |= r[rs1].nat;
         r[rd].nat = r[rs2].nat |
              (load_mem((uint8 *)(&r[rd].dat.q),&addr,4)!=IA_OK);
         return IA_OK;

      case 1*128+7*2:
         trcins("ld8.s%s r%d = [r%d], r%d\n", ldhint[BITS(instr[0],28,29)],
                                      rd,rs2,rs1);
         check_reg(rd | ALU_REG);
         add_hazard(rd | ALU_REG);
         add_hazard(rs2 | ALU_REG);
         if(rs1) check_reg(rs1 | ALU_REG);
         if(rd==rs2) return ILLEGAL_INSTR;
         addr = r[rs2].dat.q;
         r[rs2].nat |= r[rs1].nat;
         r[rd].nat = r[rs2].nat |
              (load_mem((uint8 *)(&r[rd].dat.q),&addr,8)!=IA_OK);
         return IA_OK;
                                      
      case 0x30*2:
         trcins("st1%s [r%d] = r%d\n", sthint[BITS(instr[0],28,29)],
                                      rs2,rs1);
         check_reg(rs1 | ALU_REG);
         addr = r[rs2].dat.q;
         if( (status=store_mem((uint8 *)(&r[rs1].dat.q),&addr,1)) )
            return status;
         return IA_OK;

      case 0x31*2:
         trcins("st2%s [r%d] = r%d\n", sthint[BITS(instr[0],28,29)],
                                      rs2,rs1);
         check_reg(rs1 | ALU_REG);
         addr = r[rs2].dat.q;
         if( (status=store_mem((uint8 *)(&r[rs1].dat.q),&addr,2)) )
            return status;
         return IA_OK;

      case 0x32*2:
         trcins("st4%s [r%d] = r%d\n", sthint[BITS(instr[0],28,29)],
                                      rs2,rs1);
         check_reg(rs1 | ALU_REG);
         addr = r[rs2].dat.q;
         if( (status=store_mem((uint8 *)(&r[rs1].dat.q),&addr,4)) )
            return status;
         return IA_OK;

      case 0x33*2:
         trcins("st8%s [r%d] = r%d\n", sthint[BITS(instr[0],28,29)],
                                      rs2,rs1);
         check_reg(rs1 | ALU_REG);
         addr = r[rs2].dat.q;
         if( (status=store_mem((uint8 *)(&r[rs1].dat.q),&addr,8)) )
            return status;
         return IA_OK;

      default:
         return UNIMPLEMENTED;
   }
}

/*---------------------------------------------------------------------*/
uint32 m_mgmt(qword *instr) {

   uint32 sof, soi, sol, sor, rd, rs1, opcode;
   uint64 imm;

   opcode = BITS(instr[0],33,40) &~8;

   switch(opcode) {
   
      case 1*16+1:
         imm = -((int64)BITS(instr[0],36,36)) << 24 |
              BITS(instr[0],20,32) << 11 | BITS(instr[0],6,12) << 4;
         trcins("chk.s.m r%d, %d\n", rs1,imm);
         rs1 = BITS(instr[0],13,19);
         check_reg(rs1 | ALU_REG);
         if( r[rs1].nat ) {
            ip.q += imm;
            add_hazard(PC_REG);
         }
         return IA_OK;

      case 1*16+6:
         rd = BITS(instr[0],6,12);
         sof = BITS(instr[0],13,19);
         sol = BITS(instr[0],20,26);
         sor = BITS(instr[0],27,30) << 3;
         trcins("alloc r%d = ar.pfs, {%d},%d,%d\n", rd,
                                    sol,sof - sol, sor );
         check_reg(rd | ALU_REG);
         add_hazard(rd | ALU_REG);
         if( sof<=96 && sof>=sol && sol>=sor ) {
            stop();
            r[rd].dat.q = ar[AR_PFS].q;
            cfm.q = BITS(instr[0],13,30);
         }
         else {
            return ILLEGAL_INSTR;
         }
         return IA_OK;

      case 0:
         imm = -(int64)BITS(instr[0],36,36)<<20 |
                       BITS(instr[0],6,25);
         if( BITS(instr[0],27,40)==0 ) {
            trcins("break.m %d\n", (uint32)imm);
            iim = BITS(instr[0],6,25);
            faults |= BREAK_FAULT;
            return IA_OK;
         }
         else if( BITS(instr[0],27,40)==1 ) {
            trcins("nop.m %d\n", (uint32)imm);
            return IA_OK;
         }

      default:
         return UNIMPLEMENTED;
   }            
}

/*---------------------------------------------------------------------*/
uint32 m_ldii(qword *instr) {
   int64  imm;
   uint32 r1,r2,r3, opcode, status;
   uint64 addr;
   
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


   if(r3) check_reg(r3 | ALU_REG);

   switch(opcode) {

      case 0*2:
         trcins("ld1%s r%d = [r%d],%d\n", ldhint[BITS(instr[0],28,29)],
                                      r1,r3,imm);
         check_reg(r1 | ALU_REG);
         add_hazard(r1 | ALU_REG);
         addr = r[r3].dat.q;
         if( (status=load_mem((uint8 *)(&r[r1].dat.q),&addr,1)) )
            return status;
         r[r3].dat.q += imm;
         r[r1].nat = 0;
         return IA_OK;

      case 1*2:
         trcins("ld2%s r%d = [r%d,%d]\n", ldhint[BITS(instr[0],28,29)],
                                      r1,r3,imm);
         check_reg(r1 | ALU_REG);
         add_hazard(r1 | ALU_REG);
         addr = r[r3].dat.q;
         if( (status=load_mem((uint8 *)(&r[r1].dat.q),&addr,2)) )
            return status;
         r[r3].dat.q += imm;
         r[r1].nat = 0;
         return IA_OK;

      case 2*2:
         trcins("ld4%s r%d = [r%d],%d\n", ldhint[BITS(instr[0],28,29)],
                                      r1,r3,imm);
         check_reg(r1 | ALU_REG);
         add_hazard(r1 | ALU_REG);
         addr = r[r3].dat.q;
         if( (status=load_mem((uint8 *)(&r[r1].dat.q),&addr,4)) )
            return status;
         r[r3].dat.q += imm;
         r[r1].nat = 0;
         return IA_OK;

      case 3*2:
         trcins("ld8%s r%d = [r%d],%d\n", ldhint[BITS(instr[0],28,29)],
                                      r1,r3,imm);
         check_reg(r1 | ALU_REG);
         add_hazard(r1 | ALU_REG);
         addr = r[r3].dat.q;
         if( (status=load_mem((uint8 *)(&r[r1].dat.q),&addr,8)) )
            return status;
         r[r3].dat.q += imm;
         r[r1].nat = 0;
         return IA_OK;

      case 4*2:
         trcins("ld1.s%s r%d = [r%d]\n", ldhint[BITS(instr[0],28,29)],
                                      r1,r3,imm);
         check_reg(r1 | ALU_REG);
         add_hazard(r1 | ALU_REG);
         addr = r[r3].dat.q;
         r[r1].nat = r[r3].nat | (load_mem((uint8 *)(&r[r1].dat.q),&addr,1)!=IA_OK);
         r[r3].dat.q += imm;
         return IA_OK;

      case 5*2:
         trcins("ld2.s%s r%d = [r%d]\n", ldhint[BITS(instr[0],28,29)],
                                      r1,r3,imm);
         check_reg(r1 | ALU_REG);
         add_hazard(r1 | ALU_REG);
         addr = r[r3].dat.q;
         r[r1].nat = r[r3].nat | (load_mem((uint8 *)(&r[r1].dat.q),&addr,2)!=IA_OK);
         r[r3].dat.q += imm;
         return IA_OK;

      case 6*2:
         trcins("ld4.s%s r%d = [r%d]\n", ldhint[BITS(instr[0],28,29)],
                                      r1,r3,imm);
         check_reg(r1 | ALU_REG);
         add_hazard(r1 | ALU_REG);
         addr = r[r3].dat.q;
         r[r1].nat = r[r3].nat | (load_mem((uint8 *)(&r[r1].dat.q),&addr,4)!=IA_OK);
         r[r3].dat.q += imm;
         return IA_OK;

      case 7*2:
         trcins("ld8.s%s r%d = [r%d]\n", ldhint[BITS(instr[0],28,29)],
                                      r1,r3,imm);
         check_reg(r1 | ALU_REG);
         add_hazard(r1 | ALU_REG);
         addr = r[r3].dat.q;
         r[r1].nat = r[r3].nat | (load_mem((uint8 *)(&r[r1].dat.q),&addr,8)!=IA_OK);
         r[r3].dat.q += imm;
         return IA_OK;

                                      
      case 0x30*2:
         trcins("st1%s [r%d] = r%d,%d\n", sthint[BITS(instr[0],28,29)],
                                      r3,r2,imm);
         check_reg(r2 | ALU_REG);
         addr = r[r3].dat.q;
         if( (status=store_mem((uint8 *)(&r[r2].dat.q+imm),&addr,1)) )
            return status;
         return IA_OK;

      case 0x31*2:
         trcins("st2%s [r%d] = r%d,%d\n", sthint[BITS(instr[0],28,29)],
                                      r3,r2,imm);
         check_reg(r2 | ALU_REG);
         addr = r[r3].dat.q;
         if( (status=store_mem((uint8 *)(&r[r2].dat.q+imm),&addr,2)) )
            return status;
         return IA_OK;

      case 0x32*2:
         trcins("st4%s [r%d] = r%d,%d\n", sthint[BITS(instr[0],28,29)],
                                      r3,r2,imm);
         check_reg(r2 | ALU_REG);
         addr = r[r3].dat.q;
         if( (status=store_mem((uint8 *)(&r[r2].dat.q+imm),&addr,4)) )
            return status;
         return IA_OK;

      case 0x33*2:
         trcins("st8%s [r%d] = r%d,%d\n", sthint[BITS(instr[0],28,29)],
                                      r3,r2,imm);
         check_reg(r2 | ALU_REG);
         addr = r[r3].dat.q;
         if( (status=store_mem((uint8 *)(&r[r2].dat.q+imm),&addr,8)) )
            return status;
         return IA_OK;

      default:
         return UNIMPLEMENTED;
   }
}
/*---------------------------------------------------------------------*/
uint32 m_ldfr(qword *instr) { return UNIMPLEMENTED; }
/*---------------------------------------------------------------------*/
uint32 m_ldfi(qword *instr) { return UNIMPLEMENTED; }
