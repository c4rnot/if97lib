
//          Copyright Martin Lord 2014-2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)



 
//    IAPWS-IF97 Region 2: Single phase vapour region backwards equations
/* *********************************************************************
 * *******             VALIDITY                             ************
 * 
 * 273.15 K <= T <=  623.15 K    0 <= p <= Ps(T) (Eq 30 "Saturation Pressure basic Equation")
 * 623.15 K <= T <=  863.15 K    0 <= p <= p(T)  (Eq 5 "B23 Region 2 - 3 boundry equation")
 * 863.15 K <= T <= 1073.15 K    0 <= p <= 100 MPa
 * 
 * These functions also provide reasonable values for the metastable 
 * region above 10 MPa
 * 
 * Note:  for temperatures between 273.15 and 273.16 K, the part of the 
 * range of validity between the pressures on the saturation pressure 
 * line (Eq 30) and on the sublimation line corresponds to metastable 
 * states
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
 * @file IF97_Region2bw.h
 * @author Martin Lord
 * @date 20 Jube 2015
 * @brief IAPWS-IF97 Region 2: Single phase vapour region backwards equations.
 *
 * @details 
  * VALIDITY \n
 *  \n
 * 273.15 K <= T <=  623.15 K    0 <= p <= Ps(T) (Eq 30 "Saturation Pressure basic Equation") \n
 * 623.15 K <= T <=  863.15 K    0 <= p <= p(T)  (Eq 5 "B23 Region 2 - 3 boundry equation") \n
 * 863.15 K <= T <= 1073.15 K    0 <= p <= 100 MPa \n
 * \n
 * Note:  for temperatures between 273.15 and 273.16 K, the part of the 
 * range of validity between the pressures on the saturation pressure 
 * line (Eq 30) and on the sublimation line corresponds to metastable 
 * states \n
 * 
 * For Backwards Equations \n 
 * Region 2a up to 4 MPa \n
 * Region 2b  above 5.85 kJ/kg.K \n
 * Region 2c Below 5.85  kJ/kg.K \n
 
 * @see http://www.iapws.org/relguide/IF97-Rev.html
 */



#ifndef IF97_REGION2BW_H
#define IF97_REGION2BW_H

#include "IF97_common.h"
#include <math.h> 



//**************************************************************
//********* REGION 2 BACKWARDS EQUATIONS *************


/** returns the pressure (MPa) of the 2b-2c boundary for a given 
 * enthalpy (kJ/kg) */
double IF97_B2bc_p (double h_kJperKg);


/** returns the enthalpy (kJ/kg) of the 2b-2c boundary  for a given 
 * pressure (MPa) */
double IF97_B2bc_h (double p_MPa);


/** returns temperature (K) in region 2a for a given pressure (MPa) 
 * and enhalpy (kJ/kg) */
double if97_r2a_t_ph (double p_MPa, double h_kJperkg);


/** returns temperature (K) in region 2b for a given pressure (MPa) 
 * and enhalpy (kJ/kg) */
double if97_r2b_t_ph (double p_MPa, double h_kJperkg);


/** returns temperature (K) in region 2c for a given pressure (MPa) 
 * and enhalpy (kJ/kg) */
double if97_r2c_t_ph (double p_MPa, double h_kJperkg);


/** returns temperature (K) in region 2a for a given pressure (MPa) 
 * and entropy (kJ/kg.K) */
double if97_r2a_t_ps (double p_MPa, double s_kJperkgK );


/** returns temperature (K) in region 2b for a given pressure (MPa) 
 * and entropy (kJ/kg.K) */
double if97_r2b_t_ps (double p_MPa, double s_kJperkgK );


/** returns temperature (K) in region 2c for a given pressure (MPa) 
 * and entropy (kJ/kg.K) */
double if97_r2c_t_ps (double p_MPa, double s_kJperkgK );

#endif // IF97_REGION2BW_H
