
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

# ifndef __CORENR
# define __CORENR

enum {

	//	author : Jelo Wang
	//	since : 20100802
	//	(C)TOK

	CORENR_RGB565 ,
	CORENR_RGB888 ,	
	CORENR_RGB8888 ,	
	
} ;

typedef struct {

	//	author : Jelo Wang
	//	since : 20100802
	//	(C)TOK

	unsigned char* buffer ;
	int width ;
	int height ;
	int format ;

} CORENR ;

# endif

extern int CORENRCreatePanel ( int format , int width , int height ) ;
extern int CORENROutputPanel ( char* path ) ;
extern void CORENRDestroyPanel () ;

extern void CORENRDrawPixel ( int x , int y , int red , int green , int blue ) ;
extern void CORENRDrawLine ( int x1 , int y1 , int x2 , int y2 , int red , int green , int blue ) ;
extern void CORENRDrawCircle ( int x , int y , int raduis , int red , int green , int blue ) ;
extern void CORENRFillCircle ( int x , int y , int raduis , int red , int green , int blue ) ;
extern void CORENRDrawText ( int x , int y , const char* text , int red , int green , int blue ) ;
extern void CORENRDrawRect ( int x , int y , int x_scale , int y_scale , int linewidth , int red , int green , int blue ) ;
extern void CORENRStretch ( int cx , int cy , float radius , int dx , int dy ) ;
extern void CORENRDrawGrid ( int red , int green , int blue ) ;

extern void CORENRExpDFSRender ( void* exp , int x , int y ) ;
extern int CORENRLgaExpRender ( void* lgnosiaa , int anltype , int x , int y ) ;
extern void CORENRCFBFSRender ( void* lgnosiaa , int x , int y ) ;
extern int CORENRCFDFSRender ( void* lgnosiaa , int type , int x , int y , int fx , int fy , int deep ) ;
extern int CORENRExpBFSRender ( void* exp , int x , int y ) ;	
extern void CORENRIGBFSRender ( SCClGraph* graph , int x , int y ) ;

