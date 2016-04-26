
/*

+	SYMBOLE , between SC front-para and mid-para

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

//	author: Jelo Wang
//	since : 20081109
//	(C)TOK

# include "schal.h"
# include "sccl.h"
# include "symbole.h"
# include "lgnosia.h"

# define SYMBOLE_DRCDISTUB(x)\
	(x == AZONAL_DRC_IF_DISTURB||x == AZONAL_DRC_ELSEIF_DISTURB||x == AZONAL_DRC_ELSE_DISTURB)

SYMBOLE* symbole = 0 ;

int SymboleCreate () {

	//	author : Jelo Wang
	//	input : symbol table installation
	//	(C)TOK
	
	SYMBOLE* symbole = (SYMBOLE* ) SCMalloc ( sizeof(SYMBOLE) ) ;

	if ( !symbole ) return 0 ;
	
	symbole->variable.next = 0 ;
	symbole->variable.head = 0 ;
	
	symbole->array.next = 0 ;
	symbole->array.head = 0 ;	
	
	symbole->constant.next = 0 ;
	symbole->constant.head = 0 ;
	
	symbole->function.next = 0 ;
	symbole->function.head = 0 ;	

	symbole->funccal.next = 0 ;
	symbole->funccal.head = 0 ;	
		
	symbole->controlflow.next = 0 ;
	symbole->controlflow.head = 0 ;
	
	symbole->vartotall = 0 ;
	symbole->arrtotall = 0 ;
	symbole->functotall = 0 ;
	symbole->exptotall = 0 ;
	symbole->logicalblocks = 0 ;

	return (int)symbole ;

}


void SymboleSetCurrent ( int symbol ) {

	//	author : Jelo Wang
	//	input : symbol table installation
	//	(C)TOK

	symbole = (SYMBOLE* ) symbol ;

}


AZONAL* SymboleAzonalAddVariable ( 
	
	char* name ,
	int data_type , 
	int azonal_type , 
	int scope ,
	int size , 
	int is_param ,
	int line ,
	int belong ,
	int lgabelong 
	
) {

	//	author : Jelo Wang
	//	notes : add ISA_VARIABLE
	//	since : 20100418
	//	(C)TOK

	//	(1) param : SYMBOLE* 
	//	(2) param : name
	//	(3) param : data type
	//	(4) param : azonal type
	//	(5) param : scope of variable
	//	(6) param : is it parameter of function	
	//	(7) param : line
	//	(8) param : function belong
	
	AZONAL* azonal = 0 ;
	
	if ( !symbole ) return 0 ;
	
	azonal = (AZONAL* ) SCMalloc ( sizeof(AZONAL) ) ;

	azonal->azonaltype = azonal_type ;
	azonal->datatype = data_type ;
	azonal->isparam = is_param ;	
	azonal->scope = scope ;
	azonal->size = size ;
	azonal->line = line ;
	azonal->belong = belong ;
	azonal->used = 0 ;
	azonal->head = 0 ;
	azonal->next = 0 ;
	azonal->lgabelong = lgabelong ;
	
	//	jelo 20121027
	//	azonal->DRC.deftimes = 0 ;
	//	azonal->DRC.chain = 0 ;
	//	end

	azonal->name = (char* ) SCMalloc ( sc_strlen ( name ) + 1 ) ;
	sc_strcpy ( azonal->name , name ) ;
	
	//	for parameters of function
	if ( azonal->isparam ) {
		if ( ISA_FUNCTION == ((AZONAL*)belong)->azonaltype ) {
			//	add procedure's tack
			SCClListInsert ( &((AZONAL*)belong)->tack , azonal ) ;
		}
	}

	if ( belong ) {
		//	caculate procedure size
		((AZONAL*)belong)->size += azonal->size ;
	}

	if ( 0 == symbole->variable.next ) {
		symbole->variable.next = azonal ;
		symbole->variable.head = azonal ;
	} else {
		symbole->variable.next->next = azonal ;
		symbole->variable.next = azonal ; 
	}

	symbole->vartotall ++ ;
	
	return azonal ;
	
}

AZONAL* SymboleFindAzonal ( 

	char* name ,
	int scope ,
	int lgabelong 

) {

	//	author : Jelo Wang
	//	notes : add ISA_VARIABLE
	//	since : 20100421
	//	(C)TOK

	//	(1) param : SYMBOLE* 
	//	(2) param : name
	//	(3) param : scope of variable
	//	(4) param : function belong

	AZONAL* globalanl = 0 ;
	AZONAL* azonal = 0 ;
	
	if ( !symbole ) return 0 ;

	if ( 0 == scope ) {
		
		for ( azonal = symbole->variable.head ; azonal ; azonal = azonal->next ) {

			if ( (0 == azonal->scope) && !sc_strcmp ( azonal->name , name ) ) {
				return 1 ;
			}	
			
		}

	} else if ( scope ) {

		//	找到每一个名字为name的变量，取出其lga，在其lga中查找当前lgabelong。

		for ( azonal = symbole->variable.head ; azonal ; azonal = azonal->next ) {

			if ( sc_strcmp ( azonal->name , name ) ) continue ;

			if ( LgnosiaFindContext ( (LGNOSIA*)azonal->lgabelong , lgabelong ) ) {
				return azonal ;
			}
			
		}
		
	}

	return azonal ;
	
}


AZONAL* SymboleVarAzonalSavable ( 

	char* name ,
	int scope ,
	int lgabelong 

) {

	//	author : Jelo Wang
	//	since : 20100514
	//	(C)TOK

	//	(1) param : name
	//	(2) param : scope of variable
	//	(4) param : lga belong to 
	
	AZONAL* globalanl = 0 ;
	AZONAL* azonal = 0 ;
	
	if ( !symbole ) return 0 ;

	if ( 0 == scope ) {
		
		for ( azonal = symbole->variable.head ; azonal ; azonal = azonal->next ) {

			if ( (0 == azonal->scope) && !sc_strcmp ( azonal->name , name ) ) {
				return 0 ;
			}	
			
		}

	} else if ( scope ) {


		for ( azonal = symbole->variable.head ; azonal ; azonal = azonal->next ) {

			if ( sc_strcmp ( azonal->name , name ) ) continue ;
				
//			if ( LgnosiaFindContext ( (LGNOSIA*)azonal->lgabelong , lgabelong ) ) {
				
//				if ( scope == azonal->scope ) return 0 ;
				
//			}

			//	在相同LGA 中，只允许一次定义。
			if ( lgabelong == azonal->lgabelong ) {
				return 0 ;
			}
			
		}

		
	}


	return 1 ;
	
}


void SymboleAzonalRefInc ( AZONAL* azonal ) {

	//	author : Jelo Wang
	//	since : 20100516
	//	(C)TOK

	//	increase reference times of AZONAL .

	if ( azonal ) azonal->used ++ ;

	
}

AZONAL* SymboleAddFunction ( char* name , int azonaltype , int type , int line ) {

	//	author : Jelo Wang
	//	input : name,type of return value,line
	//	output: insert a function record into symbol table
	//	(C)TOK


	AZONAL* azonal = (AZONAL* ) SCMalloc ( sizeof(AZONAL) ) ;

	if ( !azonal || !symbole  ) return 0 ;
		
	azonal -> name = (char* ) SCMalloc ( sc_strlen (name) + 1 ) ; 
	sc_strcpy ( azonal -> name , name ) ;
		
	azonal->datatype = type ;
	azonal->azonaltype = azonaltype ;
	azonal->line = line ;

	//	jelo 20121027
	//	azonal->DRC.deftimes = 0 ;
	//	azonal->DRC.chain = 0 ;
	//	end

	azonal->tack.head = 0 ;
	azonal->tack.next = 0 ;
	azonal->tack.totall = 0 ;
	
	azonal->number = ++ symbole->functotall ;
	azonal->next = 0 ;

	if ( 0 == symbole->function.next ) {
		symbole->function.next = azonal ;
		symbole->function.head = azonal ;
	} else {
		symbole->function.next->next = azonal ;
		symbole->function.next = azonal ;
	}

	symbole->logicalblocks ++ ;
	
	return azonal ;

}

AZONAL* SymboleAddFunctionCall ( char* name , int azonaltype , int tacktotall , int line ) {

	//	author : Jelo Wang
	//	20110212
	//	(C)TOK

	AZONAL* azonal = 0 ;
	
	ASSERT(name);

	azonal = SCMalloc ( sizeof(AZONAL) ) ;
	ASSERT(azonal) ;
	
	azonal->name = SCMalloc ( sc_strlen ( name ) + 1 ) ;
	ASSERT(azonal->name);
	sc_strcpy ( azonal->name , name ) ;

	azonal->azonaltype = azonaltype ;
	azonal->tack.totall = tacktotall ;
	azonal->line = line ;

	if ( 0 == symbole->funccal.next ) {
		symbole->funccal.next = azonal ;
		symbole->funccal.head = azonal ;
	} else {
		symbole->funccal.next->next = azonal ;
		symbole->funccal.next = azonal ;
	}

	return azonal ;
	
}

int SymboleGetCurrentFuncNumber () {
	
	//	author : Jelo Wang
	//	since : 20100430
	//	(C)TOK

	if ( !symbole ) return -1;

	return symbole->function.next->number ;
	
}

void* SymboleGetCurrentFunc() {
	
	//	author : Jelo Wang
	//	since : 20100508
	//	(C)TOK

	if ( !symbole ) return -1;

	return (int)symbole->function.next ;
	
}

void* SymboleGetFuncParameter ( void* azonal , char* name ) {

	//	author : Jelo Wang
	//	since : 20110224
	//	(C)TOK

	SCClList* tacklooper = 0 ;
	AZONAL* anlnode = 0 ;
	
	if ( !azonal ) return 0 ;

	for ( tacklooper = ((AZONAL*)azonal)->tack.head ; tacklooper ; tacklooper = tacklooper->next ) {
		anlnode = (AZONAL* ) tacklooper->element ;
		if ( !sc_strcmp ( anlnode->name , name ) ) return (void* )anlnode ;
	}

	return 0 ;	
	
}


AZONAL* SymboleFindFunction ( char* name ) {

	//	author : Jelo Wang	
	//	input : name
	//	notes: find a function
	//	(C)TOK

	AZONAL* walker = 0 ;
		 
	if ( !symbole )
		return 0 ;

	for ( walker = symbole->function.head ; walker && sc_strcmp ( walker -> name , name ) ; walker = walker->next ) ;

	return walker ? walker : 0 ;

}

void SymboleSetFunctionParameters ( AZONAL* function , int parameters ) {

	//	author : Jelo Wang	
	//	input : amount
	//	notes: set amount of parameters of a function
	//	(C)TOK
	
	if ( function ) function->tack.totall = parameters ;

}

int SymboleGetFunctionNumber ( char* name ) {

	//	author : Jelo Wang
	//	input : function name
	//	output: function number
	//	since : 20081231
	//	(C)TOK

	AZONAL* azonal = 0 ;
	
	if ( !symbole )
		return 0 ;
	
	for ( azonal = symbole->function.head ; azonal && sc_strcmp ( azonal->name , name ); azonal = azonal->next ) ;
		
	
	return azonal ? azonal->number : 0 ;
	
}


AZONAL* SymboleAddControlAzonal ( 

	int azonaltype , 
	int line 
	
) {

	//	author : Jelo Wang
	//	input : pointer of entry,data type
	//	notes: add control flow
	//	since : 20100115
	// (C)TOK
	
	AZONAL* azonal = (AZONAL* ) SCMalloc ( sizeof(AZONAL) ) ;
	
	if ( !azonal || !symbole ) return 0 ;
	
	azonal->name = 0 ;
	azonal->azonaltype = azonaltype ;
	azonal->line = line ;
	azonal->number = ++symbole ->logicalblocks ;

	azonal->tack.head = 0 ;
	azonal->tack.next = 0 ;
	azonal->head = 0 ;
	azonal->next = 0 ;
	
	if ( 0 == symbole->controlflow.next ) {
		symbole->controlflow.next = azonal ;
		symbole->controlflow.head = azonal ;
	} else {
		symbole->controlflow.next->next = azonal ;
		symbole->controlflow.next = azonal ;
	}

	return azonal ;
	
}

void SymboleUninstall () {

	//	author : Jelo Wang
	//	since : 20100430
	//	(C)TOK

	AZONAL* azonal = 0 ;

	for ( azonal = symbole->variable.head ; azonal ; ) {
		
		symbole->variable.next = azonal->next ;
		if ( azonal->name ) SCFree ( azonal->name ) ;
		
		//	jelo 20121027
		//	if ( azonal->DRC.chain ) SCClListDestroyEx ( azonal->DRC.chain ) ;		
		//	SymboleDRCDropDRC ( azonal ) ;
		// end
		
		SCFree ( azonal ) ;
		azonal = symbole->variable.next ;
		
	}

	for ( azonal = symbole->function.head ; azonal ; ) {
		
		symbole->function.next = azonal->next ;
		if ( azonal->scope ) SCFree ( azonal->scope ) ;
		if ( azonal->name ) SCFree ( azonal->name ) ;
		SCFree ( azonal ) ;
		azonal = symbole->function.next ;
		
	}

	for ( azonal = symbole->controlflow.head ; azonal ; ) {
		
		symbole->controlflow.next =  azonal->next ;
		if ( azonal->scope ) SCFree ( azonal->scope ) ;
		if ( azonal->name ) SCFree ( azonal->name ) ;
		SCFree ( azonal ) ;
		azonal = symbole->controlflow.next ;
		
	}

	SCFree ( symbole ) ;
	
}

AZONAL* SymboleAndNumeric ( char* name , int type ) {

	//	author : Jelo Wang
	//	since : 20110125
	//	(C)TOK

	AZONAL* azonal = (AZONAL* ) SCMalloc ( sizeof(AZONAL) ) ;

	azonal->name = (char* ) SCMalloc ( sc_strlen (name)+1 ) ;

	ASSERT(azonal->name) ;

	sc_strcpy ( azonal->name , name ) ;

	azonal->azonaltype = type ;

	return azonal ;
	
}

