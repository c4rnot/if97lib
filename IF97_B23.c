
//          Copyright Martin Lord 2014-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

//    IAPWS-IF97 Auxiliary Equation for the boundary between Regions 2 & 3

#include "IF97_constants.h"
#include "IF97_B23.h"
#include <math.h>  // for square root
  
const double dPStar23 = 1.0 ;  // MPa
const double dTStar23 = 1.0 ; // K

// Numerical coefficients of the B23 equation.  See Table 1  (5 constants.  i starts at 1, so i0 is not used)
const double B23_N[6] = { 0, 0.34805185628969E+03, -0.11671859879975E+01
	, 0.10192970039326E-02, 0.57254459862746E+03, 0.13918839778870E+02
};

// from Equation 5.   note that since PStar is 1, the equation is simplified.  Theta is simply t
double IF97_B23P (double t)  {   // Pressure in MPa for a temperature in Kelvin
	return B23_N[1] + t*B23_N[2] + t*t*B23_N[3];
}	

// from Equation 6.   note that since TStar and PStar are 1, the equation is simplified.  Theta is simply temperature
double IF97_B23T (double p)  { // temperature in Kelvin for a  Pressure in MPa 
	   return B23_N[4] + sqrt((p- B23_N[5])/B23_N[3]);
}





