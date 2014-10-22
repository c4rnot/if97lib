
//          Copyright Martin Lord 2014-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


//    IAPWS-IF97 constants used throughout the formulation

#ifndef IF97_CONSTS_H
#define IF97_CONSTS_H


// ********REGION BOUNDARY CONSTANTS**************//

	#define IF97_R1_LTEMP 273.15  // K
	#define IF97_R1_UTEMP 623.15  // K
	// #define IF97_R2_LTEMP   // doesnt exist.  Use B23 equation
	#define IF97_R2_UTEMP 1073.15  // K
	#define IF97_R3_LTEMP 623.15 // K
	// #define IF97_R3_UTEMP   // doesnt exist.  Use B23 equation
	// Region 4 is a line with its own equation
	#define IF97_R5_LTEMP 1073.15  // K
	#define IF97_R5_UTEMP 2273.15  // K
	#define IF97_B23_LTEMP 623.15 // K
	#define IF97_B23_UTEMP 863.15 // K
	
	
	#define IF97_R1_LPRESS 0.0  // MPa
	#define IF97_R1_UPRESS 100.0  // MPa
	#define IF97_R2_LPRESS 0.0  // MPa
	#define IF97_R2_UPRESS 100.0  // MPa
	//  #define IF97_R3_LPRESS 0.0  // MPa  // doesnt exist.  Use B23 equation
	#define IF97_R3_UPRESS 100.0  // MPa
	#define IF97_R4_LPRESS 0.0  // MPa
	// #define IF97_R4_UPRESS   // MPa // Region 4 is a line with its own equation
	#define IF97_R5_LPRESS 0.0  // MPa
	#define IF97_R5_UPRESS 50.0  // MPa
	#define IF97_B23_LPRESS 16.592 // MPa
	#define IF97_B23_UPRESS 100.0 // MPa



// ********MAIN REFERENCE CONSTANTS**************//

	#define IF97_R 0.461526  // KJ / Kg.K 	Specific gas constant for ordinary 'water'
	
	#define IF97_TC 647.096  //  K  		Critical Temperature
	#define IF97_PC 22.064 //  MPa			Critical Pressure
	#define IF97_RHOC 322.0  //  kg / m3	Critical Density
	
	#define PSTAR_R1  16.53  // MPa
	#define TSTAR_R1  1386.0  // K
	
	#define TSTAR_R2 540.0 // K
	#define PSTAR_R2 1.0 // MPa
	
	#define TSTAR_R2_BW 1.0 // K
	#define PSTAR_R2_BW 1.0 //MPa
	#define HSTAR_R2_BW 2000.0 //MPa	
	
	#define SSTAR_R2A_BW 2.0 // kJ/kW/K
	#define SSTAR_R2B_BW 0.7853 // kJ/kW/K
	#define SSTAR_R2C_BW 2.9251 // kJ/kW/K
	
	
	#define PSTAR_R5 1.0  //MPa
	#define TSTAR_R5 1000.0 //K
	
// ********COMMON DATA STRUCTURES **************//	
	
typedef struct sctIF97Coeffs_IJn {
	int Ii;
	int Ji;
	double ni;
} typIF97Coeffs_IJn;
	
	
	
// ********COMMON FUNCTIONS**************//	

// squares a double without using pow
double sqr (double dblArg);

// cubes a double without using pow
double cube (double dblArg);



#endif // IF97_CONSTS_H
