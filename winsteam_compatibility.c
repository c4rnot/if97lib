//          Copyright Martin Lord 2014-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


/*
 * Winsteam equivilent function calls
 */

 



#ifndef IF97_WINSTEAM_H
#define IF97_WINSTEAM_H

/*
 * Unit Set	| English	| SI		| English	| SI		| SI kPa	| Metric	| Metric	|
 * 			|			| Customary	| Gauge		| Formal	|			|			| Formal	|
 * No.		|   0		|   1		|   2		|   3		|   4		|   5		|   6		|
 * Name		|  "ENG"	|  "SI"		|  "ENGG"	|  "SIF"	|  "SIK"	|   "MET"	|  "METF"	|
 * Temp		|   F		|   C		|   F		|   K		|   C		|   C		|   C		|
 * Pressure	|   Psia	|   Bar		|   Psig	|   MPa		|   kPa		|   Bar		|   kg/cm 2	|
 * Enthalpy	|	btu/lbm	|   kJ/kg	|   btu/lbm	|   kJ/kg	|   kJ/kg	|   kCal/kg	|   kCal/kg	|
 * Entropy	| btu/lbm/F	| kJ/kg/C	| btu/lbm/F	| kJ/kg/K	| kJ/kg/C	| kJ/kg/C	| kJ/kg/C	|
 * Sp. Vol	|   ft3/lb	|   m3/kg	|   ft3/lb	|   m3/kg	|   m3/kg	|   m3/kg	|	 m3/kg	|
 * Quality	|   -		|   -		|   -		|   -		|   -		|   -		|   -		|
 * Sp. Heat	| btu/lbm/F	| kJ/kg/C	| btu/lbm/F	| kJ/kg/K	| kJ/kg/C	| kJ/kg/C	| kJ/kg/C	|
 * Thrm Cond| btu/h/ft/F|   W/m/C	| btu/h/ft/F|   W/m/K	|   W/m/C	|   W/m/C	|kCal/m/h/C	|
 * Dyn
*/

#include <stdio.h>
#include <string.h>   // for strcmp
#include <ctype.h>  // for tolower
#include "units.h"  // unit conversion library
#include <float.h>  //for minimum number storable for each type
#include "if97_lib.h" // IF 97 steam tables in MPa, K, kg, kJ 
//#include "winsteam_compatibility.h"

#define UNITSTRLEN 5




//============================================
// internally used routines

/*
char *lowercase (char *strOutput, const char *strInput, int OutLen){
	
//	char *ptrInChar = strInput; // point to first character of input
	char *ptrOutChar = strOutput; // point to first character of output
	
	int i = 0;
	
	while (*strInput != 0) { // while the character is not the end of string marker
		if (i >= OutLen) break;
		i++;
		*ptrOutChar = tolower(*strInput);  // place lowercase letter in output
		ptrOutChar++; strInput++;  //point to the next letter in the output and input
	}
	
	*ptrOutChar = 0;  // put a tring terminator on the end of the copied string


	
	return (strOutput);  //return a pointer to the lowercase string
}
*/


//=================
// EXTERNAL FUNCTIONS

//* get the integer value of the unit set calls for 67 tables  are +10*/
int getUnitSetNo (char *unitset){
	char strLowercase [UNITSTRLEN] = "";
	lowercase(strLowercase, unitset, UNITSTRLEN);
	
	
		if (strcmp (strLowercase, "0") == 0)  return 0;
		else if (strcmp (strLowercase,  "eng") == 0)  return 0;
		else if (strcmp (strLowercase,  "engo") == 0)  return 10;
		
		else if (strcmp (strLowercase,  "1") == 0)  return 1;
		else if (strcmp (strLowercase,  "si") == 0)  return 1;
		else if (strcmp (strLowercase,  "sio") == 0)  return 11;		
		
		else if (strcmp (strLowercase,  "2") == 0)  return 2;
		else if (strcmp (strLowercase,  "engg") == 0)  return 2;
		else if (strcmp (strLowercase,  "enggo") == 0)  return 12;		
		
		else if (strcmp (strLowercase,  "3") == 0)  return 3;
		else if (strcmp (strLowercase,  "sif") == 0)  return 3;
		else if (strcmp (strLowercase,  "sifo") == 0)  return 13;			

	
		else if (strcmp (strLowercase,  "4") == 0)  return 4;
		else if (strcmp (strLowercase,  "sik") == 0)  return 4;
		else if (strcmp (strLowercase,  "siko") == 0)  return 14;		

		else if (strcmp (strLowercase,  "5") == 0)  return 5;
		else if (strcmp (strLowercase,  "met") == 0)  return 5;
		else if (strcmp (strLowercase,  "meto") == 0)  return 15;	

		else if (strcmp (strLowercase,  "6") == 0)  return 6;
		else if (strcmp (strLowercase,  "metf") == 0)  return 6;
		else if (strcmp (strLowercase,  "metfo") == 0)  return 16;	
		
		else return 100;  // not found

}


// SATURATION LINE


/* saturation temperature for a given pressure */
double StmPT(double pressure, char* unitset){
	//char strLowercase [UNITSTRLEN] ="";
	
	switch ( getUnitSetNo(unitset)){
		case 0: //eng
			return convertNamedUnit(if97_Ps_t(convertNamedUnit (pressure,  "psi" , "MPa")), "kelvin", "fahrenheit");
			
		case 1: // "si"
		case 5:  // "met"
			return convertNamedUnit(if97_Ps_t(convertNamedUnit (pressure,  "bar" , "MPa")), "kelvin", "celcius");

		case 2: // "engg"
			return convertNamedUnit(if97_Ps_t(convertNamedUnit (pressure,  "psi" , "MPa") + 0.101325), "kelvin", "fahrenheit");

		case 3: //"sif":
			return if97_Ps_t(pressure);
			
			
		case 4: //"sik"
			return convertNamedUnit(if97_Ps_t(convertNamedUnit (pressure,  "kilopascal" , "MPa")), "kelvin", "celcius");
		
		case 6:  // "metf"
			return convertNamedUnit(if97_Ps_t(convertNamedUnit (pressure,  "technical atmosphere" , "MPa")), "kelvin", "celcius");
		
		case 10:  // engo
		case 11:  // meto
		case 12:  // enggo
		case 13:  // sifo
		case 14:  // siko
		case 15:  // sio
		case 16:  // metfo
			return DBL_MIN;  //1967 tables not supported yet
			
		case 100:  // unit string incorrect. Cant find unit No.
			return DBL_MIN + 1.0;
	}
	
	return DBL_MIN + 2.0;  // general error - should never occur
	
} // saturation temperature



/*  saturation pressure for a given temperatrue */
double StmTP(double temperature, char* unitset){
switch ( getUnitSetNo(unitset)){
		case 0: //eng
			return convertNamedUnit(if97_Ts_p(convertNamedUnit (temperature,  "fahrenheit" , "kelvin")),  "MPa", "psi" ); 
			
		case 1: // "si"
		case 5:  // "met"
			return convertNamedUnit(if97_Ts_p(convertNamedUnit (temperature, "celcius", "kelvin" )), "MPa", "bar"  );  

		case 2: // "engg"
			return convertNamedUnit(if97_Ts_p(convertNamedUnit (temperature, "fahrenheit", "kelvin" )) - 0.101325, "MPa"  , "psi" );  

		case 3: //"sif":
			return if97_Ts_p(temperature);
						
		case 4: //"sik"
			return convertNamedUnit(if97_Ts_p(convertNamedUnit (temperature, "celcius", "kelvin" )), "MPa" , "kilopascal" );   
		
		case 6:  // "metf"
			return convertNamedUnit(if97_Ts_p(convertNamedUnit (temperature, "celcius", "kelvin" )), "MPa" , "technical atmosphere"  ); 
		
		case 10:  // engo
		case 11:  // meto
		case 12:  // enggo
		case 13:  // sifo
		case 14:  // siko
		case 15:  // sio
		case 16:  // metfo
			return DBL_MIN;  //1967 tables not supported yet
			
		case 100:  // unit string incorrect. Cant find unit No.
			return DBL_MIN + 1.0;
	}
	
	return DBL_MIN + 2.0;  // general error - should never occur
}


// PT

/*  specific enthalpy for a given pressure and temperature */
double StmPTH(double pressure, double temperature);

/*  specific entropy for a given pressure and temperature */
double StmPTS(double pressure, double temperature);

/*  specific volume for a given pressure and temperature */
double StmPTV(double pressure, double temperature);

/*  specific heat capacity #TODO - Which one#  for a given pressure and temperature */
double StmPTC(double pressure, double temperature);

/*  thermal conductivity for a given pressure and temperature */
double StmPTK(double pressure, double temperature);

/*  dynamic viscosity for a given pressure and temperature */
double StmPTM(double pressure, double temperature);

/*  speed of sound for a given pressure and temperature */
double StmPTW(double pressure, double temperature);

/*  isentropic expansion coefficient for a given pressure and temperature */
double StmPTG(double pressure, double temperature);


// PH

/*  temperature for a given pressure and enthalpy */
double StmPHT(double pressure, double enthalpy);

/*  specific entropy for a given pressure and enthalpy */
double StmPHS(double pressure, double enthalpy);

/*  specific volume for a given pressure and enthalpy */
double StmPHV(double pressure, double enthalpy);

/*  quality for a given pressure and enthalpy */
double StmPHQ(double pressure, double enthalpy);

/*  specific heat capacity #TODO - Which one# for a given pressure and enthalpy */
double StmPHC(double pressure, double enthalpy);

/*  speed of sound for a given pressure and enthalpy */
double StmPHW(double pressure, double enthalpy);

/*  isentropic expansion coefficient for a given pressure and enthalpy */
double StmPHG(double pressure, double enthalpy);


// PS

/*  temperature for a given pressure and entropy */
double StmPST(double pressure, double entropy);

/*  specific enthalpy for a given pressure and entropy */
double StmPSH(double pressure, double entropy);

/*  specific volume for a given pressure and entropy */
double StmPSV(double pressure, double entropy);

/*  quality for a given pressure and entropy */
double StmPSQ(double pressure, double entropy);

/*  specific heat capacity #TODO - Which one# for a given pressure and entropy */
double StmPSC(double pressure, double entropy);

/*  speed of sound for a given pressure and entropy */
double StmPSW(double pressure, double entropy);

/*  isentropic expansion coefficient for a given pressure and entropy */
double StmPSG(double pressure, double entropy);


// TQ

/*  specific enthalpy for a given temperature and quality */
double StmTQH(double temperature, double quality);

/*  specific entropy for a given temperature and quality */
double StmTQS(double temperature, double quality);

/*  specific volume for a given temperature and quality */
double StmTQV(double temperature, double quality);

/*  specific heat capacity #TODO - Which one#  for a given temperature and quality */
double StmTQC(double temperature, double quality);

/*  thermal conductivity for a given temperature and quality */
double StmTQK(double temperature, double quality);

/*  dynamic viscosity for a given temperature and quality */
double StmTQM(double temperature, double quality);

/*  sonic speed for a given temperature and quality */
double StmTQW(double temperature, double quality);

/*  isentropic expansion coefficient # TODO - what is this? # for a given temperature and quality */
double StmTQG(double temperature, double quality);

//PQ

/*  specific enthalpy for a given pressure and quality */
double StmPQH(double pressure, double quality);

/*  specific entropy for a given pressure and quality */
double StmPQS(double pressure, double quality);

/*  specific volume for a given pressure and quality */
double StmPQV(double pressure, double quality);

/*  specific heat capacity #TODO - which one?# for a given pressure and quality */
double StmPQC(double pressure, double quality);

/*  thermal conductivity for a given pressure and quality */
double StmPQK(double pressure, double quality);


/*  dynamic viscosity for a given pressure and quality */
double StmPQM(double pressure, double quality);

/*  speed of sound for a given pressure and quality */
double StmPQW(double pressure, double quality);

/*  dynamic viscosity for a given pressure and quality */
double StmPQG(double pressure, double quality);


// MISCELLANEOUS

/*  converts ITS68 temperature to ITS90 */
double StmTEMPIT(double temp68);

/*  converts ITS90 temperature to ITS68 */
double StmTEMPTI(double temp90) ;

// text StmVER(void);








#endif // IF97_WINSTEAM_H

