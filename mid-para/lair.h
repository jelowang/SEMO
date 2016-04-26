
/*

+	LAIR is an abstractive machine defined by semo , part of SC mid-para

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

# ifndef __LAIR_
# define __LAIR_

# include "sccl.h"

//	Jelo Edited 20120315
#if 0
typedef enum {

	//	author : Jelo Wang
	//	since : 20100722
	//	(C)TOK

	//	lair EXPR atoms
	
	LAIR_FLOW_LOGICAL ,
	LAIR_FLOW_LOOP ,

} LAIR_FLOW_ATOM ;

typedef enum {

	//	author : Jelo Wang
	//	since : 20100722
	//	(C)TOK

	//	lair EXPR atoms

	LAIR_REG_MEMPORT ,	
	LAIR_STATIC_MEMPORT ,
	LAIR_STACK_MEMPORT ,
	LAIR_HEAP_MEMPORT ,

} LAIR_MEMPORT ;


typedef struct {

	//	author : Jelo Wang
	//	since : 20100722
	//	(C)TOK

	//	LAIR Control-flow 
	
	LAIR_FLOW_ATOM type ;
	
	char* label ;
	char* expr ;
		
} LAIR_FLOW ;

typedef struct {

	//	author : Jelo Wang
	//	since : 20100722
	//	(C)TOK

	//	LAIR symbol 
	
	char* name ;
	char* label ;

	LAIR_MEMPORT memport ;
	int based ;
	int size ;	
	
} LAIR_SYMBOL ;

#endif
//	Jelo Edited Finished

typedef enum {

	//	author : Jelo Wang
	//	since : 20121030
	//	(C)TOK

	//	defination of general lair register

	LAIR_REG_0 = 1 << 0x16 ,
	LAIR_REG_1 = 1 << 0x17 ,
	LAIR_REG_2 = 1 << 0x18 ,
	LAIR_REG_3 = 1 << 0x19 ,
	LAIR_REG_4 = 1 << 0x20 ,
	LAIR_REG_5 = 1 << 0x21 ,
	LAIR_REG_6 = 1 << 0x22 ,
	LAIR_REG_7 = 1 << 0x23 ,
	LAIR_REG_8 = 1 << 0x24 ,
	LAIR_REG_9 = 1 << 0x25 ,
	LAIR_REG_10 = 1 << 0x26 ,
	LAIR_REG_11 = 1 << 0x27 ,
	LAIR_REG_12 = 1 << 0x28 ,
	LAIR_REG_13 = 1 << 0x29 ,
	LAIR_REG_14 = 1 << 0x30 ,
	LAIR_REG_15 = 1 << 0x31 ,
	
} LAIR_REG ;

typedef enum {

	//	author : Jelo Wang
	//	since : 20100722
	//	(C)TOK

	//	A procedure , function or method and some thing like that
	LAIR_PROC = 0x0 ,	
	//	An if logic node
	LAIR_IF = 0x1 << 0 ,
	//	A lvalue
	LAIR_LVA_DELT = 0x1 << 1 ,
	//	A rvalue but you know it already
	LAIR_RVA_DELT = 0x1 << 2 ,
	//	A rvalue of memory mode
	LAIR_LVA_MEM = 0x1 << 3 ,
	//	A lvalue of memory mode
	LAIR_RVA_MEM = 0x1 << 4 ,
	//	A lvalue of register mode
	LAIR_LVA_REG = 0x1 << 5 ,
	//	a rvalue of register mode
	LAIR_RVA_REG = 0x1 << 6 ,	
	//	A function call
	LAIR_P_CALL = 0x1 << 7 ,	
	//	just change row
	LAIR_CR = 0x1 << 8 ,
	//	symbol defination
	LAIR_DEF = 0x1 << 9 ,
	
} LAIR_ATOM ;

typedef enum {
	
	//	author : Jelo Wang
	//	since : 20100722
	//	(C)TOK

	//	mode of lair memory frame

	LAIR_MF_STACK , 
	LAIR_MF_HEEP , 
	
} LAIRMEMROYFRAME;

typedef struct {

	//	author : Jelo Wang
	//	since : 20100107
	//	(C)TOK

	//	LAIR-CALL-FRAME
	//	call-frame is a manager for the parameter transfering of function-calling 

	//	type of frame , value bettwen LAIR_REG_CFRAME , LAIR_STK_CFRAME
	int type ;

	//	a pointer to an azonal name
	void* handle ;
	
	char* frame ;

	struct {

		int top ;

	} stack ;
	
} LAIRMemoryFrame ;

typedef struct LAIRA {

	//	author : Jelo Wang
	//	since : 20100505
	//	(C)TOK

	//	a LAIR flow consits two part
	//	oprand1 op oprand2
	//	and a label if necessary

	//	low 0 - 15 for lair type
	//	low 16 - 31 for lair register
	int mode ;
	
	//	a handle to another datas
	int handle ;
	//	scope of lair atom
	int scope ;
	//	line position
	int line ;
	//	totall of lair-procedure
	int proctotall ;
	int havelabel ;
	//	number of the lair atom in the whole chain
	int number ;
	int length ;
	//	lair codes length in bytes
	int colen ;
	//	splited by regoc
	int splited ;
	
	struct LAIRA* head ;
	struct LAIRA* next ;

	SCClString code ;

	//	reference chain
	SCClList refchain ;

} LAIR ;

typedef struct LMON {

	//	author : Jelo Wang
	//	since : 20100505
	//	(C)TOK
	
	LAIR* lair ;
	struct LMON* head ; 
	struct LMON* next ; 

} LABELMOI ;

# endif

extern int LairCreate () ;
extern void LairSetContext ( int context ) ;
extern LAIR* LairGetCodeEx ( int number ) ;
extern int LairAddCode ( char* string , LAIR_ATOM type , int scope ) ;
extern void LairAddCodeLabel ( int number , char* label ) ;
extern char* LairGetCode () ;
extern void LairAllocRegister ( int degreesmax ) ;
extern void LairClearCode () ;
extern void LairClearCodeEx ( LAIR* walker ) ;

extern int LairLabelMoiNew () ;
extern void LairLabelMoiSetContext ( int monitor ) ;
extern void LairLabelMoiAdd ( int lair ) ;
extern void LairLabelMoiSetLabel ( char* label ) ;
extern void LairLabelMoiClear () ;
 
