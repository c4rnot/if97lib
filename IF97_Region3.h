//          Copyright Martin Lord 2014-2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)



 
//    IAPWS-IF97 Region 3: low temperature supercritical and metastable region equations
/* *********************************************************************
 * *******             VALIDITY                             ************
 * 623.15 K <=T <= T ( p ) [B23 temperature equation]
 * p ( T ) [B23 temperature equation] <= p <= 100 MPa .
 * 
 * In addition, region 3 yields reasonable values inthe metastable regions
 * (superheated liquid and subcooleed steam close to the saturated liquid
 * and saturated vapor line
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
  * ***************************************************************** */
  
/**
 * @copyright
 * Copyright Martin Lord 2014-2015. \n
 * Distributed under the Boost Software License, Version 1.0. \n
 * (See accompanying file LICENSE_1_0.txt or copy at \n
 * http://www.boost.org/LICENSE_1_0.txt) \n
 * 
 * @file IF97_Region3.h
 * @author Martin Lord
 * @date 25 May 2015
 * @brief IAPWS-IF97 Region 3: low temperature supercritical and metastable region equations
 * @details 
  * VALIDITY \n
 * 623.15 K <=T <= T ( p ) [B23 temperature equation] \n
 * p ( T ) [B23 temperature equation] <= p <= 100 MPa.\n
 * \n
 * In addition, region 3 yields reasonable values inthe metastable regions\n
 * (superheated liquid and subcooleed steam close to the saturated liquid\n
 * and saturated vapor line\n

 
 * @see http://www.iapws.org/relguide/IF97-Rev.html
 */



#ifndef IF97_REGION3_H
#define IF97_REGION3_H

#include "IF97_common.h"
#include <math.h> 




//**************************************************************
//********* REGION 3 PROPERTY EQUATIONS (FORWARDS) *************

/** specific Helmholz free energy in region 3 (kJ/kg) for a given density
 *  (kg/m3) and temperature (K)  */
double if97_r3_hhz (double rho_kgPerM3 , double t_Kelvin) ;


/** pressure (MPa) in region 3 for a given density (kg/m3) and temperature (K) */
double if97_r3_p (double rho_kgPerM3 , double t_Kelvin );


/** specific internal energy (kJ/kg) in region 3 for a given 
 * density (kg/m3) and temperature (K) */
double if97_r3_u (double rho_kgPerM3 , double t_Kelvin );


/** specific entropy (kJ/kg K) in region 3 for a given 
 * density (kg/m3) and temperature (K) */
double if97_r3_s (double rho_kgPerM3 , double t_Kelvin ) ;


/** specific enthalpy (kJ/kg) in region 3 for a given 
 * density (kg/m3) and temperature (K) */
double if97_r3_h (double rho_kgPerM3 , double t_Kelvin );


/** specific isochoric heat capacity Cv (kJ/kg K) in region 3 for a given 
 * density (kg/m3) and temperature (K) */
double if97_r3_Cv (double rho_kgPerM3 , double t_Kelvin );


/** specific isobaric heat capacity Cp (kJ/kg K) in region 3 for a given 
 * density (kg/m3) and temperature (K) */
double if97_r3_Cp (double rho_kgPerM3 , double t_Kelvin );


/** speed of sound w (m/s) in region 3 for a given 
 * density (kg/m3) and temperature (K) */
double if97_r3_w (double rho_kgPerM3 , double t_Kelvin ) ;


// TODO Phase Equilibrium equations from table 31




/* **********************************************************
********* REGION 3 BACKWARDS EQUATIIONS v(p,t) **************
* 
* Revised Supplementary Release on Backward Equations for Specific Volume 
* as a Function of Pressure and Temperature v(p,T) for Region 3 of the 
* IAPWS Industrial Formulation 1997 for the Thermodynamic Properties
* of Water and Steam.
* 
*  
* http://iapws.org/relguide/Supp-VPT3-2014.pdf
*/

/** checks if a combination of Pressure (MPa) and Temperature (K) is in 
 * the near critical region where the IAPWS-IF97 numerical consistency 
 * criteria of 0.001% on enthalpy and entropy and 0.1% on Cp and Vs are 
 * not met.  See figure 2   
 */
bool isNearCritical(double p_MPa, double t_K);


#endif // IF97_REGION3_H
