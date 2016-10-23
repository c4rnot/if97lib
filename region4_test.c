//          Copyright Martin Lord 2014-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)



/* *****************************************************************************
* A SHORT PROGRAMME TO CHECK REGION 4 (SATURATION LINE) EQUATIONS 
* *****************************************************************************/


#include "IF97_Region4.h"
#include <stdio.h>



int main (int argc, char **argv)
{

/* **************************************
 * SATURATION LINE EQUATION:  Psat from T
* see IAPWS-IF97 Table 35
*
* The results should be as follows
* 
* T (K)			Psat (MPa)
* 300			0.353 658 941 E –2
* 500			0.263 889 776 E 1
* 600			0.123 443 146 E 2
*/ 
	printf ("\n\n************************************************************ \n" );
	printf ("IF97 REGION 4 (SATURATION LINE) EQUATIONS CHECK Psat(T) \n\n" );
	printf ("The output should be the following: \n\n");
	printf ("\tT (°K) \t\tPsat (MPa) \n");
	printf ("\t300		3.53658941e-03\n");
	printf ("\t500		2.63889776e+00\n");
	printf ("\t600		1.23443146e+01\n");
	printf ("\n*******OUTPUT************\n\n");

	printf ("\tT (°K) \t\tPsat (MPa) \n");
	printf ("\t300\t\t%.8e\n", if97_r4_ps(300.0) );		
 	printf ("\t500\t\t%.8e\n", if97_r4_ps(500.0) );
 	printf ("\t600\t\t%.8e\n", if97_r4_ps(600.0) );
  
  
 
 
/* ****************************************
 * SATURATION LINE EQUATION:  Psat from T
 * see IAPWS-IF97 Table 36
 *
 * The results should be as follows
 * 
 * P (MPa)	Ts (K)
 * 0.1		0.372 755 919 E 3
 * 1		0.453 035 632 E 3
 * 10		0.584 149 488 E 3
 */

	printf ("\n\n************************************************************ \n" );
	printf ("IF97 REGION 4 (SATURATION LINE) EQUATIONS CHECK Tsat(P) \n\n" );
	printf ("The output should be the following: \n\n");
	printf ("\tPsat (MPa) \tT (°K) \n");
	printf ("\t0.1		3.72755919e0+2\n");
	printf ("\t1		4.53035632e+02\n");
	printf ("\t10		5.84149488e+02\n");
	printf ("\n*******OUTPUT************\n\n");

	printf ("\tPsat (MPa) \t\tT (°K) \n");
	printf ("\t0.1\t\t%.8e\n", if97_r4_ts(0.1) );		
 	printf ("\t1.0\t\t%.8e\n", if97_r4_ts(1.0) );
 	printf ("\t10.0\t\t%.8e\n", if97_r4_ts(10.0) );
  
return 0 ;
}

 
