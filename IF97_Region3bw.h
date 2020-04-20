//          Copyright Martin Lord 2014-2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)



 
//    IAPWS-IF97 Region 3 Backwards Equations: low temperature supercritical region 
/* *********************************************************************
 * *******             VALIDITY                             ************
 * 623.15 K <=T <= T ( p ) [B23 temperature equation]
 * p ( T ) [B23 temperature equation] <= p <= 100 MPa .
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
  * ***************************************************************** */
  
/**
 * @copyright
 * Copyright Martin Lord 2014-2015. \n
 * Distributed under the Boost Software License, Version 1.0. \n
 * (See accompanying file LICENSE_1_0.txt or copy at \n
 * http://www.boost.org/LICENSE_1_0.txt) \n
 * 
 * @file IF97_Region3bw.h
 * @author Martin Lord
 * @date 20 June 2015
 * @brief IAPWS-IF97 Region 3: low temperature supercritical backwards equations
 * @details 
  * VALIDITY \n
 * 623.15 K <=T <= T ( p ) [B23 temperature equation] \n
 * p ( T ) [B23 temperature equation] <= p <= 100 MPa.\n
 * \n
 * 
 * @see http://www.iapws.org/relguide/Supp-VPT3-2014.pdf
 */



#ifndef IF97_REGION3BW_H
#define IF97_REGION3BW_H

#include "IF97_common.h"
#include "IF97_Region4.h"  // saturation line used in backwards calculations
#include "IF97_B23.h"   // not strictly required but used in v(p,t) subregion selector as an error detector
#include <math.h> 



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
 * criteria of 0.001% on enthalpy and entropy specific volume and 0.01% on Cp are 
 * not met with the backwards equations. They nevertheless provide a close approximation for interation
 * See figure 2   
 */
bool isNearCritical(double p_MPa, double t_K);


/**  Region 3 specific volume (m3/kg) using backwards equations.  These meet the
*    criteria of 0.001% on enthalpy and entropy specific volume and 0.01% on Cp
*    outside the near critical region, which can be checked with "isNearCritical"
*    in the near critical region the backwards equations should provide a good starting 
*    guess for iteration 
*/
double if97_R3bw_v_pt (double p_MPa, double t_K);


//******  remaining functions exposed only for unit testing **************

// Region 3 v(p, t) subregion boundary equations 

/**  Region 3a/3b boundary. see equation 2.  critical isentrope from 25MPa to 100 MPa */
double if97_r3ab_p_t (double p_MPa);

/** Region 3c/3d boundary.  See equation 1. valid: 25 - 40 MPa */
double if97_r3cd_p_t (double p_MPa);

/** Region 3e/3f boundary.  see equation 3.  valid 22.5 - 40 MPa */
double if97_r3ef_p_t (double p_MPa);

/** Region 3g/3h boundary.  See equation 1. Valid 22.5 - 25 MPa */
double if97_r3gh_p_t (double p_MPa);

/** Region 3i/3j boundary. See equation 1.  valid 22.5 - 25 MPa  ~v= 0.0041 m3/kg */
double if97_r3ij_p_t (double p_MPa);

/** Region 3j/3k boundary. See equation 1. Valid 20.5 - 25 MPa.  ~ v = v"(20.5 MPa) */
double if97_r3jk_p_t (double p_MPa);

/** Region 3m/3n boundary. See equation 1. valid: 22.5 - 23 MPa. ~v=0.0028 m3/kg */
double if97_r3mn_p_t (double p_MPa);

/** Region 3o/3p boundary. see equation 2. valid: 22.5 - 23 MPa. ~v=0.0034 m3/kg */
double if97_r3op_p_t (double p_MPa);


/** Region 3q/3u boundary. See equation 1. valid: Psat(643.15 K) - 22.5 MPa */
double if97_r3qu_p_t (double p_MPa);

/** Region 3r/3x boundary. See equation 1.  valid: Psat(643.15 K) - 22.5 MPa */
double if97_r3rx_p_t (double p_MPa);


/**  Auxiliary Region 3u/3v boundary. see equation 2.  Valid: Pressure from 21.9316 MPa to 22.5 MPa  */
double if97_r3uv_p_t (double p_MPa);

/** Auxiliary Region 3w/3x boundary.  See equation 2. Valid: Pressure from 21.9010 MPa to 22.5 MPa  */
double if97_r3wx_p_t (double p_MPa);


/** Determines subregion for calculation of v (p, t) in region 3
 * Results: 1=3a, 2=3b, 3=3c etc...   100 = near critical region   0 = error: not region 3
 *  this function assumes you are already know you are in region 3
 * see table 2
 */
char if97_r3_pt_subregion(double p_MPa, double t_K);


/* ************* Region 3 v(p, t) backwards equations************************************* */

/** specific volume (m3/kg) in region 3a for a given temperature (K) and pressure (MPa) */
double if97_r3a_v_pt (double p_MPa, double t_K);

/** specific volume (m3/kg) in region 3b for a given temperature (K) and pressure (MPa) */
double if97_r3b_v_pt (double p_MPa, double t_K);

/** specific volume (m3/kg) in region 3c for a given temperature (K) and pressure (MPa) */
double if97_r3c_v_pt (double p_MPa, double t_K);

/** specific volume (m3/kg) in region 3d for a given temperature (K) and pressure (MPa) */
double if97_r3d_v_pt (double p_MPa, double t_K);

/** specific volume (m3/kg) in region 3e for a given temperature (K) and pressure (MPa) */
double if97_r3e_v_pt (double p_MPa, double t_K);

/** specific volume (m3/kg) in region 3f for a given temperature (K) and pressure (MPa) */
double if97_r3f_v_pt (double p_MPa, double t_K);

/** specific volume (m3/kg) in region 3g for a given temperature (K) and pressure (MPa) */
double if97_r3g_v_pt (double p_MPa, double t_K);

/** specific volume (m3/kg) in region 3h for a given temperature (K) and pressure (MPa) */
double if97_r3h_v_pt (double p_MPa, double t_K);

/** specific volume (m3/kg) in region 3i for a given temperature (K) and pressure (MPa) */
double if97_r3i_v_pt (double p_MPa, double t_K);

/** specific volume (m3/kg) in region 3j for a given temperature (K) and pressure (MPa) */
double if97_r3j_v_pt (double p_MPa, double t_K);

/** specific volume (m3/kg) in region 3k for a given temperature (K) and pressure (MPa) */
double if97_r3k_v_pt (double p_MPa, double t_K);

/** specific volume (m3/kg) in region 3l for a given temperature (K) and pressure (MPa) */
double if97_r3l_v_pt (double p_MPa, double t_K);

/** specific volume (m3/kg) in region 3m for a given temperature (K) and pressure (MPa) */
double if97_r3m_v_pt (double p_MPa, double t_K);

/** specific volume (m3/kg) in region 3n for a given temperature (K) and pressure (MPa) */
double if97_r3n_v_pt (double p_MPa, double t_K);

/** specific volume (m3/kg) in region 3o for a given temperature (K) and pressure (MPa) */
double if97_r3o_v_pt (double p_MPa, double t_K);

/** specific volume (m3/kg) in region 3p for a given temperature (K) and pressure (MPa) */
double if97_r3p_v_pt (double p_MPa, double t_K);

/** specific volume (m3/kg) in region 3q for a given temperature (K) and pressure (MPa) */
double if97_r3q_v_pt (double p_MPa, double t_K);

/** specific volume (m3/kg) in region 3r for a given temperature (K) and pressure (MPa) */
double if97_r3r_v_pt (double p_MPa, double t_K);

/** specific volume (m3/kg) in region 3s for a given temperature (K) and pressure (MPa) */
double if97_r3s_v_pt (double p_MPa, double t_K);

/** specific volume (m3/kg) in region 3t for a given temperature (K) and pressure (MPa) */
double if97_r3t_v_pt (double p_MPa, double t_K);


/* ************* Region 3 v(p, t) auxiliary equations********************************** */


/** specific volume (m3/kg) in region 3u for a given temperature (K) and pressure (MPa) */
double if97_r3u_v_pt (double p_MPa, double t_K);

/** specific volume (m3/kg) in region 3v for a given temperature (K) and pressure (MPa) */
double if97_r3v_v_pt (double p_MPa, double t_K);

/** specific volume (m3/kg) in region 3w for a given temperature (K) and pressure (MPa) */
double if97_r3w_v_pt (double p_MPa, double t_K);

/** specific volume (m3/kg) in region 3x for a given temperature (K) and pressure (MPa) */
double if97_r3x_v_pt (double p_MPa, double t_K);

/** specific volume (m3/kg) in region 3y for a given temperature (K) and pressure (MPa) */
double if97_r3y_v_pt (double p_MPa, double t_K);

/** specific volume (m3/kg) in region 3z for a given temperature (K) and pressure (MPa) */
double if97_r3z_v_pt (double p_MPa, double t_K);



#endif // IF97_REGION3BW_H
