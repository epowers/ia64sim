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

/* global variables */

#include "ia64.h"

alu_reg r[128];             /* integer registers */
fpu_reg f[128];             /* floating point registers */
qword ar[128];              /* application registers */
qword b[8];                 /* branch registers */
uint32 p[64];               /* predicate registers */
qword ip;                   /* instruction ptr */
qword cfm;                  /* current frame marker */
uint32 iim;                 /* immediate interrupt control register */
uint32 faults;				/* unhandled fault conditions (bitmapped) */
uint32 trace_level;         /* controls how much to trace */
qword slot[3];

uint8 *mem;

uint32 num_reg_written;
uint32 reg_written[128];
stack_frame *last_frame;

char error_strings[12][20] = {
   "ok",
   "invalid bundle",
   "illegal instr",
   "register hazard",
   "NAT consumption",
   "parse error",
   "memory exception",
   "stack fault",
   "unimplemented",
   "break instruction",
   "program termination",
   "system termination",
};

struct seg_entry seg_tbl[MAXSEG];
uint32 cur_seg;
uint32 text_type;
uint32 inok;
struct symbol_table *symbol_first = NULL;
