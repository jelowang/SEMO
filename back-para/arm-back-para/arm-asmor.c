
/*

+	ARMv6 ASM generator , part of SC arm-back-para

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

# include "semo.h"
# include "schal.h"
# include "sccl.h"
# include "lair-grammar.h"
# include "lair-lexer.h"
# include "Regoc.h"
# include "frame.h"
# include "arm-asm.h"
# include "arm-asmor.h"

SCClString ArmAsm = { 0 , 0 , 0 , 0 , 0 } ;

# define DUA_OPERATOR(x) (LAIR_SUB <= x && x <= LAIR_HE)

static void asmor_arm_gen_exp ( SCClString* lair ) {

	//	author : Jelo Wang
	//	since : 20100723
	//	(C)TOK	

	//	generate the form like '%$R.n = An-1+An' into ARM codes
	//	results ADD Rn , An , An-1

	char* R = 0 ;
	char Rd [32] = {0} ;
	char Rn [32] = {0} ;
	char Rm [32] = {0} ;

	int state = 0 ;

	//	backup the orignal lexer
	int lexerac = LexerLairGet () ;

	//	generate a new laxer
	LexerLairSet ( LexerLairNew ( lair->data , LEXLAIR_FLITER_MODE ) ) ; 
	
	for ( LexerLairReady () ; !lexlair->stop ; ) {

		LexerLairGenv () ;		

		switch ( state ) {
			case 0 :
		
				//if ( LAIR_VREG == lexlair->v ) {		
					
					R = SCClItoa ( RegocGetRegister ( 0 , lexlair->lsn )  ) ;
					sc_strcat_ex ( "R" , R , Rd ) ;
					state = 1  ;

				//}
			break ;
			case 1 :
				if ( LAIR_VAR == lexlair->v ) {
					R = SCClItoa ( RegocGetRegister ( 0 , lexlair->lsn )  ) ;
					sc_strcat_ex ( "R" , R , Rn ) ;			
					state = 2  ;
				}
			break ;
			case 2 :
				if ( DUA_OPERATOR(lexlair->v) ) {
					state = 3  ;
				}
			break ;
			case 3 :
			 	if ( LAIR_VAR == lexlair->v ) {
			
					R = SCClItoa ( RegocGetRegister ( 0 , lexlair->lsn )  ) ;
					sc_strcat_ex ( "R" , R , Rm ) ;				
					lexlair->stop = 1 ;
 					THUMB_ADD_RdRnRm ( Rd , Rn , Rm ) ;
					
				}
			break ;
		}
			
	}

	//	set the orignal lexer
	LexerLairSet ( lexerac ) ;
		
}

static int asmor_arm_read_x () {

	
	//	author : Jelo Wang
	//	since : 20121031
	//	(C)TOK

	char R [32] = { 0 } ;
	char Rd [32] = { 0 } ;
	char Rn [32] = { 0 } ;
	char Rm [32] = { 0 } ;

	int state = 0 ;
	
	if ( LAIR_X != lexlair->v ) return 0 ;

	sc_strcpy ( R , SCClItoa ( RegocGetRegister ( 0 , lexlair->lsn ) ) ) ;
	sc_strcat_ex ( "R" , R , Rd ) ;

	while ( !lexlair->stop ) {

		LexerLairGenv () ;
				
		switch ( state ) {
			case 0 :		
 				if ( LAIR_EQU == lexlair->v ) {
					state = 1 ;
				}
			break ;
			case 1 :
				if ( LAIR_VAR == lexlair->v ) {
					if ( LAIR_CHROW != LexerLairHeadGenv(1) ) {
						char* R = SCClItoa ( RegocGetRegister ( 0 , lexlair->lsn )  ) ;
						sc_strcat_ex ( "R" , R , Rn ) ;
						state = 2 ;
					}
				} else if ( LAIR_INTNUM == lexlair->v ) {		
 					THUMB_MOV_RdImmed_8 ( Rd , lexlair->token ) ;		
				}
 			break ;
			case 2 :

				if ( LAIR_ADD == lexlair->v ) {
					char* R = 0 ;
					LexerLairGenv () ;
					R = SCClItoa ( RegocGetRegister ( 0 , lexlair->lsn ) ) ;
					sc_strcat_ex ( "R" , R , Rm ) ;
					THUMB_ADD_RdRnRm(Rd,Rn,Rm) ;
					return 1 ;			
				} else if ( LAIR_SUB == lexlair->v ) {
					char* R = 0 ;
					LexerLairGenv () ;
					R = SCClItoa ( RegocGetRegister ( 0 , lexlair->lsn ) ) ;
					sc_strcat_ex ( "R" , R , Rm ) ;
					THUMB_SUB_RdRnRm(Rd,Rn,Rm) ;
					return 1 ;			
				} else if ( LAIR_MUL == lexlair->v ) {
					char* R = 0 ;
					LexerLairGenv () ;
					R = SCClItoa ( RegocGetRegister ( 0 , lexlair->lsn ) ) ;
					sc_strcat_ex ( "R" , R , Rm ) ;
					THUMB_MUL_RdRm(Rn,Rm) ;
					//	MOV
					THUMB_MOV_RdRm(Rd,Rn) ;					
					return 1 ;			
				} else if ( LAIR_AND == lexlair->v ) {
					char* R = 0 ;
					LexerLairGenv () ;
					R = SCClItoa ( RegocGetRegister ( 0 , lexlair->lsn ) ) ;
					sc_strcat_ex ( "R" , R , Rm ) ;
					THUMB_AND_RdRm(Rn,Rm) ;
					//	MOV
					THUMB_MOV_RdRm(Rd,Rn) ;					
					return 1 ;			
				} else if ( LAIR_OR == lexlair->v ) {
					char* R = 0 ;
					LexerLairGenv () ;
					R = SCClItoa ( RegocGetRegister ( 0 , lexlair->lsn ) ) ;
					sc_strcat_ex ( "R" , R , Rm ) ;
					THUMB_ORR_RdRm(Rn,Rm) ;
					//	MOV
					THUMB_MOV_RdRm(Rd,Rn) ;					
					return 1 ;			
				} else if ( LAIR_SHL == lexlair->v ) {
					char* R = 0 ;
					LexerLairGenv () ;
					R = SCClItoa ( RegocGetRegister ( 0 , lexlair->lsn ) ) ;
					sc_strcat_ex ( "R" , R , Rm ) ;
					THUMB_LSL_RdRs(Rn,Rm) ;
					//	MOV
					THUMB_MOV_RdRm(Rd,Rn) ;					
					return 1 ;			
				}
				
				
			break ;  
		}

	}	

	return 1 ;

}


static int asmor_arm_read_variable_inf () {

	
	//	author : Jelo Wang
	//	since : 20100722
	//	(C)TOK

	char R [32] = { 0 } ;
	char Rd [32] = { 0 } ;
	char Rn [32] = { 0 } ;
	char Rm [32] = { 0 } ;

	int state = 0 ;
	
	if ( LAIR_VAR != lexlair->v ) return 0 ;

	sc_strcpy ( R , SCClItoa ( RegocGetRegister ( 0 , lexlair->lsn ) ) ) ;
	sc_strcat_ex ( "R" , R , Rd ) ;

	while ( !lexlair->stop ) {

		LexerLairGenv () ;
				
		switch ( state ) {
			case 0 :		
				if ( LAIR_EQU == lexlair->v ) {
					state = 1 ;
				}
			break ;
			case 1 :
/*				if ( LAIR_VAR == lexlair->v || LAIR_VREG == lexlair->v ) {
					char* R = SCClItoa ( RegocGetRegister ( 0 , lexlair->lsn )  ) ;
					sc_strcat_ex ( "R" , R , Rn ) ;				
 					THUMB_MOV_RdRn ( Rd , Rn ) ;
					return 1 ;
				} else if ( LAIR_INTNUM == lexlair->v ) {		
 					THUMB_MOV_RdImmed_8 ( Rd , lexlair->token ) ;
					return 1 ;				
				}
*/
 			break ;
		}

	}	

	return 1 ;

}

static int asmor_arm_read_virtual_reg () {

	
	//	author : Jelo Wang
	//	since : 20100722
	//	(C)TOK

	SCClString lair = { 0 , 0 , 0 , 0 , 0 } ;
/*
	if ( LAIR_VREG != lexlair->v ) return 0 ;
*/
	SCClStringInit ( &lair ) ;
	SCClStringAddStr ( &lair , lexlair->token ) ;
	
	while ( !lexlair->stop ) {

		LexerLairGenv () ;
		
		if ( LAIR_FEN == lexlair->v ) break ;

		SCClStringAddStr ( &lair , lexlair->token ) ;
		
	}

	SCClStringAdd ( &lair , '\0' ) ;	

	//	generate arm-exp form
	asmor_arm_gen_exp ( &lair ) ;

	return 1 ;

}

static int asmor_arm_read_peca () {

	//	author : Jelo Wang
	//	since : 20110212
	//	(C)TOK
	
	if ( LAIR_PE != lexlair->v && LAIR_CA != lexlair->v ) return 0 ;

	while ( !lexlair->stop ) {

		LexerLairGenv () ;
		
		if ( LAIR_CA == lexlair->v ) {
			LexerLairGenv () ;
			break ;
		}
		
		else if ( LAIR_PE == lexlair->v ) {			
			LexerLairGenv () ;
		}
		
	}

	THUMB_BL(lexlair->token) ;	
	
}

static char* asmor_asm_split_ssaalias ( char* name ) {

	//	author : Jelo Wang
	//	since : 20121031
	//	(C)TOK

	int looper = 0 ;

	for ( looper = 0 ; name[looper] != '.' ; )
		looper  ++ ;

	//	split alias
	name [looper-1] = '\0' ;

	return name ;
	
}

static int asmor_arm_read_stk () {

	//	author : Jelo Wang
	//	since : 20121031
	//	(C)TOK

	char R [32] = { 0 } ;
	char Rd [32] = { 0 } ;
	char Rn [32] = { 0 } ;
	
	switch ( lexlair->v ) {
		case LAIR_STKCRE :
			LexerLairGenv () ;
			SCClStringAddStr ( &ArmAsm , "SUB SP,SP," ) ;
			SCClStringAddStr ( &ArmAsm , lexlair->token ) ;
			SCClStringAddStr ( &ArmAsm , "\r\n") ;
			MemoryFrameCreate ( SCClAtoi(lexlair->token) , MF_STACK ) ;
		break ;
		case LAIR_STKREL :
			LexerLairGenv () ;
			SCClStringAddStr ( &ArmAsm , "ADD SP,SP," ) ;
			SCClStringAddStr ( &ArmAsm , lexlair->token ) ;
			SCClStringAddStr ( &ArmAsm , "\r\n" ) ;
		break ;		
		case LAIR_STKDEF :
			LexerLairGenv () ;
			MemoryFrameAdd ( lexlair->token , MF_STACK ) ;
		break ;		
		
		case LAIR_STKSET :
		{
			int offset = 0 ;

			SCClStringAddStr ( &ArmAsm , "MOV [SP+" ) ;		

			LexerLairGenv () ;
			offset = MemoryFrameGetAddress ( asmor_asm_split_ssaalias (lexlair->token) ) ;

			SCClStringAddStr ( &ArmAsm , SCClItoa (offset) ) ;	
			SCClStringAddStr ( &ArmAsm , "]," ) ;
			
			//skip LAIR_EQU
			LexerLairGenv () ;
			//Get LAIR_X
			LexerLairGenv () ;

			sc_strcpy ( R , SCClItoa ( RegocGetRegister ( 0 , lexlair->lsn ) ) ) ;
			sc_strcat_ex ( "R" , R , Rn ) ;
	
			SCClStringAddStr ( &ArmAsm , Rn ) ;
			SCClStringAddStr ( &ArmAsm , "\r\n" ) ;

		}
		break ;
		
		break ;
		case LAIR_STKGET :
		break ;
		case LAIR_STKPUSH :
		break ;
		case LAIR_STKPOP :
		break ;		

		default :
			return 0 ;
	}

	return 1 ;
	
}		

static int asmor_arm_read_symbol_inf () {

	//	author : Jelo Wang
	//	since : 20100722
	//	(C)TOK

	switch ( lexlair->v ) {
		case LAIR_VAR :
			return asmor_arm_read_variable_inf () ;
		break ;
		case LAIR_X :
			return asmor_arm_read_x () ;
		break ;
		case LAIR_PE :
		case LAIR_CA :			
			return asmor_arm_read_peca () ;
		break ;
		default : return 0 ;
	}

	return 1 ;
	
}

int asmor_arm_funcdef () {

	//	author : Jelo Wang
	//	since : 20100505
	//	(C)TOK

	int stack = 0 ;
	
	if ( LAIR_FUNCDEF != lexlair->v ) return 0 ;

	SCClStringAddStr ( &ArmAsm , "proc " ) ;
	SCClStringAddStr ( &ArmAsm , lexlair->token ) ;
	SCClStringAddStr ( &ArmAsm , "\r\n" ) ;

	//	parsing parameter scope of function
	stack = 0 ;
	while ( !lexlair->stop ) {
		
		LexerLairGenv () ;
		
		if ( LAIR_XKL == lexlair->v )
			stack ++ ;
		if ( LAIR_XKR == lexlair->v )
			stack -- ;
		
		if ( 0 == stack ) 
			break ;
		
	}

	//	parsing function body
	stack = 0 ;
	while ( !lexlair->stop ) {
		
		LexerLairGenv () ;
		
		if ( LAIR_DKL == lexlair->v )
			stack ++ ;
		if ( LAIR_DKR == lexlair->v )
			stack -- ;
		
		if ( 0 == stack ) 
			break ;		

		if ( LAIR_LABEL == lexlair->v ) {
			SCClStringAddStr ( &ArmAsm , sc_strcat ( lexlair->token , ":\r\n")  ) ;
			continue ;
		}

		if ( 1 == asmor_arm_read_stk () )
			continue ;
		
		if ( 1 == asmor_arm_read_symbol_inf () )
			continue ;
		
	}	
	
	SCClStringAddStr ( &ArmAsm , "end\r\n" ) ;
	
	
}

char* asmor_arm_run ( char* lair , char* asmout ) {

	//	author : Jelo Wang
	//	since : 20100505
	//	(C)TOK
	
	ASSERT ( lair ) ;
	
	LexerLairSet ( LexerLairNew ( lair , LEXLAIR_FLITER_MODE ) ) ; 
	LexerLairReady () ;

	SCClStringInit ( &ArmAsm ) ;

  	SEMO_ASSEMBLER_COPYRIGHTS;

	for ( LexerLairGenv () ; !lexlair->stop ; LexerLairGenv () ) {
		if ( LAIR_FUNCDEF == lexlair->v ) {
			asmor_arm_funcdef () ;
		} 
	} 

	SCClStringAdd ( &ArmAsm , '\0' ) ;

	if ( SC_SASM & semo->parameter ) {
		void* file = SCHalFileOpen ( asmout , "wb+" ) ;
		SCHalFileWrite ( file , ArmAsm.data , 1 , sc_strlen (ArmAsm.data) ) ;
		SCHalFileClose ( file ) ;
		SCFree ( asmout ) ;
	}	

	RegocAllocDestory () ;
	
	return ArmAsm.data ;

}

