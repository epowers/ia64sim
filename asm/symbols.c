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

/*-------------------------------------------------------------------*/
char *symbol_alloc(char *s) {

    /* returns a pointer to a copy of a string */
    
    char *p;
    
    p = (char *) malloc(strlen(s)+1);
    if (p != NULL) strcpy(p,s);
    return p;
    }

/*-------------------------------------------------------------------*/
struct symbol_table *get_symbol(struct symbol_table *p, char *symbol) {

   /* Returns a pointer to an existing entry or adds a new entry 
      in alphabetical order, and returns a pointer to it. Returns
      NULL if unable to create new entry. */
      
   int32 condition;
   struct symbol_table *match;
   
   if (p==NULL) {   /* add new symbol here */
     p = (struct symbol_table *) malloc(sizeof(struct symbol_table));
     p->symbol_name = symbol_alloc(symbol); 
     p->left = p->right = NULL;
     p->symbol_seg = p-> symbol_type = 0;
     p->symbol_value = -1;
     match = p;
     }
   else if ((condition = strcmp(symbol,p->symbol_name)) == 0)
     match = p;
   else if (condition < 0) {
      match = get_symbol(p->left, symbol);
      if(p->left == NULL) p->left = match;
      }
   else {
      match = get_symbol(p->right, symbol);
      if(p->right == NULL) p->right = match;
      }
   return match;
}


/*-------------------------------------------------------------------*/
uint32 assemble_label(char *input, uint32 *unit, qword *out) {

   /* assembles the instruction label pointed to by
      input, returning TRGT in
      *unit and a symbol table pointer in "out". */

   uint32 status;
   char *temp;
   struct symbol_table *target;

   while(isspace(*input)) input++;             /* strip whitespace  */
   if((temp=strchr(input,':'))) *temp = (char) NULL; /* strip label delimiter */
   target = get_symbol(symbol_first,input); /* get pointer to new symbol table entry */
   if(target == NULL) return MEMORY_EXCEPTION;
   *unit =  TRGT;
   *out = (qword)(uint64) (uint32) target;
   if(symbol_first == NULL) symbol_first = target;
   return IA_OK; 

}   

/*-------------------------------------------------------------------*/
uint32 data_label(char *input) {

   /* adds the data label pointed to by
      input to the symbol table */

   uint32 status;
   char *temp;
   struct symbol_table *target;

   while(isspace(*input)) input++;             /* strip whitespace  */
   if((temp=strchr(input,':'))) *temp = (char) NULL; /* strip label delimiter */
   target = get_symbol(symbol_first,input); /* get pointer to new symbol table entry */
   if(target == NULL) return MEMORY_EXCEPTION;
   if(symbol_first == NULL) symbol_first = target;
   target->symbol_value = seg_tbl[1].seg_length; /* set symbol value to current segment position */
   target->symbol_seg = 1;
   return IA_OK; 

}/*-------------------------------------------------------------------*/   
uint64 get_symbol_value(char *symbol) {

    struct symbol_table *entry;
    char *localp;
    uint32 l;
    
    localp = symbol_alloc(symbol); /* local copy to edit */
    l = strcspn(localp," \n\0");   /* find end of field */
    localp[l+1] = '\0';            /* delimit field */
    entry = get_symbol(symbol_first,localp);    /* point to symbol table entry */
    l = entry->symbol_value;  /* numberic value of symbol */
    free(localp);                  /* no longer need copy */
    return l;                 /* return symbol value */

}
/*-------------------------------------------------------------------*/
void print_symbols(struct symbol_table *p) {

   /* Print Symbol Table in Alphabetical Order */
   
   if (p != NULL) {
     print_symbols(p->left);
     if(trace_level)
        printf("%x %18s %8d %8d %8d %8d %x %x\n",p,p->symbol_name,
             (int32)(p->symbol_value + seg_tbl[p->symbol_seg].seg_addr),
             p->symbol_size,p->symbol_type,p->symbol_seg,p->left,p->right);
     else
        printf("%18s %8d %8d %8d %8d\n",p->symbol_name,
             (int32)( p->symbol_value + seg_tbl[p->symbol_seg].seg_addr),
             p->symbol_size,p->symbol_type,p->symbol_seg);
     print_symbols(p->right);
     }
}
   
   /*-------------------------------------------------------------------*/
void free_symbols(struct symbol_table *p) {

   /* Free Symbol Table Tree */
   
   if (p != NULL) {
     free_symbols(p->left);
     free(p->symbol_name);
     free_symbols(p->right);
     free(p->left);
     free(p->right);
     }
}
     