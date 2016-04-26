
/*

+	Semo Compiler 0.3.0

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

# ifndef __SEMO
# define __SEMO

# define SEMO_DEBUG 

enum {

	//	author : Jelo Wang
	//	since : 20100817
	//	(C)TOK

	//	notes : parameters of the framework

	//	compile for c language based on C99 
	SC_C99 = 0x1 ,
	//	compile for c plus plus language
	SC_CPP = 0x2 ,
	//	compile for java language	
	SC_JAVA = 0x4 ,
	//	export pre-compiling results to external file
	SC_PO = 0x8 ,
	//	export IR to external file
	SC_LAIR = 0x10,
	//	export semo assembly code to external file
	SC_SASM = 0x20 ,
	//	enable compiling-render and export results
	SC_CR = 0x40 ,
	//	generate object-file with ELF format by default
	SC_ELF = 0x80 ,
	//	compile and link
	SC_LINK = 0x100 ,
	//	compile only by default
	SC_NLINK = 0x200 ,
	//	lga lgnosia graph	
	SC_LGA = 0x400 ,
	//	exp expression graph
	SC_EXP = 0x800 ,
	//	delete comments	
	SC_DC = 0x1000 ,
	//	marco substitution
	SC_MS = 0x2000 ,	
	//	compile for the ARM architecture by default
	SC_ARM = 0x4000 ,
	//	compile for the X86 architecture
	SC_X86 = 0x8000 ,
	//	both dc and ms features by default , = 2^17
	SC_DM = 0x10000 ,				
	//	interference graph of live scope , = 2^18
	SC_IG = 0x20000	,
	//	__stdcall , = 2^19
	SC_C0 = 0x40000 ,
	//	__cdecll ,= 2^20
	SC_C1 = 0x80000 ,
	//	__fastcall , = 2^21
	SC_C2 = 0x100000 ,
	//	__armcall , = 2^22
	SC_C3 = 0x200000 ,
	//	convert C99 codes to JAVA codes , = 2^23
	SC_C2J = 0x400000
	
} ;

enum {

	//	author : Jelo Wang
	//	since : 20091127
	//	(C)TOK

	//	An binary IMAGE that can runing straightly
	SC_DYNAMIC_COMPILING ,
	//	An object file or something like that fashion
	SC_STATIC_COMPILING ,
		
} ;

typedef struct COMPILER_NODE {

	//	author : Jelo Wang
	//	notes : initialize compiler parameters
	//	since : 20090809
	//	(C)TOK

	//	input files list , a handle to SCCllist*
	void* il  ;
	//	object files list , a handle to SCCllist*	
	void* ol ;
	
	int parameter ;	
	//	register allocation costs
	int regoccosts ;
	//	lives spliting times
	int lssplits ;	
	
	int lines ;
	int codes ;

	//	preprocessor of code streams
	int (*PRESOR)( unsigned char* ) ;
	//	syntax & semantic analyzer
	int (*PARSER)( int* ) ;
	//	IR generator
	char* (*GENTOR)( char* ) ;
	//	assembly codes generator
	char* (*ASMOR)( char* , char* ) ;
	//	assembler
	void (*ASSEMER)( char* , char* , int* ) ;
	//	object's linker
	void (*LINKER ) ( void* ) ;
	//	release the resources that occupied by semo
	void (*RELEASE)() ;

	//	the begining time of compling
	long stime ; 
	//	the ending time of compling
	long etime ; 
	
} COMPILER ;

extern COMPILER* semo ;

# endif

extern int SCCompile ( int argc , char** argv , int type ) ;

