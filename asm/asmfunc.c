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
#include "ia64asm.h"
#include "asmfunc.h"

/*----------------------------------------------------------------------*/   
uint32 add_r_i22_r(uint32 *unit, qword *out) {
   if( op.i[0]<8192 && op.i[0]>=-8192 )
      return adds_r_i14_r(unit,out);
   else {
      if(op.r[1]>=4) return PARSE_ERROR;
      return addl_r_i22_r2(unit,out);
   }   
}
/*----------------------------------------------------------------------*/   
uint32 add_r_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT+1;
   out->q = (uint64)8<<37 | 0<<27 | op.r[0]<<6 | op.r[1]<<13
                    | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 add_r_r_r_1(uint32 *unit, qword *out) {
   *unit = IMUNIT+1;
   out->q = (uint64)8<<37 | 1<<27 | op.r[0]<<6 | op.r[1]<<13
                    | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 addp4_r_i14_r(uint32 *unit, qword *out) {
   *unit = IMUNIT+9;
   out->q = (uint64)8<<37 | (uint64)(3*2+0)<<33 | op.r[0]<<6 | (op.i[0]&127)<<13
                    | ((op.i[0]>>13)&1)<<36 | op.r[2]<<20 | op.pred
                    | ((op.i[0]>>7)&0x3f)<<27;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 addp4_r_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT+9;
   out->q = (uint64)8<<37 | (uint64)(2*4+0)<<27 | op.r[0]<<6 | op.r[1]<<13
                    | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 adds_r_i14_r(uint32 *unit, qword *out) {
   *unit = IMUNIT+4;
   out->q = (uint64)8<<37 | (uint64)(2*2+0)<<33 | op.r[0]<<6 | (op.i[0]&127)<<13
                    | ((op.i[0]>>13)&1)<<36 | op.r[1]<<20 | op.pred
                    | ((op.i[0]>>7)&0x3f)<<27;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 addl_r_i22_r2(uint32 *unit, qword *out) {
   *unit = IMUNIT+5;
   out->q = (uint64)9<<37 | op.r[0]<<6 | ((op.i[0]>>7)&0x7fff)<<22
                    | (op.i[0]&127)<<13 | (op.r[2]&3)<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 alloc_r_ar_pfs_u7_u7_u7_u7(uint32 *unit, qword *out) {
   uint32 sof,sol,sor;

   sof = op.i[0]+op.i[1]+op.i[2];
   sol = sof - op.i[2];
   sor = op.i[3] >> 3;

   if(sof>=sol && sof>=sor && sol>=sor) {
      *unit = FIRST+MUNIT+34;
      out->q = (uint64)(1*16+6)<<33 | op.r[0]<<6 | sof<<13
         | sol<<20 | (uint64)sor<<26;
      return IA_OK;
   }
   else {
      return PARSE_ERROR;
   }
}
/*-----------------------------------------------------------------------*/
uint32 and_r_i8_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (uint64)(0xb*4+0)<<27 | op.r[0]<<6 | 
               (op.i[0]&127)<<13 | ((op.i[0]>>7)&1)<<36 | 
          op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 and_r_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (3*4+0)<<27 | op.r[0]<<6 | op.r[1]<<13
                    | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 andcm_r_i8_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (uint64)(0xb*4+1)<<27 | op.r[0]<<6 | 
             (op.i[0]&127)<<13 | ((op.i[0]>>7)&1)<<36 | 
        op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 andcm_r_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (uint64)(3*4+1)<<27 | op.r[0]<<6 | op.r[1]<<13
                    | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 br_b(uint32 *unit, qword *out) {
   op.pred = 0;
   return br_cond_sptk_b(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 br_call_dpnt_b_b(uint32 *unit, qword *out) {
   *unit = BUNIT; 
   out->q = (uint64)1<<37 | (uint64)(2*DPNT+1)<<32 | op.pred
              | op.b[0]<<6 | op.b[1]<<13;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 br_call_dpnt_b_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)5<<37 | (uint64)DPNT<<33 | op.pred
              | (op.i[0]&0xfffff)<<13 | op.b[0]<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_dpnt_clr_b_b(uint32 *unit, qword *out) {
   *unit = BUNIT; 
   out->q = (uint64)1<<37 | (uint64)(2*DPNT+1)<<32 | op.pred
              | (uint64)CLR<<35 | op.b[0]<<6 | op.b[1]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_dpnt_clr_b_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)5<<37 | (uint64)DPNT<<33 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | op.b[0]<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_dpnt_few_b_b(uint32 *unit, qword *out) {
   *unit = BUNIT; 
   out->q = (uint64)1<<37 | (uint64)(2*DPNT+1)<<32 | op.pred | FEW<<6
               | op.b[0]<<6 | op.b[1]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_dpnt_few_b_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)5<<37 | (uint64)DPNT<<33 | FEW<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | op.b[0]<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_dpnt_few_clr_b_b(uint32 *unit, qword *out) {
   *unit = BUNIT; 
   out->q = (uint64)1<<37 | (uint64)(2*DPNT+1)<<32 | op.pred | FEW<<6
              | (uint64)CLR<<35 | op.b[0]<<6 | op.b[1]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_dpnt_few_clr_b_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)5<<37 | (uint64)DPNT<<33 | FEW<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | op.b[0]<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_dpnt_many_b_b(uint32 *unit, qword *out) {
   *unit = BUNIT; 
   out->q = (uint64)1<<37 | (uint64)(2*DPNT+1)<<32 | op.pred | MANY<<6
              | op.b[0]<<6 | op.b[1]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_dpnt_many_b_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)5<<37 | (uint64)DPNT<<33 | MANY<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | op.b[0]<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_dpnt_many_clr_b_b(uint32 *unit, qword *out) {
   *unit = BUNIT; 
   out->q = (uint64)1<<37 | (uint64)(2*DPNT+1)<<32 | op.pred | MANY<<6
              | (uint64)CLR<<35 | op.b[0]<<6 | op.b[1]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_dpnt_many_clr_b_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)5<<37 | (uint64)DPNT<<33 | MANY<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | op.b[0]<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_dptk_b_b(uint32 *unit, qword *out) {
   *unit = BUNIT; 
   out->q = (uint64)1<<37 | (uint64)(2*DPTK+1)<<32 | op.pred 
              | op.b[0]<<6 | op.b[1]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_dptk_b_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)5<<37 | (uint64)DPTK<<33 | op.pred
              | (op.i[0]&0xfffff)<<13 | op.b[0]<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_dptk_clr_b_b(uint32 *unit, qword *out) {
   *unit = BUNIT; 
   out->q = (uint64)1<<37 | (uint64)(2*DPTK+1)<<32 | op.pred
              | (uint64)CLR<<35 | op.b[0]<<6 | op.b[1]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_dptk_clr_b_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)5<<37 | (uint64)DPTK<<33 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | op.b[0]<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_dptk_few_b_b(uint32 *unit, qword *out) {
   *unit = BUNIT; 
   out->q = (uint64)1<<37 | (uint64)(2*DPTK+1)<<32 | op.pred | FEW<<6
              | op.b[0]<<6 | op.b[1]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_dptk_few_b_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)5<<37 | (uint64)DPTK<<33 | FEW<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | op.b[0]<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_dptk_few_clr_b_b(uint32 *unit, qword *out) {
   *unit = BUNIT; 
   out->q = (uint64)1<<37 | (uint64)(2*DPTK+1)<<32 | op.pred | FEW<<6
              | (uint64)CLR<<35 | op.b[0]<<6 | op.b[1]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_dptk_few_clr_b_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)5<<37 | (uint64)DPTK<<33 | FEW<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | op.b[0]<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_dptk_many_b_b(uint32 *unit, qword *out) {
   *unit = BUNIT; 
   out->q = (uint64)1<<37 | (uint64)(2*DPTK+1)<<32 | op.pred | MANY<<6
              | op.b[0]<<6 | op.b[1]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_dptk_many_b_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)5<<37 | (uint64)DPTK<<33 | MANY<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | op.b[0]<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_dptk_many_clr_b_b(uint32 *unit, qword *out) {
   *unit = BUNIT; 
   out->q = (uint64)1<<37 | (uint64)(2*DPTK+1)<<32 | op.pred | MANY<<6
              | (uint64)CLR<<35 | op.b[0]<<6 | op.b[1]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_dptk_many_clr_b_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)5<<37 | (uint64)DPTK<<33 | MANY<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | op.b[0]<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_spnt_b_b(uint32 *unit, qword *out) {
   *unit = BUNIT; 
   out->q = (uint64)1<<37 | (uint64)(2*SPNT+1)<<32 | op.pred 
              | op.b[0]<<6 | op.b[1]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_spnt_b_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)5<<37 | (uint64)SPNT<<33 | op.pred
              | (op.i[0]&0xfffff)<<13 | op.b[0]<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_spnt_clr_b_b(uint32 *unit, qword *out) {
   *unit = BUNIT; 
   out->q = (uint64)1<<37 | (uint64)(2*SPNT+1)<<32 | op.pred 
              | (uint64)CLR<<35 | op.b[0]<<6 | op.b[1]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_spnt_clr_b_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)5<<37 | (uint64)SPNT<<33 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | op.b[0]<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_spnt_few_b_b(uint32 *unit, qword *out) {
   *unit = BUNIT; 
   out->q = (uint64)1<<37 | (uint64)(2*SPNT+1)<<32 | op.pred | FEW<<6
               | op.b[0]<<6 | op.b[1]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_spnt_few_b_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)5<<37 | (uint64)SPNT<<33 | FEW<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | op.b[0]<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_spnt_few_clr_b_b(uint32 *unit, qword *out) {
   *unit = BUNIT; 
   out->q = (uint64)1<<37 | (uint64)(2*SPNT+1)<<32 | op.pred | FEW<<6
              | (uint64)CLR<<35 | op.b[0]<<6 | op.b[1]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_spnt_few_clr_b_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)5<<37 | (uint64)SPNT<<33 | FEW<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | op.b[0]<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_spnt_many_b_b(uint32 *unit, qword *out) {
   *unit = BUNIT; 
   out->q = (uint64)1<<37 | (uint64)(2*SPNT+1)<<32 | op.pred | MANY<<6
              | op.b[0]<<6 | op.b[1]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_spnt_many_b_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)5<<37 | (uint64)SPNT<<33 | MANY<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | op.b[0]<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_spnt_many_clr_b_b(uint32 *unit, qword *out) {
   *unit = BUNIT; 
   out->q = (uint64)1<<37 | (uint64)(2*SPNT+1)<<32 | op.pred | MANY<<6
              | (uint64)CLR<<35 | op.b[0]<<6 | op.b[1]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_spnt_many_clr_b_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)5<<37 | (uint64)SPNT<<33 | MANY<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | op.b[0]<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_sptk_b_b(uint32 *unit, qword *out) {
   *unit = BUNIT; 
   out->q = (uint64)1<<37 | (uint64)(2*SPTK+1)<<32 | op.pred
               | op.b[0]<<6 | op.b[1]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_sptk_b_i25(uint32 *unit, qword *out) {
   *unit = BUNIT+IPREL+3; op.i[0] >>= 4;
   out->q = (uint64)5<<37 | (uint64)SPTK<<33 | op.pred
              | (op.i[0]&0xfffff)<<13 | op.b[0]<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_sptk_clr_b_b(uint32 *unit, qword *out) {
   *unit = BUNIT; 
   out->q = (uint64)1<<37 | (uint64)(2*SPTK+1)<<32 | op.pred 
              | (uint64)CLR<<35 | op.b[0]<<6 | op.b[1]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_sptk_clr_b_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)5<<37 | (uint64)SPTK<<33 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | op.b[0]<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_sptk_few_b_b(uint32 *unit, qword *out) {
   *unit = BUNIT; 
   out->q = (uint64)1<<37 | (uint64)(2*SPTK+1)<<32 | op.pred | FEW<<6
              | op.b[0]<<6 | op.b[1]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_sptk_few_b_i25(uint32 *unit, qword *out) {
   *unit = BUNIT+IPREL+3; op.i[0] >>= 4;
   out->q = (uint64)5<<37 | (uint64)SPTK<<33 | FEW<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | op.b[0]<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_sptk_few_clr_b_b(uint32 *unit, qword *out) {
   *unit = BUNIT; 
   out->q = (uint64)1<<37 | (uint64)(2*SPTK+1)<<32 | op.pred | FEW<<6
              | (uint64)CLR<<35 | op.b[0]<<6 | op.b[1]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_sptk_few_clr_b_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)5<<37 | (uint64)SPTK<<33 | FEW<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | op.b[0]<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_sptk_many_b_b(uint32 *unit, qword *out) {
   *unit = BUNIT; 
   out->q = (uint64)1<<37 | (uint64)(2*SPTK+1)<<32 | op.pred | MANY<<6
              | op.b[0]<<6 | op.b[1]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_sptk_many_b_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)5<<37 | (uint64)SPTK<<33 | MANY<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | op.b[0]<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_sptk_many_clr_b_b(uint32 *unit, qword *out) {
   *unit = BUNIT; 
   out->q = (uint64)1<<37 | (uint64)(2*SPTK+1)<<32 | op.pred | MANY<<6
              | (uint64)CLR<<35 | op.b[0]<<6 | op.b[1]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_call_sptk_many_clr_b_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)5<<37 | (uint64)SPTK<<33 | MANY<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | op.b[0]<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cexit_dpnt_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPNT<<33 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 6<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cexit_dpnt_few_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPNT<<33 | FEW<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 6<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cexit_dpnt_few_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPNT<<33 | FEW<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 6<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cexit_dpnt_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPNT<<33 | op.pred
              | (op.i[0]&0xfffff)<<13 | 6<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cexit_dpnt_many_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPNT<<33 | MANY<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 6<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cexit_dpnt_many_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPNT<<33 | MANY<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 6<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cexit_dptk_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPTK<<33 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 6<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cexit_dptk_few_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPTK<<33 | FEW<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 6<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cexit_dptk_few_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPTK<<33 | FEW<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 6<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cexit_dptk_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPTK<<33 | op.pred
              | (op.i[0]&0xfffff)<<13 | 6<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cexit_dptk_many_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPTK<<33 | MANY<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 6<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cexit_dptk_many_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPTK<<33 | MANY<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 6<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cexit_spnt_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPNT<<33 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 6<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cexit_spnt_few_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPNT<<33 | FEW<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 6<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cexit_spnt_few_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPNT<<33 | FEW<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 6<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cexit_spnt_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPNT<<33 | op.pred
              | (op.i[0]&0xfffff)<<13 | 6<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cexit_spnt_many_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPNT<<33 | MANY<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 6<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cexit_spnt_many_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPNT<<33 | MANY<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 6<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cexit_sptk_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPTK<<33 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 6<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cexit_sptk_few_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPTK<<33 | FEW<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 6<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cexit_sptk_few_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPTK<<33 | FEW<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 6<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cexit_sptk_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPTK<<33 | op.pred
              | (op.i[0]&0xfffff)<<13 | 6<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cexit_sptk_many_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPTK<<33 | MANY<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 6<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cexit_sptk_many_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPTK<<33 | MANY<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 6<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cloop_dpnt_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPNT<<33 | FEW<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 5<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cloop_dpnt_few_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPNT<<33 | FEW<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 5<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cloop_dpnt_few_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPNT<<33 | FEW<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 5<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cloop_dpnt_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPNT<<33 | op.pred
              | (op.i[0]&0xfffff)<<13 | 5<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cloop_dpnt_many_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPNT<<33 | MANY<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 5<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cloop_dpnt_many_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPNT<<33 | MANY<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 5<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cloop_dptk_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPTK<<33 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 5<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cloop_dptk_few_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPTK<<33 | FEW<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 5<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cloop_dptk_few_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPTK<<33 | FEW<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 5<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cloop_dptk_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPTK<<33 | op.pred
              | (op.i[0]&0xfffff)<<13 | 5<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cloop_dptk_many_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPTK<<33 | MANY<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 5<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cloop_dptk_many_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPTK<<33 | MANY<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 5<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cloop_spnt_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPNT<<33 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 5<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cloop_spnt_few_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPNT<<33 | FEW<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 5<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cloop_spnt_few_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPNT<<33 | FEW<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 5<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cloop_spnt_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPNT<<33 | op.pred
              | (op.i[0]&0xfffff)<<13 | 5<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cloop_spnt_many_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPNT<<33 | MANY<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 5<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cloop_spnt_many_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPNT<<33 | MANY<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 5<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cloop_sptk_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPTK<<33 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 5<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cloop_sptk_few_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPTK<<33 | FEW<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 5<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cloop_sptk_few_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPTK<<33 | FEW<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 5<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cloop_sptk_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPTK<<33 | op.pred
              | (op.i[0]&0xfffff)<<13 | 5<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cloop_sptk_many_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPTK<<33 | MANY<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 5<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cloop_sptk_many_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPTK<<33 | MANY<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 5<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_clr_b(uint32 *unit, qword *out) {
   op.pred = 0;
   return br_cond_sptk_clr_b(unit, out);
}
/*---------------------------------------------------------------------------*/
uint32 br_clr_i25(uint32 *unit, qword *out) {
   op.pred = 0;
   return br_cond_sptk_clr_i25(unit, out);
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_dpnt_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x20<<27 | (uint64)DPNT<<33 | op.pred
              | 0<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_dpnt_clr_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x20<<27 | (uint64)DPNT<<33 | op.pred
              | (uint64)CLR<<35 | 0<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_dpnt_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPNT<<33 | FEW<<12 | op.pred
              | (uint64)CLR<<35 | 0<<6 | (op.i[0]&0xfffff)<<13 
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_dpnt_few_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x20<<27 | (uint64)DPNT<<33 | op.pred | FEW<<12
              | 0<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_dpnt_few_clr_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x20<<27 | (uint64)DPNT<<33 | op.pred | FEW<<12
              | (uint64)CLR<<35 | 0<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_dpnt_few_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPNT<<33 | FEW<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 0<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_dpnt_few_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPNT<<33 | FEW<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 0<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_dpnt_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPNT<<33 | op.pred
              | (op.i[0]&0xfffff)<<13 | 0<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_dpnt_many_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x20<<27 | (uint64)DPNT<<33 | op.pred | MANY<<12
              | 0<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_dpnt_many_clr_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x20<<27 | (uint64)DPNT<<33 | op.pred | MANY<<12
              | (uint64)CLR<<35 | 0<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_dpnt_many_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPNT<<33 | MANY<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 0<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_dpnt_many_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPNT<<33 | MANY<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 0<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_dptk_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x20<<27 | (uint64)DPTK<<33 | op.pred
              | 0<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_dptk_clr_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x20<<27 | (uint64)DPTK<<33 | op.pred 
              | (uint64)CLR<<35 | 0<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_dptk_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPTK<<33 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 0<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_dptk_few_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x20<<27 | (uint64)DPTK<<33 | op.pred | FEW<<12
               | 0<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_dptk_few_clr_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x20<<27 | (uint64)DPTK<<33 | op.pred | FEW<<12
              | (uint64)CLR<<35 | 0<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_dptk_few_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPTK<<33 | FEW<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 0<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_dptk_few_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPTK<<33 | FEW<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 0<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_dptk_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPTK<<33 | op.pred
              | (op.i[0]&0xfffff)<<13 | 0<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_dptk_many_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x20<<27 | (uint64)DPNT<<33 | op.pred | MANY<<12
               | 0<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_dptk_many_clr_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x20<<27 | (uint64)DPNT<<33 | op.pred | MANY<<12
              | (uint64)CLR<<35 | 0<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_dptk_many_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPTK<<33 | MANY<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 0<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_dptk_many_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPTK<<33 | MANY<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 0<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_spnt_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x20<<27 | (uint64)SPNT<<33 | op.pred 
               | 0<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_spnt_clr_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x20<<27 | (uint64)SPNT<<33 | op.pred 
              | (uint64)CLR<<35 | 0<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_spnt_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPNT<<33 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 0<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_spnt_few_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x20<<27 | (uint64)SPNT<<33 | op.pred | FEW<<12
              | 0<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_spnt_few_clr_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x20<<27 | (uint64)SPNT<<33 | op.pred | FEW<<12
              | (uint64)CLR<<35 | 0<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_spnt_few_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPNT<<33 | FEW<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 0<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_spnt_few_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPNT<<33 | FEW<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 0<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_spnt_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPNT<<33 | op.pred
              | (op.i[0]&0xfffff)<<13 | 0<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_spnt_many_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x20<<27 | (uint64)SPNT<<33 | op.pred | MANY<<12
              | 0<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_spnt_many_clr_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x20<<27 | (uint64)SPNT<<33 | op.pred | MANY<<12
              | (uint64)CLR<<35 | 0<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_spnt_many_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPNT<<33 | MANY<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 0<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_spnt_many_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPNT<<33 | MANY<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 0<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_sptk_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x20<<27 | (uint64)SPTK<<33 | op.pred 
               | 0<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_sptk_clr_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x20<<27 | (uint64)SPTK<<33 | op.pred
              | (uint64)CLR<<35 | 0<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_sptk_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPTK<<33 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 0<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_sptk_few_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x20<<27 | (uint64)SPTK<<33 | op.pred | FEW<<12
              | 0<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_sptk_few_clr_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x20<<27 | (uint64)SPTK<<33 | op.pred | FEW<<12
              | (uint64)CLR<<35 | 0<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_sptk_few_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPTK<<33 | FEW<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 0<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_sptk_few_i25(uint32 *unit, qword *out) {
   *unit = BUNIT+IPREL+1; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPTK<<33 | FEW<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 0<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_sptk_i25(uint32 *unit, qword *out) {
   *unit = BUNIT+1; op.i[0] -= seg_tbl[0].seg_length; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPTK<<33 | op.pred
              | (op.i[0]&0xfffff)<<13 | 0<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_sptk_many_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x20<<27 | (uint64)SPTK<<33 | op.pred | MANY<<12
              | (uint64)CLR<<35 | 0<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_sptk_many_clr_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x20<<27 | (uint64)SPTK<<33 | op.pred | MANY<<12
              | (uint64)CLR<<35 | 0<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_sptk_many_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPTK<<33 | MANY<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 0<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_cond_sptk_many_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPTK<<33 | MANY<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 0<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ctop_dpnt_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPNT<<33 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 7<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ctop_dpnt_few_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPNT<<33 | FEW<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 7<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ctop_dpnt_few_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPNT<<33 | FEW<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 7<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ctop_dpnt_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPNT<<33 | op.pred
              | (op.i[0]&0xfffff)<<13 | 7<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ctop_dpnt_many_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPNT<<33 | MANY<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 7<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ctop_dpnt_many_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPNT<<33 | MANY<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 7<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ctop_dptk_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPNT<<33 | MANY<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 7<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ctop_dptk_few_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPTK<<33 | FEW<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 7<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ctop_dptk_few_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPTK<<33 | FEW<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 7<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ctop_dptk_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPTK<<33 | op.pred
              | (op.i[0]&0xfffff)<<13 | 7<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ctop_dptk_many_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPTK<<33 | MANY<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 7<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ctop_dptk_many_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPTK<<33 | MANY<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 7<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ctop_spnt_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPNT<<33 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 7<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ctop_spnt_few_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPNT<<33 | FEW<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 7<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ctop_spnt_few_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPNT<<33 | FEW<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 7<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ctop_spnt_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPNT<<33 | op.pred
              | (op.i[0]&0xfffff)<<13 | 7<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ctop_spnt_many_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPNT<<33 | MANY<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 7<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ctop_spnt_many_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPNT<<33 | MANY<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 7<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ctop_sptk_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPTK<<33 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 7<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ctop_sptk_few_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPTK<<33 | FEW<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 7<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ctop_sptk_few_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPTK<<33 | FEW<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 7<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ctop_sptk_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPTK<<33 | op.pred
              | (op.i[0]&0xfffff)<<13 | 7<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ctop_sptk_many_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPTK<<33 | MANY<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 7<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ctop_sptk_many_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPTK<<33 | MANY<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 0<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_few_b(uint32 *unit, qword *out) {
   op.pred = 0;
   return br_cond_sptk_few_b(unit, out);
}
/*---------------------------------------------------------------------------*/
uint32 br_few_clr_b(uint32 *unit, qword *out) {
   op.pred = 0;
   return br_cond_sptk_few_clr_b(unit, out);
}
/*---------------------------------------------------------------------------*/
uint32 br_few_clr_i25(uint32 *unit, qword *out) {
   op.pred = 0;
   return br_cond_sptk_few_clr_i25(unit, out);
}
/*---------------------------------------------------------------------------*/
uint32 br_few_i25(uint32 *unit, qword *out) {
   op.pred = 0;
   return br_cond_sptk_few_clr_i25(unit, out);
}
/*---------------------------------------------------------------------------*/
uint32 br_i25(uint32 *unit, qword *out) {
   op.pred = 0;
   return br_cond_sptk_i25(unit, out);
}
/*---------------------------------------------------------------------------*/
uint32 br_ia_dpnt_b(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 br_ia_dpnt_clr_b(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 br_ia_dpnt_few_b(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 br_ia_dpnt_few_clr_b(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 br_ia_dpnt_many_b(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 br_ia_dpnt_many_clr_b(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 br_ia_dptk_b(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 br_ia_dptk_clr_b(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 br_ia_dptk_few_b(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 br_ia_dptk_few_clr_b(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 br_ia_dptk_many_b(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 br_ia_dptk_many_clr_b(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 br_ia_spnt_b(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 br_ia_spnt_clr_b(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 br_ia_spnt_few_b(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 br_ia_spnt_few_clr_b(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 br_ia_spnt_many_b(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 br_ia_spnt_many_clr_b(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 br_ia_sptk_b(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 br_ia_sptk_clr_b(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 br_ia_sptk_few_b(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 br_ia_sptk_few_clr_b(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 br_ia_sptk_many_b(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 br_ia_sptk_many_clr_b(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*---------------------------------------------------------------------------*/
uint32 br_many_b(uint32 *unit, qword *out) {
   op.pred = 0;
   return br_cond_sptk_many_b(unit, out);
}
/*---------------------------------------------------------------------------*/
uint32 br_many_clr_b(uint32 *unit, qword *out) {
   op.pred = 0;
   return br_cond_sptk_many_clr_b(unit, out);
}
/*---------------------------------------------------------------------------*/
uint32 br_many_clr_i25(uint32 *unit, qword *out) {
   op.pred = 0;
   return br_cond_sptk_many_clr_i25(unit, out);
}
/*---------------------------------------------------------------------------*/
uint32 br_many_i25(uint32 *unit, qword *out) {
   op.pred = 0;
   return br_cond_sptk_many_i25(unit, out);
}
/*---------------------------------------------------------------------------*/
uint32 br_ret_dpnt_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x21<<27 | (uint64)DPNT<<33 | op.pred 
              | 4<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ret_dpnt_clr_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x21<<27 | (uint64)DPNT<<33 | op.pred 
              | (uint64)CLR<<35 | 4<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ret_dpnt_few_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x21<<27 | (uint64)DPNT<<33 | op.pred | FEW<<12
              | 4<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ret_dpnt_few_clr_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x21<<27 | (uint64)DPNT<<33 | op.pred | FEW<<12
              | (uint64)CLR<<35 | 4<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ret_dpnt_many_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x21<<27 | (uint64)DPNT<<33 | op.pred | MANY<<12
              | 4<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ret_dpnt_many_clr_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x21<<27 | (uint64)DPNT<<33 | op.pred | MANY<<12
              | (uint64)CLR<<35 | 4<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ret_dptk_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x21<<27 | (uint64)DPTK<<33 | op.pred
             | 4<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ret_dptk_clr_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x21<<27 | (uint64)DPTK<<33 | op.pred
              | (uint64)CLR<<35 | 4<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ret_dptk_few_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x21<<27 | (uint64)DPTK<<33 | op.pred | FEW<<12
              | 4<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ret_dptk_few_clr_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x21<<27 | (uint64)DPTK<<33 | op.pred | FEW<<12
              | (uint64)CLR<<35 | 4<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ret_dptk_many_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x21<<27 | (uint64)DPTK<<33 | op.pred | MANY<<12
              | 4<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ret_dptk_many_clr_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x21<<27 | (uint64)DPTK<<33 | op.pred | MANY<<12
              | 4<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ret_spnt_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x21<<27 | (uint64)SPNT<<33 | op.pred 
              | 4<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ret_spnt_clr_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x21<<27 | (uint64)SPNT<<33 | op.pred 
              | (uint64)CLR<<35 | 4<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ret_spnt_few_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x21<<27 | (uint64)SPNT<<33 | op.pred | FEW<<12
               | 4<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ret_spnt_few_clr_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x21<<27 | (uint64)SPNT<<33 | op.pred | FEW<<12
              | (uint64)CLR<<35 | 4<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ret_spnt_many_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x21<<27 | (uint64)SPNT<<33 | op.pred | MANY<<12
              | 4<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ret_spnt_many_clr_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x21<<27 | (uint64)SPNT<<33 | op.pred | MANY<<12
              | (uint64)CLR<<35 | 4<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ret_sptk_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x21<<27 | (uint64)SPTK<<33 | op.pred 
              | 4<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ret_sptk_clr_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x21<<27 | (uint64)SPNT<<33 | op.pred
              | (uint64)CLR<<35 | 4<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ret_sptk_few_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x21<<27 | (uint64)SPTK<<33 | op.pred | FEW<<12
               | 4<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ret_sptk_few_clr_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x21<<27 | (uint64)SPTK<<33 | op.pred | FEW<<12
              | (uint64)CLR<<35 | 4<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ret_sptk_many_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x21<<27 | (uint64)SPTK | op.pred | MANY<<12
             | 4<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_ret_sptk_many_clr_b(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0x21<<27 | (uint64)SPTK<<33 | op.pred | MANY<<12
              | (uint64)CLR<<35 | 4<<6 | op.b[0]<<13;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wexit_dpnt_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPNT<<33 | FEW<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 2<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wexit_dpnt_few_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPNT<<33 | FEW<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 2<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wexit_dpnt_few_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPNT<<33 | FEW<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 2<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wexit_dpnt_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPNT<<33 | op.pred
              | (op.i[0]&0xfffff)<<13 | 2<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wexit_dpnt_many_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPNT<<33 | MANY<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 2<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wexit_dpnt_many_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPNT<<33 | MANY<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 2<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wexit_dptk_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPTK<<33 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 2<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wexit_dptk_few_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPTK<<33 | FEW<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 2<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wexit_dptk_few_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPTK<<33 | FEW<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 2<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wexit_dptk_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPTK<<33 | op.pred
              | (op.i[0]&0xfffff)<<13 | 2<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wexit_dptk_many_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPTK<<33 | MANY<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 2<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wexit_dptk_many_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPTK<<33 | MANY<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 2<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wexit_spnt_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPNT<<33 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 2<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wexit_spnt_few_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPNT<<33 | FEW<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 2<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wexit_spnt_few_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPNT<<33 | FEW<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 2<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wexit_spnt_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPNT<<33 | op.pred
              | (op.i[0]&0xfffff)<<13 | 2<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wexit_spnt_many_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPNT<<33 | MANY<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 2<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wexit_spnt_many_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPNT<<33 | MANY<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 2<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wexit_sptk_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPTK<<33 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 2<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wexit_sptk_few_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPTK<<33 | FEW<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 2<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wexit_sptk_few_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPTK<<33 | FEW<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 2<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wexit_sptk_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPTK<<33 | op.pred
              | (op.i[0]&0xfffff)<<13 | 2<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wexit_sptk_many_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPTK<<33 | MANY<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 2<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wexit_sptk_many_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPTK<<33 | MANY<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 2<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wtop_dpnt_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPNT<<33 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 3<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wtop_dpnt_few_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPNT<<33 | FEW<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 3<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wtop_dpnt_few_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPNT<<33 | FEW<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 3<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wtop_dpnt_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPNT<<33 | op.pred
              | (op.i[0]&0xfffff)<<13 | 3<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wtop_dpnt_many_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPNT<<33 | MANY<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 3<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wtop_dpnt_many_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPNT<<33 | MANY<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 3<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wtop_dptk_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPTK<<33 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 3<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wtop_dptk_few_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPTK<<33 | FEW<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 3<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wtop_dptk_few_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPTK<<33 | FEW<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 3<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wtop_dptk_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPTK<<33 | op.pred
              | (op.i[0]&0xfffff)<<13 | 3<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wtop_dptk_many_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPTK<<33 | MANY<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 3<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wtop_dptk_many_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPTK<<33 | MANY<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 3<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wtop_spnt_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPNT<<33 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 3<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wtop_spnt_few_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPNT<<33 | FEW<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 3<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wtop_spnt_few_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)DPTK<<33 | FEW<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 3<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wtop_spnt_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPNT<<33 | op.pred
              | (op.i[0]&0xfffff)<<13 | 3<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wtop_spnt_many_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPNT<<33 | MANY<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 3<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wtop_spnt_many_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPNT<<33 | MANY<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 3<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wtop_sptk_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPTK<<33 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 3<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wtop_sptk_few_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPTK<<33 | FEW<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 3<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wtop_sptk_few_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPTK<<33 | FEW<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 3<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wtop_sptk_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPTK<<33 | op.pred
              | (op.i[0]&0xfffff)<<13 | 3<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wtop_sptk_many_clr_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPTK<<33 | MANY<<12 | op.pred
              | (uint64)CLR<<35 | (op.i[0]&0xfffff)<<13 | 3<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 br_wtop_sptk_many_i25(uint32 *unit, qword *out) {
   *unit = BUNIT; op.i[0] >>= 4;
   out->q = (uint64)4<<37 | (uint64)SPTK<<33 | MANY<<12 | op.pred
              | (op.i[0]&0xfffff)<<13 | 3<<6
              | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*---------------------------------------------------------------------------*/
uint32 break_b_i21(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)0<<37 | (op.i[0]&0xfffff)<<6 |
          ((op.i[0]>>20)&1)<<36 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 break_f_i21(uint32 *unit, qword *out) {
   *unit = FUNIT;
   out->q = (uint64)0<<27 | (op.i[0]&0xfffff)<<6 |
          ((op.i[0]>>20)&1)<<36 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 break_i21(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*----------------------------------------------------------------------*/   
uint32 break_i_i21(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)0<<27 | (op.i[0]&0xfffff)<<6 |
          ((op.i[0]>>20)&1)<<36 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 break_m_i21(uint32 *unit, qword *out) {
   *unit = MUNIT;
   out->q = (uint64)0<<27 | (op.i[0]&0xfffff)<<6 |
          ((op.i[0]>>20)&1)<<36 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 break_x_i62(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*----------------------------------------------------------------------*/   
uint32 chk_a_clr_f_i25(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 chk_a_clr_r_i25(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 chk_a_nc_f_i25(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 chk_a_nc_r_i25(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 chk_s_f_i25(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*----------------------------------------------------------------------*/   
uint32 chk_s_i_r_i25(uint32 *unit, qword *out) {
   *unit = IUNIT; op.i[0] >>= 4;
   out->q = (uint64)0<<37 | (uint64)0x1<<33 | op.r[0]<<13 | (op.i[0]&127)<<6
                    | ((op.i[0]>>7)&0x1fff)<<20 | op.pred
                    | ((op.i[0]>>20)&1)<<36;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 chk_s_m_r_i25(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 chk_s_r_i25(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 clrrrb(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 clrrrb_pr(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*----------------------------------------------------------------------*/   
uint32 cmp4_eq_and_orcm_p_p_i8_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0],op.p[1]);
   return cmp4_ne_or_andcm_p_p_i8_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_eq_and_orcm_p_p_r_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0],op.p[1]);
   return cmp4_ne_or_andcm_p_p_r_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_eq_and_p_p_i8_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xc*32+3*4+0*16+1*2)<<32 | 0<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | (op.i[0]&127)<<13 | op.r[1]<<20
                    | ((op.i[0]>>7)&1)<<36 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_eq_and_p_p_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xc*32+1*4+0*16+1*2)<<32 | 0<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | op.r[0]<<13 | 
		    op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_eq_andcm_p_p_i8_r(uint32 *unit, qword *out) {
   return cmp4_ne_and_p_p_i8_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_eq_andcm_p_p_r_r(uint32 *unit, qword *out) {
   return cmp4_ne_and_p_p_r_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_eq_or_andcm_p_p_i8_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xe*32+3*4+0*16+1*2)<<32 | 0<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | (op.i[0]&127)<<13 | op.r[1]<<20
                    | ((op.i[0]>>7)&1)<<36 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_eq_or_andcm_p_p_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xe*32+1*4+0*16+1*2)<<32 | 0<<12 | op.p[0]<<6
             | (uint64)op.p[1]<<27 | op.r[0]<<13 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_eq_or_p_p_i8_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xd*32+3*4+0*16+1*2)<<32 | 0<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | (op.i[0]&127)<<13 | op.r[1]<<20
                    | ((op.i[0]>>7)&1)<<36 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_eq_or_p_p_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xd*32+1*4+0*16+1*2)<<32 | 0<<12 | op.p[0]<<6
             | (uint64)op.p[1]<<27 | op.r[0]<<13 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_eq_orcm_p_p_i8_r(uint32 *unit, qword *out) {
   return cmp4_ne_or_p_p_i8_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_eq_orcm_p_p_r_r(uint32 *unit, qword *out) {
   return cmp4_ne_or_p_p_r_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_eq_p_p_i8_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xe*32+3*4+0*16+0*2)<<32 | 0<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | (op.i[0]&127)<<13 | op.r[1]<<20
                    | ((op.i[0]>>7)&1)<<36 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_eq_p_p_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xe*32+1*4+0*16+0*2)<<32 | 0<<12 | op.p[0]<<6
             | (uint64)op.p[1]<<27 | op.r[0]<<13 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_eq_unc_p_p_i8_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xe*32+3*4+0*16+0*2)<<32 | 1<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | (op.i[0]&127)<<13 | op.r[1]<<20
                    | ((op.i[0]>>7)&1)<<36 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_eq_unc_p_p_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xe*32+1*4+0*16+0*2)<<32 | 1<<12 | op.p[0]<<6
             | (uint64)op.p[1]<<27 | op.r[0]<<13 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_ge_and_orcm_p_p_r0_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]);
   return cmp4_lt_or_andcm_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_ge_and_orcm_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp4_le_and_orcm_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_ge_and_p_p_r0_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xc*32+1*4+1*16+1*2)<<32 | 0<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_ge_and_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp4_le_and_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_ge_andcm_p_p_r0_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]);
   return cmp4_lt_and_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_ge_andcm_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp4_le_andcm_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_ge_or_andcm_p_p_r0_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xe*32+1*4+1*16+1*2)<<32 | 0<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_ge_or_andcm_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp4_le_or_andcm_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_ge_or_p_p_r0_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xd*32+1*4+1*16+1*2)<<32 | 0<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_ge_or_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp4_le_or_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_ge_orcm_p_p_r0_r(uint32 *unit, qword *out) {
   return cmp4_lt_or_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_ge_orcm_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp4_le_orcm_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_ge_p_p_i8_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]);
   return cmp4_lt_p_p_i8_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_ge_p_p_r_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]);
   return cmp4_lt_p_p_r_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_ge_unc_p_p_i8_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]);
   return cmp4_lt_unc_p_p_i8_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_ge_unc_p_p_r_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]);
   return cmp4_lt_unc_p_p_r_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_geu_p_p_i8_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]);
   return cmp4_ltu_p_p_i8_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_geu_p_p_r_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]);
   return cmp4_ltu_p_p_r_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_geu_unc_p_p_i8_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]);
   return cmp4_ltu_unc_p_p_i8_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_geu_unc_p_p_r_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]);
   return cmp4_ltu_unc_p_p_r_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_gt_and_orcm_p_p_r0_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]);
   return cmp4_le_or_andcm_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_gt_and_orcm_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp4_lt_and_orcm_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_gt_and_p_p_r0_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xc*32+1*4+1*16+0*2)<<32 | 0<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_gt_and_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp4_lt_and_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_gt_andcm_p_p_r0_r(uint32 *unit, qword *out) {
   return cmp4_le_and_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_gt_andcm_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp4_lt_andcm_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_gt_or_andcm_p_p_r0_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xe*32+1*4+1*16+0*2)<<32 | 0<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_gt_or_andcm_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp4_lt_or_andcm_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_gt_or_p_p_r0_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xd*32+1*4+1*16+0*2)<<32 | 0<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_gt_or_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp4_lt_or_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_gt_orcm_p_p_r0_r(uint32 *unit, qword *out) {
   return cmp4_le_or_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_gt_orcm_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp4_lt_orcm_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_gt_p_p_i8_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]); op.i[0] -= 1;
   return cmp4_lt_p_p_i8_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_gt_p_p_r_r(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp4_lt_p_p_r_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_gt_unc_p_p_i8_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]); op.i[0] -= 1;
   return cmp4_lt_unc_p_p_i8_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_gt_unc_p_p_r_r(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp4_lt_unc_p_p_r_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_gtu_p_p_i8_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]); op.i[0] -= 1;
   return cmp4_ltu_p_p_i8_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_gtu_p_p_r_r(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp4_ltu_p_p_r_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_gtu_unc_p_p_i8_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]); op.i[0] -= 1;
   return cmp4_ltu_unc_p_p_i8_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_gtu_unc_p_p_r_r(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp4_ltu_unc_p_p_r_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_le_and_orcm_p_p_r0_r(uint32 *unit, qword *out) {
   return cmp4_gt_or_andcm_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_le_and_orcm_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp4_ge_and_orcm_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_le_and_p_p_r0_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xc*32+1*4+1*16+0*2)<<32 | 1<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_le_and_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp4_ge_and_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_le_andcm_p_p_r0_r(uint32 *unit, qword *out) {
   return cmp4_gt_and_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_le_andcm_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp4_ge_and_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_le_or_andcm_p_p_r0_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xe*32+1*4+1*16+0*2)<<32 | 1<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_le_or_andcm_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp4_ge_or_andcm_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_le_or_p_p_r0_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xd*32+1*4+1*16+0*2)<<32 | 1<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_le_or_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp4_ge_or_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_le_orcm_p_p_r0_r(uint32 *unit, qword *out) {
   return cmp4_gt_or_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_le_orcm_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp4_ge_orcm_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_le_p_p_i8_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]); op.i[0] -= 1;
   return cmp4_lt_p_p_i8_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_le_p_p_r_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]); SWAPI(op.r[0], op.r[1]); 
   return cmp4_lt_p_p_r_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_le_unc_p_p_i8_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]); op.i[0] -= 1;
   return cmp4_lt_unc_p_p_i8_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_le_unc_p_p_r_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]); SWAPI(op.r[0], op.r[1]); 
   return cmp4_lt_unc_p_p_r_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_leu_p_p_i8_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]); op.i[0] -= 1;
   return cmp4_ltu_p_p_i8_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_leu_p_p_r_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]); SWAPI(op.r[0], op.r[1]); 
   return cmp4_ltu_p_p_r_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_leu_unc_p_p_i8_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]); op.i[0] -= 1;
   return cmp4_ltu_unc_p_p_i8_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_leu_unc_p_p_r_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]); SWAPI(op.r[0], op.r[1]); 
   return cmp4_ltu_unc_p_p_r_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_lt_and_orcm_p_p_r0_r(uint32 *unit, qword *out) {
   return cmp4_ge_or_andcm_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_lt_and_orcm_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]); 
   return cmp4_gt_and_orcm_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_lt_and_p_p_r0_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xc*32+1*4+1*16+1*2)<<32 | 1<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_lt_and_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]); 
   return cmp4_gt_and_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_lt_andcm_p_p_r0_r(uint32 *unit, qword *out) {
   return cmp4_ge_and_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_lt_andcm_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]); 
   return cmp4_gt_andcm_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_lt_or_andcm_p_p_r0_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xe*32+1*4+1*16+1*2)<<32 | 1<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_lt_or_andcm_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]); 
   return cmp4_gt_or_andcm_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_lt_or_p_p_r0_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xd*32+1*4+1*16+1*2)<<32 | 1<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_lt_or_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]); 
   return cmp4_gt_or_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_lt_orcm_p_p_r0_r(uint32 *unit, qword *out) {
   return cmp4_ge_or_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_lt_orcm_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]); 
   return cmp4_gt_orcm_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_lt_p_p_i8_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xc*32+3*4+0*16+0*2)<<32 | 0<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | (op.i[0]&127)<<13 | op.r[1]<<20
                    | ((op.i[0]>>7)&1)<<36 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_lt_p_p_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xc*32+1*4+0*16+0*2)<<32 | 0<<12 | op.p[0]<<6
             | (uint64)op.p[1]<<27 | op.r[0]<<13 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_lt_unc_p_p_i8_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xc*32+3*4+0*16+0*2)<<32 | 1<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | (op.i[0]&127)<<13 | op.r[1]<<20
                    | ((op.i[0]>>7)&1)<<36 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_lt_unc_p_p_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xc*32+1*4+0*16+0*2)<<32 | 1<<12 | op.p[0]<<6
             | (uint64)op.p[1]<<27 | op.r[0]<<13 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_ltu_p_p_i8_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xd*32+3*4+0*16+0*2)<<32 | 0<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | (op.i[0]&127)<<13 | op.r[1]<<20
                    | ((op.i[0]>>7)&1)<<36 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_ltu_p_p_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xd*32+1*4+0*16+0*2)<<32 | 0<<12 | op.p[0]<<6
              | (uint64)op.p[1]<<27 | op.r[0]<<13 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_ltu_unc_p_p_i8_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xd*32+3*4+0*16+0*2)<<32 | 1<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | (op.i[0]&127)<<13 | op.r[1]<<20
                    | ((op.i[0]>>7)&1)<<36 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_ltu_unc_p_p_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xd*32+1*4+0*16+0*2)<<32 | 1<<12 | op.p[0]<<6
             | (uint64)op.p[1]<<27 | op.r[0]<<13 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_ne_and_orcm_p_p_i8_r(uint32 *unit, qword *out) {
   return cmp4_eq_or_andcm_p_p_i8_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_ne_and_orcm_p_p_r_r(uint32 *unit, qword *out) {
   return cmp4_eq_or_andcm_p_p_r_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_ne_and_p_p_i8_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xc*32+3*4+0*16+1*2)<<32 | 1<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | (op.i[0]&127)<<13 | op.r[1]<<20
                    | ((op.i[0]>>7)&1)<<36 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_ne_and_p_p_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xc*32+1*4+0*16+1*2)<<32 | 1<<12 | op.p[0]<<6
              | (uint64)op.p[1]<<27 | op.r[0]<<13 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_ne_andcm_p_p_i8_r(uint32 *unit, qword *out) {
   return cmp4_eq_and_p_p_i8_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_ne_andcm_p_p_r_r(uint32 *unit, qword *out) {
   return cmp4_eq_and_p_p_r_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_ne_or_andcm_p_p_i8_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xe*32+3*4+0*16+1*2)<<32 | 1<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | (op.i[0]&127)<<13 | op.r[1]<<20
                    | ((op.i[0]>>7)&1)<<36 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_ne_or_andcm_p_p_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xe*32+1*4+0*16+1*2)<<32 | 1<<12 | op.p[0]<<6
             | (uint64)op.p[1]<<27 | op.r[0]<<13 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_ne_or_p_p_i8_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xd*32+3*4+0*16+1*2)<<32 | 1<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | (op.i[0]&127)<<13 | op.r[1]<<20
                    | ((op.i[0]>>7)&1)<<36 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_ne_or_p_p_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xd*32+1*4+0*16+1*2)<<32 | 1<<12 | op.p[0]<<6
              | (uint64)op.p[1]<<27 | op.r[0]<<13 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_ne_orcm_p_p_i8_r(uint32 *unit, qword *out) {
   return cmp4_eq_or_p_p_i8_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_ne_orcm_p_p_r_r(uint32 *unit, qword *out) {
   return cmp4_eq_or_p_p_r_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_ne_p_p_i8_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]);
   return cmp4_eq_p_p_i8_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_ne_p_p_r_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]);
   return cmp4_eq_p_p_r_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_ne_unc_p_p_i8_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]);
   return cmp4_eq_unc_p_p_i8_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp4_ne_unc_p_p_r_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]);
   return cmp4_eq_unc_p_p_r_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_eq_and_orcm_p_p_i8_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0],op.p[1]);
   return cmp_ne_or_andcm_p_p_i8_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_eq_and_orcm_p_p_r_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0],op.p[1]);
   return cmp_ne_or_andcm_p_p_r_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_eq_and_p_p_i8_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xc*32+2*4+0*16+1*2)<<32 | 0<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | (op.i[0]&127)<<13 | op.r[1]<<20
                    | ((op.i[0]>>7)&1)<<36 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp_eq_and_p_p_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xc*32+0*4+0*16+1*2)<<32 | 0<<12 | op.p[0]<<6
            | (uint64)op.p[1]<<27 | op.r[0]<<13 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp_eq_andcm_p_p_i8_r(uint32 *unit, qword *out) {
   return cmp_ne_and_p_p_i8_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_eq_andcm_p_p_r_r(uint32 *unit, qword *out) {
   return cmp_ne_and_p_p_r_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_eq_or_andcm_p_p_i8_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xe*32+2*4+0*16+1*2)<<32 | 0<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | (op.i[0]&127)<<13 | op.r[1]<<20
                    | ((op.i[0]>>7)&1)<<36 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp_eq_or_andcm_p_p_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xe*32+0*4+0*16+1*2)<<32 | 0<<12 | op.p[0]<<6
             | (uint64)op.p[1]<<27 | op.r[0]<<13 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp_eq_or_p_p_i8_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xd*32+2*4+0*16+1*2)<<32 | 0<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | (op.i[0]&127)<<13 | op.r[1]<<20
                    | ((op.i[0]>>7)&1)<<36 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp_eq_or_p_p_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xd*32+0*4+0*16+1*2)<<32 | 0<<12 | op.p[0]<<6
            | (uint64)op.p[1]<<27 | op.r[0]<<13 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp_eq_orcm_p_p_i8_r(uint32 *unit, qword *out) {
   return cmp_ne_or_p_p_i8_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_eq_orcm_p_p_r_r(uint32 *unit, qword *out) {
   return cmp_ne_or_p_p_r_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_eq_p_p_i8_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xe*32+2*4+0*16+0*2)<<32 | 0<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | (op.i[0]&127)<<13 | op.r[0]<<20
                    | ((op.i[0]>>7)&1)<<36 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp_eq_p_p_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xe*32+0*4+0*16+0*2)<<32 | 0<<12 | op.p[0]<<6
             | (uint64)op.p[1]<<27 | op.r[0]<<13 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp_eq_unc_p_p_i8_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xe*32+2*4+0*16+0*2)<<32 | 1<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | (op.i[0]&127)<<13 | op.r[1]<<20
                    | ((op.i[0]>>7)&1)<<36 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp_eq_unc_p_p_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xe*32+0*4+0*16+0*2)<<32 | 1<<12 | op.p[0]<<6
             | (uint64)op.p[1]<<27 | op.r[0]<<13 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp_ge_and_orcm_p_p_r0_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]);
   return cmp_lt_or_andcm_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_ge_and_orcm_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp_le_and_orcm_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_ge_and_p_p_r0_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xc*32+0*4+1*16+1*2)<<32 | 0<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp_ge_and_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp_le_and_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_ge_andcm_p_p_r0_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xe*32+0*4+1*16+1*2)<<32 | 0<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp_ge_andcm_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]);
   return cmp_lt_and_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_ge_or_andcm_p_p_r0_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xe*32+0*4+1*16+1*2)<<32 | 0<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp_ge_or_andcm_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp_le_or_andcm_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_ge_or_p_p_r0_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xd*32+0*4+1*16+1*2)<<32 | 0<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp_ge_or_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp_le_or_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_ge_orcm_p_p_r0_r(uint32 *unit, qword *out) {
   return cmp_lt_or_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_ge_orcm_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp_le_orcm_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_ge_p_p_i8_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]);
   return cmp_lt_p_p_i8_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_ge_p_p_r_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]);
   return cmp_lt_p_p_r_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_ge_unc_p_p_i8_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]);
   return cmp_lt_unc_p_p_i8_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_ge_unc_p_p_r_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]);
   return cmp_lt_unc_p_p_r_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_geu_p_p_i8_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]);
   return cmp_ltu_p_p_i8_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_geu_p_p_r_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]);
   return cmp_ltu_p_p_r_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_geu_unc_p_p_i8_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]);
   return cmp_ltu_unc_p_p_i8_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_geu_unc_p_p_r_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]);
   return cmp_ltu_unc_p_p_r_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_gt_and_orcm_p_p_r0_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]);
   return cmp_le_or_andcm_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_gt_and_orcm_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp_lt_and_orcm_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_gt_and_p_p_r0_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xc*32+0*4+1*16+0*2)<<32 | 0<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp_gt_and_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp_lt_and_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_gt_andcm_p_p_r0_r(uint32 *unit, qword *out) {
   return cmp_le_and_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_gt_andcm_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp_lt_andcm_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_gt_or_andcm_p_p_r0_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xe*32+0*4+1*16+0*2)<<32 | 0<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp_gt_or_andcm_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp_lt_or_andcm_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_gt_or_p_p_r0_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xd*32+0*4+1*16+0*2)<<32 | 0<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp_gt_or_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp_lt_or_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_gt_orcm_p_p_r0_r(uint32 *unit, qword *out) {
   return cmp_le_or_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_gt_orcm_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp_lt_orcm_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_gt_p_p_i8_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]); op.i[0] -= 1;
   return cmp_lt_p_p_i8_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_gt_p_p_r_r(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp_lt_p_p_r_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_gt_unc_p_p_i8_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]); op.i[0] -= 1;
   return cmp_lt_unc_p_p_i8_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_gt_unc_p_p_r_r(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp_lt_unc_p_p_r_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_gtu_p_p_i8_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]); op.i[0] -= 1;
   return cmp_ltu_p_p_i8_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_gtu_p_p_r_r(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp_ltu_p_p_r_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_gtu_unc_p_p_i8_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]); op.i[0] -= 1;
   return cmp_ltu_unc_p_p_i8_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_gtu_unc_p_p_r_r(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp_ltu_unc_p_p_r_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_le_and_orcm_p_p_r0_r(uint32 *unit, qword *out) {
   return cmp_gt_or_andcm_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_le_and_orcm_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp_ge_and_orcm_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_le_and_p_p_r0_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xc*32+0*4+1*16+0*2)<<32 | 1<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp_le_and_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp_ge_and_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_le_andcm_p_p_r0_r(uint32 *unit, qword *out) {
   return cmp_gt_and_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_le_andcm_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp_ge_and_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_le_or_andcm_p_p_r0_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xe*32+0*4+1*16+0*2)<<32 | 1<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp_le_or_andcm_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp_ge_or_andcm_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_le_or_p_p_r0_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xd*32+0*4+1*16+0*2)<<32 | 1<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp_le_or_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp_ge_or_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_le_orcm_p_p_r0_r(uint32 *unit, qword *out) {
   return cmp_gt_or_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_le_orcm_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]);
   return cmp_ge_orcm_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_le_p_p_i8_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]); op.i[0] -= 1;
   return cmp_lt_p_p_i8_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_le_p_p_r_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]); SWAPI(op.r[0], op.r[1]); 
   return cmp_lt_p_p_r_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_le_unc_p_p_i8_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]); op.i[0] -= 1;
   return cmp_lt_unc_p_p_i8_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_le_unc_p_p_r_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]); SWAPI(op.r[0], op.r[1]); 
   return cmp_lt_unc_p_p_r_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_leu_p_p_i8_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]); op.i[0] -= 1;
   return cmp_ltu_p_p_i8_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_leu_p_p_r_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]); SWAPI(op.r[0], op.r[1]); 
   return cmp_ltu_p_p_r_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_leu_unc_p_p_i8_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]); op.i[0] -= 1;
   return cmp_ltu_unc_p_p_i8_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_leu_unc_p_p_r_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]); SWAPI(op.r[0], op.r[1]); 
   return cmp_ltu_unc_p_p_r_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_lt_and_orcm_p_p_r0_r(uint32 *unit, qword *out) {
   return cmp_ge_or_andcm_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_lt_and_orcm_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]); 
   return cmp_gt_and_orcm_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_lt_and_p_p_r0_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xc*32+0*4+1*16+1*2)<<32 | 1<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp_lt_and_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]); 
   return cmp_gt_and_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_lt_andcm_p_p_r0_r(uint32 *unit, qword *out) {
   return cmp_ge_and_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_lt_andcm_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]); 
   return cmp_gt_andcm_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_lt_or_andcm_p_p_r0_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xe*32+0*4+1*16+1*2)<<32 | 1<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp_lt_or_andcm_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]); 
   return cmp_gt_or_andcm_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_lt_or_p_p_r0_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xd*32+0*4+1*16+1*2)<<32 | 1<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp_lt_or_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]); 
   return cmp_gt_or_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_lt_orcm_p_p_r0_r(uint32 *unit, qword *out) {
   return cmp_ge_or_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_lt_orcm_p_p_r_r0(uint32 *unit, qword *out) {
   SWAPI(op.r[0], op.r[1]); 
   return cmp_gt_orcm_p_p_r0_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_lt_p_p_i8_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xc*32+2*4+0*16+0*2)<<32 | 0<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | (op.i[0]&127)<<13 | op.r[1]<<20
                    | ((op.i[0]>>7)&1)<<36 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp_lt_p_p_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xc*32+0*4+0*16+0*2)<<32 | 0<<12 | op.p[0]<<6
             | (uint64)op.p[1]<<27 | op.r[0]<<13 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp_lt_unc_p_p_i8_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xc*32+2*4+0*16+0*2)<<32 | 1<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | (op.i[0]&127)<<13 | op.r[1]<<20
                    | ((op.i[0]>>7)&1)<<36 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp_lt_unc_p_p_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xc*32+0*4+0*16+0*2)<<32 | 1<<12 | op.p[0]<<6
             | (uint64)op.p[1]<<27 | op.r[0]<<13 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp_ltu_p_p_i8_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xd*32+2*4+0*16+0*2)<<32 | 0<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | (op.i[0]&127)<<13 | op.r[1]<<20
                    | ((op.i[0]>>7)&1)<<36 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp_ltu_p_p_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xd*32+0*4+0*16+0*2)<<32 | 0<<12 | op.p[0]<<6
             | (uint64)op.p[1]<<27 | op.r[0]<<13 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp_ltu_unc_p_p_i8_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xd*32+2*4+0*16+0*2)<<32 | 1<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | (op.i[0]&127)<<13 | op.r[1]<<20
                    | ((op.i[0]>>7)&1)<<36 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp_ltu_unc_p_p_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xd*32+0*4+0*16+0*2)<<32 | 1<<12 | op.p[0]<<6
             | (uint64)op.p[1]<<27 | op.r[0]<<13 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp_ne_and_orcm_p_p_i8_r(uint32 *unit, qword *out) {
   return cmp_eq_or_andcm_p_p_i8_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_ne_and_orcm_p_p_r_r(uint32 *unit, qword *out) {
   return cmp_eq_or_andcm_p_p_r_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_ne_and_p_p_i8_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xc*32+2*4+0*16+1*2)<<32 | 1<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | (op.i[0]&127)<<13 | op.r[1]<<20
                    | ((op.i[0]>>7)&1)<<36 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp_ne_and_p_p_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xc*32+0*4+0*16+1*2)<<32 | 1<<12 | op.p[0]<<6
             | (uint64)op.p[1]<<27 | op.r[0]<<13 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp_ne_andcm_p_p_i8_r(uint32 *unit, qword *out) {
   return cmp_eq_and_p_p_i8_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_ne_andcm_p_p_r_r(uint32 *unit, qword *out) {
   return cmp_eq_and_p_p_r_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_ne_or_andcm_p_p_i8_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xe*32+2*4+0*16+1*2)<<32 | 1<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | (op.i[0]&127)<<13 | op.r[1]<<20
                    | ((op.i[0]>>7)&1)<<36 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp_ne_or_andcm_p_p_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xe*32+0*4+0*16+1*2)<<32 | 1<<12 | op.p[0]<<6
             | (uint64)op.p[1]<<27 | op.r[0]<<13 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp_ne_or_p_p_i8_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xd*32+2*4+0*16+1*2)<<32 | 1<<12 | op.p[0]<<6
                    | (uint64)op.p[1]<<27 | (op.i[0]&127)<<13 | op.r[1]<<20
                    | ((op.i[0]>>7)&1)<<36 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp_ne_or_p_p_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)(0xd*32+0*4+0*16+1*2)<<32 | 1<<12 | op.p[0]<<6
              | (uint64)op.p[1]<<27 | op.r[0]<<13 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 cmp_ne_orcm_p_p_i8_r(uint32 *unit, qword *out) {
   return cmp_eq_or_p_p_i8_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_ne_orcm_p_p_r_r(uint32 *unit, qword *out) {
   return cmp_eq_or_p_p_r_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_ne_p_p_i8_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]);
   return cmp_eq_p_p_i8_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_ne_p_p_r_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]);
   return cmp_eq_p_p_r_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_ne_unc_p_p_i8_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]);
   return cmp_eq_unc_p_p_i8_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmp_ne_unc_p_p_r_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0], op.p[1]);
   return cmp_eq_unc_p_p_r_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 cmpxchg_1_acq_nt1_r_r_r_ar_ccv(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 cmpxchg_1_acq_nta_r_r_r_ar_ccv(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 cmpxchg_1_acq_r_r_r_ar_ccv(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 cmpxchg_1_rel_nt1_r_r_r_ar_ccv(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 cmpxchg_1_rel_nta_r_r_r_ar_ccv(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 cmpxchg_1_rel_r_r_r_ar_ccv(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 cmpxchg_2_acq_nt1_r_r_r_ar_ccv(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 cmpxchg_2_acq_nta_r_r_r_ar_ccv(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 cmpxchg_2_acq_r_r_r_ar_ccv(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 cmpxchg_2_rel_nt1_r_r_r_ar_ccv(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 cmpxchg_2_rel_nta_r_r_r_ar_ccv(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 cmpxchg_2_rel_r_r_r_ar_ccv(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 cmpxchg_4_acq_nt1_r_r_r_ar_ccv(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 cmpxchg_4_acq_nta_r_r_r_ar_ccv(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 cmpxchg_4_acq_r_r_r_ar_ccv(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 cmpxchg_4_rel_nt1_r_r_r_ar_ccv(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 cmpxchg_4_rel_nta_r_r_r_ar_ccv(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 cmpxchg_4_rel_r_r_r_ar_ccv(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 cmpxchg_8_acq_nt1_r_r_r_ar_ccv(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 cmpxchg_8_acq_nta_r_r_r_ar_ccv(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 cmpxchg_8_acq_r_r_r_ar_ccv(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 cmpxchg_8_rel_nt1_r_r_r_ar_ccv(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 cmpxchg_8_rel_nta_r_r_r_ar_ccv(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 cmpxchg_8_rel_r_r_r_ar_ccv(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*----------------------------------------------------------------------*/   
uint32 czx1_l_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)0<<37 | (uint64)(0*64+0x18)<<27 | op.r[0]<<6
                    | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 czx1_r_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)0<<37 | (uint64)(0*64+0x1c)<<27 | op.r[0]<<6
                    | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 czx2_l_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)0<<37 | (uint64)(0*64+0x19)<<27 | op.r[0]<<6
                    | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 czx2_r_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)0<<37 | (uint64)(0*64+0x1d)<<27 | op.r[0]<<6
                    | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 dep_r_u1_r_u6_u6(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)5<<37 | (uint64)(3*2+1)<<33 | op.i[2]<<27 |
                 op.r[0]<<6 | (op.i[1]-1)<<14 | op.r[1]<<20 |
                 op.i[0]<<36 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 dep_r_r_r_u6_u4(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)4<<37 | op.i[2]<<27 | (op.i[0]-1)<<31 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 |
                 op.i[1]<<27 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 dep_z_r_i8_u6_u6(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)5<<37 | (uint64)(1*2+1)<<33 | op.i[2]<<27 |
                 op.r[0]<<6 |  (op.i[0]&127)<<13 |(op.i[1]-1)<<20 |
                 (uint64)1<<26 | ((op.i[0]>>7)&1)<<36 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 dep_z_r_r_u6_u6(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)5<<37 | (uint64)(1*2+1)<<33 | op.i[1]<<27 |
                 op.r[0]<<6 |  op.r[1]<<13 | (op.i[0]-1)<<20 |
                 (uint64)0<<26 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 extr_r_r_u6_u6(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)5<<37 | (uint64)(1*2+0)<<33 | op.i[1]<<27 |
                 op.r[0]<<6 | (op.i[0]-1)<<14 | op.r[2]<<20 |
                 (uint64)0<<13 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 extr_u_r_r_u6_u6(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)5<<37 | (uint64)(1*2+0)<<33 | op.i[1]<<27 |
                 op.r[0]<<6 | (op.i[0]-1)<<14 | op.r[2]<<20 |
                 (uint64)1<<13 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 fabs_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fadd_d_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fadd_d_s0_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fadd_d_s1_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fadd_d_s2_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fadd_d_s3_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fadd_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fadd_s0_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fadd_s1_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fadd_s2_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fadd_s3_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fadd_s_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fadd_s_s0_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fadd_s_s1_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fadd_s_s2_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fadd_s_s3_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 famax_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 famax_s0_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 famax_s1_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 famax_s2_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 famax_s3_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 famin_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 famin_s0_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 famin_s1_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 famin_s2_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 famin_s3_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fand_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fandcm_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fc_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fchkf_i25(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fchkf_s0_i25(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fchkf_s1_i25(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fchkf_s2_i25(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fchkf_s3_i25(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_eq_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_ge_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_gt_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_le_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_lt_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_m_eq_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_m_ge_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_m_gt_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_m_le_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_m_lt_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_m_neq_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_m_nge_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_m_ngt_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_m_nlt_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_m_ord_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_m_unord_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_neq_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_nge_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_ngt_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_nlt_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_nm_eq_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_nm_ge_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_nm_gt_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_nm_le_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_nm_lt_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_nm_neq_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_nm_nge_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_nm_ngt_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_nm_nlt_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_nm_ord_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_nm_unord_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_ord_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_unc_eq_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_unc_ge_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_unc_gt_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_unc_le_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_unc_lt_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_unc_neq_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_unc_nge_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_unc_ngt_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_unc_nlt_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_unc_ord_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_unc_unord_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclass_unord_p_p_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclrf(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclrf_s0(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclrf_s1(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclrf_s2(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fclrf_s3(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_eq_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_eq_s0_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_eq_s1_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_eq_s2_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_eq_s3_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_eq_unc_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_eq_unc_s0_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_eq_unc_s1_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_eq_unc_s2_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_eq_unc_s3_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_ge_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_ge_s0_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_ge_s1_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_ge_s2_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_ge_s3_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_ge_unc_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_ge_unc_s0_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_ge_unc_s1_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_ge_unc_s2_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_ge_unc_s3_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_gt_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_gt_s0_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_gt_s1_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_gt_s2_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_gt_s3_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_gt_unc_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_gt_unc_s0_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_gt_unc_s1_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_gt_unc_s2_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_gt_unc_s3_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_le_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_le_s0_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_le_s1_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_le_s2_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_le_s3_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_le_unc_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_le_unc_s0_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_le_unc_s1_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_le_unc_s2_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_le_unc_s3_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_lt_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_lt_s0_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_lt_s1_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_lt_s2_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_lt_s3_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_lt_unc_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_lt_unc_s0_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_lt_unc_s1_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_lt_unc_s2_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_lt_unc_s3_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_neq_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_neq_s0_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_neq_s1_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_neq_s2_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_neq_s3_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_neq_unc_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_neq_unc_s0_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_neq_unc_s1_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_neq_unc_s2_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_neq_unc_s3_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_nge_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_nge_s0_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_nge_s1_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_nge_s2_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_nge_s3_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_nge_unc_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_nge_unc_s0_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_nge_unc_s1_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_nge_unc_s2_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_nge_unc_s3_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_ngt_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_ngt_s0_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_ngt_s1_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_ngt_s2_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_ngt_s3_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_ngt_unc_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_ngt_unc_s0_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_ngt_unc_s1_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_ngt_unc_s2_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_ngt_unc_s3_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_nlt_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_nlt_s0_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_nlt_s1_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_nlt_s2_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_nlt_s3_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_nlt_unc_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_nlt_unc_s0_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_nlt_unc_s1_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_nlt_unc_s2_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_nlt_unc_s3_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_ord_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_ord_s0_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_ord_s1_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_ord_s2_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_ord_s3_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_ord_unc_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_ord_unc_s0_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_ord_unc_s1_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_ord_unc_s2_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_ord_unc_s3_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_unord_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_unord_s0_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_unord_s1_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_unord_s2_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_unord_s3_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_unord_unc_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_unord_unc_s0_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_unord_unc_s1_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_unord_unc_s2_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcmp_unord_unc_s3_p_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcvt_fx_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcvt_fx_s0_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcvt_fx_s1_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcvt_fx_s2_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcvt_fx_s3_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcvt_fx_trunc_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcvt_fx_trunc_s0_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcvt_fx_trunc_s1_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcvt_fx_trunc_s2_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcvt_fx_trunc_s3_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcvt_fxu_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcvt_fxu_s0_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcvt_fxu_s1_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcvt_fxu_s2_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcvt_fxu_s3_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcvt_fxu_trunc_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcvt_fxu_trunc_s0_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcvt_fxu_trunc_s1_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcvt_fxu_trunc_s2_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcvt_fxu_trunc_s3_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcvt_xf_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcvt_xuf_d_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcvt_xuf_d_s0_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcvt_xuf_d_s1_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcvt_xuf_d_s2_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcvt_xuf_d_s3_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcvt_xuf_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcvt_xuf_s0_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcvt_xuf_s1_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcvt_xuf_s2_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcvt_xuf_s3_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcvt_xuf_s_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcvt_xuf_s_s0_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcvt_xuf_s_s1_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcvt_xuf_s_s2_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fcvt_xuf_s_s3_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fetchadd4_acq_nt1_r_r_u3(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fetchadd4_acq_nta_r_r_u3(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fetchadd4_acq_r_r_u3(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fetchadd4_rel_nt1_r_r_u3(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fetchadd4_rel_nta_r_r_u3(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fetchadd4_rel_r_r_u3(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fetchadd8_acq_nt1_r_r_u3(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fetchadd8_acq_nta_r_r_u3(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fetchadd8_acq_r_r_u3(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fetchadd8_rel_nt1_r_r_u3(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fetchadd8_rel_nta_r_r_u3(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fetchadd8_rel_r_r_u3(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 flushrs(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fma_d_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fma_d_s0_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fma_d_s1_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fma_d_s2_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fma_d_s3_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fma_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fma_s0_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fma_s1_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fma_s2_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fma_s3_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fma_s_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fma_s_s0_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fma_s_s1_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fma_s_s2_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fma_s_s3_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fmax_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fmax_s0_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fmax_s1_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fmax_s2_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fmax_s3_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fmerge_ns_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fmerge_s_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fmerge_se_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fmin_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fmin_s0_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fmin_s1_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fmin_s2_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fmin_s3_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fmix_l_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fmix_lr_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fmix_r_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fmpy_d_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fmpy_d_s0_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fmpy_d_s1_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fmpy_d_s2_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fmpy_d_s3_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fmpy_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fmpy_s0_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fmpy_s1_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fmpy_s2_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fmpy_s3_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fmpy_s_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fmpy_s_s0_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fmpy_s_s1_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fmpy_s_s2_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fmpy_s_s3_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fms_d_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fms_d_s0_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fms_d_s1_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fms_d_s2_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fms_d_s3_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fms_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fms_s0_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fms_s1_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fms_s2_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fms_s3_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fms_s_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fms_s_s0_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fms_s_s1_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fms_s_s2_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fms_s_s3_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fneg_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnegabs_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnma_d_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnma_d_s0_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnma_d_s1_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnma_d_s2_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnma_d_s3_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnma_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnma_s0_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnma_s1_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnma_s2_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnma_s3_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnma_s_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnma_s_s0_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnma_s_s1_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnma_s_s2_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnma_s_s3_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnmpy_d_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnmpy_d_s0_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnmpy_d_s1_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnmpy_d_s2_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnmpy_d_s3_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnmpy_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnmpy_s0_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnmpy_s1_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnmpy_s2_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnmpy_s3_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnmpy_s_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnmpy_s_s0_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnmpy_s_s1_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnmpy_s_s2_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnmpy_s_s3_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnorm_d_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnorm_d_s0_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnorm_d_s1_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnorm_d_s2_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnorm_d_s3_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnorm_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnorm_s0_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnorm_s1_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnorm_s2_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnorm_s3_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnorm_s_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnorm_s_s0_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnorm_s_s1_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnorm_s_s2_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fnorm_s_s3_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 for_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpabs_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpack_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpamax_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpamax_s0_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpamax_s1_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpamax_s2_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpamax_s3_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpamin_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpamin_s0_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpamin_s1_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpamin_s2_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpamin_s3_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_eq_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_eq_s0_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_eq_s1_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_eq_s2_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_eq_s3_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_ge_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_ge_s0_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_ge_s1_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_ge_s2_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_ge_s3_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_gt_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_gt_s0_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_gt_s1_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_gt_s2_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_gt_s3_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_le_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_le_s0_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_le_s1_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_le_s2_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_le_s3_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_lt_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_lt_s0_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_lt_s1_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_lt_s2_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_lt_s3_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_neq_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_neq_s0_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_neq_s1_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_neq_s2_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_neq_s3_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_nge_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_nge_s0_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_nge_s1_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_nge_s2_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_nge_s3_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_ngt_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_ngt_s0_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_ngt_s1_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_ngt_s2_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_ngt_s3_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_nlt_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_nlt_s0_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_nlt_s1_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_nlt_s2_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_nlt_s3_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_ord_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_ord_s0_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_ord_s1_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_ord_s2_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_ord_s3_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_unord_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_unord_s0_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_unord_s1_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_unord_s2_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcmp_unord_s3_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcvt_fx_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcvt_fx_s0_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcvt_fx_s1_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcvt_fx_s2_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcvt_fx_s3_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcvt_fx_trunc_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcvt_fx_trunc_s0_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcvt_fx_trunc_s1_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcvt_fx_trunc_s2_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcvt_fx_trunc_s3_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcvt_fxu_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcvt_fxu_s0_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcvt_fxu_s1_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcvt_fxu_s2_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcvt_fxu_s3_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcvt_fxu_trunc_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcvt_fxu_trunc_s0_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcvt_fxu_trunc_s1_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcvt_fxu_trunc_s2_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpcvt_fxu_trunc_s3_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpma_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpma_s0_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpma_s1_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpma_s2_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpma_s3_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpmax_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpmax_s0_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpmax_s1_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpmax_s2_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpmax_s3_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpmerge_ns_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpmerge_s_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpmerge_se_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpmin_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpmin_s0_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpmin_s1_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpmin_s2_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpmin_s3_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpmpy_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpmpy_s0_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpmpy_s1_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpmpy_s2_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpmpy_s3_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpms_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpms_s0_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpms_s1_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpms_s2_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpms_s3_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpneg_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fpnegabs_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fprcpa_f_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fprcpa_s0_f_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fprcpa_s1_f_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fprcpa_s2_f_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fprcpa_s3_f_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fprsqrta_f_p_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fprsqrta_s0_f_p_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fprsqrta_s1_f_p_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fprsqrta_s2_f_p_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fprsqrta_s3_f_p_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 frcpa_f_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 frcpa_s0_f_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 frcpa_s1_f_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 frcpa_s2_f_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 frcpa_s3_f_p_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 frsqrta_f_p_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 frsqrta_s0_f_p_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 frsqrta_s1_f_p_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 frsqrta_s2_f_p_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 frsqrta_s3_f_p_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fselect_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fsetc_u7_u7(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fsetc_s0_u7_u7(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fsetc_s1_u7_u7(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fsetc_s2_u7_u7(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fsetc_s3_u7_u7(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fsub_d_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fsub_d_s0_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fsub_d_s1_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fsub_d_s2_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fsub_d_s3_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fsub_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fsub_s0_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fsub_s1_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fsub_s2_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fsub_s3_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fsub_s_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fsub_s_s0_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fsub_s_s1_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fsub_s_s2_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fsub_s_s3_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fswap_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fswap_nl_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fswap_nr_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fsxt_l_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fsxt_r_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 fxor_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 getf_d_r_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 getf_exp_r_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 getf_s_r_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 getf_sig_r_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 invala(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 invala_e_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 invala_e_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_a_nt1_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_a_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_a_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_a_nta_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_a_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_a_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_a_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_a_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_a_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_acq_nt1_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_acq_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_acq_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_acq_nta_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_acq_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_acq_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_acq_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_acq_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_acq_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_bias_nt1_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_bias_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_bias_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_bias_nta_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_bias_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_bias_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_bias_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_bias_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_bias_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_c_clr_acq_nt1_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_c_clr_acq_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_c_clr_acq_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_c_clr_acq_nta_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_c_clr_acq_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_c_clr_acq_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_c_clr_acq_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_c_clr_acq_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_c_clr_acq_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_c_clr_nt1_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_c_clr_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_c_clr_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_c_clr_nta_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_c_clr_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_c_clr_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_c_clr_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_c_clr_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_c_clr_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_c_nc_nt1_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_c_nc_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_c_nc_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_c_nc_nta_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_c_nc_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_c_nc_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_c_nc_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_c_nc_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_c_nc_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*-----------------------------------------------------------------------*/
uint32 ld1_nt1_r_r_(uint32 *unit, qword *out) {
   return ld1_r_r_(unit,out);
}
/*-----------------------------------------------------------------------*/
uint32 ld1_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*-----------------------------------------------------------------------*/
uint32 ld1_nt1_r_r_r(uint32 *unit, qword *out) {
   return ld1_r_r_r(unit,out);
}
/*-----------------------------------------------------------------------*/
uint32 ld1_nta_r_r_(uint32 *unit, qword *out) {
   *unit = MUNIT;
   out->q = (uint64)4<<37 | (uint64)0<<30 | (uint64)3<<28 |
      op.r[0]<<6 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*-----------------------------------------------------------------------*/
uint32 ld1_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*-----------------------------------------------------------------------*/
uint32 ld1_nta_r_r_r(uint32 *unit, qword *out) {
   *unit = MUNIT;
   out->q = (uint64)4<<37 | (uint64)(1*64+0)<<30 | (uint64)3<<28 |
      op.r[0]<<6 | op.r[1]<<20 | op.r[2]<<13 | op.pred;
   return IA_OK;
}
/*-----------------------------------------------------------------------*/
uint32 ld1_r_r_(uint32 *unit, qword *out) {
   *unit = MUNIT;
   out->q = (uint64)4<<37 | (uint64)0<<30 | (uint64)0<<28 |
      op.r[0]<<6 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*-----------------------------------------------------------------------*/
uint32 ld1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*-----------------------------------------------------------------------*/
uint32 ld1_r_r_r(uint32 *unit, qword *out) {
   *unit = MUNIT;
   out->q = (uint64)4<<37 | (uint64)(1*64+0)<<30 | (uint64)0<<28 |
      op.r[0]<<6 | op.r[1]<<20 | op.r[2]<<13 | op.pred;
   return IA_OK;
}
/*-----------------------------------------------------------------------*/
uint32 ld1_s_nt1_r_r_(uint32 *unit, qword *out) {
   return ld1_s_r_r_(unit,out);
}
/*-----------------------------------------------------------------------*/
uint32 ld1_s_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*-----------------------------------------------------------------------*/
uint32 ld1_s_nt1_r_r_r(uint32 *unit, qword *out) {
   return ld1_s_r_r_r(unit,out);
}
/*-----------------------------------------------------------------------*/
uint32 ld1_s_nta_r_r_(uint32 *unit, qword *out) {
   *unit = MUNIT;
   out->q = (uint64)4<<37 | (uint64)4<<30 | (uint64)3<<28 |
      op.r[0]<<6 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*-----------------------------------------------------------------------*/
uint32 ld1_s_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*-----------------------------------------------------------------------*/
uint32 ld1_s_nta_r_r_r(uint32 *unit, qword *out) {
   *unit = MUNIT;
   out->q = (uint64)4<<37 | (uint64)(1*64+4)<<30 | (uint64)3<<28 |
      op.r[0]<<6 | op.r[1]<<20 | op.r[2]<<13 | op.pred;
   return IA_OK;
}
/*-----------------------------------------------------------------------*/
uint32 ld1_s_r_r_(uint32 *unit, qword *out) {
   *unit = MUNIT;
   out->q = (uint64)4<<37 | (uint64)4<<30 | (uint64)0<<28 |
      op.r[0]<<6 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*-----------------------------------------------------------------------*/
uint32 ld1_s_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*-----------------------------------------------------------------------*/
uint32 ld1_s_r_r_r(uint32 *unit, qword *out) {
   *unit = MUNIT;
   out->q = (uint64)4<<37 | (uint64)(1*64+4)<<30 | (uint64)0<<28 |
      op.r[0]<<6 | op.r[1]<<20 | op.r[2]<<13 | op.pred;
   return IA_OK;
}
/*-----------------------------------------------------------------------*/
uint32 ld1_sa_nt1_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_sa_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_sa_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_sa_nta_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_sa_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_sa_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_sa_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_sa_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld1_sa_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_a_nt1_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_a_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_a_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_a_nta_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_a_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_a_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_a_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_a_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_a_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_acq_nt1_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_acq_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_acq_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_acq_nta_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_acq_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_acq_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_acq_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_acq_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_acq_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_bias_nt1_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_bias_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_bias_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_bias_nta_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_bias_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_bias_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_bias_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_bias_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_bias_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_c_clr_acq_nt1_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_c_clr_acq_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_c_clr_acq_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_c_clr_acq_nta_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_c_clr_acq_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_c_clr_acq_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_c_clr_acq_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_c_clr_acq_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_c_clr_acq_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_c_clr_nt1_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_c_clr_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_c_clr_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_c_clr_nta_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_c_clr_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_c_clr_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_c_clr_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_c_clr_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_c_clr_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_c_nc_nt1_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_c_nc_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_c_nc_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_c_nc_nta_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_c_nc_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_c_nc_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_c_nc_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_c_nc_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_c_nc_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*-----------------------------------------------------------------------*/
uint32 ld2_nt1_r_r_(uint32 *unit, qword *out) {
   return ld2_r_r_(unit,out);
}
/*-----------------------------------------------------------------------*/
uint32 ld2_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*-----------------------------------------------------------------------*/
uint32 ld2_nt1_r_r_r(uint32 *unit, qword *out) {
   return ld2_r_r_r(unit,out);
}
/*-----------------------------------------------------------------------*/
uint32 ld2_nta_r_r_(uint32 *unit, qword *out) {
   *unit = MUNIT;
   out->q = (uint64)4<<37 | (uint64)1<<30 | (uint64)3<<28 |
      op.r[0]<<6 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*-----------------------------------------------------------------------*/
uint32 ld2_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*-----------------------------------------------------------------------*/
uint32 ld2_nta_r_r_r(uint32 *unit, qword *out) {
   *unit = MUNIT;
   out->q = (uint64)4<<37 | (uint64)(1*64+1)<<30 | (uint64)3<<28 |
      op.r[0]<<6 | op.r[1]<<20 | op.r[2]<<13 | op.pred;
   return IA_OK;
}
/*-----------------------------------------------------------------------*/
uint32 ld2_r_r_(uint32 *unit, qword *out) {
   *unit = MUNIT;
   out->q = (uint64)4<<37 | (uint64)1<<30 | (uint64)0<<28 |
      op.r[0]<<6 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*-----------------------------------------------------------------------*/
uint32 ld2_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*-----------------------------------------------------------------------*/
uint32 ld2_r_r_r(uint32 *unit, qword *out) {
   *unit = MUNIT;
   out->q = (uint64)4<<37 | (uint64)(1*64+1)<<30 | (uint64)0<<28 |
      op.r[0]<<6 | op.r[1]<<20 | op.r[2]<<13 | op.pred;
   return IA_OK;
}
/*-----------------------------------------------------------------------*/
uint32 ld2_s_nt1_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_s_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_s_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_s_nta_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_s_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_s_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_s_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_s_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_s_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_sa_nt1_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_sa_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_sa_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_sa_nta_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_sa_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_sa_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_sa_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_sa_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld2_sa_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_a_nt1_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_a_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_a_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_a_nta_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_a_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_a_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_a_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_a_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_a_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_acq_nt1_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_acq_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_acq_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_acq_nta_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_acq_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_acq_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_acq_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_acq_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_acq_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_bias_nt1_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_bias_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_bias_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_bias_nta_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_bias_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_bias_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_bias_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_bias_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_bias_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_c_clr_acq_nt1_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_c_clr_acq_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_c_clr_acq_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_c_clr_acq_nta_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_c_clr_acq_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_c_clr_acq_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_c_clr_acq_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_c_clr_acq_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_c_clr_acq_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_c_clr_nt1_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_c_clr_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_c_clr_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_c_clr_nta_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_c_clr_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_c_clr_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_c_clr_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_c_clr_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_c_clr_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_c_nc_nt1_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_c_nc_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_c_nc_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_c_nc_nta_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_c_nc_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_c_nc_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_c_nc_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_c_nc_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_c_nc_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*-----------------------------------------------------------------------*/
uint32 ld4_nt1_r_r_(uint32 *unit, qword *out) {
   return ld4_r_r_(unit,out);
}
/*-----------------------------------------------------------------------*/
uint32 ld4_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*-----------------------------------------------------------------------*/
uint32 ld4_nt1_r_r_r(uint32 *unit, qword *out) {
   return ld4_r_r_r(unit,out);
}
/*-----------------------------------------------------------------------*/
uint32 ld4_nta_r_r_(uint32 *unit, qword *out) {
   *unit = MUNIT;
   out->q = (uint64)4<<37 | (uint64)2<<30 | (uint64)3<<28 |
      op.r[0]<<6 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*-----------------------------------------------------------------------*/
uint32 ld4_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*-----------------------------------------------------------------------*/
uint32 ld4_nta_r_r_r(uint32 *unit, qword *out) {
   *unit = MUNIT;
   out->q = (uint64)4<<37 | (uint64)(1*64+2)<<30 | (uint64)3<<28 |
      op.r[0]<<6 | op.r[1]<<20 | op.r[2]<<13 | op.pred;
   return IA_OK;
}
/*-----------------------------------------------------------------------*/
uint32 ld4_r_r_(uint32 *unit, qword *out) {
   *unit = MUNIT;
   out->q = (uint64)4<<37 | (uint64)2<<30 | (uint64)0<<28 |
      op.r[0]<<6 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*-----------------------------------------------------------------------*/
uint32 ld4_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*-----------------------------------------------------------------------*/
uint32 ld4_r_r_r(uint32 *unit, qword *out) {
   *unit = MUNIT;
   out->q = (uint64)4<<37 | (uint64)(1*64+2)<<30 | (uint64)0<<28 |
      op.r[0]<<6 | op.r[1]<<20 | op.r[2]<<13 | op.pred;
   return IA_OK;
}
/*-----------------------------------------------------------------------*/
uint32 ld4_s_nt1_r_r_(uint32 *unit, qword *out) {
   return ld4_s_r_r_(unit,out);
}
/*-----------------------------------------------------------------------*/
uint32 ld4_s_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*-----------------------------------------------------------------------*/
uint32 ld4_s_nt1_r_r_r(uint32 *unit, qword *out) {
   return ld4_s_r_r_r(unit,out);
}
/*-----------------------------------------------------------------------*/
uint32 ld4_s_nta_r_r_(uint32 *unit, qword *out) {
   *unit = MUNIT;
   out->q = (uint64)4<<37 | (uint64)6<<30 | (uint64)3<<28 |
      op.r[0]<<6 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*-----------------------------------------------------------------------*/
uint32 ld4_s_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*-----------------------------------------------------------------------*/
uint32 ld4_s_nta_r_r_r(uint32 *unit, qword *out) {
   *unit = MUNIT;
   out->q = (uint64)4<<37 | (uint64)(1*64+6)<<30 | (uint64)3<<28 |
      op.r[0]<<6 | op.r[1]<<20 | op.r[2]<<13 | op.pred;
   return IA_OK;
}
/*-----------------------------------------------------------------------*/
uint32 ld4_s_r_r_(uint32 *unit, qword *out) {
   *unit = MUNIT;
   out->q = (uint64)4<<37 | (uint64)6<<30 | (uint64)0<<28 |
      op.r[0]<<6 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*-----------------------------------------------------------------------*/
uint32 ld4_s_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*-----------------------------------------------------------------------*/
uint32 ld4_s_r_r_r(uint32 *unit, qword *out) {
   *unit = MUNIT;
   out->q = (uint64)4<<37 | (uint64)(1*64+6)<<30 | (uint64)0<<28 |
      op.r[0]<<6 | op.r[1]<<20 | op.r[2]<<13 | op.pred;
   return IA_OK;
}
/*-----------------------------------------------------------------------*/
uint32 ld4_sa_nt1_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_sa_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_sa_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_sa_nta_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_sa_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_sa_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_sa_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_sa_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld4_sa_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_a_nt1_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_a_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_a_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_a_nta_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_a_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_a_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_a_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_a_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_a_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_acq_nt1_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_acq_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_acq_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_acq_nta_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_acq_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_acq_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_acq_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_acq_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_acq_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_bias_nt1_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_bias_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_bias_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_bias_nta_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_bias_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_bias_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_bias_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_bias_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_bias_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_c_clr_acq_nt1_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_c_clr_acq_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_c_clr_acq_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_c_clr_acq_nta_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_c_clr_acq_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_c_clr_acq_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_c_clr_acq_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_c_clr_acq_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_c_clr_acq_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_c_clr_nt1_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_c_clr_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_c_clr_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_c_clr_nta_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_c_clr_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_c_clr_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_c_clr_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_c_clr_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_c_clr_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_c_nc_nt1_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_c_nc_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_c_nc_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_c_nc_nta_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_c_nc_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_c_nc_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_c_nc_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_c_nc_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_c_nc_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_a_nt1_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_a_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_a_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_a_nta_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_a_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_a_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_a_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_a_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_a_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_acq_nt1_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_acq_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_acq_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_acq_nta_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_acq_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_acq_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_acq_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_acq_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_acq_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_bias_nt1_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_bias_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_bias_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_bias_nta_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_bias_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_bias_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_bias_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_bias_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_bias_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_c_clr_acq_nt1_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_c_clr_acq_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_c_clr_acq_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_c_clr_acq_nta_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_c_clr_acq_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_c_clr_acq_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_c_clr_acq_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_c_clr_acq_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_c_clr_acq_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_c_clr_nt1_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_c_clr_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_c_clr_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_c_clr_nta_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_c_clr_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_c_clr_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_c_clr_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_c_clr_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_c_clr_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_c_nc_nt1_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_c_nc_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_c_nc_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_c_nc_nta_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_c_nc_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_c_nc_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_c_nc_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_c_nc_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_c_nc_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_nt1_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_nta_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_s_nt1_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_s_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_s_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_s_nta_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_s_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_s_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_s_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_s_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_s_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_sa_nt1_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_sa_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_sa_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_sa_nta_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_sa_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_sa_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_sa_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_sa_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_fill_sa_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*-----------------------------------------------------------------------*/
uint32 ld8_nt1_r_r_(uint32 *unit, qword *out) {
   return ld8_r_r_(unit,out);
}
/*-----------------------------------------------------------------------*/
uint32 ld8_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*-----------------------------------------------------------------------*/
uint32 ld8_nt1_r_r_r(uint32 *unit, qword *out) {
   return ld8_r_r_r(unit,out);
}
/*-----------------------------------------------------------------------*/
uint32 ld8_nta_r_r_(uint32 *unit, qword *out) {
   *unit = MUNIT;
   out->q = (uint64)4<<37 | (uint64)3<<30 | (uint64)3<<28 |
      op.r[0]<<6 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*-----------------------------------------------------------------------*/
uint32 ld8_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*-----------------------------------------------------------------------*/
uint32 ld8_nta_r_r_r(uint32 *unit, qword *out) {
   *unit = MUNIT;
   out->q = (uint64)4<<37 | (uint64)(1*64+3)<<30 | (uint64)3<<28 |
      op.r[0]<<6 | op.r[1]<<20 | op.r[2]<<13 | op.pred;
   return IA_OK;
}
/*-----------------------------------------------------------------------*/
uint32 ld8_r_r_(uint32 *unit, qword *out) {
   *unit = MUNIT;
   out->q = (uint64)4<<37 | (uint64)3<<30 | (uint64)0<<28 |
      op.r[0]<<6 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*-----------------------------------------------------------------------*/
uint32 ld8_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*-----------------------------------------------------------------------*/
uint32 ld8_r_r_r(uint32 *unit, qword *out) {
   *unit = MUNIT;
   out->q = (uint64)4<<37 | (uint64)(1*64+3)<<30 | (uint64)0<<28 |
      op.r[0]<<6 | op.r[1]<<20 | op.r[2]<<13 | op.pred;
   return IA_OK;
}
/*-----------------------------------------------------------------------*/
uint32 ld8_s_nt1_r_r_(uint32 *unit, qword *out) {
   return ld8_s_r_r_(unit,out);
}
/*-----------------------------------------------------------------------*/
uint32 ld8_s_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*-----------------------------------------------------------------------*/
uint32 ld8_s_nt1_r_r_r(uint32 *unit, qword *out) {
   return ld8_s_r_r_r(unit,out);
}
/*-----------------------------------------------------------------------*/
uint32 ld8_s_nta_r_r_(uint32 *unit, qword *out) {
   *unit = MUNIT;
   out->q = (uint64)4<<37 | (uint64)7<<30 | (uint64)3<<28 |
      op.r[0]<<6 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*-----------------------------------------------------------------------*/
uint32 ld8_s_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*-----------------------------------------------------------------------*/
uint32 ld8_s_nta_r_r_r(uint32 *unit, qword *out) {
   *unit = MUNIT;
   out->q = (uint64)4<<37 | (uint64)(1*64+7)<<30 | (uint64)3<<28 |
      op.r[0]<<6 | op.r[1]<<20 | op.r[2]<<13 | op.pred;
   return IA_OK;
}
/*-----------------------------------------------------------------------*/
uint32 ld8_s_r_r_(uint32 *unit, qword *out) {
   *unit = MUNIT;
   out->q = (uint64)4<<37 | (uint64)7<<30 | (uint64)0<<28 |
      op.r[0]<<6 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*-----------------------------------------------------------------------*/
uint32 ld8_s_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*-----------------------------------------------------------------------*/
uint32 ld8_s_r_r_r(uint32 *unit, qword *out) {
   *unit = MUNIT;
   out->q = (uint64)4<<37 | (uint64)(1*64+7)<<30 | (uint64)0<<28 |
      op.r[0]<<6 | op.r[1]<<20 | op.r[2]<<13 | op.pred;
   return IA_OK;
}
/*-----------------------------------------------------------------------*/
uint32 ld8_sa_nt1_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_sa_nt1_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_sa_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_sa_nta_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_sa_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_sa_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_sa_r_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_sa_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ld8_sa_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_a_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_a_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_a_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_a_nt1_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_a_nt1_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_a_nt1_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_a_nta_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_a_nta_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_a_nta_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_c_clr_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_c_clr_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_c_clr_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_c_clr_nt1_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_c_clr_nt1_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_c_clr_nt1_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_c_clr_nta_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_c_clr_nta_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_c_clr_nta_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_c_nc_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_c_nc_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_c_nc_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_c_nc_nt1_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_c_nc_nt1_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_c_nc_nt1_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_c_nc_nta_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_c_nc_nta_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_c_nc_nta_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_nt1_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_nt1_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_nt1_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_nta_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_nta_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_nta_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_s_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_s_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_s_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_s_nt1_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_s_nt1_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_s_nt1_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_s_nta_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_s_nta_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_s_nta_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_sa_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_sa_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_sa_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_sa_nt1_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_sa_nt1_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_sa_nt1_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_sa_nta_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_sa_nta_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf8_sa_nta_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf_fill_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf_fill_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf_fill_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf_fill_nt1_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf_fill_nt1_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf_fill_nt1_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf_fill_nta_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf_fill_nta_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldf_fill_nta_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_a_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_a_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_a_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_a_nt1_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_a_nt1_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_a_nt1_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_a_nta_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_a_nta_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_a_nta_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_c_clr_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_c_clr_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_c_clr_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_c_clr_nt1_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_c_clr_nt1_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_c_clr_nt1_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_c_clr_nta_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_c_clr_nta_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_c_clr_nta_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_c_nc_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_c_nc_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_c_nc_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_c_nc_nt1_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_c_nc_nt1_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_c_nc_nt1_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_c_nc_nta_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_c_nc_nta_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_c_nc_nta_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_nt1_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_nt1_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_nt1_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_nta_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_nta_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_nta_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_s_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_s_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_s_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_s_nt1_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_s_nt1_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_s_nt1_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_s_nta_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_s_nta_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_s_nta_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_sa_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_sa_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_sa_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_sa_nt1_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_sa_nt1_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_sa_nt1_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_sa_nta_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_sa_nta_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfd_sa_nta_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_a_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_a_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_a_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_a_nt1_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_a_nt1_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_a_nt1_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_a_nta_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_a_nta_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_a_nta_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_c_clr_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_c_clr_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_c_clr_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_c_clr_nt1_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_c_clr_nt1_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_c_clr_nt1_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_c_clr_nta_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_c_clr_nta_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_c_clr_nta_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_c_nc_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_c_nc_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_c_nc_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_c_nc_nt1_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_c_nc_nt1_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_c_nc_nt1_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_c_nc_nta_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_c_nc_nta_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_c_nc_nta_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_nt1_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_nt1_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_nt1_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_nta_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_nta_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_nta_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_s_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_s_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_s_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_s_nt1_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_s_nt1_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_s_nt1_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_s_nta_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_s_nta_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_s_nta_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_sa_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_sa_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_sa_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_sa_nt1_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_sa_nt1_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_sa_nt1_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_sa_nta_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_sa_nta_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfe_sa_nta_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfp8_a_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfp8_a_f_f_r_16(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfp8_a_nt1_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfp8_a_nt1_f_f_r_16(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfp8_a_nta_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfp8_a_nta_f_f_r_16(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfp8_c_clr_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfp8_c_clr_f_f_r_16(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfp8_c_clr_nt1_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfp8_c_clr_nt1_f_f_r_16(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfp8_c_clr_nta_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfp8_c_clr_nta_f_f_r_16(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfp8_c_nc_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfp8_c_nc_f_f_r_16(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfp8_c_nc_nt1_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfp8_c_nc_nt1_f_f_r_16(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfp8_c_nc_nta_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfp8_c_nc_nta_f_f_r_16(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfp8_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfp8_f_f_r_16(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfp8_nt1_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfp8_nt1_f_f_r_16(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfp8_nta_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfp8_nta_f_f_r_16(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfp8_s_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfp8_s_f_f_r_16(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfp8_s_nt1_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfp8_s_nt1_f_f_r_16(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfp8_s_nta_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfp8_s_nta_f_f_r_16(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfp8_sa_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfp8_sa_f_f_r_16(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfp8_sa_nt1_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfp8_sa_nt1_f_f_r_16(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfp8_sa_nta_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfp8_sa_nta_f_f_r_16(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfpd_a_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfpd_a_f_f_r_16(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfpd_a_nt1_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfpd_a_nt1_f_f_r_16(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfpd_a_nta_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfpd_a_nta_f_f_r_16(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfpd_c_clr_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfpd_c_clr_f_f_r_16(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfpd_c_clr_nt1_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfpd_c_clr_nt1_f_f_r_16(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfpd_c_clr_nta_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfpd_c_clr_nta_f_f_r_16(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfpd_c_nc_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfpd_c_nc_f_f_r_16(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfpd_c_nc_nt1_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfpd_c_nc_nt1_f_f_r_16(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfpd_c_nc_nta_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfpd_c_nc_nta_f_f_r_16(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfpd_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfpd_f_f_r_16(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfpd_nt1_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfpd_nt1_f_f_r_16(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfpd_nta_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfpd_nta_f_f_r_16(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfpd_s_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfpd_s_f_f_r_16(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfpd_s_nt1_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfpd_s_nt1_f_f_r_16(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfpd_s_nta_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfpd_s_nta_f_f_r_16(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfpd_sa_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfpd_sa_f_f_r_16(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfpd_sa_nt1_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfpd_sa_nt1_f_f_r_16(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfpd_sa_nta_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfpd_sa_nta_f_f_r_16(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfps_a_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfps_a_f_f_r_8(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfps_a_nt1_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfps_a_nt1_f_f_r_8(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfps_a_nta_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfps_a_nta_f_f_r_8(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfps_c_clr_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfps_c_clr_f_f_r_8(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfps_c_clr_nt1_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfps_c_clr_nt1_f_f_r_8(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfps_c_clr_nta_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfps_c_clr_nta_f_f_r_8(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfps_c_nc_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfps_c_nc_f_f_r_8(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfps_c_nc_nt1_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfps_c_nc_nt1_f_f_r_8(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfps_c_nc_nta_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfps_c_nc_nta_f_f_r_8(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfps_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfps_f_f_r_8(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfps_nt1_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfps_nt1_f_f_r_8(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfps_nta_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfps_nta_f_f_r_8(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfps_s_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfps_s_f_f_r_8(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfps_s_nt1_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfps_s_nt1_f_f_r_8(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfps_s_nta_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfps_s_nta_f_f_r_8(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfps_sa_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfps_sa_f_f_r_8(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfps_sa_nt1_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfps_sa_nt1_f_f_r_8(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfps_sa_nta_f_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfps_sa_nta_f_f_r_8(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_a_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_a_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_a_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_a_nt1_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_a_nt1_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_a_nt1_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_a_nta_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_a_nta_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_a_nta_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_c_clr_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_c_clr_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_c_clr_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_c_clr_nt1_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_c_clr_nt1_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_c_clr_nt1_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_c_clr_nta_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_c_clr_nta_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_c_clr_nta_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_c_nc_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_c_nc_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_c_nc_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_c_nc_nt1_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_c_nc_nt1_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_c_nc_nt1_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_c_nc_nta_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_c_nc_nta_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_c_nc_nta_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_nt1_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_nt1_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_nt1_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_nta_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_nta_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_nta_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_s_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_s_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_s_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_s_nt1_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_s_nt1_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_s_nt1_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_s_nta_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_s_nta_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_s_nta_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_sa_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_sa_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_sa_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_sa_nt1_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_sa_nt1_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_sa_nt1_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_sa_nta_f_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_sa_nta_f_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 ldfs_sa_nta_f_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_excl_nt1_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_excl_nt1_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_excl_nt1_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_excl_nt2_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_excl_nt2_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_excl_nt2_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_excl_nta_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_excl_nta_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_excl_nta_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_excl_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_excl_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_excl_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_fault_excl_nt1_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_fault_excl_nt1_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_fault_excl_nt1_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_fault_excl_nt2_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_fault_excl_nt2_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_fault_excl_nt2_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_fault_excl_nta_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_fault_excl_nta_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_fault_excl_nta_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_fault_excl_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_fault_excl_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_fault_excl_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_fault_nt1_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_fault_nt1_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_fault_nt1_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_fault_nt2_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_fault_nt2_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_fault_nt2_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_fault_nta_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_fault_nta_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_fault_nta_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_fault_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_fault_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_fault_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_nt1_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_nt1_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_nt1_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_nt2_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_nt2_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_nt2_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_nta_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_nta_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_nta_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 lfetch_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 mf(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 mf_a(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*------------------------------------------------------------------------*/
uint32 mix1_l_r_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(0*256+0*32+0*16+2*64+2+2*4)<<28
           | op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*------------------------------------------------------------------------*/
uint32 mix1_r_r_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(0*256+0*32+0*16+2*64+0+2*4)<<28
           | op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*------------------------------------------------------------------------*/
uint32 mix2_l_r_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(0*256+1*32+0*16+2*64+2+2*4)<<28
           | op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*------------------------------------------------------------------------*/
uint32 mix2_r_r_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(0*256+1*32+0*16+2*64+0+2*4)<<28
           | op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*------------------------------------------------------------------------*/
uint32 mix4_l_r_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(1*256+0*32+0*16+2*64+2+2*4)<<28
           | op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*------------------------------------------------------------------------*/
uint32 mix4_r_r_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(1*256+0*32+0*16+2*64+0+2*4)<<28
           | op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*------------------------------------------------------------------------*/
uint32 mov_ar_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*----------------------------------------------------------------------*/   
uint32 mov_b_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)0<<37 | (uint64)(7)<<33 | op.b[0]<<6
                 | op.r[0]<<13 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 mov_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*----------------------------------------------------------------------*/   
uint32 mov_i_ar_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)0<<37 | (uint64)(0*64+0x2a)<<27 | op.r[0]<<13
                 | op.ar[0]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 mov_i_r_ar(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)0<<37 | (uint64)(0*64+0x2a)<<27 | op.r[0]<<6
                 | op.ar[0]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 mov_i_r_i8(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)0<<37 | (uint64)(0*64+0x32)<<27 | (op.i[0]&127)<<13
                 | op.ar[0]<<20 | ((op.i[0]>>7)&1)<<36 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 mov_m_ar_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 mov_m_r_ar(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 mov_m_r_i8(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*----------------------------------------------------------------------*/   
uint32 mov_pr_r_i17(uint32 *unit, qword *out) {
   *unit = IUNIT; op.i[0] >>= 1;
   out->q = (uint64)0<<37 | (uint64)(3)<<33 | op.r[0]<<13 | (op.i[0]&127)<<6
                 | ((op.i[0]>>7)&0xff)<<24 | op.pred
                 | ((op.i[0]>>15)&1)<<36;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 mov_pr_rot_i44(uint32 *unit, qword *out) {
   *unit = IUNIT; op.i[0] >>= 16;
   out->q = (uint64)0<<37 | (uint64)(2)<<33 | (op.i[0]&0x7ffffff)<<6
                 | ((op.i[0]>>27)&1)<<36 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 mov_psr_um_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 mov_r_ar(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*----------------------------------------------------------------------*/   
uint32 mov_r_b(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)0<<37 | (uint64)(0*64+0x31)<<27 | op.b[0]<<13
                 | op.r[0]<<6 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 mov_r_cpuid_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 mov_r_i22(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 mov_r_i8(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*----------------------------------------------------------------------*/   
uint32 mov_r_ip(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)0<<37 | (uint64)(0*64+0x30)<<27 | op.r[0]<<6 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 mov_r_pmd_r_(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*----------------------------------------------------------------------*/   
uint32 mov_r_pr(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)0<<37 | (uint64)(0*64+0x33)<<27 | op.r[0]<<6 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 mov_r_psr_um(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*----------------------------------------------------------------------*/   
uint32 mov_r_r(uint32 *unit, qword *out) {
   op.r[2] = 0;
   return add_r_r_r(unit, out);
}   
/*----------------------------------------------------------------------*/   
uint32 mov_ret_b_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 mov_ret_r_b(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 movl_r_i64(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*----------------------------------------------------------------------*/   
uint32 mux1_r_r_alt(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(0*256+0*32+0*16+3*64+2+2*4)<<28 |
                 op.r[0]<<6 | op.r[1]<<13 | 10<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 mux1_r_r_brcst(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(0*256+0*32+0*16+3*64+2+2*4)<<28 |
                 op.r[0]<<6 | op.r[1]<<13 | 0<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 mux1_r_r_mix(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(0*256+0*32+0*16+3*64+2+2*4)<<28 |
                 op.r[0]<<6 | op.r[1]<<13 | 8<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 mux1_r_r_rev(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(0*256+0*32+0*16+3*64+2+2*4)<<28 |
                 op.r[0]<<6 | op.r[1]<<13 | 11<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 mux1_r_r_shuf(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(0*256+0*32+0*16+3*64+2+2*4)<<28 |
                 op.r[0]<<6 | op.r[1]<<13 | 9<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 mux2_r_r_u8(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(0*256+1*32+0*16+3*64+2+2*4)<<28 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 nop_b_i21(uint32 *unit, qword *out) {
   *unit = BUNIT;
   out->q = (uint64)2<<37 | (op.i[0]&0xfffff)<<6 |
          ((op.i[0]>>20)&1)<<36 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 nop_f_i21(uint32 *unit, qword *out) {
   *unit = FUNIT;
   out->q = (uint64)1<<27 | (op.i[0]&0xfffff)<<6 |
          ((op.i[0]>>20)&1)<<36 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 nop_i21(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*----------------------------------------------------------------------*/   
uint32 nop_i_i21(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)1<<27 | (op.i[0]&0xfffff)<<6 |
          ((op.i[0]>>20)&1)<<36 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 nop_m_i21(uint32 *unit, qword *out) {
   *unit = MUNIT;
   out->q = (uint64)1<<27 | (op.i[0]&0xfffff)<<6 |
          ((op.i[0]>>20)&1)<<36 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 nop_x_i62(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*----------------------------------------------------------------------*/   
uint32 or_r_i8_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (0xb*4+2)<<27 | op.r[0]<<6 | (op.i[0]&127)<<13
                    | ((op.i[0]>>7)&1)<<36 | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 or_r_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (3*4+2)<<27 | op.r[0]<<6 | op.r[1]<<13
                    | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 pack2_sss_r_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(0*256+1*32+0*16+2*64+2+0*4)<<28 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 pack2_uss_r_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(0*256+1*32+0*16+2*64+0+0*4)<<28 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 pack4_sss_r_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(1*256+0*32+0*16+2*64+2+0*4)<<28 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 padd1_r_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (uint64)(1*256+0*128+0*64+0*4+0)<<27 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 padd1_sss_r_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (uint64)(1*256+0*128+0*64+0*4+1)<<27 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 padd1_uus_r_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (uint64)(1*256+0*128+0*64+0*4+3)<<27 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 padd1_uuu_r_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (uint64)(1*256+0*128+0*64+0*4+2)<<27 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 padd2_r_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (uint64)(1*256+0*128+1*64+0*4+0)<<27 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 padd2_sss_r_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (uint64)(1*256+0*128+1*64+0*4+1)<<27 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 padd2_uus_r_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (uint64)(1*256+0*128+1*64+0*4+3)<<27 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 padd2_uuu_r_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (uint64)(1*256+0*128+1*64+0*4+2)<<27 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 padd4_r_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (uint64)(1*256+1*128+0*64+0*4+0)<<27 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 pavg1_r_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (uint64)(1*256+0*128+0*64+2*4+2)<<27 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 pavg1_raz_r_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (uint64)(1*256+0*128+0*64+2*4+3)<<27 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 pavg2_r_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (uint64)(1*256+0*128+1*64+2*4+2)<<27 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 pavg2_raz_r_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (uint64)(1*256+0*128+1*64+2*4+3)<<27 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 pavgsub1_r_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (uint64)(1*256+0*128+0*64+3*4+2)<<27 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 pavgsub2_r_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (uint64)(1*256+0*128+1*64+3*4+2)<<27 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 pcmp1_eq_r_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (uint64)(1*256+0*128+0*64+9*4+0)<<27 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 pcmp1_gt_r_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (uint64)(1*256+0*128+0*64+9*4+1)<<27 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 pcmp2_eq_r_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (uint64)(1*256+0*128+1*64+9*4+0)<<27 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 pcmp2_gt_r_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (uint64)(1*256+0*128+1*64+9*4+1)<<27 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 pcmp4_eq_r_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (uint64)(1*256+1*128+0*64+9*4+0)<<27 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 pcmp4_gt_r_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (uint64)(1*256+1*128+0*64+9*4+1)<<27 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 pmax1_u_r_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(0*256+0*32+0*16+2*64+1+1*4)<<28 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 pmax2_r_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(0*256+1*32+0*16+2*64+3+1*4)<<28 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 pmin1_u_r_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(0*256+0*32+0*16+2*64+1+0*4)<<28 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 pmin2_r_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(0*256+1*32+0*16+2*64+3+0*4)<<28 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*------------------------------------------------------------------------*/
uint32 pmpy2_l_r_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(0*256+1*32+0*16+2*64+3+3*4)<<28
           | op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*------------------------------------------------------------------------*/
uint32 pmpy2_r_r_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(0*256+1*32+0*16+2*64+1+3*4)<<28
           | op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*------------------------------------------------------------------------*/
uint32 pmpyshr2_r_r_r_u2(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(0*256+1*32+0*16+0*64+3)<<28
           | op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred
           | op.i[0]<<30;
   return IA_OK;
}
/*------------------------------------------------------------------------*/
uint32 pmpyshr2_u_r_r_r_u2(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(0*256+1*32+0*16+0*64+1)<<28
           | op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred
           | op.i[0]<<30;
   return IA_OK;
}
/*------------------------------------------------------------------------*/
uint32 popcnt_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(0*256+1*32+0*16+1*64+1+2*4)<<28 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 psad1_r_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(0*256+0*32+0*16+2*64+3+2*4)<<28 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 pshl2_r_r_u5(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*----------------------------------------------------------------------*/   
uint32 pshl2_r_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(0*256+1*32+0*16+0*64+0+1*4)<<28 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 pshl4_r_r_u5(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*----------------------------------------------------------------------*/   
uint32 pshl4_r_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(1*256+0*32+0*16+0*64+0+1*4)<<28 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 pshladd2_r_r_u2_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (uint64)(1*256+0*128+1*64+4*4+op.i[0])<<27 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 pshr2_r_r_u5(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(0*256+1*32+0*16+1*64+3+0*4)<<28 |
                 op.r[0]<<6 | op.r[1]<<14 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 pshr2_r_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(0*256+1*32+0*16+0*64+2+0*4)<<28 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 pshr2u_r_r_u5(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(0*256+1*32+0*16+1*64+1+0*4)<<28 |
                 op.r[0]<<6 | op.r[1]<<14 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 pshr2u_r_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(0*256+1*32+0*16+0*64+0+0*4)<<28 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 pshr4_r_r_u5(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(1*256+0*32+0*16+1*64+3+0*4)<<28 |
                 op.r[0]<<6 | op.r[1]<<14 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 pshr4_r_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(0*256+1*32+0*16+0*64+2+0*4)<<28 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 pshr4u_r_r_u5(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(1*256+0*32+0*16+0*64+0+0*4)<<28 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 pshr4u_r_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(1*256+0*32+0*16+1*64+1+0*4)<<28 |
                 op.r[0]<<6 | op.r[1]<<14 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 pshradd2_r_r_u2_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 psub1_r_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (uint64)(1*256+0*128+0*64+1*4+0)<<27 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 psub1_sss_r_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (uint64)(1*256+0*128+0*64+1*4+1)<<27 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 psub1_uus_r_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (uint64)(1*256+0*128+0*64+1*4+3)<<27 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 psub1_uuu_r_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (11*256+0*128+0*64+1*4+2)<<27 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 psub2_r_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (uint64)(1*256+0*128+1*64+1*4+0)<<27 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 psub2_sss_r_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (uint64)(1*256+0*128+1*64+1*4+1)<<27 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 psub2_uus_r_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (uint64)(1*256+0*128+1*64+1*4+3)<<27 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 psub2_uuu_r_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (uint64)(1*256+0*128+1*64+1*4+2)<<27 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 psub4_r_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (uint64)(1*256+1*128+0*64+1*4+0)<<27 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 rum_i24(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 setf_d_f_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 setf_exp_f_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 setf_s_f_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 setf_sig_f_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 shl_r_r_u6(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*----------------------------------------------------------------------*/   
uint32 shl_r_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(1*256+1*32+0*16+0*64+0+1*4)<<28 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 shladd_r_r_u2_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (4*4+op.i[0])<<27 | op.r[0]<<6 | op.r[1]<<13
                    | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 shladdp4_r_r_u2_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (6*4+op.i[0])<<27 | op.r[0]<<6 | op.r[1]<<13
                    | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 shr_r_r_u6(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*----------------------------------------------------------------------*/   
uint32 shr_r_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(1*256+1*32+0*16+0*64+2+0*4)<<28 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 shr_u_r_r_u6(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*----------------------------------------------------------------------*/   
uint32 shr_u_r_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(1*256+1*32+0*16+0*64+0+0*4)<<28 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 shrp_r_r_r_u6(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)5<<37 | (uint64)(3*2+0)<<33 | op.i[0]<<27 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 srlz_i(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*----------------------------------------------------------------------*/   
uint32 st1_nta_r_r(uint32 *unit, qword *out) {
   *unit = MUNIT;
   out->q = (uint64)4<<37 | (uint64)0x30<<30 | (uint64)3<<28 |
      op.r[0]<<20 | op.r[1]<<13 | op.pred;
   return IA_OK;
}
/*-----------------------------------------------------------------------*/
uint32 st1_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*----------------------------------------------------------------------*/   
uint32 st1_r_r(uint32 *unit, qword *out) {
   *unit = MUNIT+4;
   out->q = (uint64)4<<37 | (uint64)0x30<<30 | (uint64)0<<28 |
      op.r[0]<<20 | op.r[1]<<13 | op.pred;
   return IA_OK;
}
/*-----------------------------------------------------------------------*/
uint32 st1_r_r_i9(uint32 *unit, qword *out) {
   *unit = MUNIT+5;
   out->q = (uint64)5<<37 | (uint64)0x30<<30 | (uint64)0<<28 |
      op.r[0]<<20 | op.r[1]<<13 | (op.i[0]&127)<<6 | ((op.i[0]>>7)&1)<<27 |
           ((op.i[0]>>8)&1)<<36 | op.pred;
   return IA_OK;
}
/*-----------------------------------------------------------------------*/
uint32 st1_rel_nta_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 st1_rel_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 st1_rel_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 st1_rel_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*-----------------------------------------------------------------------*/
uint32 st2_nta_r_r(uint32 *unit, qword *out) {
   *unit = MUNIT;
   out->q = (uint64)4<<37 | (uint64)0x31<<30 | (uint64)3<<28 |
      op.r[0]<<20 | op.r[1]<<13 | op.pred;
   return IA_OK;
}
/*-----------------------------------------------------------------------*/
uint32 st2_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*-----------------------------------------------------------------------*/
uint32 st2_r_r(uint32 *unit, qword *out) {
   *unit = MUNIT;
   out->q = (uint64)4<<37 | (uint64)0x31<<30 | (uint64)0<<28 |
      op.r[0]<<20 | op.r[1]<<13 | op.pred;
   return IA_OK;
}
/*-----------------------------------------------------------------------*/
uint32 st2_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 st2_rel_nta_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 st2_rel_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 st2_rel_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 st2_rel_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*-----------------------------------------------------------------------*/
uint32 st4_nta_r_r(uint32 *unit, qword *out) {
   *unit = MUNIT;
   out->q = (uint64)4<<37 | (uint64)0x32<<30 | (uint64)3<<28 |
      op.r[0]<<20 | op.r[1]<<13 | op.pred;
   return IA_OK;
}
/*-----------------------------------------------------------------------*/
uint32 st4_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*-----------------------------------------------------------------------*/
uint32 st4_r_r(uint32 *unit, qword *out) {
   *unit = MUNIT;
   out->q = (uint64)4<<37 | (uint64)0x32<<30 | (uint64)0<<28 |
      op.r[0]<<20 | op.r[1]<<13 | op.pred;
   return IA_OK;
}
/*-----------------------------------------------------------------------*/
uint32 st4_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 st4_rel_nta_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 st4_rel_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 st4_rel_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 st4_rel_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*-----------------------------------------------------------------------*/
uint32 st8_nta_r_r(uint32 *unit, qword *out) {
   *unit = MUNIT;
   out->q = (uint64)4<<37 | (uint64)0x33<<30 | (uint64)3<<28 |
      op.r[0]<<20 | op.r[1]<<13 | op.pred;
   return IA_OK;
}
/*-----------------------------------------------------------------------*/
uint32 st8_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*-----------------------------------------------------------------------*/
uint32 st8_r_r(uint32 *unit, qword *out) {
   *unit = MUNIT;
   out->q = (uint64)4<<37 | (uint64)0x33<<30 | (uint64)0<<28 |
      op.r[0]<<20 | op.r[1]<<13 | op.pred;
   return IA_OK;
}
/*-----------------------------------------------------------------------*/
uint32 st8_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 st8_rel_nta_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 st8_rel_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 st8_rel_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 st8_rel_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 st8_spill_nta_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 st8_spill_nta_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 st8_spill_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 st8_spill_r_r_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 stf8_nta_r_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 stf8_nta_r_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 stf8_r_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 stf8_r_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 stf_spill_nta_r_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 stf_spill_nta_r_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 stf_spill_r_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 stf_spill_r_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 stfd_nta_r_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 stfd_nta_r_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 stfd_r_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 stfd_r_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 stfe_nta_r_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 stfe_nta_r_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 stfe_r_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 stfe_r_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 stfs_nta_r_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 stfs_nta_r_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 stfs_r_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 stfs_r_f_i9(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*-----------------------------------------------------------------------*/
uint32 sub_r_i8_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (uint64)(9*4+1)<<27 | op.r[0]<<6 |
             (op.i[0]&127)<<13 | ((op.i[0]>>7)&1)<<36 |
             op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*-----------------------------------------------------------------------*/
uint32 sub_r_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (uint64)(1*4+1)<<27 | op.r[0]<<6 | op.r[1]<<13
                    | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*-----------------------------------------------------------------------*/
uint32 sub_r_r_r_1(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (uint64)(1*4+0)<<27 | op.r[0]<<6 | op.r[1]<<13
                    | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*-----------------------------------------------------------------------*/
uint32 sum_i24(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*-----------------------------------------------------------------------*/
uint32 sxt1_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)0<<37 | (uint64)(0*64+0x14)<<27 | op.r[0]<<6
                    | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 sxt2_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)0<<37 | (uint64)(0*64+0x15)<<27 | op.r[0]<<6
                    | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 sxt4_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)0<<37 | (uint64)(0*64+0x16)<<27 | op.r[0]<<6
                    | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 sync_i(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*----------------------------------------------------------------------*/   
uint32 tbit_nz_and_orcm_p_p_r_u6(uint32 *unit, qword *out) {
   SWAPI(op.p[0],op.p[1]);
   return tbit_z_or_andcm_p_p_r_u6(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 tbit_nz_and_p_p_r_u6(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)5<<37 | (uint64)(0*8+0*4+1*32)<<33 |
            (0*2+1)<<12 | op.p[0]<<6 | op.p[1]<<27 | op.r[0]<<20 |
            op.i[0]<<14 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 tbit_nz_andcm_p_p_r_u6(uint32 *unit, qword *out) {
   return tbit_z_and_p_p_r_u6(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 tbit_nz_or_andcm_p_p_r_u6(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)5<<37 | (uint64)(0*8+1*4+1*32)<<33 |
            (0*2+1)<<12 | op.p[0]<<6 | op.p[1]<<27 | op.r[0]<<20 |
            op.i[0]<<14 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/ 
uint32 tbit_nz_or_p_p_r_u6(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)5<<37 | (uint64)(0*8+1*4+0*32)<<33 |
            (0*2+1)<<12 | op.p[0]<<6 | op.p[1]<<27 | op.r[0]<<20 |
            op.i[0]<<14 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 tbit_nz_orcm_p_p_r_u6(uint32 *unit, qword *out) {
   return tbit_z_or_p_p_r_u6(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 tbit_nz_p_p_r_u6(uint32 *unit, qword *out) {
   SWAPI(op.p[0],op.p[1]);
   return tbit_z_p_p_r_u6(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 tbit_nz_unc_p_p_r_u6(uint32 *unit, qword *out) {
   SWAPI(op.p[0],op.p[1]);
   return tbit_z_unc_p_p_r_u6(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 tbit_z_and_orcm_p_p_r_u6(uint32 *unit, qword *out) {
   SWAPI(op.p[0],op.p[1]);
   return tbit_nz_or_andcm_p_p_r_u6(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 tbit_z_and_p_p_r_u6(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)5<<37 | (uint64)(0*8+0*4+1*32)<<33 |
            (0*2+0)<<12 | op.p[0]<<6 | op.p[1]<<27 | op.r[0]<<20 |
            op.i[0]<<14 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 tbit_z_andcm_p_p_r_u6(uint32 *unit, qword *out) {
   return tbit_nz_and_p_p_r_u6(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 tbit_z_or_andcm_p_p_r_u6(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)5<<37 | (uint64)(0*8+1*4+1*32)<<33 |
            (0*2+0)<<12 | op.p[0]<<6 | op.p[1]<<27 | op.r[0]<<20 |
            op.i[0]<<14 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 tbit_z_or_p_p_r_u6(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)5<<37 | (uint64)(0*8+1*4+0*32)<<33 |
            (0*2+0)<<12 | op.p[0]<<6 | op.p[1]<<27 | op.r[0]<<20 |
            op.i[0]<<14 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 tbit_z_orcm_p_p_r_u6(uint32 *unit, qword *out) {
   return tbit_nz_or_p_p_r_u6(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 tbit_z_p_p_r_u6(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)5<<37 | (uint64)(0*8+0*4+0*32)<<33 |
            (0*2+0)<<12 | op.p[0]<<6 | op.p[1]<<27 | op.r[0]<<20 |
            op.i[0]<<14 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 tbit_z_unc_p_p_r_u6(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)5<<37 | (uint64)(0*8+0*4+0*32)<<33 |
            (0*2+1)<<12 | op.p[0]<<6 | op.p[1]<<27 | op.r[0]<<20 |
            op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 tnat_nz_and_orcm_p_p_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0],op.p[1]);
   return tnat_z_or_andcm_p_p_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 tnat_nz_and_p_p_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)5<<37 | (uint64)(0*8+0*4+1*32)<<33 |
            (1*2+1)<<12 | op.p[0]<<6 | op.p[1]<<27 | op.r[0]<<20 |
            op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 tnat_nz_andcm_p_p_r(uint32 *unit, qword *out) {
   return tnat_z_and_p_p_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 tnat_nz_or_andcm_p_p_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)5<<37 | (uint64)(0*8+1*4+1*32)<<33 |
            (1*2+1)<<12 | op.p[0]<<6 | op.p[1]<<27 | op.r[0]<<20 |
            op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/ 
uint32 tnat_nz_or_p_p_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)5<<37 | (uint64)(0*8+1*4+0*32)<<33 |
            (1*2+1)<<12 | op.p[0]<<6 | op.p[1]<<27 | op.r[0]<<20 |
            op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 tnat_nz_orcm_p_p_r(uint32 *unit, qword *out) {
   return tnat_z_or_p_p_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 tnat_nz_p_p_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0],op.p[1]);
   return tnat_z_p_p_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 tnat_nz_unc_p_p_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0],op.p[1]);
   return tnat_z_unc_p_p_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 tnat_z_and_orcm_p_p_r(uint32 *unit, qword *out) {
   SWAPI(op.p[0],op.p[1]);
   return tnat_nz_or_andcm_p_p_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 tnat_z_and_p_p_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)5<<37 | (uint64)(0*8+0*4+1*32)<<33 |
            (1*2+0)<<12 | op.p[0]<<6 | op.p[1]<<27 | op.r[0]<<20 |
            op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 tnat_z_andcm_p_p_r(uint32 *unit, qword *out) {
   return tnat_nz_and_p_p_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 tnat_z_or_andcm_p_p_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)5<<37 | (uint64)(0*8+1*4+1*32)<<33 |
            (1*2+0)<<12 | op.p[0]<<6 | op.p[1]<<27 | op.r[0]<<20 |
            op.i[0]<<14 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 tnat_z_or_p_p_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)5<<37 | (uint64)(0*8+1*4+0*32)<<33 |
            (1*2+0)<<12 | op.p[0]<<6 | op.p[1]<<27 | op.r[0]<<20 |
            op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 tnat_z_orcm_p_p_r(uint32 *unit, qword *out) {
   return tnat_nz_or_p_p_r(unit, out);
}
/*----------------------------------------------------------------------*/   
uint32 tnat_z_p_p_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)5<<37 | (uint64)(0*8+0*4+0*32)<<33 |
            (1*2+0)<<12 | op.p[0]<<6 | op.p[1]<<27 | op.r[0]<<20 |
             op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 tnat_z_unc_p_p_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)5<<37 | (uint64)(0*8+0*4+0*32)<<33 |
            (1*2+1)<<12 | op.p[0]<<6 | op.p[1]<<27 | op.r[0]<<20 |
            op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 unpack1h_r_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(0*256+0*32+0*16+2*64+0+1*4)<<28 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 unpack1l_r_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(0*256+0*32+0*16+2*64+2+1*4)<<28 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 unpack2h_r_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(0*256+1*32+0*16+2*64+0+1*4)<<28 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 unpack2l_r_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(0*256+1*32+0*16+2*64+2+1*4)<<28 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 unpack4h_r_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(1*256+0*32+0*16+2*64+0+1*4)<<28 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 unpack4l_r_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)7<<37 | (uint64)(1*256+0*32+0*16+2*64+2+1*4)<<28 |
                 op.r[0]<<6 | op.r[1]<<13 | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 xchg1_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 xchg1_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 xchg1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 xchg2_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 xchg2_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 xchg2_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 xchg4_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 xchg4_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 xchg4_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 xchg8_nt1_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 xchg8_nta_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 xchg8_r_r_r(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 xma_h_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 xma_hu_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 xma_l_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 xma_lu_f_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 xmpy_h_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 xmpy_hu_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 xmpy_l_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
uint32 xmpy_lu_f_f_f(uint32 *unit, qword *out) {return UNIMPLEMENTED;}
/*----------------------------------------------------------------------*/   
uint32 xor_r_i8_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (uint64)(0xb*4+3)<<27 | op.r[0]<<6 |
              (op.i[0]&127)<<13 | ((op.i[0]>>7)&1)<<36 |
              op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 xor_r_r_r(uint32 *unit, qword *out) {
   *unit = IMUNIT;
   out->q = (uint64)8<<37 | (uint64)(3*4+3)<<27 | op.r[0]<<6 | op.r[1]<<13
                    | op.r[2]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 zxt1_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)0<<37 | (uint64)(0*64+0x10)<<27 | op.r[0]<<6
                    | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 zxt2_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)0<<37 | (uint64)(0*64+0x11)<<27 | op.r[0]<<6
                    | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
uint32 zxt4_r_r(uint32 *unit, qword *out) {
   *unit = IUNIT;
   out->q = (uint64)0<<37 | (uint64)(0*64+0x12)<<27 | op.r[0]<<6
                    | op.r[1]<<20 | op.pred;
   return IA_OK;
}
/*----------------------------------------------------------------------*/   
