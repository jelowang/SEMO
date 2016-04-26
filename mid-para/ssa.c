
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

# include "sccl.h"
# include "schal.h"
# include "symbole.h"

//	what a junk codes it is needs 256 ssa alias !
# define SSA_MULTI_ALIAS_MAX 256


///
//	识别多重别名很简单，我们只要进入一个作用域后将所有出现的symbol保存起来。
//	注意是把symbol每次的别名保存起来，然后出作用域后遇到的某symbol第一个赋值语句进行
//	查找，看看其是否有多重别名可用，根据作用域只有一个出口的特性，有效的多重域名
//	一定是最后一个被保存的别名。

//	如何保存有效别名?

//	多重别名出现的条件是发生了作用域嵌套，根据这个性质，一个符号的多重别名至少有两个。
//	第一个为作用域嵌套前的别名，第二个为嵌套作用域出口的别名。
//	在此我们把第一个别名定义为ancestor ，这意味着我们需要把同一个作用域内的
//	别名存起来，当获取多重别名时首先获取同一作用域内的ancestor 。
//

typedef struct {

	//	author : Jelo Wang
	//	since : 20121026
	//	(C)TOK	

	//	an abstractive symbol
	int symbol ;
	//	a number between 0,...n
	int alias ;
	//	the last flow has apeared
	int flow ;
	int scope ;

	struct {

		int totall ;
		//	the smallest scope of any of a alias
		//	基于这个条件识别是否有多重别名
		int scope ;
		int alias[ SSA_MULTI_ALIAS_MAX ] ;
		
	} MultiAlias ;
		
} SSA ;

typedef struct {

	//	author : Jelo Wang
	//	since : 20121026
	//	(C)TOK	

	int symbol ;
	int alias ;
	int scope ;
	
} ANCESTOR ;

static int SsaMultiAliasEnable = 0 ;
static SCClList aliaslist = { 0 } ;
static SCClList ancestorlist = { 0 } ;

static int SsaGetMultiAliasValid ( int symbol , int flow , int scope ) {

	//	author : Jelo Wang
	//	since : 20121026
	//	(C)TOK	

	//	判断是否可获得多重别名
	//	只要发生了flow迁移就需要判断

	SSA* ssa = 0 ;
	SCClList* looper = 0 ;
	
	for ( looper = aliaslist.head ; looper ; looper = looper->next ) {
		ssa = (SSA* ) looper->element ;
		if ( (int ) symbol == (int ) ssa->symbol ) {
			break ;
		}
	}	
	
	if ( 0 != looper ) {		
		//	多重别名作用域最大深度大于当前深度
		//	并且已经发生了flow迁移
		if ( ssa->flow != flow ) {
			return 1 ;
		}
		return 0 ;
	}

	return 0 ;
	
}

static void SsaSaveAncestorAlias ( int symbol , int alias , int scope ) {

	ANCESTOR* ancestor = 0 ;

	SCClListSetIterator ( &ancestorlist , SCCLLISTSEEK_HEAD ) ;
	
	for ( ; SCClListIteratorPermit ( &ancestorlist ) ; SCClListListIteratorNext ( &ancestorlist ) ) {

		ancestor = (ANCESTOR* ) SCClListIteratorGetElement ( &ancestorlist ) ;

		if ( 0 != ancestor && (int ) ancestor->symbol == (int ) symbol ) {
			if ( ancestor->scope == scope ) {
				//	update the lastest alias of 'symbol'
				ancestor->alias = alias ;
			} else {
				//	insert new
				ancestor = (ANCESTOR* ) SCMalloc ( sizeof(ANCESTOR) ) ;
				ASSERT(ancestor) ;
				ancestor->symbol = symbol ;
				ancestor->alias = alias ;
				ancestor->scope = scope ;		
				SCClListInsert ( &ancestorlist , (void* ) ancestor ) ;			
				
			}
		}

	}

	if ( 0 == ancestor ) {

		ancestor = (ANCESTOR* ) SCMalloc ( sizeof(ANCESTOR) ) ;
		ASSERT(ancestor) ;
		ancestor->symbol = symbol ;
		ancestor->alias = alias ;
		ancestor->scope = scope ;		
		SCClListInsert ( &ancestorlist , (void* ) ancestor ) ;
	}
		
	
}

int SsaMakeAlias ( int flow , int symbol , int scope ) {

	//	author : Jelo Wang
	//	since : 20121026
	//	(C)TOK	

	SSA* ssa = 0 ;
	SCClList* looper = 0 ;
	
	for ( looper = aliaslist.head ; looper ; looper = looper->next ) {
		ssa = (SSA* ) looper->element ;
		if ( (int ) symbol == (int ) ssa->symbol ) {
			break ;
		}
	}

	if ( 0 != looper ) {

		//	if there have a ssa form exist already , we just return it here as well
		//	increase it and return
		ssa->alias ++ ;
		
		//	save multi alias
		if ( SsaMultiAliasEnable ) {

			//	if it still in the same flow then dont increase totall just rewrite the last one
			if ( ssa->flow != flow ) {
				ssa->MultiAlias.totall ++ ;
			}

			//	only save the smallest one
			//	有没有多重别名的条件就是拿当前作用域跟ssa->MultiAlias.scope
			//	比较看谁最深
			if ( scope > ssa->MultiAlias.scope ) {
				ssa->MultiAlias.scope = scope ;
			}

			ssa->MultiAlias.alias [ssa->MultiAlias.totall] = ssa->alias ;

		}
		ssa->flow = flow ;
		ssa->scope = scope ;
		//	可以假设所有别名都是ancestor，这是没问题
		SsaSaveAncestorAlias ( symbol ,  ssa->alias , scope ) ;			
		return ssa->alias ;

	} else {

		//	else , save the symbol , and make a SSA form
		ssa = (SSA* ) SCMalloc ( sizeof(SSA) ) ;
		ASSERT(ssa) ;
		ssa->symbol = symbol ;
		ssa->alias = 0 ;
		ssa->flow = flow ;	
		ssa->scope = scope ;
		
		SCClListInsert ( &aliaslist , (void* ) ssa ) ;

		//	save multi alias
		if ( SsaMultiAliasEnable ) {
			ssa->MultiAlias.scope = scope ;			
			ssa->MultiAlias.alias [ssa->MultiAlias.totall] = ssa->alias ;
			//	这里不需要递加
			//	ssa->MultiAlias.totall ++ ;			
		}

		//	可以假设所有别名都是ancestor，这是没问题
		SsaSaveAncestorAlias ( symbol ,  ssa->alias , scope ) ;

		return ssa->alias ;

	}

}

int SsaGetAlias ( int symbol , int scope ) {

	//	author : Jelo Wang
	//	since : 20121026
	//	(C)TOK	

	SSA* ssa = 0 ;
	SCClList* looper = 0 ;
	
	for ( looper = aliaslist.head ; looper ; looper = looper->next ) {
		ssa = (SSA* ) looper->element ;
		if ( (int ) symbol == (int ) ssa->symbol ) {
			break ;
		}
	}
		
	if ( 0 != looper ) {

		//	if found , we just return it here as well
		return ssa->alias ;

	} else {
	
		return 0 ;

	}	
	
}

int SsaGetAliasFlow ( int symbol ) {

	//	author : Jelo Wang
	//	since : 20121026
	//	(C)TOK	

	SSA* ssa = 0 ;
	SCClList* looper = 0 ;
	
	for ( looper = aliaslist.head ; looper ; looper = looper->next ) {
		ssa = (SSA* ) looper->element ;
		if ( (int ) symbol == (int ) ssa->symbol ) {
			break ;
		}
	}
		
	if ( 0 != looper ) {

		//	if found , we just return it here as well
		return ssa->flow ;

	} else {
	
		return 0 ;

	}	
	
}

int SsaGetAncestorAlias ( int symbol , int scope ) {
	
	//	author : Jelo Wang
	//	since : 20121026
	//	(C)TOK	

	ANCESTOR* ancestor = 0 ;

	SCClListSetIterator ( &ancestorlist , SCCLLISTSEEK_HEAD ) ;
	
	for ( ; SCClListIteratorPermit ( &ancestorlist ) ; SCClListListIteratorNext ( &ancestorlist ) ) {

		ancestor = (ANCESTOR* ) SCClListIteratorGetElement ( &ancestorlist ) ;

		if ( 0 != ancestor && (int ) ancestor->symbol == (int ) symbol ) {
			if ( ancestor->scope == scope ) {
				return ancestor->alias ;
			}
		}

	}

	return -1 ;

	
}

void SsaMakeMultiAliasEnable () {

	//	author : Jelo Wang
	//	since : 20121026
	//	(C)TOK	
	
	SsaMultiAliasEnable ++ ;
	
} ;

void SsaMakeMultiAliasDisable () {

	//	author : Jelo Wang
	//	since : 20121026
	//	(C)TOK	
	
	SsaMultiAliasEnable -- ;

} ;

int* SsaGetMultiAlias ( int symbol , int* totall ) {

	//	author : Jelo Wang
	//	since : 20121026
	//	(C)TOK	
	
	SSA* ssa = 0 ;
	SCClList* looper = 0 ;
	
	for ( looper = aliaslist.head ; looper ; looper = looper->next ) {
		ssa = (SSA* ) looper->element ;
		if ( (int ) symbol == (int ) ssa->symbol ) {
			break ;
		}
	}	
	
	if ( 0 != looper ) {		
		if ( 0 != ssa->MultiAlias.totall ) {
			*totall = ssa->MultiAlias.totall + 1 ;
		}
		return ssa->MultiAlias.alias ;
	}

	return 0 ;
	
}

void SsaCleanMultiAlias ( int symbol , int flow ) {

	//	author : Jelo Wang
	//	since : 20121026
	//	(C)TOK	

	SSA* ssa = 0 ;
	SCClList* looper = 0 ;
	
	for ( looper = aliaslist.head ; looper ; looper = looper->next ) {
		ssa = (SSA* ) looper->element ;
		if ( (int ) symbol == (int ) ssa->symbol ) {
			break ;
		}
	}	
	
	if ( 0 != looper ) {	
		//	一旦外界发生了AST回溯，即可清除多重别名记录
		if ( ssa->flow != flow ) {
			ssa->MultiAlias.totall = 0 ;
		}
	}


}

char* SsaMakeMultiAliasString ( int symbol , char* name , int scope , int flow ) {

	//	author : Jelo Wang
	//	since : 20121026
	//	(C)TOK	

	//	out string "0,1,2,3,4...n"

	int looper = 0 ;
	int totallmultialias = 0 ;
	int* multialias = 0 ;

	SCClString string = { 0 } ;
	char* azonalname = 0 ;

	int ancestor = 0 ;
	int alias = 0 ;

	if ( 0 == SsaGetMultiAliasValid ( symbol , flow , scope ) ) {
		return 0 ;
	}
	
	multialias = SsaGetMultiAlias ( symbol , &totallmultialias ) ;
	
	if ( 0 == totallmultialias ) {
		return 0 ;
	}
	
	//	get ancestor first
//	ancestor = SsaGetAncestorAlias ( symbol , scope ) ;
//	if ( -1 != ancestor ) {
//		azonalname = sc_strcat ( name , SCClItoa (ancestor) ) ;
//		ASSERT(azonalname) ;
//		SCClStringAddStr ( &string , azonalname ) ;
//		SCClStringAdd( &string , ',' ) ;
//	} 
	
	for ( looper = 0 ; looper < totallmultialias ; looper ++ ) {
		
		azonalname = sc_strcat ( name , SCClItoa (multialias[looper]) ) ;
		ASSERT(azonalname) ;
		SCClStringAddStr ( &string , azonalname ) ;
		if ( (looper+1) < totallmultialias ) SCClStringAdd( &string , ',' ) ;
		SCFree ( azonalname ) ;
		
	}

	return string.data ;
	
	
}

void SsaClean () {

	//	author : Jelo Wang
	//	since : 20121027
	//	(C)TOK	

	SCClListDestroy ( &aliaslist ) ;
	sc_memset ( &aliaslist , 0 , sizeof(SCClList) ) ;
	
}


