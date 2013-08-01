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
uint32 i_alu(qword *instr) { 

   uint32 rs1, rs2, rd;
   uint32 opcode;
   uint32 i, temp;

   rs2 = BITS(instr[0],20,26); /* also split with immediate field */ 
   rs1 = BITS(instr[0],13,19); /* also immediate field */
   rd = BITS(instr[0],6,12);

   if(rs2) check_reg(rs2 | ALU_REG);
   check_reg(rd | ALU_REG);
   add_hazard( rd | ALU_REG );
   opcode = BITS(instr[0],27,35);

   switch(opcode >> 6) { 

      case 2*2+0:
         r[rd].dat.q = (-(uint64)BITS(instr[0],36,36)) << 13;
         r[rd].dat.q |= (uint64)( rs1 | (opcode & 0x3f) << 7 );
         trcins("add r%d = %d, r%d\n", rd,LO(r[rd].dat),rs2 );
         r[rd].dat.q += r[rs2].dat.q;
         r[rd].nat = r[rs2].nat;
         return IA_OK;

      case 3*2+0:
         r[rd].dat.q = (-(uint64)BITS(instr[0],36,36)) << 13;
         r[rd].dat.q |= (uint64)( rs1 | (opcode & 0x3f) << 7 );
         trcins("addp4 r%d = %d, r%d\n", rd,LO(r[rd].dat),rs2 );
         r[rd].dat.q = r[rs1].dat.q + r[rs2].dat.q;
         HI(r[rd].dat) = (LO(r[rs2].dat) >> 30) << 29;
         r[rd].nat = r[rs2].nat;
         return IA_OK;
   }
   switch(opcode) { 

      case 0:
         trcins("add r%d = r%d, r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         r[rd].dat.q = r[rs1].dat.q + r[rs2].dat.q;
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1:
         trcins("add r%d = r%d, r%d, 1\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         r[rd].dat.q = r[rs1].dat.q + (uint64)(1) + r[rs2].dat.q;
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*4+1:
         trcins("sub r%d = r%d, r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         r[rd].dat.q = r[rs1].dat.q - r[rs2].dat.q;
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*4+0:
         trcins("sub r%d = r%d, r%d, 1\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         r[rd].dat.q = r[rs1].dat.q - (uint64)(1) - r[rs2].dat.q;
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 2*4+0:
         trcins("addp4 r%d = r%d, r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         r[rd].dat.q = r[rs1].dat.q + r[rs2].dat.q;
         HI(r[rd].dat) = (LO(r[rs2].dat) >> 30) << 29;
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 3*4+0:
         trcins("and r%d = r%d, r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         r[rd].dat.q = r[rs1].dat.q & r[rs2].dat.q;
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 3*4+1:
         trcins("andcm r%d = r%d, r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         r[rd].dat.q = r[rs1].dat.q & ~r[rs2].dat.q;
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 3*4+2:
         trcins("or r%d = r%d, r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         r[rd].dat.q = r[rs1].dat.q | r[rs2].dat.q;
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 3*4+3:
         trcins("xor r%d = r%d, r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         r[rd].dat.q = r[rs1].dat.q ^ r[rs2].dat.q;
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 4*4+0:
      case 4*4+1:
      case 4*4+2:
      case 4*4+3:
         trcins("shladd r%d = r%d, %d, r%d\n", rd,rs1,opcode&3,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         r[rd].dat.q = r[rs1].dat.q << (opcode & 3);
         r[rd].dat.q += r[rs2].dat.q;
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 6*4+0:
      case 6*4+1:
      case 6*4+2:
      case 6*4+3:
         trcins("shladdp4 r%d = r%d, %d, r%d\n", rd,rs1,opcode&3,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         r[rd].dat.q = r[rs1].dat.q << (opcode & 3);
         r[rd].dat.q += r[rs2].dat.q;
         HI(r[rd].dat) = (LO(r[rs2].dat) >> 30) << 29;
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+0*128+0*64+0*4+0:
         trcins("padd1 r%d = r%d, r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         for(i=0; i<8; i++)
            r[rd].dat.b[i] = r[rs1].dat.b[i] + r[rs2].dat.b[i];
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+0*128+1*64+0*4+0:
         trcins("padd2 r%d = r%d, r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         for(i=0; i<4; i++)
            r[rd].dat.w[i] = r[rs1].dat.w[i] + r[rs2].dat.w[i];
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+1*128+0*64+0*4+0:
         trcins("padd4 r%d = r%d, r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         for(i=0; i<2; i++)
            r[rd].dat.d[i] = r[rs1].dat.d[i] + r[rs2].dat.d[i];
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+0*128+0*64+0*4+1:
         trcins("padd1.sss r%d = r%d, r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         for(i=0; i<8; i++) {
            temp = (int32)r[rs1].dat.b[i] +
                   (int32)r[rs2].dat.b[i];
            if( (int32)temp > 127 )
               r[rd].dat.b[i] = (int8)(0x7f);
            else if( (int32)temp < -128 )
               r[rd].dat.b[i] = (int8)(0x80);
            else
               r[rd].dat.b[i] = (int8)temp;
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+0*128+1*64+0*4+1:
         trcins("padd2.sss r%d = r%d, r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         for(i=0; i<4; i++) {
            temp = (int32)r[rs1].dat.w[i] +
                   (int32)r[rs2].dat.w[i];
            if( (int32)temp > 32767 )
               r[rd].dat.w[i] = (int16)(0x7fff);
            else if( (int32)temp < -32768 )
               r[rd].dat.w[i] = (int16)(0x8000);
            else
               r[rd].dat.w[i] = (int16)temp;
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+0*128+0*64+0*4+2:
         trcins("padd1.uuu r%d = r%d, r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         for(i=0; i<8; i++) {
            temp = (uint32)r[rs1].dat.b[i] +
                   (uint32)r[rs2].dat.b[i];
            if( (uint32)temp > 255 )
               r[rd].dat.b[i] = (uint8)(0xff);
            else
               r[rd].dat.b[i] = (uint8)temp;
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+0*128+1*64+0*4+2:
         trcins("padd2.uuu r%d = r%d, r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         for(i=0; i<4; i++) {
            temp = (uint32)r[rs1].dat.w[i] +
                   (uint32)r[rs2].dat.w[i];
            if( (uint32)temp > 65535 )
               r[rd].dat.w[i] = (uint16)(0xffff);
            else
               r[rd].dat.w[i] = (uint16)temp;
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+0*128+0*64+0*4+3:
         trcins("padd1.uus r%d = r%d, r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         for(i=0; i<8; i++) {
            temp = (uint32)r[rs1].dat.b[i] +
                   (int32)r[rs2].dat.b[i];
            if( (int32)temp > 255 )
               r[rd].dat.b[i] = (uint8)(0xff);
            else if( (int32)temp < 0 )
               r[rd].dat.b[i] = (uint8)(0x00);
            else
               r[rd].dat.b[i] = (uint8)temp;
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+0*128+1*64+0*4+3:
         trcins("padd2.uus r%d = r%d, r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         for(i=0; i<4; i++) {
            temp = (uint32)r[rs1].dat.w[i] +
                   (int32)r[rs2].dat.w[i];
            if( (int32)temp > 65535 )
               r[rd].dat.w[i] = (uint16)(0xffff);
            else if( (int32)temp < 0 )
               r[rd].dat.w[i] = (uint16)(0x0000);
            else
               r[rd].dat.w[i] = (uint16)temp;
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+0*128+0*64+1*4+0:
         trcins("psub1 r%d = r%d, r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         for(i=0; i<8; i++)
            r[rd].dat.b[i] = r[rs1].dat.b[i] - r[rs2].dat.b[i];
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+0*128+1*64+1*4+0:
         trcins("psub2 r%d = r%d, r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         for(i=0; i<4; i++)
            r[rd].dat.w[i] = r[rs1].dat.w[i] - r[rs2].dat.w[i];
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+1*128+0*64+1*4+0:
         trcins("psub4 r%d = r%d, r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         for(i=0; i<2; i++)
            r[rd].dat.d[i] = r[rs1].dat.d[i] - r[rs2].dat.d[i];
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+0*128+0*64+1*4+1:
         trcins("psub1.sss r%d = r%d, r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         for(i=0; i<8; i++) {
            temp = (int32)r[rs1].dat.b[i] -
                   (int32)r[rs2].dat.b[i];
            if( (int32)temp > 127 )
               r[rd].dat.b[i] = (int8)(0x7f);
            else if( (int32)temp < -128 )
               r[rd].dat.b[i] = (int8)(0x80);
            else
               r[rd].dat.b[i] = (int8)temp;
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+0*128+1*64+1*4+1:
         trcins("psub2.sss r%d = r%d, r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         for(i=0; i<4; i++) {
            temp = (int32)r[rs1].dat.w[i] -
                   (int32)r[rs2].dat.w[i];
            if( (int32)temp > 32767 )
               r[rd].dat.w[i] = (int16)(0x7fff);
            else if( (int32)temp < -32768 )
               r[rd].dat.w[i] = (int16)(0x8000);
            else
               r[rd].dat.w[i] = (int16)temp;
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+0*128+0*64+1*4+2:
         trcins("psub1.uuu r%d = r%d, r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         for(i=0; i<8; i++) {
            temp = (uint32)r[rs1].dat.b[i] -
                   (uint32)r[rs2].dat.b[i];
            if( (int32)temp < 0 )
               r[rd].dat.b[i] = (uint8)(0x0);
            else
               r[rd].dat.b[i] = (uint8)temp;
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+0*128+1*64+1*4+2:
         trcins("psub2.uuu r%d = r%d, r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         for(i=0; i<4; i++) {
            temp = (uint32)r[rs1].dat.w[i] -
                   (uint32)r[rs2].dat.w[i];
            if( (int32)temp < 0 )
               r[rd].dat.w[i] = (uint16)(0x0);
            else
               r[rd].dat.w[i] = (uint16)temp;
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+0*128+0*64+1*4+3:
         trcins("psub1.uus r%d = r%d, r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         for(i=0; i<8; i++) {
            temp = (uint32)r[rs1].dat.b[i] -
                   (int32)r[rs2].dat.b[i];
            if( (int32)temp > 255 )
               r[rd].dat.b[i] = (uint8)(0xff);
            else if( (int32)temp < 0 )
               r[rd].dat.b[i] = (uint8)(0x00);
            else
               r[rd].dat.b[i] = (uint8)temp;
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+0*128+1*64+1*4+3:
         trcins("psub2.uus r%d = r%d, r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         for(i=0; i<4; i++) {
            temp = (uint32)r[rs1].dat.w[i] -
                   (int32)r[rs2].dat.w[i];
            if( (int32)temp > 65535 )
               r[rd].dat.w[i] = (uint16)(0xffff);
            else if( (int32)temp < 0 )
               r[rd].dat.w[i] = (uint16)(0x0000);
            else
               r[rd].dat.w[i] = (uint16)temp;
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+0*128+0*64+2*4+2:
         trcins("pavg1 r%d = r%d, r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         for(i=0; i<8; i++) {
            temp = (uint32)r[rs1].dat.b[i] +
                   (uint32)r[rs2].dat.b[i];
            r[rd].dat.b[i] = (uint8)((temp | (temp&1)<<1) >> 1);
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+0*128+1*64+2*4+2:
         trcins("pavg2 r%d = r%d, r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         for(i=0; i<4; i++) {
            temp = (uint32)r[rs1].dat.w[i] +
                   (uint32)r[rs2].dat.w[i];
            r[rd].dat.w[i] = (uint16)((temp | (temp&1)<<1) >> 1);
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+0*128+0*64+2*4+3:
         trcins("pavg1.raz r%d = r%d, r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         for(i=0; i<8; i++) {
            temp = (uint32)r[rs1].dat.b[i] +
                   (uint32)r[rs2].dat.b[i];
            r[rd].dat.b[i] = (uint8)((temp+1) >> 1);
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+0*128+1*64+2*4+3:
         trcins("pavg2.raz r%d = r%d, r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         for(i=0; i<4; i++) {
            temp = (uint32)r[rs1].dat.w[i] +
                   (uint32)r[rs2].dat.w[i];
            r[rd].dat.w[i] = (uint16)((temp+1) >> 1);
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;


      case 1*256+0*128+0*64+3*4+2:
         trcins("pavgsub1 r%d = r%d, r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         for(i=0; i<8; i++) {
            temp = (uint32)r[rs1].dat.b[i] -
                   (uint32)r[rs2].dat.b[i];
            r[rd].dat.b[i] = (uint8)((temp | (temp&1)<<1) >> 1);
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+0*128+1*64+3*4+2:
         trcins("pavgsub2 r%d = r%d, r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         for(i=0; i<4; i++) {
            temp = (uint32)r[rs1].dat.w[i] -
                   (uint32)r[rs2].dat.w[i];
            r[rd].dat.w[i] = (uint16)((temp | (temp&1)<<1) >> 1);
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+0*128+0*64+9*4+0:
         trcins("pcmp1.eq r%d = r%d, r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         for(i=0; i<8; i++) {
            if( (int8)r[rs1].dat.b[i]==
                (int8)r[rs2].dat.b[i] )
                r[rd].dat.b[i] = 0xff;
            else
                r[rd].dat.b[i] = 0;
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+0*128+1*64+9*4+0:
         trcins("pcmp2.eq r%d = r%d, r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         for(i=0; i<4; i++) {
            if( (int16)r[rs1].dat.w[i]==
                (int16)r[rs2].dat.w[i] )
                r[rd].dat.w[i] = 0xffff;
            else
                r[rd].dat.w[i] = 0;
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+1*128+0*64+9*4+0:
         trcins("pcmp4.eq r%d = r%d, r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         for(i=0; i<2; i++) {
            if( (int32)r[rs1].dat.d[i]==
                (int32)r[rs2].dat.d[i] )
                r[rd].dat.d[i] = 0xffffffff;
            else
                r[rd].dat.d[i] = 0;
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+0*128+0*64+9*4+1:
         trcins("pcmp1.gt r%d = r%d, r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         for(i=0; i<8; i++) {
            if( (int8)r[rs1].dat.b[i]>
                (int8)r[rs2].dat.b[i] )
                r[rd].dat.b[i] = 0xff;
            else
                r[rd].dat.b[i] = 0;
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+0*128+1*64+9*4+1:
         trcins("pcmp2.gt r%d = r%d, r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         for(i=0; i<4; i++) {
            if( (int16)r[rs1].dat.w[i]> 
                (int16)r[rs2].dat.w[i] )
                r[rd].dat.w[i] = 0xffff;
            else
                r[rd].dat.w[i] = 0;
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+1*128+0*64+9*4+1:
         trcins("pcmp4.gt r%d = r%d, r%d\n", rd,rs1,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         for(i=0; i<2; i++) {
            if( (int32)r[rs1].dat.d[i]> 
                (int32)r[rs2].dat.d[i] )
                r[rd].dat.d[i] = 0xffffffff;
            else
                r[rd].dat.d[i] = 0;
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+0*128+1*64+4*4+0:
      case 1*256+0*128+1*64+4*4+1:
      case 1*256+0*128+1*64+4*4+2:
      case 1*256+0*128+1*64+4*4+3:
         trcins("pshladd2 r%d = r%d, %d, r%d\n", rd,rs1,opcode&3,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         for(i=0; i<4; i++) {
            temp = (int32)r[rs1].dat.w[i] << (opcode&3);
            if( (int32)temp > 32767 )
               temp = (int32)(0x7fff);
            else if( (int32)temp < -32768 )
               temp = (int32)(0x8000);
            temp += (int32)r[rs2].dat.w[i];
            if( (int32)temp > 32767 )
               r[rd].dat.w[i] = (int16)(0x7fff);
            else if( (int32)temp < -32768 )
               r[rd].dat.w[i] = (int16)(0x8000);
            else
               r[rd].dat.w[i] = (int16)temp;
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 1*256+0*128+1*64+6*4+0:
      case 1*256+0*128+1*64+6*4+1:
      case 1*256+0*128+1*64+6*4+2:
      case 1*256+0*128+1*64+6*4+3:
         trcins("pshradd2 r%d = r%d, %d, r%d\n", rd,rs1,opcode&3,rs2 );
         if(rs1) check_reg(rs1 | ALU_REG);
         for(i=0; i<4; i++) {
            temp = (int32)r[rs1].dat.w[i] >> (opcode&3);
            temp += (int32)r[rs2].dat.w[i];
            if( (int32)temp > 32767 )
               r[rd].dat.w[i] = (int16)(0x7fff);
            else if( (int32)temp < -32768 )
               r[rd].dat.w[i] = (int16)(0x8000);
            else
               r[rd].dat.w[i] = (int16)temp;
         }
         r[rd].nat = r[rs1].nat | r[rs2].nat;
         return IA_OK;

      case 9*4+1:
         r[rd].dat.q = (-(uint64)BITS(instr[0],36,36)) << 7 | rs1;
         trcins("sub r%d = %d, r%d\n", rd,LO(r[rd].dat),rs2 );
         r[rd].dat.q -= r[rs2].dat.q;
         r[rd].nat = r[rs2].nat;
         return IA_OK;

      case 0xb*4+0:
         r[rd].dat.q = (-(uint64)BITS(instr[0],36,36)) << 7 | rs1;
         trcins("and r%d = %d, r%d\n", rd,LO(r[rd].dat),rs2 );
         r[rd].dat.q &= r[rs2].dat.q;
         r[rd].nat = r[rs2].nat;
         return IA_OK;

      case 0xb*4+1:
         r[rd].dat.q = (-(uint64)BITS(instr[0],36,36)) << 7 | rs1;
         trcins("andcm r%d = %d, r%d\n", rd,LO(r[rd].dat),rs2 );
         r[rd].dat.q &= ~r[rs2].dat.q;
         r[rd].nat = r[rs2].nat;
         return IA_OK;

      case 0xb*4+2:
         r[rd].dat.q = (-(uint64)BITS(instr[0],36,36)) << 7 | rs1;
         trcins("or r%d = %d, r%d\n", rd,LO(r[rd].dat),rs2 );
         r[rd].dat.q |= r[rs2].dat.q;
         r[rd].nat = r[rs2].nat;
         return IA_OK;

      case 0xb*4+3:
         r[rd].dat.q = (-(uint64)BITS(instr[0],36,36)) << 7 | rs1;
         trcins("xor r%d = %d, r%d\n", rd,LO(r[rd].dat),rs2 );
         r[rd].dat.q ^= r[rs2].dat.q;
         r[rd].nat = r[rs2].nat;
         return IA_OK;

   }


   return ILLEGAL_INSTR;
}



/*----------------------------------------------------------------------*/
uint32 i_addi22(qword *instr) {

   uint32 rs2, rd;
   uint32 opcode;

   rs2 = BITS(instr[0],20,21);
   rd = BITS(instr[0],6,12);

   check_reg(rs2 | ALU_REG);
   check_reg(rd | ALU_REG);

   add_hazard( rd | ALU_REG );

   r[rd].dat.q = (-(uint64)BITS(instr[0],36,36) << 21 |
                 BITS(instr[0],13,19) |
                 BITS(instr[0],22,35) << 7 ); 
   trcins("addl r%d = %d, r%d\n", rd,LO(r[rd].dat),rs2 );
   r[rd].dat.q += r[rs2].dat.q;
   r[rd].nat = r[rs2].nat;
   return IA_OK;
}

/*----------------------------------------------------------------------*/
uint32 i_misc(qword *instr) {

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
         iim = BITS(instr[0],6,25);
         faults |= BREAK_FAULT;           
         trcins("break.i %d\n", LO(imm));
         return IA_OK;
         
      case 0*64+1:
         imm.q = BITS(instr[0],36,36) << 20 |
                 BITS(instr[0],6,25);
         trcins("nop.i %d\n", LO(imm));
         return IA_OK;

      case 0*64+0x31:
         rs1 &= 7;
         trcins("mov r%d = b%d\n", rd,rs1);
         check_reg(rs1 | BR_REG);
         check_reg(rd | ALU_REG);
         add_hazard(rd | ALU_REG);
         r[rd].nat = 0;
         r[rd].dat.q = b[rs1].q;
         return IA_OK;

      case 0*64+0x30:   
         check_reg(rd | ALU_REG);
         add_hazard(rd | ALU_REG);
         trcins("mov r%d = ip\n", rd);
         r[rd].dat.q = ip.q;
         return IA_OK;

      case 0*64+0x33:   
         check_reg(rd | ALU_REG);
         add_hazard(rd | ALU_REG);
         trcins("mov r%d = pr\n", rd);
         for(i=63; i>=0; i--) {
            r[rd].dat.q <<= 1;
            check_reg(i | PRD_REG);
            r[rd].dat.q |= p[i];
         }
         r[rd].nat = 0;
         return IA_OK;

      case 0*64+0x2a:
         rs2 = BITS(instr[0],20,26);
         if(rs1) check_reg(rs1 | ALU_REG);
         check_reg(rs2 | APP_REG);
         add_hazard(rs2 | APP_REG);
         trcins("mov.i ar%d = r%d\n", rs2, rs1);
         if(r[rs1].nat) return NAT_CONSUMPTION;
         ar[rs2].q = r[rs1].dat.q;
         return IA_OK;

      case 0*64+0x0a:
         rs2 = BITS(instr[0],20,26);
         imm.q = -((int64)BITS(instr[0],36,36)) << 7 | rs1;
         trcins("mov.i ar%d = %d\n", rs2, LO(imm));
         check_reg(rs2 | APP_REG);
         add_hazard(rs2 | APP_REG);
         ar[rs2].q = imm.q;
         return IA_OK;

      case 0*64+0x32:
         rs2 = BITS(instr[0],20,26);
         trcins("mov.i r%d = ar%d\n", rd, rs2);
         check_reg(rd | ALU_REG);
         check_reg(rs2 | APP_REG);
         add_hazard(rd | ALU_REG);
         r[rd].dat.q = ar[rs2].q;
         r[rd].nat = 0;
         return IA_OK;

      case 0*64+0x10:
         rs2 = BITS(instr[0],20,26);
         trcins("zxt1 r%d = r%d\n", rd, rs2);
         check_reg(rd | ALU_REG);
         if(rs2) check_reg(rs2 | ALU_REG);
         add_hazard(rd | ALU_REG);
         r[rd].dat.q = (uint64)r[rs2].dat.b[0];
         r[rd].nat = r[rs2].nat;
         return IA_OK;

      case 0*64+0x11:
         rs2 = BITS(instr[0],20,26);
         trcins("zxt2 r%d = r%d\n", rd, rs2);
         check_reg(rd | ALU_REG);
         if(rs2) check_reg(rs2 | ALU_REG);
         add_hazard(rd | ALU_REG);
         r[rd].dat.q = (uint64)r[rs2].dat.w[0];
         r[rd].nat = r[rs2].nat;
         return IA_OK;

      case 0*64+0x12:
         rs2 = BITS(instr[0],20,26);
         trcins("zxt4 r%d = r%d\n", rd, rs2);
         check_reg(rd | ALU_REG);
         if(rs2) check_reg(rs2 | ALU_REG);
         add_hazard(rd | ALU_REG);
         r[rd].dat.q = (uint64)LO(r[rs2].dat);
         r[rd].nat = r[rs2].nat;
         return IA_OK;

      case 0*64+0x14:
         rs2 = BITS(instr[0],20,26);
         trcins("sxt1 r%d = r%d\n", rd, rs2);
         check_reg(rd | ALU_REG);
         if(rs2) check_reg(rs2 | ALU_REG);
         add_hazard(rd | ALU_REG);
         r[rd].dat.q = (int64)(r[rs2].dat.q << 56) >> 56;
         r[rd].nat = r[rs2].nat;
         return IA_OK;

      case 0*64+0x15:
         rs2 = BITS(instr[0],20,26);
         trcins("sxt2 r%d = r%d\n", rd, rs2);
         check_reg(rd | ALU_REG);
         if(rs2) check_reg(rs2 | ALU_REG);
         add_hazard(rd | ALU_REG);
         r[rd].dat.q = (int64)(r[rs2].dat.q << 48) >> 48;
         r[rd].nat = r[rs2].nat;
         return IA_OK;

      case 0*64+0x16:
         rs2 = BITS(instr[0],20,26);
         trcins("sxt4 r%d = r%d\n", rd, rs2);
         check_reg(rd | ALU_REG);
         if(rs2) check_reg(rs2 | ALU_REG);
         add_hazard(rd | ALU_REG);
         r[rd].dat.q = (int64)(r[rs2].dat.q << 32) >> 32;
         r[rd].nat = r[rs2].nat;
         return IA_OK;

      case 0*64+0x18:
         rs2 = BITS(instr[0],20,26);
         trcins("czx1.l r%d = r%d\n", rd, rs2);
         check_reg(rd | ALU_REG);
         if(rs2) check_reg(rs2 | ALU_REG);
         add_hazard(rd | ALU_REG);
         for(i=0; i<8; i++)
            if( !r[rs2].dat.b[7-i] ) break;
         r[rd].dat.q = i;
         r[rd].nat = r[rs2].nat;
         return IA_OK;

      case 0*64+0x19:
         rs2 = BITS(instr[0],20,26);
         trcins("czx2.l r%d = r%d\n", rd, rs2);
         check_reg(rd | ALU_REG);
         if(rs2) check_reg(rs2 | ALU_REG);
         add_hazard(rd | ALU_REG);
         for(i=0; i<4; i++)
            if( !r[rs2].dat.w[3-i] ) break;
         r[rd].dat.q = i;
         r[rd].nat = r[rs2].nat;
         return IA_OK;

      case 0*64+0x1c:
         rs2 = BITS(instr[0],20,26);
         trcins("czx1.r r%d = r%d\n", rd, rs2);
         check_reg(rd | ALU_REG);
         if(rs2) check_reg(rs2 | ALU_REG);
         add_hazard(rd | ALU_REG);
         for(i=0; i<8; i++)
            if( !r[rs2].dat.b[i] ) break;
         r[rd].dat.q = i;
         r[rd].nat = r[rs2].nat;
         return IA_OK;

      case 0*64+0x1d:
         rs2 = BITS(instr[0],20,26);
         trcins("czx2.r r%d = r%d\n", rd, rs2);
         check_reg(rd | ALU_REG);
         if(rs2) check_reg(rs2 | ALU_REG);
         add_hazard(rd | ALU_REG);
         for(i=0; i<4; i++)
            if( !r[rs2].dat.w[i] ) break;
         r[rd].dat.q = i;
         r[rd].nat = r[rs2].nat;
         return IA_OK;

      case 1*64+0:
         imm.q = -((int64)BITS(instr[0],36,36)) << 24 |
              BITS(instr[0],20,32) << 11 | rd << 4;
         trcins("chk.s.i r%d, %d\n", rs1,LO(imm));
         rs1 = BITS(instr[0],13,19);
         check_reg(rs1 | ALU_REG);
         if( r[rs1].nat ) {
            ip.q += imm.q;
            add_hazard(PC_REG);
         }
         return IA_OK;

      case 7*64+0:
         rd &= 7;
         if(rs1) check_reg(rs1 | ALU_REG);
         check_reg(rd | BR_REG);
         add_hazard(rd | BR_REG);
         if( !BITS(instr[0],22,22) ) {
            trcins("mov b%d = r%d\n", rd,rs1);
            if(r[rs1].nat) return NAT_CONSUMPTION;
            b[rd].q = r[rs1].dat.q;
            return IA_OK;
         }
         else {
            trcins("mov.ret b%d = r%d\n", rd,rs1);
            if(r[rs1].nat) return NAT_CONSUMPTION;
            b[rd].q = r[rs1].dat.q;
            return IA_OK;
         }

      case 3*64+0:
         imm.q = -((int64)BITS(instr[0],36,36)) << 16 |
                    BITS(instr[0],24,31) << 8 | rd << 1;
         mask.q = imm.q;
         trcins("mov pr = r%d,%d\n", rs1, LO(imm));
         if(rs1) check_reg(rs1 | ALU_REG);
         if(r[rs1].nat) return NAT_CONSUMPTION;
         imm.q &= r[rs1].dat.q;
         for(i=0; i<64; i++) {
            if( LO(mask) & 1) {
               check_reg(i | PRD_REG);
               p[i] = LO(imm) & 1;
               add_hazard(i | PRD_REG);
            }
            imm.q >>= 1;
            mask.q >>= 1;
         }
         return IA_OK;

      case 2*64+0:
         imm.q = -((int64)BITS(instr[0],36,36)) << 43 |
                    (uint64)BITS(instr[0],6,32) << 16;
         trcins("mov pr.rot = 0x%08x%08x\n", HI(imm),LO(imm));
	 imm.q >>= 16;
         for(i=16; i<64; i++) {
            check_reg(i | PRD_REG);
            add_hazard(i | PRD_REG);
            p[i] = LO(imm) & 1;
            imm.q >>= 1;
         }
         return IA_OK;

   }

   return ILLEGAL_INSTR;
}
