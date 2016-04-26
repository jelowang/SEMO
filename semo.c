
/*

+	Semo Compiler 0.3.1

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

# include <time.h>
# include "c-lexer.h"
# include "c-presor.h"
# include "c-parser.h"
# include "arm-asmor.h"
# include "arm-assemer.h"
# include "lair-gentor.h"
# include "java-gentor.h"
# include "schal.h"
# include "sccl.h"
# include "semo.h"

COMPILER* semo = 0 ;

static void help () {
	
	//	author : Jelo Wang
	//	since : 20091125
	//	(C)TOK	
 
	SClog ( 	"\
\t - Semo C()mpiler 0.3.0 -\n\
\t - 突壳开源 Techniques of Knowledge -\n\n\
Format : \n\
\t sc option1-option2...-optionn <SET module=para> file1 file2...filen \n\
Options :\n\
\t -c \t\t compile c language based on C99\n\
\t -cpp \t\t compile c plus plus language\n\
\t -java \t\t compile java language\n\
\t -po \t\t export pre-compiling results to external file\n\
\t -lair \t\t export IR to external file\n\
\t -sasm \t\t export semo assembly code to external file\n\
\t -cr \t\t enable compiling-render and export results\n\
\t -elf \t\t generate object-file with ELF format as defaul\n\
\t -link \t\t compile and link\n\
\t -nlink \t compile only as default\n\
\t -arm \t\t compile for the ARM architecture as default\n\
\t -X86 \t\t compile for the X86 architecture\n\
\t -c0 \t\t using '__stdcall' for parameters transfer\n\
\t -c1 \t\t using '__cdecl' for parameters transfer\n\
\t -c2 \t\t using '__fastcall' for parameters transfer\n\
\t -c3 \t\t using '__armcall' for parameters transfer as default\n\
\t -c2j \t\t convert C99 codes to JAVA codes\n\
\n\
SET :\n\
\t module : po pre-compiling\n\
\t parameter : dc delete comments\n\
\t parameter : ms marco substitution\n\
\t parameter : dm both dc and ms features by default\n\
\n\
\t module : cr compiling-render\n\
\t parameter : lga lgnosia graph\n\
\t parameter : exp expression graph\n\
\t parameter : ig interference graph of live scope\n\
\n\
\t module : linker\n\
\t parameter : file1 file2...filen\n\
\n\
Example :\n\
\t sc test.c -c -X86\n\
\t sc -c -lair -sasm test.c test2.c test3.c\n\
\t sc -c -lair -sasm -cr -arm SET po=dc,ms SET cr=lga test.c test2.c\n\
\t sc test.c SET linker=test2.lib test3.lib\n\
\n\
More :\n\
\t For further more info you can visit the offical site\n\
\t http://www.tok.cc \n\
" ) ;
	
}

static int sc_command_parser ( COMPILER* compiler , int argc , char** argv ) {

	//	author : Jelo Wang
	//	since : 20091125
	//	(C)TOK

	//	parsing the **argv with a C-LEXER which is a lexical analyzer of c lanuage.
	
	int walker = 0 ;
	int state = 0 ;
	int results = 0 ;

	SCClString str = {0} ;

	//	format argv with SCClString
	for ( walker = 0 ; walker < argc-1 ; walker ++ ) {
		
		SCClStringAddStr ( &str , argv[walker + 1] ) ;
		SCClStringAdd ( &str , 0x20 ) ;	

	}	
	SCClStringAdd ( &str , '\0' ) ;
	
	//	Use lexer of c for the commands analysing
	lexerc_set ( lexerc_new ( str.data , LEXERC_DEFAULT_MODE ) ) ; 

	SCClStringReset ( &str ) ;

	//	Analyzing compiling parameters
	while ( !lexc->stop ) {

		lexerc_genv () ;

		switch ( state ) {
			
			case 0 :
				if ( !sc_strcmp ( lexc->token , "-" ) ) state = 1 ;
				else if ( lexc->token && !sc_strcmp ( lexc->token , "SET" ) ) {				
					if ( '.' != lexerc_look (0) ) {						
						state = 3 ;
						lexerc_skip_blank () ;
					} else {			
						//	save file name wit str			
						SCClStringAddStr ( &str , "SET" ) ;
						SCClStringAddStr ( &str , "." ) ;			
						lexerc_next () ;
						state = 2 ;
					}
				} else {
		
					SCClStringAddStr ( &str , lexc->token ) ;

					if ( '.' == lexerc_look (0) ) {
						SCClStringAddStr ( &str , "." ) ;
						lexerc_next () ;
					}
					
					state = 2 ;

				}
			break ;
			
			case 1 :
				
				if ( !sc_strcmp ( lexc->token , "c" ) ) {

					compiler->parameter |= SC_C99 ;

					if ( compiler->parameter & SC_CPP ) {
						SClog ( "Language '-cpp' has already specified\n" ) ;
						goto ErrorRelease ;						
					}	

					if ( compiler->parameter & SC_JAVA ) {
						SClog ( "Language '-java' has already specified\n" ) ;
						goto ErrorRelease ;						
					}							

				} else if ( !sc_strcmp ( lexc->token , "cpp" ) ) {

					compiler->parameter |= SC_CPP ;				
					SClog ( "The compiler is not supports CPP yet \n" ) ;						

					if ( compiler->parameter & SC_C99 ) {
						SClog ( "Language '-c' has already specified\n" ) ;
						goto ErrorRelease ;						
					}	
					
					if ( compiler->parameter & SC_JAVA ) {
						SClog ( "Language '-java' has already specified\n" ) ;
						goto ErrorRelease ;						
					}							

					goto ErrorRelease ;

				} else if ( !sc_strcmp ( lexc->token , "java" ) ) {

					compiler->parameter |= SC_JAVA ;			
					SClog ( "The compiler is not supports JAVA yet \n" ) ;

					if ( compiler->parameter & SC_C99 ) {
						SClog ( "Language '-c' has already specified\n" ) ;
						goto ErrorRelease ;						
					}	

					if ( compiler->parameter & SC_CPP ) {
						SClog ( "Language '-cpp' has already specified\n" ) ;
						goto ErrorRelease ;						
					}						

					goto ErrorRelease ;

				} else if ( !sc_strcmp ( lexc->token , "po" ) ) {

					compiler->parameter |= SC_PO ;						

				} else if ( !sc_strcmp ( lexc->token , "lair" ) ) {

					compiler->parameter |= SC_LAIR ;						

				} else if ( !sc_strcmp ( lexc->token , "sasm" ) ) {

					compiler->parameter |= SC_SASM ;						

				} else if ( !sc_strcmp ( lexc->token , "cr" ) ) {

					compiler->parameter |= SC_CR ;						

				} else if ( !sc_strcmp ( lexc->token , "c0" ) ) {

					//	__stdcall
					compiler->parameter |= SC_C0 ;						

				} else if ( !sc_strcmp ( lexc->token , "c1" ) ) {

					//	__cdcel				
					compiler->parameter |= SC_C1 ;						

				} else if ( !sc_strcmp ( lexc->token , "c2" ) ) {

					//	__fastcall				
					compiler->parameter |= SC_C2 ;						

				} else if ( !sc_strcmp ( lexc->token , "c3" ) ) {

					//	__armcall				
					compiler->parameter |= SC_C3 ;						

				} else if ( !sc_strcmp ( lexc->token , "elf" ) ) {

					compiler->parameter |= SC_ELF ;						

				} else if ( !sc_strcmp ( lexc->token , "link" ) ) {

					compiler->parameter |= SC_LINK ;			

					if ( compiler->parameter & SC_NLINK ) {
						SClog ( "Linker is multi-defined with '-nlink'\n" ) ;
						goto ErrorRelease ;						
					}	

				} else if ( !sc_strcmp ( lexc->token , "nlink" ) ) {
				
					compiler->parameter |= SC_NLINK ;	

					if ( compiler->parameter & SC_LINK ) {
						SClog ( "Linker is multi-defined with '-link'\n" ) ;
						goto ErrorRelease ;						
					}						

				} else if ( !sc_strcmp ( lexc->token , "arm" ) ) {

					compiler->parameter |= SC_ARM ;		

					if ( compiler->parameter & SC_X86 ) {
						SClog ( "Architecture '-X86' has already specified\n" ) ;
						goto ErrorRelease ;						
					}							

				} else if ( !sc_strcmp ( lexc->token , "X86" ) ) {

					compiler->parameter |= SC_X86 ;		

					if ( compiler->parameter & SC_ARM ) {
						SClog ( "Architecture '-arm' has already specified\n" ) ;
						SClog ( "The compiler is not supports X86 yet\n" ) ;
						goto ErrorRelease ;						
					}
					
				}  else {

					SClog ( "Undefined parameter\n" ) ;
					goto ErrorRelease ;				
					
				}

				lexerc_skip_blank () ;
				state = 0 ;
				
			break ;

			case 2 :

				if ( lexc->token && sc_is_alpha (lexc->token[0]) ) 
					SCClStringAddStr ( &str , lexc->token ) ;

				SCClStringAdd ( &str , '\0' ) ;
				SCClStringReset ( &str ) ;

				SCClListInsert ( (SCClList* )compiler->il , (void* )sc_strnew ( str.data ) ) ;

				lexerc_skip_blank () ;
				
				state = 0 ;
			
			break ;
			
			case 3 :				

				if ( !sc_strcmp ( lexc->token , "po" ) ) {
					lexerc_skip_blank () ;
					lexerc_genv () ;
					if ( sc_strcmp ( lexc->token , "=" ) ) {
						SClog ( "Incorrect 'SET' form\n" ) ;
						goto ErrorRelease ;						
					}
					lexerc_skip_blank () ;
					lexerc_genv () ;
					if ( !sc_strcmp ( lexc->token , "dc" ) ) {
						compiler->parameter |= SC_DC ;		
					} else if ( !sc_strcmp ( lexc->token , "ms" ) ) {
						compiler->parameter |= SC_MS ;		
					} else if ( !sc_strcmp ( lexc->token , "dm" ) ) {
						compiler->parameter |= SC_DM ;		
					}  else {
						SClog ( "Incorrect 'SET' form\n" ) ;
						goto ErrorRelease ;						
					}
				} else if ( !sc_strcmp ( lexc->token , "cr" ) ) {
					lexerc_skip_blank () ;
 					lexerc_genv () ;
					if ( sc_strcmp ( lexc->token , "=" ) ) {
						SClog ( "Incorrect 'SET' form\n" ) ;
						goto ErrorRelease ;						
					}
					lexerc_skip_blank () ;
					lexerc_genv () ;
					if ( !sc_strcmp ( lexc->token , "lga" ) ) {
						compiler->parameter |= SC_LGA ;		
					} else if ( !sc_strcmp ( lexc->token , "exp" ) ) {
						compiler->parameter |= SC_EXP ;		
					} else if ( !sc_strcmp ( lexc->token , "ig" ) ) {
						compiler->parameter |= SC_IG ;		
					} else {
						SClog ( "Incorrect 'SET' form\n" ) ;
						goto ErrorRelease ;						
					}				
				} else {
					SClog ( "Incorrect 'SET' form\n" ) ;
					goto ErrorRelease ;						
				}

				state = 0 ;
				
			break ;
		}
		
	}

	SCClStringDestroyEx ( &str ) ;
	
	//	check language
	results = ( SC_CPP & compiler->parameter ) || ( SC_JAVA & compiler->parameter ) ;
	//	if SC_CPP,SC_JAVA is not setting , set SC_C99 as default
	if ( 0 == results ) compiler->parameter |= SC_C99 ;

	//	check architecture
	results = SC_X86 & compiler->parameter ;
	if ( 0 == results ) compiler->parameter |= SC_ARM ;

	//	check linker
	results = SC_LINK & compiler->parameter ;
	if ( 0 == results ) compiler->parameter |= SC_NLINK ;

	//	check parameter transfer type	
	results = ( SC_C0 & compiler->parameter ) || ( SC_C1 & compiler->parameter ) || ( SC_C2 & compiler->parameter ) ;
	//	if SC_0,SC_1,SC2 is not setting , set SC_C3 as default
	if ( 0 == results ) compiler->parameter |= SC_C3 ;
		
	if ( SC_CR & compiler->parameter ) {
		if ( !(SC_EXP & compiler->parameter ) && !(SC_LGA & compiler->parameter )  )
			compiler->parameter |= SC_IG ;
	}

	lexerc_destroy () ;

	return 1 ;
	
ErrorRelease :	

	lexerc_destroy () ;
	
	return 0 ;

}


COMPILER* SCCompilerCreate () {

	//	author : Jelo Wang
	//	since : 20100430
	//	(C)TOK	
	
	COMPILER* compiler = (COMPILER* ) SCMalloc ( sizeof(COMPILER) ) ;

	if ( !compiler ) return 0 ;

	compiler->il = (SCClList* ) SCMalloc ( sizeof(SCClList) ) ;

	if ( !compiler->il ) {
		SCFree ( compiler ) ;
		return 0 ;
	}

	compiler->ol = (SCClList* ) SCMalloc ( sizeof(SCClList) ) ;

	if ( !compiler->ol ) {

		SCFree ( compiler->il ) ;
		SCFree ( compiler ) ;
		return 0 ;
		
	}

	compiler->stime = 0 ;
	compiler->etime = 0 ;
	compiler->lines = 0 ;
	compiler->codes = 0 ;
	compiler->regoccosts = 0 ;
	compiler->lssplits = 0 ;
	
	compiler->PRESOR = 0 ;
	compiler->PARSER = 0 ;
	compiler->GENTOR = 0 ;
	compiler->ASMOR = 0 ;
	compiler->ASSEMER = 0 ;
	compiler->LINKER = 0 ;

	return compiler ;
	

}

static void SCCompilerDestroy () {

	//	author : Jelo Wang
	//	since : 20091127
	//	(C)TOK	

	int walker = 0 ;
	
	if ( !semo ) return ;

	SCClListSetIterator ( semo->il , SCCLLISTSEEK_HEAD ) ;
	for ( ; SCClListIteratorPermit ( semo->il ) ; SCClListListIteratorNext ( semo->il ) )
		SCFree ( SCClListIteratorGetElement ( semo->il ) ) ;

	SCClListDestroy ( semo->il ) ;
	SCFree ( semo->il ) ;

	SCClListSetIterator ( semo->ol , SCCLLISTSEEK_HEAD ) ;
	for ( ; SCClListIteratorPermit ( semo->ol ) ; SCClListListIteratorNext ( semo->ol ) )
		SCFree ( SCClListIteratorGetElement ( semo->ol ) ) ;

	SCClListDestroy ( semo->ol ) ;
	SCFree ( semo->ol ) ;

	SCFreeEx ( &semo ) ;

	SCHalMemoryOverflowed () ;
	SCHalMemoryLeaked () ;
		  
}

static int SCCompilerReady ( int argc , char** argv  ) {

	//	author : Jelo Wang
	//	since : 20090809
	//	(C)TOK
	
	int walker = 0 ;
	int file_length = 0;   

	int inputfile = 0 ;

	unsigned char* buffer = 0;

	if ( !argc || argc < 2 ) {

		help () ;
 		SCTerminate ( 1 ) ;  
		return 0 ;
		
	}
	
	semo = SCCompilerCreate () ;

	if ( !semo ) return 0 ;
	
	if ( !sc_command_parser ( semo , argc , argv ) ) {
		SCFreeEx ( &semo ) ;
		return 0 ;
	}
	
	if ( SC_C99 & semo->parameter ) {
		//	preprocessor of c lanuage
		semo->PRESOR = (void* )presor_c_run ;
		//	parser of c lanuage
		semo->PARSER = (void* )parser_c_run ;
	}

	if ( SC_ARM & semo->parameter ) {
		//	ARMv6 assembly codes generator.
		semo->ASMOR = (void* )asmor_arm_run ;
		//	ARMv6 assembler. 		
		semo->ASSEMER = (void* )assemer_arm_run ;
	}

	if ( SC_C2J & semo->parameter ) {
		//	preprocessor of c lanuage
		semo->PRESOR = (void* )presor_c_run ;
		//	parser of c lanuage
		semo->PARSER = (void* )parser_c_run ;		
		semo->GENTOR = (void* )gentor_java_run ;
	} else {
		semo->GENTOR = (void* )LairGentorRun ;	
	}

	semo->RELEASE = (void* )SCCompilerDestroy ;
	
	return 1 ;

}

int SCCompile ( int argc , char** argv , int type ) {

	//	author : Jelo Wang
	//	since : 20091127
	//	(C)TOK

	//	lines parsed per ms. itsnt a accurate value.
	float perline = 0 ;
	//	lexical analyzer of c language.
	LEXERC* lexer = 0 ;
	//	initialize compling parameters:language of front-para,machine of back-para ect.
	if ( !SCCompilerReady ( argc , argv ) )
		return 0 ;
	
	semo->stime = clock () ;

	#ifdef SEMO_DEBUG
	{

		//	just for debug bellow
		semo->parameter |= SC_PO ;	
		semo->parameter |= SC_LAIR ;
		semo->parameter |= SC_SASM ;	
		semo->parameter |= SC_CR ;							
		semo->parameter |= SC_EXP ;	
						
	}
	#endif

	for ( ; SCClListIteratorPermit ( semo->il ) ; SCClListListIteratorNext ( semo->il ) ) {

		//	lgnosia codes
		char* lair = 0 ;
		//	assembly codes
		char* asm = 0 ;
		char* buffer = 0 ;
		//	source file of codes
		char* file = (char* ) SCClListIteratorGetElement ( semo->il ) ;
		//	objective files been compiled
		char* o = 0 ;

		int filen = 0 ;
		
		void* inputfile = SCHalFileOpen ( "F:\\TOK\\semo\\win32\\Demo\\ca.c" , "rb" ) ;
		//void* inputfile = SCHalFileOpen ( file , "rb" ) ;
		
		if ( !inputfile ) {
			SClog ("Can not open the file '%s'\n" , file ) ;
			continue ;
		}
		
		filen = SCHalFileLength ( inputfile ) ;
		if ( 0 == filen ) {
			SClog ( "File '%s' is empty\n" , file  ) ;
			SCHalFileClose ( inputfile ) ;
			continue ;
		}
		buffer = (char* ) SCMalloc ( filen ) ;

		ASSERT (buffer) ;
		
		SCHalFileSeek ( inputfile , 0 , SEEK_HEAD ) ;
		SCHalFileRead ( inputfile , buffer , 1 , filen ) ;

		//	create a lexer for c language which is a low-level analyzer under the parser
		//	lexer would cut the source streams as a 'lexical atom' to the parser.
		if ( SC_C99 & semo->parameter ) {
			lexerc_set ( lexerc_new ( buffer , LEXERC_DEFAULT_MODE ) ) ; 		
		}

		//	Preprcessor of the front-para
		if ( !semo->PRESOR ( sc_strcat (file,".po") ) ) continue ;
		
#if 1
		//	Parser of the front-para
	 	if ( !semo->PARSER ( &semo->lines ) ) continue ;

		//	Lgnosia Codes Generator
		lair = semo->GENTOR ( sc_strcat (file,".ir") ) ;		
		//	Assembly Codes Generator
		asm = semo->ASMOR ( lair , sc_strcat (file,".sasm") ) ;
		
		o = sc_strcat (file,".elf") ;
		SCClListInsert  ( (SCClList* )semo->ol , o ) ;
		semo->ASSEMER ( asm , o , &semo->codes ) ;
#endif

		lexerc_destroy () ;
		SCHalFileClose ( inputfile ) ;
		SCFree ( buffer ) ;
		
	}

	//	objective files are saved with compiler->ol which is a SCClList.
	//	its the parameter of LINKER.
	if ( SC_LINK & semo->parameter ) {
		semo->LINKER ( semo->ol ) ;
	}
	
	semo->etime = clock () ;

	//	print compiling logs bellow
	if ( semo->lines && semo->etime - semo->stime )
		perline = ((float)semo->lines / (semo->etime - semo->stime)) ;

	//	print compiling logs
	SClog ("\n") ;
	SClog ("Totall Costs : %1.3f sec\n" , (float)(semo->etime - semo->stime)/1000 ) ;
	SClog ("Orignal Lines : %d \n" , semo->lines ) ;
	SClog ("Compiling Speed : %1.3f\n" , perline ) ;
	SClog ("Binarys Compiled : %1.3fkb\n" , (float)semo->codes / 1024 ) ;
	//	live-scopes spliting times of York-Town allocator
	SClog ("Lives Splited : %d times\n" , semo->lssplits ) ;
	//	register allocation times
	SClog ("Reg-Alloc Costs : %d ms\n" , semo->regoccosts ) ;
	SClog ("\n" ) ;

	semo->RELEASE () ;

	return 1 ;
	
 }

