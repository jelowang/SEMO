
/*

+	Grammar of LAIR's codes , SC mid-para

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

# ifndef __LAIR_GRAMMAR
# define __LAIR_GRAMMAR

# define LAIR_FEN					2    //;
# define LAIR_EQU					3    //=
# define LAIR_ZKL 					4    //[
# define LAIR_ZKR 				5    //]
# define LAIR_XKL 					6    //(
# define LAIR_XKR 				7    //)
# define LAIR_DKL 				8    //{
# define LAIR_DKR 				9    //}
# define LAIR_DOU 				10   //,
# define LAIR_XY  					11   //'
# define LAIR_DY  					12   //"
# define LAIR_ZSL 					13   ///*
# define LAIR_ZSR 				14   //*/
# define LAIR_CHA 				15   //char
# define LAIR_STR 				16   //string
# define LAIR_MAO 				17   //:
# define LAIR_ASK 				18   //?

# define LAIR_JAA 				19   //++
# define LAIR_JNN 				20   //--
# define LAIR_TAN 				21   //!
# define LAIR_SUB 				22   //-
# define LAIR_ADD 				23   //+
# define LAIR_HU  					24   //|
# define LAIR_MUL 				25   //*
# define LAIR_DIV 					26   ///
# define LAIR_MOD 				27   //%
# define LAIR_SHR 				28	 //>>
# define LAIR_SHL 				29   //<<
# define LAIR_LT  					30   //<
# define LAIR_LE  					31   //<=
# define LAIR_EQ  					32   //==
# define LAIR_NE  					33   //!=
# define LAIR_GT  					34   //>
# define LAIR_GE  					35   //>=
# define LAIR_EE  					36   //=
# define LAIR_AND 				37   //&&
# define LAIR_OR  					38   //||
# define LAIR_HE  					39   //&
# define LAIR_EXO 				40   //~
# define LAIR_BEY 				41   //^=
# define LAIR_BEO 				42   //|=
# define LAIR_BEA 				43   //&=
# define LAIR_BEB 				44   //%=
# define LAIR_SLD 				45   ///=
# define LAIR_SLM 				46   //*=
# define LAIR_SLS 					47   //-=
# define LAIR_SLA 				48   //+=
# define LAIR_SHLL 				49  //<<=
# define LAIR_SHRR 				50  //>>=
# define LAIR_PNT 				51   //->
# define LAIR_FIL 					52   //.
# define LAIR_PPT 				53   //**
# define LAIR_YHH 				54   //^
# define LAIR_ASC 				55   //ASCII

# define LAIR_BREAK       			1001
# define LAIR_CASE        			1002
# define LAIR_FOR         			1003
# define LAIR_CONTINUE    			1004
# define LAIR_GOTO        			1005
# define LAIR_DEFAULT     			1006
# define LAIR_DO          			1007
# define LAIR_WHILE      		 	1008
# define LAIR_ELSE        			1009
# define LAIR_SWITCH      			1010
# define LAIR_IF          				1011
# define LAIR_RVALUE				1012
# define LAIR_REGISTER          		1013
# define LAIR_EXTERN	          		1015
# define LAIR_RETURN	          		1016
# define LAIR_SIZEOF	          		1017
# define LAIR_STATIC	          		1018
# define LAIR_VOLATILE	          	1019

# define LAIR_INTNUM      			2100 //integer
# define LAIR_FLTNUM      			2101 //float
# define LAIR_HEXNUM      			2102 //hex
# define LAIR_FLTENUM     			2103 //float e
# define LAIR_INTENUM     			2104 //int e
# define LAIR_MUS_INTNUM      		2105 //integer
# define LAIR_MUS_FLTNUM      		2106 //float
# define LAIR_MUS_HEXNUM      		2107 //hex
# define LAIR_MUS_FLTENUM     		2108 //float e
# define LAIR_MUS_INTENUM     		2109 //int e
# define LAIR_FUNCDEF				2033
# define LAIR_VAR         				2034 //general variables name
# define LAIR_VARDEF         			2035 //general variables name
# define LAIR_ARRAY      		 		2036
# define LAIR_FUNCCAL     			2037 //function call
# define LAIR_LABEL     				2038 //label
# define LAIR_STKCRE					2039
# define LAIR_STKSET					2040
# define LAIR_STKGET					2041
# define LAIR_STKPUSH				2042
# define LAIR_STKPOP					2043
# define LAIR_STKREL					2044
# define LAIR_STKDEF					2045
# define LAIR_X						2046	//	intermedia value
# define LAIR_PE						2047 //PARAMETER
# define LAIR_CA						2048 //function call
	
# define LAIR_EOS         			'\0' //ending of string
# define LAIR_ENTER				2515
# define LAIR_ESCAPE				2516
# define LAIR_CHROW				2517
# define LAIR_SPACE				2518
# define LAIR_TABLEK				2519

# endif

