
/*

+	ARMv6 assembler , part of SC arm-back-para

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
# include "elf-gen.h"
# include "arm-asm-lexer.h"
# include "arm-assemer.h"

ASSEMER_ARM AssemerArm = { THUMB16_SET } ; 

# define assemer_is_num(x) AASM_INTENUM <= x && x <= AASM_FLTENUM

int assemer_is_reg ( int reg ) {

	//	author : Jelo Wang
	//	since : 20100723
	//	(C)TOK

	if ( THUMB16_SET == AssemerArm.set ) {
		if ( THUMB16_OPC_R0 <= reg && reg <= THUMB16_OPC_R14 ) return 1 ;
	} else if ( ARM32_SET == AssemerArm.set ) {
		if ( ARM32_OPC_R0 <= reg && reg <= ARM32_OPC_R14 ) return 1 ;
	}
	
	return 0 ;
	
}

int assemer_get_reg_opc ( int reg ) {

	//	author : Jelo Wang
	//	since : 20100723
	//	(C)TOK	

	

}


void THUMB16_ADC_RdRm ( int Rd , int Rm ) {

	//	author : Jelo Wang
	//	since : 20100723
	//	(C)TOK	

	unsigned short value = 0 ;

	value |= Rm ;

	value |= Rd << 3 ;

	value |= THUMB16_OPC_ADC << 6 ;

	ElfGenAddTextValue ( value ) ;

}



void THUMB16_ADD_RdRnImmed_3 ( int Rd , int Rn , int immed_3 ) {

	
	//	author : Jelo Wang
	//	since : 20100730
	//	(C)TOK	

	//	pattern : ADD <Rd> , <Rn> , #<immed_3>
	
	unsigned short value = 0 ;	

	value |= Rd ;

	value |= Rn << 3 ;

	value |= immed_3 << 6 ;

	value |= THUMB16_OPC_ADD1 << 11 ;	

	ElfGenAddTextValue ( value ) ;	


}



void THUMB16_ADD_RdImmed_8 ( int Rd , int immed_8 ) {

	//	author : Jelo Wang
	//	since : 20100730
	//	(C)TOK	

	//	pattern : ADD <Rd> , #<immed_8>
	
	unsigned short value = 0 ;

	value |= Rd << 3 ;

	value |= THUMB16_OPC_ADD2 << 11 ;	

	ElfGenAddTextValue ( value ) ;
	
}

void THUMB16_ADD_RdRnRm ( int Rd , int Rn , int Rm ) {


	//	author : Jelo Wang
	//	since : 20100730
	//	(C)TOK	

	//	pattern : ADD <Rd> , <Rn> , <Rm>

	unsigned short value = 0 ;	

	value |= Rd ;	

	value |= Rn << 3 ;
	
	value |= Rm << 6 ;	

	value |= THUMB16_OPC_ADD3 << 9 ;	

	ElfGenAddTextValue ( value ) ;	

}


void THUMB16_ADD_RdRm ( int Rd ,int Rm ) {


	//	author : Jelo Wang
	//	since : 20100730
	//	(C)TOK	

	//	pattern : ADD <Rd> , <Rm>

	int RdV = Rd ;
	int RmV = Rm ;
	
	unsigned short value = 0 ;	

	//	<Rd> Specifies the register containing the first value , it can be any of R0 to R15.The register
	//	number is encoded in the instruction in H1 (most significant bit) and Rd (Remaining three bits).
	if ( THUMB16_OPC_R7 == RdV ) value |= 0x80 ;			//R8
	else if ( THUMB16_OPC_R8 == RdV ) value |= 0x88 ;		//R9
	else if ( THUMB16_OPC_R9 == RdV ) value |= 0x90 ;		//R10
	else if ( THUMB16_OPC_R10 == RdV ) value |= 0x98 ;		//R11
	else if ( THUMB16_OPC_R11 == RdV ) value |= 0xA0 ;		//R12
	else if ( THUMB16_OPC_R12 == RdV ) value |= 0xA8 ;		//R13
	else if ( THUMB16_OPC_R13 == RdV ) value |= 0xB0 ;		//R14
	else if ( THUMB16_OPC_R14 == RdV ) value |= 0xB8 ;		//R15
	else value |= RdV ;

	//	<Rm> Specifies the register containing the second value , it can be any of R0 to R15.The register
	//	number is encoded in the instruction in H2 (most significant bit) and Rm (Remaining three bits).	
	if ( THUMB16_OPC_R7 == RmV ) value |= 0x40 ;			//R8
	else if ( THUMB16_OPC_R8 == RmV ) value |= 0x48 ;		//R9
	else if ( THUMB16_OPC_R9 == RmV ) value |= 0x50 ;		//R10
	else if ( THUMB16_OPC_R10 == RmV ) value |= 0x58 ;		//R11
	else if ( THUMB16_OPC_R11 == RmV ) value |= 0x60 ;		//R12
	else if ( THUMB16_OPC_R12 == RmV ) value |= 0x68 ;		//R13
	else if ( THUMB16_OPC_R13 == RmV ) value |= 0x70 ;		//R14
	else if ( THUMB16_OPC_R14 == RmV ) value |= 0x78 ;		//R15
	else value |= RmV ;

	value |= THUMB16_OPC_ADD4 << 8 ;	

	ElfGenAddTextValue ( value ) ;	

}

void THUMB16_ADD_RdPCImmed_8 ( int Rd , int immed_8 ) {

	
	//	author : Jelo Wang
	//	since : 20100730
	//	(C)TOK	

	//	pattern : ADD <Rd> , PC , #<immed_8>*4

	unsigned short value = 0 ;	

	value |= immed_8 ;

	value |= Rd << 8 ;

	value |= THUMB16_OPC_ADD5 << 11 ;	

	ElfGenAddTextValue ( value ) ;	


}


void THUMB16_ADD_RdSPImmed_8 ( int Rd , int immed_8 ) {

	
	//	author : Jelo Wang
	//	since : 20100730
	//	(C)TOK	

	//	pattern : ADD <Rd> , SP , #<immed_8>*4

	unsigned short value = 0 ;	

	value |= immed_8 ;

	value |= Rd << 8 ;

	value |= THUMB16_OPC_ADD6 << 11 ;	

	ElfGenAddTextValue ( value ) ;	


}


void THUMB16_ADD_SPImmed_7 ( int immed_8 ) {


	//	author : Jelo Wang
	//	since : 20100730
	//	(C)TOK	

	//	pattern : MOV <Rd>,<Rn>

	unsigned short value = 0 ;	

	value |= immed_8 ;

	value |= THUMB16_OPC_ADD7 << 7 ;	

	ElfGenAddTextValue ( value ) ;	

}

void THUMB16_BX_Rm ( int Rm ) {

	//	author : Jelo Wang
	//	since : 20100726
	//	(C)TOK	

	//	pattern : MOV <Rd>,<Rn>
	
	unsigned short value = 0 ;

	value |= Rm << 3 ;

	value |= THUMB16_OPC_BX << 7 ;	

	ElfGenAddTextValue ( value ) ;

}

void THUMB16_MOV_RdImmed_8 ( int Rd , int immed_8 ) {

	//	author : Jelo Wang
	//	since : 20100726
	//	(C)TOK	

	//	pattern : MOV <Rd>,#<immed_8>
	
	unsigned short value = 0 ;

	value |= immed_8 ;

	value |= Rd << 8 ;

	value |= THUMB16_OPC_MOV1 << 11 ;

	ElfGenAddTextValue ( value ) ;

}

void THUMB16_MOV_RdRn ( int Rd , int Rn ) {

	//	author : Jelo Wang
	//	since : 20100726
	//	(C)TOK	

	//	pattern : MOV <Rd>,<Rn>
	
	unsigned short value = 0 ;

	value |= Rd ;

	value |= Rn << 3 ;

	value |= THUMB16_OPC_MOV3 << 6 ;
		
	ElfGenAddTextValue ( value ) ;

}


void THUMB16_STR_RdRnImmed_5 ( int Rd , int Rn , int immed_5 ) {

	//	author : Jelo Wang
	//	since : 20100729
	//	(C)TOK	

	//	pattern : STR <Rd> , [<Rn> , #<immed_5> * 4 ]
	
	unsigned short value = 0 ;

	value |= Rd ;	

	value |= Rn << 3 ;	

	value |= immed_5 << 6 ;	

	value |= THUMB16_OPC_STR1 << 11 ;

	ElfGenAddTextValue ( value ) ;

}


void THUMB16_STR_RdRnRm ( int Rd , int Rn , int Rm ) {

	//	author : Jelo Wang
	//	since : 20100729
	//	(C)TOK	

	//	pattern : STR <Rd> , [<Rn> , <Rm> ]
	
	unsigned short value = 0 ;

	value |= Rd ;

	value |= Rn << 3 ;

	value |= Rm << 6 ;	


	value |= THUMB16_OPC_STR2 << 9 ;

	ElfGenAddTextValue ( value ) ;

}


void THUMB16_STR_RdRnImmed_8 ( int Rd , int Rn , int immed_8 ) {

	//	author : Jelo Wang
	//	since : 20100729
	//	(C)TOK	

	//	pattern : STR <Rd> , [<Rn> , #<immed_8> * 4 ]
	
	unsigned short value = 0 ;

	value |= immed_8 ;
	
	value |= Rd << 8 ;

	value |= THUMB16_OPC_STR3 << 11 ;
	
	ElfGenAddTextValue ( value ) ;

}

static int assemer_thumb16_mov () {

	//	author : Jelo Wang
	//	since : 20100726
	//	(C)TOK	

	//	pattern : MOV <Rd>,#<immed_8>
	//	pattern : MOV <Rd>,<Rn>
	//	pattern : ADD SP , #<immed_7>*4

	//	generate reg
	lexerarm_genv () ;
	AssemerArm.Rd = lexarm->v ;
	//	skip AASM_DOU
	lexerarm_genv () ;
	//	generate reg
	lexerarm_genv () ;
	AssemerArm.Rn = lexarm->v ;		

	if (  assemer_is_num (lexarm->v) ) {
		THUMB16_MOV_RdImmed_8(AssemerArm.Rd,SCClAtoi(lexarm->token)) ;

	} else if ( assemer_is_reg ( lexarm->v )  ) {

	 	AssemerArm.Rn = lexarm->v ;
		THUMB16_MOV_RdRn(AssemerArm.Rd,AssemerArm.Rn) ;
		
	}

	return 1 ;

}


static int assemer_thumb16_str () {

	//	author : Jelo Wang
	//	since : 20100729
	//	(C)TOK	
	
	//	pattern : STR <Rd> , [<Rn> , #<immed_5> * 4 ]
	//	pattern : STR <Rd> , [<Rn> , <Rm> ]
	//	pattern : STR <Rd> , [SP , #<immed_8> * 4 ]
	
	//	generate reg
	lexerarm_genv () ;
	AssemerArm.Rd = lexarm->v ;

	//	skip AASM_ZKL
	lexerarm_genv () ;
	
	//	generate reg
	lexerarm_genv () ;
	AssemerArm.Rn = lexarm->v ;

	lexerarm_genv () ;

	//	form #<immed_5> * 4 , #<immed_8> * 4
	if ( AASM_SHARP == lexarm->v ) {

		int immed = 0 ;
		
		//	generate immed
		lexerarm_genv () ;

		immed = SCClAtoi(lexarm->token) ;
		
		if ( 31 >= immed ) THUMB16_STR_RdRnImmed_5 ( AssemerArm.Rd , AssemerArm.Rn , immed ) ;
		else if ( 255 >= immed ) THUMB16_STR_RdRnImmed_8 ( AssemerArm.Rd , AssemerArm.Rn , immed ) ;
		else SClog ( "ARM-ASSEMER : immed scale is overflowed\n") ;
		
	} else if ( assemer_is_reg (lexarm->v) ) {

		AssemerArm.Rm = lexarm->v ;
		THUMB16_STR_RdRnRm ( AssemerArm.Rd , AssemerArm.Rn , AssemerArm.Rm ) ;
		
	}
	
	//	skip AASM_ZKR
	lexerarm_genv () ;
	
	return 1 ;

}

static int assemer_thumb16_add () {

	//	author : Jelo Wang
	//	since : 20100730
	//	(C)TOK	
	
	//	pattern : ADD <Rd> , <Rn> , #<immed_3>
	//	pattern : ADD <Rd> , #<immed_8>
	//	pattern : ADD <Rd> , <Rn> , <Rm>
	//	pattern : ADD <Rd> , <Rm>
	//	pattern : ADD <Rd> , PC , #<immed_8>*4
	//	pattern : ADD <Rd> , SP , #<immed_8>*4
	//	pattern : ADD SP , #<immed_7>*4

	int lexv = 0;
	
	//	generate reg
	lexerarm_genv () ;
	AssemerArm.Rd = lexarm->v ;
	//	skip AASM_DOU
	lexerarm_genv () ;
	//	generate reg
	lexerarm_genv () ;
	AssemerArm.Rn = lexarm->v ;

	lexv = lexerarm_head_genv (1) ;

	if ( AASM_CHROW == lexv || AASM_ENTER == lexv ) { 
		
		//	pattern : ADD <Rd> , #<immed_8>
		//	pattern : ADD <Rd> , <Rm>		
		//	pattern : ADD SP , #<immed_7>*4	
		
		if ( assemer_is_num (lexarm->v) ) {
			
			int immed = 0 ;

			immed = SCClAtoi(lexarm->token) ;
			
			if ( 255 >= immed ) THUMB16_ADD_RdImmed_8 ( AssemerArm.Rd , immed ) ;
			else if ( 127 >= immed ) THUMB16_ADD_SPImmed_7 ( immed ) ;
			else SClog ( "ARM-ASSEMER : immed scale is overflowed\n") ;
			
		} else if ( assemer_is_reg (lexarm->v) ) {
			THUMB16_ADD_RdRm ( AssemerArm.Rd , AssemerArm.Rn ) ;
		}
		
	} else {

		//	pattern : ADD <Rd> , <Rn> , #<immed_3>
		//	pattern : ADD <Rd> , <Rn> , <Rm>		
		//	pattern : ADD <Rd> , PC , #<immed_8>*4
		//	pattern : ADD <Rd> , SP , #<immed_8>*4		
		//	skip AASM_DOU
		lexerarm_genv () ;		
		lexerarm_genv () ;

		if ( assemer_is_reg (lexarm->v) ) {
			
			AssemerArm.Rm = lexarm->v ;
 			
			THUMB16_ADD_RdRnRm ( AssemerArm.Rd , AssemerArm.Rn , AssemerArm.Rm ) ;

		} else if ( assemer_is_num (lexarm->v) ) {

			int immed = 0 ;

			//	Link Register : R13
			//	Stack Pointer : R14
			//	Program Counter : 15
			
			if ( THUMB16_OPC_R14 == AssemerArm.Rn ) {
				immed = SCClAtoi(lexarm->token) ;
				THUMB16_ADD_RdPCImmed_8 ( AssemerArm.Rd , immed ) ;
			} else if ( THUMB16_OPC_R12 == AssemerArm.Rn ) {
				immed = SCClAtoi(lexarm->token) ;
				THUMB16_ADD_RdSPImmed_8 ( AssemerArm.Rd , immed ) ;
			} else {
				immed = SCClAtoi(lexarm->token) ;
				THUMB16_ADD_RdRnImmed_3 ( AssemerArm.Rd , AssemerArm.Rn , immed ) ;			
			}
				
		}
		
	}
	
	return 1 ;
	
	
}

static int assemer_add_symbol ( int st_name , int shndx , int st_info , char* name ) {

	//	author : Jelo Wang
	//	since : 20100729
	//	(C)TOK	

	Elf32_Sym elf32_sym = { 0 } ; 

	//	st_value is a offset from start of .text
	elf32_sym.st_value = 0 ;
	//	generate st_info
	elf32_sym.st_info = st_info ;
	//	get shndx
	elf32_sym.st_shndx = shndx ;
	//	get st_name
	elf32_sym.st_name = st_name ;
	//	insert symbol name into strtab
	ElfGenStrtabInsertString ( name  ) ;	
		
	//	generate st_size
	elf32_sym.st_size = 0 ;
	//	insert symbol into symtab
	ElfGenSymtabInsertSymbol ( elf32_sym ) ;
	
	return 1 ;
	
}

static int assemer_thumb16_genmap () {

	
	//	author : Jelo Wang
	//	since : 20100729
	//	(C)TOK
	
	assemer_add_symbol ( \
		ElfGenStrtabGetEntry () ,\
		ElfGenGetShndx (".text") ,\
		ELF32_ST_INFO(STB_LOCAL,STT_SECTION) ,\
		".text"
	) ;

	//	thumb16 set identifer
	assemer_add_symbol ( \
		ElfGenStrtabGetEntry () ,\
		ElfGenGetShndx (".text") ,\
		ELF32_ST_INFO(STB_LOCAL,STT_FUNC) ,\
		"$t"
	) ;
	
	assemer_add_symbol ( \
		ElfGenStrtabGetEntry () ,\
		ElfGenGetShndx (".text") ,\
		ELF32_ST_INFO(STB_LOCAL,STT_FUNC) ,\
		"$d"
	) ;
	
	assemer_add_symbol ( \
		ElfGenStrtabGetEntry () ,\
		SHN_ABS ,\
		0 ,\
		"BuildAttributes$$THUMB_ISAv1$M$PE$A:L22$X:L11$S22$~IW$~STKCKD$~SHL$OSPACE$PRES8"
	) ;

	return 1 ;

}

static int assemer_thumb16_proc () {
	
	//	author : Jelo Wang
	//	since : 20100728
	//	(C)TOK

	int entrysec = 0 ;
	Elf32_Sym elf32_sym = { 0 } ; 
	
	if ( AASM_PROCDEF != lexarm->v ) return 0 ;

	//	get scale of text section
	entrysec = ElfGenGetTextScale ( 2 ) ;
	//	st_value is a offset from start of .text
	elf32_sym.st_value = entrysec ;
	//	generate st_info
	elf32_sym.st_info = ELF32_ST_INFO(STB_GLOBAL,STT_FUNC);
	//	get shndx
	elf32_sym.st_shndx = ElfGenGetShndx (".text") ;
	//	get st_name
	elf32_sym.st_name = ElfGenStrtabGetEntry () ;
	//	insert symbole name into strtab
	ElfGenStrtabInsertString ( lexarm->token  ) ;	
		
	while ( !lexarm->stop ) {

		lexerarm_genv () ;			

		if ( THUMB16_KEY_END == lexarm->v )
			break ;
		
		if ( THUMB16_SET_MOV == lexarm->v ) 
			assemer_thumb16_mov () ;
	
		else if ( THUMB16_SET_STR == lexarm->v )
			assemer_thumb16_str () ;

		else if ( THUMB16_SET_ADD == lexarm->v )
			assemer_thumb16_add () ;

	}
	
	//	generate st_size
	elf32_sym.st_size = ElfGenGetTextScale ( 2 ) - entrysec ;
	//	insert symbol into symtab
	ElfGenSymtabInsertSymbol ( elf32_sym ) ;
		
	return 1 ;

}

int assemer_arm_run ( char* asm , char* out , int* codes ) {
	
	//	author : Jelo Wang
	//	since : 20100723
	//	(C)TOK
	
	lexerarm_set ( lexerarm_new ( asm , AssemerArm.set ) ) ;
	lexerarm_setmode ( LEXERARM_FLITER_MODE ) ;
		
	if ( THUMB16_SET == AssemerArm.set )
		assemer_thumb16_genmap () ;

	while ( !lexarm->stop ) {

		lexerarm_genv () ;			
		
		if ( AASM_PROCDEF == lexarm->v ) {
			assemer_thumb16_proc () ;
		}

	}

	lexerarm_destroy () ;	

	if ( THUMB16_SET == AssemerArm.set ) { 
		ElfGen ( out , 2 ) ;
		*codes = ElfGenGetTextScale ( 2 )  ;
	} else if ( ARM32_SET == AssemerArm.set ) {
		ElfGen ( out , 4 ) ;
		*codes = ElfGenGetTextScale ( 4 )  ;
	}
		
	return 1 ;

}
