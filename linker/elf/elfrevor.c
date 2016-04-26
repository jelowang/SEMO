
/*

+	Executable Linking-Library 1.0.0.
+	ELL-ARM-ELF based 2.0 since 2010.
+	Architecture : ARMv7.

+	You can redistribute it and/or modify it under the terms of the gnu general public license	
+	as published by the free software foundation, either version 3 of the license or any later 	
+	version.this program is distributed in the hope	that it will be useful,but without any 		
+	warranty.without even the implied warranty of merchantability or fitness for a particular 	
+	purpose.																					
																												
+	(c)	Techniques of Knowledge
+		an open source group since 2008
+		page : http://www.tok.cc
+		email : wqw85@sina.com

+		技术支持、功能扩展、环境搭建。
+		欢迎联系我们。

*/

# include "elf.h"
# include "ell.h"
# include "ellsl.h"
# include "ellhal.h"
# include "ellrevor.h"
# include "elllinker.h"

int EllResolver ( int obid , char* path ) {

	//	author : Jelo Wang
	//	notes : Resolve elf file
	//	(C)TOK
	
	char srbuffer [ 256 ] = {0} ;
	
	int file = 0 ;
	int looper = 0 ;
	
	int sh_offset = 0 ;
	int st_offset = 0 ;

	int sh_totall = 0 ;
	
	Elf32_Ehdr elf32_ehdr = {0} ;				
	Elf32_Shdr elf32_shdr = {0} ;

	Elf32_Shdr* aelf32_shdr = 0 ;

	file = EllHalFileOpen ( path , ELLHAL_READ_OPEN ) ;
	
	if ( !file ) {
		
		# ifdef ELL_DEBUG
			EllLog ( "EllResolver Error -> Can not open the file '%s'.", path ) ;
		# endif
		
		return 0 ;
	}
	
	EllHalFileRead ( file , &elf32_ehdr , sizeof(Elf32_Ehdr) , 1 ) ;
	
	if ( !EllElfMapCheckHeader ( elf32_ehdr.e_ident ) ) return 0 ;
	if ( !EllElfMapNolSectCreate ( obid , elf32_ehdr.e_shnum ) ) return 0 ;

	sh_offset = elf32_ehdr.e_shoff ;

	ell->ObjectBased[obid] = EllLinkerMemoryPool.looper ;

	//	locate .strtab	
	EllHalFileSeek ( file , elf32_ehdr.e_shoff + elf32_ehdr.e_shentsize * elf32_ehdr.e_shstrndx , ELLHAL_SEEK_HEAD ) ;
	EllHalFileRead ( file , &elf32_shdr , sizeof(Elf32_Shdr) , 1 ) ;
	st_offset = elf32_shdr.sh_offset ;

	for ( looper = 0 ; looper < elf32_ehdr.e_shnum ; looper ++ ) {

		EllHalFileSeek ( file , sh_offset , ELLHAL_SEEK_HEAD ) ;
		EllHalFileRead ( file , &elf32_shdr , sizeof(Elf32_Shdr) , 1 ) ;
		
		EllSlGetString ( file , st_offset + elf32_shdr.sh_name , srbuffer ) ;

		if ( SHT_PROGBITS == elf32_shdr.sh_type ) {
			
			if ( !strcmp ( ".text" , srbuffer ) ) {			
				EllElfMapNolSectInsert ( obid , looper , &elf32_shdr , (const char*)".text" ) ;
			} else if ( !strcmp ( ".data" , srbuffer ) ) {
				EllElfMapNolSectInsert ( obid , looper , &elf32_shdr , (const char*)".data" ) ;
			} else if ( !strcmp ( ".rodata" , srbuffer ) ) {
				EllElfMapNolSectInsert ( obid , looper , &elf32_shdr , (const char*)".rodata" ) ;
			} else if ( !strcmp ( ".constdata" , srbuffer ) ) {
				EllElfMapNolSectInsert ( obid , looper , &elf32_shdr , (const char*)".constdata" ) ;
			} else if ( !strcmp ( "_ell_text" , srbuffer ) ) {
				EllElfMapNolSectInsert ( obid , looper , &elf32_shdr , (const char*)"_ell_text" ) ;
			}
			
		} else if ( SHT_SYMTAB == elf32_shdr.sh_type ) {
		
			if ( !strcmp ( ".symtab" , srbuffer ) ) {			
				EllElfMapNolSectInsert ( obid , looper , &elf32_shdr , (const char*)".symtab" ) ;
			} 
			
		} else if ( SHT_REL == elf32_shdr.sh_type ) {

			if ( !strcmp ( ".rel.text" , srbuffer ) ) {
				EllElfMapNolSectInsert ( obid , looper , &elf32_shdr , (const char*)".rel.text" ) ;
			} else if ( !strcmp ( ".rel.data" , srbuffer ) ) {
				EllElfMapNolSectInsert ( obid , looper , &elf32_shdr , (const char*)".rel.data" ) ;
			} else if ( !strcmp ( ".rel_ell_text" , srbuffer ) ) {
				EllElfMapNolSectInsert ( obid , looper , &elf32_shdr , (const char*)".rel_ell_text" ) ;
			}

		} else if ( SHT_NOBITS == elf32_shdr.sh_type ) {
		
			if ( !strcmp ( ".bss" , srbuffer ) ) {	
				EllElfMapNolSectInsert ( obid , looper , &elf32_shdr , (const char*)".bss" ) ;
			} 
			
		} else if ( SHT_STRTAB == elf32_shdr.sh_type ) {
		
			//	there are two types of SHT_STRTAB which is indicating SECTION string table and SYMBOL string table
			//	.strtab holds SYMBOL strings
			if ( !strcmp ( ".strtab" , srbuffer ) ) {	
				EllElfMapNolSectInsert ( obid , looper , &elf32_shdr , (const char*)".strtab" ) ;
			}
			
		}
		
		sh_offset = sh_offset + elf32_ehdr.e_shentsize ;
		
	}

	//	get .strtab offset , and resovle it
	aelf32_shdr = (Elf32_Shdr* ) EllElfMapNolSectGet ( obid , ".strtab" ) ;
	if ( 0 == aelf32_shdr ) return 0 ;
	
	st_offset = aelf32_shdr->sh_offset ;

	//	get .strtab offset , and resovle it
	aelf32_shdr = (Elf32_Shdr* ) EllElfMapNolSectGet ( obid , ".symtab" ) ;
	if ( 0 == aelf32_shdr ) return 0 ;
	
	sh_totall = aelf32_shdr->sh_size / aelf32_shdr->sh_entsize ;
	sh_offset = aelf32_shdr->sh_offset ;

	if ( !EllDynamicPoolCreateSymbols ( obid , sh_totall ) ) {
		EllHalFileClose ( file ) ;
		return 0 ;
	}
		
	for ( looper = 0 ; looper < sh_totall ; looper ++ ) {

		Elf32_Sym elf32_sym = {0} ;
		
		EllHalFileSeek ( file , sh_offset , ELLHAL_SEEK_HEAD ) ;
		EllHalFileRead ( file , &elf32_sym , sizeof(Elf32_Sym) , 1 ) ;
		EllSlGetString ( file , st_offset + elf32_sym.st_name , srbuffer ) ;

		EllDynamicPoolInsertSymbol ( obid , (void*)&elf32_sym , srbuffer , looper ) ;

		sh_offset = sh_offset + aelf32_shdr->sh_entsize ;
			
	}
	
	//	get .rel.text reloctab
	aelf32_shdr = (Elf32_Shdr* ) EllElfMapNolSectGet ( obid , ".rel.text" ) ;
	
	if ( 0 == aelf32_shdr ) aelf32_shdr = (Elf32_Shdr* ) EllElfMapNolSectGet ( obid , ".rel_ell_text" ) ;

	if ( aelf32_shdr ) {
	
		sh_totall = aelf32_shdr->sh_size / aelf32_shdr->sh_entsize ;
		sh_offset = aelf32_shdr->sh_offset ;
		EllHalFileSeek ( file , sh_offset , ELLHAL_SEEK_HEAD ) ;

		if ( !EllElfMapRelocRelCreate ( ell->TextRel.elf32_rel , obid , sh_totall ) ) {
			EllHalFileClose ( file ) ;
			return 0 ;
		}

		for ( looper = 0 ; looper < sh_totall ; looper ++ ) {

			Elf32_Rel elf32_rel = {0} ;
				
			EllHalFileRead ( file , &elf32_rel , sizeof(Elf32_Rel) , 1 ) ;
			EllElfMapRelocRelInsert ( ell->TextRel.elf32_rel , obid , &elf32_rel ) ;

		}

	}

	
	//	get .rel.data reloctab
	aelf32_shdr = (Elf32_Shdr* ) EllElfMapNolSectGet ( obid , ".rel.data" ) ;
	
	if ( aelf32_shdr ) {
	
		sh_totall = aelf32_shdr->sh_size / aelf32_shdr->sh_entsize ;
		sh_offset = aelf32_shdr->sh_offset ;
		EllHalFileSeek ( file , sh_offset , ELLHAL_SEEK_HEAD ) ;
			
		if ( !EllElfMapRelocRelCreate ( ell->DataRel.elf32_rel , obid , sh_totall ) ) {
			EllHalFileClose ( file ) ;
			return 0 ;
		}
		
		for ( looper = 0 ; looper < sh_totall ; looper ++ ) {

			Elf32_Rel elf32_rel = {0} ;
				
			EllHalFileRead ( file , &elf32_rel , sizeof(Elf32_Rel) , 1 ) ;
			EllElfMapRelocRelInsert ( ell->DataRel.elf32_rel , obid , &elf32_rel ) ;

		}

	}
	
	return file ;
	
}

