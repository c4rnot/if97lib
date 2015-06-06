//       Copyright Martin Lord 2014-2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)



/* *****************************************************************************
* A SHORT PROGRAMME TO CHECK THE LIBRARY
* *******************************************************************************/


#include "if97_lib.h"
#include "iapws_surftens.h"
#include <stdio.h>


int main (int argc, char **argv)
{

	printf ("\n\n *** Testing  if97_pt_h  ****** \n\n" );
	printf ("The output should be the following: \n\n");
	printf ("p (MPa)	T (K)	h (Kj/kg)\n");
	printf ("3	300	1.15331273e02\n");	
	printf ("80	300	1.84142828e02\n");	
	printf ("0.0035	300	2.54991145e03\n");	
	printf ("*** Region 3 not yet implimented ***\n");	
	printf ("30	1500	5.16723514e03\n");	
			
	printf ("\n\n**********RESULTS******\n");
	
	printf ("p (MPa)	T (K)	h (Kj/kg)\n");
	printf ("3	300	%.8e\n", if97_pt_h(3.0, 300.0));
	printf ("80	300	%.8e\n", if97_pt_h(80.0, 300.0));	
	printf ("0.0035	300	%.8e\n", if97_pt_h(0.0035, 300.0));	
	printf ("Region 3 not yet implimented	%.8e\n", if97_pt_h(80.0, 650.0));
	printf ("30	1500	%.8e\n\n", if97_pt_h(30.0, 1500.0));	

//*************************************

	printf ("\n\n *** Testing IAPWS Surface Tension function  ****** \n\n" );
	printf ("The output should be the following: \n\n");
	printf ("t (°C)	Surface Tension (mN/m)\n");
	printf ("0.01	75.65\n");	
	printf ("20	72.74\n");	
	printf ("100	58.91\n");	
	printf ("200	37.67\n");	
	printf ("300	14.36\n");	
	printf ("370	0.39\n");	

	printf ("\n\n**********RESULTS******\n");
	printf ("t (°C)	Surface Tension (mN/m)\n");
	printf ("0.01\t%.2f\n", iapws_surftens(0.01));
	printf ("20\t%.2f\n", iapws_surftens(20.0));
	printf ("100\t%.2f\n", iapws_surftens(100.0));
	printf ("200\t%.2f\n", iapws_surftens(200.0));
	printf ("300\t%.2f\n", iapws_surftens(300.0));
	printf ("370\t%.2f\n", iapws_surftens(370.0));

return 0;
}
