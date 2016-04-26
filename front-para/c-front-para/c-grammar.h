
/*

+	grammar of SC C-front-para

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

//	author:Jelo Wang
//	notes : syntanx analyze,leixcal analyze,semantic analyze,code generator
//	notes : operator,EXPR,data type,codes set
//	since : 2008220
//	(C)TOK

//	Lexical Def
//	ALPHA = ((a|b|c|...z)|(A|B|C|...Z))
//	DIG = (0|1|2|3|4|5|6|7|8|9)

# ifndef __C_GRAMMAR_
# define __C_GRAMMAR_

# define C_FEN				2    //	;
# define C_EQU				3    //	=
# define C_ZKL 				4    //	[
# define C_ZKR 				5    //	]
# define C_XKL 				6    //	(
# define C_XKR 				7    //	)
# define C_DKL 				8    //	{
# define C_DKR 				9    //	}
# define C_DOU 				10   //	,
# define C_XY  				11   //	'
# define C_DY  				12   //	"
# define C_ZSL 				13   ///	*
# define C_ZSR 				14   //	*/

# define C_CHA 				15   //	smart string like '*'
# define C_STR 				16   //	long string like "*"

# define C_MAO 				17   //	:
# define C_ASK 				18   //	?

# define C_JAA 				19   //	++
# define C_JNN 				20   //	--
# define C_TAN 				21   //	!
# define C_SUB 				22   //	-
# define C_ADD 				23   //	+
# define C_HU  				24   //	|
# define C_MUL 				25   //	*
# define C_DIV 				26   //	/
# define C_MOD 				27   //	%
# define C_SHR 				28	 //	>>
# define C_SHL 				29   //	<<
# define C_LT  				30   //	<
# define C_LE  				31   //	<=
# define C_EQ  				32   //	==
# define C_NE  				33   //	!=
# define C_GT  				34   //	>
# define C_GE  				35   //	>=
# define C_EE  				36   //	=
# define C_AND 				37   //	&&
# define C_OR  				38   //	||
# define C_HE  				39   //	&
# define C_EXO 				40   //	~
# define C_BEY 				41   //	^=
# define C_BEO 				42   //	|=
# define C_BEA 				43   //	&=
# define C_BEB 				44   //	%=
# define C_SLD 				45   //	/=
# define C_SLM 				46   //	*=
# define C_SLS 				47   //	-=
# define C_SLA 				48   //	+=
# define C_SHLL 				49  //	<<=
# define C_SHRR 				50  //	>>=
# define C_PNT 				51   //	->
# define C_FIL 				52   //	.
# define C_PPT 				53   //	**
# define C_YHH 				54   //	^
# define C_ASC 				55   //	ASCII

//	Keyworlds of C langauge
# define C_BREAK       			100		//	'break'
# define C_CASE        			101		//	'case'
# define C_FOR         			102		//	'for'
# define C_CONTINUE    		103		//	'continue'
# define C_GOTO        			104		//	'goto'
# define C_DEFAULT     		105		//	'default'
# define C_DO          			106		//	'do'
# define C_WHILE      		 	107		//	'while'
# define C_ELSE        			108		//	'else'
# define C_SWITCH      		109		//	'switch'
# define C_IF          			110		//	'if'
# define C_AUTO        			111		//	'auto'
# define C_CHAR        			112		//	'char'
# define C_CONST       			113		//	'const'
# define C_EXTERN      		114		//	'extern'
# define C_INT         			115		//	'int'
# define C_LONG        			116		//	'long'
# define C_REGISTER    		117		//	'register'
# define C_RETURN      		118		//	'return'
# define C_SHORT       			119		//	'short'
# define C_SIGNED      		120		//	'sigend'
# define C_STATIC      			121		//	'static'
# define C_UNSIGNED    		122		//	'unsigned'
# define C_VOID        			123		//	'void'
# define C_VOLATILE    		124		//	'volatile'
# define C_FLOAT     			125 		//	'float'
# define C_DOUBLE      		126		//	'double'
# define C_SIZEOF      			127		//	'sizeof'
# define C_STRUCT      		128		//	'struct'
# define C_ENUM        			129		//	'enum'
# define C_TYPEDEF     		130		//	'typedef'
# define C_UNION      		 	131		//	'union'

//	Reference
# define C_VAR_REF         		500 		//	general variables references | Lexical Def : (ALPHA|_)*|(ALPHA|DIG|_)
# define C_ARRAY_REF      		501		//	array references | Lexical Def : (ALPHA|_)*|(ALPHA|DIG|_)([DIG])
# define C_FUNC_REF     		502	 	//	function references | Lexical Def : (ALPHA|_)*|((DIG))
# define C_OBJ_REF			503	 	//	'struct.name' | Lexical Def : (ALPHA|_)*|((.(ALPHA|DIG|_)*)|(->(ALPHA|DIG|_)*)) 
//	For Pointer Reference
# define C_P_VAR_REF         	504 		//	references of variables pointer | Lexical Def : (*)*(ALPHA|_)*|(ALPHA|DIG|_)
# define C_P_ARRAY_REF      	505		//	references of array pointer | Lexical Def : (*)*(ALPHA|_)*|(ALPHA|DIG|_)([DIG])
# define C_P_FUNC_REF     	506	 	//	references of function pointer | Lexical Def : (*)*(ALPHA|_)*|((DIG))

//	Precessor Keyworlds
# define C_PI_INCLUDE	 		2040	//	#inlcude
# define C_PI_DEFINE				2041	//	#define
# define C_PI_IFDEF				2042	//	#ifdef
# define C_PI_IFNDEF				2043	//	#ifndef
# define C_PI_ENDIF				2044	//	#endif
# define C_PI_IF					2045	//	#if
# define C_PI_ELIF				2046	//	#elif
# define C_PI_ELSE				2047	//	#else
# define C_PI_PAGRAMA			2048	//	#pagrama
# define C_PI_DEFINED	 		2049	//	#defined

//	Digital numbers
# define C_INT_NUM      		2100 	//	integer
# define C_FLT_NUM      		2101 	//	float
# define C_HEX_NUM      		2102 	//	hex
# define C_FLT_ENUM     		2103 	//	float e
# define C_INT_ENUM     		2104 	//	int e
# define C_NEG_INTNUM      	2105 	//	integer
# define C_NEG_FLTNUM      	2106 	//	float
# define C_NEG_HEXNUM      	2107 	//	hex
# define C_NEG_FLTENUM     	2108 	//	float e
# define C_NEG_INTENUM     	2109 	//	int e
			 	
//# define C_PVAR        		2501 	//	*var
//# define C_PPVAR      		2502 	//	**var
//# define C_PARRAY			2503 	//	*a[];
//# define C_PPARRAY	 		2504 	//	**a[];
//# define C_PFUNCDEF    		2505 	//	int *function(){}
//# define C_PPFUNCDEF   		2506 	//	int **function(){}
//# define C_PFUNCDEC    		2507 	//	int *functi();
//# define C_PPFUNCDEC   		2508 	//	int **functi();
//# define C_PFUNCCAL    		2509 	//	*func() error
//# define C_PPFUNCCAL   		2510 	//	**func() error 

//# define C_SELF				2511 	//	self type
//# define C_SELFPT			2512 	//	self type pointer
# define C_EOS         			'\0' 		//	ending of string

# define C_ENTER				2515	//	'\n'
# define C_ESCAPE			2516	//	'\'
# define C_CHROW			2517	//	'\r'
# define C_SPACE				2518	//	0x20
# define C_TABLEK			2518	//	'\t'

//	Decelaration Symbols
# define C_FUNC_DEC			3000	//	Funciton Decelaration

//	Defination of Symbols
# define C_VAR_DEF         		3500 	//	variables def
# define C_ARRAY_DEF      		3501	//	array def
# define C_FUNC_DEF     		3502	//	function def
# define C_OBJ_DEF     		3502	//	structs def

//	bit map of define_head
//	0000 0000 0000 0000 0000 0000 0000 0000
//	format : type[bit:scale]
//	(1) char[1:1]
//	(2) int[2:1]
//	(3) float[3:1]
//	(4) double[4:1]
//	(5) short[5:1]
//	(6) long[6:1]
//	(7) long long[7:1]	 -> unbit (6) when (7) actived.
//	(8) self-defined[8:1]
//	(9) signed[9:1]
//	(10) unsigned[10:1]
//	(11) register[11:1]
//	(12) auto[12:1]
//	(13) static[13:1]
//	(14) extern[14:1]

# define C_VOID_BIT			0
# define C_CHAR_BIT			0x1
# define C_INT_BIT			0x2
# define C_FLOAT_BIT			0x4
# define C_DOUBLE_BIT		0x8
# define C_SHORT_BIT		0x10
# define C_LONG_BIT			0x20
# define C_LONG_LONG_BIT	0x40
# define C_SELFDEF_BIT		0x80
# define C_SIGNED_BIT		0x100
# define C_UNSIGNED_BIT		0x200
# define C_REGISTER_BIT		0x400
# define C_AUTO_BIT			0x800
# define C_STATIC_BIT		0x1000
# define C_EXTERN_BIT		0x2000

# define C_OPRAND(x)
//	(C_FUNC_REF == x ||C_VAR_REF == x || C_ARRAY_REF == x || C_STRUCT == x )

# endif

