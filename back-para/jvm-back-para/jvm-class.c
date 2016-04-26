
/*

+	The JVM Class File Specifications , part of SC jvm-back-para

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

# include "sccl.h"
# include "schal.h"
# include "jvm-class.h"

int JVMClassFileCreate ( char* path ) {

	//	author : Jelo Wang
	//	since : 20110329
	//	(C)TOK

	JClassFile ClassFile = { { 0xCA , 0xFE , 0xBA , 0xBE } , 0 } ;
	
	void* file = SCHalFileOpen ( path , "wr+" ) ;

	ASSERT ( file ) ;

	ClassFile.minor_version = JCF_MINVER ;
	ClassFile.major_version = JCF_MAJVER ;
	
	return 1 ;

}

