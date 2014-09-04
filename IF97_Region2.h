
//          Copyright Martin Lord 2014-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


//    IAPWS-IF97 Region 2 equations
// VALIDITY  x K <= T <= x K    x <= p <= x MPa

#ifndef IF97_REGION2_H
#define IF97_REGION2_H

#include "IF97_constants.h"
#include <math.h> 



//**************************************************************
//********* REGION 2 PROPERTY EQUATIONS (FORWARDS) *************

	// specific Gibbs free energy in region 2 (kJ / kg)
	double if97_r2_g (double p_MPa , double t_Kelvin);

	// specific volume in region 2  (metres cubed per kilogram)  
	double if97_r2_v (double p_MPa , double t_Kelvin );
			
	
	// specific internal energy in region 2 (KJ / Kg)
	double if97_r2_u (double p_MPa , double t_Kelvin );
	
	
	// specific entropy in region 2 (KJ / Kg.K)
	double if97_r2_s (double p_MPa , double t_Kelvin );
	
	
	// specific enthalpy in region 2 (KJ / Kg)
	double if97_r2_h (double p_MPa , double t_Kelvin );
	
	
	// specific isobaric heat capacity in region 2 (KJ / Kg.K)
	double if97_r2_Cp (double p_MPa , double t_Kelvin );
	
	
	// specific isochoric heat capacity in region 2 (KJ / Kg.K)
	double if97_r2_Cv (double p_MPa , double t_Kelvin );
	
		
	// speed of sound in region 2 (m/s)
	double if97_r2_w (double p_MPa , double t_Kelvin );







#endif // IF97_REGION2_H
