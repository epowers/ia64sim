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

#define WINDOWSIZE 7
qword window[WINDOWSIZE];
uint32 units[WINDOWSIZE]={0};

uint32 initialized=FALSE;
uint32 draining;
qword inop, fnop, mnop, bnop;

uint32 ttable[32][3] = {
{ IMUNIT | MUNIT         ,IMUNIT | IUNIT         ,IMUNIT | IUNIT          },
{ IMUNIT | MUNIT         ,IMUNIT | IUNIT         ,IMUNIT | IUNIT  | STOP  },
{ IMUNIT | MUNIT         ,IMUNIT | IUNIT | STOP  ,IMUNIT | IUNIT          },
{ IMUNIT | MUNIT         ,IMUNIT | IUNIT | STOP  ,IMUNIT | IUNIT  | STOP  },
{ IMUNIT | MUNIT         ,XUNIT1                 ,XUNIT2                  },
{ IMUNIT | MUNIT         ,XUNIT1 | STOP          ,XUNIT2 | STOP           },
{ 0                      ,0                      ,0                       },
{ 0                      ,0                      ,0                       },
{ IMUNIT | MUNIT         ,IMUNIT | MUNIT         ,IMUNIT | IUNIT          },
{ IMUNIT | MUNIT         ,IMUNIT | MUNIT         ,IMUNIT | IUNIT  | STOP  },
{ IMUNIT | MUNIT | STOP  ,IMUNIT | MUNIT         ,IMUNIT | IUNIT          },
{ IMUNIT | MUNIT | STOP  ,IMUNIT | MUNIT         ,IMUNIT | IUNIT  | STOP  },
{ IMUNIT | MUNIT         ,FUNIT                  ,IMUNIT | IUNIT          },
{ IMUNIT | MUNIT         ,FUNIT                  ,IMUNIT | IUNIT  | STOP  },
{ IMUNIT | MUNIT         ,IMUNIT | MUNIT         ,FUNIT                   },
{ IMUNIT | MUNIT         ,IMUNIT | MUNIT         ,FUNIT | STOP            },
{ IMUNIT | MUNIT         ,IMUNIT | IUNIT         ,BUNIT                   },
{ IMUNIT | MUNIT         ,IMUNIT | IUNIT         ,BUNIT | STOP            },
{ IMUNIT | MUNIT         ,BUNIT                  ,BUNIT                   },
{ IMUNIT | MUNIT         ,BUNIT                  ,BUNIT | STOP            },
{ 0                      ,0                      ,0                       },
{ 0                      ,0                      ,0                       },
{ BUNIT                  ,BUNIT                  ,BUNIT                   },
{ BUNIT                  ,BUNIT                  ,BUNIT | STOP            },
{ IMUNIT | MUNIT         ,IMUNIT | MUNIT         ,BUNIT                   },
{ IMUNIT | MUNIT         ,IMUNIT | MUNIT         ,BUNIT | STOP            },
{ 0                      ,0                      ,0                       },
{ 0                      ,0                      ,0                       },
{ IMUNIT | MUNIT         ,FUNIT                  ,BUNIT                   },
{ IMUNIT | MUNIT         ,FUNIT                  ,BUNIT | STOP            },
{ 0                      ,0                      ,0                       },
{ 0                      ,0                      ,0                       }
};

/*--------------------------------------------------------------------*/
uint32 bundle_from_file( bundle *vliw, FILE *infile ) {

   /*----------------------------------------------------------
      fills a window of WINDOWSIZE-1 instructions from infile
      (unless the file runs out or the window is already full),
      then picks at most three of the instructions to be packed into
      *vliw. The rest of the slots are filled with nops. The
      code makes sure to abide by stop rules and also moves
      remaining instructions forward in the queue.

      Returns:      IA_OK    bundle was created, file not empty
                    DRAIN    bundle created, file empty, window not empty
                    EMPTY    bundle created, file empty, window empty
                    else     bundle not created
    -----------------------------------------------------------*/

   uint32 score, index[3];
   uint32 template, maxscore, maxindex[3];
   uint32 nostop, i, j, k, m, stop, limit, status;
   uint32 mask1, mask2;
   char next_inst[100], *temp;
   qword *slotptr[3];
   struct symbol_table *ref;
   int64 iprel;

   if (initialized==FALSE) {
      inop.q = (uint64)(1)<<27;
      mnop.q = (uint64)(1)<<27;
      bnop.q = (uint64)(2)<<37;
      fnop.q = (uint64)(1)<<27;
      for (i=0;i<WINDOWSIZE;i++) {
        window[i].q = 0;
        units[i] = 0;
        }
      draining = FALSE;
      initialized = TRUE;
      }

                                /*--------------- FILL WINDOW ------------*/

   for(i=0; units[i]; i++);             /* point to next instruction */

   while( i<WINDOWSIZE-1 && !feof(infile) && !draining) {
   
      fgets( next_inst, sizeof(next_inst), infile );    /* get next line */
      
      if( next_inst[1]=='#') {
        draining = TRUE;
        break;
        }
        
      if( (temp=strchr(next_inst, '/')) )
         if (temp[1]=='/') *temp = (char) NULL;                /* strip comments */

      if( (temp=strchr(next_inst, ';')) ) {             /* look for stops */
         if (temp[1]==';') {
            *temp = (char) NULL; stop = STOP;
         }
      }
      else {
         stop = 0;
      }
      

 
        
      /* if the line is not all whitespace, assemble it
         (and apply a stop flag if necessary) */

      temp = next_inst;
      while ( *temp && isspace(*temp) ) temp++;
      
        

      if( *temp && !feof(infile) ) {
        if((temp=strchr(next_inst,':')))              /* look for label */
         status = assemble_label(next_inst,&units[i],&window[i]);
        else
         status = assemble_instruction( next_inst, &units[i], &window[i] );
         
         if( status!=IA_OK ) {
            printf("%s: %s\n", error_strings[status], next_inst);
            return status;
            }
            
         units[i++] |= stop;
         if(units[i]) units[i++] |= stop;   /* for double-size movl instr */
      }

   }

   if( (feof(infile) || draining) && !i )  {                 /* ran out of instructions? */
      initialized = FALSE;
      return EMPTY;
      }

/* Process label definition in  first position */


   while(units[0]&TRGT && window[0].q) {
     ref = (struct symbol_table *) window[0].d[0];  /* point to symbol table entry */
     ref->symbol_value = seg_tbl[0].seg_length; /* set symbol value to current segment position */
     units[0] = 0;                       /* delete label definition */
     window[0].q=0;
     for(j=0;j<i;j++) {                 /* move remaining instruction up */
         window[j].q = window[j+1].q;
         units[j] = units[j+1];
         }
     i--;
     }
     

                               /*------------PICK TEMPLATE--------------*/

      /* By now i points to the first empty window entry
         (or past the window entirely). The template to
         choose will be the winner in a competition between
         all the templates: whoever can grab three instructions
         first, or at least grab the maximum number first, wins. */
         
   stop = i;      
   if(i<3) limit = i;
   else limit = 3;
   maxscore = maxindex[0] =
   maxindex[1] = maxindex[2] = 0;

   for(i=0; i<32; i++) {
      score = 0;
      index[0] = index[1] = index[2] = 0xff;

      /* slot 0: pick the first available instruction, and
         keep looking until the window is used up or a stop
         is hit (that is not itself valid for this slot). 
	 Also, bail out immediately if an instruction with
	 a stop is encountered after a previous instruction
	 without. */

      if( (ttable[i][0] & IMUNIT) ) {
         mask1 = ttable[i][0] & ~IMUNIT;
         mask2 = (ttable[i][0] & STOP) | IMUNIT;
      }
      else {
         mask1 = ttable[i][0];
         mask2 = 0;
      }
 
      for(j=0,nostop=0; j<limit; j++) {
         if(units[j]&TRGT) break;      /* don't consider instructions following label */
	 if( !(units[j]&STOP) ) nostop = 1;
	 if( (units[j]&STOP) && nostop ) break;
         if( units[j]&FIRST || mask1==(units[j]&UMASK) || mask2==(units[j]&UMASK) ){
            index[0] = j; score++; break;
         }
         break;  /* block all out of order optimization until we can do it right */
         if ( (units[j]&STOP) ) break;
      }

      /* slot 1: If the first instruction had a stop, start looking
         beyond that instruction and stop when the window runs out
         or a stop is hit that is invalid. Without a stop in slot 0,
         start from the beginning again. The chosen instruction must
         be different from that in slot 0, but *can* come before. */ 

      if( (ttable[i][1] & IMUNIT) ) {
         mask1 = ttable[i][1] & ~IMUNIT;
         mask2 = (ttable[i][1] & STOP) | IMUNIT;
      }
      else {
         mask1 = ttable[i][1];
         mask2 = 0;
      }

      if( index[0]!=0xff && (units[index[0]] & STOP) )
         k = (j<limit? j+1 : limit);
      else
         k = 0;
         

      for(nostop=0; k<limit; k++) {
         if( index[0]!=k ) {
            if(units[k]&TRGT) break;      /* don't consider instructions following label */
	    if( !(units[k]&STOP) ) nostop = 1;
	    if( (units[k]&STOP) && nostop ) break;
            if( mask1==(units[k]&UMASK) || mask2==(units[k]&UMASK) ){
                  index[1] = k; score++; break;
            }
            break;  /* block all out of order optimization until we can do it right */
            if ( (units[k]&STOP) ) break;
         }
      }

      /* slot 2: repeat the above for the final slot. Only one of the
         first two slots can have a stop, so there's no danger of 
	 starting past a first stop but not a second */

      if( (ttable[i][2] & IMUNIT) ) {
         mask1 = ttable[i][2] & ~IMUNIT;
         mask2 = (ttable[i][2] & STOP) | IMUNIT;
      }
      else {
         mask1 = ttable[i][2];
         mask2 = 0;
      }

      if( index[0]!=0xff && (units[index[0]] & STOP) )
         m = (j<limit? j+1 : limit);
      else if( index[1]!=0xff && (units[index[1]] & STOP) )
         m = (k<limit? k+1 : limit);
      else
         m = 0;

      nostop = 0;
      for(; m<limit; m++) {
         if( index[0]!=m && index[1]!=m ) {
            if(units[m]&TRGT) break;      /* don't consider instructions following label */
	    if( !(units[m]&STOP) ) nostop = 1;
	    if( (units[m]&STOP) && nostop ) break;
            if( mask1==(units[m]&UMASK) || mask2==(units[m]&UMASK) ){
                  index[2] = m; score++; break;
            }
            break;  /* block all out of order optimization until we can do it right */
            if ( (units[m]&STOP) ) break;
         }
      }

      /* okay, compare to how everyone else did */

      if( score>maxscore ) {
         maxscore = score;
         maxindex[0] = index[0];
         maxindex[1] = index[1];
         maxindex[2] = index[2];
         template = i;
      }

      if(score==3) break;       /* no point in continuing if this happens */
   }

                               /*-----------BUILD THE BUNDLE--------------*/
   for(i=0; i<3; i++) {
      if(maxindex[i]!=0xff) { 
         if(units[maxindex[i]] & IPREL) { /* adjust immediate pointer for current IP */
           iprel = -((int64)BITS(window[maxindex[i]],36,36)) <<24 |
                      BITS(window[maxindex[i]],13,32) <<4;
           iprel -= seg_tbl[0].seg_length;
           iprel >>=4;
           window[maxindex[i]].q &= 0x1ee00001fff;
           window[maxindex[i]].q |= (((iprel & 0xfffff)<< 13) |
                                   (((iprel >> 20) & 1) << 36)) ;
           
         } /* end if(units */
         slotptr[i] = window + maxindex[i];
      } /* end if (maxindex */
  
      else {
         if( ttable[template][i] & IUNIT )
            slotptr[i] = &inop;
         if( ttable[template][i] & MUNIT )
            slotptr[i] = &mnop;
         if( ttable[template][i] & BUNIT )
            slotptr[i] = &bnop;
         if( ttable[template][i] & FUNIT )
            slotptr[i] = &fnop;
      } /* end else */
   } /* end for */

   vliw->lo_q.q = slotptr[0]->q << 5 | template;
   vliw->lo_q.q |= slotptr[1]->q << 46;
   vliw->hi_q.q = slotptr[1]->q >> 18 | slotptr[2]->q << 23;
   
   seg_tbl[0].seg_length += 16;            /* update current segment position */

   for(i=0; i<3; i++) {                   /* delete bundled instructions */
      if(maxindex[i]!=0xff) {
         window[maxindex[i]].q = 0;
         units[maxindex[i]] = 0;
      }
   }
   
   i = j = 0;                           /* move the rest up in the queue */
   while(j<stop) {
      if(!units[j])
         j++;
      else{
         window[i].q = window[j].q;
         units[i++] = units[j++];
      }	 
   }

   for(; i<WINDOWSIZE; i++) {            /* initialize the freed buffers */
      window[i].q = 0;
      units[i] = 0;
   }

   if(feof(infile))
      return DRAIN;
   else
      return IA_OK;
}

/*--------------------------------------------------------------------*/
uint32 data_from_file( uint64 base, FILE *infile ) {

   /*----------------------------------------------------------
      assembles data declarations from infile.

      Returns:      IA_OK    success.
                    else     error condition.
    -----------------------------------------------------------*/

   uint32 status,data_size;
   int64 data_value;
   char next_inst[100], *temp, *charpos;
   

   while( !feof(infile)) {
   
      if (!fgets( next_inst, sizeof(next_inst), infile )) break;    /* get next line */
      if( next_inst[1]=='#') {
        break;
        }
        
      if( (temp=strchr(next_inst, '/')) )
         if (temp[1]=='/') *temp = (char) NULL;                /* strip comments */
        
      if((temp=strchr(next_inst,':'))) {             /* look for label */
        seg_tbl[cur_seg].seg_length = base - seg_tbl[cur_seg].seg_addr;
        status = data_label(next_inst);
        next_inst[0] = '\0';                         /* no further processing for this line */
        }
        
      /* if the line is not all whitespace, assemble it */

      temp = next_inst;
      while ( *temp && isspace(*temp) ) temp++;

      if( *temp ) {
        while(isspace(*temp)) temp++;          /* strip whitespace  */
        while(isalpha(*temp)) temp++;         /* strip 'data'      */
        data_size = strtol(temp,&charpos,0);      /* get data length   */
   
        if (data_size == 1) {
          temp = charpos;                         /* point past data type */
          while(isspace(*temp)) temp++;          /* strip whitespace  */
          if (*temp == '"') {
            temp++;                               /* eat delimiter */
            while (*temp != '"') {
              *(mem+base) = *temp;
              base++;
              temp++;
              }
            }
          else {
            data_value = strtoll(temp, &charpos,0);          /* get data value */
            memmove(mem+base,&data_value,1);
            base++;
            }
          }
        else {
          temp = charpos;                         /* point past data type */
          while(isspace(*temp)) temp++;          /* strip whitespace  */
          data_value = strtoll(temp, &charpos,0);          /* get data value */
          switch (data_size) {
           case 2:
           case 4:
           case 8:
              if (base % data_size) base += (data_size - (base % data_size)); /* force alignment */
              memmove(mem+base,&data_value,data_size);
              base += data_size;
              break;
            default:
              return UNIMPLEMENTED;
            }
          }
        }
      }  
      seg_tbl[cur_seg].seg_length = base - seg_tbl[cur_seg].seg_addr;
      return IA_OK;
}
