
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

# include "c-presor.h"
# include "c-lexer.h"
# include "c-grammar.h"
# include "c-errors.h"
# include "evalor.h"
# include "schal.h"
# include "sccl.h"
# include "semo.h"

int read_if () ;
int read_ifdef () ;
int read_ifndef () ;

//	notes : used for saving macros
MACRO macro ;
MACRO* macro_walker = &macro ;

//	notes : SCClStack
SCClStack macro_stack ;

//	notes : SCClStack
SCClStack stack ;

# define MAX_PATH 260

# define MACRO_PACKED( macro , nname ) \
		macro->name = (unsigned char* ) SCMalloc ( sc_strlen (nname) ) ; \
		sc_strcpy ( macro->name , nname ) ;\

# define macro_set_iterator( macro , walker ) \
		macro . front = 0 ; \
		macro . next = 0 ; \
		walker = &macro ; \

static void precompiling () ;

static void macro_insert ( MACRO* macro ) {

	//	author : Jelo Wang
	//	since : 20090812
	//	(C)TOK
	
	//	(1) param : MACRO pointer
	//	(2) param : MACRO pointer

	//	notes : initialize macro list
	
	macro->front = 0 ;
	macro->next = 0 ;
	
	macro_walker->next = macro ;
	macro_walker = macro ;

}

static void macro_destroy () {

	//	author : Jelo Wang
	//	since : 20100731
	//	(C)TOK


# if 0
	for ( macro_walker = macro.next ; macro_walker ; macro_walker = macro_walker -> next )
		printf("macro : %s , body : %s, length : %d\n",macro_walker->name,macro_walker->body.data,sc_strlen(macro_walker->body.data));
# endif

	for ( macro_walker = macro.next ; macro_walker ; ) {

		macro.next = macro_walker->next ; 
		SCFree ( macro_walker ) ;
		macro_walker = macro.next ;
	}
	
}

static SCClString* skip_smart_brackets_scope () {

	//	author : Jelo Wang
	//	since : 200908
	//	(C)TOK
	
	SCClString* s = (SCClString* ) SCMalloc ( sizeof(SCClString) ) ;

	if ( 0 == s ) return 0 ;
		
	SCClStringInit( s ) ;

	lexc->stack = 0 ;
	
	while ( !lexc->stop ) {
		
		lexerc_genv () ;
		
		if ( 0 == lexc->stack && sc_is_space(lexc->c) ) continue ;

		if ( C_XKL == lexc->v )
			lexc->stack ++ ;
		if ( C_XKR == lexc->v )
			lexc->stack -- ;

		if ( 0 == lexc->stack )
			break;

		if ( lexc->c ) SCClStringAdd ( s , lexc->c ) ;
		else if( lexc->token ) SCClStringAddStr ( s , lexc->token ) ;

	}

	lexc->stack = 0 ;
	
	SCClStringAdd ( s , ')' ) ;
	SCClStringAdd ( s , '\0' ) ;

	return s ;

}

static char* get_macro_param ( unsigned char* data , int pos ) {
	
	//	author : Jelo Wang
	//	since : 20090826
	//	(C)TOK
	
	//	(1) param : return a param tokens including brackets

	int stack = 0 ;
	int walker = 0 ;
	int counter = 0 ;
	int data_len = sc_strlen(data) ;

	char* buffer = (char* ) SCMalloc ( data_len ) ;

	ASSERT(buffer) ;

	for ( walker = pos ; walker < data_len ; walker ++ ) {
		
		if ( 0 == stack && sc_is_space(data [ walker ]) ) continue ;

		if ( '(' == data [ walker ] ) stack ++ ;
		if ( ')' == data [ walker ] ) stack -- ;

		buffer [ counter ] =  data [ walker ] ;

		counter ++ ;

		if ( 0 == stack ) break ;


	}

	buffer [ counter ] = '\0' ;

	return buffer ;

}

static MACRO* macro_find ( char* nname , int line ) {

	//	author : Jelo Wang
	//	since : 20090814
	//	(C)TOK
	
	//	(1) param : MACRO name 


	MACRO* walker = macro.next ;

	if ( !nname ) return 0 ;

	for ( ; walker ; walker = walker->next ) {
		if ( 0 == sc_strcmp ( walker->name , nname ) ) {
			if ( line > walker->line ) {
				return walker ;
			}
		}
	}

	return 0 ;
 
}


static char* subparambody ( char* param_body , char* subedstr , int body_len , int* walker ) {

	//	author : Jelo Wang
	//	since : 20090830
	//	(C)TOK

	//	marco (..,.....(.....)...,....,....) , return parameters of marco
	
	int counter = 0 ;
	
	subedstr = (char* ) SCMalloc ( body_len ) ;

	ASSERT(subedstr) ;
		
	for ( counter = 0 ; *walker < body_len ; counter ++ ) {

		if ( ',' == param_body [ *walker ] ) {
			
			*walker = *walker + 1 ;
			subedstr [ counter ] = C_EOS ;
			return subedstr ;
			
		} 
		
		//	Jelo Committed 20120724
		else if ( ')' == param_body [ *walker ] || '\0' == param_body [ *walker ] ) {
		//else if ( ')' == param_body [ *walker ] ) {
		//	Jelo Committed Finished
		
			subedstr [ counter ] = C_EOS ;
			return subedstr ;
			
		}

		subedstr [ counter ] = param_body [ *walker ] ;
		*walker = *walker + 1 ;

		//	macrco(..()..) inner bracket detected get it all
		if ( '(' == subedstr [ counter ] ) {
			counter ++ ;
			goto GETRECURSIVE ;
		}
		

	}

GETRECURSIVE:

	for ( ; *walker < body_len ; counter ++ ) {
		
		if ( ')' == param_body [ *walker ] || '\0' == param_body [ *walker ] ) {

			subedstr [ counter ] = param_body [ *walker ] ;
			subedstr [ counter + 1 ] = C_EOS ;
			*walker = *walker + 1 ;
			return subedstr ;
			
		}		

		subedstr [ counter ] = param_body [ *walker ] ;
		*walker = *walker + 1 ;
		
	}
	
	return subedstr ;
	
}

static char* macro_subsit ( MACRO* macro , MACRO* macrof , char* param_body , int line ) {

	//	author : Jelo Wang
	//	notes : read_macro
	//	since : 20090818	
	//	(C)TOK
	
	//	#define A B E F
	//	#define B bbbb F E
	//	#define E eeee
	//	#define F ffff

	//	results : 
	//	#define A bbbb ffff eeee eeee ffff
	//	#define B bbbb ffff eeee
	//	#define E eeee
	//	#define F ffff

	//	#define A B E F
	//	#define B bbbb F E
	//	#define E eeee
	//	#define F A ffff
	//	F
	//	results : bbbb F eeee eeee F

	//	#define A F
	//	#define F A ffff
	//	A
	//	F
	//	results : A ffff
	//	results : F ffff


	//	#define C(x) A ffff x F A F ff C(x) A B(x)
	//	#define A A FF A
	//	#define F A ffff
	//	#define B(x) C(x) B(x) x
	//	C(gtkings)
	
	//	results : A FF A  ffff gtkings A FF A  ffff  A FF A  A FF A  ffff  ff C(gtkings) A FF A 
	//	C(gtkings) B(gtkings) gtkings  

	int next_str = 0 ;
	int counter = 0 ;

	int param_body_len = sc_strlen ( param_body ) ;

	char* subedstr = 0 ;
	char* param_subedstr = 0 ;
	int psub_next = 0 ;
	
	char* bodystr = 0 ;

	MACRO* macro_finder = 0 ;
	LEXERC* macro_lexer = 0 ;

	//	save subsited macros here
	SCClString macrobody = { 0 , 0 , 0 , 0 , 0 } ; 
	SCClString body_reasult = { 0 , 0 , 0 , 0 , 0 } ; 
	
	if ( !macro )
		return 0 ;
	
	SCClStringInit ( &body_reasult ) ;

	SCClStringInit ( &macrobody ) ;
	SCClStringAddStr ( &macrobody , macro -> body.data ) ;

	if ( MACRO_FUNC == macro->type ) {
		
		//	gen a temp lexer to analysis marco's param

		//	macro function calling detected
		if ( '(' == param_body[0] ) {

			int walker = 0 ;

			for  ( walker = 0 ; walker < param_body_len ; ) {
				
				if ( '(' == param_body[walker]  ) {
					lexc->stack ++ ;
					
					//	macrco(..()..) , inner-bracket is part of macro-para donnot flit it here
					if ( 1 == lexc->stack ) { 
						walker ++ ;
						continue ;
					}

					
				}
				
				if ( ')' == param_body[walker] ) {
					lexc->stack -- ;
								
					if ( 0 == lexc->stack )
						break;	

					continue ;
				}
	
				subedstr = subparambody ( param_body , subedstr , param_body_len , &walker ) ;
				
				//	params of macro would saved by the follow format :
				//	xxx xxx xxx xx , the gaps between param's name is a blank ( + 1 for the blank )
				//	if presor macthed a param A then the next param in the stream = sc_strlen(A) + 1 

				param_subedstr = sc_substr_with_pos ( macro->param.token.data , param_subedstr, psub_next ) ;
				psub_next += sc_strlen ( param_subedstr ) + 1 ; 

				if ( subedstr && param_subedstr ) {

					SCClStringRepStrMultiEx ( &macrobody , param_subedstr , subedstr , '#' ) ;
				 
					counter ++ ;

					SCFree ( subedstr ) ;
					SCFree ( param_subedstr ) ;

				}


			}

			if ( lexc->stack ) 
				cerror ( C_PRESOR_MOD , IS_C_ERROR , "error :: bracket of a macro is not matched , macro : %s , line %d\n" , macro->name ,  lexc->line ) ;
			
			if ( lexc->stack ) 
				cerror ( C_PRESOR_MOD , IS_C_ERROR , "error ::  macro : %s has %d parameters , line %d\n" , macro->name , lexc->line ) ;

			lexc->stack = 0 ;

		} 

	}
	
	//	we need a lexer to walkthrough macro body here to lookup other marcos
	//	a recursive flow
	
	//	save the current lexer in SCClStack
	SCClStackPush ( &stack , (void* ) lexc ) ;
	
	//	get a new lexer
	macro_lexer = lexerc_new ( macrobody.data , LEXERC_DEFAULT_MODE ) ;
	lexerc_set ( macro_lexer ) ;

	while ( !lexc->stop ) {
		
		lexerc_genv() ;

		switch ( lexc->v ) {
			
			case C_FUNC_REF :
			case C_VAR_REF :

				macro_finder = macro_find ( lexc->token , line ) ;

				if ( macro_finder ) {

					if ( !sc_strcmp( macro->name , macro_finder->name ) ) {

						//	#define A ffff A
						//	if a macro is pointed by himself , we return it name here
						SCClStringAddStr ( &body_reasult , macro_finder->name ) ;
						continue ;

					} else if ( SCClStackLook ( &macro_stack , macro_finder->name ) ) {
						
						SCClString* s = 0 ; 

						if ( lexc->c ) SCClStringAdd ( &body_reasult , lexc->c ) ;
						else if( lexc->token ) SCClStringAddStr ( &body_reasult , lexc->token ) ;

						//	#define C(x) B(x)
						//	#define B(x) C(x)
						//	C(gtkings)

						//	if a circuit has detected we skiping it

						if ( MACRO_FUNC == macro_finder->type ) {
							s = skip_smart_brackets_scope () ; 
							
							SCClStringAddStr ( &body_reasult , s->data ) ;

							SCFree ( s ) ;
						}

						continue ;
				
					}
					
					SCClStackPush ( &macro_stack , (void* )macro_finder->name ) ;
					
					if ( MACRO_FUNC == macro_finder->type ) 
						bodystr = macro_subsit ( macro_finder , macro , get_macro_param(lexc->code->data,lexc->code->get_walker) , line ) ;
					else 
						bodystr = macro_subsit ( macro_finder , macro , macro_finder->body.data , line ) ;
					
					//	skip the body of macro's param
					//	#define C(x) B(x)
					//	#define B(x) x
					//	C(gtkings)
					//	skip (...) of B when B has subsited
					
					if ( macro_finder && MACRO_FUNC == macro_finder->type )
						skip_smart_brackets_scope () ; 

					if ( 0 == bodystr ) {
					
						SCClStringAddStr ( &body_reasult , macro_finder->name ) ;
	
					} else {

						SCClStringAddStr ( &body_reasult , bodystr ) ;


					}

				} else {
					
					SCClStringAddStr ( &body_reasult , lexc->token ) ;

				}

			break;

			default :
				
				if ( lexc->token ) {
					SCClStringAddStr ( &body_reasult , lexc->token ) ;
				} else if ( lexc->c ) {
					SCClStringAdd ( &body_reasult , lexc->c ) ;
				}  


		}


	}
	
	SCClStringAdd( &body_reasult , 0x20 ) ;
	SCClStringAdd( &body_reasult , '\0' ) ;
	
	lexerc_destroy () ;

	lexerc_set ( (LEXERC*) SCClStackPop ( &stack ) ) ;
	SCClStackPop ( &macro_stack ) ;

	return body_reasult.data ;

}


static void skip_macro  ()  {

	//	author : Jelo Wang
	//	notes : read_macro
	//	since : 20090812	
	//	(C)TOK
	
	if ( C_PI_DEFINE != lexc->v )
		return ;

	//	skip these junk streams that we donnt needed
	lexerc_skip_space () ;
	lexerc_genv () ;

	//	get marco param
	if ( C_XKL == lexc->v ) {

		lexc->stack ++ ;
	
		for  ( ; !lexc->stop ; lexerc_genv () )  {

//			if ( lexc->v == C_CHROW  && lexc->pv != C_ESCAPE )
//				break;
//			if ( lexc->v == C_ENTER  && lexc->pv != C_ESCAPE )
//				break;

			if ( lexc->v == C_ENTER )
				break;

			if ( C_XKL == lexc->v )
				lexc->stack ++ ;

			if ( C_XKR == lexc->v )
				lexc->stack -- ;

			if ( 0 == lexc->stack )
				break;
			
		}
		

	}

	lexerc_skip_space () ;
	
	for  ( ; !lexc->stop ; lexerc_genv () )  {

		if ( lexc->v == C_ENTER && lexc->pv != C_ESCAPE )
			break;

	}

	lexc->v = 0 ;
	lexc->c = 0 ;
	lexc->pv = 0 ;
	lexc->ppv = 0 ;
	lexc->token = 0 ;

}

static int read_macro () {

	//	author : Jelo Wang
	//	notes : read_macro
	//	since : 20090812
	//	(C)TOK
	
	int counter = 0 ;
	int tempinteger = 0 ;
	int subsited_tok_walker = 0 ;
	int subsited_tok_length = 0 ;

	MACRO* nmc = 0 ;

	if ( C_PI_DEFINE != lexc->v )
		return 0;

	nmc = (MACRO* ) SCMalloc ( sizeof(MACRO) ) ; 
	
	ASSERT(nmc) ;

	sc_memset ( nmc , 0 , sizeof(MACRO) ) ;
		
	nmc->param.totall = 0 ;
	
	SCClStringInit ( &(nmc->param.token) ) ;
		
	//	skip these junk streams that we donnt needed
	lexerc_skip_space () ;
	lexerc_genv () ;
	lexerc_skip_space () ;
	
	if ( macro_find ( lexc->token , lexc->line ) )
		SClog ( "error ! macro '%s' has already defined in file %s at line : %d\n" , lexc->token , lexc->file , lexc->line );
	
	nmc->name = (char* ) SCMalloc ( sc_strlen(lexc->token) ) ;

	//	Jelo Edited 2012-07-17
	ASSERT(nmc->name) ; 
	//if ( !nmc->name ) {
	//	SClog ( "error ! Not enough memory for macro name%s,%d\n",__FILE__,__LINE__);
	//	return 0 ;
	//}
	//	Jelo Edited Finished

	sc_strcpy ( nmc->name , lexc->token ) ;

	nmc->type = MACRO_CONST ;
	nmc->line = lexc->line ;
	
	//	get marco param
	if ( C_XKL == lexerc_head_genv (1) ) {
		
		lexerc_genv () ;
		lexc->stack ++ ;
	
		while ( !lexc->stop )  {
			
			//	break loop
			//	if lexc->v == ENTER and ESCAPE != lexc->pv
			//	exp : .....()

			//	For win32 text
			//if ( lexc->v == C_CHROW  && lexc->pv != C_ESCAPE )
			//	break;
			//	For unix text
			//if ( lexc->v == C_ENTER  && lexc->pv != C_ESCAPE )
			//	break;

			if ( lexc->v == C_ENTER )
				break;
			
  			lexerc_genv () ;

			if ( C_XKL == lexc->v )
				lexc->stack ++ ;

			if ( C_XKR == lexc->v )
				lexc->stack -- ;

			if ( 0 == lexc->stack )
				break;

			if ( C_VAR_REF == lexc->v ) {

				SCClStringAddStr ( &(nmc->param.token) , lexc->token ) ;
				SCClStringAddStr ( &(nmc->param.token) , " ") ;

				nmc->param.totall ++ ;

			}
			

		}
		
		if ( lexc->stack ) 
			cerror ( C_PRESOR_MOD , IS_C_ERROR , "error ! bracket of a macro is not matched , macro : %s , line %d\n" , nmc->name , lexc->line ) ;
		
		lexc->stack = 0 ;

		nmc->type = MACRO_FUNC ;

	}

	SCClStringInit ( &(nmc->body) ) ;
	
	while ( !lexc->stop ) {

		lexerc_genv () ;

		//	For win32 text
		//if ( lexc->v == C_CHROW  && lexc->pv != C_ESCAPE )
		//	break;
		//	For unix text
		//if ( lexc->v == C_ENTER  && lexc->pv != C_ESCAPE )
		//	break;

		if ( lexc->v == C_ENTER )
			break;

		//	ignore transferred symbol '\\'
		//	flow like : #define MAIN gtkings(a,b) gtkisngasdfsadf\maindsfni hao

		if ( '\\' == lexc->c ) {
			continue ;
		}
		
		if( 0 != lexc->token ) SCClStringAddStr ( &(nmc->body) , lexc->token ) ;
		//	for #define name(cont) Hello##cont##There
		else if ( '#' == lexc->pc ) SCClStringAdd ( &(nmc->body) , lexc->pc ) ;
		else if ( 0 != lexc->c ) SCClStringAdd ( &(nmc->body) , lexc->c ) ;


	}

	SCClStringAdd ( &(nmc->body) , '\0' ) ;
		
	if ( lexc->file ) {
		
		nmc->file = (char* ) SCMalloc ( sc_strlen(lexc->file) + 1 ) ;

		//	Jelo Edited 2012-07-17
		ASSERT(nmc->file) ; 
		//if ( !nmc->file ) {
		//	SClog ( "[sc][c-presor][read_macro] not enough memory %s,%d\n",__FILE__,__LINE__);
		//	return 0 ;
		//}
		//	Jelo Edited Finished
		
		sc_strcpy ( nmc->file , lexc->file ) ;
		
	}

	macro_insert ( nmc ) ;
	
	return 1 ;

}

static void read_include () {

	//	author : Jelo Wang
	//	notes : read_include
	//	since : 20090831
	//	(C)TOK
	
	void* file = 0 ;

	char* buffer = 0 ;
	char abpath [ MAX_PATH ] = {0};

	int walker = 0 ;
	int fillen = 0 ;

	if ( C_PI_INCLUDE != lexc->v ) 
		return ;

	lexerc_skip_space () ;
	lexerc_genv () ;
	
	if ( C_STR == lexc->v ) {

//	Jelo Edited 20120313
//		int walker = 0 ;		
//		for ( ; walker < sc_strlen(lexc->token) ; walker ++ ) {
//			if ( '"' == lexc->token [ walker + 1 ] )
//				lexc->token [ walker + 1 ] = 0 ;
//			lexc->token [ walker ] = lexc->token [ walker + 1 ] ;
//		}
//	Jelo Finished

		sc_strcpy ( abpath , SCHalGetFilePath(lexc->token) ) ;

		file = SCHalFileOpen ( abpath , "rb" ) ;
			
		if ( !file ) {
			
			cerror ( C_PRESOR_MOD , IS_C_ERROR , "Can not find the file '%s' , line : %d\n" , lexc->token , lexc->line ) ;
			
			goto end ;
			
		}
		
		SCHalFileSeek ( file , 0 , SEEK_HEAD ) ;
		fillen = SCHalFileLength( file ) ;
		buffer = (char* ) SCMalloc ( fillen ) ;

		//	Jelo Edited 2012-07-17
		ASSERT(buffer) ;
		//if ( !buffer ) {
		//	SClog ( "Not enough memory %s , %d\n",__FILE__,__LINE__);
		//	SCHalFileClose ( file ) ;
		//	return ;
		//}
		//	Jelo Edited Finished

		for ( walker = lexc->code->get_walker ; walker >= 0 && '#' != lexc->code->data [walker] ; walker -- ) {
			lexc->code->data [walker] = 0x20 ;
		}

		if ( walker >= 0 && '#' == lexc->code->data [walker] ) lexc->code->data [walker] = 0x20 ;
	
		memset ( buffer , 0 , fillen ) ;
		SCHalFileSeek ( file , 0 , SEEK_HEAD ) ;		
		SCHalFileRead ( file , buffer , 1 , fillen ) ;

		SCClStringInsert ( lexc->code , buffer , walker ) ;

		lexc->code->get_walker = walker ;

		SCHalFileClose ( file ) ;

		SCFree ( buffer ) ;

		//	Jelo Edited 2011-07-17
		//lexerc_backup () ;
		//precompiling () ;
		//lexerc_rollback () ;
		//	Jelo Edited Finished
			
	} else if ( '<' == lexc->token [0] ) {

		//	desnt support #inlcude <...> yet
		//	Jelo Edited 2011-07-17
 		#if 0
		int walker = 0 ;
		char path [ MAX_PATH ] ;
		
		for ( walker = 0 ; lexc->code->get_walker < lexc->code->length ; walker ++ ) {
		
			if ( '>' == lexc->code->data [ lexc->code->get_walker ] )
				break ;

			path [ walker ] =  lexc->code->data [ lexc->code->get_walker ] ; 
			lexc->code->get_walker ++;


		}

		path [ walker ] = '\0' ;

		sc_strcpy ( abpath , SCHalGetFilePath(path) ) ;
		#endif
		//	Jelo Edited Finished

	}

end :

	lexerc_clear_status () ;

}


static int read_ifdef () {

	//	author : Jelo Wang
	//	notes : read ifdef precompiling instructions
	//	since : 200909022
	//	(C)TOK

	int key_pos = 0 ;
	int key_scope = 0 ;
	int lexer_walker = 0;
	
	if ( C_PI_IFDEF != lexc->v ) 
		return 0 ;

	//	length of token "#ifdef" save it starting pos here
	key_pos = lexc->code->get_walker - 6 ; //sc_strlen("#ifdef") ;

	lexerc_skip_space () ;

	lexerc_genv () ;	

	key_scope = lexc->code->get_walker - key_pos ;
	
	if ( macro_find ( lexc->token , lexc->line ) ) {

		SCClStringReplaireAtom ( lexc->code , 0x20 , key_pos , key_scope ) ;
		lexc->stack ++ ;

redo :
		
		while ( !lexc->stop && 
			C_PI_ENDIF != lexc->v && 
			C_PI_IFDEF != lexc->v && 
			C_PI_IFNDEF != lexc->v && 
			C_PI_IF != lexc->v && 
			C_PI_DEFINE != lexc->v ) 
		{
			lexerc_genv () ;
		}

		if ( C_PI_ENDIF == lexc->v ) {

			SCClStringReplaireAtom ( lexc->code , 0x20 , lexc->code->get_walker - 6 , 6 ) ;
			lexc->stack -- ;
			
			//	clear lexc->v
			//	this is a recursive function , when recursive is rollback we need a new value of lex
			lexc->v = 0 ;

		} else if ( C_PI_IFDEF == lexc->v ) {
		
			read_ifdef () ;

			goto redo ;
		
		} else if ( C_PI_IFNDEF == lexc->v ) {

			
			read_ifndef () ;

			goto redo ;
			
		} else if ( C_PI_IF == lexc->v ) {
			
			read_if () ;

			goto redo ;
			
		} else if ( C_PI_DEFINE == lexc->v ) {

			read_macro () ;

			goto redo ;
		}

	} else {
		
		int stack = 0 ;
		stack ++ ;

		while ( !lexc->stop ) {
			
			lexerc_genv () ;
			
			if ( C_PI_IFDEF == lexc->v ) stack ++ ;
			else if ( C_PI_IFNDEF == lexc->v ) stack ++ ;
			else if ( C_PI_IF == lexc->v ) stack ++ ;
			else if ( C_PI_ENDIF == lexc->v ) stack -- ;

			if ( 0 == stack ) break ; 

		}
		
		lexc->v = 0 ;
		SCClStringReplaireAtom ( lexc->code , 0x20 , key_pos , lexc->code->get_walker - key_pos ) ;

	}

	return 1 ;

}

static int read_ifndef () {

	//	author : Jelo Wang
	//	notes : read ifndef precompiling instructions
	//	since : 200909026
	//	(C)TOK

	int key_pos = 0 ;
	int key_scope = 0 ;

	if ( C_PI_IFNDEF != lexc->v ) 
		return 0 ;

	//	length of token "#ifdef" save it starting pos here
	//	sc_strlen("#ifndef") == 7
	key_pos = lexc->code->get_walker - 7 ; 

	lexerc_skip_space () ;

	lexerc_genv () ;	

	key_scope = lexc->code->get_walker - key_pos ;
	
	if ( !macro_find ( lexc->token , lexc->line ) ) {

		SCClStringReplaireAtom ( lexc->code , 0x20 , key_pos , key_scope ) ;
		lexc->stack ++ ;

redo :
		
		while ( !lexc->stop && 
			C_PI_ENDIF != lexc->v && 
			C_PI_IFDEF != lexc->v && 
			C_PI_IFNDEF != lexc->v && 
			C_PI_IF != lexc->v && 
			C_PI_DEFINE != lexc->v ) 
		{
			lexerc_genv () ;
		}

		if ( C_PI_ENDIF == lexc->v ) {

			SCClStringReplaireAtom ( lexc->code , 0x20 , lexc->code->get_walker - 6 , 6 ) ;
			lexc->stack -- ;
			
			//	clear lexc->v
			//	this is a recursive function , when recursiving is rollback we need a new value of lex
			lexc->v = 0 ;

		} else if ( C_PI_IFDEF == lexc->v ) {
		
			read_ifdef () ;

			goto redo ;
		
		} else if ( C_PI_IFNDEF == lexc->v ) {
			
			read_ifndef () ;

			goto redo ;
			
		} else if ( C_PI_IF == lexc->v ) {

			
			read_if () ;

			goto redo ;
			
		} else if ( C_PI_DEFINE == lexc->v ) {

			read_macro () ;

			goto redo ;
		}


	} else {
		
		int stack = 0 ;
		stack ++ ;

		while ( !lexc->stop ) {
			
			lexerc_genv () ;

			if ( C_PI_IFDEF == lexc->v ) stack ++ ;
			else if ( C_PI_IFNDEF == lexc->v ) stack ++ ;
			else if ( C_PI_IF == lexc->v ) stack ++ ;
			else if ( C_PI_ENDIF == lexc->v ) stack -- ;

			if ( 0 == stack ) break ; 

		}
		
		lexc->v = 0 ;
		SCClStringReplaireAtom ( lexc->code , 0x20 , key_pos , lexc->code->get_walker - key_pos ) ;

	}

	return 1 ;

}

static int read_if () {


	//	author : Jelo Wang
	//	notes : read if precompiling instruction
	//	since : 200909026
	//	(C)TOK


	int key_pos = 0 ;
	
	if ( C_PI_IF != lexc->v ) 
		return 0 ;

	//	length of token "#if" save it starting pos here
	//	sc_strlen("#if") == 3
	key_pos = lexc->code->get_walker - 3 ; 

	lexerc_skip_space () ;

	if ( 0 != c_evalor () ) {

		SCClStringReplaireAtom ( lexc->code , 0x20 , key_pos , lexc->code->get_walker - key_pos ) ;

redo :
		
		while ( !lexc->stop && 
			C_PI_ENDIF != lexc->v && 
			C_PI_IFDEF != lexc->v && 
			C_PI_IFNDEF != lexc->v && 
			C_PI_IF != lexc->v && 
			C_PI_DEFINE != lexc->v ) 
		{
			lexerc_genv () ;
		}

		if ( C_PI_ENDIF == lexc->v ) {

			SCClStringReplaireAtom ( lexc->code , 0x20 , lexc->code->get_walker - 6 , 6 ) ;
			lexc->stack -- ;
			
			//	clear lexc->v
			//	this is a recursive function , when recursiving is rollback we need a new value of lex
			lexc->v = 0 ;

		} else if ( C_PI_IFDEF == lexc->v ) {
		
			read_ifdef () ;

			goto redo ;
		
		} else if ( C_PI_IFNDEF == lexc->v ) {

			
			read_ifndef () ;

			goto redo ;
			
		} else if ( C_PI_IF == lexc->v ) {

			
			read_if () ;

			goto redo ;
			
		} else if ( C_PI_DEFINE == lexc->v ) {

			read_macro () ;

			goto redo ;
		}
		

	} else {

		int stack = 0 ;
		stack ++ ;

		while ( !lexc->stop ) {
			
			lexerc_genv () ;

			if ( C_PI_IFDEF == lexc->v ) stack ++ ;
			else if ( C_PI_IFNDEF == lexc->v ) stack ++ ;
			else if ( C_PI_IF == lexc->v ) stack ++ ;
			else if ( C_PI_ENDIF == lexc->v ) stack -- ;

			if ( 0 == stack ) break ; 

		}
		
		lexc->v = 0 ;
		SCClStringReplaireAtom ( lexc->code , 0x20 , key_pos , lexc->code->get_walker - key_pos ) ;

	}
	
	return 1 ;
	
}

static int read_else () {

	//	author : Jelo Wang
	//	since : 20120314
	//	(C)TOK

	int stack = 0 ;
	int key_pos = 0 ;

	if ( C_PI_ELSE != lexc->v ) 
		return 0 ;

	//	length of token "#else" save it starting pos here
	key_pos = lexc->code->get_walker - 5 ; //sc_strlen("#else") ;
	SCClStringReplaireAtom ( lexc->code , 0x20 , key_pos , lexc->code->get_walker - key_pos ) ;
	
	stack ++ ;

	while ( !lexc->stop ) {
			
		lexerc_genv () ;
			
		if ( C_PI_IFDEF == lexc->v ) stack ++ ;
		else if ( C_PI_IFNDEF == lexc->v ) stack ++ ;
		else if ( C_PI_IF == lexc->v ) stack ++ ;
		else if ( C_PI_ENDIF == lexc->v ) stack -- ;

		if ( 0 == stack ) break ; 

	}
		
	lexc->v = 0 ;
	key_pos = lexc->code->get_walker-5 ; //sc_strlen("#else") ;
	SCClStringReplaireAtom ( lexc->code , 0x20 , key_pos , lexc->code->get_walker - key_pos ) ;

	return 1 ;

}


static void precompiling () {

	//	author : Jelo Wang
	//	notes : read precompiling instructions
	//	since : 20090817
	//	(C)TOK

	lexerc_ready () ;
	lexerc_genv () ; 

	//	Format win32 text to unix text
	//	use lexer plz!
	//SCClStringRepStrMulti ( lexc->code , "\r\n" , "\n" ) ;

 	for ( ; !lexc->stop ; lexerc_genv () ) {

		switch ( lexc->v ) {
			
			case C_PI_INCLUDE :
				read_include () ;
			break ;
			
			case C_PI_DEFINE :
				read_macro () ;				
			break ;
			
			case C_PI_IFDEF :
				read_ifdef () ;
			break ;
			
			case C_PI_IFNDEF :
				read_ifndef () ;
			break ;
			
			case C_PI_IF :
				read_if () ;
			break ;

		}

	}

}

int presor_c_run ( char* presor_file ) {

	//	author : Jelo Wang
	//	since : 20090809
	//	(C)TOK

	//	notes : preprocessor of c language

	MACRO* macro_finder = 0 ;
	SCClString* presor_results = 0 ;

	char* subed = 0 ;
	int line = 0 ;
	
	void* file = 0 ;

	presor_results = SCClStringNew () ; 
	ASSERT(presor_results) ;
	
	precompiling () ;
	
	SCClStringInit ( presor_results ) ;
	
	SCClStackInit ( &stack ) ;
	SCClStackInit ( &macro_stack ) ;

	//	marco replairing
 	for ( lexerc_ready () ; !lexc->stop ; ) {

		lexerc_genv () ;
	
		skip_macro () ;

		if ( C_VAR_REF == lexc->v || C_FUNC_REF == lexc->v ) {
			
			macro_finder = macro_find ( lexc->token , lexc->line ) ;
			
			if ( macro_finder ) {
		
				SCClStackPush ( &stack , (void* ) lexc ) ;
				SCClStackPush ( &macro_stack , (void* )macro_finder->name ) ;

				//#define C A ffff F A F ff C A
				//#define A A FF A
				//#define F A ffff
				//C
				//	when C called A F is defined
				subed = macro_subsit ( macro_finder , macro_finder , get_macro_param ( lexc->code->data , lexc->code->get_walker ) , lexc->line ) ;
 				if ( MACRO_FUNC == macro_finder->type ) skip_smart_brackets_scope () ; 
				lexerc_set ( (LEXERC* ) SCClStackPop ( &stack ) ) ;
				SCClStackDestroy ( &macro_stack ) ;
 
			}
			
			 
		} 

		if ( subed ) { SCClStringAddStr( presor_results , subed ) ; subed = 0 ; }
		else if( 0 != lexc->token ) SCClStringAddStr ( presor_results , lexc->token ) ;
		else if ( 0 != lexc->c ) SCClStringAdd ( presor_results , lexc->c ) ;

	}

	SCClStringAdd ( presor_results , '\0' ) ;
	 
	line = lexc->line ;

	//	when precompling process is done gen a new lexer here for the back-para parser
 	lexerc_destroy () ;
	lexerc_set ( lexerc_new ( presor_results->data , LEXERC_DEFAULT_MODE ) ) ;

	lexc->line = line ;
	
	if ( SC_PO & semo->parameter ) {

		//	output precompiling results
		file = SCHalFileOpen ( presor_file , "w+" ) ; 
 
		SCHalFileWrite ( file , presor_results->data , 1 , presor_results->length ) ;

		SCHalFileClose ( file ) ;

		//	release it here
		SCFree ( presor_file ) ;

	}	
	
 	SCClStringDestroy ( presor_results ) ;

	macro_destroy () ;

	return 1 ;


}

