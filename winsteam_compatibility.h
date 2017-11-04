//          Copyright Martin Lord 2014-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


/*
 * Winsteam equivilent function calls
 */

/**
 * @copyright
 * Copyright Martin Lord 2014-2014. \n
 * Distributed under the Boost Software License, Version 1.0. \n
 * (See accompanying file LICENSE_1_0.txt or copy at \n
 * http://www.boost.org/LICENSE_1_0.txt) \n
 * 
 * @file IF97_Region1.h
 * @author Martin Lord
 * @date 07 Nov 2014
 * @brief Winsteam equivilent function calls
 *
 * 
 */



#ifndef IF97_WINSTEAM_H
#define IF97_WINSTEAM_H

/**
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
 * Dyn Visc	| lbm/ft/h  | Centipoise| lbm/ft/h  |  Pa.s     | Centipoise| Centipoise| Centipoise|
 * Gamma    |      -    |      -    |      -    |      -    |      -    |      -    |      -    |
 * sonic vel|   ft/s    |    m/s    |   ft/s    |    m/s    |    m/s    |    m/s    |    m/s    |
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>  // for tolower
#include "units.h"  // unit conversion library
#include <float.h>  //for minimum number storable for each type
#include "if97_lib.h" // IF 97 steam tables in MPa, K, kg, kJ 

#define UNITSTRLEN 5


// SATURATION LINE


/** saturation temperature for a given pressure */
double StmPT(double pressure, char* unitset);

/** saturation pressure for a given temperatrue */
double StmTP(double temperature, char* unitset);


// PT

/** specific enthalpy for a given pressure and temperature */
double StmPTH(double pressure, double temperature, char* unitset);

/** specific entropy for a given pressure and temperature */
double StmPTS(double pressure, double temperature, char* unitset);

/** specific volume for a given pressure and temperature */
double StmPTV(double pressure, double temperature , char* unitset);

/** specific isobaric heat capacity for a given pressure and temperature */
double StmPTC(double pressure, double temperature, char* unitset);

/** thermal conductivity for a given pressure and temperature */
double StmPTK(double pressure, double temperature, char* unitset);

/** dynamic viscosity for a given pressure and temperature */
double StmPTM(double pressure, double temperature, char* unitset);

/** speed of sound for a given pressure and temperature */
double StmPTW(double pressure, double temperature, char* unitset);

/** isentropic expansion coefficient for a given pressure and temperature */
double StmPTG(double pressure, double temperature, char* unitset);


// PH

/** temperature for a given pressure and enthalpy */
double StmPHT(double pressure, double enthalpy, char* unitset);

/** specific entropy for a given pressure and enthalpy */
double StmPHS(double pressure, double enthalpy, char* unitset);

/** specific volume for a given pressure and enthalpy */
double StmPHV(double pressure, double enthalpy, char* unitset);

/** quality for a given pressure and enthalpy */
double StmPHQ(double pressure, double enthalpy, char* unitset);

/** specific heat capacity #TODO - Which one# for a given pressure and enthalpy */
double StmPHC(double pressure, double enthalpy, char* unitset);

/** speed of sound for a given pressure and enthalpy */
double StmPHW(double pressure, double enthalpy, char* unitset);

/** isentropic expansion coefficient for a given pressure and enthalpy */
double StmPHG(double pressure, double enthalpy, char* unitset);


// PS

/** temperature for a given pressure and entropy */
double StmPST(double pressure, double entropy);

/** specific enthalpy for a given pressure and entropy */
double StmPSH(double pressure, double entropy);

/** specific volume for a given pressure and entropy */
double StmPSV(double pressure, double entropy);

/** quality for a given pressure and entropy */
double StmPSQ(double pressure, double entropy);

/** specific heat capacity #TODO - Which one# for a given pressure and entropy */
double StmPSC(double pressure, double entropy);

/** speed of sound for a given pressure and entropy */
double StmPSW(double pressure, double entropy);

/** isentropic expansion coefficient for a given pressure and entropy */
double StmPSG(double pressure, double entropy);


// TQ

/** specific enthalpy for a given temperature and quality */
double StmTQH(double temperature, double quality);

/** specific entropy for a given temperature and quality */
double StmTQS(double temperature, double quality);

/** specific volume for a given temperature and quality */
double StmTQV(double temperature, double quality);

/** specific heat capacity #TODO - Which one#  for a given temperature and quality */
double StmTQC(double temperature, double quality);

/** thermal conductivity for a given temperature and quality */
double StmTQK(double temperature, double quality);

/** dynamic viscosity for a given temperature and quality */
double StmTQM(double temperature, double quality);

/** sonic speed for a given temperature and quality */
double StmTQW(double temperature, double quality);

/** isentropic expansion coefficient # TODO - what is this? # for a given temperature and quality */
double StmTQG(double temperature, double quality);

//PQ

/** specific enthalpy for a given pressure and quality */
double StmPQH(double pressure, double quality);

/** specific entropy for a given pressure and quality */
double StmPQS(double pressure, double quality);

/** specific volume for a given pressure and quality */
double StmPQV(double pressure, double quality);

/** specific heat capacity #TODO - which one?# for a given pressure and quality */
double StmPQC(double pressure, double quality);

/** thermal conductivity for a given pressure and quality */
double StmPQK(double pressure, double quality);


/** dynamic viscosity for a given pressure and quality */
double StmPQM(double pressure, double quality);

/** speed of sound for a given pressure and quality */
double StmPQW(double pressure, double quality);

/** dynamic viscosity for a given pressure and quality */
double StmPQG(double pressure, double quality);


// MISCELLANEOUS

/** converts ITS68 temperature to ITS90 */
double StmTEMPIT(double temp68);

/** converts ITS90 temperature to ITS68 */
double StmTEMPTI(double temp90);

// text StmVER(void);

/** get the integer value of the unit set calls for 67 tables  are +10*/
int getUnitSetNo (char *unitset);

#endif // IF97_WINSTEAM_H

