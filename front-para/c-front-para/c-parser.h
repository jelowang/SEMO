
/*

+	PARSER , part of SC front-para

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

# ifndef __C_PARSER_
# define __C_PARSER_

# include "sccl.h"

enum {

	//	author : Jelo Wang
	//	since : 20100418
	//	(C)TOK
	
	PARSERC_SCOPE_PARAM = 1 ,
	//	scope of symbol evaluation
	PARSERC_SCOPE_EVA ,
		
} PARSERC_SCOPE ;

typedef struct {

	//	author : Jelo Wang
	//	since : 20091128
	//	(C)TOK
	
	short int head ;
	short int form ;
	short int stop ;
	short int scope ;
	short int stack ;
	SCClStack scopestack ;

} PARSERC ;

# endif

static void read_define_head_flow () ;
static void parserc_ready () ;
static int parserc_genv () ;
static int parser_c_read_function () ;
static int parser_c_read_if () ;
static int parser_c_read_else () ;
static int parser_c_read_while () ;
static int parser_c_read_controlflow () ;
static int parser_c_is_controlflow ( int head ) ;
static int parser_c_read_symbol_inf () ;
static int parser_c_read_variable_def () ;
static int parser_c_read_variable_inf () ;
static int parser_c_read_symbol_def () ;
static int parser_c_read_intnum_inf () ;
static int parser_c_read_funccal () ;
static int parser_c_read_expr () ;
static void parser_c_read_poroperator ( int side , int evalor_enable ) ;
extern void parser_c_skip_parenthesis_scope () ;
int parser_c_run ( int * line ) ;

