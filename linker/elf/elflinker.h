
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

# ifndef __ELLLINKER
# define __ELLLINKER

typedef enum {

	//	author : Jelo Wang
	//	since : 20100625
	//	(C)TOK

	ELL_LOCAL_LINKER ,
	ELL_GLOBAL_LINKER ,
	
} EllDynLinkerStatus ;

typedef struct {

	//	author : Jelo Wang
	//	since : 20100625
	//	(C)TOK

	int routineset ;
	int obidborder ;
	int status ;

} ELLLINKER ;

typedef struct {

	//	author : Jelo Wang
	//	since : 20100623
	//	(C)TOK
	
	unsigned char* pool ;
	int base ;
	int looper ;
	int length ;

} ELLLINKERMEMORYPOOL;

extern ELLLINKER EllLinker ;
extern ELLLINKERMEMORYPOOL EllLinkerMemoryPool ;

extern int EllLocalLinker ( int obid , int file ) ;
extern int EllGloabalLinker ( int obidborder ) ;

static int EllReloc ( Elf32_Rel* reloctab , int (*EllRelocKernal) (Elf32_Rel*,Elf32_Sym*, int) , int looper , int lborder , int obid ) ;
static int EllTextReloc ( Elf32_Rel* elf32_rel , Elf32_Sym* elf32_sym , int obid ) ;
static int EllDataReloc ( Elf32_Rel* elf32_rel , Elf32_Sym* elf32_sym , int obid ) ;

# endif

