
//          Copyright Martin Lord 2014-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


/*
 *     IAPWS-IF97 Region 1 equations
 * VALIDITY  273.15 K <= T <= 623.15 K     Psat <= p <= 100 MPa
 *  Exception:  Backwards equations not valid in metastable (superheated liquid) region
 */

/**
 * @copyright
 * Copyright Martin Lord 2014-2014. \n
 * Distributed under the Boost Software License, Version 1.0. \n
 * (See accompanying file LICENSE_1_0.txt or copy at \n
 * http://www.boost.org/LICENSE_1_0.txt) \n
 * 
 * @file IF97_Region1.h
 * @author Martin Lord
 * @date 29 Sep 2014
 * @brief IAPWS-IF97 Region 1 equations.
 *
 * @details 
 * VALIDITY  \n
 * 273.15 K <= T <= 623.15 K     Psat <= p <= 100 MPa \n
 * Exception:  Backwards equations not valid in metastable (superheated liquid) region
 * 
 * @see http://www.iapws.org/relguide/IF97-Rev.html
 */


#ifndef IF97_REGION1_H
#define IF97_REGION1_H

#include "IF97_common.h"
#include <math.h> 



//**************************************************************
//********* REGION 1 PROPERTY EQUATIONS (FORWARDS) *************

	
	/** specific Gibbs free energy in region 1 (kJ / kg)  */
	double if97_r1_g (double p_MPa , double t_Kelvin);

	/** specific volume in region 1  (metres cubed per kilogram) */
	double if97_r1_v (double p_MPa , double t_Kelvin );
			
	
	/** specific internal energy in region 1 (KJ / Kg) */
	double if97_r1_u (double p_MPa , double t_Kelvin );
	
	
	/** specific entropy in region 1 (KJ / Kg.K) */
	double if97_r1_s (double p_MPa , double t_Kelvin );
	
	
	/** specific enthalpy in region 1 (KJ / Kg) */
	double if97_r1_h (double p_MPa , double t_Kelvin );
	
	
	/** specific isobaric heat capacity in region 1 (KJ / Kg.K) */
	double if97_r1_Cp (double p_MPa , double t_Kelvin );
	
	
	/** specific isochoric heat capacity in region 1 (KJ / Kg.K) */
	double if97_r1_Cv (double p_MPa , double t_Kelvin );
	
		
	/** speed of sound in region 1 (m/s) */
	double if97_r1_w (double p_MPa , double t_Kelvin );


//***********************************************************
//********* REGION 1 BACKWARDS EQUATIONS*********************

	/** temperature (K) in region 1 for a given pressure and enthalpy */
	double if97_r1_t_ph (double p_MPa , double h_kJperKg );
	
	/** temperature (K) in region 1 for a given pressure and entropy */
	double if97_r1_t_ps (double p_MPa , double s_kJperKgK );
	
	

#endif // IF97_REGION1_H
