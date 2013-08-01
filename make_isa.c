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
/* Builds the list of instructions to be encoded
   and decoded, with all possible permutations of
   completers. */


#include "ia64.h"

FILE *jump_file, *func_file;

/*-------------------------------------------------------------------*/
char final_instr[100];

void build_mnemonic( char *src, char *dest ) {

   char *switch_end, *dptr = dest;

   while( *src ) {

      if( *src=='{' ) {

         switch_end = src++;
         dptr = dest;
         while( *switch_end!='}' ) switch_end++;
         switch_end++;

         while( src!=switch_end ) {
            while( *src!=',' && *src!='}' )
               *dptr++ = *src++;
            src++;
            build_mnemonic( switch_end, dptr );
            dptr = dest;
         }

         return;

      }
      else if(isprint(*src)) 
         *dest++ = *src++;
      else
         *src++;
   }

   *dest = 0;
   {
      uint32 i, truncated;

      fprintf(jump_file, "{\"%s\",       ", final_instr);
      truncated = 0;
      for(i=0; i<strlen(final_instr); i++) {
         if(isalnum(final_instr[i])) {
            fprintf(jump_file, "%c", final_instr[i]);
            truncated = 0;
         }
         else if(!truncated) {
            fprintf(jump_file, "_");
            truncated = 1;
         }
      }     
      fprintf(jump_file, "},\n");

      fprintf(func_file, "uint32 ");
      truncated = 0;
      for(i=0; i<strlen(final_instr); i++) {
         if(isalnum(final_instr[i])) {
            fprintf(func_file, "%c", final_instr[i]);
            truncated = 0;
         }
         else if(!truncated) {
            fprintf(func_file, "_");
            truncated = 1;
         }
      }     
      fprintf(func_file, "(uint32 *unit, qword *out);\n");
   }
}

      


/*-------------------------------------------------------------------*/
int main(int argc, char **argv) {

   char line[200];
   FILE *f;

   if(argc!=4) {
      printf("usage: %s <opfile> <headfile> <jumpfile>\n", argv[0]);
      exit(-1);
   }

   if( !(f=fopen(argv[1], "r")) ) {
      printf("error opening opcode file\n");
      exit(-1);
   }
   if( !(func_file=fopen(argv[2], "w")) ) {
      printf("error opening header file\n");
      exit(-1);
   }
   if( !(jump_file=fopen(argv[3], "w")) ) {
      printf("error opening jumptable file\n");
      exit(-1);
   }

   while(!feof(f)) {
      fgets(line, sizeof(line), f);
      if(strlen(line) && line[0]!='#')
         build_mnemonic(line, final_instr);
   }

   fclose(f); fclose(func_file); fclose(jump_file);
   return 0;
}
