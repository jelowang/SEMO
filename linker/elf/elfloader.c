
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

# include "ell.h"
# include "ellsl.h"
# include "ellhal.h"
# include "ellrevor.h"
# include "ellloader.h"
# include "elllinker.h"

void EllMemoryRegister ( void* buffer , int length ) {

	//	author : Jelo Wang
	//	register a memory pool for ell loading & linking & run
	//	(C)TOK

	EllLinkerMemoryPool.pool = (unsigned char* ) buffer ;
	EllLinkerMemoryPool.base = (int ) buffer ;

	EllLinkerMemoryPool.looper = 0 ;
	EllLinkerMemoryPool.length = length ;

}

int EllInstall ( int routineset , char* application ) {

	//	author : Jelo Wang
	//	notes : load elf files and linking
	//	(C)TOK	

	int obid = 0 ;
	int walker = 0 ;
	int results = 1 ;

	ELL* ell = (ELL* )EllDynamicPoolCreate () ;
	EllDynamicPoolSetCurrent ( (int)ell ) ;
	if ( !EllDynamicPoolInsertApplication ( application ) ) return 0 ;

	EllLinker.routineset = routineset ;
	
	for ( EllSlListSetIterator ( ell->ObjectList , ELLSLSEEK_HEAD ) ; EllSlListIteratorPermit  ( ell->ObjectList ) ; EllSlListIteratorNext ( ell->ObjectList ) ) {

		char* objectpath = (char*) EllSlListIteratorGetElement ( ell->ObjectList ) ;
	
		results = EllResolver ( obid , objectpath ) ;
		
		if ( 0 == results ) break ;
		
		results = EllLocalLinker ( obid , results ) ;
		
		if ( 0 == results ) break ;

		obid ++ ;
		
	}
	EllSlListDestroy ( ell->ObjectList ) ;

	//	link all of the object-files
	EllGlobalLinker ( obid ) ;
	
	EllElfMapNolSectDestroy ( EllLinker.obidborder ) ;
	EllElfMapRelocDestroy ( ell->TextRel.elf32_rel , EllLinker.obidborder ) ;
	EllElfMapRelocDestroy ( ell->DataRel.elf32_rel , EllLinker.obidborder ) ;
	EllFreeEx (&ell->ObjectBased) ;

	if ( !results ) EllDynamicPoolDestroy () ;

	EllDump ( "e:\\ell\\ellmem.elle" , (void*)EllLinkerMemoryPool.pool , EllLinkerMemoryPool.looper ) ;
	
	return (int)ell ;
	
}

int EllGetSymbolEntry ( int ell , char* symbol ) {

	//	author : Jelo Wang
	//	get symbol entry
	//	(C)TOK

	
	int address = 0 ;

	EllDynamicPoolSetCurrent ( ell ) ;
	address = EllDynamicPoolGetSymbolEntry ( symbol ) ;

	if ( -1 != address ) { 

		if ( ELL_THUMB16_ROUTINE == EllLinker.routineset )
			address = address + EllLinkerMemoryPool.base + 1 ;
		else if ( ELL_ARM32_ROUTINE == EllLinker.routineset )
			address = address + EllLinkerMemoryPool.base ;
		
		return address ;

	} else return 0 ;

}

void EllUninstall ( int ell ) {

	//	author : Jelo Wang
	//	(C)TOK

	EllDynamicPoolSetCurrent ( ell ) ;
	EllDynamicPoolDestroy () ;
	
}

