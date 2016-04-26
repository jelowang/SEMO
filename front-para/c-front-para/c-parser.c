
/*

+	PARSER , part of SC front-para

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

# include "c-errors.h"
# include "c-lexer.h"
# include "c-grammar.h"
# include "c-parser.h"
# include "evalor.h"
# include "sccl.h"
# include "schal.h"
# include "symbole.h"
# include "lgnosia.h"

PARSERC* parserc = 0 ;
SCClStack cfstack = { 0 , 0 , 0 , 0 , 0 } ;

# define NUMERIC_FLOW(x) ((x==C_INT_NUM) ||(x==C_FLT_NUM) ||(x==C_HEX_NUM)||(x==C_INT_ENUM)||(x==C_FLT_ENUM))
# define OPERAND_FLOW(x) (NUMERIC_FLOW(x)||(x==C_VAR_REF)||(x==C_ARRAY_REF)||(x==C_FUNC_REF))
# define DHEAD_FLOW(flow) ( C_AUTO <= flow && flow <= C_DOUBLE )

# define DUA_OPERATOR(x) (C_SUB <= x && x <= C_HE)
# define CMA_OPERATOR(x) ((x==C_DOU))

//	prior operator
# define PORL_OPERATOR(x) (x==C_XKL)
# define PORR_OPERATOR(x) (x==C_XKR)

# define RESET_PARSER()\
	parserc->head = -1 ;\
	parserc->form = -1 ;\
	parserc->scope = -1 ;\
	SCClStackDestroy ( &parserc->scopestack ) ;\

# define MATCH_PARSER_SCOPE(sscope)\
	(sscope==SCClStackGet( &parserc->scopestack ))

# define SET_PARSER_SCOPE(sscope)\
	SCClStackPush ( &parserc->scopestack , (void* )sscope ) ;\
	parserc->scope = sscope ;\

# define UNSET_PARSER_SCOPE()\
	SCClStackPop ( &parserc->scopestack ) ;\
	parserc->scope = (int) SCClStackGet ( &parserc->scopestack ) ;\

# define SCOPER_PUSH()\
	parserc->stack ++ ;\

# define SCOPER_POP()\
	parserc->stack -- ;\

# define SCOPER_GET() parserc->stack

static int parserc_azonal_type ( int headbit ) {


	//	author : Jelo Wang
	//	since : 20121030
	//	(C)TOK

	if ( C_FLOAT_BIT != ( headbit & C_FLOAT_BIT ) ) {
		return AZONAL_INT ;
	} else {
		return AZONAL_FLOAT ;
	}

}

static int parserc_azonal_size ( int headbit ) {


	//	author : Jelo Wang
	//	since : 20121030
	//	(C)TOK

	int size = 0 ;
	
	if ( C_VOID_BIT == headbit ) {
		size = 4 ;
	} else if ( C_CHAR_BIT == headbit ) {
		size = 1 ;
	} else if ( C_INT_BIT == headbit ) {
		size = 4 ;
	} else if ( C_FLOAT_BIT == headbit ) {
		size = 4 ;
	} else if ( C_DOUBLE_BIT == headbit ) {
		size = 4 ;
	} else if ( C_SHORT_BIT == headbit ) {
		size = 2 ;
	} else if ( C_LONG_BIT == headbit ) {
		size = 4 ;
	} else if ( C_LONG_LONG_BIT == headbit ) {
		size = 4 ;
	} else if ( C_REGISTER_BIT == headbit ) {
		size = 4 ;	
	} 

	return size ;

}


static void parserc_ready () {

	parserc = (PARSERC* ) SCMalloc ( sizeof(PARSERC) ) ;

	if ( !parserc ) {

		SClog ("[sc][c-front-para][parserc] initialization has failed\n") ;
		SCTerminate () ;
		return ;
		
	}
	
	parserc->form = -1 ;
	parserc->head = -1 ; 
	parserc->scope = 0 ;
	parserc->stop = 0 ;
	parserc->stack = 0 ;
	
	SCClStackInit ( &parserc->scopestack ) ;
	
}

static int parserc_genv () {

	//	author : Jelo Wang
	//	since : 20091128
	//	(C)TOK
	
	int state = 0 ;

	for ( ; !lexc->stop ; ) {

		switch ( state ) {
			
			case 0 :

				if ( DHEAD_FLOW ( lexc->v ) ) {
					parserc->head = lexc->v ;
					state = 0 ;
				} else if ( C_VAR_REF == lexc->v ) {
					parserc->form = lexc->v ;
					return 1 ;
				} else if ( C_FUNC_DEF == lexc -> v ) {
					parserc->form = lexc->v ;
					return 1 ;
				}

				lexerc_genv () ;
				
			break ;
			
		}
	}

}

static void parser_c_needs ( int lexv , char* symbol ) {

	//	author : Jelo Wang
	//	since : 20110211
	//	(C)TOK

	if ( lexv != lexc->v ) {
		cerror ( C_PARSER_MOD , IS_C_ERROR , "needs '%s' , line : %d\n" , symbol , lexc->line ) ;		
	}
	
}

static int parser_c_read_function () {

	//	author : Jelo Wang
	//	notes : read function 
	//	since : 20100116
	//	(C)TOK
	
	int stack = 0 ;

	AZONAL* azonal = 0 ;
	int lgnosia = 0 ;

	if ( C_FUNC_DEF != lexc->v )
		return 0 ;

	azonal = SymboleFindFunction ( lexc->token ) ;
	
	if ( azonal ) cerror ( C_PARSER_MOD , IS_C_ERROR , "function '%s' multi-defined , line : %d\n" , lexc->token , lexc->line ) ;
	else azonal = SymboleAddFunction ( lexc->token , ISA_FUNCTION , parserc->head , lexc->line ) ;
	
	//	create a LGA
	lgnosia = LgnosiaNew ( (int) azonal , LGNOSIA_TOP_IDENT ) ;

	//	parsing parameter scope of function
	stack = 0 ;
 	SET_PARSER_SCOPE(PARSERC_SCOPE_PARAM);
	while ( !lexc->stop ) {
		
		lexerc_genv () ;
		
		if ( C_XKL == lexc->v )
			stack ++ ;
		if ( C_XKR == lexc->v )
			stack -- ;
		
		if ( 0 == stack ) 
			break ;

		parser_c_read_symbol_def () ;
		
	}
	UNSET_PARSER_SCOPE();
	
// 	if ( lexc->stop || stack ) {
//		cerror ( C_PARSER_MOD , IS_C_ERROR , "'()' is unmatched , line : %d\n" , lexc->line ) ;
//		RESET_PARSER () ;
//		return 1 ;
//	}
	
	LgnosiaStackPush ( (int) lgnosia ) ;
	
	//	parsing function body
	stack = 0 ;
	SCOPER_PUSH () ;
	while ( !lexc->stop ) {
		
		lexerc_genv () ;
		
		if ( C_DKL == lexc->v )
			stack ++ ;
		if ( C_DKR == lexc->v )
			stack -- ;
		
		if ( 0 == stack ) 
			break ;		
		
		parser_c_read_controlflow () ;
		parser_c_read_symbol_def () ;
		parser_c_read_symbol_inf () ;

	}

	LgnosiaStackPop () ;
	SCOPER_POP () ;
		
	if ( lexc->stop ) {
		cerror ( C_PARSER_MOD , IS_C_ERROR , "'{}' is unmatched , line : %d\n" , lexc->line ) ;
		RESET_PARSER () ;
		return 1 ;
	}

}

static int parser_c_read_funccal () {

	//	author : Jelo Wang
	//	since : 20110211
	//	(C)TOK

	int stack = 0 ;
	int totall_para = 1 ;
	int have_para = 1 ;

	AZONAL* fcalanl = 0 ;
	AZONAL* azonal = 0 ;
	int lgnosia = 0 ;
	
	if ( C_FUNC_REF != lexc->v ) 
		return 0 ;

	azonal = SymboleFindFunction ( lexc->token ) ;
	
	if ( !azonal ) {
		cerror ( C_PARSER_MOD , IS_C_ERROR , "! - function '%s' is not defined , line : %d\n" , lexc->token , lexc->line ) ;
		parser_c_skip_parenthesis_scope () ;
		if ( 0 == MATCH_PARSER_SCOPE(PARSERC_SCOPE_PARAM) ) {
			if ( C_FEN != lexerc_head_genv (1) ) {
				parser_c_needs ( C_FEN , ";" ) ;
			}
		}
		return 0 ;
	}

	fcalanl = SymboleAddFunctionCall ( azonal->name , ISA_FUNCCAL , azonal->tack.totall , lexc->line ) ;
	
	//	check is there has some parameters exits of the calling.
	if ( C_XKR == lexerc_head_genv (2) ) {
		have_para = 0 ;
		//	send an error message if there hasnt parameters but azonal has.
		if ( 0 < azonal->tack.totall ) {
			cerror ( C_PARSER_MOD , IS_C_ERROR , "! - function '%s' needs parameters , line : %d\n" , lexc->token , lexc->line ) ;
		}
	}

	//	create a LGA
	lgnosia = LgnosiaNew ( (int) fcalanl , LGNOSIA_SYM_IDENT ) ;	
	LgnosiaAddContext ( LgnosiaStackTop () , lgnosia , LGNOSIA_SYM_IDENT ) ;
	LgnosiaStackPush ( lgnosia ) ;	
	while ( !lexc->stop ) {
		
		lexerc_genv () ;
		
		if ( C_XKL == lexc->v )
			stack ++ ;
		else if ( C_XKR == lexc->v )
			stack -- ;
		else if ( C_DOU == lexc->v )
			totall_para ++ ;
			
		if ( 0 == stack ) 
			break ;

		parser_c_read_symbol_inf () ;
		
	}
	LgnosiaStackPop () ;

	//	check totall parameters and data type
	if ( 0 < have_para ) {
		if ( totall_para != azonal->tack.totall ) {
			cerror ( C_PARSER_MOD , IS_C_ERROR , "! - function '%s' needs %d parameters , line : %d\n" , azonal->name , azonal->tack.totall , lexc->line ) ;
		}		
	}
	
	if ( 0 == MATCH_PARSER_SCOPE(PARSERC_SCOPE_PARAM) ) {
		if ( C_FEN != lexerc_head_genv (1) ) {
			parser_c_needs ( C_FEN , ";" ) ;
		}
	}

	return 1 ;
	
}

static int parser_c_read_if () {

	//	author : Jelo Wang
	//	since : 20100116
	//	(C)TOK

	int stack = 0 ;

	EVALOR* evalor = 0 ;
	AZONAL* azonal = 0 ;
	int lgnosia = 0 ;
	
	if ( C_IF != lexc->v ) 
		return 0 ;

	//	create control flow azonal
	azonal = SymboleAddControlAzonal ( ISA_IFCF , lexc->line ) ;
	
	//	create LGA
	lgnosia = LgnosiaNew ( (int)azonal , LGNOSIA_CP_IDENT ) ;
			
	LgnosiaAddContext ( LgnosiaStackTop () , (int) lgnosia , LGNOSIA_CP_IDENT ) ;
		
	SET_PARSER_SCOPE(PARSERC_SCOPE_PARAM);
	//	parsing parameter scope of controlflow
	//	Get an evalor
	evalor = EvalorCreate () ;
	EvalorSetCurrent ( evalor ) ;
	//	parsing EXPR flow
	parser_c_read_expr () ;
	EvalorFinish () ;
	
	//	add parameter for the LGA.
	LgnosiaAddParameter ( lgnosia , EvalorGetTree () , LGNOSIA_EXP_IDENT ) ;			
	UNSET_PARSER_SCOPE();

	//	Generate po_chain
	LgnosiaStackPush ( LgnosiaGenPOC ( lgnosia , 0 ) ) ;

	//	push if-flow
	//	we use this for ne_chain generating
	SCClStackPush ( &cfstack , (void* )lgnosia ) ;
	SCClStackPush ( &cfstack , (void* )parserc->stack ) ;

	//	parsing body
	stack = 1 ;
	SCOPER_PUSH () ;
	while ( !lexc->stop ) {

		lexerc_genv () ;

		if ( C_DKL == lexc->v )
			stack ++ ;
		if ( C_DKR == lexc->v )
			stack -- ;
		
		if ( 0 == stack ) 
			break ;		

		parser_c_read_controlflow () ;
		parser_c_read_symbol_def () ;
		parser_c_read_symbol_inf () ;
			

	}

	LgnosiaStackPop () ;
	SCOPER_POP () ;	
		
	if ( stack ) {
		cerror ( C_PARSER_MOD , IS_C_ERROR , "'{}' is unmatched , line : %d\n" , lexc->line ) ;
		RESET_PARSER () ;
		return 1 ;
	}	


}

static int parser_c_read_else () {

	//	author : Jelo Wang
	//	since : 20100502
	//	(C)TOK

	int stack = 0 ;
	int elseif = 0 ;
	
	EVALOR* evalor = 0 ;
	AZONAL* azonal = 0 ;
	int lgnosia = 0 ;
	int FatherLga = 0 ;
	int ne_chain = 0 ;
	
	if ( C_ELSE != lexc->v ) 
		return 0 ;

	if ( C_IF == lexerc_head_genv(1) ) {

		lexerc_genv () ;
		elseif = 1 ;
		//	create control flow azonal
		azonal = SymboleAddControlAzonal ( ISA_ELSEIFCF , lexc->line ) ;

		
	} else {
		elseif = 0 ;
		azonal = SymboleAddControlAzonal ( ISA_ELSECF , lexc->line ) ;
	}
	

	{
	
		//	generate ne_chain of LGNOSIA_CP_IDENT.
		
		int deep = (int )SCClStackPop ( &cfstack ) ;
		int lga = (int )SCClStackPop ( &cfstack ) ;

		while ( deep > parserc->stack ) {
			
			if ( SCClStackEmpty(&cfstack) ) break ; 

			deep = (int )SCClStackPop ( &cfstack ) ;
			lga = (int )SCClStackPop ( &cfstack ) ;

		}

		ne_chain = LgnosiaGenNEC ( lga , azonal ) ;
//		LgnosiaStackPush ( ne_chain ) ;
	
	}

	if ( 1 == elseif ) {

		//	create LGA for the else-if flow
		lgnosia = LgnosiaNew ( (int)azonal , LGNOSIA_CP_IDENT ) ;
		//	add context of ne_chain
		LgnosiaAddContext ( ne_chain, (int) lgnosia , LGNOSIA_CP_IDENT ) ;

		SET_PARSER_SCOPE(PARSERC_SCOPE_PARAM);
		//	parsing parameter scope of controlflow
		//	Get an evalor
		evalor = EvalorCreate () ;
		EvalorSetCurrent ( evalor ) ;
		//	parsing EXPR flow
		parser_c_read_expr () ;
		EvalorFinish () ;
		
		//	add parameter for the LGA.
		LgnosiaAddParameter ( lgnosia , EvalorGetTree () , LGNOSIA_EXP_IDENT ) ;			
		UNSET_PARSER_SCOPE();
		
		//	Generate po_chain
		LgnosiaStackPush ( LgnosiaGenPOC ( lgnosia , azonal ) ) ;

		//	push if-flow
		//	we use this for ne_chain generating
		SCClStackPush ( &cfstack , (void* )lgnosia ) ;
		SCClStackPush ( &cfstack , (void* )parserc->stack ) ;

		//	parsing body
		stack = 1 ;
	
	} else {		

		LgnosiaStackPush ( ne_chain ) ;
		stack = 0 ;
	}
	
	SCOPER_PUSH () ;
	while ( !lexc->stop ) {

		lexerc_genv () ;

		if ( C_DKL == lexc->v )
			stack ++ ;
		if ( C_DKR == lexc->v )
			stack -- ;
		
		if ( 0 == stack ) 
			break ;		

		parser_c_read_controlflow () ;
		parser_c_read_symbol_def () ;
		parser_c_read_symbol_inf () ;
			

	}
	LgnosiaStackPop () ;
	SCOPER_POP () ;	

	if ( 1 == elseif ) { 
		//	push else-if flow
		LgnosiaQueueEnter ( lgnosia ) ;
	}

	if ( stack ) {
		cerror ( C_PARSER_MOD , IS_C_ERROR , "'{}' is unmatched , line : %d\n" , lexc->line ) ;
		RESET_PARSER () ;
		return 1 ;
	}	


}

static int parser_c_read_while () {

	//	author : Jelo Wang
	//	since : 20100502
	//	(C)TOK

	int stack = 0 ;

	EVALOR* evalor = 0 ;
	AZONAL* azonal = 0 ;
	int lgnosia = 0 ;
	
	if ( C_WHILE != lexc->v ) 
		return 0 ;

	//	create control flow azonal
	azonal = SymboleAddControlAzonal ( ISA_WHILECF , lexc->line ) ;
		
	//	create LGA
	lgnosia = LgnosiaNew ( (int)azonal , LGNOSIA_CP_IDENT ) ;

	LgnosiaAddContext ( LgnosiaStackTop () , (int) lgnosia , LGNOSIA_CP_IDENT ) ;
	LgnosiaStackPush ( lgnosia ) ;
	
	SET_PARSER_SCOPE(PARSERC_SCOPE_PARAM);
	//	parsing parameter scope of controlflow
	//	Get an evalor
	evalor = EvalorCreate () ;
	EvalorSetCurrent ( evalor ) ;
	//	parsing EXPR flow
	parser_c_read_expr () ;
	EvalorFinish () ;
	
	//	add parameter for the LGA.
	LgnosiaAddParameter ( lgnosia , EvalorGetTree () , LGNOSIA_EXP_IDENT ) ;			
	UNSET_PARSER_SCOPE();

	//	parsing body
	stack = 1 ;
	SCOPER_PUSH () ;
	while ( !lexc->stop ) {

		lexerc_genv () ;

		if ( C_DKL == lexc->v )
			stack ++ ;
		if ( C_DKR == lexc->v )
			stack -- ;
		
		if ( 0 == stack ) 
			break ;		

		parser_c_read_controlflow () ;
		parser_c_read_symbol_def () ;
		parser_c_read_symbol_inf () ;
			

	}

	LgnosiaStackPop () ;
	SCOPER_POP () ;	
	
	if ( stack ) {
		cerror ( C_PARSER_MOD , IS_C_ERROR , "'{}' is unmatched , line : %d\n" , lexc->line ) ;
		RESET_PARSER () ;
		return 1 ;
	}	


}


static int parser_c_read_controlflow () {

	//	author : Jelo Wang
	//	since : 20100116
	//	(C)TOK

	switch ( lexc -> v ) {

		case C_IF :
			parser_c_read_if () ;
		break ;

		case C_ELSE :
			parser_c_read_else () ;
		break ;

		case C_FOR :
		break ;

		case C_WHILE :
			parser_c_read_while () ;
		break ;

		case C_DO :
		break ;

		case C_SWITCH :
		break ;

		default :
			return 0 ;

	}

}

static int parser_c_is_controlflow ( int head ) {

	//	author : Jelo Wang
	//	since : 20100116
	//	(C)TOK

	switch ( head ) {

		case C_IF :
		case C_FOR :
		case C_WHILE :
		case C_DO :
		case C_SWITCH :
			return 1 ;

		default :
			return 0 ;

	}


}

static int parser_c_read_symbol_inf () {

	//	author : Jelo Wang
	//	since : 20100202
	//	(C)TOK


	switch ( lexc->v ) {
		case C_VAR_REF :
			return parser_c_read_variable_inf () ;
		break ;
		case C_INT_NUM :
			return parser_c_read_intnum_inf () ;
		break ;
		case C_FUNC_REF :
			parser_c_read_funccal () ;			
		break ;		
		default : return 0 ;
	}

	return 1 ;
	
}

static int parser_c_read_symbol_def () {

	//	author : Jelo Wang
	//	since : 20100202
	//	(C)TOK


	switch ( lexc->v ) {
		case C_VAR_DEF :
			parser_c_read_variable_def () ;
		break ;
		default : return 0 ;
	}

	return 1 ;
	
}


static int parser_c_read_variable_def () {

	//	author : Jelo Wang
	//	since : 20100202
	//	(C)TOK

	int scope = 0 ;

	AZONAL* azonal = 0 ;
	int lgnosia = 0 ;
	EVALOR* evalor = 0 ;

	if ( C_VAR_DEF != lexc->v )
		return 0 ;

	//	gen scope value
	scope = SCOPER_GET () ;

	//	if the inf isnt exits on the scope and search for it in funtion parameter list then
	azonal = (AZONAL* ) SymboleGetCurrentFunc () ;
	azonal = (AZONAL* ) SymboleGetFuncParameter ( azonal , lexc->token ) ;

	if ( azonal ) {
		cerror ( C_PARSER_MOD , IS_C_ERROR , "! - variable is multi-defined: '%s' , line : %d\n" , lexc->token , lexc->line ) ;		
	} else if ( 0 == SymboleVarAzonalSavable ( lexc->token , scope , LgnosiaStackTop () ) ) {
		cerror ( C_PARSER_MOD , IS_C_ERROR , "! - variable is multi-defined: '%s' , line : %d\n" , lexc->token , lexc->line ) ;
	} 

	azonal = SymboleAzonalAddVariable (
		lexc->token ,
		parserc_azonal_type ( lexc->headbit ) ,
		ISA_VARIABLE ,
		scope ,
		parserc_azonal_size ( lexc->headbit ) ,
		( parserc->scope == PARSERC_SCOPE_PARAM ) ,
		lexc->line ,
		SymboleGetCurrentFunc () ,
		(int) LgnosiaStackTop ()
	) ;

	LEXERC_HEADBIT_RESET();

	//	create a LGNOSIA_SYM_IDENT
	lgnosia = LgnosiaNew ( (int)azonal , LGNOSIA_SYM_IDENT ) ;

	//	and context to the top LGA.
	LgnosiaAddContext ( LgnosiaStackTop () , (int)lgnosia , LGNOSIA_SYM_IDENT ) ;

	if ( C_EQU == lexerc_head_genv(1) ) {
		
		SET_PARSER_SCOPE(PARSERC_SCOPE_PARAM);
		lexerc_genv () ;
		
		if ( OPERAND_FLOW(lexerc_head_genv(1) )) {
			lexerc_genv () ;
		}

		//	Get an evalor
		evalor = EvalorCreate () ;
		EvalorSetCurrent ( evalor ) ;
		//	parsing EXPR flow
		parser_c_read_expr () ;
		EvalorFinish () ;
	
		//	add parameter for the LGNOSIA.
		LgnosiaAddParameter ( lgnosia , EvalorGetTree () , 0 ) ;	
		UNSET_PARSER_SCOPE();
		
	}

	return 1 ;

}

static int parser_c_read_intnum_inf () {

	//	author : Jelo Wang
	//	since : 20110125
	//	(C)TOK

	if ( C_INT_NUM != lexc->v )
		return 0 ;

	return (int)SymboleAndNumeric ( lexc->token , ISA_INTEGER ) ;

}

static int parser_c_read_variable_inf () {

	//	author : Jelo Wang
	//	since : 20100202
	//	(C)TOK

	int scope = 0 ;

	AZONAL* azonal = 0 ;
	EVALOR* evalor = 0 ;
	int lgnosia = 0 ;
	
	if ( C_VAR_REF != lexc->v )
		return 0 ;

	//	gen scope value
	scope = SCOPER_GET () ;

	azonal = SymboleFindAzonal ( lexc->token , scope , LgnosiaStackTop () ) ;

	if ( !azonal ) {
		//	if the inf isnt exits on the scope and search for it in funtion parameter list then
		azonal = (AZONAL* ) SymboleGetCurrentFunc () ;
		azonal = (AZONAL* ) SymboleGetFuncParameter ( azonal , lexc->token ) ;
	}

	if ( !azonal ) {
		cerror ( C_PARSER_MOD , IS_C_ERROR , "! - undefined variable : '%s' , line : %d\n" , lexc->token , lexc->line ) ;
	}

	SymboleAzonalRefInc ( azonal ) ;
	
	if ( 0 == MATCH_PARSER_SCOPE(PARSERC_SCOPE_PARAM) ) {

		//	and context to the top LGA.
		//	create a LGNOSIA_SYM_IDENT
		lgnosia = LgnosiaNew ( (int)azonal , LGNOSIA_SYM_IDENT ) ;
		LgnosiaAddContext ( LgnosiaStackTop () , (int)lgnosia , LGNOSIA_SYM_IDENT ) ;

	}
	
	if ( C_EQU == lexerc_head_genv (1) ) {	
		SET_PARSER_SCOPE(PARSERC_SCOPE_PARAM);
		lexerc_genv () ;
		if ( OPERAND_FLOW(lexerc_head_genv(1) )) {
			lexerc_genv () ;
		}

		//	Get an evalor
		evalor = EvalorCreate () ;
		EvalorSetCurrent ( evalor ) ;		
		//	parsing EXPR flow
		parser_c_read_expr () ;
		EvalorFinish () ;

		//	add parameter for the LGNOSIA.
		LgnosiaAddParameter ( lgnosia , EvalorGetTree () , 0 ) ;	
		UNSET_PARSER_SCOPE();
	
	}
		
	return (int)azonal ;


}

static int parser_c_read_expr () {

	//	author : Jelo Wang
	//	since : 20100202
	//	(C)TOK

	//	(1) a1+a2*a3 ;

	int symbol = 0 ;

	if ( PORL_OPERATOR(lexerc_head_genv (1)) ) {
		lexerc_genv () ;
		parser_c_read_poroperator ( 0 , 1 ) ;
	}

	while (!lexc->stop) {
			
		//	read prior operator
		parser_c_read_poroperator ( 0 , 1 ) ;

		//	parsing symbol and return its handle
		symbol = parser_c_read_symbol_inf () ;

		//	if the handle is zero , break the processing.
		if ( 0 == symbol ) break ;
			
		//	push the symbol in evalor.
		EvalorOperandPush ( symbol ) ;

		lexerc_genv () ;

reoperator :
		if ( DUA_OPERATOR(lexc->v) ) {
			//	push the operator in evalor
			EvalorOperatorPush ( lexc->v ) ;
			lexerc_genv () ;
		}
		
		//	read prior operator
		parser_c_read_poroperator ( 1 , 1 ) ;

		if ( DUA_OPERATOR(lexc->v) )
			goto reoperator ;	

	}

	return 1 ;

		
}

static void parser_c_read_poroperator ( int side , int evalor_enable ) {

	//	author : Jelo Wang
	//	since : 20091124
	//	(C)TOK
	
	//	(1) side == 0 , left bracket
	//	(2) side == 1 , right bracket
	//	(3) if evalor_enable == 1 , push bracket in it.	

	int key = 0 ;
	
	if ( 0 == side ) key = C_XKL ;
	else if ( 1 == side ) key = C_XKR ;
	
	if ( key != lexc->v ) return ;
	
	while ( !lexc->stop ) {	
		if ( key != lexc->v ) break ;
		if ( 1 == evalor_enable ) EvalorOperatorPush ( lexc->v ) ;
		if ( key != lexerc_head_genv (1) ) {
			lexerc_genv () ;
			break ;
		} else lexerc_genv () ;
	}

}

void parser_c_skip_parenthesis_scope () {

	//	author : Jelo Wang
	//	since : 20110211
	//	(C)TOK
	
	while ( !lexc->stop && C_XKR != lexc->v ) {
		lexerc_genv () ;
	}
	
}

int parser_c_run ( int* lines ) {

	//	author : Jelo Wang
	//	since : 20091124
	//	(C)TOK

	int success = 1 ;
	int state = 0 ;
	
	int symbole = SymboleCreate () ;
	SymboleSetCurrent ( symbole ) ;
	
	parserc_ready () ;
	
	lexerc_ready () ;
	lexerc_setmode ( LEXERC_FLITER_MODE | LEXERC_HEADBIT_MODE ) ;
	
	for ( lexerc_genv () ; !lexc->stop ; lexerc_genv () ) {
				
		if ( C_VAR_DEF == lexc->v ) {
			parser_c_read_variable_def () ;
		} else if ( C_ARRAY_DEF == lexc->v ) {
		} else if ( C_FUNC_DEF == lexc->v ) {
			parser_c_read_function () ;
		} else {
			cerror ( C_PARSER_MOD , IS_C_ERROR , "undefined grammar : %x , line : %d\n" , lexc->v , lexc->line ) ;
		}

	}

	*lines = *lines +  lexc->line ;
		
	lexerc_destroy () ;
	SCClStackDestroy ( &parserc->scopestack ) ;
	SCFreeEx ( &parserc ) ;

	return 1 ;
	
}

