
/*

+	LEXER , part of SC c-front-para

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

# ifndef __C_LEXER_
# define __C_LEXER_

# include "sccl.h"

# define LEXERC_HEADBIT_RESET() lexc->headbit = 0 ;
# define lexerc_clearv() lexc->v = 0 ;

enum {

	//	author : Jelo Wang
	//	(C)TOK

	//	normal mode without specical features
	LEXERC_DEFAULT_MODE = 0x1	 ,
	//	normal mode with specical features , like blank fliter
	LEXERC_FLITER_MODE = 0x2 ,
	//	define-head bitmap generation process
	LEXERC_HEADBIT_MODE = 0x4
 	

} LEXERC_MODE ;

enum {

	//	author : Jelo Wang
	//	notes : bracket type , smart , mid , strong
	//	since : 20100414
	//	(C)TOK

	LEXC_SMART_BRACKET ,
	LEXC_MID_BRACKET ,
	LEXC_STRONG_BRACKET 
	
} BRACKET_TYPE ;


typedef struct {
	
	//	author : Jelo Wang
	//	notes : key words of c language
	//	since : 2008
	//	(C)TOK

	char* name ;
	int   type ;

} C_KEYWORDS ;


typedef struct {
	
	//	author : Jelo Wang
	//	notes : CODE
	//	since : 20090811
	//	(C)TOK
	
	//	(1) : lexical value 
	//	(2) : previous lexical value

	char* token ;
	char* file ;
	char c ;
	char pc ;
	
	int v ;
	int pv ;
	int ppv ;
	int line ;
	int stop ;
	int mode ;
	// if recursive occured , deep + 1
	int deep ;	
	int headbit ;
	
	//	ignore odd symbols
	int ios ;
	int stack ; 

	//	as for a C_P_VAR_REF , sacle = *****... totall of '*'
	int scale ;
	
	SCClStack* scstack ;
	SCClString* code ;


} LEXERC ;


# endif


extern LEXERC* lexc ;

extern char* lexerc_get_operator ( int operator ) ;
extern void lexerc_setmode ( int mode ) ;
extern void lexerc_ready () ;
extern int lexerc_overflowed () ;
extern int lexerc_next () ;
extern unsigned char lexerc_get_atom () ;
extern void lexerc_put_atom ( unsigned char atom ) ; 
extern void lexerc_backup () ;
extern void lexerc_rollback () ;
extern void lexerc_setback ( int step ) ;
extern void lexerc_skip_blank () ;
extern void lexerc_skip_space () ;
extern void lexerc_jump ( int step ) ;
extern int lexerc_look ( int step ) ;
extern int lexerc_genv () ;
extern LEXERC* lexerc_new ( unsigned char* data , int mmode ) ;
extern void lexerc_set ( LEXERC* lexer ) ;
extern void lexerc_clear_status () ;
extern void lexerc_set_file ( char* name ) ;
extern void lexerc_destroy () ;
extern LEXERC* lexerc_get () ;
extern void lexerc_clear_headbit () ;
extern int lexerc_head_genv ( int border ) ;
extern int lexerc_expect ( int atom ) ;

