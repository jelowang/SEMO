
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

# include <stdio.h>
# include <ctype.h>
# include "c-lexer.h"
# include "schal.h"
# include "sccl.h"
# include "evalor.h"
# include "c-grammar.h"
# include "symbole.h"

//	author: Jelo Wang
//	since : 20090102
//	in arithmetic EXPR
# define ARITHMETIC_SCOPE  3 
//	in bit EXPR
# define BITAND_SCOPE  4 

SCClStack suffix_stack = { 0 , 0 , 0 } ;
SCClStack evalor_stack = { 0 , 0 , 0 } ;
EVALOR* evalor = 0 ;

# ifdef __EVALOR_DEBUG
static char* evalor_disp_operator ( int operator ) {

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
		return 6 ;

		case C_HU :	return "|" ;

		case C_AND : return "&&" ;

		case C_OR :	return "||" ;

	}
}
# endif

static int is_functer ( int functer ) {

	//	author: Jelo Wang
	//	since : 20091123
	//	(C)TOK


	if ( C_JAA <= functer && functer <= C_SHRR ) 
		return 1 ;
		return 0 ;

}

static int functer_priority ( int functer , int scope ) {

	//	author: Jelo Wang
	//	since : 20081229
	//	(C)TOK

	switch ( functer ) {

		case C_MUL :
		case C_DIV :
		case C_MOD :
		return 13 ;

		case C_ADD :
		case C_SUB :
		return 12 ;

		case C_SHL :
		case C_SHR :
		return 11 ;

		case C_GE :
		case C_GT :
		case C_LT :
		case C_LE :
		return 10 ;

		case C_EQ :
		case C_NE :
		return 9 ;

		case C_YHH :
		return 7 ;

		case C_HE :
		return 6 ;

		case C_HU :
		return 6 ;
		
		case C_AND :
		return 5 ;

		case C_OR :
		return 4 ;
		
		default : return -1 ;

	}

}


static int have_priority ( int priority , int object ) {

	//	author: Jelo Wang
	//	since : 20090102

	//	參數：運算符優先級

	if( priority < object )
		return -1 ;
	if( priority == object )
		return 0 ;
		return 1 ;

}

static long int value_calair ( int op_1 , int op_2 , int functer ) {
	
	//	author: Jelo Wang
	//	since : 20091123
	
	switch ( functer ) {

		case C_SUB : return op_1 - op_2 ;
		case C_ADD : return op_1 + op_2 ;

		default : return 0 ;
		
	}
	
}

static void c_suffix_exp_build ( SCClStack* SCClStack , int value , int scope ) {

	//	author : Jelo Wang
	//	since : 20081229
	//	(C)TOK

	switch( value ) {

		case C_XKL : SCClStackPush ( SCClStack , (void* )C_XKL ) ; break ;

		case C_XKR :
			
			while( !SCClStackEqual ( SCClStack , (void* )C_XKL ) && !SCClStackEmpty ( SCClStack ) ) {
				SCClStackPush ( &suffix_stack , SCClStackPop( SCClStack ) ) ;
			}
			
			SCClStackPop ( SCClStack ) ;
			
		break ;
		
		default :

			if( SCClStackEmpty ( SCClStack ) ) {

				SCClStackPush ( SCClStack ,  (void* )value ) ;
				
			} else if( -1 == have_priority ( functer_priority ( value , scope ) , functer_priority ( SCClStackGet ( SCClStack ) , scope ) ) ) {
				
				while( !SCClStackEmpty ( SCClStack ) ) {
					
					int priority = have_priority (functer_priority ( value , scope ) , functer_priority ( SCClStackGet ( SCClStack ) , scope ) ) ;
					
					if( SCClStackEqual ( SCClStack , (void* )C_XKL ) ) {

						SCClStackPop ( SCClStack ) ;
						
						break ;

					} else if ( -1 == priority ) {
						SCClStackPush ( &suffix_stack , SCClStackPop( SCClStack ) ) ;
					} else if( 1 != priority ) {

						SCClStackPop ( SCClStack ) ;
						
						break ;
					}
					
				}
				
				SCClStackPush ( SCClStack ,  (void* )value ) ;
			 
			} else
				SCClStackPush ( SCClStack ,  (void* )value ) ;

	}

}


static void suffix_exp_done ( SCClStack* SCClStack ) {

	//author: Jelo Wang
	//since : 20090102

	while ( !SCClStackEmpty ( SCClStack ) )

		if ( !SCClStackEqual ( SCClStack , (void* )C_XKL ) )  {

			SCClStackPush ( &suffix_stack , SCClStackPop( SCClStack ) ) ;

		} else {

			SCClStackPop ( SCClStack )  ;
		
		}
		
}

static void c_gen_suffix_exp () {


	//	author : Jelo Wang
	//	since : 20091123
	//	(C)TOK

	SCClStack* ieval = (SCClStack* ) SCMalloc ( sizeof(SCClStack) ) ;
	
	if ( 0 == ieval ) {
		return ;
	}

	SCClStackInit ( ieval ) ;
	SCClStackInit ( &suffix_stack ) ;
	
	while ( !lexc -> stop ) {
		
		lexerc_genv() ;

//		if ( lexc -> v == C_ENTER  && lexc -> pv != C_ESCAPE )
//			break;
//		if ( lexc -> v == C_CHROW  && lexc -> pv != C_ESCAPE )
//			break;

		if ( lexc->v == C_ENTER  )
			break;

		if ( is_functer  ( lexc -> v ) )
			c_suffix_exp_build ( ieval , lexc -> v , ARITHMETIC_SCOPE ) ;
		else {
			SCClStackPush ( &suffix_stack , (void* )SCClAtoi ( lexc -> token ) ) ;
		}
	}

	suffix_exp_done ( ieval ) ;

}

static long int c_suffix_exp_evalor () {

	//	author : Jelo Wang
	//	since : 20091123
	//	(C)TOK

	int results = 0;
	
//	for ( ; !SCClStackEmpty ( &suffix_stack ) ;) {

		int functer = (int )SCClStackPop ( &suffix_stack ) ;

		int op_2 = (int )SCClStackPop ( &suffix_stack ) ;
		int op_1 = (int )SCClStackPop ( &suffix_stack ) ;

		SCClStackPush ( &suffix_stack , (void* )value_calair ( op_1 , op_2 , functer ) ) ;
							
//	}

	results = (int )SCClStackPop ( &suffix_stack ) ;

	return results ;


	
}

long int c_evalor () {

	//	author : Jelo Wang
	//	since : 20091123
	//	(C)TOK

	while ( C_ENTER != lexc->v ) {
		
		lexerc_genv () ;	

		SClog ( "lexical value %x\n" , lexc->v ) ;
		
	}
	
	return 1 ;


}

EVALOR* EvalorCreate () {
	
	//	author : Jelo Wang
	//	since : 20100427
	//	(C)TOK

	EVALOR* evalor = (EVALOR* ) SCMalloc (sizeof(EVALOR)) ;

	evalor->suffix.element = 0 ;
	evalor->suffix.head = 0 ;
	evalor->suffix.next = 0 ;

	SCClStackInit ( &evalor->operator ) ;

	evalor->tree = 0 ;
	
	return evalor ;
	
}

void EvalorSetCurrent ( EVALOR* eva ) {

	//	author : Jelo Wang
	//	since : 20100427
	//	(C)TOK

	SCClStackPush ( &evalor_stack , (void* ) evalor ) ;
	evalor = eva ;
	
}

void EvalorUnsetCurrent () {

	//	author : Jelo Wang
	//	since : 20100427
	//	(C)TOK

	evalor = SCClStackPop ( &evalor_stack ) ;
	
}

void EvalorOperandPush ( int operand ) {
	
	//	author : Jelo Wang
	//	since : 20100427
	//	(C)TOK


	EXPR* expression = (EXPR* ) SCMalloc ( sizeof(EXPR) ) ;

	if ( !expression ) return ;
	if ( !evalor ) return ;

	expression->delt = 0 ;
	expression->type = EXP_OPERAND ;
	expression->delttype = EXP_DELT_DEFAULT ;
	expression->handle = operand ;
	expression->left = 0 ;
	expression->right = 0 ;
	expression->brother = 0 ;
	
	SCClListInsert ( &evalor->suffix , (void* )expression ) ;

	
}

void EvalorOperatorPush ( int operator ) {
	
	//	author : Jelo Wang
	//	since : 20100427
	//	(C)TOK

	EXPR* expression = (EXPR* ) SCMalloc ( sizeof(EXPR) ) ;
	EXPR* stackexp = 0 ;
	EXPR* tree = 0 ;
	EXPR* operand1 = 0 ;
	EXPR* operand2 = 0 ;

	if ( !expression ) return ;
	if ( !evalor ) return ;

	expression->delt = 0 ;
	expression->type = EXP_OPERATOR ;
	expression->delttype = EXP_DELT_DEFAULT ;	
	expression->handle = operator ;
	expression->left = 0 ;
	expression->right = 0 ;
	expression->brother = 0 ;
	
	switch ( expression->handle ) {

		case C_XKL :SCClStackPush ( &evalor->operator , expression ) ; break ;

		case C_XKR :
			
			while( !SCClStackEmpty ( &evalor->operator ) ) {
				
				tree = (EXPR* ) SCClStackPop ( &evalor->operator ) ;

				if ( C_XKL == tree->handle ) break ;

				SCClListInsert ( &evalor->suffix , (void* )tree ) ; 

			}
			
		break ;

		default :

			if ( SCClStackEmpty ( &evalor->operator ) ) {

				SCClStackPush ( &evalor->operator ,  expression ) ;
				
			} else {

				int priority = 0 ;

				stackexp = (EXPR* )SCClStackGet ( &evalor->operator ) ;

				priority = have_priority ( expression->handle , stackexp->handle ) ;

				if ( -1 != priority ) {
					SCClStackPush ( &evalor->operator , (void* )expression ) ;
				} else {
					
					while( !SCClStackEmpty ( &evalor->operator ) ) {
						
						stackexp = (EXPR* )SCClStackGet ( &evalor->operator ) ;

						priority = have_priority ( expression->handle , stackexp->handle ) ;
						
						if( C_XKL == stackexp->handle ) {
		
							SCClStackPop ( &evalor->operator ) ;
						
							break ;

						} else if ( -1 == priority ) {		
							
							SCClListInsert ( &evalor->suffix , (void* )SCClStackPop ( &evalor->operator ) ) ; 

						} else if( -1 != priority ) {
		
							break ;
						
						}
					
					}
				
					SCClStackPush ( &evalor->operator , expression ) ;

				}
				
			}
			
	}
	
}


void EvalorFinish () {
	
	//	author : Jelo Wang
	//	since : 20100427
	//	(C)TOK

	int pri = 0 ;
	EXPR* tree = 0 ;
	SCClStack suffix_stack = { 0 , 0 , 0 } ;
	SCClList* suffix_walker = 0 ;


	# ifdef __EVALOR_DEBUG
	{
		static int times = 1 ;
		SClog ( "Exp (%d) : " , times ) ;
		times ++ ;
	}
	# endif

				
	if ( !evalor ) return ;

   	while( !SCClStackEmpty (&evalor->operator) )
		SCClListInsert ( &evalor->suffix , SCClStackPop (&evalor->operator) ) ;
	
	for ( suffix_walker = evalor->suffix.head ; suffix_walker ; suffix_walker = suffix_walker->next ) {
		
		tree = (EXPR* ) suffix_walker->element ;
		tree->delt = 0 ;
		
		switch ( tree->type ) {

			case EXP_OPERATOR :
				
				# ifdef __EVALOR_DEBUG
					SClog ( "%s " , evalor_disp_operator (tree->handle) ) ;
				# endif

				tree->right = SCClStackPop ( &suffix_stack );
				tree->left = SCClStackPop ( &suffix_stack );
				tree->right->brother = tree->left ;
				tree->left->brother = tree->right ;
				SCClStackPush ( &suffix_stack , (void* )tree );
				
			break ;
			
			case EXP_OPERAND :
	
				# ifdef __EVALOR_DEBUG
				{
					AZONAL* azonal = tree->handle ;
					SClog ( "%s " , azonal->name ) ;
				}
				# endif
				
 				SCClStackPush ( &suffix_stack , (void* )tree );
				
		}
	}

	evalor->tree = (int) SCClStackPop ( &suffix_stack ) ;

	EvalorClear () ;

	# ifdef __EVALOR_DEBUG
		SClog ( "\n" ) ;
	# endif
	
}

void EvalorClear () {
	
	//	author : Jelo Wang
	//	since : 20100427
	//	(C)TOK

	SCClListDestroy ( &evalor->suffix ) ;
	SCClStackDestroy ( &evalor->operator ) ;

}


int EvalorGetTree () {
	
	//	author : Jelo Wang
	//	since : 20100427
	//	(C)TOK

	int tree = evalor->tree ;

	EXPR* t = tree ;
	AZONAL* a = t->handle ;

	EvalorUnsetCurrent () ;
			
	return tree ;

}


