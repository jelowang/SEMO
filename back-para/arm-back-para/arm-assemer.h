
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

# ifndef __ASSEMER_ARM
# define __ASSEMER_ARM

typedef enum {

	//	author : Jelo Wang
	//	since : 20100723
	//	(C)TOK

	//	notes : ARM Instructions SET

		//	THUMB16 instructions : 143
	ARM32_SET_ADC ,
	ARM32_SET_ADD ,
	ARM32_SET_AND ,
	ARM32_SET_B ,
	ARM32_SET_BL ,
	ARM32_SET_BIC ,
	ARM32_SET_BKPT ,
	ARM32_SET_BLX ,
	ARM32_SET_BX ,
	ARM32_SET_BXJ ,
	ARM32_SET_CDP ,
	ARM32_SET_CLZ ,
	ARM32_SET_CMN ,
	ARM32_SET_CMP ,
	ARM32_SET_CPS ,
	ARM32_SET_CPY ,
	ARM32_SET_EOR ,
	ARM32_SET_LDC ,
	ARM32_SET_LDM ,
	ARM32_SET_LDR ,
	ARM32_SET_LDRB ,
	ARM32_SET_LDRBT ,
	ARM32_SET_LDRD ,
	ARM32_SET_LDREX ,
	ARM32_SET_LDRH ,
	ARM32_SET_LDRSB ,
	ARM32_SET_LDRSH ,
	ARM32_SET_LDRT ,
	ARM32_SET_MCR ,
	ARM32_SET_MCRR ,
	ARM32_SET_MLA ,
	ARM32_SET_MOV , 
	ARM32_SET_MRC ,
	ARM32_SET_MRRC ,
	ARM32_SET_MRS ,
	ARM32_SET_MSR ,
	ARM32_SET_MUL ,
	ARM32_SET_MVN ,
	ARM32_SET_ORR ,
	ARM32_SET_PKHBT ,
	ARM32_SET_PKHTB ,
	ARM32_SET_PLD ,
	ARM32_SET_QADD ,
	ARM32_SET_QADD16 ,
	ARM32_SET_QADD8 ,
	ARM32_SET_QADDSUBX ,
	ARM32_SET_QDADD ,
	ARM32_SET_QDSUB ,
	ARM32_SET_QSUB ,
	ARM32_SET_QSUB16 ,
	ARM32_SET_QSUBB ,
	ARM32_SET_QSUBADDX ,
	ARM32_SET_REV ,
	ARM32_SET_REV16 ,
	ARM32_SET_REVSH ,
	ARM32_SET_RFE ,
	ARM32_SET_RSB ,
	ARM32_SET_RSC ,
	ARM32_SET_SADD16 ,
	ARM32_SET_SADD8 ,
	ARM32_SET_SADDSUBX ,
	ARM32_SET_SBC ,
	ARM32_SET_SEL ,
	ARM32_SET_SETEND ,
	ARM32_SET_SHADD16 ,
	ARM32_SET_SHADD8 ,
	ARM32_SET_SHADDSUBX ,
	ARM32_SET_SHSUB16 ,
	ARM32_SET_SHSUB8 ,
	ARM32_SET_SHSUBADDX ,
	ARM32_SET_SMLA ,
	ARM32_SET_SMLAD ,
	ARM32_SET_SMLAL ,
	ARM32_SET_SMLALD ,
	ARM32_SET_SMLAW ,
	ARM32_SET_SMLSD ,
	ARM32_SET_SMLSLD ,
	ARM32_SET_SMMLA ,
	ARM32_SET_SMMLS ,
	ARM32_SET_SMMUL ,
	ARM32_SET_SMUAD ,
	ARM32_SET_SMUL ,
	ARM32_SET_SMULL ,
	ARM32_SET_SMULW ,
	ARM32_SET_SMUSD ,
	ARM32_SET_SRS ,
	ARM32_SET_SSAT ,
	ARM32_SET_SSAT16 ,
	ARM32_SET_SSUB16 ,
	ARM32_SET_SSUB8 ,
	ARM32_SET_SSUBADDX ,
	ARM32_SET_STC ,
	ARM32_SET_STM ,
	ARM32_SET_STR ,
	ARM32_SET_STRB ,
	ARM32_SET_STRBT ,
	ARM32_SET_STRD ,
	ARM32_SET_STREX ,
	ARM32_SET_STRH ,
	ARM32_SET_STRT ,
	ARM32_SET_SUB ,
	ARM32_SET_SWI ,
	ARM32_SET_SWP ,
	ARM32_SET_SWPB ,
	ARM32_SET_SXTAB ,
	ARM32_SET_SXTAB16 ,
	ARM32_SET_SXTAH ,
	ARM32_SET_SXTB ,
	ARM32_SET_SXTB16 ,
	ARM32_SET_SXTH ,
	ARM32_SET_TEQ ,
	ARM32_SET_TST ,
	ARM32_SET_UADD16 ,
	ARM32_SET_UADD8 ,
	ARM32_SET_UADDSUBX ,
	ARM32_SET_UHADD16 ,
	ARM32_SET_UHADD8 ,
	ARM32_SET_UHADDSUBX ,
	ARM32_SET_UHSUB16 ,
	ARM32_SET_UHSUB8 ,
	ARM32_SET_UHSUBADDX ,
	ARM32_SET_UMAAL ,
	ARM32_SET_UMLAL ,
	ARM32_SET_UMULL ,
	ARM32_SET_UQADD16 ,
	ARM32_SET_UQADD8 ,
	ARM32_SET_UQADDSUBX ,
	ARM32_SET_UQSUB16 ,
	ARM32_SET_UQSUB8 ,
	ARM32_SET_UQSUBADDX ,
	ARM32_SET_USAD8 ,
	ARM32_SET_USADA8 ,
	ARM32_SET_USAT ,
	ARM32_SET_USAT16 ,
	ARM32_SET_USUB16 ,
	ARM32_SET_USUB8 ,
	ARM32_SET_USUBADDX ,
	ARM32_SET_UXTAB ,
	ARM32_SET_UXTAB16 ,
	ARM32_SET_UXTAH ,
	ARM32_SET_UXTB ,
	ARM32_SET_UXTB16 ,
	ARM32_SET_UXTH ,

		//	ASM Keyword : 2	
	ARM32_KEY_PROC ,
	ARM32_KEY_CALL ,
	ARM32_KEY_END ,		
	
} ARM32S ;

typedef enum {

	//	author : Jelo Wang
	//	since : 20100723
	//	(C)TOK

	//	notes : THUMB Instructions SET

		//	THUMB16 instructions : 48
	THUMB16_SET_ADC ,
	THUMB16_SET_ADD ,
	THUMB16_SET_AND ,
	THUMB16_SET_ASR ,	
	THUMB16_SET_B ,
	THUMB16_SET_BIC ,
	THUMB16_SET_BKPT ,	
	THUMB16_SET_BL ,
	THUMB16_SET_BLX ,
	THUMB16_SET_BX ,
	THUMB16_SET_CMN ,
	THUMB16_SET_CMP ,
	THUMB16_SET_CPS ,
	THUMB16_SET_CPY ,
	THUMB16_SET_EOR ,
	THUMB16_SET_LDMIA ,
	THUMB16_SET_LDR ,
	THUMB16_SET_LDRB ,
	THUMB16_SET_LDRH ,
	THUMB16_SET_LDRSB ,
	THUMB16_SET_LDRSH ,
	THUMB16_SET_LDRT ,
	THUMB16_SET_LSL ,	
	THUMB16_SET_LSR ,		
	THUMB16_SET_MOV , 
	THUMB16_SET_MUL ,
	THUMB16_SET_MVN ,
	THUMB16_SET_NEG ,	
	THUMB16_SET_ORR ,
	THUMB16_SET_POP ,
	THUMB16_SET_PUSH ,
	THUMB16_SET_REV ,
	THUMB16_SET_REV16 ,
	THUMB16_SET_REVSH ,
	THUMB16_SET_ROR ,
	THUMB16_SET_SBC ,
	THUMB16_SET_SETEND ,
	THUMB16_SET_STMIA ,
	THUMB16_SET_STR ,
	THUMB16_SET_STRB ,
	THUMB16_SET_STRH ,
	THUMB16_SET_SUB ,
	THUMB16_SET_SWI ,
	THUMB16_SET_SXTB ,
	THUMB16_SET_SXTH ,
	THUMB16_SET_TST ,
	THUMB16_SET_UXTB ,
	THUMB16_SET_UXTH ,
	
		//	ASM Keyword : 2	
	THUMB16_KEY_PROC ,
	THUMB16_KEY_CALL ,
	THUMB16_KEY_END ,		
	
} THUMB16S ;

typedef struct {
	
	//	author : Jelo Wang
	//	since : 20100723
	//	(C)TOK

	char* name ;
	int	type ;

} ARM_SETMAP ;

typedef enum {

	//	author : Jelo Wang
	//	since : 20100723
	//	(C)TOK

	THUMB16_SET ,
	ARM32_SET ,
	
} ARM_SET ;


typedef enum {

	//	author : Jelo Wang
	//	since : 20100723
	//	(C)TOK	

	//	notes : thumb16 set opcodes

	THUMB16_OPC_R0 = 0x1 ,
	THUMB16_OPC_R1 = 0x2 ,
	THUMB16_OPC_R2 = 0x3 ,
	THUMB16_OPC_R3 = 0x4 ,
	THUMB16_OPC_R4 = 0x5 ,
	THUMB16_OPC_R5 = 0x6 ,
	THUMB16_OPC_R6 = 0x7 ,
	THUMB16_OPC_R7 = 0x8 ,
	THUMB16_OPC_R8 = 0x9 ,
	THUMB16_OPC_R9 = 0xa ,
	THUMB16_OPC_R10 = 0xb ,
	THUMB16_OPC_R11 = 0xc ,
	THUMB16_OPC_R12 = 0xd ,
	THUMB16_OPC_R13 = 0xe ,
	THUMB16_OPC_R14 = 0xf ,
	THUMB16_OPC_ADC = 0x105 ,
	THUMB16_OPC_ADD1 = 0xE ,
	THUMB16_OPC_ADD2 = 0x6 ,	
	THUMB16_OPC_ADD3 = 0xC ,	
	THUMB16_OPC_ADD4 = 0x44 ,	
	THUMB16_OPC_ADD5 = 0x14 ,	
	THUMB16_OPC_ADD6 = 0x15 ,	
	THUMB16_OPC_ADD7 = 0x160 ,		
	THUMB16_OPC_BX	= 0x8E ,
	THUMB16_OPC_STR1 = 0xC ,
	THUMB16_OPC_STR2 = 0x28 ,
	THUMB16_OPC_STR3 = 0x12 ,
	THUMB16_OPC_MOV1 = 0x4 ,
	THUMB16_OPC_MOV2 = 0x70 ,
	THUMB16_OPC_MOV3 = 0x118 ,	
	
} THUMB16_OPC ;

typedef enum {
	
	//	author : Jelo Wang
	//	since : 20100723
	//	(C)TOK	

	//	notes : arm32 set opcodes
	
	ARM32_OPC_R0 = 0x1 ,
	ARM32_OPC_R1 = 0x2 ,
	ARM32_OPC_R2 = 0x3 ,
	ARM32_OPC_R3 = 0x4 ,
	ARM32_OPC_R4 = 0x5 ,
	ARM32_OPC_R5 = 0x6 ,
	ARM32_OPC_R6 = 0x7 ,
	ARM32_OPC_R7 = 0x8 ,
	ARM32_OPC_R8 = 0x9 ,
	ARM32_OPC_R9 = 0xa ,
	ARM32_OPC_R10 = 0xb ,
	ARM32_OPC_R11 = 0xc ,
	ARM32_OPC_R12 = 0xd ,
	ARM32_OPC_R13 = 0xe ,
	ARM32_OPC_R14 = 0xf ,	
	
} ARM32_OPC ;

typedef struct {

	//	author : Jelo Wang
	//	since : 20100723
	//	(C)TOK	

	int set ;

	int Rd ;
	int Rn ;
	int Rm ;
	int immed ;

} ASSEMER_ARM ;

//	ARM32 instructions key : 143
//	Register key : 15
//	ASM key : 3
# define ARM32_INSSET_TOTALL 143+15+3
//	THUMB16 instructions key : 48
//	Register key : 15
//	ASM key : 3
# define THUMB16_INSSET_TOTALL 48+15+3

# endif

extern ASSEMER_ARM AssemerArm ;
extern ARM_SETMAP arm32_insset [] ;
extern ARM_SETMAP thumb16_insset []  ;

extern int assemer_is_reg ( int reg ) ;
extern int assemer_get_reg_opc ( int reg ) ;
extern int assemer_arm_run ( char* asm , char* out , int* codes ) ;

