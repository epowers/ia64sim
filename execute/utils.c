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
/* Updated by Chuck Crayne (ccrayne@pacific.net) */

#include "ia64.h"
#include "execute.h"

/*-------------------------------------------------------------------*/

void trcins(char *format, ...) {
  if (trace_level) {
   va_list arg;
   
   va_start(arg, format);
   vfprintf(stdout, format, arg);
   va_end(arg);   }
}


/*-------------------------------------------------------------------*/
#if 0
void dprint(char *format, ...) {

   va_list arg;

   va_start(arg, format);
   vfprintf(stdout, format, arg);
   va_end(arg);
}
#endif

/*-------------------------------------------------------------------*/
uint32 detect_hazard( uint32 reg ) {

   uint32 i;
   char reg_type[10][4] = {"","r","f","ar","p","b","ip"};

   if(reg & ALU_REG) {
      if( (reg & 0x7f) >= (32+(cfm.q&0x7f)) )
         return STACK_FAULT;
   }

   for(i=3; i<num_reg_written; i++) {  /* i=0 creates invalid results */
      if(reg==reg_written[i]) {
         printf("register hazard: %s%d\n",
                   reg_type[reg>>8], reg & 255);
         return REGISTER_HAZARD;
      }
   }

   return 0;
}

/*-------------------------------------------------------------------*/
void add_hazard( uint32 reg ) {

   reg_written[ num_reg_written++ ] = reg;
}

/*-------------------------------------------------------------------*/
void push_stack_frame(void){

   /* WARNING: make sure cfm has been saved to pfs by now! */

   stack_frame *temp_frame;
   uint32 sof, sol, i;

   sof = BITS(cfm,0,6);
   sol = BITS(cfm,7,13);

   temp_frame = (stack_frame *)malloc(sizeof(stack_frame));
   if(!temp_frame) {
      printf("simulator error: memory allocation failed\n");
      return;
   }

   if(!last_frame) 
      temp_frame->last = NULL;
   else
      temp_frame->last = last_frame;
   last_frame = temp_frame;

   for(i=0; i<sol; i++) {
      last_frame->frame[i].dat.q = r[32+i].dat.q;
      last_frame->frame[i].nat = r[32+i].nat;
   }
   for(i=0; i<(sof-sol); i++) {
      r[32+i].dat.q = r[32+sol+i].dat.q;
      r[32+i].nat = r[32+sol+i].nat;
   }
   for(; i<sof; i++) {
      r[32+i].dat.q = 0; r[32+i].nat = 0;
   }

   cfm.q = sof-sol;
}

/*-------------------------------------------------------------------*/
uint32 pop_stack_frame(void){

   /* WARNING: make sure cfm has been restored from pfs by now! */

   stack_frame *temp_frame;
   uint32 sof, sol, i;

   sof = BITS(cfm,0,6);
   sol = BITS(cfm,7,13);

   for(i=0; i<(sof-sol); i++) {
      r[32+sol+i].dat.q = r[32+i].dat.q;
      r[32+sol+i].nat = r[32+i].nat;
   }

   if(!last_frame) {
      printf("error: no previous frame to restore\n");
      return STACK_FAULT;
   }
   else {
   for(i=0; i<sol; i++) {
      r[32+i].dat.q = last_frame->frame[i].dat.q;
      r[32+i].nat = last_frame->frame[i].nat;
   }
      temp_frame = last_frame->last;
      free(last_frame);
      last_frame = temp_frame;
      return IA_OK;
   }
}

/*-------------------------------------------------------------------*/
uint32 load_mem(uint8 *reg, uint64 *addr, uint32 size) {

   if( addr[0]>MEMSIZE || (addr[0] & (size-1)) )
      return MEMORY_EXCEPTION;
   else {
      memset( reg, 0, 8 );
      memcpy( reg, mem+addr[0], size);
   }

   return IA_OK;
}
/*-------------------------------------------------------------------*/
uint32 store_mem(uint8 *reg, uint64 *addr, uint32 size) {

   if( addr[0]>MEMSIZE || (addr[0] & (size-1)) )
      return MEMORY_EXCEPTION;
   else {
      memcpy( mem+addr[0], reg, size);
   }

   return IA_OK;
}
/*-------------------------------------------------------------------*/
void rotate_registers(void){
   qword temp_cfm;
   alu_reg temp_r;
   fpu_reg temp_f;
   uint32 temp_p, sof, sol, sor, rbbgr, rbbfr, rbbpr, i;
   
   sof = BITS(cfm,0,6);
   sol = BITS(cfm,7,13);
   sor = BITS(cfm,14,17)<<3;
   rbbgr = BITS(cfm,18,24);
   rbbfr = BITS(cfm,25,31);
   rbbfr = BITS(cfm,32,37);

   if(sor) {
     temp_r = r[sor+31];
     for(i=sor+31;i>31;i--) r[i] = r[i-1];
     r[32] = temp_r;
   }
   if(rbbgr) rbbgr--;
   else rbbgr = sor-1;
   
   temp_f = f[127];
   for(i=127;i>32;i--) f[i] = f[i-1];
   f[32] = temp_f;
   if(rbbfr) rbbfr--;
   else rbbfr = 127;
   
   temp_p = p[63];
   for(i=63;i>16;i--) p[i] = p[i-1];
   p[16] = temp_p;
   if(rbbpr) rbbpr--;
   else rbbpr = 63;
   
   temp_cfm.q = cfm.q & 0x3ffff;
   cfm.q = temp_cfm.q | (uint64)rbbgr << 18 | (uint64)rbbfr << 25 | (uint64)rbbpr << 32;
    
}


