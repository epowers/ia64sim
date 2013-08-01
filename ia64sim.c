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

/* The simulator code */

#include "ia64.h"
#include "ia64asm.h"

#define MIN(a,b) ((a)<(b)? (a):(b))

char *nextchar, tokens[5] = " =\n";
char cmd[100];
uint64 breaks[8];
uint32 currslot;
uint64 currmem;

#define SLOT0 0x1
#define SLOT1 0x2
#define SLOT2 0x4

/*-----------------------------------------------------------------*/
void help(void) {
   printf("\n");
   printf("List of commands  ( '{}' is optional )\n\n");
   printf("target = value    : assign 'value' (number or register) to\n");
   printf("                       'target' (memory or register). Spec-\n");
   printf("                       ifying ip as target resets the current\n");
   printf("                       instruction slot to slot 0\n");
   printf("B addr            : set a breakpoint at (bundle holding) 'addr'\n");
   printf("C file {base}     : assemble 'file' and load at 'base'\n");
   printf("G                 : execute until breakpoint is hit\n");
   printf("I                 : execute next instruction (not bundle)\n");
   printf("L file {base}     : loads binary file 'file' starting at 'base'\n");
   printf("P                 : issue 'stop' to clear hazards\n");
   printf("Q                 : quit\n");
   printf("R {start} {#regs} : display like register types in current frame\n");
   printf("                       from index 'start' for #regs\n");
   printf("S file addr size  : stores 'size' bytes to 'file' from 'addr'\n");
   printf("T level           : set trace level\n");
   printf("X{I,B,S} {base}     : examine memory {at address 'base'}\n");
   printf("                       (constants can be decimal or '0x'-hex)\n");
   printf("                       Default is page after last use of 'X'\n");
}

/*-----------------------------------------------------------------*/
void pred(void) {

   int32 i;

   printf("predicate registers:\n");
printf("   6         5         4         3         2         1         0\n");
printf("3210987654321098765432109876543210987654321098765432109876543210\n");
printf("----------------------------------------------------------------\n");

   for(i=63; i>=0; i--) {
      if( (p[i]&1) )
         printf("1");
      else
         printf("0");
   }

   printf("\n");
}

/*-----------------------------------------------------------------*/
void assign(void) {

   char *first, *second, *junk;
   uint32 size1, size2, offset;
   uint64 i1, i2;

   first = strtok(cmd, tokens);
   second = strtok(NULL, tokens);
   if(!first || !second) {
      printf("1syntax error\n");
      return;
   }
   
   if(!isdigit(*first)) {           /*-------- PARSE FIRST ARGUMENT ------*/

      if(first[0]=='i' && first[1]=='p') {
         first = (char *)(&ip);
         size1 = sizeof(uint64);
         currslot = SLOT0;
         stop(); /* Clear pending register hazzards */
      }
      else {
         i1 = strtoll(first+1, &junk, 0);
         if(*junk) {
            printf("3syntax error\n");
            return;
         }

         switch(*first) {
            case 'r':
               if( i1 > ( (cfm.q&127) + 32) || i1<0) {
                  printf("register outside frame\n");
                  return;
               }
               first = (char *)(r + i1);
               size1 = sizeof(alu_reg);
               break;
   
            case 'b':
               if( i1 > 7 || i1<0) {
                  printf("4syntax error\n");
                  return;
               }
               first = (char *)(b + i1);
               size1 = sizeof(uint64);
               break;
   
            case 'p':
               if( i1 > 63 || i1<0) {
                  printf("5syntax error\n");
                  return;
               }
               first = (char *)(p + i1);
               size1 = 1;
               break;
   
            default:
               printf("not implemented\n"); return;
         }
      }
   }
   else {
      i1 = strtoll(first, &junk, 0);
      if(*junk) {
         printf("6syntax error\n");
         return;
      }
      if(i1 > MEMSIZE) {
         printf("address out of range\n");
         return;
      }
      first = mem + (i1 & ~7);
      size1 = sizeof(uint64);
   }
                                   /*-------- PARSE SECOND ARGUMENT ------*/

   if(!isdigit(*second) && second[0]!='-') { 

      if(second[0]=='i' && second[1]=='p') {
         second = (char *)(&ip);
         size2 = sizeof(uint64);
      }
      else {
         i2 = strtoll(second+1, &junk, 0);
         if(*junk) {
            printf("7syntax error\n");
            return;
         }

         switch(*second) {
            case 'r':
               if( i2 > ( (cfm.q&127) + 32) || i2<0) {
                  printf("register outside frame\n");
                  return;
               }
               second = (char *)(r + i2);
               size2 = sizeof(uint64);
               break;
   
            case 'b':
               if( i2 > 7 || i2<0) {
                  printf("8syntax error\n");
                  return;
               }
               second = (char *)(b + i2);
               size2 = sizeof(uint64);
               break;
   
            case 'p':
               if( i2 > 63 || i2<0) {
                  printf("9syntax error\n");
                  return;
               }
               second = (char *)(p + i2);
               size2 = 1;
               break;
   
            default:
               printf("not implemented\n");
               return;
         }
      }
   }
   else {
      i2 = strtoll(second, &junk, 0);
      if(*junk) {
         printf("Asyntax error\n");
         return;
      }
      second = (char *)(&i2); size2 = sizeof(uint64);
   }

   memcpy(first, second, MIN(size1, size2));      /* do the assignment */
}

/*-----------------------------------------------------------------*/
void showhere(void) {

         bundle vliw;

         memcpy((char *)(&vliw),mem+ip.q,sizeof(bundle));
         decode_instruction(&vliw,currslot,cmd);
         printf("\n\nip:    0x%08x%08x", HI(ip), LO(ip));
         printf("%s\n",cmd);
         printf("gp:    0x%08x%08x      ", HI(r[GP].dat), LO(r[GP].dat));
         printf("sp:    0x%08x%08x\n", HI(r[SP].dat), LO(r[SP].dat));
         printf("cfm:   0x%08x%08x      ", HI(cfm), LO(cfm));
         printf("pfs:   0x%08x%08x\n", HI(ar[AR_PFS]), LO(ar[AR_PFS]));
         printf("ar.lc: 0x%08x%08x      ", HI(ar[AR_LC]), LO(ar[AR_LC]));
         printf("ar.ec: 0x%08x%08x\n", HI(ar[AR_EC]), LO(ar[AR_EC]));
         return;
         }
         
/*-----------------------------------------------------------------*/
void showreg(void) {

   char *first, *second, *junk;
   uint32 i1, i2, i;

   first = strtok(NULL, tokens);
   second = strtok(NULL, tokens);
   
   if(!first) {
     showhere();
     return;
     }

   if (*first+1 == '\0') i1=0;
   else {   
     i1 = strtoul(first+1, &junk, 10);
     if(*junk) {
      printf("Invalid register number\n");
      return;
      }
   }
   
   if(!second) {
    i2 = 0;
    }
   else {
    i2 = i1 + strtoul(second, &junk, 10);
    if(*junk) {
      printf("Invalid # of registers\n");
      return;
      }
   }

   switch( *first ) {

      case 'r':
         if (!i2) i2 = i1 + 15;
         if(i2 >= ((cfm.q&127)+32) ) i2 = (cfm.q&127) + 31;
         if(i1 >= i2) {
           printf("Register range not valid\n");
           return;
           }
         printf("\n");
         for(i=0; i<(i2-i1+1); i++) {
            printf("r%d: 0x%08x%08x NAT: %d    ", i1+i,
                      HI(r[i1+i].dat), LO(r[i1+i].dat), r[i1+i].nat);
            if(i&1) printf("\n");
         }

         return;

      case 'i':
         i1 += 32;
         if (!i2) i2 = i1 + 15;
         else i2 += i1;
         if(i2 >= ((cfm.q&127)+32) ) i2 = (cfm.q&127) + 31;
         if(i1 >= i2) {
           printf("Register range not valid\n");
           return;
           }
         printf("\n");
         for(i=0; i<(i2-i1+1); i++) {
            printf("r%d: 0x%08x%08x NAT: %d    ", i1+i,
                      HI(r[i1+i].dat), LO(r[i1+i].dat), r[i1+i].nat);
            if(i&1) printf("\n");
         }

         return;
         
      case 'o':
         i1 += (32 + (cfm.q >> 7)&127);
         if (!i2) i2 = i1 + 15;
         else i2 += i1;
         if(i2 >= ((cfm.q&127)+32) ) i2 = (cfm.q&127) + 31;
         if(i1 >= i2) {
           printf("Register range not valid\n");
           return;
           }
         printf("\n");
         for(i=0; i<(i2-i1+1); i++) {
            printf("r%d: 0x%08x%08x NAT: %d    ", i1+i,
                      HI(r[i1+i].dat), LO(r[i1+i].dat), r[i1+i].nat);
            if(i&1) printf("\n");
         }

         return;
         
      case 'b':
         if (!i2) i2 = i1 + 7;
         if(i2 > 7  ) i2 = 7;
         printf("\n");
         for(i=0; i<(i2-i1+1); i++) {
            printf("b%d: 0x%08x%08x    ", i1+i, HI(b[i1+i]),
                      LO(b[i1+i]) );
            if((i%3+1)==3) printf("\n");
         }
         printf("\n");
         return;
         
      case 'p':
         pred();
         return;

      default:
         printf("register type not implemented\n");
   }
}

/*-----------------------------------------------------------------*/
void showmem(void) {

   bundle vliw;
   char prtbuf[8],*junk;
   uint8 *base;
   uint64 temp;
   uint32 i, j, mask, fmt;

   base = strtok(NULL, tokens);
   if(base) {
      temp = strtoll(base, &junk, 0);
      if(*junk) {
         printf("2syntax error\n");
         return;
      }
      currmem = temp & ~15;
   }

  nextchar++; /* point past initial cmd letter */
  switch (*nextchar) {
  
    case '\0':
    case ' ':
    case 'i':
     for(i=0; i<64 && currmem<MEMSIZE; i+=16) {

      memcpy((char *)(&vliw), mem+currmem, sizeof(bundle));

      base = mem+currmem;
      fmt = base[0] & 0x1f;

      for(j=0,mask=1; j<4; j++) {
         if (!j) printf("\n   %08x: 0x%02x",(uint32)currmem, fmt);
         else {               
           decode_instruction( &vliw, mask, cmd);
           printf("             0x%03x%08x ", HI(slot[j-1]),LO(slot[j-1]));

            if(ip.q==currmem && currslot==mask)
               printf("-> ");
            else
               printf("   ");
            printf("%s", cmd);
            mask <<= 1;
         }

         base += 4;
         printf("\n");
      }

      currmem += 16;
   }
   break;
   
   case 'b':
     for(i=0; i<16 && currmem<MEMSIZE; i++) {

        base = mem+currmem;
        memcpy((char *)(&prtbuf), mem+currmem, sizeof(prtbuf));
        for(j=0;j<sizeof(prtbuf);j++) if(prtbuf[j]<' ') prtbuf[j]='.';
         
        printf("\n   %08x: %02x %02x %02x %02x %02x %02x %02x %02x",
                (uint32)currmem, base[0], base[1], base[2], base[3],
                                base[4], base[5], base[6], base[7]);
        printf("    %8s",prtbuf);
        
        currmem += 8;
        }
    printf("\n");
    break;
    
    case 's':
      if (symbol_first != NULL) {
        printf("symbol name             value    size     type     section\n");
        print_symbols(symbol_first);
        }
      else
        printf("empty symbol table\n");
      break;
   
   default:
     printf("Invalid format type");
   }        
}

/*-----------------------------------------------------------------*/
void setup(uint64 entry_point,uint64 short_data) {
   ip.q = entry_point;
   currslot = SLOT0;
   r[1].dat.q = short_data; /* gp */
   r[12].dat.q = MEMSIZE-16; /* sp */
   r[13].dat.q = (uint64) 0; /* tp */
   b[0].q = -1; /* return link */
   ar[40].q = 0x000000000000003f; /* fpsr */
   cfm.q = 4; /* four output regs, no input or local regs */
   push_stack_frame();
   currmem = entry_point;
   breaks[0]=  -1;
   showhere();
}


/*-----------------------------------------------------------------*/
void compile(void)  {

   FILE *infile;
   char *first, *second, *junk;
   uint64 base;
   uint32 i, status;
   int32  entry;
   bundle vliw;

   first = strtok(NULL, tokens);
   second = strtok(NULL, tokens);
   if(!first ) {
      printf("File name must be specified\n");
      return;
   }
   if(!second) base = 0;
   else {
   base = strtoll(second, &junk, 0) & ~15;
   if(*junk) {
      printf("Invalid base address\n");
      return;
      }
   }

   infile = fopen(first, "r");
   if(!infile) {
      printf("couldn't open '%s'\n", first);
      return;
   }

/* clear segment table entries */

   for(i=0;i<MAXSEG;i++) {
     seg_tbl[i].seg_name = 0;
     seg_tbl[i].seg_addr = 0;
     seg_tbl[i].seg_length = 0;
     seg_tbl[i].seg_anchor = 0;
     seg_tbl[i].seg_type = 0;
     }
     
     
   
   if(symbol_first != NULL) {         /* clear symbol table */
   free_symbols(symbol_first);
   free(symbol_first);
   symbol_first = NULL;
   }
   
   cur_seg = 1;                  /* sdata segment is always segment 1 */
  
   seg_tbl[cur_seg].seg_addr = base;   /* set segment load address */
   seg_tbl[cur_seg].seg_anchor = base; /* assume gp -> beginning of data */
   
   
   status = data_from_file( base, infile );
   
   
   base = (seg_tbl[1].seg_length+15)&~15;  /* align code after data */
   cur_seg = 0;                  /* text segment is always segment 0 */
   seg_tbl[cur_seg].seg_addr = base;   /* set segment load address */
   seg_tbl[cur_seg].seg_anchor = base; /* assume entry point at beginning of code */
  
   while(base<MEMSIZE) {
      status = bundle_from_file( &vliw, infile );
      if(status!=IA_OK && status!=DRAIN) break;
      memcpy(mem+base, (char *)(&vliw), sizeof(bundle));
      base += 16;
   }
   if (status = EMPTY) status = IA_OK;
   fclose(infile);
   entry = get_symbol_value("main");
     if(entry != -1) seg_tbl[0].seg_anchor = seg_tbl[cur_seg].seg_addr + entry;    /* set entry point */
   
   if(!status) setup(seg_tbl[0].seg_anchor,seg_tbl[1].seg_anchor);
}

/*-----------------------------------------------------------------*/
void load(void)  {

   FILE *infile;
   char *first, *second, *junk, *file_image, *string_ptr, *symbol_ptr, *loctr;
   uint64 base, function_ptr,entry_point, short_data, file_length;
   uint32 status, i,j,namex,type,offset,size,addr;

   first = strtok(NULL, tokens);
   second = strtok(NULL, tokens);
   if(!first ) {
      printf("File name must be specified\n");
      return;
   }
   if(!second) base = 0;
   else {
   base = strtoll(second, &junk, 0) & ~15;
   if(*junk) {
      printf("Invalid base address\n");
      return;
      }
   }

   infile = fopen(first, "rb");
   if(!infile) {
      printf("couldn't open '%s'\n", first);
      return;
   }

/* load entire file image into temporary memory */   
   fseek(infile,0,SEEK_END);
   file_length = ftell(infile);
   file_image = malloc(file_length);
   fseek(infile,0,SEEK_SET);
   fread(file_image,file_length,1,infile);
   fclose(infile);

   if(symbol_first != NULL) {         /* clear symbol table */
   free_symbols(symbol_first);
   free(symbol_first);
   symbol_first = NULL;
   }
   
   
   #if defined(WIN32)
   printf()"\nCOFF support not yet implemented.\nWould you like to volunteer?\n");
   free(file_image);
   return;
   #else
  
   status = load_elf(file_image,base);
   #endif
   
   
}
/*-----------------------------------------------------------------*/
void save(void)  {

   FILE *outfile;
   char *first, *second, *third, *junk;
   uint64 base, module_size, load_image, function_ptr ,entry_point, short_data;
   uint32 status;
   bundle vliw;

   first = strtok(NULL, tokens);
   second = strtok(NULL, tokens);
   third = strtok(NULL, tokens);
   if(!first || !second || !third) {
      printf("File name, address, and size must all be specified\n");
      return;
   }

   base = strtoll(second, &junk, 0) & ~15;
   if(*junk) {
      printf("Invalid base address\n");
      return;
   }

   module_size = strtoll(third, &junk, 0) & ~15;
   if(*junk) {
      printf("Invalid length\n");
      return;
   }

   outfile = fopen(first, "wb");
   if(!outfile) {
      printf("couldn't open '%s'\n", first);
      return;
   }
   load_image = 4 * sizeof (uint64);
   function_ptr = 2 * sizeof (uint64);
   entry_point = ip.q;
   short_data = r[1].dat.q;
   fwrite(&load_image,sizeof(load_image),1,outfile);
   fwrite(&function_ptr,sizeof(function_ptr),1,outfile);
   fwrite(&entry_point,sizeof(entry_point),1,outfile);
   fwrite(&short_data,sizeof(short_data),1,outfile);
   fwrite(mem+base,module_size,1,outfile);

   fclose(outfile);
}

/*-----------------------------------------------------------------*/
uint32 faultcheck(void) {
   uint32 sw, status = IA_OK;
   while (faults) {
     if (faults & BREAK_FAULT) {
       faults &= ~BREAK_FAULT;
       switch(iim >> 17) {
         case 0:
           switch(iim & 0x1ffff) {
             case 0:printf("Unknown Program Error\n");
             break; 
             case 1:printf("Integer Divide by Zero\n");
             break; 
             case 2:printf("Integer Overflow\n");
             break; 
             case 3:printf("Range Check Error\n");
             break; 
             case 4:printf("Nil Pointer Dereference\n");
             break; 
             case 5:printf("Misaligned Data\n");
             break; 
             case 6:printf("Decimal Overflow\n");
             break; 
             case 7:printf("Decimal Devide by Zero\n");
             break; 
             case 8:printf("Packed Decimal Error\n");
             break; 
             case 9:printf("Invalid ASCII Digit\n");
             break; 
             case 10:printf("Invalid Decimal Digit\n");
             break; 
             case 11:printf("Paragraph Stack Overflow\n");
             break;
             default: printf("Reserved Software Interrupt\n");
             }
           status = BREAK_INSTR;
           break;
         case 1: 
           printf("Application Interrupt\n");
           status = BREAK_INSTR;
           break;
         case 2:
         case 3: 
           printf("Debug Breakpoint\n");
           status = BREAK_INSTR;
           break;
         case 4:
           switch(iim & 0x1ffff) {
             case 0:
               printf("\nProgram ended with return code: %u \n",r[32].dat.q);
               status = PGM_TERM;
               break; 
             case 1:
               trcins("Printf Call\n");
               vprintf(mem+r[32].dat.q,mem+r[33].dat.q);
               status = IA_OK;
               break;
             case 2:
               trcins("Getline Call\n");
               fgets(mem+r[32].dat.q,(uint32) mem+r[33].dat.q,stdin);
               break; 
             default: printf("Reserved System Call\n");
               status = UNIMPLEMENTED;
             }
           break;
         case 5 ... 7:  
           printf("Undefined System Call\n");
           status = UNIMPLEMENTED;
           break;
       }
     }
   }
   return status;
} 

/*-----------------------------------------------------------------*/
uint32 step_once(void) {

   bundle vliw;
   uint32 status;
   uint64 oldip;

   memcpy( &vliw, mem+ip.q, sizeof(bundle) );

   oldip = ip.q;
   if( (status=execute_instruction( &vliw, currslot )) ) {
      printf("error: %s\n", error_strings[status]);
      return status;
   }
   else {
      if( currslot==SLOT2 ) {
         currslot = SLOT0;
         if(oldip==ip.q)
            ip.q += 16;
         if(ip.q == breaks[0])
            return BREAK_INSTR;
      }
      else {
         currslot <<= 1;
      }
   }
   return  faultcheck();
}

/*-----------------------------------------------------------------*/
void step(void) {
   uint32 status;
   status = step_once();
   showhere(); 
}

/*-----------------------------------------------------------------*/
void run(void) {
   uint32 status;
   while (!(status = step_once()));
   if (!(status == PGM_TERM)) showhere();
}

/*-----------------------------------------------------------------*/
void setbp(void) {
   char *first, *junk;
   uint64 break_address;
   
   first = strtok(NULL, tokens);
   if(!first) {
     printf("Break point address is required\n"); 
     return;
   }
   break_address = strtoll(first,&junk,0) & ~15;
   if(*junk) {
     printf("Invalid break point address\n");
     return;
   }
   breaks[0] = break_address; 
}
/*-----------------------------------------------------------------*/
void settrace(void) {
   char *first, *junk;
   uint32 level;
   
   first = strtok(NULL, tokens);
   if(!first) {
     printf("Trace level is required\n"); 
     return;
   }
   level = strtoll(first,&junk,0);
   if(*junk) {
     printf("Invalid trace level\n");
     return;
   }
   trace_level = level;
}


/*-----------------------------------------------------------------*/
int main(int argc, char **argv) {

   uint32 i;

   printf("\nIA64 ISA simulator, version 0.4\n");
   printf("Memory limits: 0x%08x-0x%08x\n", 0, MEMSIZE-1);
   printf("Try 'h' for help\n\n");

   for(i=0; i<128; i++) {
      r[i].dat.q = 0; r[i].nat = 0;
   }
   for(i=0; i<128; i++) {
      f[i].mant.q = 0; f[i].exp = 0; f[i].sgn = 0;
   }
   for(i=0; i<64; i++)  p[i] = 0;
   for(i=0; i<128; i++)  ar[i].q = 0;
   for(i=0; i<8; i++)  b[i].q = 0;
   ip.q = currmem = 0;
   cfm.q = 0;
   p[0] = 1;
   currslot=1;
   num_reg_written = 3;
   reg_written[0] = 0 | ALU_REG;
   reg_written[1] = 0 | FPU_REG;
   reg_written[2] = 1 | FPU_REG;

   mem = (uint8 *)calloc( MEMSIZE, sizeof(uint8) );
   if(!mem) {
      printf("memory allocation failed; exiting\n");
      exit(-1);
   }

   while(1) {
      printf("IA64sim>> ");
      fgets(cmd,sizeof(cmd),stdin);

      nextchar = cmd;
      while(*nextchar) *nextchar++ = tolower(*nextchar);

      if(strchr(cmd,'=')) {
         assign();
      }
      else {
         nextchar = strtok(cmd, tokens);
         if(!nextchar) continue;

         switch( *nextchar ) {
   
            case 'a': assign(); break;
            case 'b': setbp(); break;
            case 'c': compile(); break;
            case 'g': run(); break;
            case 'h': help(); break;
            case 'i': step(); break;
            case 'l': load(); break;
            case 'p': stop(); break;
            case 'q': free(mem); return 0;
            case 'r': showreg(); break;
            case 's': save(); break;
            case 't': settrace();break;
            case 'x': showmem(); break;
   
            default:
               printf("  unknown command\n");
         }
      }
      printf("\n");
      r[0].dat.q = 0;
      r[0].nat = 0;
      p[0] = 1;
      ip.q &= ~15;
   }

   return 0;   /* should never get here */
}
