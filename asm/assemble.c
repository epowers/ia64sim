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
/* Updated 1/15/00 by Chuck Crayne <ccrayne@pacific.net> */

#include "ia64.h"
#include "ia64asm.h"

parse_regs op;

/*-------------------------------------------------------------------*/
int32 compare_mnemonic( void *key, void *elem ) {

   /* comparison function for the first word in an
      instruction line. This is meant for use by bsearch()
      to narrow down the number of possible instructions
      that *target can be. verify_format() does the
      much harder work of picking the one instruction that
      matches */

   char *target = (char *)key;
   lookup *this_lookup = (lookup *)elem;
   char *lookup_string = this_lookup->format;

   while( *target && isspace(*target) ) target++;    /* kill whitespace */

   while( *target && *lookup_string          /* compare input string to  */
          && !isspace(*lookup_string) ) {    /* version stored in lookup */
                                             /* table, byte by byte      */
      if( *target == '.' )
         *target++;                        /* only compare alphanumerics */
      if( *lookup_string == '.' )
         *lookup_string++;

      if( *target != *lookup_string ) {
         return *target - *lookup_string;
      }
      target++; lookup_string++;
   } 

   /* We have a match if both point to spaces or
      nulls by this point. */

   if( (!(*lookup_string) || *lookup_string==' ') && 
       (!(*target) || *target==' ') )
      return 0;
   else
      return *target - *lookup_string;
}

/*-------------------------------------------------------------------*/
int32 verify_format( char *target, char *lookup_string ) {

   /* compares an input line of IA64 assembly lan-
      guage with the stored (correct) format; returns
      TRUE if the two match, FALSE otherwise. */

   while( *target && *lookup_string ) {        /* while !(end of strings) */

                                               /* kill off whitespace */
      while( *target && isspace(*target) )
         target++;
      while( *lookup_string && isspace(*lookup_string) )
         lookup_string++;
      if( !(*target) || !(*lookup_string) )
         break;
                                               
      if(*lookup_string=='\\') {                 /* if operand reached */
         lookup_string++;

         if( *target == *lookup_string ) {       

            /* check that the operand letter (or
            "ar" in the case of app. registers)
            matches between reference and target, then
            skip to the next non-number (i.e. ignore
            the actual register number). */

            lookup_string++; target++;
            if( target[-1]=='a' && target[0]=='r' ) {
               lookup_string++; target++;
            }   
            while( *lookup_string && isdigit(*lookup_string) ) 
               lookup_string++;
            while( *target && isdigit(*target) ) 
               target++;
         }
         else if( (*lookup_string=='i' || *lookup_string=='u') ) {

            /* otherwise match up immediate data if any,
               and step over *that* */

            lookup_string++;
            while( *lookup_string && isdigit(*lookup_string) ) 
               lookup_string++;

            if( *target=='-' ) target++;

            if( target[0]=='0' && target[1]=='x' ) {
               target += 2;
               while( *target && isxdigit(*target) ) target++;
            }
            else {
               while( *target && (isdigit(*target)) || (isalpha(*target))) 
                  target++;
            }   
         }   
         else {
            return FALSE;          /* neither of the above, just an error */
         }   
      }
      else {
         if( *target != *lookup_string )  /* not an operand, but a char- */
            return FALSE;                 /* acter that must match up    */
         target++; lookup_string++;
      }

   }

   while( *target && isspace(*target) )      /* kill whitespace again */
      target++;
   while( *lookup_string && isspace(*lookup_string) )
      lookup_string++;

   if( !(*target) && !(*lookup_string) )     /* if both strings ran out, */
      return TRUE;                           /* there's a match          */
   else
      return FALSE;
}

/*-------------------------------------------------------------------*/
lookup *lookup_instruction(char *input) {

   lookup *found;
   uint32 score, i;

   /* traverse the jump table looking for an assembly
      opcode that matches that of the input string. Returns
      a pointer to the matching lookup table entry, or NULL.

      Verification of table lookups right now is a two
      stage process. The jumptable is in alphabetic order,
      but verifying the format of the entire assembly line
      at the same time it's being looked up will cause
      bsearch() to give wrong results. Hence the binary
      search looks up only the first word of the instruction,
      and the computed pointer is subjected to a second
      more rigorous comparison with the input line */

   found = (lookup *)bsearch( input, jumptable, NUM_LOOKUPS,
                         sizeof(lookup), (void *)compare_mnemonic );
   if(!found)
      return NULL;
   else if( (verify_format( input, found->format ))==TRUE )
      return found;

   /* By this point, the first choice failed to match the
      form of the input line. All is not lost though: some
      instructions (notably "mov" and those with both
      immediate and register data) have several formats
      even though the opcode is identical. It's possible that
      bsearch() just picked a wrong candidate from several
      possible choices. The following code will walk the
      jump table both behind and in front of the chosen lookup,
      feeling around to see if another instruction located
      nearby will match the input string */
      
   for(i=1; !compare_mnemonic(input, found+i); i++ ) 
      if( (verify_format(input, found[i].format))==TRUE )
         return found+i;
   for(i=-1; !compare_mnemonic(input, found+i); i-- ) 
      if( (verify_format(input, found[i].format))==TRUE )
         return found+i;

   return NULL;
}
                         
/*-------------------------------------------------------------------*/
uint32 parse_operands(char *input, char *template) {

   /* Given a line of IA64 assembly language and its cor-
      responding (known correct) reference format, this code
      converts the operands specified in the instruction into
      integers and fills an internal structure with their
      values. Values are separated by type, and stored in the
      order they occur in the instruction. The code which uses
      the structure to assemble hex knows which values are
      meaningful.

      All immediate data is promoted to 64 bits for consistency,
      and the code performs bounds checking also. This would
      likely make a *really* slow back end to an assembler.

      WARNING: make sure any qualifying predicates are stripped off
      before calling this code. Also, if not using gcc, make sure
      you have a string-to-64bit conversion like strtoll() */

   uint32 rptr,pptr,arptr,fptr,bptr,iptr;
   uint32 index, maxbits;
   char *inptr, *refptr,
        *throwaway, tokens[8] = " ,[]=\t";
   int64 imm;
   char parsebuf[100];

   if( strlen(input)+1 > sizeof(parsebuf) )         /* make local copy of */
      return PARSE_ERROR;                        /* instr, since strtok() */
   strncpy(parsebuf, input, sizeof(parsebuf));   /* fills it with nulls   */
   rptr=pptr=arptr=fptr=bptr=iptr=0;
   inptr = strtok(parsebuf, tokens);        /* point to start of mnemonic */
   refptr = strchr(template,'\\');        /* find 1st operand in template */

   while( inptr && refptr ) {                  /* while !(end of strings) */

      inptr = strtok(NULL, tokens);                /* point to next field */
      if(isalpha(*inptr) && !isdigit(inptr[1]) &&
              (inptr[0]=='a' && inptr[1]=='r' &&
              !isdigit(inptr[2])) )
         inptr = strtok(NULL, tokens);      /* skip text that isn't a reg */

      switch(refptr[1]) {                   /* what operand type is next? */

         case 'r':                                       /* ALU reg */
            index = strtoul(inptr+1, &throwaway, 10);
            if( isdigit(refptr[2]) ) {
               maxbits = refptr[2]-'0';
               if( index >= (1<<maxbits) )      
                  return PARSE_ERROR;
               else
                  op.r[rptr++] = index;
            }
            else {
               if( index > 127 )
                  return PARSE_ERROR;
               else
                  op.r[rptr++] = index;
            }
            break;

         case 'f':                                       /* FPU reg */
            index = strtoul(inptr+1, &throwaway, 10);
            if( index > 127 )
               return PARSE_ERROR;
            else
               op.f[fptr++] = index;
            break;

         case 'p':                                       /* pred. reg */
            index = strtoul(inptr+1, &throwaway, 10);
            if( index > 63 )
               return PARSE_ERROR;
            else
               op.p[pptr++] = index;
            break;

         case 'b':                                       /* branch reg */
            index = strtoul(inptr+1, &throwaway, 10);
            if( index > 7 )
               return PARSE_ERROR;
            else
               op.b[bptr++] = index;
            break;

         case 'a':                                    /* application reg */
            index = strtoul(inptr+2, &throwaway, 10);
            if( index > 127 )
               return PARSE_ERROR;
            else
               op.ar[arptr++] = index;
            break;

         case 'i':                                   /* signed immediate */
            if(!isalpha(inptr[0]))
              imm = strtoll(inptr, &throwaway, 0);
            else
              imm = get_symbol_value(inptr);  
            if( isdigit(refptr[3]) )
               maxbits = (refptr[2]-'0')*10 + (refptr[3]-'0');
            else
               maxbits = (refptr[2]-'0');

            if( imm >= ((int64)(1)<<(maxbits-1)) ||
                imm < -((int64)(1)<<(maxbits-1)) )
               return PARSE_ERROR;
            else
               op.i[iptr++] = imm;
            break;

         case 'u':                                 /* unsigned immediate */
            if(!isalpha(inptr[0]))
              imm = strtoll(inptr, &throwaway, 0);
            else
              imm = get_symbol_value(inptr);  
            maxbits = (refptr[2]-'0');
	    if(maxbits==6)
	       index = 65;	      /* <-- some ops need 1-64, not 0-63 */
	    else
	       index = 1<<maxbits;

            if( imm >= (int64)index )
               return PARSE_ERROR;        
            else                         
               op.i[iptr++] = imm;      
            break;

         default:
            return UNIMPLEMENTED;
      }

      refptr = strchr(refptr+1,'\\');              /* find next operand */
   }

   return IA_OK;
}

/*-------------------------------------------------------------------*/
uint32 assemble_instruction(char *input, uint32 *unit, qword *out) {

   /* assembles the IA64 instruction pointed to by
      input, returning the functional unit to use in
      *unit and the binary code in "out". */

   lookup *ref;
   uint32 status;

   while(isspace(*input)) input++;             /* parse and strip off  */
   if( *input=='(' ) {                         /* qualifying predicate */
      while(!isdigit(*input)) input++;
      if( isdigit(input[1]) )
         op.pred = (input[0]-'0')*10 + (input[1]-'0');
      else
         op.pred = (input[0]-'0');
      while(!isalpha(*input)) input++;
   }
   else {
      op.pred = 0;
   }

   if( (ref=lookup_instruction(input))==NULL )
      return PARSE_ERROR;

   if( (status=parse_operands(input, ref->format))!=IA_OK )
      return status;

   return (*(ref->asm_func))(unit, out);
}
