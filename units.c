
//          Copyright Martin Lord 2014-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


//  Unit conversions

// Converts input to SI then SI to output


#include "units.h"
#include <string.h>
#include <stdbool.h>
//#include <math.h>


#include <stdio.h>



// dblPow0 + dblPow1.x  
//  dblPow0  + x . dblPow1 + x . dblPow2 ^ 2 ....
// log to base dblLogBase (dblPow0  + x . dblPow1 + x . dblPow2 ^ 2 ....)
// TODO calcs for logs
double applyCoeffs(double input, typConvCoeffs coeffs) {
return coeffs[0] + input * coeffs[1];
}



double applyInvCoeffs

const typUnit builtInUnits[] = {
	{ "void", "-", DIMENSIONLESS, { {0, 1 }, 0.0}, "void", false} //dummy to make sure include file consistency
#include "built_in_units.dat"
}
			
				
int getUnitIndex ( char[50] strUnit){
// TODO
	int i = 0;

while ( builtInUnits[i].strName  != strUnit ) { /* While x is less than 10 */
      
      i++;             /* go to next unit*/
  }	
		
return i;	
}


double convertUnit (double inVal, int inUnit, int outUnit){
// TODO

}


double isCompatible (int inUnit, int outUnit){
	// TODO
}

typDimensions getDimensions (int unit){
	// TODO
}



char[50] getUnitType (int unit){
// TODO
}			
				
				
		


#include <stdio.h>

int main(int argc, char **argv)
{

	printf (" \n\nIndex of ""Celcius"":  %i \n", getUnitIndex("Celcius"));


/*	
	printf (" \n\n0 C converts to %f Kelvin\n", applyCoeffs(0.0, Celcius.tConvCoeffs));
	printf (" 20 C converts to %f Kelvin\n", applyCoeffs(20.0, Celcius.tConvCoeffs));
	printf (" 100 C converts to %f Kelvin\n\n", applyCoeffs(100.0, Celcius.tConvCoeffs));
	
	printf (" 0 f converts to %f Kelvin\n", applyCoeffs(0.0, Fahrenheit.tConvCoeffs));
	printf (" 20 F converts to %f Kelvin\n", applyCoeffs(20.0, Fahrenheit.tConvCoeffs));
	printf (" 100 F converts to %f Kelvin\n\n", applyCoeffs(100.0, Fahrenheit.tConvCoeffs));
	
	printf (" 0 R converts to %f Kelvin\n", applyCoeffs(0.0, Rankine.tConvCoeffs));
	printf (" 20 R converts to %f Kelvin\n", applyCoeffs(20.0, Rankine.tConvCoeffs));
	printf (" 100 R converts to %f Kelvin\n\n", applyCoeffs(100.0, Rankine.tConvCoeffs));
*/	
	

	
	return 0;
}		
				
#undef COEFFS_MAXPOW		// allow any other file to use COEFFS_MAXPOW without confusion
