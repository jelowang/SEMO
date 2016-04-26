
/*

+	error mssage & mssage manager of SC

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

# include <stdarg.h> 
# include "c-errors.h"

extern int strlen( char* ) ;
extern int strcpy( char* , char* ) ;
extern void* SCMalloc ( int ) ;

ERRORC errorc ;
ERRORC* errorc_walker = &errorc;

unsigned short ERROR_ENABLE = 1;
unsigned short WARNING_ENABLE = 1;

unsigned short C_PRESOR_ERROR_ENABLE = 1 ;
unsigned short C_LEXER_ERROR_ENABLE = 1 ;
unsigned short C_PARSER_ERROR_ENABLE = 1 ;
unsigned short C_SEMAN_ERROR_ENABLE = 1 ;

unsigned short C_PRESOR_WARNING_ENABLE = 1 ;
unsigned short C_LEXER_WARNING_ENABLE = 1 ;
unsigned short C_PARSER_WARNING_ENABLE = 1 ;
unsigned short C_SEMAN_WARNING_ENABLE = 1 ;

int cerror ( int module , int type , const   char* SClog , ... ) {

	//	author : Jelo Wang
	//	notes : printer
	//	since : 20091124
	
	va_list ap ;   
	va_start ( ap , SClog ) ;   
	vprintf ( SClog , ap ) ;
	va_end ( ap ) ;
	
	return 1 ;

}
