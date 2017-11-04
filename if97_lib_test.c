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

// check threading using OpenMP test
	printf ("\n\n************************************************************ \n" );
	printf ("CHECK FOR MULTITHREADDING \n\n" );
#pragma omp parallel
	{
	printf ("thread\n");
	}


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
	
	printf (" *** StmPT\n\n" );
	printf ("StmPT (100 [bar], ""SI" " ) should be 310.99 °C:  Calculated as %.8f\n", StmPT(100, "SI"));
	printf ("StmPT (100 [bar], ""1" " ) should be 310.99 °C:  Calculated as %.8f\n", StmPT(100, "1"));
	printf ("StmPT (100 [psi], ""eng" " ) should be 327.82 °F:  Calculated as %.8f\n", StmPT(100, "enG"));
	printf ("StmPT (100 (psig), ""engg" " ) should be 337.88 °F:  Calculated as %.8f\n", StmPT(100, "engg"));
	printf ("StmPT (100 (psig), ""enggo" " ) should fail:  Calculated as %.8f\n", StmPT(100, "enggo"));	
	printf ("StmPT (10 (MPa), ""SIF" " ) should be 584.149 K °C:  Calculated as %.8f\n", StmPT(10, "SIF"));	
	printf ("StmPT (100 (kPa), ""SIK" " ) should be 99.61 °C:  Calculated as %.8f\n", StmPT(100, "Sik"));
	printf ("StmPT (100 (bar), ""MET" " ) should be 310.99 °C:  Calculated as %.8f\n", StmPT(100, "met"));
	printf ("StmPT (100 (ata), ""METF" " ) should be 309.57 °C:  Calculated as %.8f\n", StmPT(100, "metf"));
	
	
	printf (" *** StmTP\n\n" );
	printf ("StmTP (100 [°C], ""SI" " ) should be 1.01417978 bar:  Calculated as %.8f\n", StmTP(100, "SI"));
	printf ("StmTP (100 [°C], ""1" " ) should be 1.01417978 bar  Calculated as %.8f\n", StmTP(100, "1"));
	printf ("StmTP (100 [°F], ""eng" " ) should be 0.95043933:  Calculated as %.8f\n", StmTP(100, "enG"));
	printf ("StmTP (100 (°F), ""engg" " ) should be -13.74550945 psig:  Calculated as %.8f\n", StmTP(100, "engg"));  //ERROR 
	printf ("StmTP (100 (°F), ""enggo" " ) should fail:  Calculated as %.8f\n", StmTP(100, "enggo"));	
	printf ("StmTP (300 (°K), ""SIF" " ) should be 0.00353659 MPa:  Calculated as %.8f\n", StmTP(300, "SIF"));	
	printf ("StmTP (100 (°C), ""SIK" " ) should be 101.417978 kPa:  Calculated as %.8f\n", StmTP(100, "Sik"));
	printf ("StmTP (100 (°C), ""MET" " ) should be 1.01417978 bar:  Calculated as %.8f\n", StmTP(100, "met"));
	printf ("StmTP (100 (°C), ""METF" " ) should be 1.03417556 ata:  Calculated as %.8f\n", StmTP(100, "metf"));
	
	printf (" *** StmPTH\n\n" );
	printf ("StmPTH (100 [bar],  [500 [°C], ""SI" " ) should be 3375.05844 kJ/kg:  Calculated as %.8f\n", StmPTH(100, 500, "SI"));
	printf ("StmPTH (100 [bar], 500 [°C], ""1" " ) should be 3375.05844 kJ/kg:  Calculated as %.8f\n", StmPTH(100, 500, "1")); 
	printf ("StmPTH (100 [psia], 500 [°F], ""eng" " ) should be 1279.32095 BTU/lb:  Calculated as %.8f\n", StmPTH(100, 500, "enG"));
	printf ("StmPTH (100 [psig], 500 (°F), ""engg" " ) should be 1277.89101 BTU/lb:  Calculated as %.8f\n", StmPTH(100, 500, "engg"));  //ERROR 
	printf ("StmPTH (100 [psig] 500 (°F), ""enggo" " ) should fail:  Calculated as %.8f\n", StmPTH(100, 500, "enggo"));	
	printf ("StmPTH (100 [MPa], 500 (°K), ""SIF" " ) should be 1015.39368 kJ/kg:  Calculated as %.8f\n", StmPTH(100, 500, "SIF"));	
	printf ("StmPTH (100 [kPa], 500 (°C), ""SIK" " ) should be 3488.70857 kJ/kg:  Calculated as %.8f\n", StmPTH(100, 500, "Sik"));
	printf ("StmPTH (100 [bar], 500 (°C), ""MET" " ) should be 806.118860 kCal/kg:  Calculated as %.8f\n", StmPTH(100, 500, "met"));
	printf ("StmPTH (100 [ata], 500 (°C), ""METF" " ) should be 806.688261 kCal/kg:  Calculated as %.8f\n", StmPTH(100, 500, "metf"));	
	
		printf (" *** StmPTS\n\n" );
	printf ("StmPTS (100 [bar],  [500 [°C], ""SI" " ) should be 6.59932253 kJ/kg K:  Calculated as %.8f\n", StmPTS(100, 500, "SI"));
	printf ("StmPTS (100 [bar], 500 [°C], ""1" " ) should be 6.59932253 kJ/kg K:  Calculated as %.8f\n", StmPTS(100, 500, "1")); 
	printf ("StmPTS (100 [psia], 500 [°F], ""eng" " ) should be 1.70889598 BTU/lb R:  Calculated as %.8f\n", StmPTS(100, 500, "enG"));
	printf ("StmPTS (100 [psig], 500 (°F), ""engg" " ) should be 1.69268747 BTU/lb R:  Calculated as %.8f\n", StmPTS(100, 500, "engg"));  //ERROR 
	printf ("StmPTS (100 [psig] 500 (°F), ""enggo" " ) should fail:  Calculated as %.8f\n", StmPTS(100, 500, "enggo"));	
	printf ("StmPTS (100 [MPa], 500 (°K), ""SIF" " ) should be 2.43662387 kJ/kg K:  Calculated as %.8f\n", StmPTS(100, 500, "SIF"));	
	printf ("StmPTS (100 [kPa], 500 (°C), ""SIK" " ) should be 8.83606364 kJ/kg K:  Calculated as %.8f\n", StmPTS(100, 500, "Sik"));
	printf ("StmPTS (100 [bar], 500 (°C), ""MET" " ) should be 1.57622111 kCal/kg C:  Calculated as %.8f\n", StmPTS(100, 500, "met"));
	printf ("StmPTS (100 [ata], 500 (°C), ""METF" " ) should be 1.57893849 kCal/kg C:  Calculated as %.8f\n", StmPTS(100, 500, "metf"));

	/*
		printf (" *** StmPTV\n\n" );
	printf ("StmPTV (100 [bar],  [500 [°C], ""SI" " ) should be 6.59932253 kJ/kg K:  Calculated as %.8f\n", StmPTV(100, 500, "SI"));
	printf ("StmPTV (100 [bar], 500 [°C], ""1" " ) should be 6.59932253 kJ/kg K:  Calculated as %.8f\n", StmPTV(100, 500, "1")); 
	printf ("StmPTV (100 [psia], 500 [°F], ""eng" " ) should be 1.70889598 BTU/lb R:  Calculated as %.8f\n", StmPTV(100, 500, "enG"));
	printf ("StmPTV (100 [psig], 500 (°F), ""engg" " ) should be 1.69268747 BTU/lb R:  Calculated as %.8f\n", StmPTV(100, 500, "engg"));  //ERROR 
	printf ("StmPTV (100 [psig] 500 (°F), ""enggo" " ) should fail:  Calculated as %.8f\n", StmPTV(100, 500, "enggo"));	
	printf ("StmPTV (100 [MPa], 500 (°K), ""SIF" " ) should be 2.43662387 kJ/kg K:  Calculated as %.8f\n", StmPTV(100, 500, "SIF"));	
	printf ("StmPTV (100 [kPa], 500 (°C), ""SIK" " ) should be 8.83606364 kJ/kg K:  Calculated as %.8f\n", StmPTV(100, 500, "Sik"));
	printf ("StmPTV (100 [bar], 500 (°C), ""MET" " ) should be 1.57622111 kCal/kg C:  Calculated as %.8f\n", StmPTV(100, 500, "met"));
	printf ("StmPTV (100 [ata], 500 (°C), ""METF" " ) should be 1.57893849 kCal/kg C:  Calculated as %.8f\n", StmPTV(100, 500, "metf"));

	
	
	
	
	
	
	
	
	*/
	
return 0;
}
