
/*

+	JVM assembler , part of SC jvm-back-para

+	'Semo Compiler' is a multi-objective compiler which is developing under the terms of the 
+	GNU general public license as published by the Free Software Foundation.
+	The project lunched by Jelo Wang since 2.Feb.2008 from 'Techniques of Knowledge' community. 

+	You can redistribute it and/or modify it under the terms of the gnu general public version 3 of 
+	the license as published by the free software foundation.this program is distributed in the hope 
+	that it will be useful,but WITHOUT ANY WARRANTY.without even the implied warranty of merchantability 
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


# ifndef ASSEMER_JVM
# define ASSEMER_JVM

typedef enum {

	//	author : Jelo Wang
	//	since : 20110330
	//	(C) TOK

	JVM_SET_AALOAD				= 0x32 ,
	JVM_STE_AASTORE				= 0x53 ,
	JVM_SET_ALOAD				= 0x19 ,
	JVM_SET_ALOAD0				= 0x2a ,
	JVM_SET_ALOAD1				= 0x2b ,
	JVM_SET_ALOAD2				= 0x2c ,
	JVM_SET_ALOAD3				= 0x2d ,
	JVM_SET_ACONST_NULL 		= 0x1 ,
	JVM_SET_ARETURN				= 0xb0 ,
	JVM_SET_ICONSTm1			= 0x2 ,
	JVM_SET_ICONST0				= 0x3 ,
	JVM_SET_ICONST1				= 0x4 ,
	JVM_SET_ICONST2				= 0x5 ,
	JVM_SET_ICONST3				= 0x6 ,
	JVM_SET_ICONST4				= 0x7 ,
	JVM_SET_ICONST5				= 0x8 ,
	JVM_SET_IDIV				= 0x6c ,
	
	//	the bellow sets are reserved by JVM
	//	JVM_SET_IMPDEP1 		= 0xCA ,
	//	JVM_SET_IMPDEP2 		= 0xFE ,
	//	JVM_SET_BREAKPOINT 		= 0xFF ,

} JVMSET ;

typedef struct {
	
	//	author : Jelo Wang
	//	since : 20100723
	//	(c)TOK

	char* name ;
	int	opcode ;

} JVM_SETMAP ;

# endif

