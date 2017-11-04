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
 * Set No.	|   0		|   1		|   2		|   3		|   4		|   5		|   6		|
 * Name		|  "ENG"	|  "SI"		|  "ENGG"	|  "SIF"	|  "SIK"	|   "MET"	|  "METF"	|
 * Temp		|   F		|   C		|   F		|   K		|   C		|   C		|   C		|
 * Pressure	|   Psia	|   Bar		|   Psig	|   MPa		|   kPa		|   Bar		|   kg/cm2 	|
 * Enthalpy	|	btu/lbm	|   kJ/kg	|   btu/lbm	|   kJ/kg	|   kJ/kg	|   kCal/kg	|   kCal/kg	|
 * Entropy	| btu/lbm/F	| kJ/kg/C	| btu/lbm/F	| kJ/kg/K	| kJ/kg/C	| kCal/kg/C	| kCal/kg/C	|
 * Sp. Vol	|   ft3/lb	|   m3/kg	|   ft3/lb	|   m3/kg	|   m3/kg	|   m3/kg	|	 m3/kg	|
 * Quality	|   -		|   -		|   -		|   -		|   -		|   -		|   -		|
 * Cp       | btu/lbm/F	| kJ/kg/C	| btu/lbm/F	| kJ/kg/K	| kJ/kg/C	| kCal/kg/C	| kCal/kg/C	|
 * Thrm Cond| btu/h/ft/F|   W/m/C	| btu/h/ft/F|   W/m/K	|   W/m/C	|   W/m/C	|kCal/m/h/C	|
 * Dyn Visc |  lb/ft/h  | centipoise|  lb/ft/h  |   Pa.s    | centipoise| centipoise| centipoise|
 * Cp/Cv    |     -     |     -     |     -     |     -     |     -     |     -     |     -     |
 * Sonic Vel|  ft/s     |    m/s    |    m/s    |    m/s    |    m/s    |    m/s    |    m/s    |
 *
 * IFC-67 tables add 10 to the Set No.
 */

#include <stdio.h>
#include <string.h>   // for strcmp
#include <ctype.h>  // for tolower
#include "units.h"  // unit conversion library, lowercase
#include <float.h>  //for minimum number storable for each type
#include "if97_lib.h" // IF 97 steam tables in MPa, K, kg, kJ 
#include "winsteam_compatibility.h"

#define UNITSTRLEN 5




const char *arrUSets [7][12] =  {  // [unitset][unittype]
	{ "ENG",  "F", "psia",  "btu/lb", "btu/lb/F", "ft3/lb", "-", "btu/lb/F", "btu/h/ft/F",  "lb/ft/h",    "-", "ft/s" },
	{ "SI",   "C", "bar",   "kJ/kg",  "kJ/kg/K",  "m3/kg",  "-", "kJ/kg/C",  "W/m/C",       "centipoise", "-", "m/s"},
	{ "ENGG", "F", "psig",  "btu/lb", "btu/lb/F", "ft3/lb", "-", "btu/lb/F", "btu/h/ft/F",  "lb/ft/h",    "-", "ft/s"},
	{ "SIF",  "K", "MPa",   "kJ/kg",  "kJ/kg/K",  "m3/kg",  "-", "kJ/kg/K",  "W/m/K",       "Pa.s",       "-", "m/s" },
	{ "SIK",  "C", "kPa",   "kJ/kg",  "kJ/kg/C",  "m3/kg",  "-", "kJ/kg/C",  "W/m/C",       "centipoise", "-", "m/s" },
	{ "MET",  "C", "bar",   "kCal/kg","kCal/kg/C","m3/kg",  "-", "kCal/kg/C","kCal/m/h/C",       "centipoise", "-", "m/s"},
	{ "METF", "C", "kg/cm2","kCal/kg","kCal/kg/C","m3/kg",  "-", "kCal/kg/C","kCal/m/h/C",  "centipoise", "-", "m/s" }
};


enum param { NAME, TEMP, PRESS, ENTH, ENTR, VOL, QUAL, SPEC_HEAT, COND, VISC, GAMMA, VEL };

enum uSet { ENG, SI, ENGG, SIF, SIK, MET, METF};

//============================================
// internally used routines

/*  already provided by libunuts
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

//* get the integer value of the unit set calls for IFC 67 tables  are +10 */
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


// ********   SATURATION LINE   **************


/* saturation temperature for a given pressure */
double StmPT(double pressure, char* unitset){
	
	int iUSet = getUnitSetNo(unitset);
	
	if (iUSet > 6 ) return DBL_MIN;  //1967 tables not supported yet
	if (iUSet == 100)  return DBL_MIN + 1.0;  //unit string incorrect. Cant find unit No.
	
	return convertNamedUnit(if97_Ps_t(convertNamedUnit (pressure,  arrUSets[iUSet][PRESS] , arrUSets[SIF][PRESS])), arrUSets[SIF][TEMP], arrUSets[iUSet][TEMP]);
	

} // StmPT



/*  saturation pressure for a given temperatrue */
double StmTP(double temperature, char* unitset){
	
	int iUSet = getUnitSetNo(unitset);
	
	if (iUSet > 6 ) return DBL_MIN;  //1967 tables not supported yet
	if (iUSet == 100)  return DBL_MIN + 1.0;  //unit string incorrect. Cant find unit No.
	
	return convertNamedUnit(if97_Ts_p(convertNamedUnit (temperature,  arrUSets[iUSet][TEMP] , arrUSets[SIF][TEMP])), arrUSets[SIF][PRESS], arrUSets[iUSet][PRESS]);

} // StmTP




// ***************** PT *********************

/*  specific enthalpy for a given pressure and temperature*/
double StmPTH(double pressure, double temperature, char* unitset){
	
	int iUSet = getUnitSetNo(unitset);
	
	if (iUSet > 6 ) return DBL_MIN;  //1967 tables not supported yet
	if (iUSet == 100)  return DBL_MIN + 1.0;  //unit string incorrect. Cant find unit No.
	
	return convertNamedUnit(if97_pt_h(convertNamedUnit (pressure,  arrUSets[iUSet][PRESS] , arrUSets[SIF][PRESS]), \
									  convertNamedUnit (temperature,  arrUSets[iUSet][TEMP] , arrUSets[SIF][TEMP])), \
							arrUSets[SIF][ENTH], arrUSets[iUSet][ENTH]);

} // StmPTH



/*  specific entropy for a given pressure and temperature */
double StmPTS(double pressure, double temperature, char* unitset){
	
	int iUSet = getUnitSetNo(unitset);
	
	if (iUSet > 6 ) return DBL_MIN;  //1967 tables not supported yet
	if (iUSet == 100)  return DBL_MIN + 1.0;  //unit string incorrect. Cant find unit No.
	
	return convertNamedUnit(if97_pt_s(convertNamedUnit (pressure,  arrUSets[iUSet][PRESS] , arrUSets[SIF][PRESS]), \
									  convertNamedUnit (temperature,  arrUSets[iUSet][TEMP] , arrUSets[SIF][TEMP])), \
							arrUSets[SIF][ENTR], arrUSets[iUSet][ENTR]);

} // StmPTS


/*  specific volume for a given pressure and temperature */
double StmPTV(double pressure, double temperature, char* unitset){
	
	int iUSet = getUnitSetNo(unitset);
	
	if (iUSet > 6 ) return DBL_MIN;  //1967 tables not supported yet
	if (iUSet == 100)  return DBL_MIN + 1.0;  //unit string incorrect. Cant find unit No.
	
	return convertNamedUnit(if97_pt_v(convertNamedUnit (pressure,  arrUSets[iUSet][PRESS] , arrUSets[SIF][PRESS]), \
									  convertNamedUnit (temperature,  arrUSets[iUSet][TEMP] , arrUSets[SIF][TEMP])), \
							arrUSets[SIF][VOL], arrUSets[iUSet][VOL]);

} // StmPTV



/*  specific isobaric heat capacity Cp for a given pressure and temperature */
double StmPTC(double pressure, double temperature, char* unitset){
	
	int iUSet = getUnitSetNo(unitset);
	
	if (iUSet > 6 ) return DBL_MIN;  //1967 tables not supported yet
	if (iUSet == 100)  return DBL_MIN + 1.0;  //unit string incorrect. Cant find unit No.
	
	return convertNamedUnit(if97_pt_Cp(convertNamedUnit (pressure,  arrUSets[iUSet][PRESS] , arrUSets[SIF][PRESS]), \
									  convertNamedUnit (temperature,  arrUSets[iUSet][TEMP] , arrUSets[SIF][TEMP])), \
							arrUSets[SIF][SPEC_HEAT], arrUSets[iUSet][SPEC_HEAT]);

} // StmPTC




/*  thermal conductivity for a given pressure and temperature */
double StmPTK(double pressure, double temperature, char* unitset){
	
	int iUSet = getUnitSetNo(unitset);
	
	if (iUSet > 6 ) return DBL_MIN;  //1967 tables not supported yet
	if (iUSet == 100)  return DBL_MIN + 1.0;  //unit string incorrect. Cant find unit No.
	
	return convertNamedUnit(if97_pt_k(convertNamedUnit (pressure,  arrUSets[iUSet][PRESS] , arrUSets[SIF][PRESS]), \
									  convertNamedUnit (temperature,  arrUSets[iUSet][TEMP] , arrUSets[SIF][TEMP])), \
							arrUSets[SIF][COND], arrUSets[iUSet][COND]);

} // StmPTK



/*  dynamic viscosity for a given pressure and temperature */
double StmPTM(double pressure, double temperature, char* unitset){
	
	int iUSet = getUnitSetNo(unitset);
	
	if (iUSet > 6 ) return DBL_MIN;  //1967 tables not supported yet
	if (iUSet == 100)  return DBL_MIN + 1.0;  //unit string incorrect. Cant find unit No.
	
	return convertNamedUnit(if97_pt_mu(convertNamedUnit (pressure,  arrUSets[iUSet][PRESS] , arrUSets[SIF][PRESS]), \
									  convertNamedUnit (temperature,  arrUSets[iUSet][TEMP] , arrUSets[SIF][TEMP])), \
							arrUSets[SIF][VISC], arrUSets[iUSet][VISC]);

} // StmPTM




/*  speed of sound for a given pressure and temperature */
double StmPTW(double pressure, double temperature, char* unitset){
	
	int iUSet = getUnitSetNo(unitset);
	
	if (iUSet > 6 ) return DBL_MIN;  //1967 tables not supported yet
	if (iUSet == 100)  return DBL_MIN + 1.0;  //unit string incorrect. Cant find unit No.
	
	return convertNamedUnit(if97_pt_Vs(convertNamedUnit (pressure,  arrUSets[iUSet][PRESS] , arrUSets[SIF][PRESS]), \
									  convertNamedUnit (temperature,  arrUSets[iUSet][TEMP] , arrUSets[SIF][TEMP])), \
							arrUSets[SIF][VEL], arrUSets[iUSet][VEL]);

} // StmPTW



/*  isentropic expansion coefficient for a given pressure and temperature */
double StmPTG(double pressure, double temperature, char* unitset){
	
	int iUSet = getUnitSetNo(unitset);
	
	if (iUSet > 6 ) return DBL_MIN;  //1967 tables not supported yet
	if (iUSet == 100)  return DBL_MIN + 1.0;  //unit string incorrect. Cant find unit No.
	
	return convertNamedUnit(if97_pt_gamma(convertNamedUnit (pressure,  arrUSets[iUSet][PRESS] , arrUSets[SIF][PRESS]), \
									  convertNamedUnit (temperature,  arrUSets[iUSet][TEMP] , arrUSets[SIF][TEMP])), \
							arrUSets[SIF][GAMMA], arrUSets[iUSet][GAMMA]);

} // StmPTG

/*  Uncomment as these become availabel in if97_lib

// ************** PH *********************

//  temperature for a given pressure and enthalpy
double StmPHT(double pressure, double enthalpy, char* unitset){
	
	int iUSet = getUnitSetNo(unitset);
	
	if (iUSet > 6 ) return DBL_MIN;  //1967 tables not supported yet
	if (iUSet == 100)  return DBL_MIN + 1.0;  //unit string incorrect. Cant find unit No.
	
	return convertNamedUnit(if97_ph_t(convertNamedUnit (pressure,  arrUSets[iUSet][PRESS] , arrUSets[SIF][PRESS]), \
									  convertNamedUnit (enthalpy,  arrUSets[iUSet][ENTH] , arrUSets[SIF][ENTH])), \
							arrUSets[SIF][TEMP], arrUSets[iUSet][TEMP]);

} // StmPHT




//  specific entropy for a given pressure and enthalpy 
double StmPHS(double pressure, double enthalpy, char* unitset){
	
	int iUSet = getUnitSetNo(unitset);
	
	if (iUSet > 6 ) return DBL_MIN;  //1967 tables not supported yet
	if (iUSet == 100)  return DBL_MIN + 1.0;  //unit string incorrect. Cant find unit No.
	
	return convertNamedUnit(if97_ph_s(convertNamedUnit (pressure,  arrUSets[iUSet][PRESS] , arrUSets[SIF][PRESS]), \
									  convertNamedUnit (enthalpy,  arrUSets[iUSet][ENTH] , arrUSets[SIF][ENTH])), \
							arrUSets[SIF][ENTR], arrUSets[iUSet][ENTR]);

} // StmPHS



//  specific volume for a given pressure and enthalpy
double StmPHV(double pressure, double enthalpy, char* unitset){
	
	int iUSet = getUnitSetNo(unitset);
	
	if (iUSet > 6 ) return DBL_MIN;  //1967 tables not supported yet
	if (iUSet == 100)  return DBL_MIN + 1.0;  //unit string incorrect. Cant find unit No.
	
	return convertNamedUnit(if97_ph_v(convertNamedUnit (pressure,  arrUSets[iUSet][PRESS] , arrUSets[SIF][PRESS]), \
									  convertNamedUnit (enthalpy,  arrUSets[iUSet][ENTH] , arrUSets[SIF][ENTH])), \
							arrUSets[SIF][VOL], arrUSets[iUSet][VOL]);

} // StmPHV



//  quality for a given pressure and enthalpy
double StmPHQ(double pressure, double enthalpy, char* unitset){
	
	int iUSet = getUnitSetNo(unitset);
	
	if (iUSet > 6 ) return DBL_MIN;  //1967 tables not supported yet
	if (iUSet == 100)  return DBL_MIN + 1.0;  //unit string incorrect. Cant find unit No.
	
	return convertNamedUnit(if97_ph_q(convertNamedUnit (pressure,  arrUSets[iUSet][PRESS] , arrUSets[SIF][PRESS]), \
									  convertNamedUnit (enthalpy,  arrUSets[iUSet][ENTH] , arrUSets[SIF][ENTH])), \
							arrUSets[SIF][QUAL], arrUSets[iUSet][QUAL]);

} // StmPHQ



//  specific isobaric heat capacity Cp for a given pressure and enthalpy
double StmPHC(double pressure, double enthalpy, char* unitset){
	
	int iUSet = getUnitSetNo(unitset);
	
	if (iUSet > 6 ) return DBL_MIN;  //1967 tables not supported yet
	if (iUSet == 100)  return DBL_MIN + 1.0;  //unit string incorrect. Cant find unit No.
	
	return convertNamedUnit(if97_ph_Cp(convertNamedUnit (pressure,  arrUSets[iUSet][PRESS] , arrUSets[SIF][PRESS]), \
									  convertNamedUnit (enthalpy,  arrUSets[iUSet][ENTH] , arrUSets[SIF][ENTH])), \
							arrUSets[SIF][SPEC_HEAT], arrUSets[iUSet][SPEC_HEAT]);

} // StmPHC



//  speed of sound for a given pressure and enthalpy
double StmPHW(double pressure, double enthalpy, char* unitset){
	
	int iUSet = getUnitSetNo(unitset);
	
	if (iUSet > 6 ) return DBL_MIN;  //1967 tables not supported yet
	if (iUSet == 100)  return DBL_MIN + 1.0;  //unit string incorrect. Cant find unit No.
	
	return convertNamedUnit(if97_ph_Vs(convertNamedUnit (pressure,  arrUSets[iUSet][PRESS] , arrUSets[SIF][PRESS]), \
									  convertNamedUnit (enthalpy,  arrUSets[iUSet][ENTH] , arrUSets[SIF][ENTH])), \
							arrUSets[SIF][VEL], arrUSets[iUSet][VEL]);

} // StmPHW



//  isentropic expansion coefficient for a given pressure and enthalpy
double StmPHG(double pressure, double enthalpy, char* unitset){
	
	int iUSet = getUnitSetNo(unitset);
	
	if (iUSet > 6 ) return DBL_MIN;  //1967 tables not supported yet
	if (iUSet == 100)  return DBL_MIN + 1.0;  //unit string incorrect. Cant find unit No.
	
	return convertNamedUnit(if97_ph_gamma(convertNamedUnit (pressure,  arrUSets[iUSet][PRESS] , arrUSets[SIF][PRESS]), \
									  convertNamedUnit (enthalpy,  arrUSets[iUSet][ENTH] , arrUSets[SIF][ENTH])), \
							arrUSets[SIF][GAMMA], arrUSets[iUSet][GAMMA]);

} // StmPHW


// *************  PS  ***************************

//  temperature for a given pressure and entropy
double StmPST(double pressure, double entropy, char* unitset){
	
	int iUSet = getUnitSetNo(unitset);
	
	if (iUSet > 6 ) return DBL_MIN;  //1967 tables not supported yet
	if (iUSet == 100)  return DBL_MIN + 1.0;  //unit string incorrect. Cant find unit No.
	
	return convertNamedUnit(if97_ps_t(convertNamedUnit (pressure,  arrUSets[iUSet][PRESS] , arrUSets[SIF][PRESS]), \
									  convertNamedUnit (entropy,  arrUSets[iUSet][ENTR] , arrUSets[SIF][ENTR])), \
							arrUSets[SIF][TEMP], arrUSets[iUSet][TEMP]);

} // StmPST



//  specific enthalpy for a given pressure and entropy
double StmPSH(double pressure, double entropy, char* unitset){
	
	int iUSet = getUnitSetNo(unitset);
	
	if (iUSet > 6 ) return DBL_MIN;  //1967 tables not supported yet
	if (iUSet == 100)  return DBL_MIN + 1.0;  //unit string incorrect. Cant find unit No.
	
	return convertNamedUnit(if97_ps_h(convertNamedUnit (pressure,  arrUSets[iUSet][PRESS] , arrUSets[SIF][PRESS]), \
									  convertNamedUnit (entropy,  arrUSets[iUSet][ENTR] , arrUSets[SIF][ENTR])), \
							arrUSets[SIF][ENTH], arrUSets[iUSet][ENTH]);

} // StmPSH



//  specific volume for a given pressure and entropy
double StmPSV(double pressure, double entropy, char* unitset){
	
	int iUSet = getUnitSetNo(unitset);
	
	if (iUSet > 6 ) return DBL_MIN;  //1967 tables not supported yet
	if (iUSet == 100)  return DBL_MIN + 1.0;  //unit string incorrect. Cant find unit No.
	
	return convertNamedUnit(if97_ps_v(convertNamedUnit (pressure,  arrUSets[iUSet][PRESS] , arrUSets[SIF][PRESS]), \
									  convertNamedUnit (entropy,  arrUSets[iUSet][ENTR] , arrUSets[SIF][ENTR])), \
							arrUSets[SIF][VOL], arrUSets[iUSet][VOL]);

} // StmPSV



//  quality for a given pressure and entropy
double StmPSQ(double pressure, double entropy, char* unitset){
	
	int iUSet = getUnitSetNo(unitset);
	
	if (iUSet > 6 ) return DBL_MIN;  //1967 tables not supported yet
	if (iUSet == 100)  return DBL_MIN + 1.0;  //unit string incorrect. Cant find unit No.
	
	return convertNamedUnit(if97_ps_q(convertNamedUnit (pressure,  arrUSets[iUSet][PRESS] , arrUSets[SIF][PRESS]), \
									  convertNamedUnit (entropy,  arrUSets[iUSet][ENTR] , arrUSets[SIF][ENTR])), \
							arrUSets[SIF][QUAL], arrUSets[iUSet][QUAL]);

} // StmPSQ



//  specific isobaric heat capacity Cp for a given pressure and entropy
double StmPSC(double pressure, double entropy, char* unitset){
	
	int iUSet = getUnitSetNo(unitset);
	
	if (iUSet > 6 ) return DBL_MIN;  //1967 tables not supported yet
	if (iUSet == 100)  return DBL_MIN + 1.0;  //unit string incorrect. Cant find unit No.
	
	return convertNamedUnit(if97_ps_Cp(convertNamedUnit (pressure,  arrUSets[iUSet][PRESS] , arrUSets[SIF][PRESS]), \
									  convertNamedUnit (entropy,  arrUSets[iUSet][ENTR] , arrUSets[SIF][ENTR])), \
							arrUSets[SIF][SPEC_HEAT], arrUSets[iUSet][SPEC_HEAT]);

} // StmPSC

//  speed of sound for a given pressure and entropy
double StmPSW(double pressure, double entropy, char* unitset){
	
	int iUSet = getUnitSetNo(unitset);
	
	if (iUSet > 6 ) return DBL_MIN;  //1967 tables not supported yet
	if (iUSet == 100)  return DBL_MIN + 1.0;  //unit string incorrect. Cant find unit No.
	
	return convertNamedUnit(if97_ps_Vs(convertNamedUnit (pressure,  arrUSets[iUSet][PRESS] , arrUSets[SIF][PRESS]), \
									  convertNamedUnit (entropy,  arrUSets[iUSet][ENTR] , arrUSets[SIF][ENTR])), \
							arrUSets[SIF][VEL], arrUSets[iUSet][VEL]);

} // StmPSW



//  isentropic expansion coefficient for a given pressure and entropy
double StmPSG(double pressure, double entropy, char* unitset){
	
	int iUSet = getUnitSetNo(unitset);
	
	if (iUSet > 6 ) return DBL_MIN;  //1967 tables not supported yet
	if (iUSet == 100)  return DBL_MIN + 1.0;  //unit string incorrect. Cant find unit No.
	
	return convertNamedUnit(if97_ps_gamma(convertNamedUnit (pressure,  arrUSets[iUSet][PRESS] , arrUSets[SIF][PRESS]), \
									  convertNamedUnit (entropy,  arrUSets[iUSet][ENTR] , arrUSets[SIF][ENTR])), \
							arrUSets[SIF][GAMMA], arrUSets[iUSet][GAMMA]);

} // StmPSG



// *************  TQ  ********************

//  specific enthalpy for a given temperature and quality
double StmTQH(double temperature, double quality, char* unitset){
	
	int iUSet = getUnitSetNo(unitset);
	
	if (iUSet > 6 ) return DBL_MIN;  //1967 tables not supported yet
	if (iUSet == 100)  return DBL_MIN + 1.0;  //unit string incorrect. Cant find unit No.
	
	return convertNamedUnit(if97_tq_h(convertNamedUnit (temperature,  arrUSets[iUSet][TEMP] , arrUSets[SIF][TEMP]), \
									  convertNamedUnit (quality,  arrUSets[iUSet][QUAL] , arrUSets[SIF][QUAL])), \
							arrUSets[SIF][ENTH], arrUSets[iUSet][ENTH]);

} // StmTQH



//  specific entropy for a given temperature and quality
double StmTQS(double temperature, double quality, char* unitset){
	
	int iUSet = getUnitSetNo(unitset);
	
	if (iUSet > 6 ) return DBL_MIN;  //1967 tables not supported yet
	if (iUSet == 100)  return DBL_MIN + 1.0;  //unit string incorrect. Cant find unit No.
	
	return convertNamedUnit(if97_tq_s(convertNamedUnit (temperature,  arrUSets[iUSet][TEMP] , arrUSets[SIF][TEMP]), \
									  convertNamedUnit (quality,  arrUSets[iUSet][QUAL] , arrUSets[SIF][QUAL])), \
							arrUSets[SIF][ENTR], arrUSets[iUSet][ENTR]);

} // StmTQS



//  specific volume for a given temperature and quality
double StmTQV(double temperature, double quality, char* unitset){
	
	int iUSet = getUnitSetNo(unitset);
	
	if (iUSet > 6 ) return DBL_MIN;  //1967 tables not supported yet
	if (iUSet == 100)  return DBL_MIN + 1.0;  //unit string incorrect. Cant find unit No.
	
	return convertNamedUnit(if97_tq_v(convertNamedUnit (temperature,  arrUSets[iUSet][TEMP] , arrUSets[SIF][TEMP]), \
									  convertNamedUnit (quality,  arrUSets[iUSet][QUAL] , arrUSets[SIF][QUAL])), \
							arrUSets[SIF][VOL], arrUSets[iUSet][VOL]);

} // StmTQV



//  specific isobaric heat capacity Cp for a given temperature and quality
double StmTQC(double temperature, double quality, char* unitset){
	
	int iUSet = getUnitSetNo(unitset);
	
	if (iUSet > 6 ) return DBL_MIN;  //1967 tables not supported yet
	if (iUSet == 100)  return DBL_MIN + 1.0;  //unit string incorrect. Cant find unit No.
	
	return convertNamedUnit(if97_tq_Cp(convertNamedUnit (temperature,  arrUSets[iUSet][TEMP] , arrUSets[SIF][TEMP]), \
									  convertNamedUnit (quality,  arrUSets[iUSet][QUAL] , arrUSets[SIF][QUAL])), \
							arrUSets[SIF][SPEC_HEAT], arrUSets[iUSet][SPEC_HEAT]);

} // StmTQC



//  thermal conductivity for a given temperature and quality
double StmTQK(double temperature, double quality, char* unitset){
	
	int iUSet = getUnitSetNo(unitset);
	
	if (iUSet > 6 ) return DBL_MIN;  //1967 tables not supported yet
	if (iUSet == 100)  return DBL_MIN + 1.0;  //unit string incorrect. Cant find unit No.
	
	return convertNamedUnit(if97_tq_k(convertNamedUnit (temperature,  arrUSets[iUSet][TEMP] , arrUSets[SIF][TEMP]), \
									  convertNamedUnit (quality,  arrUSets[iUSet][QUAL] , arrUSets[SIF][QUAL])), \
							arrUSets[SIF][COND], arrUSets[iUSet][COND]);

} // StmTQK



//  dynamic viscosity for a given temperature and quality
double StmTQM(double temperature, double quality, char* unitset){
	
	int iUSet = getUnitSetNo(unitset);
	
	if (iUSet > 6 ) return DBL_MIN;  //1967 tables not supported yet
	if (iUSet == 100)  return DBL_MIN + 1.0;  //unit string incorrect. Cant find unit No.
	
	return convertNamedUnit(if97_tq_mu(convertNamedUnit (temperature,  arrUSets[iUSet][TEMP] , arrUSets[SIF][TEMP]), \
									  convertNamedUnit (quality,  arrUSets[iUSet][QUAL] , arrUSets[SIF][QUAL])), \
							arrUSets[SIF][VISC], arrUSets[iUSet][VISC]);

} // StmTQM



//  sonic speed for a given temperature and quality
double StmTQW(double temperature, double quality, char* unitset){
	
	int iUSet = getUnitSetNo(unitset);
	
	if (iUSet > 6 ) return DBL_MIN;  //1967 tables not supported yet
	if (iUSet == 100)  return DBL_MIN + 1.0;  //unit string incorrect. Cant find unit No.
	
	return convertNamedUnit(if97_tq_Vs(convertNamedUnit (temperature,  arrUSets[iUSet][TEMP] , arrUSets[SIF][TEMP]), \
									  convertNamedUnit (quality,  arrUSets[iUSet][QUAL] , arrUSets[SIF][QUAL])), \
							arrUSets[SIF][VEL], arrUSets[iUSet][VEL]);

} // StmTQW



//  isentropic expansion coefficient for a given temperature and quality
double StmTQG(double temperature, double quality, char* unitset){
	
	int iUSet = getUnitSetNo(unitset);
	
	if (iUSet > 6 ) return DBL_MIN;  //1967 tables not supported yet
	if (iUSet == 100)  return DBL_MIN + 1.0;  //unit string incorrect. Cant find unit No.
	
	return convertNamedUnit(if97_tq_gamma(convertNamedUnit (temperature,  arrUSets[iUSet][TEMP] , arrUSets[SIF][TEMP]), \
									  convertNamedUnit (quality,  arrUSets[iUSet][QUAL] , arrUSets[SIF][QUAL])), \
							arrUSets[SIF][GAMMA], arrUSets[iUSet][GAMMA]);

} // StmTQG



// **************** PQ  ******************

//  specific enthalpy for a given pressure and quality
double StmPQH(double pressure, double quality, char* unitset){
	
	int iUSet = getUnitSetNo(unitset);
	
	if (iUSet > 6 ) return DBL_MIN;  //1967 tables not supported yet
	if (iUSet == 100)  return DBL_MIN + 1.0;  //unit string incorrect. Cant find unit No.
	
	return convertNamedUnit(if97_pq_h(convertNamedUnit (pressure,  arrUSets[iUSet][PRESS] , arrUSets[SIF][PRESS]), \
									  convertNamedUnit (quality,  arrUSets[iUSet][QUAL] , arrUSets[SIF][QUAL])), \
							arrUSets[SIF][ENTH], arrUSets[iUSet][ENTH]);

} // StmPQH



//  specific entropy for a given pressure and quality 
double StmPQS(double pressure, double quality, char* unitset){
	
	int iUSet = getUnitSetNo(unitset);
	
	if (iUSet > 6 ) return DBL_MIN;  //1967 tables not supported yet
	if (iUSet == 100)  return DBL_MIN + 1.0;  //unit string incorrect. Cant find unit No.
	
	return convertNamedUnit(if97_pq_s(convertNamedUnit (pressure,  arrUSets[iUSet][PRESS] , arrUSets[SIF][PRESS]), \
									  convertNamedUnit (quality,  arrUSets[iUSet][QUAL] , arrUSets[SIF][QUAL])), \
							arrUSets[SIF][ENTR], arrUSets[iUSet][ENTR]);

} // StmPQS



//  specific volume for a given pressure and quality
double StmPQV(double pressure, double quality, char* unitset){
	
	int iUSet = getUnitSetNo(unitset);
	
	if (iUSet > 6 ) return DBL_MIN;  //1967 tables not supported yet
	if (iUSet == 100)  return DBL_MIN + 1.0;  //unit string incorrect. Cant find unit No.
	
	return convertNamedUnit(if97_pq_v(convertNamedUnit (pressure,  arrUSets[iUSet][PRESS] , arrUSets[SIF][PRESS]), \
									  convertNamedUnit (quality,  arrUSets[iUSet][QUAL] , arrUSets[SIF][QUAL])), \
							arrUSets[SIF][VOL], arrUSets[iUSet][VOL]);

} // StmPQV



//  specific isobaric heat capacity Cp for a given pressure and quality
double StmPQC(double pressure, double quality, char* unitset){
	
	int iUSet = getUnitSetNo(unitset);
	
	if (iUSet > 6 ) return DBL_MIN;  //1967 tables not supported yet
	if (iUSet == 100)  return DBL_MIN + 1.0;  //unit string incorrect. Cant find unit No.
	
	return convertNamedUnit(if97_pq_Cp(convertNamedUnit (pressure,  arrUSets[iUSet][PRESS] , arrUSets[SIF][PRESS]), \
									  convertNamedUnit (quality,  arrUSets[iUSet][QUAL] , arrUSets[SIF][QUAL])), \
							arrUSets[SIF][SPEC_HEAT], arrUSets[iUSet][SPEC_HEAT]);

} // StmPQC



//  thermal conductivity for a given pressure and quality
double StmPQK(double pressure, double quality, char* unitset){
	
	int iUSet = getUnitSetNo(unitset);
	
	if (iUSet > 6 ) return DBL_MIN;  //1967 tables not supported yet
	if (iUSet == 100)  return DBL_MIN + 1.0;  //unit string incorrect. Cant find unit No.
	
	return convertNamedUnit(if97_pq_k(convertNamedUnit (pressure,  arrUSets[iUSet][PRESS] , arrUSets[SIF][PRESS]), \
									  convertNamedUnit (quality,  arrUSets[iUSet][QUAL] , arrUSets[SIF][QUAL])), \
							arrUSets[SIF][COND], arrUSets[iUSet][COND]);

} // StmPQK


//  dynamic viscosity for a given pressure and quality
double StmPQM(double pressure, double quality, char* unitset){
	
	int iUSet = getUnitSetNo(unitset);
	
	if (iUSet > 6 ) return DBL_MIN;  //1967 tables not supported yet
	if (iUSet == 100)  return DBL_MIN + 1.0;  //unit string incorrect. Cant find unit No.
	
	return convertNamedUnit(if97_pq_mu(convertNamedUnit (pressure,  arrUSets[iUSet][PRESS] , arrUSets[SIF][PRESS]), \
									  convertNamedUnit (quality,  arrUSets[iUSet][QUAL] , arrUSets[SIF][QUAL])), \
							arrUSets[SIF][VISC], arrUSets[iUSet][VISC]);

} // StmPQM



//  speed of sound for a given pressure and quality
double StmPQW(double pressure, double quality, char* unitset){
	
	int iUSet = getUnitSetNo(unitset);
	
	if (iUSet > 6 ) return DBL_MIN;  //1967 tables not supported yet
	if (iUSet == 100)  return DBL_MIN + 1.0;  //unit string incorrect. Cant find unit No.
	
	return convertNamedUnit(if97_pq_Vs(convertNamedUnit (pressure,  arrUSets[iUSet][PRESS] , arrUSets[SIF][PRESS]), \
									  convertNamedUnit (quality,  arrUSets[iUSet][QUAL] , arrUSets[SIF][QUAL])), \
							arrUSets[SIF][VEL], arrUSets[iUSet][VEL]);

} // StmPQW



//  dynamic viscosity for a given pressure and quality
double StmPQG(double pressure, double quality, char* unitset){
	
	int iUSet = getUnitSetNo(unitset);
	
	if (iUSet > 6 ) return DBL_MIN;  //1967 tables not supported yet
	if (iUSet == 100)  return DBL_MIN + 1.0;  //unit string incorrect. Cant find unit No.
	
	return convertNamedUnit(if97_pq_gamma(convertNamedUnit (pressure,  arrUSets[iUSet][PRESS] , arrUSets[SIF][PRESS]), \
									  convertNamedUnit (quality,  arrUSets[iUSet][QUAL] , arrUSets[SIF][QUAL])), \
							arrUSets[SIF][GAMMA], arrUSets[iUSet][GAMMA]);

} // StmPQH





// MISCELLANEOUS

//  converts ITS68 temperature to ITS90
double StmTEMPIT(double temp68, char* unitset);

//  converts ITS90 temperature to ITS68
double StmTEMPTI(double temp90, char* unitset) ;

// text StmVER(void);

*/






#endif // IF97_WINSTEAM_H

