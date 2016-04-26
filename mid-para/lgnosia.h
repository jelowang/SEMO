
/*

+	LGNOSIA abstractive grammar, part of SC mid-para

+	Semo C()mpiler is a free software created by (c)Techniques of Knowledge since 20080202.
+	(C)TOK Jelo Wang

+	You can redistribute it and/or modify it under the terms of the gnu general public license	
+	as published by the free software foundation, either version 3 of the license or any later 	
+	version.this program is distributed in the hope that it will be useful,but without any 		
+	warranty.without even the implied warranty of merchantability or fitness for a particular 	
+	purpose.																					
																													
+	(c)	Techniques of Knowledge
+		an open source group since 2008
+		page : http://www.tok.cc 
+		email : jelo.wang@gmail.com

*/

# ifndef __LGNOSIA
# define __LGNOSIA

# include "sccl.h"

typedef enum {

	//	author : Jelo Wang
	//	since : 20100712
	//	(C)TOK

	//	notes : type of LGNOSIA node

	//	a data symbol like var azonal
	LGNOSIA_SYM_IDENT ,
	//	control point
	LGNOSIA_CP_IDENT ,
	//	EXPR
	LGNOSIA_EXP_IDENT ,
	//	positive chain of control point
	LGNOSIA_POC_IDENT ,
	//	negative chain of control point
	LGNOSIA_NEC_IDENT ,
	//	a function , class or something like this
	//	idedicates the top node of LGA
	LGNOSIA_TOP_IDENT ,

} LGNOSIA_IDENT ;

typedef struct LGNOSIAN {

	//	author : Jelo Wang
	//	since : 20100712
	//	(C)TOK	

	//	notes : LGA
		
	LGNOSIA_IDENT type ;

	int azonal ;
	int deep ;
	int father ;

	int allinyer ;
	int x ;
	int y ;

	SCClList parameter ;
	SCClList context ;
	
	struct LGNOSIAN* po_chain ;
	struct LGNOSIAN* ne_chain ;
	
} LGNOSIA ;

# endif

extern int LgnosiaNew ( int azonal , LGNOSIA_IDENT type ) ;
extern void LgnosiaAddContext ( int lgnosia , int handle , LGNOSIA_IDENT type ) ;
extern void LgnosiaAddParameter ( int lgnosia , int handle , LGNOSIA_IDENT type ) ;
extern int LgnosiaFindContext ( int flgnosia , int clgnosia ) ;
extern int LgnosiaFindContextEx ( int flgnosia , int clgnosia ) ;
extern int LgnosiaDFSNormalize( int lga , int flga , int deep ) ;
extern int LgnosiaGenPOC ( int lgnosia , int azonal ) ;
extern int LgnosiaGenNEC ( int lgnosia , int azonal ) ;
extern int LgnosiaGet () ;
extern void LgnosiaDestroy ( int handle ) ;
extern void LgnosiaStackPush ( int handle ) ;
extern int LgnosiaStackPop () ;
extern int LgnosiaStackTop () ;
extern void LgnosiaQueueEnter ( int lgnosia ) ;
extern int LgnosiaQueueOut () ;
extern void LgnosiaQueueDestroy () ;

