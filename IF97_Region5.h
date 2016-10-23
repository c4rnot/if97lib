
//          Copyright Martin Lord 2014-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)



 
//    IAPWS-IF97 Region 5: High Temperature Region equations
/* *********************************************************************
 * *******             VALIDITY                             ************
 * 
 * Region 5 of IAPWS-IF97 is valid for the following temperature and 
 * pressure range:
 * 1073.15 K <= T <= 2273.15 K
 * 0 < p <= 50 MPa.
 * 
 * It is only valid for pure undissociated water; any dissociation will 
 * have to be taken into account separately.
 * 
 * 
 * ****************************************************************** */
/* ********************************************************************
  *         COMPILE AND LINK INSTRUCTIONS    (gcc)                     *
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
 * @file IF97_Region5.h
 * @author Martin Lord
 * @date 15 Oct 2014
 * @brief IAPWS-IF97 Region 5: High Temperature region.
 *
 * @details 
  * VALIDITY \n
 *  \n
  * Region 5 of IAPWS-IF97 is valid for the following temperature and 
 * pressure range \n
 * 1073.15 K <= T <= 2273.15 K \n
 * 0 < p <= 50 MPa. \n
 * \n
 * It is only valid for pure undissociated water; any dissociation will 
 * have to be taken into account separately. \n
 * \n 
 * @see http://www.iapws.org/relguide/IF97-Rev.html
 */



#ifndef IF97_REGION5_H
#define IF97_REGION5_H

#include "IF97_common.h"
#include <math.h> 




//**************************************************************
//********* REGION 5 PROPERTY EQUATIONS (FORWARDS) *************

	/** specific Gibbs free energy in region 5 (kJ / kg) */
	double if97_r5_g (double p_MPa , double t_Kelvin);

	/** specific volume in region 5  (metres cubed per kilogram) */ 
	double if97_r5_v (double p_MPa , double t_Kelvin );
			
	
	/** specific internal energy in region 5 (KJ / Kg) */
	double if97_r5_u (double p_MPa , double t_Kelvin );
	
	
	/** specific entropy in region 5 (KJ / Kg.K) */
	double if97_r5_s (double p_MPa , double t_Kelvin );
	
	
	/** specific enthalpy in region 5 (KJ / Kg) */
	double if97_r5_h (double p_MPa , double t_Kelvin );
	
	
	/** specific isobaric heat capacity in region 5 (KJ / Kg.K) */
	double if97_r5_Cp (double p_MPa , double t_Kelvin );
	
	
	/** specific isochoric heat capacity in region 5 (KJ / Kg.K) */
	double if97_r5_Cv (double p_MPa , double t_Kelvin );
	
		
	/** speed of sound in region 5 (m/s) */
	double if97_r5_w (double p_MPa , double t_Kelvin );



#endif // IF97_REGION5_H
