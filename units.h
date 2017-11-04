//          Copyright Martin Lord 2014-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


//  Unit conversions

// Converts input to SI then SI to output


#ifndef UNITS_H
#define UNITS_H



#include <string.h>
#include <stdbool.h>
#include <float.h>  //for minimum number storable for each type

#define NAMESTRLEN 50

/* *********************************************************************
 *            TYPE DEFINITIONS 
 */

enum unitType_t { 
	DIMENSIONLESS = 0,  DIMLESS = 0, 
	
	MASS = 1, M = 1,
	LENGTH = 2, L = 2,
	TIME = 3, T = 3,

	ANGLE = 5,
	AREA =6,
	VOLUME = 7,

	ENTHALPY = 9,
	ENERGY = 10, 
	ENTROPY = 11,
	SPEED = 12, VELOCITY =12,
	TEMPERATURE =13, 
	PRESSURE = 14, 
	POWER = 15,	
	TORQUE =16,
	ACCELERATION = 17,
	MOMENTUM = 18,
	FORCE = 19,

	SPECIFIC_ENERGY = 20,   SPECIFIC_ENTHALPY =20,
	SPECIFIC_ENTROPY = 21,
	SPECIFIC_HEAT_CAPACITY  = 23,				
	SPECIFIC_POWER = 25,
	SPECIFIC_VOLUME = 27,
	DENSITY =15,

	ELECTRICAL_POTENTIAL =20,
	ELECTRICAL_CURRENT = 21,
	ELECTRICAL_RESISTANCE = 22,
	ELECTRICAL_CHARGE = 23,
	ELECTRIC_DIPOLE = 24,
	ELECTRICAL_CAPACITANCE =25,
	MAGNETIC_FLUX = 26,
	MAGNETIC_FLUX_DENSITY = 27,
	ELECTRICAL_INDUCTANCE = 28,
	INFORMATION_ENTROPY = 29,

	LUMINOUS_INTENSITY = 30,
	LUMINANCE = 31,
	LUMINOUS_FLUX = 32,
	ILLUMINANCE = 33,
	RADIATION_SOURCE_ACTIVITY = 35,
	RADIATION_EXPOSURE = 36,
	RADIATION_ABSORBED_DOSE = 37,
	RADIATION_EQUIVALENT_DOSE = 38,

	MASS_FLOW = 40,
	VOLUMETRIC_FLOW = 41,
	DYNAMIC_VISCOSITY =42,
	KINEMATIC_VISCOSITY = 43,
	THERMAL_CONDUCTIVITY = 45,
};


typedef struct sctConvCoeffs {
	double dblPow[2];
	double dblLogBase;
} typConvCoeffs;


typedef struct sctUnit {
	char strName[NAMESTRLEN];
	char strSymbol[15];
	enum unitType_t unitType;
	typConvCoeffs tConvCoeffs;
	char strConvertsTo[NAMESTRLEN]; // strName of the SI Unit
	bool isSI;
} typUnit;




/* *********************************************************************
 *        FUNCTION DEFINITIONS
 */




/** gets the index of the unit for passing to other functions */
int getUnitIndex ( const char strUnit[]);

/** get the conversion coefficients corresponding to UnitIndex */
typConvCoeffs getCoeffs (int UnitIndex);

/** Converts a unit by passing the unit reference indices  */
double convertUnit (double inVal, int inUnit, int outUnit);

/** converts a unit by passing the unit names. It will determine the 
 * indices from the names.  Error value equal to DBL_MIN in <float.h>*/
double convertNamedUnit (double inVal, const char strInUnit[], const char strOutUnit[]);

double isCompatible (int inUnit, int outUnit);


//typDimensions getDimensions (int unit);

void getUnitType (int unit, char strUnitType[]);


/** puts a lowercase version of strInput into strOutput. returns pointer to strOutput  */
char *lowercase (char *strOutput, const char *strInput, int maxLen);


#endif // UNITS_H

