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

#ifndef _IA64ASM_H_
#define _IA64ASM_H_

#include "ia64.h"

typedef struct {
   uint32 pred;
   uint32 p[2];
   uint32 b[2];
   uint32 r[3];
   uint64 i[4];
   uint32 f[4];
   uint32 ar[2];
} parse_regs;

typedef struct {
   char format[40];
   uint32 (*asm_func)(uint32 *unit, qword *out);
} lookup;

void decode_instruction( bundle *vliw, uint32 mask, char *dest );
uint32 assemble_instruction(char *input, uint32 *unit, qword *out);
uint32 assemble_data(char *input, uint32 *unit, qword *out);
uint32 assemble_label(char *input, uint32 *unit, qword *out);
uint32 data_label(char *input); 
uint32 bundle_from_file( bundle *vliw, FILE *infile );
uint32 data_from_file( uint64 base, FILE *infile );
uint64 get_symbol_value(char *symbol);
#define NUM_LOOKUPS 2125

extern lookup jumptable[];
extern parse_regs op;

#define UMASK   0x0001ff00
#define IUNIT   0x00000100
#define MUNIT   0x00000200
#define FUNIT   0x00000400
#define BUNIT   0x00000800
#define XUNIT1  0x00001000
#define XUNIT2  0x00002000
#define IMUNIT  0x00004000
#define FIRST   0x00008000
#define STOP    0x00010000
#define TMPL    0x00020000
#define TRGT    0x00040000
#define IPREL   0x00080000

#define DRAIN   0x100
#define EMPTY   0x200
#define ERROR   0x400


#define FEW 0
#define MANY 1
#define SPTK 0
#define SPNT 1
#define DPTK 2
#define DPNT 3
#define CLR 1

#endif /* !_IA64ASM_H_ */
