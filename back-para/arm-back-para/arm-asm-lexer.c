 
/*

+	ARMv6 ASM lexer , part of SC arm-back-para

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

# include "schal.h"
# include "sccl.h"
# include "arm-asm-lexer.h"
# include "arm-assemer.h"

LEXERARM* lexarm = 0 ;
SCClString ARMTOK = { 0 , 0 , 0 , 0 , 0 } ;

LEXERARM* lexerarm_new ( char* asm , int set ) {

	//	author : Jelo Wang
	//	since : 20100723
	//	(C)TOK	

	LEXERARM* larm = (LEXERARM* ) SCMalloc ( sizeof(LEXERARM) ) ;

	if ( !larm ) return 0 ;

	larm->code = SCClStringCreate ( asm , sc_strlen ( asm ) ) ;
	
	larm->set = set ;

	SCClStackInit ( &larm->stack ) ;

	SCClStringInitEx ( &ARMTOK , 256 ) ;
		
	return larm ;
	
}

void lexerarm_set ( LEXERARM* larm ) {

	//	author : Jelo Wang
	//	since : 20100723
	//	(C)TOK	

	lexarm = larm ;
	
}

void lexerarm_setmode ( int mode ) {

	//	author : Jelo Wang
	//	since : 20110130
	//	(C)TOK	

	lexarm->mode = mode ;
	
}

static int lexerarm_find_set ( char* token ) {

	//	author : Jelo Wang
	//	since : 2008
	//	updated : 20090816
	//	(C)TOK
	
	//	notes : look over keywords table

	int looper = 0 ;

	if ( ARM32_SET == lexarm->set ) {
		
		while ( looper < ARM32_INSSET_TOTALL ) {
			
			if ( !sc_strcmp ( arm32_insset [ looper ].name , token ) )
				return arm32_insset [ looper ].type ;
			
			looper ++ ;
		}
		
	} else if ( THUMB16_SET == lexarm->set ) {
	
		while ( looper < THUMB16_INSSET_TOTALL ) {
			
			if ( !sc_strcmp ( thumb16_insset [ looper ].name , token ) )
				return thumb16_insset [ looper ].type ;
			
			looper ++ ;
		}	
		
	}

	return -1 ;

}


int lexerarm_overflowed ()  {

	//	author : Jelo Wang
	//	since : 20100723
	//	(C)TOK
	
	if ( ( lexarm->code->get_walker >= lexarm->code->length ) ) {
		lexarm->c = 0;
		lexarm->v = 0;
		lexarm->stop = 1;
		return 1 ;
	}
	
	return 0;

}


char lexerarm_get_atom ()  {

	//	author : Jelo Wang
	//	since : 20100723
	//	(C)TOK
	
	if ( lexerarm_overflowed () )
		return 0;
	
	lexarm->c = lexarm->code->data [ lexarm->code->get_walker ] ;

	return lexarm->c ;

}


int lexerarm_next ()  {

	//	author : Jelo Wang
	//	since : 20100723
	//	(C)TOK
	
	if ( lexerarm_overflowed () )
		return 1 ;

	lexarm->c = lexarm->code->data [ lexarm->code->get_walker + 1 ] ;

	lexarm->code->last_walker = lexarm->code->get_walker ;
	lexarm->code->get_walker ++ ;

	return 1 ;

}

void lexerarm_rollback ()  {

	//	author : Jelo Wang
	//	since : 20100723
	//	(C)TOK
	
	lexarm->code->get_walker = SCClStackPop ( &lexarm->stack ) ; 

}


void lexerarm_backup () { 

	//	author : Jelo Wang
	//	since : 20100723
	//	(C)TOK

	SCClStackPush ( &lexarm->stack , (void* )lexarm->code->get_walker ) ;
	
}

void lexerarm_backup_clear () { 

	//	author : Jelo Wang
	//	since : 20100723
	//	(C)TOK

	SCClStackPop ( &lexarm->stack ) ;
	
}

void lexerarm_skip_blank () {

	//	author : Jelo Wang
	//	since : 20100723
	//	(C)TOK
	
	while ( sc_is_blank ( lexerarm_get_atom () ) ) {
		
		lexerarm_next () ;

	}


}

void lexerarm_skip_space () {

	//	author : Jelo Wang
	//	since : 20100723
	//	(C)TOK
	
	while ( 0x20 == lexarm->code->data[ lexarm->code->get_walker ] || '\t' == lexarm->code->data[ lexarm->code->get_walker ] ) {
	
		lexerarm_next () ;

	}


}

void lexerarm_jump ( int step ) {

	//	author : Jelo Wang
	//	since : 20100723
	//	(C)TOK
	
	if ( lexarm->code->get_walker + step >= lexarm->code ->length )
		return ;
	
	lexarm->code->last_walker = lexarm->code->get_walker ;
	lexarm->code->get_walker = lexarm->code->get_walker + step ;
	lexarm->c = lexarm->code->data [ lexarm->code->get_walker ] ;	


}

int lexerarm_look ( int step )  {

	//	author : Jelo Wang
	//	since : 20100723
	//	(C)TOK
	
	if ( lexarm->code->get_walker + step >= lexarm->code->length )
		return 0 ;
	
	return lexarm->code->data [ lexarm->code->get_walker + step ] ; 


}

int lexerarm_is_var () {
	
	//	author : Jelo Wang
	//	rex model : $VAR => alpha(alpha | _)*
	//	since : 2008
	//	(C)TOK
	
	if( sc_is_alpha(lexarm->c)||'_' == lexarm->c|| sc_is_digit(lexarm->c) ) {
		SCClStringAdd ( &ARMTOK , lexerarm_get_atom () ) ;
		lexerarm_next () ;
		return 1 ;
	}

	SCClStringAdd ( &ARMTOK , '\0' ) ;

	return 0 ;

}


int lexerarm_is_num () {

	//	author : Jelo Wang
	//	rex model : $INTNUM
	//	since : 2008
	//	(C)TOK
	
	if( sc_is_digit(lexarm->c) ) {
		SCClStringAdd ( &ARMTOK , lexerarm_get_atom () ) ;
		lexerarm_next () ;
		return 1 ;
	}  

	SCClStringAdd ( &ARMTOK , '\0' ) ;
	
	return 0 ;
	
}

# define AASMHEX(x) (sc_is_digit(x)||('a'<= x && x<='f')||('A'<=x && x<='F'))
	
int lexerarm_is_hex() {

	//	author : Jelo Wang
	//	rex model : $HEXNUM
	//	since : 2008
	//	(C)TOK
	
	if(AASMHEX(lexarm->c)) {
		SCClStringAdd ( &ARMTOK , lexerarm_get_atom () ) ;
		lexerarm_next () ;
		return 1 ;
	}  

	SCClStringAdd ( &ARMTOK , '\0' ) ;
	
	return 0 ;
	
}

int lexerarm_genv () {

	//	author : Jelo Wang
	//	since : 20100723
	//	(C)TOK	

	int state = 0 ;

	SCClStringReset ( &ARMTOK ) ;
		
	while ( !lexarm->stop ) {
		
		lexerarm_get_atom () ;

		switch ( state ) {

			case 0:

				if ( sc_is_alpha (lexarm->c) ) state = 1 ;
				
				else if ( sc_is_digit (lexarm->c) ) state = 3 ;

				else if ( 0x20 == lexarm->c ) {

					lexerarm_next () ;
					lexerarm_get_atom () ;

				} else if ( '\n' == lexarm->c  ) {

					lexerarm_next () ;
					
					lexarm->c = '\n';
					lexarm->pv = lexarm->v ;				
					lexarm->v = AASM_ENTER ;
					
					if ( !(lexarm->mode & LEXERARM_FLITER_MODE) )
						return 1 ;

				} else if ( 13 == lexarm->c ) {
					
					lexerarm_next () ;
					
					lexarm->c = '\n';
					lexarm->pv = lexarm->v ;				
					lexarm->v = AASM_CHROW ;

					if ( !(lexarm->mode & LEXERARM_FLITER_MODE) )
						return 1 ;

				} else if ( '[' == lexarm->c ) { 

					lexerarm_next () ;
					lexarm->token = 0 ;
					lexarm->c = '[' ;
					lexarm->pv = lexarm->v ;				
					lexarm->v = AASM_ZKL ;	

					return 1 ;

				} else if ( ']' == lexarm->c ) { 

					lexerarm_next () ;
					lexarm->token = 0 ;
					lexarm->c = ']' ;
					lexarm->pv = lexarm->v ;				
					lexarm->v = AASM_ZKR ;	

					return 1 ;

				} else if ( ',' == lexarm->c ) { 

					lexerarm_next () ;
					lexarm->token = 0 ;
					lexarm->c = ',' ;
					lexarm->pv = lexarm->v ;				
					lexarm->v = AASM_DOU ;	
					
					return 1 ;
					
				} else if ( '#' == lexarm->c ) {
					
					lexerarm_next () ;
					lexarm->token = 0 ;
					lexarm->c = ',' ;
					lexarm->pv = lexarm->v ;				
					lexarm->v = AASM_SHARP ;	
					
					return 1 ;					
					
				} else {
		
					lexerarm_next () ;

				}

				
			break;

			case 1:
				
				while( lexerarm_is_var () ) ;

				lexarm->token = ARMTOK.data ;					
				
				lexarm->pv = lexarm->v;

				lexarm->v = lexerarm_find_set ( lexarm->token ) ;

				if ( 0 == lexarm->v ) {

					//	backup lexer walker
					lexerarm_backup () ;										
					lexerarm_skip_space () ;
						
					if ( '[' == lexarm->c ) {

						lexarm->v = AASM_ARRAY ;	
						lexarm->c = 0 ;		
												
					} else if ( ':' == lexarm->c ) {

						lexarm->v = AASM_LABEL ;	
						lexarm->c = 0 ;									

					} else {	

						lexarm->v = AASM_VAR ;	
						lexarm->c = 0 ;		

					}

					//	rollback walker of lexer
					lexerarm_rollback () ;
				
				} else if ( THUMB16_KEY_PROC == lexarm->v ) {

					lexerarm_genv () ;

					lexarm->token = ARMTOK.data ;
					lexarm->c = 0 ;
					lexarm->pv = lexarm->v ;				
					lexarm->v = AASM_PROCDEF ;						
					
				} else {

					//	skip ',' here
//					if ( ',' == lexerarm_look (0) ) lexerarm_jump ( 1 ) ;	

					lexarm->c = 0 ;

				}
				
				return 1 ;

			break;

			case 2:
				
				//	skip these junk streams that we donnt needed
				lexerarm_skip_space () ;
				
				while( lexerarm_is_var () ) ;

				lexarm->token = ARMTOK.data ;
				
				lexarm->c = 0 ;
					
				lexarm->pv = lexarm->v;

				lexarm->v = AASM_VAR ;	
				
				//	skip ',' here
//				if ( ',' == lexerarm_look (0) ) lexerarm_jump ( 1 ) ;	

											
				return 1 ;

			break;


			case 3:
				
				while( lexerarm_is_num () ) ;

				lexarm->token = ARMTOK.data ;
				
				lexarm->c = 0 ;
				
				lexarm->pv = lexarm->v;

				lexarm->v = AASM_INTENUM ;	

				//	skip ',' here
//				if ( ',' == lexerarm_look (0) ) lexerarm_jump ( 1 ) ;

				return 1 ;
				
			break;

			default :
				return 1 ;

		}

	}
	
	return 0 ;

}


int lexerarm_head_genv ( int border ) {

	//	author : Jelo Wang
	//	since : 20100730
	//	(C)TOK

	int value = -1 ;
	int walker = 0 ;

	int orgvalue = lexarm->v ;	
	int orgpv = lexarm->pv ;
	
	char* orgtoken = 0 ;
	char orgchar = lexarm->c ;

	lexerarm_backup () ;
	
	if ( lexarm->token ) {
		orgtoken = (char*) SCMalloc ( sc_strlen ( lexarm->token ) + 1 ) ;
		sc_strcpy_withlen ( orgtoken , lexarm->token , sc_strlen (lexarm->token) ) ;
	}
	
	for ( walker = 0 ; walker < border ; walker ++ )  {
		lexerarm_genv () ;
	}

	if ( orgtoken ) {
		SCClStringReset ( &ARMTOK ) ;
		SCClStringAddStr ( &ARMTOK , orgtoken ) ;
		SCClStringAdd ( &ARMTOK , '\0' ) ;
		SCFree ( orgtoken ) ;
	}
	
	value = lexarm->v ;

	lexarm->v = orgvalue ;
	lexarm->token = ARMTOK.data ;
	lexarm->pv = orgpv ;

	lexerarm_rollback () ;
	
	return value ; 
	
}


int lexerarm_destroy () {

	//	author : Jelo Wang
	//	since : 20100723
	//	(C)TOK	

	if ( ! lexarm ) return 0 ;

	SCClStringDestroy ( lexarm->code ) ;

	SCFreeEx (&lexarm ) ;

	return 1 ;

}
