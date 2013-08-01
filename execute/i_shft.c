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

/*----------------------------------------------------------------------*/
uint32 i_mmshift(qword *instr) {

   uint32 rs1, rs2, rd;
   uint32 opcode;
   uint32 shifts[]={0,7,15,16};
   uint32 i, t0;

   rs2 = BITS(instr[0],20,26); 
   rs1 = BITS(instr[0],13,19);
   rd = BITS(instr[0],6,12);

   check_reg(rd | ALU_REG);
   add_hazard( rd | ALU_REG );
   opcode = BITS(instr[0],28,36);

   switch(opcode) {

      case 0*256+1*32+0*16+0*64+3+0*4: 
      case 0*256+1*32+0*16+0*64+3+1*4: 
      case 0*256+1*32+0*16+0*64+3+2*4: 
      case 0*256+1*32+0*16+0*64+3+3*4:
         trcins("pmpyshr2 r%d = r%d,r%d,%d\n", rd,rs1,rs2,
                        shifts[(opcode>>2)&3] );
         if(rs1) check_reg(rs1 | ALU_REG); if(rs2) check_reg(rs2 | ALU_REG);
         for(i=0; i<4; i++) {
            t0 = (int32)( (int16)(r[rs1].dat.w[i]) *
                          (int16)(r[rs2].dat.w[i]) );
            r[rd].dat.w[i] = ((int32)t0) >> shifts[(opcode>>2)&3];
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 0*256+1*32+0*16+0*64+1+0*4: 
      case 0*256+1*32+0*16+0*64+1+1*4: 
      case 0*256+1*32+0*16+0*64+1+2*4: 
      case 0*256+1*32+0*16+0*64+1+3*4:
         trcins("pmpyshr2.u r%d = r%d,r%d,%d\n", rd,rs1,rs2,
                        shifts[(opcode>>2)&3] );
         if(rs1) check_reg(rs1 | ALU_REG); if(rs2) check_reg(rs2 | ALU_REG);
         for(i=0; i<4; i++) {
            t0 = r[rs1].dat.w[i] * r[rs2].dat.w[i];
            r[rd].dat.w[i] = t0 >> shifts[(opcode>>2)&3];
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 0*256+1*32+0*16+2*64+1+3*4:
         trcins("pmpy2.r r%d = r%d,r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG); if(rs2) check_reg(rs2 | ALU_REG);
         for(i=0; i<2; i++) {
            t0 = (int16)(r[rs1].dat.w[2*i]) *
                 (int16)(r[rs2].dat.w[2*i]);
            r[rd].dat.d[i] = t0;
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 0*256+1*32+0*16+2*64+3+3*4:
         trcins("pmpy2.l r%d = r%d,r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG); if(rs2) check_reg(rs2 | ALU_REG);
         for(i=0; i<4; i+=2) {
            t0 = (int16)(r[rs1].dat.w[2*i+1]) *
                 (int16)(r[rs2].dat.w[2*i+1]);
            r[rd].dat.d[i] = t0;
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 0*256+0*32+0*16+2*64+0+2*4:
         trcins("mix1.r r%d = r%d,r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG); if(rs2) check_reg(rs2 | ALU_REG);
         for(i=0; i<4; i++) {
            r[rd].dat.b[2*i+1] = r[rs1].dat.b[2*i];
            r[rd].dat.b[2*i] = r[rs2].dat.b[2*i];
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 0*256+1*32+0*16+2*64+0+2*4:
         trcins("mix2.r r%d = r%d,r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG); if(rs2) check_reg(rs2 | ALU_REG);
         for(i=0; i<2; i++) {
            r[rd].dat.w[2*i+1] = r[rs1].dat.w[2*i];
            r[rd].dat.w[2*i] = r[rs2].dat.w[2*i];
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+0*32+0*16+2*64+0+2*4:
         trcins("mix4.r r%d = r%d,r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG); if(rs2) check_reg(rs2 | ALU_REG);
         HI(r[rd].dat) = LO(r[rs1].dat);
         LO(r[rd].dat) = LO(r[rs2].dat);
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 0*256+0*32+0*16+2*64+2+2*4:
         trcins("mix1.l r%d = r%d,r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG); if(rs2) check_reg(rs2 | ALU_REG);
         for(i=0; i<4; i++) {
            r[rd].dat.b[2*i+1] = r[rs1].dat.b[2*i+1];
            r[rd].dat.b[2*i] = r[rs2].dat.b[2*i+1];
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 0*256+1*32+0*16+2*64+2+2*4:
         trcins("mix2.l r%d = r%d,r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG); if(rs2) check_reg(rs2 | ALU_REG);
         for(i=0; i<2; i++) {
            r[rd].dat.w[2*i+1] = r[rs1].dat.w[2*i+1];
            r[rd].dat.w[2*i] = r[rs2].dat.w[2*i+1];
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+0*32+0*16+2*64+2+2*4:
         trcins("mix4.l r%d = r%d,r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG); if(rs2) check_reg(rs2 | ALU_REG);
         HI(r[rd].dat) = HI(r[rs1].dat);
         LO(r[rd].dat) = HI(r[rs2].dat);
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 0*256+1*32+0*16+2*64+0+0*4:
         trcins("pack2.uss r%d = r%d,r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG); if(rs2) check_reg(rs2 | ALU_REG);
         for(i=0; i<4; i++) {
            if( (int16)r[rs2].dat.w[i] > 255 )
               r[rd].dat.b[i] = 0xff;
            else if( (int16)r[rs2].dat.w[i] < 0 )
               r[rd].dat.b[i] = 0x00;
            else
               r[rd].dat.b[i] = r[rs2].dat.b[2*i];
            if( (int16)r[rs1].dat.w[i] > 255 )
               r[rd].dat.b[i+4] = 0xff;
            else if( (int16)r[rs1].dat.w[i] < 0 )
               r[rd].dat.b[i+4] = 0x00;
            else
               r[rd].dat.b[i+4] = r[rs2].dat.b[2*i];
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 0*256+1*32+0*16+2*64+2+0*4:
         trcins("pack2.sss r%d = r%d,r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG); if(rs2) check_reg(rs2 | ALU_REG);
         for(i=0; i<4; i++) {
            if( (int16)r[rs2].dat.w[i] > 127 )
               r[rd].dat.b[i] = 0x7f;
            else if( (int16)r[rs2].dat.w[i] < -128 )
               r[rd].dat.b[i] = 0x80;
            else
               r[rd].dat.b[i] = r[rs2].dat.b[2*i];
            if( (int16)r[rs1].dat.w[i] > 127 )
               r[rd].dat.b[i+4] = 0x7f;
            else if( (int16)r[rs1].dat.w[i] < -128 )
               r[rd].dat.b[i+4] = 0x80;
            else
               r[rd].dat.b[i+4] = r[rs2].dat.b[2*i];
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+0*32+0*16+2*64+2+0*4:
         trcins("pack4.sss r%d = r%d,r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG); if(rs2) check_reg(rs2 | ALU_REG);
         for(i=0; i<2; i++) {
            if( (int32)r[rs2].dat.d[i] > 32767 )
               r[rd].dat.w[i] = 0x7fff;
            else if( (int32)r[rs2].dat.d[i] < -32768 )
               r[rd].dat.w[i] = 0x8000;
            else
               r[rd].dat.w[i] = r[rs2].dat.w[2*i];
            if( (int32)(r[rs2].dat.d[i]) > 32767 )
               r[rd].dat.w[i+2] = 0x7fff;
            else if( (int32)r[rs2].dat.d[i] < -32768 )
               r[rd].dat.w[i+2] = 0x8000;
            else
               r[rd].dat.w[i+2] = r[rs2].dat.w[2*i];
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 0*256+0*32+0*16+2*64+0+1*4:
         trcins("unpack1.h r%d = r%d,r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG); if(rs2) check_reg(rs2 | ALU_REG);
         for(i=0; i<4; i++) {
            r[rd].dat.b[2*i+1] = r[rs1].dat.b[i+4];
            r[rd].dat.b[2*i] = r[rs2].dat.b[i+4];
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 0*256+1*32+0*16+2*64+0+1*4:
         trcins("unpack2.h r%d = r%d,r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG); if(rs2) check_reg(rs2 | ALU_REG);
         for(i=0; i<2; i++) {
            r[rd].dat.w[2*i+1] = r[rs1].dat.w[i+2];
            r[rd].dat.w[2*i] = r[rs2].dat.w[i+2];
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+0*32+0*16+2*64+0+1*4:
         trcins("unpack4.h r%d = r%d,r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG); if(rs2) check_reg(rs2 | ALU_REG);
         HI(r[rd].dat) = HI(r[rs1].dat);
         LO(r[rd].dat) = HI(r[rs2].dat);
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;


      case 0*256+0*32+0*16+2*64+2+1*4:
         trcins("unpack1.l r%d = r%d,r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG); if(rs2) check_reg(rs2 | ALU_REG);
         for(i=0; i<4; i++) {
            r[rd].dat.b[2*i+1] = r[rs1].dat.b[i];
            r[rd].dat.b[2*i] = r[rs2].dat.b[i];
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 0*256+1*32+0*16+2*64+2+1*4:
         trcins("unpack2.l r%d = r%d,r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG); if(rs2) check_reg(rs2 | ALU_REG);
         for(i=0; i<2; i++) {
            r[rd].dat.w[2*i+1] = r[rs1].dat.w[i];
            r[rd].dat.w[2*i] = r[rs2].dat.w[i];
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+0*32+0*16+2*64+2+1*4:
         trcins("unpack4.l r%d = r%d,r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG); if(rs2) check_reg(rs2 | ALU_REG);
         HI(r[rd].dat) = LO(r[rs1].dat);
         LO(r[rd].dat) = LO(r[rs2].dat);
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 0*256+0*32+0*16+2*64+1+0*4:
         trcins("pmin1.u r%d = r%d,r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG); if(rs2) check_reg(rs2 | ALU_REG);
         for(i=0; i<8; i++) {
            if( r[rs1].dat.b[i] < r[rs2].dat.b[i] )
                r[rd].dat.b[i] = r[rs1].dat.b[i];
             else
                r[rd].dat.b[i] = r[rs2].dat.b[i];
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 0*256+0*32+0*16+2*64+1+1*4:
         trcins("pmax1.u r%d = r%d,r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG); if(rs2) check_reg(rs2 | ALU_REG);
         for(i=0; i<8; i++) {
            if( r[rs1].dat.b[i] > r[rs2].dat.b[i] )
                r[rd].dat.b[i] = r[rs1].dat.b[i];
             else
                r[rd].dat.b[i] = r[rs2].dat.b[i];
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 0*256+1*32+0*16+2*64+3+0*4:
         trcins("pmin2 r%d = r%d,r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG); if(rs2) check_reg(rs2 | ALU_REG);
         for(i=0; i<4; i++) {
            if( (int16)r[rs1].dat.w[i] <
                  (int16)r[rs2].dat.w[i] )
                r[rd].dat.w[i] = r[rs1].dat.w[i];
             else
                r[rd].dat.w[i] = r[rs2].dat.w[i];
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 0*256+1*32+0*16+2*64+3+1*4:
         trcins("pmax2 r%d = r%d,r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG); if(rs2) check_reg(rs2 | ALU_REG);
         for(i=0; i<4; i++) {
            if( (int16)r[rs1].dat.w[i] >
                  (int16)r[rs2].dat.w[i] )
                r[rd].dat.w[i] = r[rs1].dat.w[i];
             else
                r[rd].dat.w[i] = r[rs2].dat.w[i];
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 0*256+0*32+0*16+2*64+3+2*4:
         trcins("psad1 r%d = r%d,r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG); if(rs2) check_reg(rs2 | ALU_REG);
         for(i=0,t0=0; i<8; i++)
            t0 += abs(r[rs1].dat.b[i] - r[rs2].dat.b[i]);
         r[rd].dat.q = (uint64)t0;
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 0*256+0*32+0*16+3*64+2+2*4:
         if(rs1) check_reg(rs1 | ALU_REG);
         switch(rs2&15) {
            case 0:
               trcins("mux1 r%d = r%d,@brcast\n", rd,rs1);
               for(i=0; i<8; i++)
                  r[rd].dat.b[i] = r[rs1].dat.b[0];
               r[rd].nat = r[rs1].nat;
               return IA_OK;

            case 8:
               trcins("mux1 r%d = r%d,@mix\n", rd,rs1);
               r[rd].dat.q = r[rs1].dat.q;
               r[rd].dat.b[1] = r[rs1].dat.b[4];
               r[rd].dat.b[3] = r[rs1].dat.b[6];
               r[rd].dat.b[5] = r[rs1].dat.b[1];
               r[rd].dat.b[7] = r[rs1].dat.b[3];
               r[rd].nat = r[rs1].nat;
               return IA_OK;

            case 9:
               trcins("mux1 r%d = r%d,@shuf\n", rd,rs1);
               r[rd].dat.b[0] = r[rs1].dat.b[0];
               r[rd].dat.b[1] = r[rs1].dat.b[4];
               r[rd].dat.b[2] = r[rs1].dat.b[1];
               r[rd].dat.b[3] = r[rs1].dat.b[5];
               r[rd].dat.b[4] = r[rs1].dat.b[2];
               r[rd].dat.b[5] = r[rs1].dat.b[6];
               r[rd].dat.b[6] = r[rs1].dat.b[3];
               r[rd].dat.b[7] = r[rs1].dat.b[7];
               r[rd].nat = r[rs1].nat;
               return IA_OK;

            case 10:
               trcins("mux1 r%d = r%d,@alt\n", rd,rs1);
               r[rd].dat.b[0] = r[rs1].dat.b[0];
               r[rd].dat.b[1] = r[rs1].dat.b[2];
               r[rd].dat.b[2] = r[rs1].dat.b[4];
               r[rd].dat.b[3] = r[rs1].dat.b[6];
               r[rd].dat.b[4] = r[rs1].dat.b[1];
               r[rd].dat.b[5] = r[rs1].dat.b[3];
               r[rd].dat.b[6] = r[rs1].dat.b[5];
               r[rd].dat.b[7] = r[rs1].dat.b[7];
               r[rd].nat = r[rs1].nat;
               return IA_OK;

            case 11:
               trcins("mux1 r%d = r%d,@rev\n", rd,rs1);
               for(i=0; i<8; i++)
                  r[rd].dat.b[i] = r[rs1].dat.b[7-i];
               r[rd].nat = r[rs1].nat;
               return IA_OK;

            default:
               return ILLEGAL_INSTR;
         }
         return IA_OK;

      case 0*256+1*32+0*16+3*64+2+2*4:
         rs2 = BITS(instr[0],20,27);
         trcins("mux2 r%d = r%d,0x%02x\n", rd,rs1,rs2);
         if(rs1) check_reg(rs1 | ALU_REG);
         for(i=0; i<4; i++) {
            r[rd].dat.w[ rs2 & 3 ] = r[rs1].dat.w[i];
            rs2 >>= 2;
         }
         r[rd].nat = r[rs1].nat;
         return IA_OK;

      case 0*256+1*32+0*16+0*64+2+0*4:
         trcins("pshr2 r%d = r%d,r%d\n", rd,rs2,rs1 );
         if(rs1) check_reg(rs1 | ALU_REG); if(rs2) check_reg(rs2 | ALU_REG);
         for(i=0; i<4; i++)
            r[rd].dat.w[i] = (int16)r[rs2].dat.w[i] >> LO(r[rs1].dat);
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+0*32+0*16+0*64+2+0*4:
         trcins("pshr4 r%d = r%d,r%d\n", rd,rs2,rs1 );
         if(rs1) check_reg(rs1 | ALU_REG); if(rs2) check_reg(rs2 | ALU_REG);
         r[rd].dat.d[0] = (int32)r[rs2].dat.d[0] >> LO(r[rs1].dat);
         r[rd].dat.d[1] = (int32)r[rs2].dat.d[1] >> LO(r[rs1].dat);
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+1*32+0*16+0*64+2+0*4:
         trcins("shr r%d = r%d,r%d\n", rd,rs2,rs1 );
         if(rs1) check_reg(rs1 | ALU_REG); if(rs2) check_reg(rs2 | ALU_REG);
         r[rd].dat.q = (int64)r[rs2].dat.q >> LO(r[rs1].dat);
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 0*256+1*32+0*16+0*64+0+0*4:
         trcins("pshr2.u r%d = r%d,r%d\n", rd,rs2,rs1 );
         if(rs1) check_reg(rs1 | ALU_REG); if(rs2) check_reg(rs2 | ALU_REG);
         for(i=0; i<4; i++)
            r[rd].dat.w[i] = r[rs2].dat.w[i] >> LO(r[rs1].dat);
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+0*32+0*16+0*64+0+0*4:
         trcins("pshr4.u r%d = r%d,r%d\n", rd,rs2,rs1 );
         if(rs1) check_reg(rs1 | ALU_REG); if(rs2) check_reg(rs2 | ALU_REG);
         r[rd].dat.d[0] = r[rs2].dat.d[0] >> LO(r[rs1].dat);
         r[rd].dat.d[1] = r[rs2].dat.d[1] >> LO(r[rs1].dat);
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+1*32+0*16+0*64+0+0*4:
         trcins("shr.u r%d = r%d,r%d\n", rd,rs2,rs1 );
         if(rs1) check_reg(rs1 | ALU_REG); if(rs2) check_reg(rs2 | ALU_REG);
         r[rd].dat.q = r[rs2].dat.q >> LO(r[rs1].dat);
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 0*256+1*32+0*16+1*64+3+0*4:
         trcins("pshr2 r%d = r%d,%d\n", rd,rs2,rs1>>1 );
         if(rs2) check_reg(rs2 | ALU_REG); rs1 >>= 1;
         for(i=0; i<4; i++)
            r[rd].dat.w[i] = (int16)r[rs2].dat.w[i] >> rs1;
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+0*32+0*16+1*64+3+0*4:
         trcins("pshr4 r%d = r%d,%d\n", rd,rs2,rs1>>1 );
         if(rs2) check_reg(rs2 | ALU_REG); rs1 >>= 1;
         r[rd].dat.d[0] = (int32)r[rs2].dat.d[0] >> rs1;
         r[rd].dat.d[1] = (int32)r[rs2].dat.d[1] >> rs1;
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 0*256+1*32+0*16+1*64+1+0*4:
         trcins("pshr2.u r%d = r%d,%d\n", rd,rs2,rs1>>1 );
         if(rs2) check_reg(rs2 | ALU_REG); rs1 >>= 1;
         for(i=0; i<4; i++)
            r[rd].dat.w[i] = r[rs2].dat.w[i] >> rs1;
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+0*32+0*16+1*64+1+0*4:
         trcins("pshr4.u r%d = r%d,%d\n", rd,rs2,rs1>>1 );
         if(rs2) check_reg(rs2 | ALU_REG); rs1 >>= 1;
         r[rd].dat.d[0] = r[rs2].dat.d[0] >> rs1;
         r[rd].dat.d[1] = r[rs2].dat.d[1] >> rs1;
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 0*256+1*32+0*16+0*64+0+1*4:
         trcins("pshl2 r%d = r%d,r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG); if(rs2) check_reg(rs2 | ALU_REG);
         for(i=0; i<4; i++)
            r[rd].dat.w[i] = r[rs1].dat.w[i] << LO(r[rs2].dat);
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+0*32+0*16+0*64+0+1*4:
         trcins("pshl4 r%d = r%d,r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG); if(rs2) check_reg(rs2 | ALU_REG);
         r[rd].dat.d[0] = r[rs1].dat.d[0] << LO(r[rs2].dat);
         r[rd].dat.d[1] = r[rs1].dat.d[1] << LO(r[rs2].dat);
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+1*32+0*16+0*64+0+1*4:
         trcins("shl r%d = r%d,r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG); if(rs2) check_reg(rs2 | ALU_REG);
         r[rd].dat.q = r[rs1].dat.q << LO(r[rs2].dat);
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 0*256+1*32+0*16+1*64+1+2*4:
         trcins("popc r%d = r%d\n", rd,rs2 );
	 if(rs2) check_reg(rs2 | ALU_REG);
         if(rs1) return ILLEGAL_INSTR;
         r[rd].dat.q = r[rs2].dat.q; t0 = 0;
         for(i=0; i<64; i++) {
            t0 += LO(r[rd].dat) & 1;
            r[rd].dat.q >>= 1;
         }
         r[rd].dat.q = (uint64)t0;
         r[rd].nat = r[rs2].nat;
         return IA_OK;

   }

   return ILLEGAL_INSTR;
}

/*----------------------------------------------------------------------*/
uint32 i_shift(qword *instr) {

   uint32 rs1, rs2, rd;
   uint32 opcode, count, pos;
   qword temp;

   rs2 = BITS(instr[0],20,26);  
   rs1 = BITS(instr[0],13,19);
   rd = BITS(instr[0],6,12);
   pos = ~BITS(instr[0],20,25);

   check_reg(rd & ALU_REG);
   add_hazard( rd | ALU_REG );
   opcode = BITS(instr[0],33,35);
   count = BITS(instr[0],27,32);

   switch(opcode) {

      case 3*2+0:
         trcins("shrp r%d = r%d,r%d,%d\n", rd,rs1,rs2,count );
         if(rs1) check_reg(rs1 & ALU_REG); if(rs2) check_reg(rs2 & ALU_REG);
         temp.q = r[rs2].dat.q >> count;
         r[rd].dat.q = r[rs1].dat.q << (64-count);
         r[rd].dat.q |= temp.q;
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*2+0:
         if(rs2) check_reg(rs2 & ALU_REG);
         if( rs1 & 1 ) {
            rs1 >>= 1; count++;
            trcins("extr.u r%d = r%d,%d,%d\n", rd,rs2,rs1,count );
            r[rd].dat.q = r[rs2].dat.q << (64-rs1-count);
            r[rd].dat.q = r[rd].dat.q >> (64-count);
         }
         else {
            rs1 >>= 1; count++;
            trcins("extr r%d = r%d,%d,%d\n", rd,rs2,rs1,count );
            r[rd].dat.q = r[rs2].dat.q << (64-rs1-count);
            r[rd].dat.q = (int64)r[rd].dat.q >> (64-count);
         }
         r[rd].nat = r[rs2].nat;
         return IA_OK;

      case 1*2+1:
         if( rs2 >> 6 ) {
            temp.q = -((int64)BITS(instr[0],36,36)) << 7 | rs1;
            rs2 &= 63; count++;
            trcins("dep.z r%d = %d,%d,%d\n", rd,LO(temp),rs2,count );
            r[rd].dat.q = ((uint64)(1) << count );
            r[rd].dat.q = (r[rd].dat.q | temp.q) << rs2;
         }
         else {
            pos &= 63; count++;
            trcins("dep.z r%d = r%d,%d,%d\n", rd,rs1,pos,count );
            if(rs1) check_reg(rs1 | ALU_REG);
            r[rd].dat.q = ((uint64)(1) << count );
            r[rd].dat.q = (r[rd].dat.q | r[rs1].dat.q) << pos;
            r[rd].nat = r[rs1].nat;
         }
         return IA_OK;

      case 3*2+1:
         temp.q = -(int64)BITS(instr[0],36,36);
         rs1 >>= 1; count++;
         trcins("dep r%d = %d,r%d,%d,%d\n", rd,LO(temp),rs2,rs1,count );
         if(rs2) check_reg(rs2 | ALU_REG);
         r[rd].dat.q = ((uint64)(1) << count) - 1;
         temp.q &= r[rd].dat.q;
         r[rd].dat.q = r[rs2].dat.q & ~(r[rd].dat.q << rs1);
         r[rd].dat.q = r[rd].dat.q | (temp.q << rs1);
         r[rd].nat = r[rs1].nat;
         return IA_OK;

   }

   return ILLEGAL_INSTR;
}

/*----------------------------------------------------------------------*/
uint32 i_deposit(qword *instr) {

   uint32 rs1, rs2, rd;
   uint32 len, pos;
   qword temp;

   pos = BITS(instr[0],31,36);  
   len = BITS(instr[0],27,30)+1;  
   rs2 = BITS(instr[0],20,26);  
   rs1 = BITS(instr[0],13,19);
   rd = BITS(instr[0],6,12);

   if(rs1) check_reg(rs1 & ALU_REG);
   if(rs2) check_reg(rs2 & ALU_REG);
   check_reg(rd & ALU_REG);
   add_hazard( rd | ALU_REG );

   trcins("dep r%d = r%d,r%d,%d,%d\n", rd,rs1,rs2,pos,len );
   r[rd].dat.q = ((uint64)(1) << len) - 1;
   temp.q = r[rs1].dat.q & r[rd].dat.q;
   r[rd].dat.q = r[rs2].dat.q & ~(r[rd].dat.q << pos);
   r[rd].dat.q = r[rd].dat.q | (temp.q << pos);
   r[rd].nat = r[rs1].nat | r[rs2].nat;

   return IA_OK;
}

/*--------------------------------------------------------------------*/
uint32 i_test(qword *instr) { 

   uint32 p1, p2, r1, pred;
   uint32 opcode, pos;
   qword imm;

   pred = LO(instr[0]) & 63;
   p1 = BITS(instr[0],6,11);
   p2 = BITS(instr[0],27,32);
   r1 = BITS(instr[0],20,26);
   pos = BITS(instr[0],14,19);

   check_reg(p1 | PRD_REG);
   check_reg(p2 | PRD_REG);
   if(r1) check_reg(r1 | ALU_REG);

   if( p1==p2 ) return ILLEGAL_INSTR;

   opcode = BITS(instr[0],33,36) << 2 |
            BITS(instr[0],12,13);

   switch(opcode) {

      case 0*8+0*4+0*32+0*2+0:
         trcins("tbit.z p%d,p%d = r%d,%d\n",p1,p2,r1,pos);
         if( p[pred]==0 ) return IA_OK;
         if( !(r[r1].dat.q >> pos & 1) ) {
            p[p1] = 1; p[p2] = 0;
         }
         else {
            p[p1] = 0; p[p2] = 1;
         }
         if( r[r1].nat ) p[p1] = p[p2] = 0;
         if(p1) add_hazard(p1 | PRD_REG);
         if(p2) add_hazard(p2 | PRD_REG);
         p[0] = 1; return IA_OK;
            
      case 0*8+0*4+0*32+0*2+1:
         trcins("tbit.z.unc p%d,p%d = r%d,%d\n",p1,p2,r1,pos);
         p[p1] = p[p2] = 0;
         if(p1) add_hazard(p1 | PRD_REG);
         if(p2) add_hazard(p2 | PRD_REG);
         if( p[pred]==0 ) {
             p[0] = 1; return IA_OK;
         }
         if( !(r[r1].dat.q >> pos & 1) ) {
            p[p1] = 1; p[p2] = 0;
         }
         else {
            p[p1] = 0; p[p2] = 1;
         }
         if( r[r1].nat ) p[p1] = p[p2] = 0;
         p[0] = 1; return IA_OK;
            
      case 0*8+0*4+1*32+0*2+0:
         trcins("tbit.z.and p%d,p%d = r%d,%d\n",p1,p2,r1,pos);
         if( p[pred]==0 ) return IA_OK;
         if( !(r[r1].dat.q >> pos & 1) || r[r1].nat ) {
            p[p1] = p[p2] = 0;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0*8+0*4+1*32+0*2+1:
         trcins("tbit.nz.and p%d,p%d = r%d,%d\n",p1,p2,r1,pos);
         if( p[pred]==0 ) return IA_OK;
         if( (r[r1].dat.q >> pos & 1) || r[r1].nat ) {
            p[p1] = p[p2] = 0;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0*8+1*4+0*32+0*2+0:
         trcins("tbit.z.or p%d,p%d = r%d,%d\n",p1,p2,r1,pos);
         if( p[pred]==0 ) return IA_OK;
         if( !(r[r1].dat.q >> pos & 1) || r[r1].nat ) {
            p[p1] = p[p2] = 1;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
                        
      case 0*8+1*4+0*32+0*2+1:
         trcins("tbit.nz.or p%d,p%d = r%d,%d\n",p1,p2,r1,pos);
         if( p[pred]==0 ) return IA_OK;
         if( (r[r1].dat.q >> pos & 1) || r[r1].nat ) {
            p[p1] = p[p2] = 1;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
                        
      case 0*8+1*4+1*32+0*2+0:
         trcins("tbit.z.or.andcm p%d,p%d = r%d,%d\n",p1,p2,r1,pos);
         if( p[pred]==0 ) return IA_OK;
         if( !(r[r1].dat.q >> pos & 1) || r[r1].nat ) {
            p[p1] = 1; p[p2] = 0;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0*8+1*4+1*32+0*2+1:
         trcins("tbit.nz.or.andcm p%d,p%d = r%d,%d\n",p1,p2,r1,pos);
         if( p[pred]==0 ) return IA_OK;
         if( !(r[r1].dat.q >> pos & 1) || r[r1].nat ) {
            p[p1] = 1; p[p2] = 0;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0*8+0*4+0*32+1*2+0:
         trcins("tnat.z p%d,p%d = r%d\n",p1,p2,r1);
         if( p[pred]==0 ) return IA_OK;
         if( !r[r1].nat ) {
            p[p1] = 1; p[p2] = 0;
         }
         else {
            p[p1] = 0; p[p2] = 1;
         }
         if(p1) add_hazard(p1 | PRD_REG);
         if(p2) add_hazard(p2 | PRD_REG);
         p[0] = 1; return IA_OK;
            
      case 0*8+0*4+0*32+1*2+1:
         trcins("tnat.z.unc p%d,p%d = r%d\n",p1,p2,r1);
         p[p1] = p[p2] = 0;
         if(p1) add_hazard(p1 | PRD_REG);
         if(p2) add_hazard(p2 | PRD_REG);
         if( p[pred]==0 ) {
             p[0] = 1; return IA_OK;
         }
         if( !r[r1].nat ) {
            p[p1] = 1; p[p2] = 0;
         }
         else {
            p[p1] = 0; p[p2] = 1;
         }
         p[0] = 1; return IA_OK;
            
      case 0*8+0*4+1*32+1*2+0:
         trcins("tnat.z.and p%d,p%d = r%d\n",p1,p2,r1);
         if( p[pred]==0 ) return IA_OK;
         if( !r[r1].nat ) {
            p[p1] = p[p2] = 0;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0*8+0*4+1*32+1*2+1:
         trcins("tnat.nz.and p%d,p%d = r%d\n",p1,p2,r1);
         if( p[pred]==0 ) return IA_OK;
         if( r[r1].nat ) {
            p[p1] = p[p2] = 0;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0*8+1*4+0*32+1*2+0:
         trcins("tnat.z.or p%d,p%d = r%d\n",p1,p2,r1);
         if( p[pred]==0 ) return IA_OK;
         if( !r[r1].nat ) {
            p[p1] = p[p2] = 1;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
                        
      case 0*8+1*4+0*32+1*2+1:
         trcins("tnat.nz.or p%d,p%d = r%d\n",p1,p2,r1);
         if( p[pred]==0 ) return IA_OK;
         if( r[r1].nat ) {
            p[p1] = p[p2] = 1;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
                        
      case 0*8+1*4+1*32+1*2+0:
         trcins("tnat.z.or.andcm p%d,p%d = r%d\n",p1,p2,r1);
         if( p[pred]==0 ) return IA_OK;
         if( !r[r1].nat ) {
            p[p1] = 1; p[p2] = 0;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
      case 0*8+1*4+1*32+1*2+1:
         trcins("tnat.nz.or.andcm p%d,p%d = r%d\n",p1,p2,r1);
         if( p[pred]==0 ) return IA_OK;
         if( r[r1].nat ) {
            p[p1] = 1; p[p2] = 0;
            if(p1) add_hazard(p1 | PRD_REG);
            if(p2) add_hazard(p2 | PRD_REG);
         }
         p[0] = 1; return IA_OK;
            
   }

   return ILLEGAL_INSTR;
}
