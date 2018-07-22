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
	 
	printf ("\n\n *** Testing Winsteam Compatibility  ****** " );
	
	printf ("\n\n *** StmPT\n" );
	printf ("StmPT (100 [bar], ""SI" " ) should be 310.99 °C:  Calculated as %.8f\n", StmPT(100, "SI"));  //OK
	printf ("StmPT (100 [bar], ""1" " ) should be 310.99 °C:  Calculated as %.8f\n", StmPT(100, "1"));  //OK
	printf ("StmPT (100 [psi], ""eng" " ) should be 327.82 °F:  Calculated as %.8f\n", StmPT(100, "enG"));  //OK
	printf ("StmPT (100 (psig), ""engg" " ) should be 337.88 °F:  Calculated as %.8f\n", StmPT(100, "engg"));  //OK
	printf ("StmPT (100 (psig), ""enggo" " ) should fail:  Calculated as %.8f\n", StmPT(100, "enggo"));	//NO ERROR CODE 
	printf ("StmPT (10 (MPa), ""SIF" " ) should be 584.149 K °C:  Calculated as %.8f\n", StmPT(10, "SIF"));	  //OK
	printf ("StmPT (100 (kPa), ""SIK" " ) should be 99.61 °C:  Calculated as %.8f\n", StmPT(100, "Sik"));  //OK
	printf ("StmPT (100 (bar), ""MET" " ) should be 310.99 °C:  Calculated as %.8f\n", StmPT(100, "met"));  //OK
	printf ("StmPT (100 (ata), ""METF" " ) should be 309.57 °C:  Calculated as %.8f\n", StmPT(100, "metf"));  //OK
	
	
	printf ("\n\n *** StmTP\n" );
	printf ("StmTP (100 [°C], ""SI" " ) should be 1.01417978 bar:  Calculated as %.8f\n", StmTP(100, "SI"));  //OK
	printf ("StmTP (100 [°C], ""1" " ) should be 1.01417978 bar  Calculated as %.8f\n", StmTP(100, "1"));  //OK
	printf ("StmTP (100 [°F], ""eng" " ) should be 0.95043933:  Calculated as %.8f\n", StmTP(100, "enG"));  //OK
	printf ("StmTP (100 (°F), ""engg" " ) should be -13.74550945 psig:  Calculated as %.8f\n", StmTP(100, "engg"));  //ERROR ?
	printf ("StmTP (100 (°F), ""enggo" " ) should fail:  Calculated as %.8f\n", StmTP(100, "enggo"));	//NO ERROR CODE 
	printf ("StmTP (300 (°K), ""SIF" " ) should be 0.00353659 MPa:  Calculated as %.8f\n", StmTP(300, "SIF"));	  //OK
	printf ("StmTP (100 (°C), ""SIK" " ) should be 101.417978 kPa:  Calculated as %.8f\n", StmTP(100, "Sik"));  //OK
	printf ("StmTP (100 (°C), ""MET" " ) should be 1.01417978 bar:  Calculated as %.8f\n", StmTP(100, "met"));  //OK
	printf ("StmTP (100 (°C), ""METF" " ) should be 1.03417556 ata:  Calculated as %.8f\n", StmTP(100, "metf"));  //OK
	
	printf ("\n\n *** StmPTH\n" );
	printf ("StmPTH (100 [bar],  [500 [°C], ""SI" " ) should be 3375.05844 kJ/kg:  Calculated as %.8f\n", StmPTH(100, 500, "SI"));  //OK
	printf ("StmPTH (100 [bar], 500 [°C], ""1" " ) should be 3375.05844 kJ/kg:  Calculated as %.8f\n", StmPTH(100, 500, "1"));   //OK
	printf ("StmPTH (100 [psia], 500 [°F], ""eng" " ) should be 1279.32095 BTU/lb:  Calculated as %.8f\n", StmPTH(100, 500, "enG"));  //OK
	printf ("StmPTH (100 [psig], 500 (°F), ""engg" " ) should be 1277.89101 BTU/lb:  Calculated as %.8f\n", StmPTH(100, 500, "engg"));  //ERROR ??
	printf ("StmPTH (100 [psig] 500 (°F), ""enggo" " ) should fail:  Calculated as %.8f\n", StmPTH(100, 500, "enggo"));	 //NO ERROR CODE 
	printf ("StmPTH (100 [MPa], 500 (°K), ""SIF" " ) should be 1015.39368 kJ/kg:  Calculated as %.8f\n", StmPTH(100, 500, "SIF"));	  //OK
	printf ("StmPTH (100 [kPa], 500 (°C), ""SIK" " ) should be 3488.70857 kJ/kg:  Calculated as %.8f\n", StmPTH(100, 500, "Sik"));  //OK
	printf ("StmPTH (100 [bar], 500 (°C), ""MET" " ) should be 806.118860 kCal/kg:  Calculated as %.8f\n", StmPTH(100, 500, "met"));  //OK
	printf ("StmPTH (100 [ata], 500 (°C), ""METF" " ) should be 806.688261 kCal/kg:  Calculated as %.8f\n", StmPTH(100, 500, "metf"));	  //OK
	
	printf ("\n\n *** StmPTS\n" );
	printf ("StmPTS (100 [bar],  [500 [°C], ""SI" " ) should be 6.59932253 kJ/kg K:  Calculated as %.8f\n", StmPTS(100, 500, "SI"));  //OK
	printf ("StmPTS (100 [bar], 500 [°C], ""1" " ) should be 6.59932253 kJ/kg K:  Calculated as %.8f\n", StmPTS(100, 500, "1"));   //OK
	printf ("StmPTS (100 [psia], 500 [°F], ""eng" " ) should be 1.70889598 BTU/lb R:  Calculated as %.8f\n", StmPTS(100, 500, "enG"));   //OK
	printf ("StmPTS (100 [psig], 500 (°F), ""engg" " ) should be 1.69268747 BTU/lb R:  Calculated as %.8f\n", StmPTS(100, 500, "engg"));  //ERROR ??
	printf ("StmPTS (100 [psig] 500 (°F), ""enggo" " ) should fail:  Calculated as %.8f\n", StmPTS(100, 500, "enggo"));		 //NO ERROR CODE 
	printf ("StmPTS (100 [MPa], 500 (°K), ""SIF" " ) should be 2.43662387 kJ/kg K:  Calculated as %.8f\n", StmPTS(100, 500, "SIF"));  //OK
	printf ("StmPTS (100 [kPa], 500 (°C), ""SIK" " ) should be 8.83606364 kJ/kg K:  Calculated as %.8f\n", StmPTS(100, 500, "Sik"));  //OK
	printf ("StmPTS (100 [bar], 500 (°C), ""MET" " ) should be 1.57622111 kCal/kg C:  Calculated as %.8f\n", StmPTS(100, 500, "met"));  //OK
	printf ("StmPTS (100 [ata], 500 (°C), ""METF" " ) should be 1.57893849 kCal/kg C:  Calculated as %.8f\n", StmPTS(100, 500, "metf"));  //OK

	
	printf ("\n\n *** StmPTV\n" );
	printf ("StmPTV (100 [bar],  [500 [°C], ""SI" " ) should be 0.0328128630 m3/kg:  Calculated as %.8f\n", StmPTV(100, 500, "SI")); //OK
	printf ("StmPTV (100 [bar], 500 [°C], ""1" " ) should be 0.0328128630 m3/kg:  Calculated as %.8f\n", StmPTV(100, 500, "1")); //OK
	printf ("StmPTV (100 [psia], 500 [°F], ""eng" " ) should be 5.58753886 ft3/lbm:  Calculated as %.8f\n", StmPTV(100, 500, "enG"));  // ERROR??
	printf ("StmPTV (100 [psig], 500 (°F), ""engg" " ) should be XXX:  Calculated as %.8f\n", StmPTV(100, 500, "engg"));  //ERROR 
	printf ("StmPTV (100 [psig] 500 (°F), ""enggo" " ) should fail:  Calculated as %.8f\n", StmPTV(100, 500, "enggo"));	//NO ERROR CODE 
	printf ("StmPTV (100 [MPa], 500 (°K), ""SIF" " ) should be 0.00111209398 m3/kg:  Calculated as %.8f\n", StmPTV(100, 500, "SIF"));	 //OK
	printf ("StmPTV (100 [kPa], 500 (°C), ""SIK" " ) should be 3.56558322 m3/kg:  Calculated as %.8f\n", StmPTV(100, 500, "Sik"));   //OK
	printf ("StmPTV (100 [bar], 500 (°C), ""MET" " ) should be 0.0328128630 m3/kg:  Calculated as %.8f\n", StmPTV(100, 500, "met"));
	printf ("StmPTV (100 [ata], 500 (°C), ""METF" " ) should be 0.0335198514 m3/kg:  Calculated as %.8f\n", StmPTV(100, 500, "metf"));  //OK

	printf ("\n\n *** StmPTC\n" );
	printf ("StmPTC (100 [bar],  [500 [°C], ""SI" " ) should be 2.58333874 kJ/kg/C:  Calculated as %.8f\n", StmPTC(100, 500, "SI"));  //OK
	printf ("StmPTC (100 [bar], 500 [°C], ""1" " ) should be 2.58333874 kJ/kg/C:  Calculated as %.8f\n", StmPTC(100, 500, "1"));  //OK
	printf ("StmPTC (100 [psia], 500 [°F], ""eng" " ) should be XXX btu/lbm/F:  Calculated as %.8f\n", StmPTC(100, 500, "enG")); 
	printf ("StmPTC (100 [psig], 500 (°F), ""engg" " ) should be XXX btu/lbm/F:  Calculated as %.8f\n", StmPTC(100, 500, "engg"));
	printf ("StmPTC (100 [psig] 500 (°F), ""enggo" " ) should fail:  Calculated as %.8f\n", StmPTC(100, 500, "enggo"));	//NO ERROR CODE 
	printf ("StmPTC (100 [MPa], 500 (°K), ""SIF" " ) should be XXX kJ/kg/K:  Calculated as %.8f\n", StmPTC(100, 500, "SIF"));
	printf ("StmPTC (100 [kPa], 500 (°C), ""SIK" " ) should be XXX kJ/kg/C:  Calculated as %.8f\n", StmPTC(100, 500, "Sik"));
	printf ("StmPTC (100 [bar], 500 (°C), ""MET" " ) should be XXX kJ/kg/C:  Calculated as %.8f\n", StmPTC(100, 500, "met"));
	printf ("StmPTC (100 [ata], 500 (°C), ""METF" " ) should be XXX kJ/kg/C:  Calculated as %.8f\n", StmPTC(100, 500, "metf"));
	
	printf ("\n\n *** StmPTK\n" );																							//ERROR 
	printf ("StmPTK (100 [bar],  [500 [°C], ""SI" " ) should be 0.0759223503 W/m/C:  Calculated as %.8f\n", StmPTK(100, 500, "SI"));
	printf ("StmPTK (100 [bar], 500 [°C], ""1" " ) should be 0.0759223503 W/m/C:  Calculated as %.8f\n", StmPTK(100, 500, "1"));
	printf ("StmPTK (100 [psia], 500 [°F], ""eng" " ) should be XXX btu/h/ft/F:  Calculated as %.8f\n", StmPTK(100, 500, "enG")); 
	printf ("StmPTK (100 [psig], 500 (°F), ""engg" " ) should be XXX btu/h/ft/F:  Calculated as %.8f\n", StmPTK(100, 500, "engg"));
	printf ("StmPTK (100 [psig] 500 (°F), ""enggo" " ) should fail:  Calculated as %.8f\n", StmPTK(100, 500, "enggo"));	//NO ERROR CODE 
	printf ("StmPTK (100 [MPa], 500 (°K), ""SIF" " ) should be XXX W/m/K:  Calculated as %.8f\n", StmPTK(100, 500, "SIF"));
	printf ("StmPTK (100 [kPa], 500 (°C), ""SIK" " ) should be XXX W/m/C:  Calculated as %.8f\n", StmPTK(100, 500, "Sik"));
	printf ("StmPTK (100 [bar], 500 (°C), ""MET" " ) should be 0.0759223503 W/m/C:  Calculated as %.8f\n", StmPTK(100, 500, "met"));
	printf ("StmPTK (100 [ata], 500 (°C), ""METF" " ) should be XXX kCal/m/h/C:  Calculated as %.8f\n", StmPTK(100, 500, "metf"));

	
	printf ("\n\n *** StmPTM\n" );																							//ERROR 
	printf ("StmPTM (100 [bar],  [500 [°C], ""SI" " ) should be XXX Centipoise:  Calculated as %.8f\n", StmPTM(100, 500, "SI"));
	printf ("StmPTM (100 [bar], 500 [°C], ""1" " ) should be XXX m/s:  Calculated as %.8f\n", StmPTM(100, 500, "1"));
	printf ("StmPTM (100 [psia], 500 [°F], ""eng" " ) should be XXX lbm/ft/h:  Calculated as %.8f\n", StmPTM(100, 500, "enG")); 
	printf ("StmPTM (100 [psig], 500 (°F), ""engg" " ) should be XXX lbm/ft/h:  Calculated as %.8f\n", StmPTM(100, 500, "engg"));
	printf ("StmPTM (100 [psig] 500 (°F), ""enggo" " ) should fail:  Calculated as %.8f\n", StmPTM(100, 500, "enggo"));	//NO ERROR CODE 
	printf ("StmPTM (100 [MPa], 500 (°K), ""SIF" " ) should be XXX Pa.s:  Calculated as %.8f\n", StmPTM(100, 500, "SIF"));
	printf ("StmPTM (100 [kPa], 500 (°C), ""SIK" " ) should be XXX Centipoise:  Calculated as %.8f\n", StmPTM(100, 500, "Sik"));
	printf ("StmPTM (100 [bar], 500 (°C), ""MET" " ) should be XXX Centipoise:  Calculated as %.8f\n", StmPTM(100, 500, "met"));
	printf ("StmPTM (100 [ata], 500 (°C), ""METF" " ) should be XXX Centipoise:  Calculated as %.8f\n", StmPTM(100, 500, "metf"));
	
	printf ("\n\n *** StmPTW\n" );
	printf ("StmPTW (100 [bar],  [500 [°C], ""SI" " ) should be 647.885589 m/s:  Calculated as %.8f\n", StmPTW(100, 500, "SI"));  //OK
	printf ("StmPTW (100 [bar], 500 [°C], ""1" " ) should be 647.885589 m/s:  Calculated as %.8f\n", StmPTW(100, 500, "1"));  //OK
	printf ("StmPTW (100 [psia], 500 [°F], ""eng" " ) should be XXX ft/s:  Calculated as %.8f\n", StmPTW(100, 500, "enG")); 
	printf ("StmPTW (100 [psig], 500 (°F), ""engg" " ) should be XXX ft/s:  Calculated as %.8f\n", StmPTW(100, 500, "engg"));
	printf ("StmPTW (100 [psig] 500 (°F), ""enggo" " ) should fail:  Calculated as %.8f\n", StmPTW(100, 500, "enggo"));	//NO ERROR CODE 
	printf ("StmPTW (100 [MPa], 500 (°K), ""SIF" " ) should be XXX m/s:  Calculated as %.8f\n", StmPTW(100, 500, "SIF"));
	printf ("StmPTW (100 [kPa], 500 (°C), ""SIK" " ) should be XXX m/s:  Calculated as %.8f\n", StmPTW(100, 500, "Sik"));
	printf ("StmPTW (100 [bar], 500 (°C), ""MET" " ) should be 647.885589 m/s:  Calculated as %.8f\n", StmPTW(100, 500, "met"));  //OK
	printf ("StmPTW (100 [ata], 500 (°C), ""METF" " ) should be XXX m/s:  Calculated as %.8f\n", StmPTW(100, 500, "metf"));	
	
	printf ("\n\n *** StmPTG\n" );
	printf ("StmPTG (100 [bar],  [500 [°C], ""SI" " ) should be XXX :  Calculated as %.8f\n", StmPTG(100, 500, "SI"));
	printf ("StmPTG (100 [bar], 500 [°C], ""1" " ) should be XXX :  Calculated as %.8f\n", StmPTG(100, 500, "1"));
	printf ("StmPTG (100 [psia], 500 [°F], ""eng" " ) should be XXX :  Calculated as %.8f\n", StmPTG(100, 500, "enG")); 
	printf ("StmPTG (100 [psig], 500 (°F), ""engg" " ) should be XXX :  Calculated as %.8f\n", StmPTG(100, 500, "engg"));
	printf ("StmPTG (100 [psig] 500 (°F), ""enggo" " ) should fail:  Calculated as %.8f\n", StmPTG(100, 500, "enggo"));	//NO ERROR CODE 
	printf ("StmPTG (100 [MPa], 500 (°K), ""SIF" " ) should be XXX :  Calculated as %.8f\n", StmPTG(100, 500, "SIF"));
	printf ("StmPTG (100 [kPa], 500 (°C), ""SIK" " ) should be XXX :  Calculated as %.8f\n", StmPTG(100, 500, "Sik"));
	printf ("StmPTG (100 [bar], 500 (°C), ""MET" " ) should be XXX :  Calculated as %.8f\n", StmPTG(100, 500, "met"));
	printf ("StmPTG (100 [ata], 500 (°C), ""METF" " ) should be XXX :  Calculated as %.8f\n", StmPTG(100, 500, "metf"));	
	
	
return 0;
}
