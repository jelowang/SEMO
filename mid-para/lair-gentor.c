
/*

+	LAIR's Code Generator , part of SC mid-para

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
# include "lair-gentor.h"
# include "lair.h"
# include "corenr.h"

static SCClStack LabelStack = { 0 , 0 , 0 , 0 , 0 } ;
static SCClString* lairgentor_results = 0 ;
static LAIR_GENTOR lairgentor = { -1 , 0 , 0 , 0 , 0 , 0 , 0 } ;

int corenr = 0 ;

# define LAIRGENTOR_STATUS_RESET()\
	lairgentor.form = -1;\

# define SET_LAIRGENTOR_DELT(deltn)\
	SCClStackPush ( &lairgentor.deltstack, (void* )lairgentor.delt );\
	lairgentor.delt = deltn;\

# define UNSET_LAIRGENTOR_DELT()\
	lairgentor.delt = SCClStackPop ( &lairgentor.deltstack ) ;\

# define GET_LAIRGENTOR_SCOPE() SCClStackGet( &lairgentor.scopestack )

# define SET_LAIRGENTOR_SCOPE(sscope)\
	SCClStackPush ( &lairgentor.scopestack , (void* )sscope ) ;\

# define UNSET_LAIRGENTOR_SCOPE()\
	SCClStackPop ( &lairgentor.scopestack ) ;\

# define PUSH_LAIRGENTOR_LGA(lga)\
	SCClStackPush ( &lairgentor.lgastack  , (void* )lga ) ;\
	
# define GET_LAIRGENTOR_LGA() SCClStackGet( &lairgentor.lgastack )

# define POP_LAIRGENTOR_LGA()\
	SCClStackPop ( &lairgentor.lgastack ) ;\

static void lairgentor_gen_else ( LGNOSIA* lgnosia , AZONAL* azonal ) ;
static void lairgentor_gen_while ( LGNOSIA* lgnosia , AZONAL* azonal ) ;
static void lairgentor_switcher ( LGNOSIA* lgnosia ) ;
static void lairgentor_gen_variable ( LGNOSIA* lgnosia , AZONAL* azonal ) ; 
static void lairgentor_gen_funccal ( LGNOSIA* lgnosia , AZONAL* azonal ) ;
static void lairgentor_gen_pochain ( LGNOSIA* po_chain ) ;
static void lairgentor_gen_nechain ( LGNOSIA* ne_chain ) ;

static void LAIRIdentorPush () {

	//	author : Jelo Wang
	//	since : 20100130
	//	(C)TOK

	SCClStackPush ( &lairgentor.identor , (void* )1 ) ;
	
}

static void LAIRIdentorPop () {

	//	author : Jelo Wang
	//	since : 20100130
	//	(C)TOK

	SCClStackPop ( &lairgentor.identor ) ;
	
}

static char* lairgentor_get_register ( int index ) {

	//	author : Jelo Wang
	//	since : 20100425

	static char tempv [64] = {"%$X"} ;
	
	return sc_strcat ( tempv , SCClItoa (index) ) ;
	
}


static char* lairgentor_get_label () {

	//	author : Jelo Wang
	//	since : 20100503

	static char tempv [64] = {"L"} ;
	static char result [64] = {0} ;

	char* A = sc_strcat ( tempv , SCClItoa (lairgentor.label++) ) ;
	char* B = sc_strcat ( A , ":" ) ;

	SCFree ( A ) ;

	sc_strcpy ( result , B ) ;

	SCFree ( B ) ;

	return result ;
	
}


static char* lairgentor_head_label ( int label ) {

	//	author : Jelo Wang
	//	since : 20100503

	static char tempv [64] = {"L"} ;
	static char result [64] = {0} ;
		
	char* A = sc_strcat ( tempv , SCClItoa (label) ) ;
	char* B = sc_strcat ( A , ":" ) ;
	
	SCFree ( A ) ;

	sc_strcpy ( result , B ) ;

	SCFree ( B ) ;

	return result ;
	
}

static char* lairgentor_get_identor () {

	//	author : Jelo Wang
	//	since : 20100130
	//	(C)TOK

	//	generate a label
	
	int walker = 0 ;
	char* scope = 0 ;
	
	if ( 0 == lairgentor.identor.deep ) return 0 ;
	
	scope = (char* ) SCMalloc ( lairgentor.identor.deep + 1 ) ;

	for ( walker = 0 ; walker < lairgentor.identor.deep ; walker ++ )
		scope [ walker ] = '\t' ;
	
	scope [ walker ] = '\0' ;

	return scope ;

}

static void lairgentor_ready () {
	
	//	author : Jelo Wang
	//	since : 20100107
	//	(C)TOK
	
	lairgentor.form =-1 ;
	lairgentor.stop = 0  ;
	lairgentor.delt = 0 ;
	lairgentor.label = 0 ;
	lairgentor.tree = 0 ;
	lairgentor.lgnosia = 0 ;
	
	SCClStackInit ( &lairgentor.identor ) ;
	SCClStackInit ( &lairgentor.deltstack ) ;

	//	get LGA tree
	lairgentor.tree = (SCClList* ) LgnosiaGet () ;
	
	if ( lairgentor.tree ) 
		lairgentor.lgnosia = lairgentor.tree->element ;

} 

static void lairgentor_next () {
	
	//	author : Jelo Wang
	//	since : 20100107
	//	(C)TOK

	if ( lairgentor.tree ) lairgentor.tree = lairgentor.tree->next ;
	
	if ( !lairgentor.tree ) {
		lairgentor.stop = 1 ; 
		return ;
	}

	lairgentor.lgnosia = lairgentor.tree->element ;

}

static void lairgentor_genv () {
	
	//	author : Jelo Wang
	//	since : 20100107
	//	(C)TOK
	
	if ( !lairgentor.tree ) {
		lairgentor.form = -1 ;
		return ;
	}

	if ( !lairgentor.stop ) {
		
		if ( !lairgentor.lgnosia ) return ;
		
		lairgentor.form = ((LGNOSIA*)(lairgentor.lgnosia))->type ;
		
	}
	
}


static void lairgentor_gen_funcdef () {
	
	//	author : Jelo Wang
	//	since : 20100107
	//	(C)TOK

	AZONAL* azonal = 0 ;	
	LGNOSIA* lgnosia = 0 ;
	SCClList* listlooper = 0 ;

	if ( LGNOSIA_TOP_IDENT != lairgentor.form ) {
		LAIRGENTOR_STATUS_RESET() ;
		return ;
	}

	lgnosia = (LGNOSIA* ) lairgentor.lgnosia ;
	azonal = (AZONAL* ) lgnosia->azonal ;

	LairAddCode ( azonal->name , LAIR_PROC , lairgentor.identor.deep ) ;
	LairAddCode ( "(" , -1 , -1 ) ;

	//	gen __armcall para	 pattern
	//	根据__armcall 调用规范生成参数列表
	if ( 0 < azonal->tack.totall ) {
		
		int counter = 0 ;
		char* vn [ 4 ] = { "%$V1" , "%$V2" , "%$V3" , "%$V4" } ;

		AZONAL* anl = 0 ;
		SCClList* looper = 0 ;

		//	get parameters
		for ( listlooper = azonal->tack.head ; listlooper ; listlooper = listlooper->next , counter ++ ) {

			anl = (AZONAL* ) listlooper->element ;

			if ( counter < 4 ) {
				LairAddCode ( vn[counter] , LAIR_LVA_DELT , -1 ) ;
			} else {
			}
				
		}
		
	}
	
	LairAddCode ( ")\r\n" , LAIR_CR , -1 ) ;
	LairAddCode ( "{\r\n" , LAIR_CR , -1 ) ;
	
	PUSH_LAIRGENTOR_LGA(lgnosia) ;
	SET_LAIRGENTOR_SCOPE(ISA_FUNCTION);
	SET_LAIRGENTOR_DELT(0);
	LAIRIdentorPush () ;
	//	if this function has local variables
	//	we initialize the MF here
	if ( 0 < azonal->size) {
		LairAddCode ( lairgentor_get_identor () , -1 , -1 ) ;
		LairAddCode ( "%$STK.Creat " , -1 , -1 ) ;
		LairAddCode ( SCClItoa (azonal->size) , -1 , -1 ) ;
		LairAddCode ("\r\n",LAIR_CR,-1);
	}
	for ( listlooper = lgnosia->context.head ; listlooper ; listlooper = listlooper->next ) {
		lairgentor_switcher ( (LGNOSIA*)listlooper->element ) ;		
	}

	//	if this function has local variables
	//	release stack memory
	if ( 0 < azonal->size) {
		LairAddCode ( lairgentor_get_identor () , -1 , -1 ) ;
		LairAddCode ( "%$STK.Release " , -1 , -1 ) ;
		LairAddCode ( SCClItoa (azonal->size) , -1 , -1 ) ;
		LairAddCode ("\r\n",LAIR_CR,-1);
	}
	
	LAIRIdentorPop () ;
	UNSET_LAIRGENTOR_DELT();
	UNSET_LAIRGENTOR_SCOPE();
	POP_LAIRGENTOR_LGA() ;
	
	LairAddCode ( "}" , LAIR_CR , -1 ) ;
	LairAddCode ("\r\n",LAIR_CR,-1);


	
	SCFree ( (void* )lairgentor.lgnosia ) ;

}

static void lairgentor_gen_ifcf ( LGNOSIA* lgnosia , AZONAL* azonal ) {
	
	//	author : Jelo Wang
	//	since : 20100130
	//	(C)TOK
	
	int lair = 0 ;
	static int ifflow_deep = 0 ;
	LGNOSIA* lgnosiawalker = 0 ;
	SCClList* parameter = 0 ;

	PUSH_LAIRGENTOR_LGA(lgnosia) ;
	SET_LAIRGENTOR_SCOPE(ISA_IFCF);

	SET_LAIRGENTOR_DELT(0);
	LAIRIdentorPush () ;
	parameter = lgnosia->parameter.head ;
	lairgentor_gen_expr ( (EXPR*)parameter->element , 1 ) ;
	LAIRIdentorPop () ;
	UNSET_LAIRGENTOR_DELT();

	LairAddCode ( sc_strcat ( lairgentor_get_identor () , "if ( " ) , LAIR_IF , -1 ) ;
	LairAddCode ( ((EXPR*)parameter->element)->delt , LAIR_RVA_DELT , lairgentor.identor.deep ) ;

	lair = LairAddCode ( " ) FALSE goto " , -1 , -1 ) ;
	LairAddCode ( "\r\n" , LAIR_CR , -1 );

	//	push lga and lair 
	//	for if-flow label generating
	SCClStackPush ( &LabelStack , (void* )lair ) ;
	SCClStackPush ( &LabelStack , (void* )lgnosia ) ;
	
	ifflow_deep ++ ;

	SET_LAIRGENTOR_DELT(0);
	LAIRIdentorPush () ;
	//	generate po_chain
	lairgentor_gen_pochain ( lgnosia->po_chain ) ;
	LAIRIdentorPop () ;
	UNSET_LAIRGENTOR_DELT();
	
	{

		//	checking NEC
		//	if NEC exits make a label for it
		LGNOSIA* lga = (LGNOSIA* ) SCClStackPop ( &LabelStack ) ;
		int lairc = SCClStackPop ( &LabelStack ) ;
		
		if ( lga && lga->ne_chain ) {
			LairAddCodeLabel ( lair , lairgentor_head_label (lairgentor.label) ) ;
		} else {
			LairLabelMoiAdd ( lairc ) ;
		}
		
	}

	LairLabelMoiAdd ( LairAddCode ( sc_strcat ( lairgentor_get_identor () , "goto ") , -1 , -1 ) ) ;
	LairAddCode ( "\r\n" , LAIR_CR , -1 );
	
	SET_LAIRGENTOR_DELT(0);
	LAIRIdentorPush () ;		
	//	generate ne_chain	
	lairgentor_gen_nechain ( lgnosia->ne_chain ) ;
	LAIRIdentorPop () ;
	UNSET_LAIRGENTOR_DELT();

	ifflow_deep -- ;
	//	确保当前节点为控制链最后一个节点
	if ( 0 == ifflow_deep ) {
		//	set label
		LairLabelMoiSetLabel ( lairgentor_head_label (lairgentor.label) ) ;
	}

	UNSET_LAIRGENTOR_SCOPE();
	POP_LAIRGENTOR_LGA() ;

	SCFree ( lgnosia ) ;



}


static void lairgentor_gen_elseifcf ( LGNOSIA* lgnosia , AZONAL* azonal ) {
	
	//	author : Jelo Wang
	//	since : 20100811
	//	(C)TOK
	
	int lair = 0 ;
	static int ifflow_deep = 0 ;
	LGNOSIA* lgnosiawalker = 0 ;
	SCClList* parameter = 0 ;

	PUSH_LAIRGENTOR_LGA(lgnosia) ;
	SET_LAIRGENTOR_SCOPE(ISA_ELSEIFCF);

	parameter = lgnosia->parameter.head ;
	lairgentor_gen_expr ( (EXPR*)parameter->element , 1 ) ;
		
	LairAddCode ( sc_strcat ( lairgentor_get_identor () , "if ( " ) , LAIR_IF , -1  ) ;
	LairAddCode ( ((EXPR*)parameter->element)->delt , LAIR_RVA_DELT , lairgentor.identor.deep ) ;

	lair = LairAddCode ( " ) FALSE goto " , -1 , -1 ) ;
	LairAddCode ( "\r\n" , LAIR_CR , -1 );

	//	push lga and lair 
	//	for if-flow label generating
	SCClStackPush ( &LabelStack , (void* )lair ) ;
	SCClStackPush ( &LabelStack , (void* )lgnosia ) ;

//	SET_LAIRGENTOR_DELT(0);
//	LAIRIdentorPush () ;			
	//	generate po_chain
	lairgentor_gen_pochain ( lgnosia->po_chain ) ;
//	LAIRIdentorPop () ;
//	UNSET_LAIRGENTOR_DELT();

	{

		LGNOSIA* lga = (LGNOSIA* ) SCClStackPop ( &LabelStack ) ;
		int lairc = SCClStackPop ( &LabelStack ) ;
		
		if ( lga && lga->ne_chain ) {
			
			LairAddCodeLabel ( lair , lairgentor_head_label (lairgentor.label) ) ;

		} else {
			LairLabelMoiAdd ( lairc ) ;
		}
		
	}

	LairLabelMoiAdd ( LairAddCode ( sc_strcat ( lairgentor_get_identor () , "goto ") , -1 , -1 ) ) ;
	LairAddCode ( "\r\n" , LAIR_CR , -1 );
	
//	SET_LAIRGENTOR_DELT(0);
//	LAIRIdentorPush () ;		
	//	generate ne_chain	
	lairgentor_gen_nechain ( lgnosia->ne_chain ) ;
//	LAIRIdentorPop () ;
//	UNSET_LAIRGENTOR_DELT();

	//	确保当前节点为控制链最后一个节点
	if ( 0 == ifflow_deep ) {
		//	set label
		LairLabelMoiSetLabel ( lairgentor_head_label (lairgentor.label) ) ;
	}

	UNSET_LAIRGENTOR_SCOPE();
	POP_LAIRGENTOR_LGA() ;

	SCFree ( lgnosia ) ;
	
	
}


static void lairgentor_gen_else ( LGNOSIA* lgnosia , AZONAL* azonal ) {
	
	//	author : Jelo Wang
	//	since : 20100502
	//	(C)TOK
	
	int lair = 0 ;

	SCClList* listlooper = 0 ;

	PUSH_LAIRGENTOR_LGA(lgnosia) ;
	SET_LAIRGENTOR_SCOPE(ISA_ELSECF);
	
	LairAddCode ( sc_strcat ( lairgentor_get_identor () , "// else-flow\r\n" ) , LAIR_CR , -1 ) ;
		
	for ( listlooper = lgnosia->context.head ; listlooper ; listlooper = listlooper->next ) {
			
		lairgentor_switcher ( (LGNOSIA*)listlooper->element ) ;
			
	}

	LairLabelMoiAdd ( LairAddCode ( sc_strcat ( lairgentor_get_identor () , "goto ") , -1 , -1 ) ) ;
	LairAddCode ( "\r\n" , LAIR_CR , -1 );

	//	release the LGA
	SCFree ( lgnosia ) ;	

	UNSET_LAIRGENTOR_SCOPE();
	POP_LAIRGENTOR_LGA() ;

	SCFree ( lgnosia ) ;


}


static void lairgentor_gen_while ( LGNOSIA* lgnosia , AZONAL* azonal ) {
	
	//	author : Jelo Wang
	//	since : 20100502
	//	(C)TOK

	char* entry_label = 0 ;
	SCClList* listlooper = 0 ;
	SCClList* parameter = 0 ;
	int lair = 0 ;

	PUSH_LAIRGENTOR_LGA(lgnosia) ;
	SET_LAIRGENTOR_DELT(0);
	parameter = lgnosia->parameter.head ;
	lairgentor_gen_expr ( (EXPR*)parameter->element , 0 ) ;
	UNSET_LAIRGENTOR_DELT();

	entry_label = lairgentor_get_label () ;
	LairAddCode ( entry_label , -1 , -1 );
	LairAddCode ( "\r\n" , LAIR_CR , -1 );		

	LairAddCode ( sc_strcat ( lairgentor_get_identor () , "while ( " ) , -1 , -1 ) ;
	LairAddCode ( ((EXPR*)parameter->element)->delt , LAIR_RVA_DELT , lairgentor.identor.deep ) ;
	lair = LairAddCode ( " ) FALSE goto " , -1 , -1 ) ;
	LairAddCode ( "\r\n" , LAIR_CR , -1 );
	
	LairLabelMoiAdd ( lair ) ;

	SET_LAIRGENTOR_DELT(0);
	LAIRIdentorPush () ;

	for ( listlooper = lgnosia->context.head ; listlooper ; listlooper = listlooper->next ) {	
		lairgentor_switcher ( (LGNOSIA*)listlooper->element ) ;
	}

	lairgentor_gen_expr ( (EXPR*)parameter->head , 1 ) ;
	LairAddCode ( sc_strcat ( lairgentor_get_identor () , "goto " ) , -1 , -1 ) ;
	LairAddCode ( entry_label , -1 , -1 ) ;
	LairAddCode ( "\r\n" , LAIR_CR , -1 );

	SCFreeEx ( &((EXPR*)parameter->element)->delt ) ;
	SCFreeEx ( (void**)&parameter->element ) ;	
	
	LAIRIdentorPop () ;
	UNSET_LAIRGENTOR_DELT();
	
	LairLabelMoiSetLabel ( lairgentor_head_label (lairgentor.label) ) ;
	POP_LAIRGENTOR_LGA() ;

	SCFree ( lgnosia ) ;


}



static void lairgentor_switcher ( LGNOSIA* lgnosia ) {

	//	author : Jelo Wang
	//	since : 20100130
	//	(C)TOK

	AZONAL* azonal = (AZONAL* ) lgnosia->azonal ;
	
	if ( !azonal ) return ;
		
	switch ( azonal->azonaltype ) {
		
		case ISA_IFCF :
			LairAddCode ( lairgentor_get_label() , -1 , -1 );
			LairAddCode ( "\r\n" , LAIR_CR , -1 ) ;						
			SsaMakeMultiAliasEnable () ;
			lairgentor_gen_ifcf ( lgnosia , azonal ) ;
			SsaMakeMultiAliasDisable () ;
		break ;
		case ISA_ELSEIFCF :
			LairAddCode ( lairgentor_get_label() , -1 , -1 );
			LairAddCode ( "\r\n" , LAIR_CR , -1 ) ;						
			SsaMakeMultiAliasEnable () ;
			lairgentor_gen_elseifcf ( lgnosia , azonal ) ;
			SsaMakeMultiAliasDisable () ;
		break ;
		case ISA_ELSECF :
			LairAddCode ( lairgentor_get_label() , -1 , -1 );
			LairAddCode ( "\r\n" , LAIR_CR , -1 ) ;						
			SsaMakeMultiAliasEnable () ;
			lairgentor_gen_else ( lgnosia , azonal ) ;
			SsaMakeMultiAliasDisable () ;			
		break ;
		case ISA_WHILECF :
			LairAddCode ( lairgentor_get_label() , -1 , -1 );
			LairAddCode ( "\r\n" , LAIR_CR , -1 ) ;						
			SsaMakeMultiAliasEnable () ;
			lairgentor_gen_while ( lgnosia , azonal ) ;
			SsaMakeMultiAliasDisable () ;
		break ;

		case ISA_VARIABLE :
			lairgentor_gen_variable ( lgnosia , azonal ) ;
		break ;
		case ISA_FUNCCAL :
			lairgentor_gen_funccal ( lgnosia , azonal ) ;			
		break ;	

	}


}

static void lairgentor_gen_variable ( LGNOSIA* lgnosia , AZONAL* azonal ) {

	//	author : Jelo Wang
	//	since : 20100425
	//	(C)TOK
	
	EXPR* expression = 0 ;

	if ( 0 == lgnosia->parameter.head ) {
		
		LairAddCode ( lairgentor_get_identor () , -1 , -1 ) ;
		LairAddCode ( "%$STK.Def " , -1 , -1 ) ;	
		LairAddCode ( azonal->name , LAIR_DEF , lairgentor.identor.deep ) ;		
		LairAddCode ( "\r\n" , LAIR_CR , -1 ) ;	

	} else if ( lgnosia->parameter.head ) {

		char* name = 0 ;
			
		expression = (EXPR*)lgnosia->parameter.head->element ;
		lairgentor_gen_expr ( (EXPR*)expression , 1 ) ;

		//	for local azonal
		if ( 0 != azonal->lgabelong ) {
			
			name = (char* ) SCMalloc ( sc_strlen (azonal->name) + sc_strlen ("%$STK.Set ")+ 1 ) ;
			ASSERT(name) ;
			sc_strcpy ( name , "%$STK.Set " ) ;

		} else {
		//	for global azonal
			name = (char* ) SCMalloc ( sc_strlen (azonal->name) + sc_strlen ("%$HEAP.")+ 1 ) ;
			ASSERT(name) ;		
			sc_strcpy ( name , "%$HEAP." ) ;
		}
		
		sc_strcat_ex ( name , azonal->name , name ) ;
		name = sc_strcat (name , SCClItoa(SsaMakeAlias ( GET_LAIRGENTOR_LGA(),azonal,lairgentor.identor.deep) ) ) ;
		SsaCleanMultiAlias ( azonal , GET_LAIRGENTOR_LGA() , azonal,lairgentor.identor.deep ) ;

		LairAddCode ( lairgentor_get_identor () , -1 , -1 ) ;

		LairAddCode ( name , LAIR_LVA_DELT , lairgentor.identor.deep ) ;
		LairAddCode ( "=" , -1 , -1 ) ;

		if ( expression->delttype == EXP_DELT_ANLNUMERIC ) 
			LairAddCode ( ((EXPR*)expression)->delt , -1 , -1 ) ;
		else 
			LairAddCode ( ((EXPR*)expression)->delt , LAIR_RVA_DELT , lairgentor.identor.deep ) ;
		
		LairAddCode ( "\r\n" , LAIR_CR , -1 ) ;

		SCFreeEx ( &((EXPR*)expression)->delt ) ;
		SCFreeEx ( (void**)&expression ) ;
		SCFree ( (void*)name ) ;
		
	}	

	SCFree ( lgnosia ) ;

}

static void lairgentor_gen_calltransfer ( AZONAL* azonal , int type ) {

	//	author : Jelo Wang
	//	since : 20110223
	//	(C)TOK

	//	generate __stdcall,__cdecl,__fastcall,__armcall pattern

	int counter = 0 ;
	
	//	before calling
	if ( 0 == type ) {

		if ( SC_C0 & semo->parameter ) {

			//	for __stdcall type			
			LairAddCode ( lairgentor_get_identor () , -1 , -1 ) ;
			LairAddCode ( "%$STACK INIT " , -1 , -1 ) ;
			LairAddCode ( SCClItoa ( azonal->tack.totall ) , -1 , -1 ) ; 
			LairAddCode ( " ;\r\n" , LAIR_CR , -1 ) ;
		
		} else if ( SC_C3 & semo->parameter ) {
		
			//	for __armcall type						
			LairAddCode ( lairgentor_get_identor () , -1 , -1 ) ;
//			LairAddCode ( "%$STACK INIT " , -1 , -1 ) ;
//			LairAddCode ( SCClItoa ( azonal->tack.totall ) , -1 , -1 ) ; 
//			LairAddCode ( " ;\r\n" , LAIR_CR , -1 ) ;

			if ( 5 > azonal->tack.totall ) {
				AZONAL* looper = azonal->tack.head ;
				while ( looper ) {
					LairAddCode ( "%$V1 = " , -1 , -1 ) ;
					LairAddCode ( looper->name , -1 , -1 ) ;
					LairAddCode ( " ;\r\n" , LAIR_CR , -1 ) ;
					looper = looper->next ;
				}
			}
		
		}

		
 	} else if ( 1 == type ) {

		//	after calling
		if ( SC_C3 & semo->parameter ) {
		
			//	for __armcall type						
			LairAddCode ( lairgentor_get_identor () , -1 , -1 ) ;
			LairAddCode ( "%$STACK UNIT " , -1 , -1 ) ;
			LairAddCode ( SCClItoa ( azonal->tack.totall ) , -1 , -1 ) ; 
			LairAddCode ( " ;\r\n" , LAIR_CR , -1 ) ;
		
		}
		
	}

}

static void lairgentor_gen_funccal ( LGNOSIA* lgnosia , AZONAL* azonal ) {

	//	author : Jelo Wang
	//	since : 20110212
	//	(C)TOK

	char* name = 0 ;
	char* vn [ 4 ] = { "%$V1" , "%$V2" , "%$V3" , "%$V4" } ;

	int walker = 0 ;
	int counter = 0 ;

	SCClList* looper = 0 ;

	//	backup V1,V2,V3,V4
	for ( walker = 0 ; walker < azonal->tack.totall && 0 != walker-4 ; walker ++ ) {
		LairAddCode ( lairgentor_get_identor () , -1 , -1 ) ; 											
		LairAddCode ( "%$STACK IN " , -1 , -1 ) ;				
		LairAddCode ( vn[walker] , -1 , -1 ) ;				
		LairAddCode ( " ;\r\n" , LAIR_CR , -1 ) ; 						
	}
	
	//	generate __armcall pattern
	if ( SC_C3 & semo->parameter ) {
		
		//	totall parameters is <= 4
		if ( 4 >= azonal->tack.totall ) {			
			
			for ( walker = 0 , counter = 0 , looper = lgnosia->context.head ; looper ; looper = looper->next , counter ++ ) {

				LGNOSIA* lga = (LGNOSIA* ) looper->element ;
				AZONAL* anl = (AZONAL* ) lga->azonal ;
				
				LairAddCode ( lairgentor_get_identor () , -1 , -1 ) ;	

				LairAddCode ( vn[walker] , -1 , -1 ) ;				
				LairAddCode ( " = " , -1 , -1 ) ;

				//	jelo 20121027
				//	SSA get Alias
				//	name = SymboleDRCGetDRC ( anl , lairgentor.identor.deep , GET_LAIRGENTOR_LGA() ) ;
				//	LairAddCode ( name , LAIR_RVA_DELT , lairgentor.identor.deep ) ;		
				//	SCFree ( name ) ;
				//	jelo
				
				LairAddCode ( " ;\r\n" , LAIR_CR , -1 ) ;			

				
			}

			LairAddCode ( lairgentor_get_identor () , -1 , -1 ) ; 
			LairAddCode ( "%$CA " , -1 , -1 ) ;
			LairAddCode ( azonal->name , LAIR_P_CALL , lairgentor.identor.deep ) ;
			LairAddCode ( " ;\r\n" , LAIR_CR , -1 ) ;
			
		} else if ( 4 < azonal->tack.totall ) {

			//	skip REGSITER usage
			for ( counter = 0 , looper = lgnosia->context.head ; counter < 4 ; counter ++ ) {
				looper = looper->next ;
			}

			//	generate STACK usage
			LairAddCode ( lairgentor_get_identor () , -1 , -1 ) ; 
			LairAddCode ( "%$STACK INIT " , -1 , -1 ) ;
			LairAddCode ( SCClItoa ( azonal->tack.totall - counter ) , -1 , -1 ) ;
			LairAddCode ( " ;\r\n" , LAIR_CR , -1 ) ;
			
			for ( ; looper ; looper = looper->next ) {
			
				LGNOSIA* lga = (LGNOSIA* ) looper->element ;
				AZONAL* anl = (AZONAL* ) lga->azonal ;				
			
				LairAddCode ( lairgentor_get_identor () , -1 , -1 ) ; 								
				LairAddCode ( "%$STACK IN " , -1 , -1 ) ;
				//	jelo 20121027
				//	SSA get Alias
				//	name = SymboleDRCGetDRC ( anl , lairgentor.identor.deep , GET_LAIRGENTOR_LGA() ) ;
				//	LairAddCode ( name , LAIR_RVA_DELT , lairgentor.identor.deep ) ; 	
				//	LairAddCode ( " ;\r\n" , LAIR_CR , -1 ) ; 		
				//	SCFree ( name ) ;
				//	jelo end
				
			}
			//	end
			
			for ( counter = 0 , looper = lgnosia->context.head ; looper && counter < 4 ; looper = looper->next , counter ++ ) {

				LGNOSIA* lga = (LGNOSIA* ) looper->element ;
				AZONAL* anl = (AZONAL* ) lga->azonal ;
				
				LairAddCode ( lairgentor_get_identor () , -1 , -1 ) ;	
				LairAddCode ( vn[counter] , -1 , -1 ) ;				
				LairAddCode ( " = " , -1 , -1 ) ;

				//	jelo 20121027
				//	SSA get Alias
				//	name = SymboleDRCGetDRC ( anl , lairgentor.identor.deep , GET_LAIRGENTOR_LGA() ) ;
				//	LairAddCode ( name , LAIR_RVA_DELT , lairgentor.identor.deep ) ;		
				//	LairAddCode ( " ;\r\n" , LAIR_CR , -1 ) ;			
				//	SCFree ( name ) ;
				//	end
				
			}
			
			LairAddCode ( lairgentor_get_identor () , -1 , -1 ) ; 
			LairAddCode ( "%$CA " , -1 , -1 ) ;
			LairAddCode ( azonal->name , LAIR_P_CALL , lairgentor.identor.deep ) ;
			LairAddCode ( " ;\r\n" , LAIR_CR , -1 ) ;
			
			LairAddCode ( lairgentor_get_identor () , -1 , -1 ) ; 								
			LairAddCode ( "%$STACK UNIT " , -1 , -1 ) ;
			LairAddCode ( SCClItoa ( azonal->tack.totall - counter ) , -1 , -1 ) ;
			LairAddCode ( " ;\r\n" , LAIR_CR , -1 ) ;
			
		}

	}

	for ( walker = 0 ; walker < azonal->tack.totall && 0 != walker-4 ; walker ++ ) {
		LairAddCode ( lairgentor_get_identor () , -1 , -1 ) ; 							
		LairAddCode ( "%$STACK OUT " , -1 , -1 ) ; 	 		
		LairAddCode ( vn[walker] , -1 , -1 ) ;				
		LairAddCode ( " ;\r\n" , LAIR_CR , -1 ) ; 						
	}
	

}

static int lairgentor_gen_expr ( EXPR* expression , int drop ) {

	//	author : Jelo Wang
	//	since : 20100425
	//	(C)TOK

	int recursived = 0 ;
	
	if ( !expression ) {
		return 0 ;
	}
	
	if ( expression->left ) {
		recursived = 1 ;
		lairgentor_gen_expr ( expression->left , drop ) ;
	}

	if ( expression->right ) {
		recursived = 1 ;		
		lairgentor_gen_expr ( expression->right , drop ) ;
	}

	if ( EXP_OPERAND == expression->type ) {

		char* name = 0 ; 
		AZONAL* azonal = expression->handle ;

		//	if this azonal is not a parameter
		//	else it would be in LAIR-CALL-FRAME
		if ( 0 == azonal->isparam ) {
		
			name = (char* ) SCMalloc ( sc_strlen (azonal->name) + 1 ) ;
			ASSERT(name) ;	
			sc_strcpy ( name , azonal->name ) ;

	 		if ( ISA_INTEGER != azonal->azonaltype ) {

				LGNOSIA* lgnosia = 0 ;

				//	在最近Alias 的flow中寻找当前flow
				//	如果找到了则不获取多重别名，直接使用最近的alias
				lgnosia = LgnosiaFindContextEx ( SsaGetAliasFlow (azonal) , GET_LAIRGENTOR_LGA() ) ;
		
				if ( 0 != lgnosia && LGNOSIA_CP_IDENT == lgnosia->type ) { 
					
					name = sc_strcat (name,SCClItoa(SsaGetAlias (azonal,lairgentor.identor.deep) ) ) ;
					expression->delt = (char* ) SCMalloc ( sc_strlen (name) + 1 ) ;
					sc_strcpy ( expression->delt  , name ) ;
					
				} else {

					char* multialias = 0 ;
	 				multialias = SsaMakeMultiAliasString(azonal,azonal->name,lairgentor.identor.deep , GET_LAIRGENTOR_LGA() ) ;

					if ( 0 != multialias ) {
						//	using multialias first
						name = multialias ;
						expression->delt = (char* ) SCMalloc ( sc_strlen (name) + sc_strlen ( "$%SSA()" ) ) ;
						sc_strcpy ( expression->delt  , "$%SSA(" ) ;
						sc_strcat_ex ( expression->delt  , name , expression->delt   ) ;
						sc_strcat_ex ( expression->delt  , ")" , expression->delt ) ;
					} else {
						name = sc_strcat (name,SCClItoa(SsaGetAlias (azonal,lairgentor.identor.deep) ) ) ;
						expression->delt = (char* ) SCMalloc ( sc_strlen (name) + 1 ) ;
						sc_strcpy ( expression->delt  , name ) ;
					}
					
				}
						
				SCFree ( name ) ;
				
			} else {		
				expression->delt = (char* ) SCMalloc ( sc_strlen (azonal->name) + 1 ) ;
				sc_strcpy ( expression->delt  , azonal->name ) ;
			}

			if ( ISA_INTEGER == azonal->azonaltype )
				expression->delttype = EXP_DELT_ANLNUMERIC ;
			else 
				expression->delttype = EXP_DELT_ANLDATA ;

		} else {
//			char* frame = LAIRMemoryFrameGet ( azonal->name ) ;
//			//	get LAIR-CALL-FRAME
//			expression->delt = (char* ) SCMalloc ( sc_strlen (frame) + 1 ) ;
//			sc_strcpy ( expression->delt  , frame ) ;		
//			expression->delttype = EXP_DELT_ANLDATA ;			
		}
		
		
	} else if ( EXP_OPERATOR == expression->type ) {

		char* tvalue = 0 ;
		
		LairAddCode ( lairgentor_get_identor () , -1 , -1 ) ;

		tvalue = lairgentor_get_register (lairgentor.delt+1) ;
		expression->delt = (char* ) SCMalloc ( sc_strlen(tvalue) + 1 ) ;
		sc_strcpy ( expression->delt  , tvalue ) ;

		expression->delttype = EXP_DELT_DEFAULT ;
		
		LairAddCode ( expression->delt , LAIR_LVA_DELT , lairgentor.identor.deep ) ;
		LairAddCode ( "=" , -1 , -1 ) ;
		LairAddCode ( expression->left->delt , LAIR_RVA_DELT , lairgentor.identor.deep ) ;
		LairAddCode ( lexerc_get_operator (expression->handle) , -1 , -1 ) ;
		LairAddCode ( expression->right->delt , LAIR_RVA_DELT , lairgentor.identor.deep ) ;
		LairAddCode ( "\r\n" , LAIR_CR , -1 ) ;

		lairgentor.delt = lairgentor.delt + 1 ;
		
		if ( (EXP_DELT_DEFAULT == expression->left->delttype) && (EXP_DELT_DEFAULT == expression->right->delttype) ) {
			lairgentor.delt = 0 ;
		}

		if ( drop ) {
			SCFreeEx ( &expression->left->delt ) ;
			SCFreeEx ( &expression->right->delt ) ;
			SCFreeEx ( &expression->left ) ;
			SCFreeEx ( &expression->right ) ;
		}
		
	}

	return 1 ;
	
}

static void lairgentor_gen_pochain ( LGNOSIA* po_chain ) {

	//	author : Jelo Wang
	//	since : 20100719
	//	(C)TOK

	//	gen po_chain		

	if ( po_chain ) {
	
		SCClList* listlooper = 0 ;

		for ( listlooper = po_chain->context.head ; listlooper ; listlooper = listlooper->next ) {
	
			lairgentor_switcher ( (LGNOSIA* )listlooper->element ) ;
	
		}

	} 

}

static void lairgentor_gen_nechain ( LGNOSIA* ne_chain ) {

	//	author : Jelo Wang
	//	since : 20100719
	//	(C)TOK

	//	gen ne_chain

	if ( ne_chain ) {
		
		SCClList* listlooper = 0 ;
	
		for ( listlooper = ne_chain->context.head ; listlooper ; listlooper = listlooper->next ) {
		
			lairgentor_switcher ( (LGNOSIA* )listlooper->element ) ;
		
		}

	} 


}

char* LairGentorRun ( char* lairfile ) {

	//	author : Jelo Wang
	//	since : 20100107
	//	(C)TOK

	static int runtimes = 0 ;

	char* lair = 0 ;
	//	Create LAIR Pool
	int lairtex = LairCreate () ;
	int LabelMoiGlobal = 0 ;

	int lgay = 120 ;

	LairSetContext ( lairtex ) ;
	//	generate Global LabelMoi
	LabelMoiGlobal = LairLabelMoiNew () ;
	//	Set Current Monitor
	LairLabelMoiSetContext ( LabelMoiGlobal ) ;

	//	Initialize Lair Gentor
	lairgentor_ready () ; 
	
	LairAddCode ( "# Semo C()mpiler 0.3.1\r\n" , -1 , -1 ) ;
	LairAddCode ( "# Techniques of Knowledge\r\n" , -1 , -1 ) ;
	LairAddCode ( "# 突壳开源\r\n\r\n" , -1 , -1 ) ;	
	
	//	use compiling-render
	if ( SC_CR & semo->parameter ) {

		CORENRCreatePanel ( CORENR_RGB565 , 2048 , 2048 ) ;
		CORENRDrawText ( 15 , 10 , "Semo C()mpiler Compiling-Render" , 200 , 200 , 200 ) ;	
		CORENRDrawText ( 15 , 10+14 , "(C)Techniques of Knowledge" , 200 , 200 , 200 ) ;	
		CORENRDrawText ( 15 , 10+14+14 , "   http://www.tok.cc" , 200 , 200 , 200 ) ;	

	}
	
	while ( !lairgentor.stop ) {
		
		lairgentor_genv () ;

		switch ( lairgentor.form ) {
			
			case LGNOSIA_TOP_IDENT :
				
				if ( SC_CR & semo->parameter ) {		

					if ( SC_EXP & semo->parameter )
						lgay = CORENRLgaExpRender ( lairgentor.lgnosia , LGNOSIA_TOP_IDENT , 200 , lgay ) ;	
					//if ( SC_LGA & semo->parameter )
					//	CORENRCFDFSRender ( lairgentor.lgnosia , 0 , 1024 , lgay , 1024 , lgay , 1 ) ;
					
	 				//CORENRCFBFSRender ( corenr , lairgentor.lgnosia , 200 , lgay ) ;
				}
				
				lairgentor_gen_funcdef () ; 
				
			break ;
			
		}

		LgnosiaDestroy ( lairgentor.lgnosia ) ;		
		lairgentor_next () ;

	}

	//	generate live-scope and start register allocation
	if ( SC_ARM & semo->parameter ) {
		//	there are only 8 gernel registers for programer from ARM
		LairAllocRegister ( 1008 ) ;
	}
	
	//	Get Lair Codes
	lair = LairGetCode () ;
	LairClearCode () ;

	if ( SC_CR & semo->parameter ) {
		char* file = sc_strcat ( lairfile , ".cr.bmp" ) ;		
		CORENROutputPanel ( file ) ;
		CORENRDestroyPanel () ;
		SCFree ( file ) ;
	}

	if ( SC_LAIR & semo->parameter ) {
		void* file = SCHalFileOpen ( lairfile , "wb+" ) ;
		SCHalFileWrite ( file , lair , 1 , sc_strlen (lair) ) ;
		SCHalFileClose ( file ) ;
		//	release it here
		SCFree ( lairfile ) ;
	}

	SsaClean () ;
	SymboleUninstall () ;

	return lair ;

}

