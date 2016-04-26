	
/*

+	LEXER , part of SC c-front-para

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

//	new feature about lexer
//	lexer should able to changing his contents dymanticlly . as for some reason like susbiting a macro_func .
//	cound let lexer analysing param's stream instead of code streams
//	since 20090825

//$VAR		=> alpha(alpha | _)*
//$HEXNUM	=> 0x(digit|hex)*
//$INTNUM	=> digit(digit)*
//$INTENUM	=> digit(digit)* e digit(digit)*
//$FLTNUM	=> digit(digit)* . digit(digit)*
//$FLTENUM	=> digit(digit)* . e digit(digit)*
//$PT		=> *$VAR
//$PPT		=> **$VAR

# include "c-lexer.h"
# include "c-grammar.h"
# include "c-errors.h"
# include "schal.h"
# include "sccl.h"

# define TOK_DEFAULT_LENGTH 256
# define C_LANGUGE_KEYWORDS_TOTALL 32

# define DefineHeadBitmapClear() lexc->headbit = 0 ;
# define DefineHeadBitmapSet( bit ,op) lexc->headbit op= bit ;
# define is_define_head_flow(flow) ( C_AUTO <= flow && flow <= C_DOUBLE )

#define CTOK_LENGTH 256 

static SCClString CTOK = { 0 , 0 , 0 , 0 , 0 } ;

LEXERC* lexc = 0 ;

static C_KEYWORDS c_key[ C_LANGUGE_KEYWORDS_TOTALL ] = {

	{ "auto" , C_AUTO } ,
	{ "break" , C_BREAK } ,
	{ "case" , C_CASE } ,
	{ "char" , C_CHAR } ,
	{ "continue" , C_CONTINUE } ,
	{ "const" , C_CONST } ,
	{ "default" , C_DEFAULT } ,
	{ "do" , C_DO } ,
	{ "double" , C_DOUBLE } ,
	{ "else" , C_ELSE } ,
	{ "enum" , C_ENUM } ,
	{ "extern" , C_EXTERN } ,
	{ "float" , C_FLOAT } ,
	{ "for" , C_FOR } ,
	{ "goto" , C_GOTO } ,
	{ "int" , C_INT } ,
	{ "if" , C_IF } ,
	{ "long" , C_LONG } ,
	{ "register" , C_REGISTER } ,
	{ "return" , C_RETURN } ,
	{ "short" , C_SHORT} ,
	{ "signed" , C_SIGNED } ,
	{ "sizeof" , C_SIZEOF } ,
	{ "static" , C_STATIC } ,
	{ "struct" , C_STRUCT } ,
	{ "switch" , C_SWITCH } ,
	{ "typedef" , C_TYPEDEF } ,
	{ "union" , C_UNION } ,
	{ "unsigned" , C_UNSIGNED } ,
	{ "void" , C_VOID } ,
	{ "volatile" , C_VOLATILE } ,
	{ "while" , C_WHILE } ,

};


static C_KEYWORDS c_presorinstruc [10] = {

	{ "define" , C_PI_DEFINE } ,
	{ "ifdef" , C_PI_IFDEF } ,
	{ "ifndef" , C_PI_IFNDEF } ,
	{ "pagrama" ,C_PI_PAGRAMA } ,
	{ "defined" ,C_PI_DEFINED } ,
	{ "endif" , C_PI_ENDIF } ,
	{ "include" , C_PI_INCLUDE } ,
	{ "if" , C_PI_IF } ,
	{"elif",C_PI_ELIF} ,
	{"else",C_PI_ELSE} 


} ;


char* lexerc_get_operator ( int operator ) {

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


LEXERC* lexerc_new ( unsigned char* data , int mmode ) {

	//	author : Jelo Wang
	//	since : 20090825
	//	(C)TOK

	//	return a new lexer

	LEXERC* lexer = (LEXERC* ) SCMalloc ( sizeof(LEXERC) ) ; 

	lexer->v = 0 ;
	lexer->pv = 0 ;
	lexer->ppv = 0;

	lexer->line = 1 ;
	lexer->stop = 0 ;
	lexer->file = 0 ;
	lexer->mode = mmode ;
	lexer->ios = 1 ;
	lexer->stack = 0 ;
	lexer->deep = 0 ;
	lexer->headbit = 0 ;

	lexer->code = SCClStringCreate ( data , sc_strlen ( data ) ) ;
	lexer->scstack = (SCClStack* ) SCMalloc ( sizeof ( SCClStack ) ) ;

	SCClStringInitEx ( &CTOK , CTOK_LENGTH ) ;

	return lexer ;

}

void lexerc_set ( LEXERC* lexer ) {

	//	author : Jelo Wang
	//	since : 20090825
	//	(C)TOK

	//	set a runtime lexer

	lexc = lexer ;

}

void lexerc_clear_status () {

	//	author : Jelo Wang
	//	since : 20091127
	//	(C)TOK

	//	set value

	if ( !lexc ) return ;

	lexc->v = 0 ;
	lexc->c = 0 ;
	lexc->pv = 0 ;
	lexc->ppv = 0 ;
	lexc->token = 0 ;

}

void lexerc_set_file ( char* name ) {

	//	author : Jelo Wang
	//	since : 20090921
	//	(C)TOK

	//	set file name 

	if ( lexc->file ) {
		SCFree ( lexc->file ) ;
	}

	lexc->file = (char* ) SCMalloc ( sc_strlen ( name ) + 1 ) ;
	sc_strcpy ( lexc->file , name ) ;

}

void lexerc_destroy () {

	//	author : Jelo Wang
	//	since : 20090828
	//	(C)TOK

	//	notes : destroy the current lexer

	if ( ! lexc ) return ;

	SCClStringDestroy ( lexc->code ) ;

	if ( lexc->file ) SCFree ( lexc->file ) ;  

	if ( lexc->scstack ) {
		SCClStackDestroy ( lexc->scstack ) ;
		SCFree ( lexc->scstack ) ;
	}

	SCFreeEx (& lexc ) ;

	SCClStringDestroyEx ( &CTOK ) ;

}


LEXERC* lexerc_get () {

	//	author : Jelo Wang
	//	since : 20090901
	//	(C)TOK

	return lexc ;

}

void lexerc_setmode ( int mmode ) {

	//	author : Jelo Wang
	//	since : 20100129
	//	(C)TOK

	//	set lexer runing mode

	if ( lexc ) lexc->mode = mmode ;

}

void lexerc_ready ()  {

	//	author : Jelo Wang
	//	notes : code_set_iterator
	//	since : 20090810
	//	(C)TOK

	( (SCClString*) lexc->code)->get_walker = 0 ;
	( (SCClString*) lexc->code)->last_walker = 0 ;

	lexc->v = 0 ;
	lexc->pv = 0 ;
	lexc->ppv = 0;
	lexc->c = 0 ;
	lexc->token = 0 ;
	
	lexc->line = 1 ;
	lexc->stop = 0 ;

	lexc->deep = 0 ;
	lexc->headbit = 0 ;	


}


static void lexerc_gen_headbit () {

	//	author : Jelo Wang
	//	since : 20091124
	//	(C)TOK
	
//	for ( ; !lexc->stop && (is_define_head_flow ( lexc->v )) ; lexerc_genv () ) {
			
		switch ( lexc->v ) {
			case C_CHAR :

				if ( (lexc->headbit & C_CHAR_BIT) || \
					(lexc->headbit & C_INT_BIT) || \
					(lexc->headbit & C_FLOAT_BIT) || \
					(lexc->headbit & C_DOUBLE_BIT) ) {
					cerror ( C_PARSER_MOD , IS_C_ERROR , "basics type multi-defined , line : %d\n" , lexc->line ) ;
				}
										
				//	char
				//	0000 0000 0000 0000 0000 0000 0000 0001
				lexc->headbit |= C_CHAR_BIT ;
				//	unsigned
//				lexc->headbit |= 0x200 ;
			break ;
			case C_INT :

				if ( (lexc->headbit & C_CHAR_BIT) || \
					(lexc->headbit & C_INT_BIT) || \
					(lexc->headbit & C_FLOAT_BIT) || \
					(lexc->headbit & C_DOUBLE_BIT) ) {
					cerror ( C_PARSER_MOD , IS_C_ERROR , "basics type multi-defined , line : %d\n" , lexc->line ) ;
				}
				
				//	int
				//	0000 0000 0000 0000 0000 0000 0000 0010
				lexc->headbit |= C_INT_BIT ;
				//	signed
//				lexc->headbit |= 0x100 ;				
			break ;		
			case C_FLOAT :

				if ( (lexc->headbit & C_CHAR_BIT) || \
					(lexc->headbit & C_INT_BIT) || \
					(lexc->headbit & C_FLOAT_BIT) || \
					(lexc->headbit & C_DOUBLE_BIT) ) {
					cerror ( C_PARSER_MOD , IS_C_ERROR , "basics type multi-defined , line : %d\n" , lexc->line ) ;
				}
				
				//	float
				//	0000 0000 0000 0000 0000 0000 0000 0100
				lexc->headbit |= C_FLOAT_BIT ;
				//	signed
//				lexc->headbit |= 0x100 ;					
			break ;		
			case C_DOUBLE :

				if ( (lexc->headbit & C_CHAR_BIT) || \
					(lexc->headbit & C_INT_BIT) || \
					(lexc->headbit & C_FLOAT_BIT) || \
					(lexc->headbit & C_DOUBLE_BIT) ) {
					cerror ( C_PARSER_MOD , IS_C_ERROR , "basics type multi-defined , line : %d\n" , lexc->line ) ;
				}
				
				//	double
				//	0000 0000 0000 0000 0000 0000 0000 1000
				lexc->headbit |= C_DOUBLE_BIT ;
				//	signed
//				lexc->headbit |= 0x100 ;					
			break ;		
			case C_SHORT :
				//	short
				//	0000 0000 0000 0000 0000 0000 0001 0000
				lexc->headbit |= C_SHORT_BIT ;			
			break ;		
			case C_LONG :
				//	long
				//	0000 0000 0000 0000 0000 0000 0010 0000
				if ( lexc->headbit & C_LONG_BIT ) {
					//	...0...
					//	(7) long long[7:1]	 -> unbit (6) when (7) actived.
					lexc->headbit &= 0xffffffdf ;
					//	long long
					//	0000 0000 0000 0000 0000 0000 0100 0000
					lexc->headbit |= C_LONG_LONG_BIT ;
				} else if ( lexc->headbit & C_SHORT_BIT ) {
					cerror ( C_PARSER_MOD , IS_C_ERROR , "basics type multi-defined , line : %d\n" , lexc->line ) ;
				} else {
					lexc->headbit |= C_LONG_BIT ;
				}
			break ;
			case C_SIGNED :
				//	signed
				//	0000 0000 0000 0000 0000 0001 0000 0000
				if ( (lexc->headbit & C_UNSIGNED_BIT) || (lexc->headbit & C_SIGNED_BIT) ) {
					cerror ( C_PARSER_MOD , IS_C_ERROR , "signed type multi-defined , line : %d\n" , lexc->line ) ;
				}

				//	signed
				lexc->headbit |= C_SIGNED_BIT ;	
				
			break ;
			case C_UNSIGNED :
				//	unsigned
				//	0000 0000 0000 0000 0000 0010 0000 0000
				if ( (lexc->headbit & C_UNSIGNED_BIT) || (lexc->headbit & C_SIGNED_BIT) ) {
					cerror ( C_PARSER_MOD , IS_C_ERROR , "signed type multi-defined , line : %d\n" , lexc->line ) ;
				}				
				//	unsigned
				lexc->headbit |= C_UNSIGNED_BIT ;
				
			break ;
			case C_REGISTER :		
				//	register
				//	0000 0000 0000 0000 0000 0100 0000 0000				
				lexc->headbit |= C_REGISTER_BIT ;
			break ;			
			case C_AUTO :		
				//	auto
				//	0000 0000 0000 0000 0000 01000 0000 0000				
				lexc->headbit |= C_AUTO_BIT ;
			break ;	
			case C_STATIC :	

				if ( (lexc->headbit & 0x1000) || (lexc->headbit & 0x2000) ) {
					cerror ( C_PARSER_MOD , IS_C_ERROR , "scope multi-defined , line : %d\n" , lexc->line ) ;
				}
				
				//	static
				//	0000 0000 0000 0000 0000 1000 0000 0000				
				lexc->headbit |= C_STATIC_BIT ;
				
			break ;			
			case C_EXTERN :		
				//	extern
				//	0000 0000 0000 0000 0001 0000 0000 0000				
				lexc->headbit |= C_EXTERN_BIT ;
			break ;				
		}
		
//	}

	
	
}


static int lexerc_digit_lex ( int v ) {
	
	//	author : Jelo Wang
	//	since : 20100412
	//	(C)TOK

	switch ( v ) {
		case C_INT_NUM : 
		case C_FLT_NUM :   
		case C_HEX_NUM :   
		case C_FLT_ENUM :  
		case C_INT_ENUM : 
			return 1 ; 
		break ;
	}

	return 0 ;

}

static int lexerc_find_key ( C_KEYWORDS key[] , char* token , int klen ) {

	//	author : Jelo Wang
	//	since : 2008
	//	updated : 20090816
	//	(C)TOK
	
	//	notes : look over keywords table

#if 0
	int count = 0 ;

	while ( count < C_LANGUGE_KEYWORDS_TOTALL ) {

		if ( !sc_strcmp ( c_key[ count ].name , token ) ) 
			return c_key[ count ].type ;

		count ++ ;
 
	}

	return 0 ;
#endif


	int count = 0 ;
	int results = 1 ;

	while ( results && count < klen ) {
		results = sc_strcmp ( key[ count ].name , token ) ;
		count ++ ;
	}

	if ( !results )
		return key[ count - 1 ].type ;
	    return 0 ;

	

}


int lexerc_overflowed ()  {

	//	author : Jelo Wang
	//	notes : code_overflowed
	//	since : 20090810
	//	(C)TOK
	
	if ( ( lexc->code->get_walker >= lexc->code->length ) ) {
		lexc->token = 0 ;
		lexc->c = 0 ;
		lexc->v = 0;
		lexc->stop = 1;
		return 1 ;
	}

	lexc->c = lexc->code->data [ lexc->code->get_walker ] ;

	return 0;

}

int lexerc_next ()  {

	//	author : Jelo Wang
	//	notes : code_next
	//	since : 20090810
	//	(C)TOK
	
	if ( lexerc_overflowed () )
		return 1;

	//	flit '\r'
	if( '\r' == lexc->code->data [ lexc->code->get_walker + 1 ] && 
		'\n' == lexc->code->data [ lexc->code->get_walker + 2 ] ) {
		//	replaire '\r\n' with '\n\0x20' is no problem
		lexc->code->data [ lexc->code->get_walker + 1 ] = '\n' ;
		lexc->code->data [ lexc->code->get_walker + 2 ] = ' ' ;
	}

	lexc->c = lexc->code->data [ lexc->code->get_walker + 1 ] ;

	lexc->code->last_walker = lexc->code->get_walker ;
	lexc->code->get_walker ++ ;
	

}

unsigned char lexerc_get_atom ()  {

	//	author : Jelo Wang
	//	notes : code_get
	//	since : 20090810
	//	(C)TOK
	
	if ( lexerc_overflowed () )
		return 0;

	//	flit '\r'
	if( '\r' == lexc->code->data [ lexc->code->get_walker ] && 
		'\n' == lexc->code->data [ lexc->code->get_walker + 1 ] ) {
		//	replaire '\r\n' with '\n\0x20' is no problem
		lexc->code->data [ lexc->code->get_walker ] = '\n' ;
		lexc->code->data [ lexc->code->get_walker + 1 ] = ' ' ;
	}	
	
	lexc->pc = lexc->c ;
	lexc->c = lexc->code->data [ lexc->code->get_walker ] ;

	return lexc->code->data [ lexc->code->get_walker ] ;

}



void lexerc_put_atom ( unsigned char atom )  {

	//	author : Jelo Wang
	//	notes : code_get
	//	since : 20090926
	//	(C)TOK
	
	if ( lexerc_overflowed () )
		return ;
	
	lexc->code->data [ lexc->code->get_walker ] = atom ;

}

void lexerc_rollback ()  {

	//	author : Jelo Wang
	//	notes : code_setback_walker
	//	since : 20090810
	//	(C)TOK

	lexc->stop = SCClStackPop ( lexc->scstack ) ;
	lexc->code->get_walker = SCClStackPop ( lexc->scstack ) ; 

}


void lexerc_backup () { 

	//	author : Jelo Wang
	//	since : 20100308
	//	(C)TOK

	SCClStackPush ( lexc->scstack , (void* )lexc->code->get_walker ) ;
	SCClStackPush ( lexc->scstack , (void* )lexc->stop ) ;
	
	
	
}

void lexerc_backup_clear () { 

	//	author : Jelo Wang
	//	since : 20100308
	//	(C)TOK

	SCClStackPop ( lexc->scstack ) ;
	
}


void lexerc_skip_blank () {

	//	author : Jelo Wang
	//	notes : is_blank_token
	//	since : 20090810
	//	(C)TOK
	
	while ( sc_is_blank ( lexc->code->data [ lexc->code->get_walker ] ) ) {
		
		if ( '\n' == lexc->c ) lexc->line ++ ;
		
		lexc->c = lexc->code->data [ lexc->code->get_walker + 1 ] ;
		lexc->code->last_walker = lexc->code->get_walker ;
		lexc->code->get_walker ++ ;

	}


}

void lexerc_skip_space () {

	//	author : Jelo Wang
	//	notes : lexerc_skip_space
	//	since : 20090816
	//	(C)TOK
	
	while ( 0x20 == lexc->code->data[ lexc->code->get_walker ] || '\t' == lexc->code->data[ lexc->code->get_walker ] ) {
	
		lexerc_next () ;

	}

}

void lexerc_jump ( int step ) {

	//	author : Jelo Wang
	//	notes : code_setback_walker
	//	since : 20090813
	//	(C)TOK
	
	if ( lexc->code->get_walker + step >= lexc->code->length )
		return ;

	//	flit '\r'
	if( '\r' == lexc->code->data [ lexc->code->get_walker + step ] && 
		'\n' == lexc->code->data [ lexc->code->get_walker + step + 1 ] ) {
		//	replaire '\r\n' with '\n\0x20' is no problem
		lexc->code->data [ lexc->code->get_walker + step ] = '\n' ;
		lexc->code->data [ lexc->code->get_walker + step + 1 ] = ' ' ;
	}	
	
	lexc->code->last_walker = lexc->code->get_walker ;
	lexc->code->get_walker = lexc->code->get_walker + step ;
	lexc->c = lexc->code->data [ lexc->code->get_walker ] ;	


}

int lexerc_look ( int step )  {

	//	author : Jelo Wang
	//	notes : lexerc_look
	//	since : 20090813
	//	(C)TOK
	
	if ( (lexc->code->get_walker + step) < 0 || (lexc->code->get_walker + step >= lexc->code->length) )
		return 0 ;
	
	return lexc->code->data [ lexc->code->get_walker + step ] ; 


}

int lexerc_is_suffix ( char suffix ) {

	//	author : Jelo Wang
	//	since : 20090829
	//	since : suffix of constant number are 'u' 'l'
	//	(C)TOK
	
	if ( 'u' == suffix || 'U' == suffix || 'l' == suffix || 'L' == suffix || '.' == suffix ) {
		return 1 ;
	}
	
	return 0 ;


}

int lexerc_is_var () {
	
	//	author : Jelo Wang
	//	rex model : $VAR => alpha(alpha | _)*
	//	since : 2008
	//	(C)TOK
	
	if( sc_is_alpha(lexc->c)||'_' == lexc->c|| sc_is_digit(lexc->c)) {
		SCClStringAdd ( &CTOK , lexerc_get_atom () ) ;
		lexerc_next () ;
		return 1 ;
	}

	SCClStringAdd ( &CTOK , C_EOS ) ;

	return 0 ;

}


int lexerc_is_num () {

	//	author : Jelo Wang
	//	rex model : $INTNUM
	//	since : 2008
	//	(C)TOK
	
	if( sc_is_digit(lexc->c) ) {
		SCClStringAdd ( &CTOK , lexerc_get_atom () ) ;
		lexerc_next () ;
		return 1 ;
	}   
	
	SCClStringAdd ( &CTOK , '\0' ) ;

	return 0 ;
	
}

# define LEXERC_HEX(x) (sc_is_digit(x)||('a'<= x && x<='f')||('A'<=x && x<='F'))
	
int lexerc_is_hex() {

	//	author : Jelo Wang
	//	rex model : $HEXNUM
	//	since : 2008
	//	(C)TOK
	
	if(LEXERC_HEX(lexc->c)) {
		SCClStringAdd ( &CTOK , lexerc_get_atom () ) ;
		lexerc_next () ;
		return 1 ;
	}  

	SCClStringAdd ( &CTOK , C_EOS ) ;
	
	return 0 ;
	
}


int lexerc_cluster ( int el ) {

	//0x22 is "
	//0x5c is \
	//identify the contents of character varible,char*gtkings="[[][[";
	//dismatching bracket in quotation marks varible is not solecism
	//since : 2008
	//	(C)TOK
	
	int state = 0 ;

	if ( '"' == el ) {



		while ( lexc->code->get_walker < lexc->code->length ) {

			
			switch ( state ) {

				case 0 :

					if ( '"' == lexc->c ) { 
						state = 1 ; 
						//	Jelo Edited 20120313 , No need " for C_STR
						//	SCClStringAdd ( &CTOK , lexc->c ) ;
						//	Jelo Finished
						lexerc_next () ; 
					} else { lexerc_next () ; return 0 ; }
						
				break ;

				case 1 :

					if ( '\\' == lexc->c ) { state = 2 ; lexerc_next () ; }
					else if ( '"' == lexc->c ) {

						//	Jelo Edited 20120313
						//	SCClStringAdd ( &CTOK , '"' ) ;
						//	Jelo Finished
						
						SCClStringAdd ( &CTOK , C_EOS ) ;
											
						lexc->token = CTOK.data ;
						lexc->c = 0 ;
						lexc->pv = lexc->v ;
						lexc->v = C_STR ;

						lexerc_next () ;

						return 1 ;

						
					} else { SCClStringAdd ( &CTOK , lexc->c ) ; state = 1 ; lexerc_next () ; }
				
					
						
				break ;		

				case 2 :

					SCClStringAdd ( &CTOK , lexc->c ) ; 
					lexerc_next () ; 
					state = 1 ;
						
				break ;	
				
			}
		}

	
	} else if ( '\'' == el ) {

		int step = 0 ;
		
		while ( step < 3 ) {

			
			switch ( state ) {

				case 0 :

					if ( '\'' == lexc->c ) { state = 1 ; SCClStringAdd ( &CTOK , lexc->c ) ; lexerc_next () ; step ++ ; }
					else { lexerc_next () ; return 0 ; }
						
				break ;

				case 1 :

					if ( '\\' == lexc->c ) { state = 2 ; lexerc_next () ; }
					else if ( '\'' == lexc->c ) {

						SCClStringAdd ( &CTOK , '\'' ) ;
						SCClStringAdd ( &CTOK , C_EOS ) ;

						lexc->token = CTOK.data ;
						lexc->c = 0 ;
						lexc->pv = lexc->v ;
						lexc->v = C_CHA ;
						
						lexerc_next () ;
						
						return 1 ;
						
						
					} else { SCClStringAdd ( &CTOK , lexc->c ) ; state = 1 ; step ++ ; lexerc_next () ; }
				
						
				break ;		

				case 2 :

					SCClStringAdd ( &CTOK , lexc->c ) ; 
					lexerc_next () ; 
					state = 1 ;
					step ++ ; 
					
				break ;	
				
			}
		}
		
	}


	return 1 ;


}


int lexerc_drop_junk ( int el ) {

	//	author : Jelo Wang
	//	since : 2008
	//	updated : 20090828
	//	notes : lexerc_drop_junk streams like //.... /*...*/
	//	(C)TOK
	
	int get_walker = 0 ;
	
	char escape = 0 ; 

	if ( '/' == el ) {
		
		while ( lexc->code->get_walker < lexc->code->length ) {
			
			if ( '\\' == lexc->code->data[lexc->code->get_walker] ) {
				escape = '\\' ;
			} else if( '\n' == lexc->code->data[lexc->code->get_walker] && '\\' != escape ) {
				break ;
			} else escape = 0 ;
			
			lexc->code->data [ lexc->code->get_walker ] = 0x20 ;

			lexerc_next () ; 

		}

	} else if( '*' == el ) {

		//	(exp) /*FASDF*//*asdfsdf****///*/
		//	(1)	/*FASDF*/
		//	(2)	/*asdfsdf****/
		//	(3)	//*/

		int state = 0 ;
		int atom = 0 ;


		while ( lexc->code->get_walker < lexc->code->length ) {

			atom = lexc->code->data [ lexc->code->get_walker ] ;

			switch ( state ) {
				
				case 0 :
					
					if( '/' == atom ) {
						lexerc_next () ; 
						state = 1 ;
					} else if ( '*' == atom ) {
						lexerc_next () ; 
						state = 2 ;
					} else {
						lexc->code->data [ lexc->code->get_walker ] = 0x20 ;
						lexerc_next () ; 
					}

				break ;
				
				case 1 :

					if ( '*' == atom ) {
						lexc->code->data [ lexc->code->get_walker - 1 ] = 0x20 ;
						lexc->code->data [ lexc->code->get_walker ] = 0x20 ;
						lexerc_next () ; 
					} else {
						lexc->code->data [ lexc->code->get_walker - 1 ] = 0x20 ;
						lexc->code->data [ lexc->code->get_walker ] = 0x20 ;
						lexerc_next () ; 
					}

					state = 0 ;

				break;

				case 2 :

					if ( '/' == atom ) {
						lexc->code->data [ lexc->code->get_walker - 1 ] = 0x20 ;
						lexc->code->data [ lexc->code->get_walker ] = 0x20 ;
						lexerc_next () ; 
						goto end_junk ;
					} 
					
					lexc->code->data [ lexc->code->get_walker - 1 ] = 0x20 ;
					state = 0 ;

					
				break ;

			}

		}
	

	}
	
end_junk :
	
	return 1 ; 

}


int lexerc_lookahead ( char element , int step ) {

	//	author : Jelo Wang
	//	since : 20100111
	//	(C)TOK


	//	do nothing
	if ( -1 == step ) return -1 ;

	//	search with element
	if ( -2 == step ) {

		int step_orignal = lexc->code->get_walker ;

		for ( ; lexc->code->get_walker < lexc->code->length && lexc->code->data[lexc->code->get_walker] != element; ) {
			lexc->code->get_walker  ++ ;
		}

		if ( element  == lexc->code->data[lexc->code->get_walker] ) {
			lexc->code->get_walker  = step_orignal ;
			return 1 ;
		} else {
			return -1 ;
		}

	//	search with step
	} else {

		int step_append =  lexc->code->get_walker + step ;

		if ( step_append < lexc->code->length ) {
			if ( element  == lexc->code->data[step_append] )
				return 1 ;
			else
				return 1 ;
		} else return 0 ;
	}
	
}

int lexerc_search_function () {

	//	author : Jelo Wang
	//	since : 20100116
	//	(C)TOK

	int step_orignal = lexc->code->get_walker ;

	for ( ; lexc->code->get_walker < lexc->code->length && lexc->code->data[lexc->code->get_walker] != '{'; ) {
		lexc->code->get_walker  ++ ;
	}

	if (  '{'  == lexc->code->data[lexc->code->get_walker] ) {

		for ( ; lexc->code->get_walker < lexc->code->length && lexc->code->data[lexc->code->get_walker] != ')'; )
			lexc->code->get_walker  -- ;

		if (  ')'  == lexc->code->data[lexc->code->get_walker] ) {

			lexc->code->get_walker  = step_orignal ;
			return 1 ;

		} else {

			lexc->code->get_walker  = step_orignal ;
			return 0 ;

		}

	} else {
		lexc->code->get_walker  = step_orignal ;
		return 0 ;
	}

	
}

int lexerc_expect ( int atom ) {

	//	author : Jelo Wang
	//	since : 20100430
	//	(C)TOK

	if ( atom != lexerc_head_genv (1) ) {
		cerror ( C_LEXER_MOD , IS_C_ERROR , "expected symbols inintegrity , line : %d\n" , lexc->line ) ;
		return 0 ;
	} else {
		lexerc_genv () ;
		return 1 ;
	}
	
}

int lexerc_skip_bracket ( int bracket_type ) {

	//	author : Jelo Wang
	//	since : 20100116
	//	(C)TOK

	//	skip '(.....)'

	int stack = 0 ;
	
	char bracket_left = 0 ;
	char bracket_right = 0 ;

	if ( LEXC_STRONG_BRACKET == bracket_type ) {
		
		bracket_left = '{' ;
		bracket_right = '}' ;
		
	} else if ( LEXC_MID_BRACKET == bracket_type ) { 
	
		bracket_left = '[' ;
		bracket_right = ']' ;	
		
	} else if ( LEXC_SMART_BRACKET == bracket_type ) {
	
		bracket_left = '(' ;
		bracket_right = ')' ;	
		
	}
	
	if ( bracket_left != lexc->c ) return 0 ;

	while ( !lexc->stop ) {

		if ( bracket_left == lexc->c ) stack ++ ;
		if ( bracket_right == lexc->c ) stack -- ;

		//	为了出现语法错误时，尽快地结束扫描。
		if ( ';' == lexc->c ) break ;
		else if ( '(' == bracket_left || '[' == bracket_left ) {
			if ( '{' == lexc->c || '}' == lexc->c ) 
				break ;
		}
		
		lexerc_next () ;

		if ( 0 == stack ) break ;
			
	}

	//	brackets unmatched
	if ( stack ) {
		cerror ( C_PARSER_MOD , IS_C_ERROR , "unmatched '%c' , line : %d\n" , bracket_right , lexc->line ) ;
		return 0 ;
	}
		
	return 1 ;	

	
	
}

int lexerc_matchop ( int el ) {

	//	author : Jelo Wang
	//	since : 2008
	//	updated : 20090813
	//	notes : identify operator
	//	(C)TOK
	
	int STATE=0;
	int TYPE=0;

	static char buffer [ 4 ] ;

	if ( '=' == lexerc_look ( 1 ) ) {
		
		switch ( el ) {
			
			case '=':
				
				lexerc_jump ( 2 ) ;
				sc_strcpy ( buffer , "==" ) ;
				
				lexc->token = buffer ;
				lexc->c = 0 ;
				lexc->pv = lexc->v ;
				lexc->v = C_EQ ;

			break;
			
			case '<':
				lexerc_jump ( 2 ) ;
				sc_strcpy ( buffer , "<=" ) ;
				
				lexc->token = buffer ;
				lexc->c = 0 ;
				lexc->pv = lexc->v ;				
				lexc->v = C_LE ;	

			break;
			
			case '>':
				lexerc_jump ( 2 ) ;
				sc_strcpy ( buffer , ">=" ) ;
				
				lexc->token = buffer ;
				lexc->c = 0 ;
				lexc->pv = lexc->v ;				
				lexc->v = C_GE ;					
			break;
			
			case '!':
				lexerc_jump ( 2 ) ;
				sc_strcpy ( buffer , "!=" ) ;
				
				lexc->token = buffer ;
				lexc->c = 0 ;
				lexc->pv = lexc->v ;				
				lexc->v = C_NE ;						
			break;
			
			case '+':
				lexerc_jump ( 2 ) ;
				sc_strcpy ( buffer , "+=" ) ;
				
				lexc->token = buffer ;
				lexc->c = 0 ;
				lexc->pv = lexc->v ;				
				lexc->v = C_SLA ;							
			break;
			
			case '-':
				lexerc_jump ( 2 ) ;
				sc_strcpy ( buffer , "-=" ) ;
				
				lexc->token = buffer ;
				lexc->c = 0 ;
				lexc->pv = lexc->v ;				
				lexc->v = C_SLS ;				
			break;
			
			case '*':
				lexerc_jump ( 2 ) ;
				sc_strcpy ( buffer , "*=" ) ;
				
				lexc->token = buffer ;
				lexc->c = 0 ;
				lexc->pv = lexc->v ;				
				lexc->v = C_SLM ;							
			break;
			
			case '/':
				lexerc_jump ( 2 ) ;
				sc_strcpy ( buffer , "/=" ) ;
				
				lexc->token = buffer ;
				lexc->c = 0 ;
				lexc->pv = lexc->v ;				
				lexc->v = C_SLD ;						
			break;
			
			case '|':
				lexerc_jump ( 2 ) ;
				sc_strcpy ( buffer , "|=" ) ;
				
				lexc->token = buffer ;
				lexc->c = 0 ;
				lexc->pv = lexc->v ;				
				lexc->v = C_BEO ;						
			break;
			
			case '&':
				lexerc_jump ( 2 ) ;
				sc_strcpy ( buffer , "&=" ) ;
				
				lexc->token = buffer ;
				lexc->c = 0 ;
				lexc->pv = lexc->v ;				
				lexc->v = C_BEA ;						
			break;
			
			case '^':
				lexerc_jump ( 2 ) ;
				sc_strcpy ( buffer , "^=" ) ;
				
				lexc->token = buffer ;
				lexc->c = 0 ;
				lexc->pv = lexc->v ;				
				lexc->v = C_BEY ;						
			break;
			
			case '%':
				lexerc_jump ( 2 ) ;
				sc_strcpy ( buffer , "%=" ) ;
				
				lexc->token = buffer ;
				lexc->c = 0 ;
				lexc->pv = lexc->v ;				
				lexc->v = C_BEB ;						
			break;
			
		}
		
	} else {

		switch ( el ) {
			
			case '=':
				
				lexerc_jump ( 1 ) ;
				sc_strcpy ( buffer , "=" ) ;
				
				lexc->token = buffer ;
				lexc->c = 0 ;
				lexc->pv = lexc->v ;				
				lexc->v = C_EQU ;			
				
			break;
			
			case '<':

				if ( '<' == lexerc_look ( 1 ) && '=' == lexerc_look ( 2 ) ) {
					
					lexerc_jump(3) ;
					sc_strcpy ( buffer , "<<=" ) ;
					
					lexc->token = buffer ;
					lexc->c = 0 ;
					lexc->pv = lexc->v ;					
					lexc->v = C_SHLL ;	
				
				} else if ( '<' == lexerc_look ( 1 ) ) {

					lexerc_jump ( 2 ) ;
					sc_strcpy ( buffer , "<<" ) ;
					
					lexc->token = buffer ;
					lexc->c = 0 ;
					lexc->pv = lexc->v ;					
					lexc->v = C_SHL ;						

				} else {
				
					lexerc_jump ( 1 ) ;
					sc_strcpy ( buffer , "<" ) ;
					
					lexc->token = buffer ;
					lexc->c = 0 ;
					lexc->pv = lexc->v ;					 
					lexc->v = C_LT ;			

				}
				
			break;
			
			case '>':
				
				if ( '>' == lexerc_look ( 1 ) && '=' == lexerc_look ( 2 ) ) {

					lexerc_jump ( 3 )  ;
					sc_strcpy ( buffer , ">>=" ) ;
					
					lexc->token = buffer ;
					lexc->c = 0 ;
					lexc->pv = lexc->v ;					 
					lexc->v = C_SHRR ;
				
				} else if ( '>' == lexerc_look ( 1 ) ) {

					lexerc_jump ( 2 ) ;
					sc_strcpy ( buffer , ">>" ) ;
					
					lexc->token = buffer ;
					lexc->c = 0 ;
					lexc->pv = lexc->v ;	
				 	lexc->v = C_SHR ;
				
				} else {
			    	
					lexerc_jump ( 1 ) ;
					sc_strcpy ( buffer , ">" ) ;
					
					lexc->token = buffer ;
					lexc->c = 0 ;
					lexc->pv = lexc->v ;					
				 	lexc->v = C_GT ;		
				
				}
					
			break;
			
			case '!':
				
				lexerc_jump ( 1 ) ;
				sc_strcpy ( buffer , "!" ) ;
				
				lexc->token = buffer ;
				lexc->c = 0 ;
				lexc->pv = lexc->v ;				
				lexc->v = C_TAN ;						
				
			break;
			
			case '+':
				
				if ( '+' == lexerc_look ( 1 ) ) {

					lexerc_jump ( 2 ) ;
					sc_strcpy ( buffer , "++" ) ;
					
					lexc->token = buffer ;
					lexc->c = 0 ;
					lexc->pv = lexc->v ;					
					lexc->v = C_JAA ;	
					
				} else {
					
					int lexv = 0 ; 
					lexerc_jump ( 1 ) ;

					//	Jelo Commited 20120307 , Because the '+' of 'x/(x1+x3)' would be missed for c_evalor
					//	lexv = lexerc_head_genv (1) ;			
					//	if ( lexerc_digit_lex (lexv) ) {						
					//	lexerc_genv () ;
					//	return 1 ;
					//	} else {

						sc_strcpy ( buffer , "+" ) ;
						
						lexc->token = buffer ;
						lexc->c = 0 ;
						lexc->pv = lexc->v ;					 
						lexc->v = C_ADD ;
					
					//}
					//	Jelo Commiting Finished
					
				}
				
			break;
			
			case '-':
				
				if ( '-' == lexerc_look ( 1 ) ) {
					
					lexerc_jump ( 2 ) ;
					sc_strcpy ( buffer , "--" ) ;
					
					lexc->token = buffer ;
					lexc->c = 0 ;
					lexc->pv = lexc->v ;					
					lexc->v = C_JNN ;
				
				} else if ( '>' == lexerc_look ( 1 ) ) {
			    	
					 lexerc_jump ( 2 ) ;
					sc_strcpy ( buffer , "->" ) ;
					
					lexc->token = buffer ;
					lexc->c = 0 ;
					lexc->pv = lexc->v ;					 
					lexc->v = C_PNT ;					 
					 
				} else if ( lexc->pv ) {

					int lexv = 0 ; 
					lexerc_jump ( 1 ) ;
					lexv = lexerc_head_genv (1) ;
					
					if ( lexerc_digit_lex (lexv) ) {
						
						lexerc_genv () ;
						SCClStringInsert ( &CTOK , "-" , 0 ) ;
						
						switch ( lexc->v ) {
							case C_INT_NUM : 
								lexc->pv = lexc->v ;
								lexc->v = C_NEG_INTNUM ;
							break ;
							case C_FLT_NUM :   
								lexc->pv = lexc->v ;
								lexc->v = C_NEG_FLTNUM ;
							break ;
							case C_HEX_NUM :   
								lexc->pv = lexc->v ;
								lexc->v = C_NEG_HEXNUM ;
							break ;
							case C_FLT_ENUM :  
								lexc->pv = lexc->v ;
								lexc->v = C_NEG_FLTENUM ;
							break ;
							case C_INT_ENUM : 
								lexc->pv = lexc->v ;
								lexc->v = C_NEG_INTENUM ;
							break ;
						}

						return 1 ;

					} else {
						
						sc_strcpy ( buffer , "-" ) ;
						
						lexc->token = buffer ;
						lexc->c = 0 ;
						lexc->pv = lexc->v ;					 
						lexc->v = C_SUB ;			
					
					}
					 
				}
				
			break;
			
			//	int* gtkings;
			//	int a,a2,a3,*a4,a5;
			//	pointer gtkings;
			//	pointer a4;
			
			case '*':
				
				if ( '*' == lexerc_look ( 1 ) ) {

					lexerc_jump ( 2 ) ;
					sc_strcpy ( buffer , "**" ) ;
					
					lexc->token = buffer ;
					lexc->c = 0 ;
					lexc->pv = lexc->v ;					
					lexc->v = C_PPT ;							 

				} else {

					lexerc_jump ( 1 ) ;
					sc_strcpy ( buffer , "*" ) ;
					
					lexc->token = buffer ;
					lexc->c = 0 ;
					lexc->pv = lexc->v ;					
					lexc->v = C_MUL ;						 

				}
				
			break;
			
			case '/':
				

				if ( '/' == lexerc_look ( 1 ) ) {
					
					lexerc_drop_junk ('/') ;

					lexc->token = 0 ;
					lexc->c = 0 ;
					lexc->pv = 0 ;				
					lexc->v = 0 ;

				} else if( '*' == lexerc_look ( 1 ) ) {

					lexerc_drop_junk ('*') ;

					lexc->token = 0 ;
					lexc->c = 0 ;
					lexc->pv = 0 ;				
					lexc->v = 0 ;

				} else {

					lexerc_jump ( 1 ) ;
					sc_strcpy ( buffer , "/" ) ;
					
					lexc->token = buffer ;
					lexc->c = 0 ;
					lexc->pv = lexc->v ;				
					lexc->v = C_DIV ;
				
				}
				
			break;
			
			case '|':
				
				if ( '|' == lexerc_look ( 1 ) ) {

					lexerc_jump ( 2 ) ;
					sc_strcpy ( buffer , "||" ) ;
					
					lexc->token = buffer ;
					lexc->c = 0 ;
					lexc->pv = lexc->v ;					
					lexc->v = C_OR ;						 

				} else {

					lexerc_jump ( 1 ) ;
					sc_strcpy ( buffer , "|" ) ;
					
					lexc->token = buffer ;
					lexc->c = 0 ;
					lexc->pv = lexc->v ;					
					lexc->v = C_HU ;						 

				}
				
			break;
			
			case '&':
				
				if ( '&' == lexerc_look ( 1 ) ) {

					lexerc_jump ( 2 ) ;
					sc_strcpy ( buffer , "&&" ) ;
					
					lexc->token = buffer ;
					lexc->c = 0 ;
					lexc->pv = lexc->v ;					
					lexc->v = C_AND ;			 
					 
				} else {

					lexerc_jump ( 1 ) ;
					sc_strcpy ( buffer , "&" ) ;
					
					lexc->token = buffer ;
					lexc->c = 0 ;
					lexc->pv = lexc->v ;					
					lexc->v = C_HE ;		
					
				}
				
			break;
			
			case '^':
				
				lexerc_jump ( 1 ) ;
				sc_strcpy ( buffer , "^" ) ;
					
				lexc->token = buffer ;
				lexc->c = 0 ;
				lexc->pv = lexc->v ;				
				lexc->v = C_YHH ;					
				
			break;
			
			case '%':
				
				lexerc_jump ( 1 ) ;
				sc_strcpy ( buffer , "%" ) ;
					
				lexc->token = buffer ;
				lexc->c = 0 ;
				lexc->pv = lexc->v ;				
				lexc->v = C_MOD ;						
				
			break;

		}
	
		
	}
	
	return STATE;

}

int lexerc_genv ()  {

	//	author : Jelo Wang
	//	since : 2008
	//	(C)TOK
	
	int state = 0;

	char suffix = 0 ;
	
	SCClStringReset ( &CTOK ) ;

REDO :
	
	while ( !lexc->stop ) {
		
		lexerc_get_atom () ;

		switch ( state ) {

			case 0:

				//	read L macro
				if ( 'L' == lexc->c ) {
					lexerc_skip_blank () ;
					if ( '"' == lexerc_look (1) ) {
						lexerc_next () ;
						state = 6 ;
					} else {
						state = 1 ;
					}
					
				} else if ( sc_is_alpha (lexc->c) || '_' == lexc->c ) state = 1 ;
				
				else if ( sc_is_digit (lexc->c) ) state = 3 ;

				else if ( 0x20 == lexc->c ) {

					lexerc_next () ;
					//lexerc_get_atom () ;
					
					lexc->token = 0 ;

					lexc->c = ' ';

					lexc->ppv = lexc->pv ;
					lexc->pv = lexc->v ;				
					lexc->v = C_SPACE ;

					if ( !(lexc->mode & LEXERC_FLITER_MODE) )
						return 1 ;

				} else if ( '\n' == lexc->c  ) {

					lexc->line ++;
					lexerc_next () ;

					lexc->token = 0 ;
					
					lexc->c = '\n';
					lexc->ppv = lexc->pv ;
					lexc->pv = lexc->v ;				
					lexc->v = C_ENTER ;
					
					if ( !(lexc->mode & LEXERC_FLITER_MODE) )
						return 1 ;

				} 

				else if ( '\r' == lexc->c ) {

					lexerc_next () ;

					lexc->token = 0 ;
						
					lexc->c = '\r' ;

					lexc->ppv = lexc->pv ;
					lexc->pv = lexc->v ;				
					lexc->v = C_CHROW ;

					if ( !(lexc->mode & LEXERC_FLITER_MODE) )
						return 1 ;
						
				}

				else if ( '\\' == lexc->c ) {

					lexerc_next () ;
					
					if ( C_ENTER == lexerc_head_genv (1) ) {

						//	flit '\'
						lexerc_next () ;
						//	flit '\n'
						lexerc_next () ;	

							
						//	return space
						lexc->token = 0 ;

						lexc->c = ' ';

						lexc->ppv = lexc->pv ;
						lexc->pv = lexc->v ;				
						lexc->v = C_SPACE ;

						if ( !(lexc->mode & LEXERC_FLITER_MODE) )
							return 1 ;
					
					} else {
 					
						lexerc_next () ;

						lexc->token = 0 ;
						
						lexc->c = '\\';
						lexc->ppv = lexc->pv ;
						lexc->pv = lexc->v ;
						lexc->v = C_ESCAPE ;
	 
						return 1 ;
						
					}

				}				
				
				else if ( '\t' == lexc->c ) {

					lexerc_next () ;
					lexerc_get_atom () ;

					lexc->token = 0 ;
					
					lexc->c = '\t';
					lexc->ppv = lexc->pv ;
					lexc->pv = lexc->v ;
					lexc->v = C_TABLEK ;
					
					//return 0 ;

				}

				else if ( '#' == lexc->c ) {
					lexc->pc = lexc->c ;
					lexerc_next () ;
					state = 2 ;
				}
				
				else if ( '=' == lexc->c ) state = 5 ;
				else if ( '+' == lexc->c ) state = 5 ;
				else if ( '-' == lexc->c ) state = 5 ;
				else if ( '<' == lexc->c ) state = 5 ;
				else if ( '>' == lexc->c ) state = 5 ;
				else if ( '|' == lexc->c ) state = 5 ;
				else if ( '&' == lexc->c ) state = 5 ;
				else if ( '!' == lexc->c ) state = 5 ;
				else if ( '*' == lexc->c ) state = 5 ;
				else if ( '/' == lexc->c ) state = 5 ;
				else if ( '%' == lexc->c ) state = 5 ;
				else if ( '^' == lexc->c ) state = 5 ;
				else if( '\''== lexc->c ) state = 6;
				else if( '"'== lexc->c ) state = 6;
				else if ( '{' == lexc->c ) { 

					lexerc_next () ;
					lexc->token = 0 ;
					lexc->c = '{' ;
					lexc->pv = lexc->v ;				
					lexc->v = C_DKL ;
					
					return 1 ;

				} else if ( '}' == lexc->c ) { 

					lexerc_next () ;
					lexc->token = 0 ;
					lexc->c = '}' ;
					lexc->pv = lexc->v ;				
					lexc->v = C_DKR ;	

					return 1 ;

				} else if ( '[' == lexc->c ) { 

					lexerc_next () ;
					lexc->token = 0 ;
					lexc->c = '[' ;
					lexc->pv = lexc->v ;				
					lexc->v = C_ZKL ;	

					return 1 ;

				} else if ( ']' == lexc->c ) { 

					lexerc_next () ;
					lexc->token = 0 ;
					lexc->c = ']' ;
					lexc->pv = lexc->v ;				
					lexc->v = C_ZKR ;	

					return 1 ;

				} else if ( '(' == lexc->c ) { 

					lexerc_next () ;
					lexc->token = 0 ;
					lexc->c = '(' ;
					lexc->ppv = lexc->pv ;
					lexc->pv = lexc->v ;				
					lexc->v = C_XKL ;	

					return 1 ;

				} else if ( ')' == lexc->c ) { 

					lexerc_next () ;
					lexc->token = 0 ;
					lexc->c = ')' ;
					lexc->pv = lexc->v ;				
					lexc->v = C_XKR ;	
					return 1 ;

				} else if ( ';' == lexc->c ) { 

					lexerc_next () ;
					lexc->token = 0 ;
					lexc->c = ';' ;
					lexc->pv = lexc->v ;				
					lexc->v = C_FEN ;	
					return 1 ;

				} else if ( ',' == lexc->c ) { 

					lexerc_next () ;
					lexc->token = 0 ;
					lexc->c = ',' ;
					lexc->pv = lexc->v ;				
					lexc->v = C_DOU ;	
					return 1 ;

				} else if ( '.' == lexc->c ) { 

					lexerc_next () ;
					lexc->token = 0 ;
					lexc->c = '.' ;
					lexc->pv = lexc->v ;				
					lexc->v = C_FIL ;	
					return 1 ;

				} else if ( ':' == lexc->c ) { 

					lexerc_next () ;
					lexc->token = 0 ;
					lexc->c = ':' ;
					lexc->pv = lexc->v ;				
					lexc->v = C_MAO ;	
					return 1 ;

				} else if ( '?' == lexc->c ) { 

					lexerc_next () ;
					lexc->token = 0 ;
					lexc->c = '?' ;
					lexc->pv = lexc->v ;				
					lexc->v = C_ASK ;	
					return 1 ;

				} else if ( '~' == lexc->c ) { 

					lexerc_next () ;
					lexc->token = 0 ;
					lexc->c = '~' ;
					lexc->pv = lexc->v ;
					lexc->v = C_EXO ;	
					return 1 ;

				} else {
					
					if ( 0 == lexc->ios ) {
						
						lexc->token = 0 ;
						lexc->pv = lexc->v ;				
						lexc->v = 0 ;	

						SClog ( "unrecognizable symbol : %x detected , on line : %d\n" , lexc->c , lexc->line ) ;

						lexerc_next () ;
						
						return 0 ;

					} 

					lexerc_next () ;

				}

				
			break;

			case 1:

				while( lexerc_is_var () ) ;

				lexc->token = CTOK.data ;					
				
				lexc->pv = lexc->v;

				lexc->v = lexerc_find_key ( c_key , lexc->token , C_LANGUGE_KEYWORDS_TOTALL ) ;

				if ( 0 == lexc->v ) {

					//	backup lexer walker
					lexerc_backup () ;										
					lexerc_skip_space () ;
						
					if ( '(' == lexerc_look ( 0 ) ) {

						lexerc_skip_bracket ( LEXC_SMART_BRACKET ) ;
						lexerc_skip_blank () ;
					
						if ( '{' == lexerc_look ( 0 ) ) {

							lexc->v = C_FUNC_DEF ;	
							lexc->c = 0 ;
						
						} else {
						
							lexc->v = C_FUNC_REF ;	
							lexc->c = 0 ;
							
						}
						
					} else if ( '[' == lexc->c ) {

						if ( 0 == lexc->headbit ) {
							lexc->v = C_ARRAY_REF ;	
							lexc->c = 0 ;		
						} else {
							lexc->v = C_ARRAY_DEF ;	
							lexc->c = 0 ;								
						}
						
					} else {	

						if ( 0 == lexc->headbit ) {
							lexc->v = C_VAR_REF ;	
							lexc->c = 0 ;		
						} else {
							lexc->v = C_VAR_DEF ;	
							lexc->c = 0 ;								
						}						
						
					}

					//	rollback walker of lexer
					lexerc_rollback () ;
				
				} else {
		
					//	read type defination flow
					if (  (lexc->mode & LEXERC_HEADBIT_MODE) && (is_define_head_flow (lexc->v)) && 0 == lexc->deep ) {
						
						int head_lex = 0 ;

						//	gen head bitmap
						lexerc_gen_headbit () ;

						//	skip blank flow
						lexerc_skip_blank () ;

						//	lexical head-genv
						head_lex = lexerc_head_genv (1) ;

						if ( is_define_head_flow (head_lex) ) {
							goto REDO ;
						} 

						lexerc_skip_blank () ;
						
						//	gen lexical atom
						lexc->deep ++ ;
						lexerc_genv() ;
						lexc->deep -- ;

						if ( C_VAR_REF == lexc->v || C_ARRAY_REF == lexc->v || C_FUNC_DEF == lexc->v ||
							C_VAR_DEF == lexc->v || C_ARRAY_DEF == lexc->v ) {
							state = 1 ;
						} else {
							lexc->headbit = 0 ;
							cerror ( C_LEXER_MOD , IS_C_ERROR , "basics type defination error , line : %d\n" , lexc->line ) ;
						}
						
						
					}
				
					lexc->c = 0 ;
						
				}
				
				return 1 ;

			break;

			case 2:
				
				//	skip these junk streams that we donnt needed
				lexerc_skip_space () ;
				
 				while( lexerc_is_var () ) ;

				//	state 2 is coming from # token , when # detected there maybe a another # is flowing
				//	so lexc->token could be 0
				if ( 0 < strlen(CTOK.data) ) {
					lexc->token = CTOK.data ;
				} else {
					lexc->token = 0 ;
				}
				
				lexc->c = 0 ;
				lexc->v = 0 ;
					
				lexc->pv = lexc->v;

				lexc->v = lexerc_find_key ( c_presorinstruc , lexc->token , 10  ) ;
				
				if ( !lexc->v ) lexc->v = C_VAR_REF ;	
											
				return 1 ;

			break;


			case 3:
				
				while( lexerc_is_num () ) ;

				if( '.' == lexc->c ) {

					SCClStringAdd ( &CTOK , lexc->c  ) ;
					lexerc_jump ( 1 ) ;
					state = 4 ;

				} else if( 'e' == lexc->c || 'E' == lexc->c ) {

					SCClStringAdd ( &CTOK , lexc->c  ) ;
					lexerc_jump ( 1 ) ;
					
					if ( '-' == lexc->c || '+' == lexc->c  ) {
						SCClStringAdd ( &CTOK , lexc->c  ) ;
						lexerc_jump ( 1 ) ;
					}

					while( lexerc_is_num () ) ;

					SCClStringAdd ( &CTOK , C_EOS ) ;

					lexc->token = CTOK.data ;
					lexc->pv = lexc->v ;
					lexc->v = C_INT_ENUM ;

					return 1 ;

				} else {

					suffix = lexc->code->data [ lexc->code->get_walker ] ;
					
					//	suffix of constant number are 'u' 'l'
					if ( lexerc_is_suffix( suffix ) ) {
						SCClStringAdd ( &CTOK , suffix  ) ;
					}

					SCClStringAdd ( &CTOK , C_EOS ) ;

					lexc->token = CTOK.data ;
					lexc->pv = lexc->v ;
					lexc->v = C_INT_NUM ;

					return 1 ;	
					
				}

				
			break;


			case 4:

				if( 'e' == lexc->c ) {
					
					SCClStringAdd ( &CTOK , lexc->c ) ;
					lexerc_jump ( 1 ) ;

					if ( '-' == lexc->c || '+' == lexc->c  ) {
						SCClStringAdd ( &CTOK , lexc->c ) ;
						lexerc_jump ( 1 ) ;
					}
					
					while( lexerc_is_num () ) ;
					
					SCClStringAdd ( &CTOK , C_EOS ) ;

					lexc->token = CTOK.data ;
					lexc->pv = lexc->v ;
					lexc->v = C_FLT_ENUM ;

					return 1 ;

				} else {
			
					while( lexerc_is_num () ) ;

					if( 'e' == lexc->c ) {

						state = 4 ;

					} else {
					
						SCClStringAdd ( &CTOK , C_EOS ) ;

						lexc->token = CTOK.data ;
							
						lexc->pv = lexc->v ;
						lexc->v = C_FLT_NUM ;
						
						return 1 ;

					}

				

				}
			
			break;

			case 5 :
				
				lexerc_matchop ( lexc->c ) ;

				return 1 ;
				
			break;

			case 6 :

				lexerc_cluster ( lexc->c ) ;

				return 1 ;
				
			break ;

			default :
				return 1 ;

		}

	}

	return 0 ;
	

}


void lexerc_clear_headbit () {

	//	author : Jelo Wang
	//	since : 20100508
	//	(C)TOK

	if ( !lexc ) return ;

	lexc->headbit = 0 ;
	
}

int lexerc_head_genv ( int border ) {

	//	author : Jelo Wang
	//	since : 20100119
	//	(C)TOK

	int value = -1 ;
	int walker = 0 ;

	int orgvalue = lexc->v ;	
	int orgline = lexc->line ;
	int orgpv = lexc->pv ;
	int orgppv = lexc->ppv ;
	
	char* orgtoken = 0 ;
	char orgchar = lexc->c ;

	lexerc_backup () ;
	
	if ( lexc->token ) {
		orgtoken = (char*) SCMalloc ( sc_strlen ( lexc->token ) + 1 ) ;
		sc_strcpy_withlen ( orgtoken , lexc->token , sc_strlen (lexc->token) ) ;
	}
	
	for ( walker = 0 ; walker < border ; walker ++ )  {
		lexc->deep ++ ;
		lexerc_genv () ;
		lexc->deep -- ;
	}

	if ( orgtoken ) {
		SCClStringReset ( &CTOK ) ;
		SCClStringAddStr ( &CTOK , orgtoken ) ;
		SCClStringAdd ( &CTOK , '\0' ) ;
		SCFree ( orgtoken ) ;
	}
	
	value = lexc->v ;

	lexc->v = orgvalue ;
	lexc->token = CTOK.data ;
	lexc->line = orgline ;
	lexc->pv = orgpv ;
	lexc->ppv = orgppv ;

	lexerc_rollback () ;
	
	return value ; 
	
}

