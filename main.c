# include "sccl.h"
# include "semo.h"

int main ( int argc , char* argv [] ) {

	# ifdef SEMO_DEBUG
	{

		int count = 4 ;
		 
		char* value [] = {
			
			"sc",
			"ca.txt" ,
			" -cr" ,
			" -lair" ,

		} ;
   
   		SCCompile ( count , value , SC_STATIC_COMPILING ) ;

		return 0 ;
	}
	# else 	
	{
		
	   	SCCompile ( argc , argv , SC_STATIC_COMPILING ) ;
	  
 		return 0 ;
	}
	# endif

	return 0 ;

 
}   
