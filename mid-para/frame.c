
/*

+	'Memory Frame' of SC c-front-para

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

# include "schal.h"
# include "frame.h"

typedef struct {

	//	author : Jelo Wang
	//	since : 20121031
	//	(C)TOK

	char* name ;
	int address ;

} FRAME ;

typedef struct {

	//	author : Jelo Wang
	//	since : 20121031
	//	(C)TOK
	
	FRAME* StackFrame ;
	int StackFrameWalker ;
	int StackFrameLength ;
	
	FRAME* HeapFrame ;
	int HeapFrameWalker ;
	int HeapFrameLength ;

} MEMORY ;

static MEMORY Memory = { 0 } ;

void MemoryFrameCreate ( int length , int type ) {

	//	author : Jelo Wang
	//	since : 20121031
	//	(C)TOK

	if ( MF_STACK == type ) {

		Memory.StackFrame = (FRAME* ) SCMalloc ( sizeof(FRAME) * length ) ;
		ASSERT(Memory.StackFrame) ;
		Memory.StackFrameLength = length ;
		
	} else if ( MF_HEAP == type ) {

		Memory.HeapFrame = (FRAME* ) SCMalloc ( sizeof(FRAME) * length ) ;
		ASSERT(Memory.HeapFrame) ;
		Memory.HeapFrameLength = length ;
		
	}

}

void MemoryFrameAdd ( char* symbol , int type ) {

	//	author : Jelo Wang
	//	since : 20121031
	//	(C)TOK
	
	if ( MF_STACK == type ) {
		
		Memory.StackFrame[Memory.StackFrameWalker].name = (char* ) SCMalloc ( sc_strlen(symbol) + 1 ) ;
		sc_strcpy ( Memory.StackFrame[Memory.StackFrameWalker].name , symbol ) ;
		Memory.StackFrame[Memory.StackFrameWalker].address = Memory.StackFrameWalker ;
		Memory.StackFrameWalker ++ ;
		
	} else if ( MF_HEAP == type ) {
		
		Memory.HeapFrame[Memory.HeapFrameWalker].name = (char* ) SCMalloc ( sc_strlen(symbol) + 1 ) ;
		sc_strcpy ( Memory.HeapFrame[Memory.StackFrameWalker].name , symbol ) ;
		Memory.HeapFrame[Memory.HeapFrameWalker].address = Memory.HeapFrameWalker ;
		Memory.HeapFrameWalker ++ ;
		
	}	
	
}

int MemoryFrameGetAddress ( char* symbol ) {
	
	//	author : Jelo Wang
	//	since : 20121031
	//	(C)TOK
	
	int looper = 0 ;
		
	for ( looper = 0 ; looper < Memory.StackFrameLength ; looper ++ ) {
		if ( 0 == sc_strcmp ( Memory.StackFrame[looper].name , symbol ) ) {
			return Memory.StackFrame[looper].address ;
		}
	}	

	return 0 ;

}

void MemoryFrameDestroy ( int type ) {

	//	author : Jelo Wang
	//	since : 20121031
	//	(C)TOK

	if ( MF_STACK == type && Memory.StackFrame ) {
		
		int looper = 0 ;
		
		for ( looper = 0 ; looper < Memory.StackFrameLength ; looper ++ ) {
			SCFree ( Memory.StackFrame[looper].name ) ;
		}
		SCFree ( Memory.StackFrame ) ;
		
	} else if ( MF_HEAP == type && Memory.HeapFrame ) {
		
		int looper = 0 ;
		
		for ( looper = 0 ; looper < Memory.HeapFrameLength ; looper ++ ) {
			SCFree ( Memory.HeapFrame[looper].name ) ;
		}
		SCFree ( Memory.HeapFrame ) ;
		
	}	
	
}

