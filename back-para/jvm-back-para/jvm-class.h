
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

# ifndef JAVACLASS
# define JAVACLASS

# define JU1 unsigned char 
# define JU2 unsigned short
# define JU4 unsigned int

//	Class File Minor Version	
# define JCF_MINVER 0
//	Class File Major Version
# define JCF_MAJVER 45

//	notes : class access and property modifiers
# define ACC_PUBLIC		0x0001 
# define ACC_FINAL		0x0010 
# define ACC_SUPER		0x0020 
# define ACC_INTERFACE	0x0200 
# define ACC_ABSTRACT	0x0400 
# define ACC_SYNTHETIC	0x1000 
# define ACC_ANNOTATION	0x2000 
# define ACC_ENUM		0x4000 

typedef struct {

	//	author : Jelo Wang
	//	since : 20110329
	//	(C)TOK
	
	JU1 tag ;
	JU1* info ;

} JCpInfo ;

typedef struct {

	//	author : Jelo Wang
	//	since : 20110329
	//	(C)TOK

	JU2 attribute_name_index ;
	JU4 attribute_length ;
	//	the length of 'info' is 'attribute_length'
	JU1* info;

} JAttributeInfo ;

typedef struct {

	//	author : Jelo Wang
	//	since : 20110329
	//	(C)TOK
	
	JU2 access_flags ;
	JU2 name_index ;
	JU2 descriptor_index ;
	JU2 attributes_count ;
	//	the length of 'attributes' is attributes_count
	JAttributeInfo* attributes ; 

} JFieldInfo ;

typedef struct {

	//	author : Jelo Wang
	//	since : 20110329
	//	(C)TOK
	
	JU2 access_flags ;
	JU2 name_index ;
	JU2 descriptor_index ;
	JU2 attributes_count ;
	//	the length of 'attributes' is attributes_count
	JAttributeInfo* attributes ; 

} JMethodInfo ;

typedef struct {

	//	author : Jelo Wang
	//	since : 20110329
	//	(C)TOK

	JU1 magic [ 4 ] ;
	JU2 minor_version ;
	JU2 major_version ;
	JU2 constant_pool_count ;
	//	the length of 'constant_pool' is 'constant_pool_count-1' bellow
	JCpInfo* constant_pool  ;
	JU2 access_flags ;
	JU2 this_class ;
	JU2 super_class ;
	JU2 interfaces_count ;
	// the length of 'interfaces_count' is 'interfaces_count' bellow
	JU2* interfaces ;
	JU2 fields_count ;
	//	the length of 'fields' is fields_count
	JFieldInfo* fields ; 
	JU2 methods_count ;
	//	the length of 'methods' is fields_count
	JMethodInfo* methods  ;
	JU2 attributes_count ;
	//	the length of 'attributes' is attributes_count
	JAttributeInfo* attributes ; 	

} JClassFile ;

# endif

