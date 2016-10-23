
//          Copyright Martin Lord 2014-2015.
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
 * @file IF97_Region1bw.h
 * @author Martin Lord
 * @date 20 June 2015
 * @brief IAPWS-IF97 Region 1 backwards equations.
 *
 * @details 
 * VALIDITY  \n
 * 273.15 K <= T <= 623.15 K     Psat <= p <= 100 MPa \n
 * Exception:  Backwards equations not valid in metastable (superheated liquid) region
 * 
 * @see http://www.iapws.org/relguide/IF97-Rev.html
 */


#ifndef IF97_REGION1BW_H
#define IF97_REGION1BW_H

#include "IF97_common.h"
#include <math.h> 



//***********************************************************
//********* REGION 1 BACKWARDS EQUATIONS*********************

	/** temperature (K) in region 1 for a given pressure and enthalpy */
	double if97_r1_t_ph (double p_MPa , double h_kJperKg );
	
	/** temperature (K) in region 1 for a given pressure and entropy */
	double if97_r1_t_ps (double p_MPa , double s_kJperKgK );
	
	

#endif // IF97_REGION1BW_H
