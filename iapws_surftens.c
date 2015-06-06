//          Copyright Martin Lord 2014-2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


/*    Surface tension according to 
 * IAPWS Revised Release on Surface Tension of Ordinary Water Substance 
 * Moscow June 2014  */

/* *********************************************************************
 * *******             VALIDITY                             ************
 *  From 0.01°C (273.16 °K) to critical Temperature (647.096 °K).
 * It also gives reasonably accurate values when extrapolated into the 
 * supercooled region to temperatures as low as -25 °C\n
 * 
 * see http://www.iapws.org/relguide/Surf-H2O.html
 * ****************************************************************** */
 
 /* ********************************************************************
  *         COMPILE AND LINK INSTRUCTIONS    (gcc)                      
  * 
  * This library uses math.h, so must have the -lm  link flag
  * 
  * The library is programmed to be able to use OpenMP multithreading   
  * use the -fopenmp complie flag to enable multithreadded code
  * 
  * ****************************************************************** */
   


#include <math.h> // for pow
#include "iapws_surftens.h"

// surface tension in mN/m for a given temperature °C according to ITS-90
double iapws_surftens(double t_Celcius){

	const double mu = 1.256 ;
	const double b  = -0.625 ;
	const double B = 235.8 ; // mN/m
	const double Tc = 647.096 ;  // °K
	const double T_K_ICE = 273.15 ; // °K

	double tau = 1.0 - (t_Celcius + T_K_ICE) /Tc ;

return B * pow(tau, mu)*(1+b*tau);
}



