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
#ifndef _EXECUTE_H_
#define _EXECUTE_H_

#include "ia64.h"

#define check_reg(x) {                           \
                uint32 status;                   \
                if( (status=detect_hazard(x)) )  \
                   return status;                \
                }

uint32 i_unit(qword *instr);
uint32 f_unit(qword *instr);
uint32 b_unit(qword *instr);
uint32 m_unit(qword *instr);
uint32 x_unit(qword *instr);
void stop(void);
uint32 faultcheck(void);

void dprint(char *format, ...);
uint32 detect_hazard( uint32 reg );
void add_hazard( uint32 reg );
void rotate_registers(void);
void push_stack_frame(void);
uint32 pop_stack_frame(void);
void trcins(char *fmt, ...);
uint32 load_mem(uint8 *reg, uint64 *addr, uint32 size);
uint32 store_mem(uint8 *reg, uint64 *addr, uint32 size);

uint32 i_misc(qword *instr);
uint32 i_deposit(qword *instr);
uint32 i_shift(qword *instr);
uint32 i_test(qword *instr);
uint32 i_mmshift(qword *instr);
uint32 i_alu(qword *instr);
uint32 i_addi22(qword *instr);
uint32 i_compare(qword *instr);
uint32 m_mgmt(qword *instr);
uint32 m_ldir(qword *instr);
uint32 m_ldii(qword *instr);
uint32 m_ldfr(qword *instr);
uint32 m_ldfi(qword *instr);
uint32 f_misc(qword *instr);
uint32 f_compare(qword *instr);
uint32 f_class(qword *instr);
uint32 f_fma(qword *instr);
uint32 f_fms(qword *instr);
uint32 f_fnma(qword *instr);
uint32 f_xma(qword *instr);
uint32 b_branchr(qword *instr);
uint32 b_callr(qword *instr);
uint32 b_nop(qword *instr);
uint32 b_break(qword *instr);
uint32 b_branchi(qword *instr);
uint32 b_calli(qword *instr);
uint32 x_misc(qword *instr);
uint32 x_movl(qword *instr);

#endif		/* !_EXECUTE_H_ */
