
/*

+	ARMv6 ASM patterns , part of SC arm-back-para

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

# ifndef __ARM_ASM_PATTERN_
# define __ARM_ASM_PATTERN_

	//	author : Jelo Wang
	//	since : 20100508
	//	(C)TOK
# define SEMO_ASSEMBLER_COPYRIGHTS\
	SCClStringAddStr ( &ArmAsm , "# Semo C()mpiler 0.3.1\r\n") ;\
	SCClStringAddStr ( &ArmAsm , "# Techniques of Knowledge\r\n") ;\
	SCClStringAddStr ( &ArmAsm , "# 突壳开源\r\n\r\n") ;\
	
//	ADC <Rd> , <Rm>
# define THUMB_ADC_RdRm(Rd,Rm)\
	SCClStringAddStr ( &ArmAsm , sc_strcat ("ADC ",Rd) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (",",Rm) ) ;\
	SCClStringAddStr ( &ArmAsm , "\r\n" ) ;\

//	ADD <Rd> , <Rn> , #<immed_3>
# define THUMB_ADD_RdRnImmed_3(Rd,Rn,immed_3)\
	SCClStringAddStr ( &ArmAsm , sc_strcat ("ADD ",Rd) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (",",Rn) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (",",immed_3) ) ;\
	SCClStringAddStr ( &ArmAsm , "\r\n" ) ;\
	
//	ADD <Rd> , #<immed_8>
# define THUMB_ADD_RdImmed_8(Rd,immed_8)\
	SCClStringAddStr ( &ArmAsm , sc_strcat ("ADD ",Rd) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (",",immed_8) ) ;\
	SCClStringAddStr ( &ArmAsm , "\r\n" ) ;\
	
//	ADD <Rd> , <Rn> , <Rm>
# define THUMB_ADD_RdRnRm(Rd,Rn,Rm)\
	SCClStringAddStr ( &ArmAsm , sc_strcat ("ADD ",Rd) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (",",Rn) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (",",Rm) ) ;\
	SCClStringAddStr ( &ArmAsm , "\r\n" ) ;\

//	ADD <Rd> , <Rm>
# define THUMB_ADD_RdRm(Rd,Rm)\
	SCClStringAddStr ( &ArmAsm , sc_strcat ("ADD ",Rd) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (",",Rm) ) ;\
	SCClStringAddStr ( &ArmAsm , "\r\n" ) ;\

//	ADD <Rd> , PC , #<immed_8> * 4
# define THUMB_ADD_RdPcImmed_8M4(Rd,immed_8)\
	SCClStringAddStr ( &ArmAsm , sc_strcat ("ADD ",Rd) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (",","PC") ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (",",immed_8) ) ;\
	SCClStringAddStr ( &ArmAsm , "*4" ) ;\
	SCClStringAddStr ( &ArmAsm , "\r\n" ) ;\

//	ADD <Rd> , SP , #<immed_8> * 4
# define THUMB_ADD_RdSpImmed_8M4(Rd,immed_8)\
	SCClStringAddStr ( &ArmAsm , sc_strcat ("ADD ",Rd) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (",","SP") ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (",",immed_8) ) ;\
	SCClStringAddStr ( &ArmAsm , "*4" ) ;\
	SCClStringAddStr ( &ArmAsm , "\r\n" ) ;\

//	ADD SP , #<immed_7> * 4
# define THUMB_ADD_SpImmed_7M4(immed_7)\
	SCClStringAddStr ( &ArmAsm , sc_strcat ("ADD ","SP") ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (",",immed_7) ) ;\
	SCClStringAddStr ( &ArmAsm , "*4" ) ;\
	SCClStringAddStr ( &ArmAsm , "\r\n" ) ;\

//	AND <Rd> , <Rm>
# define THUMB_AND_RdRm(Rd,Rm)\
	SCClStringAddStr ( &ArmAsm , sc_strcat ("AND ",Rd) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (",",Rm) ) ;\
	SCClStringAddStr ( &ArmAsm , "\r\n" ) ;\

//	BL <target_addr>
# define THUMB_BL(target)\
	SCClStringAddStr ( &ArmAsm , "BL " ) ;\
	SCClStringAddStr ( &ArmAsm , target ) ;\
	SCClStringAddStr ( &ArmAsm , "\r\n" ) ;\

//	LDMIA <Rn>! , <register>
# define THUMB_LDMIA(Rn)\
	SCClStringAddStr ( &ArmAsm , sc_strcat ("LDMIA ",Rn) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (",","R0-R7") ) ;\
	SCClStringAddStr ( &ArmAsm , "\r\n" ) ;\

//	LDR <Rd> , [<Rn> , #<immed_5> * 4 ]
# define THUMB_LDR_RdRnImmed_5(Rd,Rn,Immed_5)\
	SCClStringAddStr ( &ArmAsm , sc_strcat ("LDR ",Rd) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (" , [ ",Rn) ) ;\
	SCClStringAddStr ( &ArmAsm , "," ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat ( Immed_5 , "*4") ) ;\
	SCClStringAddStr ( &ArmAsm , " ]" ) ;\
	SCClStringAddStr ( &ArmAsm , "\r\n" ) ;\

//	LDR <Rd> , [<Rn> , <Rm> ]
# define THUMB_LDR_RdRnRm(Rd,Rn,Rm)\
	SCClStringAddStr ( &ArmAsm , sc_strcat ("LDR ",Rd) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (" , [ ",Rn) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat ( "," , Rm) ) ;\
	SCClStringAddStr ( &ArmAsm , " ]" ) ;\
	SCClStringAddStr ( &ArmAsm , "\r\n" ) ;\

//	LDR <Rd> , [<PC> , #<immed_8> * 4 ]
# define THUMB_LDR_RdPcImmed_8(Rd,Immed_8)\
	SCClStringAddStr ( &ArmAsm , sc_strcat ("LDR ",Rd) ) ;\
	SCClStringAddStr ( &ArmAsm , " , [ PC , " ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat ( Immed_8 , "*4") ) ;\
	SCClStringAddStr ( &ArmAsm , " ]" ) ;\
	SCClStringAddStr ( &ArmAsm , "\r\n" ) ;\


//	LDR <Rd> , [<SP> , #<immed_8> * 4 ]
# define THUMB_LDR_RdSpImmed_8(Rd,Immed_8)\
	SCClStringAddStr ( &ArmAsm , sc_strcat ("LDR ",Rd) ) ;\
	SCClStringAddStr ( &ArmAsm , " , [ SP , " ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat ( Immed_8 , "*4") ) ;\
	SCClStringAddStr ( &ArmAsm , " ]" ) ;\
	SCClStringAddStr ( &ArmAsm , "\r\n" ) ;\

//	LDRB <Rd> , [<Rn> , #<immed_5> ]
# define THUMB_LDRB_RdSpImmed_5(Rd,Immed_5)\
	SCClStringAddStr ( &ArmAsm , sc_strcat ("LDRB ",Rd) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (" , [ ",Rn) ) ;\
	SCClStringAddStr ( &ArmAsm , "," ) ;\
	SCClStringAddStr ( &ArmAsm , Immed_5 ) ;\
	SCClStringAddStr ( &ArmAsm , " ]" ) ;\
	SCClStringAddStr ( &ArmAsm , "\r\n" ) ;\

//	LDRB <Rd> , [<Rn> , <Rm> ]
# define THUMB_LDRB_RdRnRm(Rd,Rn,Rm)\
	SCClStringAddStr ( &ArmAsm , sc_strcat ("LDRB ",Rd) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (" , [ ",Rn) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat ( "," , Rm) ) ;\
	SCClStringAddStr ( &ArmAsm , " ]" ) ;\
	SCClStringAddStr ( &ArmAsm , "\r\n" ) ;\

//	LDRH <Rd> , [<Rn> , #<immed_5> * 2 ]
# define THUMB_LDRH_RdSpImmed_5(Rd,Immed_5)\
	SCClStringAddStr ( &ArmAsm , sc_strcat ("LDRH ",Rd) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (" , [ ",Rn) ) ;\
	SCClStringAddStr ( &ArmAsm , "," ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat ( Immed_5 , "*2") ) ;\
	SCClStringAddStr ( &ArmAsm , " ]" ) ;\
	SCClStringAddStr ( &ArmAsm , "\r\n" ) ;\
	
//	LDRH <Rd> , [<Rn> , <Rm> ]
# define THUMB_LDRH_RdRnRm(Rd,Rn,Rm)\
	SCClStringAddStr ( &ArmAsm , sc_strcat ("LDRH ",Rd) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (" , [ ",Rn) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat ( "," , Rm) ) ;\
	SCClStringAddStr ( &ArmAsm , " ]" ) ;\
	SCClStringAddStr ( &ArmAsm , "\r\n" ) ;\

//	LDRSB <Rd> , [<Rn> , <Rm> ]
# define THUMB_LDRSB_RdRnRm(Rd,Rn,Rm)\
	SCClStringAddStr ( &ArmAsm , sc_strcat ("LDRSB ",Rd) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (" , [ ",Rn) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat ( "," , Rm) ) ;\
	SCClStringAddStr ( &ArmAsm , " ]" ) ;\
	SCClStringAddStr ( &ArmAsm , "\r\n" ) ;\

//	LDRSH <Rd> , [<Rn> , <Rm> ]
# define THUMB_LDRSH_RdRnRm(Rd,Rn,Rm)\
	SCClStringAddStr ( &ArmAsm , sc_strcat ("LDRSH ",Rd) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (" , [ ",Rn) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat ( "," , Rm) ) ;\
	SCClStringAddStr ( &ArmAsm , " ]" ) ;\
	SCClStringAddStr ( &ArmAsm , "\r\n" ) ;\

//	MOV <Rd> , #immed_8
# define THUMB_MOV_RdImmed_8(Rd,Immed_8)\
	SCClStringAddStr ( &ArmAsm , sc_strcat ("MOV ",Rd) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (",",Immed_8) ) ;\
	SCClStringAddStr ( &ArmAsm , "\r\n" ) ;\

//	MOV <Rd> , <Rn>
# define THUMB_MOV_RdRn(Rd,Rn)\
	SCClStringAddStr ( &ArmAsm , sc_strcat ("MOV ",Rd) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (",",Rn) ) ;\
	SCClStringAddStr ( &ArmAsm , "\r\n" ) ;\

//	MOV <Rd> , <Rm>
# define THUMB_MOV_RdRm(Rd,Rm)\
	SCClStringAddStr ( &ArmAsm , sc_strcat ("MOV ",Rd) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (",",Rm) ) ;\
	SCClStringAddStr ( &ArmAsm , "\r\n" ) ;\

//	STMIA <Rn>! , <register>
# define THUMB_STMIA(Rn)\
	SCClStringAddStr ( &ArmAsm , sc_strcat ("STMIA ",Rn) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (",","R0-R7") ) ;\
	SCClStringAddStr ( &ArmAsm , "\r\n" ) ;\

//	STR <Rd> , [<Rn> , #<immed_5> * 4 ]
# define THUMB_STR_RdRnImmed_5(Rd,Rn,Immed_5)\
	SCClStringAddStr ( &ArmAsm , sc_strcat ("STR ",Rd) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (",[",Rn) ) ;\
	SCClStringAddStr ( &ArmAsm , "," ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat ( "#" , Immed_5) ) ;\
	SCClStringAddStr ( &ArmAsm , "]" ) ;\
	SCClStringAddStr ( &ArmAsm , "\r\n" ) ;\

//	STR <Rd> , [<Rn> , <Rm> ]
# define THUMB_STR_RdRnRm(Rd,Rn,Rm)\
	SCClStringAddStr ( &ArmAsm , sc_strcat ("STR ",Rd) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (",[",Rn) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat ( "," , Rm) ) ;\
	SCClStringAddStr ( &ArmAsm , "]" ) ;\
	SCClStringAddStr ( &ArmAsm , "\r\n" ) ;\

//	STRB <Rd> , [<Rn> , #<immed_5> ]
# define THUMB_STRB_RdSpImmed_5(Rd,Immed_5)\
	SCClStringAddStr ( &ArmAsm , sc_strcat ("STRB ",Rd) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (" , [ ",Rn) ) ;\
	SCClStringAddStr ( &ArmAsm , "," ) ;\
	SCClStringAddStr ( &ArmAsm , Immed_5 ) ;\
	SCClStringAddStr ( &ArmAsm , " ]" ) ;\
	SCClStringAddStr ( &ArmAsm , "\r\n" ) ;\

//	STRB <Rd> , [<Rn> , <Rm> ]
# define THUMB_STRB_RdRnRm(Rd,Rn,Rm)\
	SCClStringAddStr ( &ArmAsm , sc_strcat ("STRB ",Rd) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (" , [ ",Rn) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat ( "," , Rm) ) ;\
	SCClStringAddStr ( &ArmAsm , " ]" ) ;\
	SCClStringAddStr ( &ArmAsm , "\r\n" ) ;\

//	STRH <Rd> , [<Rn> , #<immed_5> * 2 ]
# define THUMB_STRH_RdSpImmed_5(Rd,Immed_5)\
	SCClStringAddStr ( &ArmAsm , sc_strcat ("STRH ",Rd) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (" , [ ",Rn) ) ;\
	SCClStringAddStr ( &ArmAsm , "," ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat ( Immed_5 , "*2") ) ;\
	SCClStringAddStr ( &ArmAsm , " ]" ) ;\
	SCClStringAddStr ( &ArmAsm , "\r\n" ) ;\
	
//	STRH <Rd> , [<Rn> , <Rm> ]
# define THUMB_STRH_RdRnRm(Rd,Rn,Rm)\
	SCClStringAddStr ( &ArmAsm , sc_strcat ("STRH ",Rd) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (" , [ ",Rn) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat ( "," , Rm) ) ;\
	SCClStringAddStr ( &ArmAsm , " ]" ) ;\
	SCClStringAddStr ( &ArmAsm , "\r\n" ) ;\

//	SUB <Rd> , <Rn> , #<immed_3>
# define THUMB_SUB_RdRnImmed_3(Rd,Rn,immed_3)\
	SCClStringAddStr ( &ArmAsm , sc_strcat ("SUB ",Rd) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (",",Rn) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (",",immed_3) ) ;\
	SCClStringAddStr ( &ArmAsm , "\r\n" ) ;\

//	SUB <Rd> , #<immed_3>
# define THUMB_SUB_RdImmed_3(Rd,immed_3)\
	SCClStringAddStr ( &ArmAsm , sc_strcat ("SUB ",Rd) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (",",immed_3) ) ;\
	SCClStringAddStr ( &ArmAsm , "\r\n" ) ;\

//	SUB <Rd> , <Rn> , <Rm>
# define THUMB_SUB_RdRnRm(Rd,Rn,Rm)\
	SCClStringAddStr ( &ArmAsm , sc_strcat ("SUB ",Rd) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (",",Rn) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (",",Rm) ) ;\
	SCClStringAddStr ( &ArmAsm , "\r\n" ) ;\

//	SUB SP , #<immed_7> * 4
# define THUMB_SUB_SpImmed_7M4(immed_7)\
	SCClStringAddStr ( &ArmAsm , sc_strcat ("SUB ","SP") ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (",",immed_7) ) ;\
	SCClStringAddStr ( &ArmAsm , "*4" ) ;\
	SCClStringAddStr ( &ArmAsm , "\r\n" ) ;\

//	MUL <Rd> , <Rm>
# define THUMB_MUL_RdRm(Rd,Rm)\
	SCClStringAddStr ( &ArmAsm , sc_strcat ("MUL ",Rd) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (",",Rm) ) ;\
	SCClStringAddStr ( &ArmAsm , "\r\n" ) ;\

//	ORR <Rd> , <Rm>
# define THUMB_ORR_RdRm(Rd,Rm)\
	SCClStringAddStr ( &ArmAsm , sc_strcat ("OR ",Rd) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (",",Rm) ) ;\
	SCClStringAddStr ( &ArmAsm , "\r\n" ) ;\

//	LSL <Rd> , <Rs>
# define THUMB_LSL_RdRs(Rd,Rs)\
	SCClStringAddStr ( &ArmAsm , sc_strcat ("LSL ",Rd) ) ;\
	SCClStringAddStr ( &ArmAsm , sc_strcat (",",Rs) ) ;\
	SCClStringAddStr ( &ArmAsm , "\r\n" ) ;\

# endif

