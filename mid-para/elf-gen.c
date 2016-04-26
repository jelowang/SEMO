
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

# include "elf.h"
# include "sccl.h"
# include "schal.h"
# include "elf-gen.h"

static ELFGENTOR ElfGentor = { 0 } ;
static int sectoffset [ELF_SECTIONS] = { 0 } ;

int ElfGenInsertTextRelItem ( Elf32_Rel rel ) {

	//	author : Jelo Wang
	//	since : 20100727
	//	(C)TOK

	Elf32_Rel* elf32_rel = (Elf32_Rel* )SCMalloc ( sizeof(Elf32_Rel) ) ;

	if ( !elf32_rel ) return 0 ;	

	SCMemcpy ( elf32_rel , &rel , sizeof (Elf32_Rel) ) ;
	
	SCClListInsert ( &ElfGentor.TextRel , (void* )elf32_rel ) ;	
	
	return 1 ;

}

int ElfGenInsertTextRelaItem ( Elf32_Rela rela ) {

	//	author : Jelo Wang
	//	since : 20100727
	//	(C)TOK

	Elf32_Rela* elf32_rela = (Elf32_Rela* )SCMalloc ( sizeof(Elf32_Rela) ) ;

	if ( !elf32_rela ) return 0 ;	

	SCMemcpy ( elf32_rela , &rela , sizeof (Elf32_Rela) ) ;

	SCClListInsert ( &ElfGentor.TextRela , (void* )elf32_rela ) ;

	return 1 ;
	
}
int ElfGenInsertDataRelItem ( Elf32_Rel rel ) {

	//	author : Jelo Wang
	//	since : 20100727
	//	(C)TOK

	Elf32_Rel* elf32_rel = (Elf32_Rel* )SCMalloc ( sizeof(Elf32_Rel) ) ;

	if ( !elf32_rel ) return 0 ;	

	SCMemcpy ( elf32_rel , &rel , sizeof (Elf32_Rel) ) ;

	SCClListInsert ( &ElfGentor.DataRel , (void* )elf32_rel ) ;	

	return 1 ;

}

int ElfGenInsertDataRelaItem ( Elf32_Rela rela ) {

	//	author : Jelo Wang
	//	since : 20100727
	//	(C)TOK
	
	Elf32_Rela* elf32_rela = (Elf32_Rela* )SCMalloc ( sizeof(Elf32_Rela) ) ;

	if ( !elf32_rela ) return 0 ;	

	SCMemcpy ( elf32_rela , &rela , sizeof (Elf32_Rela) ) ;

	SCClListInsert ( &ElfGentor.DataRela , (void* )elf32_rela ) ;
	
	return 1 ;

}


int ElfGenSymtabGetSymbolInfo () {

	//	author : Jelo Wang
	//	since : 20100728
	//	(C)TOK

	return ElfGentor.Symtab.totall ;

}


int ElfGenSymtabInsertSymbol ( Elf32_Sym sym ) {

	//	author : Jelo Wang
	//	since : 20100727
	//	(C)TOK

	Elf32_Sym* elf32_sym = (Elf32_Sym* )SCMalloc ( sizeof(Elf32_Sym) ) ;

	if ( !elf32_sym ) return 0 ;

	SCMemcpy ( elf32_sym , &sym , sizeof (Elf32_Sym) ) ;
	
	SCClListInsert ( &ElfGentor.Symtab , (void* )elf32_sym ) ;

	return 1 ;

}

int ElfGenShstrtabInsertString ( char* string ) {

	//	author : Jelo Wang
	//	since : 20100728
	//	(C)TOK

	char* s = 0 ; 
	//	plus 1 for '\0'
	ElfGentor.ShstrtabMonior.length = ElfGentor.ShstrtabMonior.length + sc_strlen (string) + 1 ;
	
	sc_strcpy_ex ( &s , string ) ;

	SCClListInsert ( &ElfGentor.Shstrtab , (void* )s ) ;

	return 1 ;

	
}


int ElfGenShstrtabGetEntry () {

	//	author : Jelo Wang
	//	since : 20100728
	//	(C)TOK

	//	plus one for the first element of strtab
	return ElfGentor.ShstrtabMonior.length+1 ;
	
	
}

int ElfGenStrtabInsertString ( char* string ) {

	//	author : Jelo Wang
	//	since : 20100727
	//	(C)TOK
	
	char* s = 0 ; 
	//	plus 1 for '\0'
	ElfGentor.StrtabMonior.length = ElfGentor.StrtabMonior.length + sc_strlen (string) + 1 ;
	
	sc_strcpy_ex ( &s , string ) ;

	SCClListInsert ( &ElfGentor.Strtab , (void* )s ) ;

	return 1 ;
	
}

int ElfGenStrtabGetEntry () {

	//	author : Jelo Wang
	//	since : 20100728
	//	(C)TOK

	//	plus one for the first element of strtab
	return ElfGentor.StrtabMonior.length+1 ;
	
}



int ElfGenGetShndx ( char* sec ) {

	//	author : Jelo Wang
	//	since : 20100728
	//	(C)TOK

	//	hard-coded here is no problem 
	if ( !sc_strcmp ( sec , ".text" ) ) return 2 ;

	return -1 ;
	
}


int ElfGenAddTextValue ( int value ) {

	//	author : Jelo Wang
	//	since : 20100727
	//	(C)TOK

	SCClListInsert ( &ElfGentor.Text , (void* )value ) ;

	return 1 ;
	
}

int ElfGenGetTextScale ( int mul ) {

	//	author : Jelo Wang
	//	since : 20100728
	//	(C)TOK

	return ElfGentor.Text.totall*mul ;
	
}


int ElfGen ( char* out , int setscale ) {

	//	author : Jelo Wang
	//	since : 20100727
	//	(C)TOK

	//	notes : generate elf file
	//	setscale : instruction scale of bytes

	char ident [EI_NIDENT] = {

		ELFMAG0 , ELFMAG1 , ELFMAG2 , ELFMAG3 ,
		ELFCLASS32 , ELFDATA2LSB , ELFCLASS32

	} ;
	
	char copy [] = { "SEMO C()MPILER 0.3.0 Build , (C)Techniques of Knowledge | http://www.tok.cc"} ;
	void* file = SCHalFileOpen ( out , "wb+" ) ;

	SCFree ( out ) ;
	
	if ( !file ) return 0 ;

	SCMemcpy ( ElfGentor.header.e_ident , ident , EI_NIDENT ) ;
	//	section header generating
	ElfGentor.header.e_type = ET_REL ;
	ElfGentor.header.e_machine = 40 ;
	ElfGentor.header.e_version = EV_CURRENT ;
	ElfGentor.header.e_entry = 0 ;
	ElfGentor.header.e_phoff = 0 ;
	ElfGentor.header.e_shoff = sizeof(Elf32_Ehdr)+sizeof(copy) ;
	ElfGentor.header.e_flags = 33554432 ;
	ElfGentor.header.e_ehsize = sizeof(Elf32_Ehdr) ;
	ElfGentor.header.e_phentsize = sizeof(Elf32_Phdr) ;
	ElfGentor.header.e_phnum = 0 ;
	ElfGentor.header.e_shentsize = sizeof(Elf32_Shdr) ;
	ElfGentor.header.e_shnum = 5 ;
	ElfGentor.header.e_shstrndx = 1 ;
		
	//	generate elf header
	SCHalFileWrite ( file , &ElfGentor.header , sizeof (Elf32_Ehdr) , 1 ) ;
	
	//	generate copyrights
	SCHalFileWrite ( file , copy , sizeof(copy) , 1 ) ;

	//	generate sections 

	sectoffset[ELF_UNDEF] = sizeof(Elf32_Ehdr)+sizeof(copy) ;
	ElfGentor.sect[ELF_UNDEF].sh_type = SHT_NULL ;
	ElfGentor.sect[ELF_UNDEF].sh_link = SHN_UNDEF ;

	//	.shstrtab 
	sectoffset[ELF_SHSTRTAB] = sectoffset[ELF_UNDEF]+sizeof(Elf32_Shdr) ;
	ElfGentor.sect[ELF_SHSTRTAB].sh_type = SHT_STRTAB ;
	ElfGentor.sect[ELF_SHSTRTAB].sh_flags = 0 ;
	ElfGentor.sect[ELF_SHSTRTAB].sh_name = ElfGenShstrtabGetEntry () ;
	ElfGentor.sect[ELF_SHSTRTAB].sh_entsize = sizeof(Elf32_Shdr) ;
	ElfGenShstrtabInsertString ( ".shstrtab"  ) ;
	
	//	.text
	sectoffset[ELF_TEXT] = sectoffset[ELF_SHSTRTAB]+sizeof(Elf32_Shdr) ;
	ElfGentor.sect[ELF_TEXT].sh_type = SHT_PROGBITS ;
	ElfGentor.sect[ELF_TEXT].sh_flags = SHF_EXECINSTR | SHF_ALLOC ;
	ElfGentor.sect[ELF_TEXT].sh_name = ElfGenShstrtabGetEntry () ;
	ElfGentor.sect[ELF_TEXT].sh_entsize = sizeof(Elf32_Shdr) ;
	ElfGenShstrtabInsertString ( ".text"  ) ;		
	
	//	.symtab
	sectoffset[ELF_SYMTAB] = sectoffset[ELF_TEXT]+sizeof(Elf32_Shdr) ;
	ElfGentor.sect[ELF_SYMTAB].sh_type = SHT_SYMTAB ;
	ElfGentor.sect[ELF_SYMTAB].sh_size = ElfGentor.Symtab.totall*sizeof(Elf32_Sym);	
	ElfGentor.sect[ELF_SYMTAB].sh_name = ElfGenShstrtabGetEntry () ;
	ElfGentor.sect[ELF_SYMTAB].sh_entsize = sizeof(Elf32_Sym) ;
	ElfGenShstrtabInsertString ( ".symtab"  ) ;		
	
	//	.strtab
	sectoffset[ELF_STRTAB] = sectoffset[ELF_SYMTAB]+sizeof(Elf32_Shdr) ;
	ElfGentor.sect[ELF_STRTAB].sh_type = SHT_STRTAB ;
	ElfGentor.sect[ELF_STRTAB].sh_flags = 0 ;
	ElfGentor.sect[ELF_STRTAB].sh_name = ElfGenShstrtabGetEntry () ;
	ElfGentor.sect[ELF_STRTAB].sh_entsize = 0 ;
	ElfGenShstrtabInsertString ( ".strtab"  ) ;		

	//	write .text contents
	SCHalFileSeek ( file , sectoffset[ELF_STRTAB]+sizeof(Elf32_Shdr) , SEEK_HEAD ) ;
	ElfGentor.sect[ELF_TEXT].sh_offset = sectoffset[ELF_STRTAB]+sizeof(Elf32_Shdr) ;
	ElfGentor.sect[ELF_TEXT].sh_size = ElfGenGetTextScale ( setscale ) ;
		
	for ( SCClListSetIterator ((void* )&ElfGentor.Text , SCCLLISTSEEK_HEAD ) ; 
		SCClListIteratorPermit ((void* )&ElfGentor.Text) ; 
		SCClListListIteratorNext ((void* )&ElfGentor.Text) ) 
	{
		int value = (int )SCClListIteratorGetElement ( (void* )&ElfGentor.Text ) ;
		SCHalFileWrite ( file , &value , setscale , 1 ) ;
	}


	//	write .shstrtab contents 
	SCHalFileSeek ( file , sectoffset[ELF_STRTAB]+sizeof(Elf32_Shdr)+ElfGenGetTextScale ( setscale ) , SEEK_HEAD ) ;
	ElfGentor.sect[ELF_SHSTRTAB].sh_offset = sectoffset[ELF_STRTAB]+sizeof(Elf32_Shdr)+ElfGenGetTextScale ( setscale )  ;
	//	generate shstrtab
	{
		int value = 0 ; 
		//	the first element of shstrtab must be null
		SCHalFileWrite ( file , &value , 1 , 1 ) ;
		ElfGentor.sect[ELF_SHSTRTAB].sh_size ++ ;
	}
	for ( SCClListSetIterator ((void* )&ElfGentor.Shstrtab , SCCLLISTSEEK_HEAD ) ; 
		SCClListIteratorPermit ((void* )&ElfGentor.Shstrtab) ; 
		SCClListListIteratorNext ((void* )&ElfGentor.Shstrtab) ) 
	{
		int lens = 0 ;
		int value = 0 ;
		char* string = (char* )SCClListIteratorGetElement ( (void* )&ElfGentor.Shstrtab ) ;
		lens = sc_strlen (string) ;
		SCHalFileWrite ( file , string , 1 , lens ) ;
		SCHalFileWrite ( file , &value , 1 , 1 ) ;
		SCFree ( string ) ;
		ElfGentor.sect[ELF_SHSTRTAB].sh_size = ElfGentor.sect[ELF_SHSTRTAB].sh_size + lens + 1 ;
	}
		
	//	write .strtab contents 
	SCHalFileSeek ( file , sectoffset[ELF_STRTAB]+sizeof(Elf32_Shdr)+ElfGenGetTextScale ( setscale )+ElfGentor.sect[ELF_SHSTRTAB].sh_size , SEEK_HEAD ) ;
	ElfGentor.sect[ELF_STRTAB].sh_offset = sectoffset[ELF_STRTAB]+sizeof(Elf32_Shdr)+ElfGenGetTextScale ( setscale )+ElfGentor.sect[ELF_SHSTRTAB].sh_size  ;
	//	generate strtab
	{
		int value = 0 ; 
		//	the first element of strtab be null
		SCHalFileWrite ( file , &value , 1 , 1 ) ;
		ElfGentor.sect[ELF_STRTAB].sh_size ++ ;
	}
	for ( SCClListSetIterator ((void* )&ElfGentor.Strtab , SCCLLISTSEEK_HEAD ) ; 
		SCClListIteratorPermit ((void* )&ElfGentor.Strtab) ; 
		SCClListListIteratorNext ((void* )&ElfGentor.Strtab) ) 
	{
		int lens = 0 ;
		int value = 0 ;
		char* string = (char* )SCClListIteratorGetElement ( (void* )&ElfGentor.Strtab ) ;
		lens = sc_strlen (string) ;
		SCHalFileWrite ( file , string , 1 , lens ) ;
		SCHalFileWrite ( file , &value , 1 , 1 ) ;
		SCFree ( string ) ;
		ElfGentor.sect[ELF_STRTAB].sh_size = ElfGentor.sect[ELF_STRTAB].sh_size + lens + 1 ;
	}
		
	SCHalFileSeek ( file , sectoffset[ELF_STRTAB]+sizeof(Elf32_Shdr)+ElfGenGetTextScale ( setscale )+ElfGentor.sect[ELF_STRTAB].sh_size , SEEK_HEAD ) ;
	ElfGentor.sect[ELF_SYMTAB].sh_offset = sectoffset[ELF_STRTAB]+sizeof(Elf32_Shdr)+ElfGenGetTextScale ( setscale )+\
		ElfGentor.sect[ELF_STRTAB].sh_size+ElfGentor.sect[ELF_SHSTRTAB].sh_size  ;
	
	//	write .symtab contents
	SCHalFileSeek ( file , ElfGentor.sect[ELF_SYMTAB].sh_offset , SEEK_HEAD ) ;
	for ( SCClListSetIterator ((void* )&ElfGentor.Symtab , SCCLLISTSEEK_HEAD ) ; 
		SCClListIteratorPermit ((void* )&ElfGentor.Symtab) ; 
		SCClListListIteratorNext ((void* )&ElfGentor.Symtab) ) 
	{
		Elf32_Sym* elf32_sym = (Elf32_Sym* )SCClListIteratorGetElement ( (void* )&ElfGentor.Symtab ) ;
		SCHalFileWrite ( file , elf32_sym , sizeof (Elf32_Sym) , 1 ) ;
		SCFree ( elf32_sym ) ;
	}

	//	the first section must be ELF_UNDEF
	SCHalFileSeek ( file , sectoffset[ELF_UNDEF] , SEEK_HEAD ) ;
	SCHalFileWrite ( file , &ElfGentor.sect[ELF_UNDEF] , sizeof(Elf32_Shdr)   , 1 ) ;
	
	SCHalFileSeek ( file , sectoffset[ELF_SHSTRTAB] , SEEK_HEAD ) ;
	SCHalFileWrite ( file , &ElfGentor.sect[ELF_SHSTRTAB] , sizeof(Elf32_Shdr)  , 1 ) ;
	
	SCHalFileSeek ( file , sectoffset[ELF_TEXT] , SEEK_HEAD ) ;
	SCHalFileWrite ( file , &ElfGentor.sect[ELF_TEXT] , sizeof(Elf32_Shdr)  , 1 ) ;

	SCHalFileSeek ( file , sectoffset[ELF_SYMTAB] , SEEK_HEAD ) ;
	SCHalFileWrite ( file , &ElfGentor.sect[ELF_SYMTAB] , sizeof(Elf32_Shdr) , 1 ) ;

	SCHalFileSeek ( file , sectoffset[ELF_STRTAB] , SEEK_HEAD ) ;
	SCHalFileWrite ( file , &ElfGentor.sect[ELF_STRTAB] , sizeof(Elf32_Shdr) , 1 ) ;
				
	SCClListDestroy ( &ElfGentor.TextRel ) ;
	SCClListDestroy ( &ElfGentor.TextRela ) ;
	SCClListDestroy ( &ElfGentor.DataRel ) ;
	SCClListDestroy ( &ElfGentor.DataRela ) ;
	SCClListDestroy ( &ElfGentor.Shstrtab ) ;
	SCClListDestroy ( &ElfGentor.Symtab ) ;
	SCClListDestroy ( &ElfGentor.Strtab ) ;
	
	SCHalFileClose ( file ) ;
	
	return 1 ;

}

