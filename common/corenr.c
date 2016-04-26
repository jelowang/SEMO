
/*

+	Corenr : Compiling-Render ,  part of SC mid-para

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

# include <math.h>
# include "sccl.h"
# include "schal.h"
# include "symbole.h"
# include "lgnosia.h"
# include "lair.h"
# include "corenr.h"
# include "corenr-text.h"

# define CORENRPI 3.1415926
# define CORENRDTR(x) (CORENRPI*x/180.0)

# define X_MIN 200
# define X_MAX 1000
# define Y_MIN 200
# define Y_MAX 1000

# define CORENRRGB565COLOR(color,red,green,blue)\
	color = color | ( (red >> 3) << 11 ) ;\
	color = color | ( (green >> 2) << 5 ) ;\
	color = color | (blue >> 3) ;\

static CORENR* corenr = 0 ;

int CORENRCreatePanel ( int format , int width , int height ) {

	//	author : Jelo Wang
	//	since : 20100802
	//	(C)TOK

	corenr = (CORENR* ) SCMalloc ( sizeof(CORENR) ) ;

	if ( !corenr ) return 0 ;

	corenr->format = format ;
	corenr->width = width ;
	corenr->height = height ;

	if ( CORENR_RGB565 == corenr->format ) {
		corenr->buffer = (unsigned char* ) SCMalloc ( corenr->width*corenr->height << 1 ) ;		
		memset ( corenr->buffer , 0xffff , corenr->width*corenr->height << 1 ) ;
	} else if ( CORENR_RGB888 == corenr->format ) {
		corenr->buffer = (unsigned char* ) SCMalloc ( corenr->width*corenr->height*3 ) ;	
	} else if ( CORENR_RGB8888 == corenr->format ) {
		corenr->buffer = (unsigned char* ) SCMalloc ( corenr->width*corenr->height*4 ) ;
	}
	
	if ( !corenr->buffer ) {
		SCFree ( corenr ) ;
		return 0 ;
	}

	return (int)corenr ;

}

int CORENROutputPanel ( char* path ) {

	//	author : Jelo Wang
	//	since : 20100802
	//	(C)TOK
	
	unsigned char bmp565header [70] = {
	
		0x42, 0x4D, 0x88, 0xD1, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x00, 0x00, 0x00, 0x38, 0x00, 
		0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x01, 0x00, 0x10, 0x00, 0x03, 0x00, 
		0x00, 0x00, 0x42, 0xD1, 0x0C, 0x00, 0x12, 0x0B, 0x00, 0x00, 0x12, 0x0B, 0x00, 0x00, 0x00, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x00, 0xE0, 0x07, 0x00, 0x00, 0x1F, 0x00, 
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00
		
	};
	
	short* panel16 = (short* )((CORENR*)corenr)->buffer ;
	
	void* file = SCHalFileOpen ( path , "wb+" ) ;
	
	if ( !file ) return 0 ;

	SCHalFileWrite ( file , bmp565header , 1 , 70 ) ;

	if ( CORENR_RGB565 == ((CORENR*)corenr)->format ) {

		int x = 0 ;
		int y = 0 ;

		for ( y = ((CORENR*)corenr)->height-1 ; y >= 0 ; y -- ) {
			for ( x = 0 ; x < ((CORENR*)corenr)->width ; x ++ ) {
				int value = panel16 [ y*((CORENR*)corenr)->width + x ] ;
				SCHalFileWrite ( file , &value , 2 , 1 ) ;
			}
		}
		
	}
	
	SCHalFileClose ( file ) ; 

	return 1 ;

}

void CORENRDestroyPanel () {
	
	//	author : Jelo Wang
	//	since : 20100802
	//	(C)TOK
	
	SCFree ( ((CORENR*)corenr)->buffer ) ;
	SCFree ( (void*)corenr ) ;

}


void CORENRDrawPixel ( int x , int y , int red , int green , int blue ) { 
	
	//	author : Jelo Wang
	//	since : 20100802
	//	(C)TOK

	int color = 0 ;
	short* panel16 = (short* )((CORENR*)corenr)->buffer ;

	if ( CORENR_RGB565 == ((CORENR*)corenr)->format ) {
		CORENRRGB565COLOR(color,red,green,blue);
	}	

	if ( 0 > x || 0 > y ) return ;
	else if ( x >= ((CORENR*)corenr)->width || y >= ((CORENR*)corenr)->height ) return ;

	panel16 [ y*((CORENR*)corenr)->width + x ] = color ;

	
}  


void CORENRDrawLine ( int x0 , int y0 , int x1 , int y1 , int red , int green , int blue ) {
	
	//	author : Jelo Wang
	//	since : 20100802
	//	(C)TOK
	
	int color = 0 ;
	int panel_width = ((CORENR*)corenr)->width ;
	int panel_height = ((CORENR*)corenr)->height ;
	short* panel16 = (short* )((CORENR*)corenr)->buffer ;
	float dy,dx,x,y,m = 0 ;
	
	if ( 0 > x0 || 0 > y0 ) return ;
	else if ( x1 >= panel_width || y1 >= panel_height ) return ;

	dx = x1-x0 ;
	dy = y1-y0 ;
	m = dy/dx ;

	if ( CORENR_RGB565 == ((CORENR*)corenr)->format ){
		CORENRRGB565COLOR(color,red,green,blue) ;
	}

	if ( x0 < x1 ) {
		
		if ( m <= 1 && m >= -1 ) {
			
			y = y0 ;
			
			for ( x = x0 ; x <= x1 ; x++ ) {
				panel16 [ ((int)(y+0.5))*panel_width + ((int)x) ] = color ;
				y +=m ;
			}
		}
		
	}


	if ( x0 > x1 ) {
		
		if ( m <=1 && m >= -1 ) {
			
			y = y0 ;
			
			for ( x = x0 ; x >= x1 ; x-- ) {
				panel16 [ ((int)(y+0.5))*panel_width + ((int)x) ] = color ;
				y -= m ;
			}
			
		}
		
	}
	
	if ( y0 < y1 ) {
		
		if ( m >= 1 || m <= -1 ) {
			
			m = 1 / m ;
			x = x0 ;
			
			for ( y = y0 ; y <= y1 ; y++ ) {
				panel16 [ ((int)(y+0.5))*panel_width + ((int)x) ] = color ;
				x+=m;
			}
			
		}
		
	}
	
	if ( y0 > y1 ) {
		
		if ( m <= -1 || m >= 1 ) {

			m = 1 / m ;
			x = x0 ;
			
			for ( y = y0 ; y >= y1 ; y-- ) {
				panel16 [ ((int)(y+0.5))*panel_width + ((int)x) ] = color ;
				x-=m;
			}
			
		}
		
	}


}


void CORENRDrawCircle ( int x , int y , int raduis , int red , int green , int blue ) {

	//	author : Jelo Wang
	//	since : 20100802
	//	(C)TOK

	int theta = 1 ;
	int looper = 0 ;

	for ( looper = 0 ; looper < 360 ; looper ++ , theta ++ ) {

		int xx = raduis * cos(CORENRDTR(theta)) ;
		int yy = raduis * sin(CORENRDTR(theta)) ;
		
		CORENRDrawPixel ( xx+x , yy+y , red , green , blue ) ;
		
	}

	
}

void CORENRFillCircle ( int x , int y , int raduis , int red , int green , int blue ) {

	//	author : Jelo Wang
	//	since : 20100802
	//	(C)TOK

	int xx = 0 ;
	int yy = 0 ;

	int rect_x = x-raduis ;
	int rect_x2 = x+raduis ;

	int rect_y = y-raduis ;
	int rect_y2 = y+raduis ;
	
   	for ( yy = rect_y ; yy < rect_y2 ; yy ++ ) {
		for ( xx = rect_x ; xx < rect_x2 ; xx ++  ) {
			
			int r = sqrt((double)((xx-x)*(xx-x))+((yy-y)*(yy-y))) ;
			
			if ( r < raduis ) CORENRDrawPixel ( xx , yy , red , green , blue ) ;	

		}
	}
	

}

void CORENRDrawRect ( int x , int y , int x_scale , int y_scale , int linewidth , int red , int green , int blue ) {

	//	author : Jelo Wang
	//	since : 20100803
	//	(C)TOK

	int yy = 0 ;
	int xx = 0 ;
	
	int color = 0 ;
	int panel_width = ((CORENR*)corenr)->width ;
	int panel_height = ((CORENR*)corenr)->height ;
	short* panel16 = (short* )((CORENR*)corenr)->buffer ;
	
	if ( 0 > x || 0 > y ) return ;
	else if ( x >= panel_width || y >= panel_height ) return ;

	if ( CORENR_RGB565 == ((CORENR*)corenr)->format ){
		CORENRRGB565COLOR(color,red,green,blue);
	}

	x_scale = x + x_scale ;
	y_scale = y + y_scale ;
	
	for ( yy = y ; yy < y_scale && yy < panel_height ; yy ++ ) {
		for ( xx = x ; xx < x_scale && xx < panel_width ; xx ++ ) {

			if ( yy == y || xx == x || xx == x_scale-1 || yy == y_scale-1 ) {
				panel16 [ xx + (yy * panel_width) ] = color ;
			}

			if ( (yy >= y && yy < (y+linewidth)) || (yy >= (y_scale-linewidth)) ) {
				panel16 [ xx + (yy * panel_width) ] = color ;
			}
			
			if ( (xx >= x && xx < (x+linewidth)) || (xx >= (x_scale-linewidth)) ) {
				panel16 [ xx + (yy * panel_width) ] = color ;
			}	

		}
		
	}

	
}

void CORENRDrawGrid ( int red , int green , int blue ) {

	//	author : Jelo Wang
	//	since : 20100803
	//	(C)TOK
 
	int yy = 0 ;
	int xx = 0 ;
	
	int panel_width = ((CORENR*)corenr)->width ;
	int panel_height = ((CORENR*)corenr)->height ;

	for ( xx = 0 ; xx < panel_width ; xx += 10 ) {
		CORENRDrawLine ( xx , 0 , xx , panel_height-1 , red , green , blue ) ;			
	}

	for ( yy = 0 ; yy < panel_height; yy += 10 ) {
		CORENRDrawLine ( 0 , yy , panel_width-1 , yy , red , green , blue ) ;			
	}	



}

# define CorenrBaiscFunction(x) \
	(( x >= 1.0 ) ? 0 : x)

void CORENRStretch ( int cx , int cy , float radius , int dx , int dy ) {

	//	author : Jelo Wang
	//	since : 20090612
	//	(C)TOK

	int x = 0 ;
	int y = 0 ;
	int xx = 0 ;
	int yy = 0 ;
	int stridey = 0 ;
	
	int half_width = ((CORENR*)corenr)->width >> 1 ;
	int half_height = ((CORENR*)corenr)->height >> 1 ;

	int stride = ((CORENR*)corenr)->width << 1 ;

	float r = 0 ;
	float r1 = 0 ;
	float r2 = 0 ;
	float r3 = 0 ;

	unsigned char* image = (unsigned char*)((CORENR*)corenr)->buffer ;
	unsigned char* outimage = (unsigned char*)SCMalloc ( ((CORENR*)corenr)->width*((CORENR*)corenr)->height << 1 ) ;

	if ( !outimage ) return ;
	
	cy = abs( cy - ((CORENR*)corenr)->height ) ;
	dy = abs( dy - ((CORENR*)corenr)->height ) ;
	
	if ( CORENR_RGB565 == ((CORENR*)corenr)->format ) {
		for ( y = 0 , stridey = 0 ; y < ((CORENR*)corenr)->height ; y++ ) {
			for ( x = 0 ; x < ((CORENR*)corenr)->width ; x++ ) {
				
				r = sqrt ( (x - cx)*(x - cx)+(y - cy)*(y - cy) ) ;
				r2 = r/radius ;

				if ( r2 < 1.0 ) {

					r3 = 1-r2 ;

					if(dx >= half_width )
						xx = abs((int)(x-abs(dx-cx) * CorenrBaiscFunction(r3))) ;
					else
						xx = abs((int)(x+abs(dx-cx) * CorenrBaiscFunction(r3))) ;

				} else xx = x;


				r = sqrt ( (x - cx)*(x - cx)+(y - cy)*(y - cy) ) ;
				r2 = r/radius ;

				if ( r2 < 1.0 ) {

					r3 = 1-r2 ;

					if( dy >= half_height )
						yy = abs ( (int)(y-abs(dy-cy) * CorenrBaiscFunction(r3)) ) ;
					else
						yy = abs ( (int)(y+abs(dy-cy) * CorenrBaiscFunction(r3)) ) ;

				} else yy = y ;

				
				*(outimage + stridey + (x<<1) ) = *(image + yy * stride + (xx<<1) ) ;
				*(outimage + stridey + (x<<1) + 1) = *(image + yy * stride + (xx<<1) + 1) ;

					
			}	

			//	y * stride 
			stridey = stridey + stride ;
				
		}		
	}

	SCMemcpy ( ((CORENR*)corenr)->buffer , outimage , ((CORENR*)corenr)->width*((CORENR*)corenr)->height << 1 ) ;

	SCFree ( outimage ) ;
	
}

static void CORENRDrawTextModel ( int x , int y , const char* words , int red , int green , int blue ) {

	//	author : Jelo Wang
	//	since : 20100802
	//	(C)TOK
	
	int font_walker = 0;
	int rowwalker = 0;
	int colwalker = 0;
	int wdlen = 0;

	//	14x14 点阵
	# define MATRIXSCALE 14

	if ( !words ) return ;

	wdlen = (MATRIXSCALE) / 8 ;

	for( font_walker = 0; font_walker < MATRIXSCALE; font_walker ++ ) {
		for( rowwalker = 0 ;rowwalker < wdlen; rowwalker ++ ) {
	
			for(colwalker = 0; colwalker < 8; colwalker ++ ) {
				if(words[font_walker * wdlen + rowwalker] & (0x80 >> colwalker)) {
					CORENRDrawPixel ( x+rowwalker*8+colwalker , y+font_walker , red , green , blue ) ;
				}
			}
		}
	}

}

static void* CORENRSearchTextModel ( char element ) {

	//	author : Jelo Wang
	//	since : 20100802
	//	(C)TOK
	
	int walker = 0 ;

	for ( walker = 0 ; walker < 94 ; walker ++ ) {
		if ( EX_FONT_CHAR ( element ) == AnsiFont7x14 [ walker ].value ) 
			return AnsiFont7x14 [ walker ].data ;
	}

	return 0 ;

}

static int corenr_get_exp_deep ( EXPR* expression , EXPR* father , int deep , int* totall ) {

	//	author : Jelo Wang
	//	since : 20100802
	//	(C)TOK

	int ld = 0 ;
	int rd = 0 ;
	
	if ( !expression ) {
		return deep ;
	}

	*totall = *totall + 1 ;

	expression->deep = deep ;
	expression->father = father ;
	
	ld = corenr_get_exp_deep ( expression->left , expression , deep+1 , totall ) ; 
	rd = corenr_get_exp_deep ( expression->right , expression , deep+1 , totall ) ;
	
	return ld  > rd ? ld  : rd ;
	
}

void CORENRDrawText ( int x , int y , const char* text , int red , int green , int blue ) {

	//	author : Jelo Wang
	//	since : 20100802
	//	(C)TOK

	int walker = 0 ;

	if ( !corenr || !text ) return ;
	
	for ( ; text[walker] !='\0' && x < corenr->width ; walker ++ ) {
		CORENRDrawTextModel ( x  , y , (char*)CORENRSearchTextModel( text[walker]) , red , green , blue );
		x = x + 8 ;
	}

}

void CORENRExpDFSRender ( void* exp , int x , int y ) {

	//	author : Jelo Wang
	//	since : 20100802
	//	(C)TOK
	
	static int deep = 0 ;

	EXPR* expression = (EXPR* )exp ;
	
	int ratiox1 = x-5 ;
	int ratiox2 = x+5 ;

	AZONAL* azonal = 0 ;
	
	if ( !expression ) 
		return ;

	if ( EXP_OPERATOR == expression->type ) {
		
		extern char* lexerc_get_operator( int ) ;

		CORENRDrawText ( x+5 , y , lexerc_get_operator(expression->handle) , 255 , 0 , 0 ) ;
		CORENRDrawCircle ( x , y , 5 , 255 , 0 , 0 ) ;
		
		CORENRDrawLine ( x , y , ratiox1 , y+40 , 0 , 0 , 0 ) ;
		CORENRDrawLine ( x , y , ratiox2 , y+40 , 0 , 0 , 0 ) ;		
		
	} else if ( EXP_OPERAND == expression->type ) {
		AZONAL* azonal = expression->handle ;		
		CORENRDrawText ( x+5 , y , azonal->name , 0 , 0 , 255 ) ;
		CORENRDrawCircle ( x , y , 5 , 0 , 0 , 0 ) ;		
		azonal = expression->handle ;
	} 
	
	if ( expression->left ) {

		deep ++ ;
		CORENRExpDFSRender ( (int)expression->left , ratiox1 , y+40 ) ;
		deep -- ;

	}

	if ( expression->right ) {

		deep ++ ;
		CORENRExpDFSRender ( (int)expression->right , ratiox2 , y+40 ) ;
		deep -- ;

	}
	
	return ;
	
}

int CORENRExpBFSRender ( void* exp , int x , int y ) {

	//	author : Jelo Wang
	//	since : 20100802
	//	(C)TOK

	int xx = x ;

	//	all nodes that in a same layer
	int allinlayerc = 0 ;
	int allinlayerl = 0 ;
	
	SCClQueue queue = {0} ;
	EXPR* expression = (EXPR* )exp ;

	SCClQueueInit ( &queue ) ;
	SCClQueueEnter ( &queue , (void* )expression ) ;

	while ( expression ) {

		int lastdeep = 0 ;
		int thedeep = 0 ;

		lastdeep = expression->deep ;		
		expression = (EXPR* )SCClQueueOut ( &queue ) ;

		if ( !expression ) break ;

		if ( expression->left ) 
			SCClQueueEnter ( &queue , (void* )expression->left ) ;

		if ( expression->right ) 
			SCClQueueEnter ( &queue , (void* )expression->right ) ;

		thedeep = expression->deep ;	
		
		expression->x = xx ;
		expression->y = y ;

		if ( lastdeep == thedeep ) {

			EXPR* father = 0 ;
			
			father = expression->father ;
			
			if ( EXP_OPERAND == expression->type ) {
				AZONAL* azonal = expression->handle ;						
				CORENRFillCircle ( xx , y , 5 , 70 , 70 , 70 ) ;
				if ( father ) CORENRDrawLine ( expression->x , expression->y , father->x , father->y+7 , 70 , 70 , 70 );
				CORENRDrawText ( xx , y , azonal->name , 70 , 70 , 70 ) ;	
				xx = xx + 20 ;				
			} else if ( EXP_OPERATOR == expression->type ) {
		
				extern char* lexerc_get_operator ( int ) ;				
				CORENRFillCircle ( xx , y , 7 , 200 , 200 , 200 ) ;				
				if ( father ) CORENRDrawLine ( expression->x , expression->y-7 , father->x , father->y+7 , 200 , 200 , 200 );
				CORENRDrawText ( xx-3 , y-7 , lexerc_get_operator(expression->handle) , 0 , 0 , 0 ) ;
				xx = xx + 17 ;
				
			}

			allinlayerc ++ ;
			
		
		} else {

			EXPR* father = 0 ;
			
			y = y + 30 ;
			x = x - 5 ;
			xx = x ;

			expression->x = xx ;
			expression->y = y ;
			father = expression->father ;
			
			if ( EXP_OPERAND == expression->type ) {
				
				AZONAL* azonal = expression->handle ;						
				CORENRFillCircle ( xx , y , 5 , 70 , 70 , 70 ) ;
				if ( father ) CORENRDrawLine ( expression->x , expression->y , father->x , father->y+7 , 70 , 70 , 70 );
				CORENRDrawText ( xx , y , azonal->name , 70 , 70 , 70 ) ;	
				xx = xx + 20 ;
				
			} else if ( EXP_OPERATOR == expression->type ) {
	
				extern char* lexerc_get_operator ( int ) ;
				CORENRFillCircle ( xx , y , 7 , 200 , 200 , 200 ) ;
				if ( father ) CORENRDrawLine ( expression->x , expression->y-7 , father->x , father->y+7 , 200 , 200 , 200 );
				CORENRDrawText ( xx-3 , y-7 , lexerc_get_operator(expression->handle) , 0 , 0 , 0 ) ;
				xx = xx + 17 ;
			}			
			
			if ( allinlayerl < allinlayerc ) 
				allinlayerl = allinlayerc ;

			allinlayerc = 0 ;
			
		}

		
		
	}
	
	return allinlayerl ;
	
}


int CORENRLgaExpRender ( void* lgnosiaa , int anltype , int x , int y ) {

	//	author : Jelo Wang
	//	since : 20100803
	//	(C)TOK	

	SCClList* listlooper = 0 ;
	LGNOSIA* lgnosia = (LGNOSIA*)lgnosiaa ;
	AZONAL* azonal = 0 ; 

	char* name = 0 ;

	int procx = x ;
	int procy = y ;

	//	cacaulate the deepest value of LGA
	int deepestc = 0 ;
	int deepestl = 0 ;

	if ( !lgnosia ) return 0 ;

	azonal = (AZONAL* ) lgnosia->azonal ; 
	
	if ( ISA_FUNCTION == anltype ) {
		name = sc_strcat("   PROC:",azonal->name) ; 
		CORENRDrawText ( x , y , name , 150 , 150 , 150 ) ;
		SCFree ( name ) ;
	} else if ( ISA_IFCF == anltype ) {
		CORENRDrawText ( x , y , "IF Flow" , 150 , 150 , 150 ) ;
		CORENRDrawRect ( procx-2 , procy-2 , sc_strlen("IF Flow")*14 , 14+8 , 1 , 150 , 150 , 150 ) ;
	} else if ( ISA_ELSECF == anltype ) {
		CORENRDrawText ( x , y , "ELSEIF Flow" , 150 , 150 , 150 ) ;
		CORENRDrawRect ( procx-2 , procy-2 , sc_strlen("ELSEIF Flow")*14 , 14+8 , 1 , 150 , 150 , 150 ) ;
	}
		
	y = y + 14+ 8 ;
		
	for ( listlooper = lgnosia->context.head ; listlooper ; listlooper = listlooper->next ) {

	
		AZONAL* azonal = 0 ; 

		lgnosia = (LGNOSIA*)listlooper->element ;
		azonal = (AZONAL* ) lgnosia->azonal ;
		
		if ( !azonal ) return 0 ;
		
		switch ( azonal->azonaltype ) {

			case ISA_IFCF :
				deepestl = CORENRLgaExpRender ( lgnosia->po_chain , ISA_IFCF , x , y+deepestl ) ;
			break ;
			
			case ISA_VARIABLE :	

				if ( lgnosia->parameter.head ) {
					
					int namelen = 0 ;
					int totall = 0 ;
					int deep = 0 ;
						
					EXPR* expression = (EXPR*)lgnosia->parameter.head->element ;

					deep = corenr_get_exp_deep ( expression , 0 , 0 , &totall ) ;			

					if ( 1 == totall ) continue ;
					
					name = sc_strcat(" ",azonal->name) ;
					namelen = sc_strlen(name) ;
						
					x = x + 50 ;
					CORENRDrawText ( x , y+2 , name , 150 , 150 , 150 ) ;
					CORENRDrawRect ( x , y , namelen*10+2 , 14+8 , 1 , 150 , 150 , 150 ) ;	
					CORENRDrawLine ( x+namelen*10+2 , y+10 , x+namelen*10+2+30 , y+10 , 150 , 150 , 150 );
					x = x+namelen*10+2+30 ;
					SCFree ( name ) ;
							
					CORENRDrawText ( x-20-60 , y+14+10 , "H = " , 150 , 150 , 150 ) ;
					CORENRDrawText ( x-20-30 , y+14+10 , SCClItoa (deep) , 150 , 150 , 150 ) ;
							
					CORENRDrawText ( x-20-60 , y+14+14+10 , "N = " , 150 , 150 , 150 ) ;
					CORENRDrawText ( x-20-30 , y+14+14+10 , SCClItoa (totall) , 150 , 150 , 150 ) ;

					deepestc = deep ;

					if ( deepestl < deepestc ) deepestl = deepestc ;								
					
					x = x + CORENRExpBFSRender ( expression , x  , y+10 )*14 ;
					
				}
			
			break ;
		
		}
		
	}

	if ( ISA_FUNCTION == anltype ) CORENRDrawRect ( procx-2 , procy-2 , x , 14+8 , 1 , 150 , 150 , 150 ) ;
	
	return deepestl*37 ;

}


void CORENRCFBFSRender ( void* lgnosiaa , int x , int y ) {

	//	author : Jelo Wang
	//	since : 20100804
	//	(C)TOK	

	int xx = x ;
	int yy = 0 ;
	
	//	all nodes that in a same layer
	int allinlayerc = 0 ;
	int allinlayerl = 0 ;
	
	int raduis = 0 ;
	int red = 0 ;
	int green = 0 ;
	int blue = 0 ;
			
	SCClList* looper = 0 ;
	SCClQueue queue = {0} ;
	LGNOSIA* lgnosia = (LGNOSIA* )lgnosiaa ;
	AZONAL* azonal = 0 ;
		
	SCClQueueInit ( &queue ) ;
	SCClQueueEnter ( &queue , (int)lgnosia ) ;

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
				if ( looper->element ) SCClQueueEnter ( &queue , (void* )looper->element ) ;
			}

		}
		
		thedeep = lgnosia->deep ;	

		if ( lastdeep == thedeep ) {
			
			LGNOSIA* father = 0 ;
			
			father = lgnosia->father ;

			if ( LGNOSIA_CP_IDENT == lgnosia->type ) {
				raduis = 4 ;
				red = 0 ;
				green = 0 ;
				blue = 255 ;
				xx = xx + 0 ;
			} else if ( LGNOSIA_POC_IDENT == lgnosia->type ) {
				raduis = 4 ;
				red = 0 ;
				green = 255 ;
				blue = 0 ;	
				xx = xx + 15 ;
			} else if ( LGNOSIA_NEC_IDENT == lgnosia->type ) {			
				raduis = 4 ;
				red = 255 ;
				green = 0 ;
				blue = 0 ;
				xx = xx - 15 ;
			} else {
				raduis = 4 ;
				red = 0 ;
				green = 0 ;
				blue = 0 ;			
				xx = xx + 15 ;
			}


			lgnosia->x = xx ;
			lgnosia->y = y ;

			CORENRFillCircle ( xx , y , raduis , red , green , blue ) ;
			if ( father ) CORENRDrawLine ( lgnosia->x , lgnosia->y , father->x , father->y , red , green , blue );
		
			allinlayerc ++ ;
					
			
		} else {

			
			LGNOSIA* father = 0 ;
			
			y = y + 30 ;
		
			father = lgnosia->father ;
			
			if ( LGNOSIA_CP_IDENT == lgnosia->type ) {
				raduis = 4 ;
				red = 0 ;
				green = 0 ;
				blue = 255 ;
				xx = xx + 0 ;
			} else if ( LGNOSIA_POC_IDENT == lgnosia->type ) {
				raduis = 4 ;
				red = 0 ;
				green = 255 ; 
				blue = 0 ;	
				xx = xx + 15 ;
			} else if ( LGNOSIA_NEC_IDENT == lgnosia->type ) {			
				raduis = 4 ;
				red = 255 ;
				green = 0 ;
				blue = 0 ;
				xx = xx - 15 ;
			} else {
				raduis = 4 ;
				red = 0 ;
				green = 0 ;
				blue = 0 ;			
				xx = xx + 15 ;
			}


			lgnosia->x = xx ;
			lgnosia->y = y ;

			CORENRFillCircle ( xx , y , raduis , red , green , blue ) ;
			if ( father ) CORENRDrawLine ( lgnosia->x , lgnosia->y , father->x , father->y , red , green , blue );
			
			if ( allinlayerl < allinlayerc ) 
				allinlayerl = allinlayerc ;

			allinlayerc = 0 ;

		}

		
	}
	
	
}

int CORENRCFDFSRender ( void* lgnosiaa , int type , int x , int y , int fx , int fy , int deep ) {

	//	author : Jelo Wang
	//	since : 20100804
	//	(C)TOK	

	int xx = x ;

	char* name = 0 ;
	
	SCClList* looper = 0 ;
	LGNOSIA* lgnosia = (LGNOSIA* )lgnosiaa ;
  	AZONAL* azonal = 0 ;

	if ( !lgnosia ) return 0 ;

	//	the deep must be above 0
	if ( 1 > deep ) return 0 ;

	if ( 1 == deep ) LgnosiaBFSNormalize ( lgnosia ) ;

   	if ( LGNOSIA_POC_IDENT == type ) {
		CORENRDrawText ( xx-7 , y-30 , "POC" , 0 , 255 , 0 );
		CORENRDrawLine ( xx , y , fx , fy , 0 , 255 , 0 );
		CORENRDrawRect ( xx-7 , y , 20 , 20 , 2 , 0 , 255 , 0 ) ;	
	} else if ( LGNOSIA_NEC_IDENT == type ) {
		CORENRDrawText ( xx-7 , y-30 , "NEC" , 255 , 0 , 0 );
		CORENRDrawLine ( xx , y , fx , fy , 255 , 0 , 0 );
		CORENRDrawRect ( xx-7 , y , 20 , 20 , 2 , 255 , 0 , 0 ) ;	
	} else {
		CORENRDrawLine ( xx , y , fx , fy , 0 , 0 , 0 );	
		CORENRDrawRect ( xx-7 , y , 20 , 20 , 2 , 0 , 255 , 0 ) ;	
	}	
	
	for ( looper = lgnosia->context.head ; looper ; looper = looper->next ) {
		
		if ( LGNOSIA_CP_IDENT == looper->eltype ) {

			LGNOSIA* lga = (LGNOSIA* )looper->element ;
			LGNOSIA* poc = lga->po_chain ;
			LGNOSIA* nec = lga->ne_chain ;
			
			CORENRFillCircle ( xx , y , 3 , 0 , 0 , 255 ) ;

			CORENRCFDFSRender ( (void* )poc , LGNOSIA_POC_IDENT , xx - (lga->allinyer+400)*(1.0/deep) , y + 30 , xx , y , deep + 1 ) ;
			CORENRCFDFSRender ( (void* )nec , LGNOSIA_NEC_IDENT , xx + (lga->allinyer+400)*(1.0/deep) , y + 30 , xx , y , deep + 1 ) ;
		
			xx = xx + (lga->allinyer+400)*(1.0/deep) ;

		} 

	}

	
	
}

void CORENRIGBFSRender ( SCClGraph* graph , int x , int y ) {

	//	author : Jelo Wang
	//	since : 20110119
	//	(C)TOK	
	

	int* visited = 0 ;
	
	SCClQueue queue = {0} ;
	SCClList* looper = 0 ;	
	SCClList* inlooper = 0 ;	
	
	if ( !graph ) return ;
	
	visited = (int* ) SCMalloc ( sizeof(int)*graph->totall ) ;

	ASSERT(visited) ;

	CORENRDrawText ( 30 , 100 , "interference-graph" , 150 , 150 , 150 ) ;	
	CORENRDrawText ( 30 , 115 , "live scopes : " , 150 , 150 , 150 ) ;	
	CORENRDrawText ( 135 , 115 , SCClItoa ( graph->totall ) , 150 , 150 , 150 ) ;	

	SCClQueueInit ( &queue ) ;	

	for ( looper = graph->nl.head ; looper ; looper = looper->next ) {
				
		SCClQueueEnter ( &queue , (void* )looper->element ) ;
		
		while( !SCClQueueEmpty ( &queue) ) {

			SCClGraphNode* genode = 0 ;
			
			genode = (SCClGraphNode* ) SCClQueueOut ( &queue) ;
			
			if ( !visited[genode->id] ) {
				visited[genode->id] = 1 ;
				genode->x = sc_randex ( X_MIN , X_MAX ) + x ;
				genode->y = sc_randex ( Y_MIN , Y_MAX ) + y ;
		 		CORENRFillCircle ( genode->x , genode->y , 15 , genode->color*sc_randex ( 10 , 150 ) , genode->color*sc_randex ( 10 , 150 ) , 0 ) ;	
				
			}

			for ( inlooper = genode->nei.head ; inlooper ; inlooper = inlooper->next ) {    

				SCClGraphNode* innode = (SCClGraphNode* ) inlooper->element ;	

				if( !visited[innode->id] ) {

					visited[innode->id] = 1 ;

					innode->x = sc_randex ( X_MIN , X_MAX ) + x ;
					innode->y = sc_randex ( Y_MIN , Y_MAX ) + y ;
					
	 				CORENRFillCircle ( innode->x , innode->y , 15 , innode->color*sc_randex ( 0 , 50 ) , innode->color*sc_randex ( 50 , 150 ) , 0 ) ;					
					SCClQueueEnter ( &queue , (void* )innode ) ;
					
				}
	
			}			

		}
	}


	{
		
		SCClList* llooper = 0 ;
		SCClList* inlooper = 0 ;
		SCClGraphNode* node = 0 ;		
		
		for ( llooper = graph->nl.head ; llooper ; llooper = llooper->next ) {

			LAIR* lair = 0 ;
			
			int textcolor = 0 ;
			
			node = (SCClGraphNode* ) llooper->element ;

			lair = (LAIR* ) node->handle ;
			
			textcolor = ~(node->color*sc_randex ( 10 , 150 )) ;

			CORENRDrawText ( node->x , node->y , lair->code.data, textcolor , textcolor , 0 ) ;	
			CORENRDrawText ( node->x , node->y-15 , SCClItoa(node->degree), textcolor , textcolor , 0 ) ;	

			for ( inlooper = node->nei.head ; inlooper ;  inlooper = inlooper->next ) {
				
				SCClGraphNode* innode = (SCClGraphNode* ) inlooper->element ;
				textcolor = ~(innode->color*sc_randex ( 0 , 50 ) , innode->color*sc_randex ( 50 , 150 ) )  ;
					
				CORENRDrawLine ( node->x , node->y , innode->x , innode->y , innode->color*sc_randex ( 0 , 50 ) , innode->color*sc_randex ( 0 , 50 ) , 0 ) ;
			
			}
		
							
		}	
	}
	
	SCFree ( visited ) ;
	
}

