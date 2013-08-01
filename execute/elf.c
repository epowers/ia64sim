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

/* ELF specific code */

#include "ia64.h"
#include "ia64asm.h"

/*---------------------------------------------------------------------*/
uint32 extract_ins(char *addr, qword *work) {
   bundle *vliw;
   uint32 ins_slot;

   ins_slot = (uint32) addr&3;
   vliw = (void *) ((uint32) addr&~3);
   slot[0].q = ( vliw->lo_q.q >> 5 ); HI(slot[0]) &= 0x1ff;
   slot[1].q = ( vliw->lo_q.q >> 46 ) | ( vliw->hi_q.q << 18 );
                 HI(slot[1]) &= 0x1ff;
   slot[2].q = ( vliw->hi_q.q >> 23 ); HI(slot[2]) &= 0x1ff;
   work->q =  slot[ins_slot].q;
   return IA_OK;
}
/*---------------------------------------------------------------------*/
uint32 stuff_ins(char *addr, qword *work) {
bundle *vliw;
uint32 ins_slot, template;

   ins_slot = (uint32) addr&3;
   vliw = (void *) ((uint32)addr&~3);
   template = LO(vliw->lo_q) & 0x1f;
   slot[ins_slot].q = work->q;
   vliw->lo_q.q = slot[0].q << 5 | template;
   vliw->lo_q.q |= slot[1].q << 46;
   vliw->hi_q.q = slot[1].q >> 18 | slot[2].q << 23;
   return IA_OK;
}
/*---------------------------------------------------------------------*/
uint32 reloc_i22(int64 value, qword *work) {
   work->q |= (uint64) ((value>>7)&0x7fff)<<22 | (value&127)<<13;
   return IA_OK;
}
/*-------------------------------------------------------------------*/

void trcelf(char *format, ...) {
  if (trace_level) {
   va_list arg;
   
   va_start(arg, format);
   vfprintf(stdout, format, arg);
   va_end(arg);   }
}

/*---------------------------------------------------------------------*/
   uint32 load_elf(char *file_image,uint64 base) {
   
   Elf64_Ehdr *elf_hdr;
   Elf64_Shdr *elf_shdr, *this_shdr, *string_hdr, *symbol_hdr, *link_hdr;
   Elf64_Rel  *rel;
   Elf64_Rela *rela;
   Elf64_Sym  *sym;
   char *string_ptr, *symbol_ptr, *loctr;
   uint64 function_ptr,entry_point, short_data, file_length;
   uint32 status, i,j,namex,type,offset,size,addr;
   qword target_ins;
   struct symbol_table *p;
/* process section table entries */
   elf_hdr = (void *) file_image;
   elf_shdr =  (void *) file_image + elf_hdr->e_shoff;
   string_hdr = elf_shdr+elf_hdr->e_shstrndx;
   string_ptr = (void *) file_image + string_hdr ->sh_offset;
   loctr = mem+base;
   this_shdr = elf_shdr;
   for(i=0;i<elf_hdr->e_shnum;i++) {
     switch(this_shdr->sh_type) {
       case SHT_NULL: break; /* silently ignore null header */
       case SHT_PROGBITS: {
        if(this_shdr->sh_size) {
          memmove(loctr,file_image+this_shdr->sh_offset,this_shdr->sh_size);
          this_shdr->sh_addr = (uint32) loctr;
          if(!strcmp(string_ptr+this_shdr->sh_name,".text"))
            entry_point = (uint32) loctr - (uint32) mem;
          else if(!strcmp(string_ptr+this_shdr->sh_name,".sdata"))
            short_data = (uint32) loctr - (uint32) mem;
          loctr += this_shdr->sh_size;
          trcelf("loading section %s\n",string_ptr+this_shdr->sh_name);
          }
        break;
        }
       case SHT_SYMTAB: {
        sym = (void *) file_image + this_shdr->sh_offset;
        link_hdr = elf_shdr + this_shdr->sh_link;
        symbol_ptr = (void *) file_image + link_hdr-> sh_offset;
        trcelf("Symbol Table section %s\n",string_ptr+this_shdr->sh_name);
        for(j=0;j<(this_shdr->sh_size/this_shdr->sh_entsize);j++) {
          if(sym->st_info == 17) entry_point = sym-> st_value;
          namex = sym-> st_name;
          type = sym-> st_info;
          addr = sym-> st_shndx;
          offset = sym-> st_value;
          size = sym-> st_size;
          trcelf("%d  %d  %d  %d  %s\n",type,addr,offset,size,symbol_ptr+namex);
          p = get_symbol(symbol_first,symbol_ptr+namex);
          if (!symbol_first) symbol_first=p;
          p-> symbol_value = sym->st_value;
          p-> symbol_type = sym->st_info;
          p-> symbol_seg = sym->st_shndx;
          p-> symbol_size = sym->st_size;
          sym++;
          }
        break;
        }
       case SHT_STRTAB: {
        trcelf("String Table section %s\n",string_ptr+this_shdr->sh_name);
        break;
        }
       case SHT_RELA: {
        rela = (void *) file_image + this_shdr->sh_offset; /* start of rela entries */
        symbol_hdr = elf_shdr + this_shdr->sh_link;        /* symbol table header */
        link_hdr = elf_shdr + symbol_hdr->sh_link;
        symbol_ptr = (void *) file_image + link_hdr-> sh_offset;
        trcelf("Relocation Table Section %s\n",string_ptr+this_shdr->sh_name);
        for(j=0;j<(this_shdr->sh_size/this_shdr->sh_entsize);j++) {
          sym = (void *) file_image + symbol_hdr->sh_offset; /* start of symbol table entries */
          sym += ELF64_R_SYM(rela->r_info);                  /* related symbol table entry */
          offset = rela->r_offset;
          type = ELF64_R_TYPE(rela->r_info);
          namex = sym-> st_name;
          size = rela->r_addend;
          link_hdr = elf_shdr + 1;             /* target segment header */
          extract_ins((void *) (uint32) link_hdr->sh_addr + rela->r_offset,&target_ins);
          switch(ELF64_R_TYPE(rela->r_info)) {
            case 34:                                  /* immediate 22 */
              reloc_i22(sym->st_value+rela->r_addend,&target_ins);
              break;
            case 42:                                  /* gprel */
              reloc_i22(sym->st_value+rela->r_addend,&target_ins);
              break;
            case 50:                                  /* ltoff */
              reloc_i22(sym->st_value+rela->r_addend,&target_ins);
              break;
            case 103:                                 /* secrel */
              reloc_i22(sym->st_value+rela->r_addend,&target_ins);
              break;
            default:
              printf("Relocation type %d not implemented\n",ELF64_R_TYPE(rela->r_info));
            }
          stuff_ins((void *) (uint32) link_hdr->sh_addr + rela->r_offset,&target_ins);
          trcelf("%d  %d  %d  %s\n",offset,type, size,symbol_ptr+namex);
          rela++;
          }
        break;
        }
       case SHT_NOBITS: {
         if (this_shdr->sh_size) {
           loctr += this_shdr->sh_size;
           trcelf("reserving space for section %s\n",string_ptr+this_shdr->sh_name);
           }
        break;
        } 
       default:
         namex = this_shdr->sh_name;
         type = this_shdr->sh_type;
         addr = this_shdr->sh_addr;
         offset = this_shdr->sh_offset;
         size = this_shdr->sh_size;
         trcelf("%x:  %d  %d  %d  %d  %s\n", this_shdr,
                type,addr,offset,
                size,string_ptr + namex);
     }
     this_shdr++;
   } 
   free(file_image);
   setup(entry_point,short_data);
}