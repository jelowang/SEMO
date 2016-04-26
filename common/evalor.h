
/*

+	Constant Evaluator of SC c-front-para

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

# ifndef __EVALOR_
# define __EVALOR_

# include "sccl.h"

//# define __EVALOR_DEBUG

typedef struct {

	//	author : Jelo Wang
	//	since : 20100427
	//	(C)TOK

	//	EXPR evalutor

	//	(1) suffix  
	//	(2) operator stack
	//	(3) EXPR tree
	
	SCClList suffix ;
	SCClStack operator ;
	int tree ;

} EVALOR ;

# endif

extern long int c_evalor () ;
extern EVALOR* EvalorCreate () ;
extern void EvalorSetCurrent ( EVALOR* evalor );
extern void EvalorUnsetCurrent () ;
extern void EvalorOperandPush ( int operand ) ;
extern void EvalorOperatorPush (int operator ) ;
extern void EvalorFinish () ;
extern void EvalorClear () ;
extern int EvalorGetTree () ;

