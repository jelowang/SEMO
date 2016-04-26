
/*

+	LAIR is an abstractive machine defined by semo , part of SC mid-para

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

# include "semo.h"
# include "schal.h"
# include "sccl.h"
# include "lair.h"
# include "regoc.h"
# include "corenr.h"

static LAIR* lair = 0 ;
static LAIR* lairswaper = 0 ;
static LABELMOI* labelmoi = 0 ;

static void LairRefChainInsert ( int head , int chain ) {

	//	author : Jelo Wang
	//	since : 20110121
	//	(C)TOK

	LAIR* lair = (LAIR* )head ;

	SCClListInsert ( &lair->refchain , chain ) ;

}

static void LairLiveScopeSplit ( int laira ) {

	//	author : Jelo Wang
	//	since : 20110121
	//	(C)TOK

	//	将lair 引用链上的节点分裂

	LAIR* lair = laira ;
	
	SCClList* looper = 0 ;
	
	semo->lssplits ++ ;

	if ( 1 == lair->splited ) {
		return ;
	}
	
	lair->mode = LAIR_LVA_MEM ;
	lair->splited = 1 ;
	
	SCClStringInsert ( &lair->code , "%$STK.Set " , 0 ) ;

	for ( looper = lair->refchain.head ; looper ; looper = looper->next ) {
		LAIR* lairnode = looper->element ;	

		if ( 0 == lairnode->splited ) {
			SCClStringInsert ( &lairnode->code , "%$STK.Get " , 0 ) ;
			lairnode->mode  = LAIR_RVA_MEM ;		
			lairnode->splited = 1 ;
		}
		
	}

		
}

int LairCreate () {

	//	author : Jelo Wang
	//	since : 20100505
	//	(C)TOK

	LAIR* lair = (LAIR* ) SCMalloc ( sizeof(LAIR) ) ;
	
	SCClStringInit ( &lair->code ) ;
	lair->number = 0 ;
	lair->length = 0 ; 
	lair->head = 0 ;
	lair->next = 0 ;

	return (int) lair ;

} 

void LairSetContext ( int context ) {

	//	author : Jelo Wang
	//	since : 20100505
	//	(C)TOK

	lair = (LAIR* ) context ;

} 

int LairGetContext () {

	//	author : Jelo Wang
	//	since : 20121028
	//	(C)TOK

	return (int ) lair ;
} 

void LairSwapIn () {

	//	author : Jelo Wang
	//	since : 20110122
	//	(C)TOK
	
	lairswaper = lair ;
}

void LairSwapBack () {

	//	author : Jelo Wang
	//	since : 20110122
	//	(C)TOK
	
	lair = lairswaper ;
}

int LairAddCode ( char* string , LAIR_ATOM type , int scope ) {

	//	author : Jelo Wang
	//	since : 20100505
	//	(C)TOK
	
	LAIR* lairn = (LAIR* ) SCMalloc ( sizeof(LAIR) ) ;
	
	SCClStringInitEx ( &lairn->code , sc_strlen ( string ) ) ;
	SCClStringAddStr ( &lairn->code , string ) ;	
	SCClStringAdd ( &lairn->code , '\0' ) ;
	
	lairn->havelabel = 0 ;
	lairn->length = 0 ;
	lairn->mode = type ;
	lairn->scope = scope ;
	lairn->splited = 0 ;
	//	number of lair node
	lairn->number = lair->length ; 
	lairn->line = lair->line ;
	lairn->head = 0 ;
	lairn->next = 0 ;

	if ( LAIR_CR == (LAIR_CR & lairn->mode) )
		lair->line ++ ;

	if ( LAIR_PROC == (LAIR_PROC &lairn->mode) )	
		lair->proctotall ++ ;
	
	if ( 0 == lair->head ) {
		lair->head = lairn ;
		lair->next = lairn ;
	} else {
		lair->next->next = lairn ;
		lair->next = lairn ;
	}

	lair->length ++ ;
	lair->colen = lair->colen + lairn->code.length ;

	return lairn ;

}

char* LairGetCode () {

	//	author : Jelo Wang
	//	since : 20100505
	//	(C)TOK
	
	//	convert LAIR to char*
	
	LAIR* walker = lair->head ; 
	SCClString string = { 0 , 0 , 0 , 0 , 0 } ;

	SCClStringInitEx ( &string , lair->colen ) ;

	for ( ; walker ; walker = walker->next ) {
		SCClStringAddStr ( &string , walker->code.data ) ;
	}	

	SCClStringAdd ( &string , '\0' ) ;

	return string.data ;

	
}

LAIR* LairGetCodeEx ( int number ) {

	//	author : Jelo Wang
	//	since : 20100505
	//	(C)TOK
	
	LAIR* walker = lair->head ; 
	
	for ( ; walker && number != walker->number ; walker = walker->next ) ;

	return walker ;

}

void LairAddCodeLabel ( int lairn , char* label ) {

	//	author : Jelo Wang
	//	since : 20100505
	//	(C)TOK
	
	LAIR* lairhandle = (LAIR* ) lairn ;
		
	if ( !lairhandle ) return ;
	if ( 1 == lairhandle->havelabel ) return ; 
	
	SCClStringAddStr ( &lairhandle->code , label ) ;	
	SCClStringAdd ( &lairhandle->code , '\0' ) ;	

	lairhandle->havelabel = 1 ;
	
}



void LairAllocRegister ( int degreesmax ) {
	
	//	author : Jelo Wang
	//	since : 20110118
	//	(C)TOK

	//	1. locate live scopes of LAIR per procedure 
	//	2. build interference graphs with live scopes
	//	3. color the i-graphs
	//	4. alloc register for the live scope based on colored i-graphs
	//	5. out new lair codes

	
	SCClGraph* iG = 0 ;
	int totall_colors = degreesmax ;	
	int lairlsnumber = 0 ;

	int stime = 0 ;
	int etime = 0 ;
	
	LAIR* llooper = 0 ;
	LAIR* newlair = 0 ;
	LAIR* orignallair = lair->head ;

	stime = clock () ;

	RegocRegisterReady ( lair->proctotall ) ;
	//	backup the orignal LAIR context
	LairSwapIn () ;
	
restart :

	newlair = LairCreate () ;
	LairSetContext ( newlair ) ;
	
	for ( llooper = orignallair ; llooper ; ) {

		LAIR* lairnode = 0 ;

		lairnode = LairAddCode ( llooper->code.data , llooper->mode , llooper->scope ) ;
				
readproc :

		if ( LAIR_PROC != llooper->mode ) {
			
			llooper = llooper->next ;
			
		} else {

			lairlsnumber = 0 ;
						
			RegocLiveScopeMoiCreate () ;

			//	get next looper
			for ( llooper = llooper->next ; llooper ; llooper = llooper->next ) {			

				lairnode = LairAddCode ( llooper->code.data , llooper->mode , llooper->scope ) ; 
				
				if ( LAIR_LVA_DELT == lairnode->mode ) {		

					//	添加生命域
					//	将原始的llooper加入LiveScopeMonitor，因为生命域分列时需要操作它
					int lsn = RegocLiveScopeAdd ( llooper->code.data , llooper->scope , llooper->line , (void* )llooper ) ;
					//	生命域编号
					char* value = sc_strcat ( "." , SCClItoa (lsn) ) ;		
					SCClStringAddStr ( &lairnode->code , value ) ;
					SCClStringAdd ( &lairnode->code , 0 ) ;
					SCFree ( value ) ;	

					lairlsnumber ++ ;
					
				} else if ( LAIR_RVA_DELT == lairnode->mode ) {

					//	生命域引用
					//	获取其编号
					int lsn = RegocCheckLiveScope ( lairnode->code.data , lairnode->scope , lairnode->line ) ;
					char* value = 0 ; 
					
					if ( -1 != lsn ) {
						value = sc_strcat ( "." , SCClItoa (lsn) ) ;					
						SCClStringAddStr ( &lairnode->code , value ) ;
						SCClStringAdd ( &lairnode->code , '\0' ) ;
						SCFree ( value ) ;		
					} else if ( -1 == lsn ) {
						//	set register 0 as default if the ls hasnt defined
						SCClStringAddStr ( &lairnode->code , ".0" ) ;
						SCClStringAdd ( &lairnode->code , '\0' ) ;
					}
					
					if ( -1 < lsn ) {
						//	将lair 添加到其引用链，目的在于记录每一个引用
						//	无法染色时将其全部分裂
						int lair = 0 ;					
						lair = RegocLiveScopeGetLAIR () ;
						LairRefChainInsert ( lair , llooper ) ;
					}
					

				} else if ( LAIR_PROC == llooper->mode || 0 == llooper->next ) {

					if ( 0 < lairlsnumber ) {
						//	get another lair proc
						//	coloring the graph of the last proc
						int lair = 0 ;
						
						iG = RegocIGraphCreate () ;
						lair = SCClGraphColoring ( iG , degreesmax ) ;	
						
						if ( -1 != lair && lair ) {						
							//	graph coloring is failed if lair itsnt equal -1
							//	split the problem
							LairLiveScopeSplit ( lair ) ;
							RegocLiveScopeMoiDestroy () ;
							SCClGraphDestroy ( iG ) ;							
							LairClearCode () ;
							
							goto restart ;
							
						}
						
						if ( SC_IG & semo->parameter ) {
							CORENRIGBFSRender ( (SCClGraph* ) iG , 300 , 0 ) ;
						}

						//	start register allocation based on interference-graph					
						RegocRegisterAlloc ( iG , lairlsnumber ) ;
						RegocLiveScopeMoiDestroy () ;	

					}
					
					if ( 0 == llooper->next ) {
						goto success ;
					}

					//	end of LAIR flow
					goto readproc ;
					
				}
				
				
			}

			
		}

		
	}

success :

	//	destroy the orginal LAIR flow
	LairClearCodeEx ( orignallair ) ;

	etime = clock () ;
	
	semo->regoccosts = etime - stime ;
	
	return ;
	
			
}

void LairClearCode () {

	//	author : Jelo Wang
	//	since : 20100505
	//	(C)TOK


	LAIR* walker = lair->head ; 
 
	for ( ; walker ; ) {

		lair->next = walker->next ; 
		SCFree ( walker->code.data ) ;
		SCFree ( walker ) ;
		walker = lair->next ;
		
	}	

	SCFreeEx ( &lair ) ;

} 

void LairClearCodeEx ( LAIR* walker ) {

	//	author : Jelo Wang
	//	since : 20110122
	//	(C)TOK

	for ( ; walker ; ) {

		lair->next = walker->next ; 
		SCFree ( walker->code.data ) ;
		SCFree ( walker ) ;
		walker = lair->next ;
	}	

} 

int LairLabelMoiNew () {

	//	author : Jelo Wang
	//	since : 20100505
	//	(C)TOK

	//	控制流监视器
	
	LABELMOI* monitor = (LABELMOI* ) SCMalloc ( sizeof(LABELMOI) ) ;

	monitor->lair = 0 ;
	monitor->head = 0 ;
	monitor->next = 0 ;

	return (int) monitor ;

} 

void LairLabelMoiSetContext ( int monitor ) {

	//	author : Jelo Wang
	//	since : 20100505
	//	(C)TOK
	
	labelmoi = (LABELMOI* ) monitor ;

} 

void LairLabelMoiAdd ( int lair ) {

	//	author : Jelo Wang
	//	since : 20100505
	//	(C)TOK

	//	将某LAIR 节点加入监控队列
	
	LABELMOI* la = (LABELMOI* ) SCMalloc ( sizeof(LABELMOI) ) ;
		
	la->lair = (LAIR* ) lair ;
	la->head = 0 ;
	la->next = 0 ;
	
	if ( 0 == labelmoi->head ) {
		labelmoi->head = la ;
		labelmoi->next = la ;
	} else {
		labelmoi->next->next = la ;
		labelmoi->next = la ;
	}

}

void LairLabelMoiSetLabel ( char* label ) {

	//	author : Jelo Wang
	//	since : 20100505
	//	(C)TOK

	//	为所监控的LAIR节点添加label
	
	LABELMOI* walker = labelmoi->head ;

	while ( walker ) {
		
		labelmoi->next = walker->next ;
		LairAddCodeLabel ( walker->lair , label ) ;
		SCFree ( walker ) ;
		walker = labelmoi->next ;

	}

	labelmoi->head = 0 ;

}

void LairLabelMoiClear () {

	//	author : Jelo Wang
	//	since : 20100505
	//	(C)TOK

}

