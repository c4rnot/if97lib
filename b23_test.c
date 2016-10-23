
//          Copyright Martin Lord 2014-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// ****SHORT PROGRAM FOR UNIT TESTING *******

// For verifivation the above equations must meet the following T-p point
//
// T= 0.623 150 000 E3 Kelvin
// p= 0.165 291 643 E2 MPa
//

//#include "B23test.h"
#include "IF97_B23.h"
#include <stdio.h>

int main(int argc, char **argv)
{
	
	printf("\n\nIF97 REGION 2-3 BOUNDARY FUNCTION CHECK \n\n" );
	printf ("The output should be the following: \n");
	printf("Pressure: \t\t1.65291643e01 MPa \n");	
	printf("Temperature: \t\t6.23150000e02 Kelvin \n\n*******OUTPUT************\n");	
	
	printf ( "IF97_B23P pressure \t%.8e MPa\n" , IF97_B23P (0.623150000e3) ); 
	printf ( "IF97_B23T temperature \t%.8e Kelvin\n\n" , IF97_B23T (0.165291643e2) ); 
	
	return 0;
}
