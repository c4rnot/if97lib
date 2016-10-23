
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
};
			

// for M L T dimensional analysis plus SI additions
typedef struct sctDimensions {
	int iMass;
	int iLength;
	int iTime;
	int iTemp;
	int iCurrent;
	int iNumParticles;
	int iLumIntens;	
} typDimensions;



typedef struct sctUnitTypeDefinition {
	char strTypeName[50];
	typDimensions fundamentalDimensions;
}typUnitTypeDefinition;


// this needs to match unitTypeT for mumbers and names
/*
typUnitTypeDefinition UnitTypeDefinitions[] = {
	{"DIMENSIONLESS", 	{0, 0, 0}} // 0	
	,{"MASS",			{1, 0, 0}} // 1
	,{"LENGTH", 		{0, 1, 0}} // 2
	,{"TIME",			{0, 0, 1}} // 3
	,{"UNUSED", 		{0, 0, 0}} // 4		unused		
	,{"ANGLE", 			{0, 0, 0}} // 5
	,{"AREA",			{0, 2, 0}} // 6
	,{"VOLUME",			{0, 3, 0}} // 7
	,{"UNUSED", 		{0, 0, 0}} // 8		unused	
	,{"ENTHALPY", 		{1, 2, -2}} // 9						
	,{"ENERGY",			{1, 2, -2}} // 10, 
	//			ENTROPY = 11,
	,{"SPEED", 			{1, 0, -1}} // 12,      needs new home VELOCITY {1, 0, -1} =12,
	//			TEMPERATURE =13, 
	,{"PRESSURE", 		{1, -1, -2}} // 14, 
	,{"POWER", 			{1, 2, -3}} // 15,	
	,{"TORQUE",			{1, 2, -2}} // 16;
	,{"ACCELERATION", 	{1, 0, -2}} // 17,
	,{"MOMENTUM", 		{1, 1, -1}} // 18,
	,{"FORCE", 			{1, 1, -2}} // 19,
				
				
	,{"SPECIFIC_ENERGY", {0, 2, -2}} // 20,  needs new home SPECIFIC_ENTHALPY {0, 2, -2}=20,
	//			SPECIFIC_ENTROPY = 21,
	//			SPECIFIC_HEAT_CAPACITY  = 23,				
	,{"SPECIFIC_POWER", {0, 2, -3}} // 25,
	,{"SPECIFIC_VOLUME", {-1, 3, 0}} // 27,
	,{"DENSITY",		{1, -3, 0}} // 15,   Number??
				
				
		/*		ELECTRICAL_POTENTIAL =20,
				ELECTRICAL_CURRENT = 21,
				ELECTRICAL_RESISTANCE = 22,
				ELECTRICAL_CHARGE = 23;
				ELECTRIC_DIPOLE = 24;
				ELECTRICAL_CAPACITANCE =25;
				MAGNETIC_FLUX = 26;
				MAGNETIC_FLUX_DENSITY = 27;
				ELECTRICAL_INDUCTANCE = 28;
				INFORMATION_ENTROPY = 29;
				
				LUMINOUS_INTENSITY   = 30;
				LUMINANCE = 31;
				LUMINOUS_FLUX = 32;
				ILLUMINANCE = 33;
	,{"RADIATION_SOURCE_ACTIVITY", {0, 0, -1}} // 35;
				RADIATION_EXPOSURE = 36;
				RADIATION_ABSORBED_DOSE = 37;
				RADIATION_EQUIVALENT_DOSE = 38;
						
			
				
	,{"MASS_FLOW",			{1, 0, -1}} // 40,
	,{"VOLUMETRIC_FLOW",	{0, 3, -1}} // 41,
	,{"DYNAMIC_VISCOSITY", {1, -1, -1}} // 42;
	,{"KINEMATIC VISCOSITY",	{0, 2, -1}} // 43;
	,{"BULK_MODULOUS", 		{1, -1, -2}} // 44
};
*/

typedef struct sctConvCoeffs {
	double dblPow[2];
	double dblLogBase;
} typConvCoeffs;


typedef struct sctUnit {
	char strName[50];
	char strSymbol[15];
	enum unitType_t unitType;
	typConvCoeffs tConvCoeffs;
	char strConvertsTo[50]; // strName of the SI Unit
	bool isSI;
} typUnit;





/* *********************************************************************
 *        FUNCTION DEFINITIONS
 */

/** gets the index of the unit for passing to other functions */
int getUnitIndex ( char strUnit[]);


double convertUnit (double inVal, int inUnit, int outUnit);


double isCompatible (int inUnit, int outUnit);


typDimensions getDimensions (int unit);

void getUnitType (int unit, char strUnitType[]);


#endif // UNITS_H






