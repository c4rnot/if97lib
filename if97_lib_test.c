//       Copyright Martin Lord 2014-2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)



/* *****************************************************************************
* A SHORT PROGRAMME TO CHECK THE LIBRARY
* *******************************************************************************/


#include "if97_lib.h"
#include "iapws_surftens.h"
#include "solve.h"
#include <stdio.h>
#include "winsteam_compatibility.h"



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
	printf ("Tcrit	0.0\n");	

	printf ("\n\n**********RESULTS******\n");
	printf ("t (°C)	Surface Tension (mN/m)\n");
	printf ("0.01\t%.2f\n", iapws_surftens(0.01));
	printf ("20\t%.2f\n", iapws_surftens(20.0));
	printf ("100\t%.2f\n", iapws_surftens(100.0));
	printf ("200\t%.2f\n", iapws_surftens(200.0));
	printf ("300\t%.2f\n", iapws_surftens(300.0));
	printf ("370\t%.2f\n", iapws_surftens(370.0));
	printf ("Tcrit\t%.2f\n", iapws_surftens(647.096-273.15));


	printf ("\n\n *** Testing function call by reference  ****** \n\n" );
		printf ("The output should be the following: \n\n");	
	printf ("3	300	1.15331273e02\n");		
		printf ("\n\n**********RESULTS******\n");
		
	// *func is a pointer to a function which takes two doubles and returns a double
	// here we make it point to if97_ph_t
	double (*func)(double, double) = &if97_pt_h;
	printf ("3	300	%.8e\n", (*func)(3.0, 300.0));	
	
	/* try to solve p(h, t) using secant solver.  one from table 18.
	 * 
	 * @ 450 K 1.5 MPa, enthalpy should be 2.72134539e03 kJ/kg
	 */
	 
	printf ("\n\n *** Testing secant solver  ****** \n\n" );
	printf ("The output should be the following: \n\n");
	printf ("p (MPa)h (Kj/kg)	T (K)	\n");
	printf ("1.5	2.72134539e03	450.0\n");		
	printf ("\n\n**********RESULTS******\n");	  
	printf ("p (MPa)	h (Kj/kg)	T (K)\n");	
	
	printf ("first result should be 	%.8f\n", if97_pt_h(30.0,650));	
	
	typSolvResult solution =  secant_solv(&if97_pt_h, 30.0, true,  2.63149474e03, 710.0, 0.05, 0.000000001, 20 );
	printf ("1.5	2.72134539e03	%.8f\n", solution.dSolution);
	printf ("iterations	%li\n", solution.lIterations);		
	printf ("error code	%i\n", solution.iErrCode);		
	 
		printf  ("\n\n\n");
	 
	printf ("\n\n *** Testing Winsteam Compatibility  ****** \n\n" );
	printf ("StmPT (100 [bar], ""SI"" should be 310.99 °C:  Calculated as %.8f\n", StmPT(100, "SI"));
	printf ("StmPT (100 [bar], ""1"" should be 310.99 °C:  Calculated as %.8f\n", StmPT(100, "1"));
	printf ("StmPT (100 [psi], ""eng"" should be 327.82 °F:  Calculated as %.8f\n", StmPT(100, "enG"));
	printf ("StmPT (100 (psig), ""engg"" should be 337.88 °F:  Calculated as %.8f\n", StmPT(100, "engg"));
	printf ("StmPT (100 (psig), ""enggo"" should fail:  Calculated as %.8f\n", StmPT(100, "enggo"));	
	printf ("StmPT (10 (MPa), ""SIF"" should be 584.149 K °C:  Calculated as %.8f\n", StmPT(10, "SIF"));	
	printf ("StmPT (100 (kPa), ""SIK"" should be 99.61 °C:  Calculated as %.8f\n", StmPT(100, "Sik"));
	printf ("StmPT (100 (bar), ""MET"" should be 310.99 °C:  Calculated as %.8f\n", StmPT(100, "met"));
	printf ("StmPT (100 (ata), ""METF"" should be 309.57 °C:  Calculated as %.8f\n", StmPT(100, "metf"));
	
return 0;
}
