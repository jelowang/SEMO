
/*

+	Register Allocation , part of SC mid-para

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

# ifndef __REGOC
# define __REGOC

# define LIVE_NAME_LENGTH 32

typedef struct {

	//	author : Jelo Wang
	//	since : 20110107
	//	(C)TOK

	//	register name 
	char live [ LIVE_NAME_LENGTH ] ;

	//	a handle to LAIR
	void* lair ;

	//	number of live scope
	int number ; 
	
	int scope ;

	//	for splited LIVE who neednt register
	int splited ;
	
	//	start position of a register's live scope
	int start_line ;
	//	end position of a register's live scope	
	int end_line ;
	
} LIVESCOPE ;

# endif

extern void RegocReady () ;
extern void RegocRelease () ;
extern int RegocRegPoolCreate ( char** regs , int totall ) ;
extern void RegocBuildRefGraph ( char* code , int position ) ;
extern void RegocLiveScopeMoiCreate () ;
extern void RegocLiveScopeMoiDestroy () ;
extern int RegocLiveScopeGetLAIR () ;
extern int RegocLiveScopeAdd ( char* live , int scope , int line , void* lair ) ; 
extern int RegocCheckLiveScope ( char* live , int scope , int line ) ;
extern int RegocIGraphCreate () ;
extern void RegocLiveScopeClear () ;
extern int RegocGetRegister ( int pn , int lsn ) ;
extern void RegocRegisterReady ( int proctotall ) ;
extern void RegocRegisterAlloc ( int graphp , int lstotall ) ;
extern void RegocAllocDestory () ;

