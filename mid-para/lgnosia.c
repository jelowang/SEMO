
/*

+	LGNOSIA , part of SC mid-para

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
# include "sccl.h"
# include "lgnosia.h"

struct {

	//	author : Jelo Wang
	//	since : 20100717
	//	(C)TOK

	SCClStack stack ;
	SCClList queue ;
	SCClList pool ;
	
} LgaGentor = { {0,0,0,0,0} , {0,0,0} , {0,0,0} } ;

int LgnosiaNew ( int azonal , LGNOSIA_IDENT type ) {

	//	author : Jelo Wang
	//	since : 20100717
	//	(C)TOK

	LGNOSIA* lgnosia = (LGNOSIA* ) SCMalloc ( sizeof(LGNOSIA) ) ;

	if ( !lgnosia ) return 0 ;

	lgnosia->type = type ;
	lgnosia->azonal = azonal ;

	if ( LGNOSIA_TOP_IDENT == lgnosia->type ) 
		SCClListInsert ( &LgaGentor.pool , (void* )lgnosia ) ;

	return (int )lgnosia ;
	
}

void LgnosiaAddContext ( int lgnosia , int handle , LGNOSIA_IDENT type ) {

	//	author : Jelo Wang
	//	since : 20100718
	//	(C)TOK

	//	notes : add context for LGNOSIA

	LGNOSIA* lgnosiap = (LGNOSIA* )lgnosia ;
	
	if ( !lgnosiap ) return ;

	SCClListInsertEx ( &lgnosiap->context , (void* )handle , (int)type ) ;	
	
}

void LgnosiaAddParameter ( int lgnosia , int handle , LGNOSIA_IDENT type ) {

	
	//	author : Jelo Wang
	//	since : 20100718
	//	(C)TOK

	//	notes : add context for LGNOSIA

	LGNOSIA* lgnosiap = (LGNOSIA* )lgnosia ;
	
	if ( !lgnosiap ) return ;

	SCClListInsertEx ( &lgnosiap->parameter , (void* )handle , type ) ;	

	
}

int LgnosiaFindContext ( int flgnosia , int clgnosia ) {

	//	author : Jelo Wang
	//	since : 20100720
	//	(C)TOK

	//	notes : 在一个点下搜索一个LGA。

	SCClList* looper = 0 ;
	LGNOSIA* lgnosiap = (LGNOSIA* )flgnosia ;
  
	if ( !lgnosiap ) return 0 ;
	if ( flgnosia == clgnosia ) return clgnosia ;

	for ( looper = lgnosiap->context.head ; looper ; looper = looper->next ) {

		if ( clgnosia == looper->element ) 
			return looper->element  ;
		
		if ( LGNOSIA_CP_IDENT == looper->eltype ) {

			//	获取poc、nec 继续搜索clgnosia
			LGNOSIA* lga = (LGNOSIA* )looper->element ;		

			//	搜poc
			if ( 0 != LgnosiaFindContext ( lga->po_chain , clgnosia )  ) 
				return clgnosia ;

			//	搜nec
			if ( 0 != LgnosiaFindContext ( lga->ne_chain , clgnosia )  ) 
				return clgnosia ;				

		}

		
	}
	
	return 0 ;
	
	
}

int LgnosiaFindContextEx ( int flgnosia , int clgnosia ) {

	//	author : Jelo Wang
	//	since : 20121027
	//	(C)TOK

	//	notes : 在一个控点下搜索一个LGA。

	SCClList* looper = 0 ;
	LGNOSIA* lgnosiap = (LGNOSIA* )flgnosia ;
  
	if ( !lgnosiap ) return 0 ;
	if ( flgnosia == clgnosia ) return clgnosia ;

	if ( 0 == lgnosiap->context.head ) {
		
		//	搜poc
		if ( 0 != LgnosiaFindContextEx ( lgnosiap->po_chain , clgnosia )  ) 
			return clgnosia ;

		//	搜nec
		if ( 0 != LgnosiaFindContextEx ( lgnosiap->ne_chain , clgnosia )  ) 
			return clgnosia ;	
		
	} else {
	
		for ( looper = lgnosiap->context.head ; looper ; looper = looper->next ) {

			if ( clgnosia == looper->element ) 
				return looper->element  ;
			
			if ( LGNOSIA_CP_IDENT == looper->eltype ) {

				//	获取poc、nec 继续搜索clgnosia
				LGNOSIA* lga = (LGNOSIA* )looper->element ;		
				LGNOSIA* poc = lga->po_chain ;
				LGNOSIA* nec = lga->ne_chain ;
				LGNOSIA* found = 0 ;

				//	搜poc
				found = LgnosiaFindContext ( poc , clgnosia ) ;
				if ( 0 != found  ) return found ;

				//	搜nec
				found = LgnosiaFindContext ( nec , clgnosia ) ;
				if ( 0 != found  ) return found ;

			}

			
		}

	}
	
	return 0 ;
	
	
}

int LgnosiaDFSNormalize ( int lga , int flga , int deep ) {

	//	author : Jelo Wang
	//	since : 20100804
	//	(C)TOK

	//	notes : 计算节点深度，设置父节点

	int ld = 0 ;
	int rd = 0 ;
	int value = 0 ;
	
	int recdeep = 0 ;
	SCClList deeplist = {0} ;

	SCClList* looper = 0 ;
	LGNOSIA* lgnosia = (LGNOSIA* )lga ;
  
	if ( !lgnosia ) return deep ;
	
	lgnosia->deep = deep ;
	lgnosia->father = flga ;

	for ( looper = lgnosia->context.head ; looper ; looper = looper->next ) {

		LGNOSIA* lga = (LGNOSIA* )looper->element ;
		
		if ( LGNOSIA_CP_IDENT == lga->type ) {

			LGNOSIA* poc = lga->po_chain ;
			LGNOSIA* nec = lga->ne_chain ;
			
			recdeep ++ ;
			ld = LgnosiaDFSNormalize ( poc , lga , deep+1 ) ;
			recdeep -- ;
			
			recdeep ++ ;
			rd = LgnosiaDFSNormalize ( nec , lga , deep+1 ) ;
			recdeep -- ;

			SCClListInsert ( &deeplist , (void* )ld ) ;
			SCClListInsert ( &deeplist , (void* )rd ) ;

		}

		lga->father = flga ;
		
	}
	
	value = SCClListSearchBigestElement ( &deeplist ) ;
	SCClListDestroy ( &deeplist ) ;

	return value ;

	
}

int LgnosiaBFSNormalize( int lgnosiaa ) {

	//	author : Jelo Wang
	//	since : 20100804
	//	(C)TOK

	//	notes : 计算某一层的节点总数
	
	//	all nodes that in a same layer
	int allinlayerc = 0 ;

	SCClList* looper = 0 ;
	SCClList samelayernode = { 0 } ;
	SCClQueue queue = {0} ;
	LGNOSIA* lgnosia = (LGNOSIA* )lgnosiaa ;
	
	SCClQueueInit ( &queue ) ;
	SCClQueueEnter ( &queue , (void* )lgnosia ) ;

	LgnosiaDFSNormalize ( lgnosiaa , 0 , 1 ) ;

	while ( lgnosia ) {

		int lastdeep = 0 ;
		int thedeep = 0 ;

		lastdeep = lgnosia->deep ;		
		lgnosia = (LGNOSIA* )SCClQueueOut ( &queue ) ;

		if ( !lgnosia ) break ;
		
		if ( LGNOSIA_CP_IDENT == lgnosia->type ) {
			if (lgnosia->po_chain)
				SCClQueueEnter ( &queue , (void* )lgnosia->po_chain ) ;
			if (lgnosia->ne_chain)
				SCClQueueEnter ( &queue , (void* )lgnosia->ne_chain ) ;
		} else {

			for ( looper = lgnosia->context.head ; looper ; looper = looper->next ) {
				SCClQueueEnter ( &queue , (void*)looper->element ) ;
			}

		}
		
		thedeep = lgnosia->deep ;	

		if ( lastdeep == thedeep ) {
			
			allinlayerc ++ ;

			SCClListInsert ( &samelayernode , (void* )lgnosia ) ;
		
		} else {

			{
				SCClList* walker = samelayernode.head ;
				for ( walker ; walker ; walker = walker->next ) {
					LGNOSIA* lga = (LGNOSIA*) walker->element ;
					if ( 0 == lga->allinyer ) lga->allinyer = allinlayerc ;
				}
			}
				
			allinlayerc = 1 ;

			SCClListInsert ( &samelayernode , (void* )lgnosia ) ;
			
		}

		
	}

	SCClListDestroy ( &samelayernode ) ;
	
	return 1 ;

}



int LgnosiaGenPOC ( int lgnosia , int azonal ) {

	//	author : Jelo Wang
	//	since : 20100718
	//	(C)TOK

	//	add positive chain of LGNOSIA_CP_IDENT
	
	LGNOSIA* lgnosiap = (LGNOSIA* )lgnosia ;
	
	if ( !lgnosiap ) return 0 ;

	lgnosiap->po_chain = (LGNOSIA* ) SCMalloc ( sizeof(LGNOSIA) ) ;
	if ( lgnosiap->po_chain ) lgnosiap->po_chain->azonal = azonal ;
	//lgnosiap->po_chain->type = LGNOSIA_CP_POC_IDENT ;

	return (int)lgnosiap->po_chain ;

}


int LgnosiaGenNEC ( int lgnosia , int azonal ) {

	//	author : Jelo Wang
	//	since : 20100718
	//	(C)TOK

	//	add negative chain of LGNOSIA_CP_IDENT
	
	LGNOSIA* lgnosiap = (LGNOSIA* )lgnosia ;
	
	if ( !lgnosiap ) return 0 ;
	
	lgnosiap->ne_chain = (LGNOSIA* ) SCMalloc ( sizeof(LGNOSIA) ) ;
	if ( lgnosiap->ne_chain ) lgnosiap->ne_chain->azonal = azonal ;
//	lgnosiap->ne_chain->type = LGNOSIA_CP_NEC_IDENT ;

	return (int)lgnosiap->ne_chain ;
	
}


int LgnosiaGet () {

	//	author : Jelo Wang
	//	since : 20100107
	//	(C)TOK

	//	notes : get lgnostic tree

	if ( LgaGentor.pool.head )
		return (int )LgaGentor.pool.head ;
	
	else return 0 ;

}


void LgnosiaDestroy ( int lgnosia ) {
	
	//	author : Jelo Wang
	//	since : 20100731
	//	(C)TOK
/*
	SCClList* looper = 0 ;
	LGNOSIA* lgnosiap = (LGNOSIA* )lgnosia ;

	if ( !lgnosiap ) return ;
	
		for ( looper = lgnosiap->context.head ; looper ; looper = looper->next ) {
		
		if ( LGNOSIA_CP_IDENT == looper->eltype ) {

			LGNOSIA* lga = (LGNOSIA* )looper->element ;
			LGNOSIA* poc = lga->po_chain ;
			LGNOSIA* nec = lga->ne_chain ;
		
			LgnosiaDestroy ( poc ) ;
			LgnosiaDestroy ( nec ) ;

		}

		SCFree ( (void*)looper->element ) ;

	}
*/		
	
}

void LgnosiaStackPush ( int handle ) {

	//	author : Jelo Wang
	//	since : 20100717
	//	(C)TOK

	SCClStackPush ( &LgaGentor.stack , (void* )handle ) ;
	
}

int LgnosiaStackPop () {

	//	author : Jelo Wang
	//	since : 20100717
	//	(C)TOK

	return (int )SCClStackPop ( &LgaGentor.stack ) ;
	
}

int LgnosiaStackTop () {

	//	author : Jelo Wang
	//	since : 20100718
	//	(C)TOK

	return SCClStackGet ( &LgaGentor.stack ) ;
	
}

void LgnosiaQueueEnter ( int lgnosia ) {

	//	author : Jelo Wang
	//	since : 20100719
	//	(C)TOK

	SCClListInsert ( &LgaGentor.queue , (void* )lgnosia ) ;
	
}

int LgnosiaQueueOut () {

	//	author : Jelo Wang
	//	since : 20100719
	//	(C)TOK

	SCClList* node = LgaGentor.queue.next ;
	return (int)node->element ;
	
}

void LgnosiaQueueDestroy () {

	//	author : Jelo Wang
	//	since : 20100719
	//	(C)TOK
	
}

