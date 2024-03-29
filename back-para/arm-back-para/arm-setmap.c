
/*

+	Part of SC arm-back-para

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

# include "arm-assemer.h"

ARM_SETMAP arm32_insset [ ARM32_INSSET_TOTALL ] = {

	//	author : Jelo Wang
	//	since : 20100723
	//	(C)TOK
	
	{ "ADC" , ARM32_SET_ADC } ,
	{ "ADD" , ARM32_SET_ADD } ,
	{ "AND" , ARM32_SET_AND } ,
	{ "B" , ARM32_SET_B } ,
	{ "BL" , ARM32_SET_BL } ,
	{ "BIC" , ARM32_SET_BIC } ,
	{ "BKPT" , ARM32_SET_BKPT } ,
	{ "BLX" , ARM32_SET_BLX } ,
	{ "BX" , ARM32_SET_BX } ,
	{ "BXJ" , ARM32_SET_BXJ } ,
	{ "CDP" , ARM32_SET_CDP } ,
	{ "CLZ" , ARM32_SET_CLZ } ,
	{ "CMN" , ARM32_SET_CMN } ,
	{ "CMP" , ARM32_SET_CMP } ,
	{ "CPS" , ARM32_SET_CPS } ,
	{ "CPY" , ARM32_SET_CPY } ,
	{ "EOR" , ARM32_SET_EOR } ,
	{ "LDC" , ARM32_SET_LDC } ,
	{ "LDM" , ARM32_SET_LDM } ,
	{ "LDR" , ARM32_SET_LDR } ,
	{ "LDRB" , ARM32_SET_LDRB } ,
	{ "LDRBT" , ARM32_SET_LDRBT } ,
	{ "LDRD" , ARM32_SET_LDRD } ,
	{ "LDREX" , ARM32_SET_LDREX } ,
	{ "LDRH" , ARM32_SET_LDRH } ,
	{ "LDRSB" , ARM32_SET_LDRSB } ,
	{ "LDRSH" , ARM32_SET_LDRSH } ,
	{ "LDRT" , ARM32_SET_LDRT } ,
	{ "MCR" , ARM32_SET_MCR } ,
	{ "MCRR" , ARM32_SET_MCRR } ,
	{ "MLA" , ARM32_SET_MLA } ,
	{ "MOV" , ARM32_SET_MOV } , 
	{ "MRC" , ARM32_SET_MRC } ,
	{ "MRRC" , ARM32_SET_MRRC } ,
	{ "MRS" , ARM32_SET_MRS } ,
	{ "MSR" , ARM32_SET_MSR } ,
	{ "MUL" , ARM32_SET_MUL } ,
	{ "MVN" , ARM32_SET_MVN } ,
	{ "ORR" , ARM32_SET_ORR } ,
	{ "PKHBT" , ARM32_SET_PKHBT } ,
	{ "PKHTB" , ARM32_SET_PKHTB } ,
	{ "PLD" , ARM32_SET_PLD } ,
	{ "QADD" , ARM32_SET_QADD } ,
	{ "QADD16" , ARM32_SET_QADD16 } ,
	{ "QADD8" , ARM32_SET_QADD8 } ,
	{ "QADDSUBX" , ARM32_SET_QADDSUBX } ,
	{ "QDADD" , ARM32_SET_QDADD } ,
	{ "QDSUB" , ARM32_SET_QDSUB } ,
	{ "QSUB" , ARM32_SET_QSUB } ,
	{ "QSUB16" , ARM32_SET_QSUB16 } ,
	{ "QSUBB" , ARM32_SET_QSUBB } ,
	{ "QSUBADDX" , ARM32_SET_QSUBADDX } ,
	{ "REV" , ARM32_SET_REV } ,
	{ "REV16" , ARM32_SET_REV16 } ,
	{ "REVSH" , ARM32_SET_REVSH } ,
	{ "RFE" , ARM32_SET_RFE } ,
	{ "RSB" , ARM32_SET_RSB } ,
	{ "RSC" , ARM32_SET_RSC } ,
	{ "SADD16" , ARM32_SET_SADD16 } ,
	{ "SADD8" , ARM32_SET_SADD8 } ,
	{ "SADDSUBX" , ARM32_SET_SADDSUBX } ,
	{ "SBC" , ARM32_SET_SBC } ,
	{ "SEL" , ARM32_SET_SEL } ,
	{ "SETEND" , ARM32_SET_SETEND } ,
	{ "SHADD16" , ARM32_SET_SHADD16 } ,
	{ "SHADD8" , ARM32_SET_SHADD8 } ,
	{ "SHADDSUBX" , ARM32_SET_SHADDSUBX } ,
	{ "SHSUB16" , ARM32_SET_SHSUB16 } ,
	{ "SHSUB8" , ARM32_SET_SHSUB8 } ,
	{ "SHSUBADDX" , ARM32_SET_SHSUBADDX } ,
	{ "SMLA" , ARM32_SET_SMLA } ,
	{ "SMLAD" , ARM32_SET_SMLAD } ,
	{ "SMLAL" , ARM32_SET_SMLAL } ,
	{ "SMLALD" , ARM32_SET_SMLALD } ,
	{ "SMLAW" , ARM32_SET_SMLAW } ,
	{ "SMLSD" , ARM32_SET_SMLSD } ,
	{ "SMLSLD" , ARM32_SET_SMLSLD } ,
	{ "SMMLA" , ARM32_SET_SMMLA } ,
	{ "SMMLS" , ARM32_SET_SMMLS } ,
	{ "SMMUL" , ARM32_SET_SMMUL } ,
	{ "SMUAD" , ARM32_SET_SMUAD } ,
	{ "SMUL" , ARM32_SET_SMUL } ,
	{ "SMULL" , ARM32_SET_SMULL } ,
	{ "SMULW" , ARM32_SET_SMULW } ,
	{ "SMUSD" , ARM32_SET_SMUSD } ,
	{ "SRS" , ARM32_SET_SRS } ,
	{ "SSAT" , ARM32_SET_SSAT } ,
	{ "SSAT16" , ARM32_SET_SSAT16 } ,
	{ "SSUB16" , ARM32_SET_SSUB16 } ,
	{ "SSUB8" , ARM32_SET_SSUB8 } ,
	{ "SSUBADDX" , ARM32_SET_SSUBADDX } ,
	{ "STC" , ARM32_SET_STC } ,
	{ "STM" , ARM32_SET_STM } ,
	{ "STR" , ARM32_SET_STR } ,
	{ "STRB" , ARM32_SET_STRB } ,
	{ "STRBT" , ARM32_SET_STRBT } ,
	{ "STRD" , ARM32_SET_STRD } ,
	{ "STREX" , ARM32_SET_STREX } ,
	{ "STRH" , ARM32_SET_STRH } ,
	{ "STRT" , ARM32_SET_STRT } ,
	{ "SUB" , ARM32_SET_SUB } ,
	{ "SWI" , ARM32_SET_SWI } ,
	{ "SWP" , ARM32_SET_SWP } ,
	{ "SWPB" , ARM32_SET_SWPB } ,
	{ "SXTAB" , ARM32_SET_SXTAB } ,
	{ "SXTAB16" , ARM32_SET_SXTAB16 } ,
	{ "SXTAH" , ARM32_SET_SXTAH } ,
	{ "SXTB" , ARM32_SET_SXTB } ,
	{ "SXTB16" , ARM32_SET_SXTB16 } ,
	{ "SXTH" , ARM32_SET_SXTH } ,
	{ "TEQ" , ARM32_SET_TEQ } ,
	{ "TST" , ARM32_SET_TST } ,
	{ "UADD16" , ARM32_SET_UADD16 } ,
	{ "UADD8" , ARM32_SET_UADD8 } ,
	{ "UADDSUBX" , ARM32_SET_UADDSUBX } ,
	{ "UHADD16" , ARM32_SET_UHADD16 } ,
	{ "UHADD8" , ARM32_SET_UHADD8 } ,
	{ "UHADDSUBX" , ARM32_SET_UHADDSUBX } ,
	{ "UHSUB16" , ARM32_SET_UHSUB16 } ,
	{ "UHSUB8" , ARM32_SET_UHSUB8 } ,
	{ "UHSUBADDX" , ARM32_SET_UHSUBADDX } ,
	{ "UMAAL" , ARM32_SET_UMAAL } ,
	{ "UMLAL" , ARM32_SET_UMLAL } ,
	{ "UMULL" , ARM32_SET_UMULL } ,
	{ "UQADD16" , ARM32_SET_UQADD16 } ,
	{ "UQADD8" , ARM32_SET_UQADD8 } ,
	{ "UQADDSUBX" , ARM32_SET_UQADDSUBX } ,
	{ "UQSUB16" , ARM32_SET_UQSUB16 } ,
	{ "UQSUB8" , ARM32_SET_UQSUB8 } ,
	{ "UQSUBADDX" , ARM32_SET_UQSUBADDX } ,
	{ "USAD8" , ARM32_SET_USAD8 } ,
	{ "USADA8" , ARM32_SET_USADA8 } ,
	{ "USAT" , ARM32_SET_USAT } ,
	{ "USAT16" , ARM32_SET_USAT16 } ,
	{ "USUB16" , ARM32_SET_USUB16 } ,
	{ "USUB8" , ARM32_SET_USUB8 } ,
	{ "USUBADDX" , ARM32_SET_USUBADDX } ,
	{ "UXTAB" , ARM32_SET_UXTAB } ,
	{ "UXTAB16" , ARM32_SET_UXTAB16 } ,
	{ "UXTAH" , ARM32_SET_UXTAH } ,
	{ "UXTB" , ARM32_SET_UXTB } ,
	{ "UXTB16" , ARM32_SET_UXTB16 } ,
	{ "UXTH" , ARM32_SET_UXTH } ,
	{ "R0" , ARM32_OPC_R0 } ,
	{ "R1" , ARM32_OPC_R1 } ,
	{ "R2" , ARM32_OPC_R2 } ,
	{ "R3" , ARM32_OPC_R3 } ,
	{ "R4" , ARM32_OPC_R4 } ,
	{ "R5" , ARM32_OPC_R5 } ,
	{ "R6" , ARM32_OPC_R6 } ,
	{ "R7" , ARM32_OPC_R7 } ,
	{ "R8" , ARM32_OPC_R8 } ,
	{ "R9" , ARM32_OPC_R9 } ,
	{ "R10" , ARM32_OPC_R10 } ,
	{ "R11" , ARM32_OPC_R11 } ,
	{ "R12" , ARM32_OPC_R12 } ,
	{ "R13" , ARM32_OPC_R13 } ,
	{ "R14" , ARM32_OPC_R14 } ,
	{ "proc" , ARM32_KEY_PROC } ,
	{ "end" , ARM32_KEY_END } ,	

} ;

ARM_SETMAP thumb16_insset [ THUMB16_INSSET_TOTALL ] = {

	//	author : Jelo Wang
	//	since : 20100723
	//	(C)TOK
	
	{ "ADC" , THUMB16_SET_ADC } ,
	{ "ADD" , THUMB16_SET_ADD } ,
	{ "AND" , THUMB16_SET_AND } ,
	{ "ASR" , THUMB16_SET_ASR } ,	
	{ "B" , THUMB16_SET_B } ,
	{ "BIC" , THUMB16_SET_BIC } ,
	{ "BKPT" , THUMB16_SET_BKPT } ,	
	{ "BL" , THUMB16_SET_BL } ,
	{ "BLX" , THUMB16_SET_BLX } ,
	{ "BX" , THUMB16_SET_BX } ,
	{ "CMN" , THUMB16_SET_CMN } ,
	{ "CMP" , THUMB16_SET_CMP } ,
	{ "CPS" , THUMB16_SET_CPS } ,
	{ "CPY" , THUMB16_SET_CPY } ,
	{ "EOR" , THUMB16_SET_EOR } ,
	{ "LDMIA" , THUMB16_SET_LDMIA } ,
	{ "LDR" , THUMB16_SET_LDR } ,
	{ "LDRB" , THUMB16_SET_LDRB } ,
	{ "LDRH" , THUMB16_SET_LDRH } ,
	{ "LDRSB" , THUMB16_SET_LDRSB } ,
	{ "LDRSH" , THUMB16_SET_LDRSH } ,
	{ "LDRT" , THUMB16_SET_LDRT } ,
	{ "LSL" , THUMB16_SET_LSL } ,	
	{ "LSR" , THUMB16_SET_LSR } ,		
	{ "MOV" , THUMB16_SET_MOV } , 
	{ "MUL" , THUMB16_SET_MUL } ,
	{ "MVN" , THUMB16_SET_MVN } ,
	{ "NEG" , THUMB16_SET_NEG } ,	
	{ "ORR" , THUMB16_SET_ORR } ,
	{ "POP" , THUMB16_SET_POP } ,
	{ "PUSH" , THUMB16_SET_PUSH } ,
	{ "REV" , THUMB16_SET_REV } ,
	{ "REV16" , THUMB16_SET_REV16 } ,
	{ "REVSH" , THUMB16_SET_REVSH } ,
	{ "ROR" , THUMB16_SET_ROR } ,
	{ "SBC" , THUMB16_SET_SBC } ,
	{ "SETEND" , THUMB16_SET_SETEND } ,
	{ "STMIA" , THUMB16_SET_STMIA } ,
	{ "STR" , THUMB16_SET_STR } ,
	{ "STRB" , THUMB16_SET_STRB } ,
	{ "STRH" , THUMB16_SET_STRH } ,
	{ "SUB" , THUMB16_SET_SUB } ,
	{ "SWI" , THUMB16_SET_SWI } ,
	{ "SXTB" , THUMB16_SET_SXTB } ,
	{ "SXTH" , THUMB16_SET_SXTH } ,
	{ "TST" , THUMB16_SET_TST } ,
	{ "UXTB" , THUMB16_SET_UXTB } ,
	{ "UXTH" , THUMB16_SET_UXTH } ,
	{ "R0" , THUMB16_OPC_R0 } ,
	{ "R1" , THUMB16_OPC_R1 } ,
	{ "R2" , THUMB16_OPC_R2 } ,
	{ "R3" , THUMB16_OPC_R3 } ,
	{ "R4" , THUMB16_OPC_R4 } ,
	{ "R5" , THUMB16_OPC_R5 } ,
	{ "R6" , THUMB16_OPC_R6 } ,
	{ "R7" , THUMB16_OPC_R7 } ,
	{ "R8" , THUMB16_OPC_R8 } ,
	{ "R9" , THUMB16_OPC_R9 } ,
	{ "R10" , THUMB16_OPC_R10 } ,
	{ "R11" , THUMB16_OPC_R11 } ,
	{ "R12" , THUMB16_OPC_R12 } ,
	{ "R13" , THUMB16_OPC_R13 } ,
	{ "R14" , THUMB16_OPC_R14 } ,
	{ "proc" , THUMB16_KEY_PROC } ,
	{ "end" , THUMB16_KEY_END } ,	

} ;
