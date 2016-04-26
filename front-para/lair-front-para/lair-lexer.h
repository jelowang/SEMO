
/*

+	LAIR's codes lex , part of SC mid-para

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

# ifndef __LAIR_LEXER_
# define __LAIR_LEXER_

# include "sccl.h"

# define LexerLair_clearv() lexc->v = 0 ;

enum {

	//	author : Jelo Wang
	//	since : 20100503
	//	(C)TOK

	//	normal mode without specical features
	LEXLAIR_DEFAULT_MODE = 0x1	 ,
	//	normal mode with specical features , like blank fliter
	LEXLAIR_FLITER_MODE = 0x2 ,
	//	define-head bitmap generation process
	LEXLAIR_HEADBIT_MODE = 0x4
 	

} LAIR_LEXER_MODE ;

enum {

	//	author : Jelo Wang
	//	notes : bracket type , smart , mid , strong
	//	since : 20100503
	//	(C)TOK

	LEXLAIR_SMART_BRACKET ,
	LEXLAIR_MID_BRACKET ,
	LEXLAIR_STRONG_BRACKET 
	
} LAIR_BRACKET_TYPE ;

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
	int deep ;	// if recursive occured , deep + 1
	int headbit ;

	//	live scope number
	int lsn ;
	
	//	ignore odd symbols
	int ios ;
	int stack ; 

	//	lair defination form STACK n 
	//	scale = n
	int scale ;
	
	SCClStack* scstack ;
	SCClString* code ;


} LEXERLAIR ;


# endif


extern LEXERLAIR* lexlair ;

extern void LexerLairSetmode ( int mode ) ;
extern void LexerLairReady () ;
extern int LexerLairOverflowed () ;
extern int LexerLairNext () ;
extern unsigned char LexerLairGetAtom () ;
extern void LexerLairPutAtom ( unsigned char atom ) ; 
extern void LexerLairRollback () ;
extern void LexerLairSetback ( int step ) ;
extern void LexerLairSkipBlank () ;
extern void LexerLairSkipSpace () ;
extern void LexerLairJump ( int step ) ;
extern int LexerLairLook ( int step ) ;
extern int LexerLairGenv () ;
extern LEXERLAIR* LexerLairNew ( unsigned char* data , int mmode ) ;
extern void LexerLairSet ( LEXERLAIR* lexer ) ;
extern void LexerLairClearStatus () ;
extern void LexerLairSetFile ( char* name ) ;
extern void LexerLairDestroy () ;
extern LEXERLAIR* LexerLairGet () ;
extern void LexerLairClearScale () ;
extern int LexerLairHeadGenv ( int border ) ;
extern int LexerLairExpect ( int atom ) ;

