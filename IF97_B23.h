
//          Copyright Martin Lord 2014-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


//    IAPWS-IF97 Auxiliary Equation for the boundary between Regions 2 & 3

#ifndef IF97_B23_H
#define IF97_B23_H

	#include "IF97_common.h"
	#include "IF97_B23.h"
	#include <math.h>  // for square root
	#include <stdio.h> // for unit testing.  Remove once tested

	
	
	// from Equation 5.   note that since PStar is 1, the equation is simplified.  Theta is simply t
	double IF97_B23P (double t);   // Pressure in MPa for a temperature in Kelvin
	
	
	// from Equation 6.   note that since TStar and PStar are 1, the equation is simplified.  Theta is simply temperature
	double IF97_B23T (double p);  // temperature in Kelvin for a  Pressure in MPa 

#endif  // IF97_B23_H
