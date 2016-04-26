
/*

+	JAVA Code Generator based on LGNOSIA , part of SC mid-para

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


# include "semo.h"
# include "schal.h"
# include "sccl.h"
# include "c-grammar.h"
# include "symbole.h"
# include "evalor.h"
# include "lgnosia.h"
# include "java-gentor.h"

static SCClStack LabelStack = { 0 , 0 , 0 , 0 , 0 } ;
static SCClString jgentor_results = { 0 } ;
static JAVA_GENTOR jgentor = { -1 , 0 , 0 , 0 , 0 , 0 } ;

# define JGENTOR_STATUS_RESET()\
	jgentor.form = -1;\

# define GET_JGENTOR_SCOPE() SCClStackGet( &jgentor.scopestack )

# define SET_JGENTOR_SCOPE(sscope)\
	SCClStackPush ( &jgentor.scopestack , (void* )sscope ) ;\

# define UNSET_JGENTOR_SCOPE()\
	SCClStackPop ( &jgentor.scopestack ) ;\

# define PUSH_JGENTOR_LGA(lga)\
	SCClStackPush ( &jgentor.lgastack  , (void* )lga ) ;\
	
# define GET_JGENTOR_LGA() SCClStackGet( &jgentor.lgastack )

# define POP_JGENTOR_LGA()\
	SCClStackPop ( &jgentor.lgastack ) ;\

char* jgentor_gen_operator ( int operator ) {

	//	author : Jelo Wang
	//	since : 20100428
	//	(C)TOK

	switch ( operator ) {

		case C_MUL : return "*" ;
		case C_DIV : return "/" ;
		case C_MOD : return "%" ;

		case C_ADD : return "+" ;
		case C_SUB : return "-" ;

		case C_SHL : return "<<" ;
		case C_SHR : return ">>" ;

		case C_GE :	return ">=" ;
		case C_GT :	return ">" ;
		case C_LT :	return "<" ;
		case C_LE :	return "<=" ;

		case C_EQ :	return "==" ;
		case C_NE :	return "!=" ;

		case C_YHH : return "^" ;

		case C_HE :	return "&" ;

		case C_HU :	return "|" ;

		case C_AND : return "&&" ;

		case C_OR :	return "||" ;

	}

}

static void JIdentorPush () {

	//	author : Jelo Wang
	//	since : 20100130
	//	(C)TOK

	SCClStackPush ( &jgentor.identor , (void* )1 ) ;
	
}

static void JIdentorPop () {

	//	author : Jelo Wang
	//	since : 20100130
	//	(C)TOK

	SCClStackPop ( &jgentor.identor ) ;
	
}


static char* jgentor_get_identor () {

	//	author : Jelo Wang
	//	since : 20100130
	//	(C)TOK

	//	generate a label
	
	int walker = 0 ;
	char* scope = 0 ;
	
	if ( 0 == jgentor.identor.deep ) return 0 ;
	
	scope = (char* ) SCMalloc ( jgentor.identor.deep + 1 ) ;

	for ( walker = 0 ; walker < jgentor.identor.deep ; walker ++ )
		scope [ walker ] = '\t' ;
	
	scope [ walker ] = '\0' ;

	return scope ;

}

void Jgentor_ready () {
	
	//	author : Jelo Wang
	//	since : 20100107
	//	(C)TOK
	
	jgentor.form =-1 ;
	jgentor.stop = 0  ;
	jgentor.label = 0 ;
	jgentor.tree = 0 ;
	jgentor.lgnosia = 0 ;
	
	SCClStackInit ( &jgentor.identor ) ;

	//	get LGA tree
	jgentor.tree = (SCClList* ) LgnosiaGet () ;
	
	if ( jgentor.tree ) 
		jgentor.lgnosia = jgentor.tree->element ;

} 

void jgentor_genv () {
	
	//	author : Jelo Wang
	//	since : 20100107
	//	(C)TOK
	
	if ( !jgentor.tree ) {
		jgentor.form = -1 ;
		return ;
	}

	if ( !jgentor.stop ) {
		
		if ( !jgentor.lgnosia ) return ;
		
		jgentor.form = ((LGNOSIA*)(jgentor.lgnosia))->type ;
		
	}
	
}

void jgentor_next () {
	
	//	author : Jelo Wang
	//	since : 20100107
	//	(C)TOK

	if ( jgentor.tree ) jgentor.tree = jgentor.tree->next ;
	
	if ( !jgentor.tree ) {
		jgentor.stop = 1 ; 
		return ;
	}

	jgentor.lgnosia = jgentor.tree->element ;

}

static void jgentor_classdef_begin ( char* name ) {

	//	author : Jelo Wang
	//	since : 20110406
	//	(C)TOK

	char* classn = sc_substr_with_atom ( name , '.' ) ;

	SCClStringAddStr ( &jgentor_results , "public class " ) ;
	SCClStringAddStr ( &jgentor_results , classn ) ;
	SCClStringAddStr ( &jgentor_results , "\r\n{\r\n" ) ;
	JIdentorPush () ;

	SCFree ( classn ) ;
	
}

static void jgentor_classdef_end () {

	//	author : Jelo Wang
	//	since : 20110406
	//	(C)TOK
	
	SCClStringAddStr ( &jgentor_results , "}\r\n" ) ;	
	JIdentorPop () ;
	
}

static void jgentor_classfield () {

	//	author : Jelo Wang
	//	since : 20110406
	//	(C)TOK

	AZONAL* anllooper = symbole->variable.head ;

	if ( anllooper ) {

		JIdentorPush () ;
		for ( ; anllooper  ; anllooper = anllooper->next ) {
			
			char* S = 0 ;
			
			if ( C_STATIC_BIT & anllooper->datatype ) S = sc_strcat ( jgentor_get_identor () , "private " ) ;
			else S = sc_strcat ( jgentor_get_identor () , "public " ) ;
			SCClStringAddStr ( &jgentor_results , S ) ;			
			SCFree ( S ) ;
			
			if ( C_CHAR_BIT & anllooper->datatype )  SCClStringAddStr ( &jgentor_results , "char " ) ;	
			else if ( C_INT_BIT & anllooper->datatype ) SCClStringAddStr ( &jgentor_results , "int " ) ;	

			SCClStringAddStr ( &jgentor_results , anllooper->name ) ;
			SCClStringAddStr ( &jgentor_results , " ;\r\n" ) ;	
			SCFree ( S ) ;
			
		}
		JIdentorPop () ;
		
	}

	
}

char* gentor_java_run ( char* file ) {

	//	author : Jelo Wang
	//	since : 20100107
	//	(C)TOK
		
	SCClStringInit ( &jgentor_results ) ;
	
	jgentor_classdef_begin ( file ) ;
	jgentor_classfield ( file ) ;
	
	while ( !jgentor.stop ) {
		
		jgentor_genv () ;

		switch ( jgentor.form ) {
			
			case LGNOSIA_TOP_IDENT :
			break ;
			
		}

		LgnosiaDestroy ( jgentor.lgnosia ) ;		
		jgentor_next () ;

	}

	jgentor_classdef_end () ;

	{
		void* fo = SCHalFileOpen ( file , "wb+" ) ;
		SCHalFileWrite ( fo , jgentor_results.data , 1 , jgentor_results.add_walker ) ;
		SCHalFileClose ( fo ) ;
	}

	SCClStringDestroy ( &jgentor_results ) ;

	return 0 ;

}
	
