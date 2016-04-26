
/*

+	ELF generator , part of SC common library

+	'Semo Compiler' is a multi-objective compiler which is developing under the terms of the 
+	GNU general public license as published by the Free Software Foundation.
+	The project lunched by Jelo Wang since 2.Feb.2008 from 'Techniques of Knowledge' community. 

+	You can redistribute it and/or modify it under the terms of the gnu general public version 3 of 
+	the license as published by the free software foundation.this program is distributed in the hope 
+	that it will be useful,but without any warranty.without even the implied warranty of merchantability 
+	or fitness for a particular purpose.																					
																												
+	(C)	突壳开源Techniques of Knowledge
+		an open source community since 2008
+		Community : http://www.tok.cc
+		Contact Us : jelo.wang@gmail.com

+		-Thanks to Our Committers and Friends
+		-Best Wish to all who Contributed and Inspired
+		-Techniques of Knowledge 
+		-致伙伴们最美好祝愿
+		-突壳开源社区

*/ 

# ifndef __ELF_GEN
# define __ELF_GEN

# include "elf.h"
# include "sccl.h"

enum {

	//	author : Jelo Wang	
	//	since : 20100728
	//	(C)TOK

	ELF_UNDEF ,
	ELF_SHSTRTAB ,
	ELF_TEXT ,
	ELF_DATA ,
	ELF_RODATA ,
	ELF_CONST ,
	ELF_BSS ,

	ELF_SYMTAB ,
	ELF_STRTAB ,
	
	ELF_TEXT_REL ,
	ELF_TEXT_RELA ,
	ELF_DATA_REL ,
	ELF_DATA_RELA ,

	ELF_SECTIONS = 13 , 

} ;

typedef struct {

	//	author : Jelo Wang	
	//	since : 20100727
	//	(C)TOK

	Elf32_Ehdr header ;

	//	sections
	Elf32_Shdr sect[ELF_SECTIONS] ;

	//	items of text.rel,text.rel	,data.rel,data.rela	
	SCClList TextRel ;
	SCClList TextRela ;
	SCClList DataRel ;
	SCClList DataRela ;

	//	items of symtab
	//	and contents of strtab
	
	SCClList Shstrtab ;
	SCClList Symtab ;
	SCClList Strtab ;

	//	save data of '.text','.data','.rodata','.constdata','bss'
	SCClList Text ;
	SCClList Data ;
	SCClList ConstData ;
	SCClList Rodata ;

	struct {
		int length ;
	} ShstrtabMonior ;

	struct {
		int length ;
	} StrtabMonior ;	

} ELFGENTOR ;

# endif

extern int ElfGenCreateTextRel ( Elf32_Rel elf32_rel ) ;
extern int ElfGenCreateTextRela ( Elf32_Rela elf32_rela ) ;
extern int ElfGenCreateDataRel ( Elf32_Rel elf32_rel ) ;
extern int ElfGenCreateDataRela ( Elf32_Rela elf32_rela ) ;

extern int ElfGenInsertTextRelItem ( Elf32_Rel elf32_rel ) ;
extern int ElfGenInsertTextRelaItem ( Elf32_Rela elf32_rela ) ;
extern int ElfGenInsertDataRelItem ( Elf32_Rel elf32_rel ) ;
extern int ElfGenInsertDataRelaItem ( Elf32_Rela elf32_rela ) ;

extern int ElfGenAddTextValue ( int value ) ; 
extern int ElfGenGetTextScale ( int mul ) ;

extern int ElfGenSymtabGetSymbolInfo () ;
extern int ElfGenSymtabInsertSymbol ( Elf32_Sym sym ) ;
extern int ElfGenStrtabInsertString ( char* string ) ;
extern int ElfGenStrtabGetEntry () ;
extern int ElfGenGetShndx ( char* sec ) ;

extern int ElfGen ( char* out , int setscale ) ;


