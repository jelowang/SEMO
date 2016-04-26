
/*

+	SSA Form Generator, part of SC mid-para

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

extern int SsaMakeAlias ( int flow , int symbol , int scope ) ;
extern int* SsaGetMultiAlias ( int symbol , int* totall ) ;
extern int SsaGetAlias ( int symbol , int scope ) ;
extern int SsaGetAliasFlow ( int symbol ) ;
extern int SsaGetAncestorAlias ( int symbol , int scope )  ;
extern char* SsaMakeMultiAliasString ( int symbol , char* name , int scope , int flow ) ;
extern void SsaMakeMultiAliasEnable () ;
extern void SsaMakeMultiAliasDisable () ;
extern void SsaCleanMultiAlias ( int symbol , int flow , int scope ) ;
extern void SsaClean () ;

