
//          Copyright Martin Lord 2014-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


//    IAPWS-IF97 Region 2: metastable vapour region <= 10 MPa equations
/* *********************************************************************
 * *******             VALIDITY                             ************
 * 
 * Valid in the metastable vapour region  from the saturated vapour line 
 * to the 5% equilibrium moisture line (determined from the equilibrium 
 * h' and h'' values calculated at the given pressure) for pressures 
 * from the triple poiint to 10 MPa

 *  611.657 Pa <= p <= 10 MPa
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
 * Copyright Martin Lord 2014-2014. \n
 * Distributed under the Boost Software License, Version 1.0. \n
 * (See accompanying file LICENSE_1_0.txt or copy at \n
 * http://www.boost.org/LICENSE_1_0.txt) \n
 * 
 * @file IF97_Region2_met.h
 * @author Martin Lord
 * @date 29 Sep 2014
 * @brief IAPWS-IF97 Region 2: metastable vapour region <= 10 MPa equations
 *
 * @details 
 * VALIDITY \n
 * \n
 * Valid in the metastable vapour region  from the saturated vapour line 
 * to the 5% equilibrium moisture line (determined from the equilibrium 
 * h' and h'' values calculated at the given pressure) for pressures 
 * from the triple poiint to 10 MPa \n
 * \n
 *  611.657 Pa <= p <= 10 MPa \n
 * \n
 * Note:  for temperatures between 273.15 and 273.16 K, the part of the 
 * range of validity between the pressures on the saturation pressure 
 * line (Eq 30) and on the sublimation line corresponds to metastable 
 * states. \n
 * 
 * @see http://www.iapws.org/relguide/IF97-Rev.html
 */


#ifndef IF97_REGION2_MET_H
#define IF97_REGION2_MET_H

#include "IF97_common.h"
#include <math.h> 




//**************************************************************
//********* REGION 2 PROPERTY EQUATIONS (FORWARDS) *************

	/** specific Gibbs free energy in region 2 (kJ / kg) 
	// broken */
	double if97_r2met_g (double p_MPa , double t_Kelvin);

	/** specific volume in region 2  (metres cubed per kilogram)  */
	double if97_r2met_v (double p_MPa , double t_Kelvin );
			
	
	/** specific internal energy in region 2 (KJ / Kg) */
	double if97_r2met_u (double p_MPa , double t_Kelvin );
	
	
	/** specific entropy in region 2 (KJ / Kg.K) */
	double if97_r2met_s (double p_MPa , double t_Kelvin );
	
	
	/** specific enthalpy in region 2 (KJ / Kg) */
	double if97_r2met_h (double p_MPa , double t_Kelvin );
	
	
	/**specific isobaric heat capacity in region 2 (KJ / Kg.K) */
	double if97_r2met_Cp (double p_MPa , double t_Kelvin );
	
	
	/** specific isochoric heat capacity in region 2 (KJ / Kg.K) */
	double if97_r2met_Cv (double p_MPa , double t_Kelvin );
	
		
	/** speed of sound in region 2 (m/s) */
	double if97_r2met_w (double p_MPa , double t_Kelvin );




#endif // IF97_REGION2_MET_H
