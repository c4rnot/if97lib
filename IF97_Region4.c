//          Copyright Martin Lord 2014-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


//    IAPWS-IF97 Region 4: Saturation line equations
/* *********************************************************************
 * *******             VALIDITY                             ************
 * Region 4 (Saturation line) is valid along the entire vapor-liquid 
 * saturation line from the triple-point temperature T t to the critical
 * temperature Tc and can be simply extrapolated to 273.15 K so that it
 * covers the temperature range
 * 
 * 273.15 K 	<= T <= 647.096 K 
 * 611.213 Pa 	<= p <= 22.064 MPa .
 * 
 * 
 * ****************************************************************** */
 
 /* ********************************************************************
  *         COMPILE AND LINK INSTRUCTIONS    (gcc)                          *
  * 
  * This library uses math.h, so must have the -lm  link flag
  * 
  * The library is programmed to be able to use OpenMP multithreading   
  * use the -fopenmp complie flag to enable multithreadded code
  * 
  * ****************************************************************** */
   

#include "IF97_common.h"  //PSTAR TSTAR & sqr
#include "IF97_Region4.h"
#include <math.h> // for pow, log
/* #ifdef _OPENMP // multithreading via libgomp
 # include <omp.h>
 #endif
*/
#include <stdio.h>  // used for debugging only


//***************************************************************
//****** REGION 4 SATURATION LINE *******************************


const double IF97_R3_n[] = {
	0  //n[0] is not used
	,0.11670521452767E4  // 1
	,-0.72421316703206E6
	,-0.17073846940092E2
	,0.12020824702470E5
	,-0.32325550322333E7
	,0.14915108613530E2
	,-0.48232657361591E4
	,0.40511340542057E6
	,-0.23855557567849
	,0.65017534844798E3  // 10
};


/* Returns the saturation pressure (MPa) for a given temperature (Kelvin) */
// Checked OK
double if97_r4_ps ( double ts_Kelvin) {
	
	// Eq 29b: simplified because T* = 1 K
	double I = ts_Kelvin + IF97_R3_n[9] / (ts_Kelvin - IF97_R3_n[10]) ;	
	
	// Eq 30	
	double A = sqr(I) + IF97_R3_n[1] *I + IF97_R3_n[2] ;
	
	double B = IF97_R3_n[3] * sqr(I) +  IF97_R3_n[4] * I +  IF97_R3_n[5] ;
	
	
	double C = IF97_R3_n[6] * sqr(I) +  IF97_R3_n[7] * I +  IF97_R3_n[8] ;
	
return sqr(sqr(2 * C / (-B + sqrt( sqr(B) -4 * A * C)) ));
}
	
	
	
/* Returns the saturation temperature (Kelvin) for a given pressure (MPa) */
// Checked OK
double if97_r4_ts (double ps_MPa ) {	
	
	// Eq 29a: simplified because p* = 1 MPa 
	double B = pow(ps_MPa, 0.25) ;
	
	double E = sqr(B) +  IF97_R3_n[3] * B +  IF97_R3_n[6] ;
	
	double F = IF97_R3_n[1] * sqr(B) +  IF97_R3_n[4] * B +  IF97_R3_n[7] ;
	
	double G = IF97_R3_n[2] * sqr(B) +  IF97_R3_n[5] * B +  IF97_R3_n[8] ;
	
	double D = 2 * G / (-F - sqrt(sqr(F) -4 * E * G)) ;
 
// Eq 31: Simplified because T* = 1 K 
return 	0.5 * (IF97_R3_n[10] + D - 
		sqrt(sqr(IF97_R3_n[10] + D) - 4 * ( IF97_R3_n[9] + IF97_R3_n[10] * D)));
}
	
	
	
	
	
	
	
	
	
	
	
