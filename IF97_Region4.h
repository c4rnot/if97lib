
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


/**
 * @copyright
 * Copyright Martin Lord 2014-2014. \n
 * Distributed under the Boost Software License, Version 1.0. \n
 * (See accompanying file LICENSE_1_0.txt or copy at \n
 * http://www.boost.org/LICENSE_1_0.txt) \n
 * 
 * @file IF97_Region4.h
 * @author Martin Lord
 * @date 29 Sep 2014
 * @brief IAPWS-IF97 Region 4 (Saturation Line) equations.
 *
 * @details 
 * VALIDITY  \n
 * 273.15 K 	<= T <= 647.096 K \n 
 * 611.213 Pa 	<= p <= 22.064 MPa \n
 * 
 * @see http://www.iapws.org/relguide/IF97-Rev.html
 */


#ifndef IF97_REGION4_H
#define IF97_REGION4_H

#include "IF97_common.h"
#include <math.h> 

/** Saturation pressure (MPa) for a given temperature (Kelvin) */
double if97_r4_ps ( double ts_Kelvin);

/** Saturation temperature (Kelvin) for a given pressure (MPa) */
double if97_r4_ts (double ps_MPa );


#endif // IF97_REGION4_H
