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

/* ia64.h:
   header file for IA64 ISA and runtime
   environment.
*/

#ifndef _IA64_H_
#define _IA64_H_

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <elf.h>

#define TRUE 1
#define FALSE 0

        /* extract up to 31 bits from the qword x,
           from positions lo to hi inclusive.
           0<=lo,hi<=63 and lo<=hi */

#define BITS(x,lo,hi) (      \
           (uint32)(x.q >> lo) &   \
	   ( (1<<(hi-lo+1)) - 1 )    \
	   )

#define SWAPI(x,y) {                         \
                uint32 temp;                 \
                temp=(x); (x)=(y); (y)=temp; \
             }

#define MEMSIZE (1<<20)

/*--------------------------- TYPES -------------------------------*/
#if defined(WIN32)
typedef unsigned __int64 uint64;
typedef __int64 int64;
int64 strtoll( char *text, char **after, int radix );
#else
typedef unsigned long long uint64;
typedef long long int64;
#endif
typedef unsigned long uint32;
typedef long int32;
typedef unsigned short uint16;
typedef short int16;
typedef unsigned char uint8;
typedef char int8;

typedef union {
   uint64 q;
   uint32 d[2];
   uint16 w[4];
   uint8 b[8];
} qword;

#define HI(x)  (x.d[1])
#define LO(x)  (x.d[0])

typedef struct {
   qword dat;
   uint32 nat;
} alu_reg;

typedef struct {
   qword mant;
   uint32 exp;
   uint32 sgn;
} fpu_reg;

typedef struct {
   qword lo_q;
   qword hi_q;
} bundle;

struct symbol_table { /* symbol table node definition */
   int32 symbol_value;
   uint32 symbol_type;
   uint32 symbol_seg;
   uint32 symbol_size;
   char *symbol_name;
   struct symbol_table *left;
   struct symbol_table *right;
   };

struct _stack_frame {
   struct _stack_frame *last;
   alu_reg frame[96];
};

typedef struct _stack_frame stack_frame;

/*--------------------------- GLOBALS --------------------------------*/

#define ALU_REG 0x0100                /* register types in the */
#define FPU_REG 0x0200                     /* reg_written list */
#define APP_REG 0x0400
#define PRD_REG 0x0800
#define BR_REG  0x1000
#define PC_REG  0x2000

extern alu_reg r[128];                    /* integer registers */
extern fpu_reg f[128];             /* floating point registers */
extern qword ar[128];                 /* application registers */
extern qword b[8];                         /* branch registers */
extern uint32 p[64];                    /* predicate registers */
extern qword ip;                            /* instruction ptr */
extern qword cfm;                      /* current frame marker */
extern uint32 iim;                     /* interruption immediate register */
extern qword slot[3];                 /* instruction slots */
extern stack_frame *last_frame;

struct symbol_table *get_symbol(struct symbol_table *p, char *symbol);
void print_symbols(struct symbol_table *p);
void free_symbols(struct symbol_table *p);
uint32 load_elf(char *file_image,uint64 base);
extern struct symbol_table *symbol_first;
extern uint32 faults;                /* unhandled fault conditions (bitmapped); */
#define BREAK_FAULT 1

#define MAXSEG 2          /* maximum entries in segment table */

struct seg_entry {
  char *seg_name;
  uint64 seg_addr;
  uint64 seg_length;
  uint64 seg_anchor;
  uint32 seg_type;
  };
  
extern struct seg_entry seg_tbl[];
extern uint32 cur_seg;
extern uint32 text_type;
extern uint32 inok;
  
extern uint32 trace_level;            /* controls how much to trace */

extern uint32 num_reg_written;  /* number of registers written so far */
extern uint32 reg_written[128];   /* list of registers written so far */
extern uint8 *mem;                          /* pointer to IA64 memory */


#define AR_PFS 64                       /* previous function state */
#define AR_LC 65                                   /* loop counter */
#define AR_EC 66                                 /* epilog counter */

#define GP 1                      /* global data pointer */
#define SP 12                     /* stack pointer */
#define TP 13                     /* thread pointer */

extern char error_strings[12][20];

enum templates{ mii, miis, misi, misis, mx, mxs,
                mmi=8, mmis, msmi, msmis, mfi, mfis,
                mmf, mmfs, mib, mibs, mbb, mbbs,
                bbb=22, bbbs, mmb, mmbs, mfb=28, mfbs };

/*--------------------------- FUNCTIONS-------------------------------*/

uint32 execute_instruction(bundle *vliw, uint32 mask);

/*-------------------------- ERROR STUFF -----------------------------*/

enum error_codes{
   IA_OK=0,
   INVALID_BUNDLE,
   ILLEGAL_INSTR,
   REGISTER_HAZARD,
   NAT_CONSUMPTION,
   PARSE_ERROR,
   MEMORY_EXCEPTION,
   STACK_FAULT,
   UNIMPLEMENTED,
   BREAK_INSTR,
   PGM_TERM,
   SYS_TERM
};


#endif /* !_IA64_H_ */
