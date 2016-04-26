
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

# include "schal.h"
# include "sccl.h"
# include "lair-lexer.h"
# include "lair-grammar.h"

# define LAIR_TOK_DEFAULT_LENGTH 256
# define LAIR_LANGUGE_KEYWORDS_TOTALL 10

SCClString LAIRTOK = { 0 , 0 , 0 , 0 , 0 } ;
LEXERLAIR* lexlair = 0 ;

typedef struct {
	
	//	author : Jelo Wang
	//	notes : key words of lair language
	//	since : 20100503
	//	(C)TOK

	char* name ;
	int   type ;

} LAIR_KEYWORDS ;

LAIR_KEYWORDS LAIR_key[ LAIR_LANGUGE_KEYWORDS_TOTALL ] = {

	{ "else" , LAIR_ELSE } ,
	{ "extern" , LAIR_EXTERN } ,
	{ "for" , LAIR_FOR } ,
	{ "goto" , LAIR_GOTO } ,
	{ "if" , LAIR_IF } ,
	{ "register" , LAIR_REGISTER } ,
	{ "return" , LAIR_RETURN } ,
	{ "static" , LAIR_STATIC } ,
	{ "volatile" , LAIR_VOLATILE } ,
	{ "while" , LAIR_WHILE } ,

} ;

static int LexerLair_digit_lex ( int v ) {
	
	//	author : Jelo Wang
	//	since : 20100412
	//	(C)TOK

	switch ( v ) {
		case LAIR_INTNUM : 
		case LAIR_FLTNUM :   
		case LAIR_HEXNUM :   
		case LAIR_FLTENUM :  
		case LAIR_INTENUM : 
			return 1 ; 
		break ;
	}

	return 0 ;

}


static int LexerLair_find_key ( char* token ) {

	//	author : Jelo Wang
	//	since : 2008
	//	updated : 20090816
	//	(C)TOK
	
	//	notes : look over keywords table

	int count = 0 ;


	while ( count < LAIR_LANGUGE_KEYWORDS_TOTALL ) {

		if ( !sc_strcmp ( LAIR_key[ count ].name , token ) )
			return LAIR_key[ count ].type ;
		
		count ++ ;
		
	}

	return 0 ;

}


static int LexerLair_search_function () {

	//	author : Jelo Wang
	//	since : 20100116
	//	(C)TOK

	int step_orignal = lexlair->code->get_walker ;

	for ( ; lexlair->code->get_walker < lexlair->code->length && lexlair->code->data[lexlair->code->get_walker] != '{'; ) {
		lexlair->code->get_walker  ++ ;
	}

	if (  '{'  == lexlair->code->data[lexlair->code->get_walker] ) {

		for ( ; lexlair->code->get_walker < lexlair->code->length && lexlair->code->data[lexlair->code->get_walker] != ')'; )
			lexlair->code->get_walker  -- ;

		if (  ')'  == lexlair->code->data[lexlair->code->get_walker] ) {

			lexlair->code->get_walker  = step_orignal ;
			return 1 ;

		} else {

			lexlair->code->get_walker  = step_orignal ;
			return 0 ;

		}

	} else {
		lexlair->code->get_walker  = step_orignal ;
		return 0 ;
	}

	
}


static int LexerLair_matchop ( int el ) {

	//	author : Jelo Wang
	//	since : 2008
	//	updated : 20090813
	//	notes : identify operator
	//	(C)TOK
	
	int STATE=0;
	int TYPE=0;

	static char buffer [ 4 ] ;

	if ( '=' == LexerLairLook ( 1 ) ) {
		
		switch ( el ) {
			
			case '=':
				
				LexerLairJump ( 2 ) ;
				sc_strcpy ( buffer , "==" ) ;
				
				lexlair->token = buffer ;
				lexlair->c = 0 ;
				lexlair->pv = lexlair->v ;
				lexlair->v = LAIR_EQ ;

			break;
			
			case '<':
				LexerLairJump ( 2 ) ;
				sc_strcpy ( buffer , "<=" ) ;
				
				lexlair->token = buffer ;
				lexlair->c = 0 ;
				lexlair->pv = lexlair->v ;				
				lexlair->v = LAIR_LE ;	

			break;
			
			case '>':
				LexerLairJump ( 2 ) ;
				sc_strcpy ( buffer , ">=" ) ;
				
				lexlair->token = buffer ;
				lexlair->c = 0 ;
				lexlair->pv = lexlair->v ;				
				lexlair->v = LAIR_GE ;					
			break;
			
			case '!':
				LexerLairJump ( 2 ) ;
				sc_strcpy ( buffer , "!=" ) ;
				
				lexlair->token = buffer ;
				lexlair->c = 0 ;
				lexlair->pv = lexlair->v ;				
				lexlair->v = LAIR_NE ;						
			break;
			
			case '+':
				LexerLairJump ( 2 ) ;
				sc_strcpy ( buffer , "+=" ) ;
				
				lexlair->token = buffer ;
				lexlair->c = 0 ;
				lexlair->pv = lexlair->v ;				
				lexlair->v = LAIR_SLA ;							
			break;
			
			case '-':
				LexerLairJump ( 2 ) ;
				sc_strcpy ( buffer , "-=" ) ;
				
				lexlair->token = buffer ;
				lexlair->c = 0 ;
				lexlair->pv = lexlair->v ;				
				lexlair->v = LAIR_SLS ;				
			break;
			
			case '*':
				LexerLairJump ( 2 ) ;
				sc_strcpy ( buffer , "*=" ) ;
				
				lexlair->token = buffer ;
				lexlair->c = 0 ;
				lexlair->pv = lexlair->v ;				
				lexlair->v = LAIR_SLM ;							
			break;
			
			case '/':
				LexerLairJump ( 2 ) ;
				sc_strcpy ( buffer , "/=" ) ;
				
				lexlair->token = buffer ;
				lexlair->c = 0 ;
				lexlair->pv = lexlair->v ;				
				lexlair->v = LAIR_SLD ;						
			break;
			
			case '|':
				LexerLairJump ( 2 ) ;
				sc_strcpy ( buffer , "|=" ) ;
				
				lexlair->token = buffer ;
				lexlair->c = 0 ;
				lexlair->pv = lexlair->v ;				
				lexlair->v = LAIR_BEO ;						
			break;
			
			case '&':
				LexerLairJump ( 2 ) ;
				sc_strcpy ( buffer , "&=" ) ;
				
				lexlair->token = buffer ;
				lexlair->c = 0 ;
				lexlair->pv = lexlair->v ;				
				lexlair->v = LAIR_BEA ;						
			break;
			
			case '^':
				LexerLairJump ( 2 ) ;
				sc_strcpy ( buffer , "^=" ) ;
				
				lexlair->token = buffer ;
				lexlair->c = 0 ;
				lexlair->pv = lexlair->v ;				
				lexlair->v = LAIR_BEY ;						
			break;
			
			case '%':
				LexerLairJump ( 2 ) ;
				sc_strcpy ( buffer , "%=" ) ;
				
				lexlair->token = buffer ;
				lexlair->c = 0 ;
				lexlair->pv = lexlair->v ;				
				lexlair->v = LAIR_BEB ;						
			break;
			
		}
		
	} else {

		switch ( el ) {
			
			case '=':
				
				LexerLairJump ( 1 ) ;
				sc_strcpy ( buffer , "=" ) ;
				
				lexlair->token = buffer ;
				lexlair->c = 0 ;
				lexlair->pv = lexlair->v ;				
				lexlair->v = LAIR_EQU ;			
				
			break;
			
			case '<':

				if ( '<' == LexerLairLook ( 1 ) && '=' == LexerLairLook ( 2 ) ) {
					
					LexerLairJump(3) ;
					sc_strcpy ( buffer , "<<=" ) ;
					
					lexlair->token = buffer ;
					lexlair->c = 0 ;
					lexlair->pv = lexlair->v ;					
					lexlair->v = LAIR_SHLL ;	
				
				} else if ( '<' == LexerLairLook ( 1 ) ) {

					LexerLairJump ( 2 ) ;
					sc_strcpy ( buffer , "<<" ) ;
					
					lexlair->token = buffer ;
					lexlair->c = 0 ;
					lexlair->pv = lexlair->v ;					
					lexlair->v = LAIR_SHL ;						

				} else {
				
					LexerLairJump ( 1 ) ;
					sc_strcpy ( buffer , "<" ) ;
					
					lexlair->token = buffer ;
					lexlair->c = 0 ;
					lexlair->pv = lexlair->v ;					 
					lexlair->v = LAIR_LT ;			

				}
				
			break;
			
			case '>':
				
				if ( '>' == LexerLairLook ( 1 ) && '=' == LexerLairLook ( 2 ) ) {

					LexerLairJump ( 3 )  ;
					sc_strcpy ( buffer , ">>=" ) ;
					
					lexlair->token = buffer ;
					lexlair->c = 0 ;
					lexlair->pv = lexlair->v ;					 
					lexlair->v = LAIR_SHRR ;
				
				} else if ( '>' == LexerLairLook ( 1 ) ) {

					LexerLairJump ( 2 ) ;
					sc_strcpy ( buffer , ">>" ) ;
					
					lexlair->token = buffer ;
					lexlair->c = 0 ;
					lexlair->pv = lexlair->v ;	
				 	lexlair->v = LAIR_SHR ;
				
				} else {
			    	
					LexerLairJump ( 1 ) ;
					sc_strcpy ( buffer , ">" ) ;
					
					lexlair->token = buffer ;
					lexlair->c = 0 ;
					lexlair->pv = lexlair->v ;					
				 	lexlair->v = LAIR_GT ;		
				
				}
					
			break;
			
			case '!':
				
				LexerLairJump ( 1 ) ;
				sc_strcpy ( buffer , "!" ) ;
				
				lexlair->token = buffer ;
				lexlair->c = 0 ;
				lexlair->pv = lexlair->v ;				
				lexlair->v = LAIR_TAN ;						
				
			break;
			
			case '+':
				
				if ( '+' == LexerLairLook ( 1 ) ) {

					LexerLairJump ( 2 ) ;
					sc_strcpy ( buffer , "++" ) ;
					
					lexlair->token = buffer ;
					lexlair->c = 0 ;
					lexlair->pv = lexlair->v ;					
					lexlair->v = LAIR_JAA ;	
					
				} else {
					
					int lexv = 0 ; 
					LexerLairJump ( 1 ) ;
					//lexv = LexerLairHeadGenv (1) ;

					//if ( LexerLair_digit_lex (lexv) ) {
						
					//	LexerLairGenv () ;

					//	return 1 ;

					//} else {

						sc_strcpy ( buffer , "+" ) ;
						
						lexlair->token = buffer ;
						lexlair->c = 0 ;
						lexlair->pv = lexlair->v ;					 
						lexlair->v = LAIR_ADD ;
						
					//}

					
				}
				
			break;
			
			case '-':
				
				if ( '-' == LexerLairLook ( 1 ) ) {
					
					LexerLairJump ( 2 ) ;
					sc_strcpy ( buffer , "--" ) ;
					
					lexlair->token = buffer ;
					lexlair->c = 0 ;
					lexlair->pv = lexlair->v ;					
					lexlair->v = LAIR_JNN ;
				
				} else if ( '>' == LexerLairLook ( 1 ) ) {
			    	
					 LexerLairJump ( 2 ) ;
					sc_strcpy ( buffer , "->" ) ;
					
					lexlair->token = buffer ;
					lexlair->c = 0 ;
					lexlair->pv = lexlair->v ;					 
					lexlair->v = LAIR_PNT ;					 
					 
				} else {

					int lexv = 0 ; 
					LexerLairJump ( 1 ) ;
					lexv = LexerLairHeadGenv (1) ;
					
					if ( LexerLair_digit_lex (lexv) ) {
						
						LexerLairGenv () ;
						SCClStringInsert ( &LAIRTOK , "-" , 0 ) ;
						
						switch ( lexlair->v ) {
							case LAIR_INTNUM : 
								lexlair->pv = lexlair->v ;
								lexlair->v = LAIR_MUS_INTNUM ;
							break ;
							case LAIR_FLTNUM :   
								lexlair->pv = lexlair->v ;
								lexlair->v = LAIR_MUS_FLTNUM ;
							break ;
							case LAIR_HEXNUM :   
								lexlair->pv = lexlair->v ;
								lexlair->v = LAIR_MUS_HEXNUM ;
							break ;
							case LAIR_FLTENUM :  
								lexlair->pv = lexlair->v ;
								lexlair->v = LAIR_MUS_FLTENUM ;
							break ;
							case LAIR_INTENUM : 
								lexlair->pv = lexlair->v ;
								lexlair->v = LAIR_MUS_INTENUM ;
							break ;
						}

						return 1 ;

					} else {
						
						sc_strcpy ( buffer , "-" ) ;
						
						lexlair->token = buffer ;
						lexlair->c = 0 ;
						lexlair->pv = lexlair->v ;					 
						lexlair->v = LAIR_SUB ;			
					
					}
					 
				}
				
			break;
			
			//	int* gtkings;
			//	int a,a2,a3,*a4,a5;
			//	pointer gtkings;
			//	pointer a4;
			
			case '*':
				
				if ( '*' == LexerLairLook ( 1 ) ) {

					LexerLairJump ( 2 ) ;
					sc_strcpy ( buffer , "**" ) ;
					
					lexlair->token = buffer ;
					lexlair->c = 0 ;
					lexlair->pv = lexlair->v ;					
					lexlair->v = LAIR_PPT ;							 

				} else {

					LexerLairJump ( 1 ) ;
					sc_strcpy ( buffer , "*" ) ;
					
					lexlair->token = buffer ;
					lexlair->c = 0 ;
					lexlair->pv = lexlair->v ;					
					lexlair->v = LAIR_MUL ;						 

				}
				
			break;
			
			case '/':
				

				if ( '/' == LexerLairLook ( 1 ) ) {
					
					LexerLairDropJunk ('/') ;

					lexlair->token = 0 ;
					lexlair->c = 0 ;
					lexlair->pv = 0 ;				
					lexlair->v = 0 ;

				} else if( '*' == LexerLairLook ( 1 ) ) {

					LexerLairDropJunk ('*') ;

					lexlair->token = 0 ;
					lexlair->c = 0 ;
					lexlair->pv = 0 ;				
					lexlair->v = 0 ;

				} else {

					LexerLairJump ( 1 ) ;
					sc_strcpy ( buffer , "/" ) ;
					
					lexlair->token = buffer ;
					lexlair->c = 0 ;
					lexlair->pv = lexlair->v ;				
					lexlair->v = LAIR_DIV ;
				
				}
				
			break;
			
			case '|':
				
				if ( '|' == LexerLairLook ( 1 ) ) {

					LexerLairJump ( 2 ) ;
					sc_strcpy ( buffer , "||" ) ;
					
					lexlair->token = buffer ;
					lexlair->c = 0 ;
					lexlair->pv = lexlair->v ;					
					lexlair->v = LAIR_OR ;						 

				} else {

					LexerLairJump ( 1 ) ;
					sc_strcpy ( buffer , "|" ) ;
					
					lexlair->token = buffer ;
					lexlair->c = 0 ;
					lexlair->pv = lexlair->v ;					
					lexlair->v = LAIR_HU ;						 

				}
				
			break;
			
			case '&':
				
				if ( '&' == LexerLairLook ( 1 ) ) {

					LexerLairJump ( 2 ) ;
					sc_strcpy ( buffer , "&&" ) ;
					
					lexlair->token = buffer ;
					lexlair->c = 0 ;
					lexlair->pv = lexlair->v ;					
					lexlair->v = LAIR_AND ;			 
					 
				} else {

					LexerLairJump ( 1 ) ;
					sc_strcpy ( buffer , "&" ) ;
					
					lexlair->token = buffer ;
					lexlair->c = 0 ;
					lexlair->pv = lexlair->v ;					
					lexlair->v = LAIR_HE ;		
					
				}
				
			break;
			
			case '^':
				
				LexerLairJump ( 1 ) ;
				sc_strcpy ( buffer , "^" ) ;
					
				lexlair->token = buffer ;
				lexlair->c = 0 ;
				lexlair->pv = lexlair->v ;				
				lexlair->v = LAIR_YHH ;					
				
			break;

/*

							{

								//	generate lsn
								SCClString Lsn = {0} ; 
								LexerLairNext () ;
									
								while ( sc_is_digit(lexlair->c) ) {
									SCClStringAdd ( &Lsn , LexerLairGetAtom () ) ;
									LexerLairNext () ;
								}   
								
								SCClStringAdd ( &Lsn , '\0' ) ;

								lexlair->lsn = SCClAtoi ( Lsn.data ) ;								
								SCClStringDestroyEx ( &Lsn ) ;
	
							}
*/
			
			case '%':

				if (  '$' == LexerLairLook ( 1 )  ) {
					
					LexerLairJump ( 2 ) ;
					
					//	skip these junk streams that we donnt needed
					LexerLairSkipSpace () ;
					
					while( LexerLairIsVar () ) ;

					if ( 0 == sc_strcmp ( "STK" , LAIRTOK.data ) ) {
						
						SCClStringReset ( &LAIRTOK ) ;

						//	skip .
						LexerLairNext () ;
						//	get stream
						while( LexerLairIsVar () ) ;
						
						lexlair->pv = lexlair->v ;	
						lexlair->token = LAIRTOK.data ;

						if ( 0 == sc_strcmp ("Creat",LAIRTOK.data) ) {			
							lexlair->v = LAIR_STKCRE ;
						}  else if ( 0 == sc_strcmp ("Release",LAIRTOK.data) ) {							
							lexlair->v = LAIR_STKREL ;
						} else if ( 0 == sc_strcmp ("Def",LAIRTOK.data) ) {							
							lexlair->v = LAIR_STKDEF ;
						} else if ( 0 == sc_strcmp ("Set",LAIRTOK.data) ) {			
							lexlair->v = LAIR_STKSET ;
						} else if ( 0 == sc_strcmp ("Get",LAIRTOK.data) ) {			
							lexlair->v = LAIR_STKGET ;
						} else if ( 0 == sc_strcmp ("Push",LAIRTOK.data) ) {			
							lexlair->v = LAIR_STKPUSH ;
						} else if ( 0 == sc_strcmp ("Pop",LAIRTOK.data) ) {							
							lexlair->v = LAIR_STKPOP ;
						} 

					} else if ( 'X' == LAIRTOK.data[0] ) {

						//	generating lsn
						SCClString Lsn = {0} ; 
						//	skip .
						LexerLairNext () ;
									
						while ( sc_is_digit(lexlair->c) ) {
							SCClStringAdd ( &Lsn , LexerLairGetAtom () ) ;
							LexerLairNext () ;
						}   
								
						SCClStringAdd ( &Lsn , '\0' ) ;

						lexlair->lsn = SCClAtoi ( Lsn.data ) ;
 						SCClStringDestroyEx ( &Lsn ) ;
						
						lexlair->pv = lexlair->v ;				
						lexlair->v = LAIR_X ;
						lexlair->token = LAIRTOK.data ;						

 					} else if ( 'P' == LAIRTOK.data[0] && 'E' == LAIRTOK.data[1] ) {
						
						lexlair->pv = lexlair->v ;				
						lexlair->v = LAIR_PE ;
						lexlair->token = LAIRTOK.data ;						

 					} else if ( 'C' == LAIRTOK.data[0] && 'A' == LAIRTOK.data[1] ) {

						lexlair->pv = lexlair->v ;				
						lexlair->v = LAIR_CA ;
						lexlair->token = LAIRTOK.data ;						

					}
					
					return STATE;
					
				}
				
				LexerLairJump ( 1 ) ;
				sc_strcpy ( buffer , "%" ) ;
					
				lexlair->token = buffer ;
				lexlair->c = 0 ;
				lexlair->pv = lexlair->v ;				
				lexlair->v = LAIR_MOD ;						
				
			break;

		}
	
		
	}
	
	return STATE;

}


LEXERLAIR* LexerLairNew ( unsigned char* data , int mmode ) {

	//	author : Jelo Wang
	//	since : 20090825
	//	(C)TOK

	//	return a new lexer

	LEXERLAIR* lexer = (LEXERLAIR* ) SCMalloc ( sizeof(LEXERLAIR) ) ; 

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
	lexer->scale = 0 ;

	lexer->code = SCClStringCreate ( data , sc_strlen ( data ) ) ;

	lexer->scstack = (SCClStack* ) SCMalloc ( sizeof ( SCClStack ) ) ;

	SCClStringInitEx ( &LAIRTOK , 256 ) ;	

	return lexer ;

}

void LexerLairSet ( LEXERLAIR* lexer ) {

	//	author : Jelo Wang
	//	since : 20090825
	//	(C)TOK

	//	set a runtime lexer

	lexlair = lexer ;

}

void LexerLairClearStatus () {

	//	author : Jelo Wang
	//	since : 20091127
	//	(C)TOK

	//	set value

	if ( !lexlair ) return ;

	lexlair->v = 0 ;
	lexlair->c = 0 ;
	lexlair->pv = 0 ;
	lexlair->ppv = 0 ;
	lexlair->token = 0 ;
	lexlair->scale = 0 ;

}

void LexerLairSetFile ( char* name ) {

	//	author : Jelo Wang
	//	since : 20090921
	//	(C)TOK

	//	set file name 

	if ( lexlair -> file ) {
		SCFree ( lexlair -> file ) ;
	}

	lexlair -> file = (char* ) SCMalloc ( sc_strlen ( name ) + 1 ) ;
	sc_strcpy ( lexlair -> file , name ) ;

}

void LexerLairDestroy () {

	//	author : Jelo Wang
	//	since : 20090828
	//	(C)TOK

	//	notes : destroy the current lexer

	if ( ! lexlair ) return ;

	SCClStringDestroy ( lexlair->code ) ;

	if ( lexlair->file ) SCFree ( lexlair->file ) ;  

	if ( lexlair->scstack ) {
		SCClStackDestroy ( lexlair->scstack ) ;
		SCFree ( lexlair->scstack ) ;
	}

	SCFreeEx (& lexlair ) ;

	if ( LAIRTOK.data ) {
		SCFreeEx ( &LAIRTOK.data ) ;
	}

}


LEXERLAIR* LexerLairGet () {

	//	author : Jelo Wang
	//	since : 20090901
	//	(C)TOK

	return lexlair ;

}

void LexerLairSetmode ( int mmode ) {

	//	author : Jelo Wang
	//	since : 20100129
	//	(C)TOK

	//	set lexer runing mode

	if ( lexlair )
		lexlair->mode = mmode ;

}

void LexerLairReady ()  {

	//	author : Jelo Wang
	//	notes : code_set_iterator
	//	since : 20090810
	//	(C)TOK
	
	( (SCClString*) lexlair->code)->get_walker = 0 ;
	( (SCClString*) lexlair->code)->last_walker = 0 ;

	lexlair->v = 0 ;
	lexlair->pv = 0 ;
	lexlair->ppv = 0;

	lexlair->line = 1 ;
	lexlair->stop = 0 ;
	lexlair->stack = 0 ;

	lexlair->deep = 0 ;
	lexlair->headbit = 0 ;	


}

int LexerLairOverflowed ()  {

	//	author : Jelo Wang
	//	notes : code_overflowed
	//	since : 20090810
	//	(C)TOK
	
	if ( ( lexlair->code -> get_walker >= lexlair->code -> length ) ) {
		lexlair->c = 0;
		lexlair->token = 0;
		lexlair->v = 0;
		lexlair -> stop = 1;
		return 1 ;
	}

	lexlair->c = lexlair->code -> data [ lexlair->code -> get_walker ] ;

	return 0;

}

int LexerLairNext ()  {

	//	author : Jelo Wang
	//	notes : code_next
	//	since : 20090810
	//	(C)TOK
	
	if ( LexerLairOverflowed () )
		return 1;

	lexlair->c = lexlair->code -> data [ lexlair->code -> get_walker + 1 ] ;

	lexlair->code -> last_walker = lexlair->code -> get_walker ;
	lexlair->code -> get_walker ++ ;
	

}

unsigned char LexerLairGetAtom ()  {

	//	author : Jelo Wang
	//	notes : code_get
	//	since : 20090810
	//	(C)TOK
	
	if ( LexerLairOverflowed () )
		return 0;
	
	lexlair->c = lexlair->code -> data [ lexlair->code -> get_walker ] ;

	return lexlair->code -> data [ lexlair->code -> get_walker ] ;

}



void LexerLairPutAtom ( unsigned char atom )  {

	//	author : Jelo Wang
	//	notes : code_get
	//	since : 20090926
	//	(C)TOK
	
	if ( LexerLairOverflowed () )
		return 0;
	
	lexlair->code -> data [ lexlair->code -> get_walker ] = atom ;

}

void LexerLairRollback ()  {

	//	author : Jelo Wang
	//	notes : code_setback_walker
	//	since : 20090810
	//	(C)TOK
	
	lexlair->code -> get_walker = SCClStackPop ( lexlair -> scstack ) ; 

}



void LexerLairBackup () { 

	//	author : Jelo Wang
	//	since : 20100308
	//	(C)TOK

	SCClStackPush ( lexlair -> scstack , lexlair->code -> get_walker ) ;
	
}

void LexerLairBackupClear () { 

	//	author : Jelo Wang
	//	since : 20100308
	//	(C)TOK

	SCClStackPop ( lexlair->scstack ) ;
	
}


void LexerLairSetback ( int step )  {

	//	author : Jelo Wang
	//	notes : code_setback_walker
	//	since : 20090810
	//	(C)TOK
	
	if ( 0 > lexlair->code->get_walker - step )
		return 0 ;

	lexlair->code->get_walker = lexlair->code->get_walker - step ;


}

void LexerLairSkipBlank () {

	//	author : Jelo Wang
	//	notes : is_blank_token
	//	since : 20090810
	//	(C)TOK
	
	while ( sc_is_blank ( LexerLairGetAtom () ) ) {

		if ( '\r' == lexlair->c ) lexlair->line ++ ;	
		if ( '\n' == lexlair->c ) lexlair->line ++ ;
		
		LexerLairNext () ;

	}


}

void LexerLairSkipSpace () {

	//	author : Jelo Wang
	//	notes : LexerLairSkipSpace
	//	since : 20090816
	//	(C)TOK
	
	while ( 0x20 == lexlair->code -> data[ lexlair->code -> get_walker ] || '\t' == lexlair->code -> data[ lexlair->code -> get_walker ] ) {
	
		LexerLairNext () ;

	}


}

void LexerLairJump ( int step ) {

	//	author : Jelo Wang
	//	notes : code_setback_walker
	//	since : 20090813
	//	(C)TOK
	
	if ( lexlair->code -> get_walker + step >= lexlair->code -> length )
		return ;
	
	lexlair->code -> last_walker = lexlair->code -> get_walker ;
	lexlair->code -> get_walker = lexlair->code -> get_walker + step ;
	lexlair->c = lexlair->code -> data [ lexlair->code -> get_walker ] ;	


}

int LexerLairLook ( int step )  {

	//	author : Jelo Wang
	//	notes : LexerLairLook
	//	since : 20090813
	//	(C)TOK
	
	if ( lexlair->code -> get_walker + step >= lexlair->code -> length )
		return 0 ;
	
	return lexlair->code -> data [ lexlair->code -> get_walker + step ] ; 


}

int LexerLairIsVar () {
	
	//	author : Jelo Wang
	//	rex model : $VAR => alpha(alpha | _)*
	//	since : 2008
	//	(C)TOK
	
	if( sc_is_alpha(lexlair->c)||'_' == lexlair->c|| sc_is_digit(lexlair->c) ) {
		SCClStringAdd ( &LAIRTOK , LexerLairGetAtom () ) ;
		LexerLairNext () ;
		return 1 ;
	}

	SCClStringAdd ( &LAIRTOK , LAIR_EOS ) ;

	return 0 ;

}


int LexerLairIsNum () {

	//	author : Jelo Wang
	//	rex model : $INTNUM
	//	since : 2008
	//	(C)TOK
	
	if( sc_is_digit(lexlair->c) ) {
		SCClStringAdd ( &LAIRTOK , LexerLairGetAtom () ) ;
		LexerLairNext () ;
		return 1 ;
	}   
	
	SCClStringAdd ( &LAIRTOK , '\0' ) ;

	return 0 ;
	
}

# define LAIRHEX(x) (sc_is_digit(x)||('a'<= x && x<='f')||('A'<=x && x<='F'))
	
int LexerLairIsHex() {

	//	author : Jelo Wang
	//	rex model : $HEXNUM
	//	since : 2008
	//	(C)TOK
	
	if(LAIRHEX(lexlair->c)) {
		SCClStringAdd ( &LAIRTOK , LexerLairGetAtom () ) ;
		LexerLairNext () ;
		return 1 ;
	}  

	SCClStringAdd ( &LAIRTOK , LAIR_EOS ) ;
	
	return 0 ;
	
}


int LexerLairCluster ( int el ) {

	//0x22 is "
	//0x5c is \
	//identify the contents of character varible,char*gtkings="[[][[";
	//dismatching bracket in quotation marks varible is not solecism
	//since : 2008
	//	(C)TOK
	
	int state = 0 ;

	if ( '"' == el ) {



		while ( lexlair->code->get_walker < lexlair->code->length ) {

			
			switch ( state ) {

				case 0 :

					if ( '"' == lexlair->c ) { state = 1 ; SCClStringAdd ( &LAIRTOK , lexlair->c ) ; LexerLairNext () ; }
					else { LexerLairNext () ; return 0 ; }
						
				break ;

				case 1 :

					if ( '\\' == lexlair->c ) { state = 2 ; LexerLairNext () ; }
					else if ( '"' == lexlair->c ) {

						SCClStringAdd ( &LAIRTOK , '"' ) ;
						SCClStringAdd ( &LAIRTOK , LAIR_EOS ) ;
											
						lexlair->token = LAIRTOK.data ;
						lexlair->c = 0 ;
						lexlair->pv = lexlair->v ;
						lexlair->v = LAIR_STR ;

						LexerLairNext () ;

						return 1 ;

						
					} else { SCClStringAdd ( &LAIRTOK , lexlair->c ) ; state = 1 ; LexerLairNext () ; }
				
					
						
				break ;		

				case 2 :

					SCClStringAdd ( &LAIRTOK , lexlair->c ) ; 
					LexerLairNext () ; 
					state = 1 ;
						
				break ;	
				
			}
		}

	
	} else if ( '\'' == el ) {

		int step = 0 ;
		
		while ( step < 3 ) {

			
			switch ( state ) {

				case 0 :

					if ( '\'' == lexlair->c ) { state = 1 ; SCClStringAdd ( &LAIRTOK , lexlair->c ) ; LexerLairNext () ; step ++ ; }
					else { LexerLairNext () ; return 0 ; }
						
				break ;

				case 1 :

					if ( '\\' == lexlair->c ) { state = 2 ; LexerLairNext () ; }
					else if ( '\'' == lexlair->c ) {

						SCClStringAdd ( &LAIRTOK , '\'' ) ;
						SCClStringAdd ( &LAIRTOK , LAIR_EOS ) ;

						lexlair->token = LAIRTOK.data ;
						lexlair->c = 0 ;
						lexlair->pv = lexlair->v ;
						lexlair->v = LAIR_CHA ;
						
						LexerLairNext () ;
						
						return 1 ;
						
						
					} else { SCClStringAdd ( &LAIRTOK , lexlair->c ) ; state = 1 ; step ++ ; LexerLairNext () ; }
				
						
				break ;		

				case 2 :

					SCClStringAdd ( &LAIRTOK , lexlair->c ) ; 
					LexerLairNext () ; 
					state = 1 ;
					step ++ ; 
					
				break ;	
				
			}
		}
		
	}


	return 1 ;


}


int LexerLairDropJunk ( int el ) {

	//	author : Jelo Wang
	//	since : 2008
	//	updated : 20090828
	//	notes : drop_junk streams like //.... /*...*/
	//	(C)TOK
	
	int get_walker = 0 ;
	
	char escape = 0 ; 

	if ( '/' == el ) {

		while ( lexlair->code->get_walker < lexlair->code->length ) {

			if ( '\\' == lexlair->code -> data[lexlair->code->get_walker] ) {
				escape = '\\' ;
			} else if( 13 == lexlair->code -> data[lexlair->code->get_walker] && '\\' != escape ) {
				break ;
			} else escape = 0 ;
			
			lexlair->code -> data [ lexlair->code->get_walker ] = 0x20 ;

			LexerLairNext () ; 

		}

	} else if( '*' == el ) {

		//	(exp) /*FASDF*//*asdfsdf****///*/
		//	(1)	/*FASDF*/
		//	(2)	/*asdfsdf****/
		//	(3)	//*/

		int state = 0 ;
		int atom = 0 ;


		while ( lexlair->code->get_walker < lexlair->code->length ) {

			atom = lexlair->code -> data [ lexlair->code->get_walker ] ;

			switch ( state ) {
				
				case 0 :
					
					if( '/' == atom ) {
						LexerLairNext () ; 
						state = 1 ;
					} else if ( '*' == atom ) {
						LexerLairNext () ; 
						state = 2 ;
					} else {
						lexlair->code -> data [ lexlair->code->get_walker ] = 0x20 ;
						LexerLairNext () ; 
					}

				break ;
				
				case 1 :

					if ( '*' == atom ) {
						lexlair->code -> data [ lexlair->code->get_walker - 1 ] = 0x20 ;
						lexlair->code -> data [ lexlair->code->get_walker ] = 0x20 ;
						LexerLairNext () ; 
					} else {
						lexlair->code -> data [ lexlair->code->get_walker - 1 ] = 0x20 ;
						lexlair->code -> data [ lexlair->code->get_walker ] = 0x20 ;
						LexerLairNext () ; 
					}

					state = 0 ;

				break;

				case 2 :

					if ( '/' == atom ) {
						lexlair->code -> data [ lexlair->code->get_walker - 1 ] = 0x20 ;
						lexlair->code -> data [ lexlair->code->get_walker ] = 0x20 ;
						LexerLairNext () ; 
						goto end_junk ;
					} 
					
					lexlair->code -> data [ lexlair->code->get_walker - 1 ] = 0x20 ;
					state = 0 ;

					
				break ;

			}

		}
	

	}
	
end_junk :
	
	return 1 ; 

}


int LexerLairLookahead ( char element , int step ) {

	//	author : Jelo Wang
	//	since : 20100111
	//	(C)TOK


	//	do nothing
	if ( -1 == step ) return -1 ;

	//	search with element
	if ( -2 == step ) {

		int step_orignal = lexlair->code->get_walker ;

		for ( ; lexlair->code->get_walker < lexlair->code->length && lexlair->code->data[lexlair->code->get_walker] != element; ) {
			lexlair->code->get_walker  ++ ;
		}

		if ( element  == lexlair->code->data[lexlair->code->get_walker] ) {
			lexlair->code->get_walker  = step_orignal ;
			return 1 ;
		} else {
			return -1 ;
		}

	//	search with step
	} else {

		int step_append =  lexlair->code->get_walker + step ;

		if ( step_append < lexlair->code->length ) {
			if ( element  == lexlair->code->data[step_append] )
				return 1 ;
			else
				return 1 ;
		} else return 0 ;
	}
	
}


int LexerLairExpect ( int atom ) {

	//	author : Jelo Wang
	//	since : 20100430
	//	(C)TOK

	if ( atom != LexerLairHeadGenv (1) ) {
		return 0 ;
	} else {
		LexerLairGenv () ;
		return 1 ;
	}
	
}

int LexerLairSkipBracket ( int bracket_type ) {

	//	author : Jelo Wang
	//	since : 20100116
	//	(C)TOK

	//	skip '(.....)'

	int stack = 0 ;
	
	char bracket_left = 0 ;
	char bracket_right = 0 ;

	if ( LEXLAIR_STRONG_BRACKET == bracket_type ) {
		
		bracket_left = '{' ;
		bracket_right = '}' ;
		
	} else if ( LEXLAIR_MID_BRACKET == bracket_type ) { 
	
		bracket_left = '[' ;
		bracket_right = ']' ;	
		
	} else if ( LEXLAIR_SMART_BRACKET == bracket_type ) {
	
		bracket_left = '(' ;
		bracket_right = ')' ;	
		
	}
	
	if ( bracket_left != lexlair->c ) return 0 ;

	while ( !lexlair -> stop ) {

		if ( bracket_left == lexlair->c ) stack ++ ;
		if ( bracket_right == lexlair->c ) stack -- ;

		LexerLairNext () ;

		if ( 0 == stack ) break ;
			
	}

	return 1 ;

	
	
}

int LexerLairGenv ()  {

	//	author : Jelo Wang
	//	since : 2008
	//	(C)TOK
	
	int state = 0;

	char suffix = 0 ;

	SCClStringReset ( &LAIRTOK ) ;

	while ( !lexlair->stop ) {
		
		LexerLairGetAtom () ;

		switch ( state ) {

			case 0:

				if ( sc_is_alpha (lexlair->c) || '_' == lexlair->c ) state = 1 ;
				
				else if ( sc_is_digit (lexlair->c) ) state = 3 ;

				else if ( 0x20 == lexlair->c ) {

					LexerLairNext () ;
					LexerLairGetAtom () ;
					
					lexlair->token = 0 ;

					lexlair->c = ' ';

					lexlair->ppv = lexlair->pv ;
					lexlair->pv = lexlair->v ;				
					lexlair->v = LAIR_SPACE ;

					if ( !(lexlair->mode & LEXLAIR_FLITER_MODE) )
						return 1 ;

				} else if ( '\n' == lexlair->c  ) {

					lexlair->line ++;
					LexerLairNext () ;

					lexlair->token = 0 ;
					
					lexlair->c = '\n';
					lexlair->ppv = lexlair->pv ;
					lexlair->pv = lexlair->v ;				
					lexlair->v = LAIR_ENTER ;

					if ( !(lexlair->mode & LEXLAIR_FLITER_MODE) )
						return 1 ;
					

				} 

				else if ( 13 == lexlair->c ) {
					
					//	dont return a changing row token to above modules
					//	lexlair->line ++;
					//	LexerLairNext () ;
					//	lexlair->ppv = lexlair->pv ;
					//	lexlair->pv = lexlair->v ;				
					//	lexlair->v = CHROW ;
					
					LexerLairNext () ;
					
					lexlair->token = 0 ;
					
					lexlair->c = 0 ;

					lexlair->ppv = lexlair->pv ;
					lexlair->pv = lexlair->v ;				
					lexlair->v = LAIR_CHROW ;

					if ( !(lexlair->mode & LEXLAIR_FLITER_MODE) )
						return 1 ;
					
				}

				else if ( '\\' == lexlair->c ) {

					LexerLairNext () ;

					lexlair->token = 0 ;
					
					lexlair->c = '\\';
					lexlair->ppv = lexlair->pv ;
					lexlair->pv = lexlair->v ;
					lexlair->v = LAIR_ESCAPE ;
 
					return 1 ;

				}				
				
				else if ( '\t' == lexlair->c ) {

					LexerLairNext () ;
					LexerLairGetAtom () ;

					lexlair->token = 0 ;
					
					lexlair->c = '\t';
					lexlair->ppv = lexlair->pv ;
					lexlair->pv = lexlair->v ;
					lexlair->v = LAIR_TABLEK ;
					
					//return 0 ;

				}
				
				else if ( '=' == lexlair->c ) state = 5 ;
				else if ( '+' == lexlair->c ) state = 5 ;
				else if ( '-' == lexlair->c ) state = 5 ;
				else if ( '<' == lexlair->c ) state = 5 ;
				else if ( '>' == lexlair->c ) state = 5 ;
				else if ( '|' == lexlair->c ) state = 5 ;
				else if ( '&' == lexlair->c ) state = 5 ;
				else if ( '!' == lexlair->c ) state = 5 ;
				else if ( '*' == lexlair->c ) state = 5 ;
				else if ( '/' == lexlair->c ) state = 5 ;
				else if ( '%' == lexlair->c ) state = 5 ;
				else if ( '^' == lexlair->c ) state = 5 ;

				else if( '\''== lexlair->c ) state = 6;
				else if( '"'== lexlair->c ) state = 6;

				else if ( '{' == lexlair->c ) { 

					LexerLairNext () ;
					lexlair->token = 0 ;
					lexlair->c = '{' ;
					lexlair->pv = lexlair->v ;				
					lexlair->v = LAIR_DKL ;
					
					return 1 ;

				} else if ( '}' == lexlair->c ) { 

					LexerLairNext () ;
					lexlair->token = 0 ;
					lexlair->c = '}' ;
					lexlair->pv = lexlair->v ;				
					lexlair->v = LAIR_DKR ;	

					return 1 ;

				} else if ( '[' == lexlair->c ) { 

					LexerLairNext () ;
					lexlair->token = 0 ;
					lexlair->c = '[' ;
					lexlair->pv = lexlair->v ;				
					lexlair->v = LAIR_ZKL ;	

					return 1 ;

				} else if ( ']' == lexlair->c ) { 

					LexerLairNext () ;
					lexlair->token = 0 ;
					lexlair->c = ']' ;
					lexlair->pv = lexlair->v ;				
					lexlair->v = LAIR_ZKR ;	

					return 1 ;

				} else if ( '(' == lexlair->c ) { 

					LexerLairNext () ;
					lexlair->token = 0 ;
					lexlair->c = '(' ;
					lexlair->ppv = lexlair->pv ;
					lexlair->pv = lexlair->v ;				
					lexlair->v = LAIR_XKL ;	

					return 1 ;

				} else if ( ')' == lexlair->c ) { 

					LexerLairNext () ;
					lexlair->token = 0 ;
					lexlair->c = ')' ;
					lexlair->pv = lexlair->v ;				
					lexlair->v = LAIR_XKR ;	
					return 1 ;

				} else if ( ';' == lexlair->c ) { 

					LexerLairNext () ;
					lexlair->token = 0 ;
					lexlair->c = ';' ;
					lexlair->pv = lexlair->v ;				
					lexlair->v = LAIR_FEN ;	
					return 1 ;

				} else if ( ',' == lexlair->c ) { 

					LexerLairNext () ;
					lexlair->token = 0 ;
					lexlair->c = ',' ;
					lexlair->pv = lexlair->v ;				
					lexlair->v = LAIR_DOU ;	
					return 1 ;

				} else if ( '.' == lexlair->c ) { 

					LexerLairNext () ;
					lexlair->token = 0 ;
					lexlair->c = '.' ;
					lexlair->pv = lexlair->v ;				
					lexlair->v = LAIR_FIL ;	
					return 1 ;

				} else {
					LexerLairNext () ;
				}
				
			break;

			case 1:
				
				while( LexerLairIsVar () ) ;

				lexlair->token = LAIRTOK.data ;					
				
				lexlair->pv = lexlair->v;

				lexlair->v = LexerLair_find_key ( lexlair->token ) ;

				if ( 0 == lexlair->v ) {

					//	backup lexer walker
					LexerLairBackup () ;										
					LexerLairSkipSpace () ;
						
					if ( '(' == LexerLairLook ( 0 ) ) {

						LexerLairSkipBracket ( LEXLAIR_SMART_BRACKET ) ;
						LexerLairSkipBlank () ;
						
						if ( '{' == LexerLairLook ( 0 ) ) {

							lexlair->v = LAIR_FUNCDEF ;	
							lexlair->c = 0 ;
						
						} else {
						
							lexlair->v = LAIR_FUNCCAL ;	
							lexlair->c = 0 ;
							
						}
						
					} else if ( '[' == lexlair->c ) {

						lexlair->v = LAIR_ARRAY ;	
						lexlair->c = 0 ;		
												
					} else if ( ':' == lexlair->c ) {

						lexlair->v = LAIR_LABEL ;	
						lexlair->c = 0 ;									

					} else {	

						//	variable-name.number
						if ( '.' == lexlair->c && sc_is_digit (LexerLairLook (1)) ) {

							{

								//	generate lsn
								SCClString Lsn = {0} ; 
								LexerLairNext () ;
									
								while ( sc_is_digit(lexlair->c) ) {
									SCClStringAdd ( &Lsn , LexerLairGetAtom () ) ;
									LexerLairNext () ;
								}   
								
								SCClStringAdd ( &Lsn , '\0' ) ;

								lexlair->lsn = SCClAtoi ( Lsn.data ) ;
 								SCClStringDestroyEx ( &Lsn ) ;

								SCClStringAdd ( &LAIRTOK , '.' ) ;
								SCClStringAddStr ( &LAIRTOK , SCClItoa (lexlair->lsn)) ;
								SCClStringAdd ( &LAIRTOK , '\0' ) ;
						
							}
								
							if ( 0 == lexlair->scale ) {
								lexlair->v = LAIR_VAR ;	
								lexlair->c = 0 ;		
							} else {

								lexlair->v = LAIR_VARDEF ;	
								lexlair->c = 0 ;	
							}

							return 1 ;
							
						}
						
					}

					//	rollback walker of lexer
					LexerLairRollback () ;
				
				} else {
		
					lexlair->c = 0 ;

				}
				
				return 1 ;

			break;

			case 2:

				lexlair -> pc = lexlair->c ;
				
				//	skip these junk streams that we donnt needed
				LexerLairSkipSpace () ;
				
 				while( LexerLairIsVar () ) ;

				lexlair->token = LAIRTOK.data ;
				
				lexlair->c = 0 ;
				lexlair->v = 0 ;
					
				lexlair->pv = lexlair->v;

				lexlair->v = LAIR_VAR ;	
											
				return 1 ;

			break;


			case 3:
				
				while( LexerLairIsNum () ) ;

				if( '.' == lexlair->c ) {

					SCClStringAdd ( &LAIRTOK , lexlair->c  ) ;
					LexerLairJump ( 1 ) ;
					state = 4 ;

				} else if( 'e' == lexlair->c || 'E' == lexlair->c ) {

					SCClStringAdd ( &LAIRTOK , lexlair->c  ) ;
					LexerLairJump ( 1 ) ;
					
					if ( '-' == lexlair->c || '+' == lexlair->c  ) {
						SCClStringAdd ( &LAIRTOK , lexlair->c  ) ;
						LexerLairJump ( 1 ) ;
					}

					while( LexerLairIsNum () ) ;

					SCClStringAdd ( &LAIRTOK , LAIR_EOS ) ;

					lexlair->token = LAIRTOK.data ;
					lexlair->pv = lexlair->v ;
					lexlair->v = LAIR_INTENUM ;

					return 1 ;

				} else {

					suffix = lexlair->code -> data [ lexlair->code -> get_walker ] ;
					
					SCClStringAdd ( &LAIRTOK , LAIR_EOS ) ;

					lexlair->token = LAIRTOK.data ;
					lexlair->pv = lexlair->v ;
					lexlair->v = LAIR_INTNUM ;

					return 1 ;	
					
				}

				
			break;


			case 4:

				if( 'e' == lexlair->c ) {
					
					SCClStringAdd ( &LAIRTOK , lexlair->c ) ;
					LexerLairJump ( 1 ) ;

					if ( '-' == lexlair->c || '+' == lexlair->c  ) {
						SCClStringAdd ( &LAIRTOK , lexlair->c ) ;
						LexerLairJump ( 1 ) ;
					}
					
					while( LexerLairIsNum () ) ;
					
					SCClStringAdd ( &LAIRTOK , LAIR_EOS ) ;

					lexlair->token = LAIRTOK.data ;
					lexlair->pv = lexlair->v ;
					lexlair->v = LAIR_FLTENUM ;

					return 1 ;

				} else {
			
					while( LexerLairIsNum () ) ;

					if( 'e' == lexlair->c ) {

						state = 4 ;

					} else {
					
						SCClStringAdd ( &LAIRTOK , LAIR_EOS ) ;

						lexlair->token = LAIRTOK.data ;
							
						lexlair->pv = lexlair->v ;
						lexlair->v = LAIR_FLTNUM ;
						
						return 1 ;

					}

				

				}
			
			break;

			case 5 :

				LexerLair_matchop ( lexlair->c ) ;

				return 1 ;
				
			break;

			case 6 :

				LexerLairCluster ( lexlair->c ) ;

				return 1 ;
				
			break ;

			default :
				return 1 ;

		}

	}

	return 0 ;

}


void LexerLairClearScale () {

	//	author : Jelo Wang
	//	since : 20100508
	//	(C)TOK

	if ( !lexlair ) return ;

	lexlair->scale = 0 ;
	
}


int LexerLairHeadGenv ( int border ) {

	//	author : Jelo Wang
	//	since : 20100119
	//	(C)TOK

	int value = -1 ;
	int walker = 0 ;

	int orgvalue = lexlair->v ;	
	int orgline = lexlair->line ;
	int orgpv = lexlair->pv ;
	int orgppv = lexlair->ppv ;
	int orgscale = lexlair->scale ;

	
	char* orgtoken = 0 ;
	char orgchar = lexlair->c ;

	LexerLairBackup () ;
	
	if ( lexlair->token ) {
		orgtoken = (char*) SCMalloc ( sc_strlen ( lexlair->token ) + 1 ) ;
		sc_strcpy_withlen ( orgtoken , lexlair->token , sc_strlen (lexlair->token) ) ;
	}
	
	for ( walker = 0 ; walker < border ; walker ++ )  {
		lexlair->deep ++ ;
		LexerLairGenv () ;
		lexlair->deep -- ;
	}

	if ( orgtoken ) {
		SCClStringReset ( &LAIRTOK ) ;
		SCClStringAddStr ( &LAIRTOK , orgtoken ) ;
		SCClStringAdd ( &LAIRTOK , '\0' ) ;
		SCFree ( orgtoken ) ;
	}
	
	value = lexlair->v ;

	lexlair->v = orgvalue ;
	lexlair->token = LAIRTOK.data ;
	lexlair->line = orgline ;
	lexlair->pv = orgpv ;
	lexlair->ppv = orgppv ;
	lexlair->scale = orgscale ;
	
	LexerLairRollback () ;
	
	return value ; 
	
}

