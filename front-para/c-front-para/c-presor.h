
/*

+	PRESOR , part of SC c-front-para

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

# ifndef __C_PRESOR_
# define __C_PRESOR_

# include "sccl.h"

enum {
	
	//	author : Jelo Wang
	//	notes : there are only two macros type in c language , function or constant
	//	since : 20090809

	MACRO_FUNC ,
	MACRO_CONST ,

} MACRO_TYPE ;

typedef struct c_macro {

	//	author : Jelo Wang
	//	notes : MACRO 
	//	since : 20090809
	
	//	(1) name of macro
	//	(2) file name that the macro belong to .
	//	(3) macro body saved here , we used SCClString to handle it .
	//	(4) type of macro : MACRO_FUNC or MACRO_CONST
	//	(5) params of macro are saved here
	
	char* name ;
	char* file ;
	int line ;
	SCClString body ;

	unsigned short type ;

	struct {
		
		//	params of macro would saved by the follow format :
		//	xxx xxx xxx xx , the gaps between param's name is a blank

		int totall ;
		
		SCClString token ;

	} param ;

	struct c_macro* front ;	
	struct c_macro* next ;

} MACRO ; 

# endif

extern int presor_c_run ( char* ) ;

