
/*

+	SYMBOLE , between SC front-para and mid-para

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

//	author: Jelo Wang
//	since : 20081109
//	(C)TOK

# ifndef __SYMBOLE
# define __SYMBOLE

typedef enum {

	//	author : Jelo Wang
	//	since : 2008
	//	(C)TOK

	//	data type of azonal
	
	AZONAL_INT ,
	AZONAL_FLOAT ,
	
} ;

typedef enum {

	//	author : Jelo Wang
	//	since : 2008
	//	(C)TOK

	ISA_INTEGER ,
	ISA_VARIABLE ,
	ISA_ARRAY ,		
	ISA_FUNCTION ,
	ISA_FUNCCAL ,
	ISA_CONST ,
	ISA_STRUCTHEAD ,	
	ISA_VARSTRUCT ,
	ISA_ARRSTRUCT ,
	ISA_VARUNION , 
	ISA_ARRUNION ,
	ISA_TYPEDEF ,
	ISA_IFCF ,	
	ISA_ELSECF ,
	ISA_ELSEIFCF ,
	ISA_FORCF ,	
	ISA_WHILECF ,	
	ISA_DOWHILECF ,
	ISA_SWITCHCF ,

} AZONALTYPE ;

typedef enum {

	//	author : Jelo Wang
	//	since : 20100423
	//	(C)TOK

	EXP_OPERAND ,
	EXP_OPERATOR 
	
} EXPATOM ;

typedef enum {

	//	author : Jelo Wang
	//	since : 20100502
	//	(C)TOK
	
	EXP_DELT_DEFAULT ,
	EXP_DELT_ANLDATA ,
	EXP_DELT_ANLNUMERIC ,	

} EXPDELTTYPE ;

typedef struct EXPRE {

	//	author : Jelo Wang
	//	since : 20100202
	//	(C)TOK

	//	field (1) type of an EXPR
	//	field (2) linked to an azonal or a functer
	
	char* delt ;

	int delttype ;	
	int type ;
	int handle ; 
	int deep ;

	//	for compiling-render 
	int x ;
	int y ;
			
	struct EXPRE* father ;
	struct EXPRE* left ;
	struct EXPRE* right ;
	struct EXPRE* brother ;

} EXPR ;

typedef struct ANODE {

	//	author : Jelo Wang
	//	since : 20090321	
	
	//	notes : ANL , the meaings of it depends on its scope.Identify an array , a variable or a structed symbol.

	char* name ;
	//	azonaltype , value between ISA_VARIABLE , ISA_ARRAY , ISA_FUNCTION , ISA_EXPR , ISA_CONTROLFLOW , ISA_NUMERAL
	int azonaltype ;		
	//	datatype , data type of an AZONAL
	int datatype ;	
	//	which symbol is belong to , symbol between function or structed symbol. 
	int belong ;		
	//	defination scope
	int scope ;
	//	a handle to LGA
	//	representing  the AZONAL which field is belong to
	int lgabelong ;
	int used ;
	int line ;
	int isparam ;			
	int number ;		

	// if azonaltype is ISA_ARRAY , layer = the totall '{' of array
	// if azonaltype is ISA_FUNCTION , layer = the totall ANLDATA in scope
	int layer ;			

	//	totall length in bytes
	//	for a function this is the aoznal totall length
	int size ;

	int scale ;
	
	//	tack , SC use tack to handling additional information about a ANL , like a function which parameters are handled here
	SCClList tack ;
	
	struct ANODE* head ;
	struct ANODE* next ;

} AZONAL ;

typedef struct SNODE {

	//	author : Jelo Wang
	//	notes : symbol table

	AZONAL constant ;	
	AZONAL variable ;
	AZONAL array ;
	AZONAL object ;
	AZONAL function ;	
	AZONAL funccal ;
	AZONAL controlflow ;
	
	int vartotall ;
	int arrtotall ;
	int objecttotall ;
	int functotall ;
	int exptotall ;
	int logicalblocks ;

} SYMBOLE ;

extern SYMBOLE* symbole ;

# endif


extern int SymboleCreate () ;
extern void SymboleSetCurrent ( int ) ;
extern AZONAL* SymboleAzonalAddVariable ( 

	char* name ,
	int data_type , 
	int azonal_type , 
	int scope ,
	int size , 
	int is_param ,
	int line ,
	int belong ,
	int lgabelong 
	
) ;

extern  int SymboleAddFunctionParameter ( AZONAL* function , AZONAL* azonal ) ;
extern AZONAL* SymboleFindAzonal ( char* name ,int scope ,int belong ) ;
extern AZONAL* SymboleVarAzonalSavable ( char* name ,int scope ,int lgabelong ) ;
extern void SymboleAzonalRefInc ( AZONAL* azonal ) ;
extern AZONAL* SymboleAddFunction ( char* name , int azonaltype , int type , int line ) ;
extern AZONAL* SymboleAddFunctionCall ( char* name , int azonaltype , int tacktotall , int line ) ;
extern AZONAL* SymboleFindFunction ( char* name ) ;
extern int  SymboleAddFunctionParameter ( AZONAL* function , AZONAL* para ) ;
extern void SymboleSetFunctionParameters ( AZONAL* function , int parameters ) ;
extern int SymboleGetFunctionNumber ( char* name ) ;
extern AZONAL* SymboleAddControlAzonal ( int azonaltype , int line ) ; 
extern void SymboleUninstall () ;
extern int SymboleGetCurrentFuncNumber () ;
extern void* SymboleGetCurrentFunc () ; 
extern void* SymboleGetFuncParameter ( void* azonal , char* name ) ;
extern AZONAL* SymboleAndNumeric ( char* name , int type ) ;
extern int SymboleAddFunctionLayer ( AZONAL* function ) ;

