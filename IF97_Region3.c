//          Copyright Martin Lord 2014-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


//    IAPWS-IF97 Region 3: vapour: saturation to critical region equations
/* *********************************************************************
 * *******             VALIDITY                             ************
 * 623.15 K <=T <= T ( p ) [B23 temperature equation]
 * p ( T ) [B23 temperature equation] <= p <= 100 MPa .
 * 
 * In addition, region 3 yields reasonable values inthe metastable regions
 * (superheated liquid and subcooleed steam close to the saturated liquid
 * and saturated vapor line
 * 
 * ****************************************************************** */
 
 /* ********************************************************************
  *         COMPILE AND LINK INSTRUCTIONS    (gcc)                          *
  * 
  * This library uses math.h, so must have the -lm  link flag
  * 
  * The library is programmed to be able to use OpenMP multithreading   
  * use the -fopenmp complie flag to enable multithreadded code
  * 
  * ****************************************************************** */
   

#include "IF97_common.h"  //PSTAR TSTAR & sqr
#include "IF97_Region3.h"
#include <math.h> // for pow, log
/* #ifdef _OPENMP // multithreading via libgomp
 # include <omp.h>
 #endif
*/
// #include <stdio.h>  //used for debugging only


//***************************************************************
//****** REGION 3 HELMHOLTZ FREE ENERGY AND DERIVATIVES**************


	
const typIF97Coeffs_IJn  PHI_COEFFS_R3[] = {
	{0,    0,	 0.0} 				   //0  i starts at 1, so 0th i is not used
	,{ 0,    0,    0.10658070028513E1}	// "0, 0" are "-, -" in the standard
	,{ 0,    0,    -0.15732845290239E2}
	,{ 0,    1,    0.20944396974307E2}
	,{ 0,    2,    -0.76867707878716E1}
	,{ 0,    7,    0.26185947787954E1}
	,{ 0,    10,    -0.28080781148620E1}
	,{ 0,    12,    0.12053369696517E1}
	,{ 0,    23,    -0.84566812812502E-2}
	,{ 1,    2,    -0.12654315477714E1}
	,{ 1,    6,    -0.11524407806681E1}
	,{ 1,    15,    0.88521043984318}
	,{ 1,    17,    -0.64207765181607}
	,{ 2,    0,    0.38493460186671}
	,{ 2,    2,    -0.85214708824206}
	,{ 2,    6,    0.48972281541877E1}
	,{ 2,    7,    -0.30502617256965E1}
	,{ 2,    22,    0.39420536879154E-1}
	,{ 2,    26,    0.12558408424308}
	,{ 3,    0,    -0.27999329698710}
	,{ 3,    2,    0.13899799569460E1}
	,{ 3,    4,    -0.20189915023570E1}
	,{ 3,    16,    -0.82147637173963E-2}
	,{ 3,    26,    -0.47596035734923}
	,{ 4,    0,    0.43984074473500E-1}
	,{ 4,    2,    -0.44476435428739}
	,{ 4,    4,    0.90572070719733}
	,{ 4,    26,    0.70522450087967}
	,{ 5,    1,    0.10770512626332}
	,{ 5,    3,    -0.32913623258954}
	,{ 5,    26,    -0.50871062041158}
	,{ 6,    0,    -0.22175400873096E-1}
	,{ 6,    2,    0.94260751665092E-1}
	,{ 6,    26,    0.16436278447961}
	,{ 7,    2,    -0.13503372241348E-1}
	,{ 8,    26,    -0.14834345352472E-1}
	,{ 9,    2,    0.57922953628084E-3}
	,{ 9,    26,    0.32308904703711E-2}
	,{ 10,    0,    0.80964802996215E-4}
	,{ 10,    1,    -0.16557679795037E-3}
	,{ 11,    26,    -0.44923899061815E-4}
};

const int MAX_COEFFS_PHI_R3 = 40;

// dimensionless helmholz free energy in Region3 :   See Equation 28
double if97_r3_phi (double if97_delta, double  if97_tau) {
	
	int i;
	double dblPhiSum = 0.0;
	
	
	#pragma omp parallel for reduction(+:dblPhiSum) 	//handle loop multithreaded
	for (i=2; i <= MAX_COEFFS_PHI_R3 ; i++) {
	
		dblPhiSum +=   PHI_COEFFS_R3[i].ni *  pow(if97_delta, PHI_COEFFS_R3[i].Ii)*  pow((if97_tau ), PHI_COEFFS_R3[i].Ji)	;
		}		 
	
return  PHI_COEFFS_R3[1].ni * log(if97_delta) + dblPhiSum;
}





// [d phi / d delta] keeping tau constant
double if97_r3_PhiDelta(double if97_delta, double if97_tau) {
	
	int i;
	double dblPhiSum = 0.0;
	
	
	#pragma omp parallel for reduction(+:dblPhiSum) 	//handle loop multithreaded
	for (i=2; i <= MAX_COEFFS_PHI_R3 ; i++) {
	
		dblPhiSum +=   PHI_COEFFS_R3[i].ni *   PHI_COEFFS_R3[i].Ii *  pow(if97_delta , PHI_COEFFS_R3[i].Ii - 1.0 ) *  pow( if97_tau, PHI_COEFFS_R3[i].Ji)	;
		}		 
	
return  PHI_COEFFS_R3[1].ni /if97_delta + dblPhiSum;
}



// [d phi / d tau] keeping delta constant
double if97_r3_PhiTau(double if97_delta, double if97_tau) {
	
	int i;
	double dblPhiSum = 0.0;
	
	
	#pragma omp parallel for reduction(+:dblPhiSum) 	//handle loop multithreaded
	for (i=2; i <= MAX_COEFFS_PHI_R3 ; i++) {
	
		dblPhiSum +=   PHI_COEFFS_R3[i].ni *  pow(if97_delta ,  PHI_COEFFS_R3[i].Ii) *  PHI_COEFFS_R3[i].Ji *  pow( if97_tau, PHI_COEFFS_R3[i].Ji - 1.0)	;
		}		 
	
return  dblPhiSum;
}


// [d phi / d delta sqr] keeping tau constant
double if97_r3_PhiDeltaDelta(double if97_delta, double if97_tau) {
	
	int i;
	double dblPhiSum = 0.0;
	
	
	#pragma omp parallel for reduction(+:dblPhiSum) 	//handle loop multithreaded
	for (i=2; i <= MAX_COEFFS_PHI_R3 ; i++) {
	
		dblPhiSum +=   PHI_COEFFS_R3[i].ni *   PHI_COEFFS_R3[i].Ii * (PHI_COEFFS_R3[i].Ii -1 ) * pow(if97_delta , PHI_COEFFS_R3[i].Ii - 2.0 ) *  pow( if97_tau, PHI_COEFFS_R3[i].Ji)	;
		}		 
	
return  - PHI_COEFFS_R3[1].ni / sqr(if97_delta) + dblPhiSum;
}



// [d phi / d Tau sqr] keeping delta constant
double if97_r3_PhiTauTau(double if97_delta, double if97_tau) {
	
	int i;
	double dblPhiSum = 0.0;
	
	
	#pragma omp parallel for reduction(+:dblPhiSum) 	//handle loop multithreaded
	for (i=2; i <= MAX_COEFFS_PHI_R3 ; i++) {
	
		dblPhiSum +=   PHI_COEFFS_R3[i].ni *   pow (if97_delta, PHI_COEFFS_R3[i].Ii )  *   PHI_COEFFS_R3[i].Ji * (PHI_COEFFS_R3[i].Ji - 1.0) *  (PHI_COEFFS_R3[i].Ii -1 ) *  pow( if97_tau, PHI_COEFFS_R3[i].Ji - 2.0)	;
		}		 
	
return   dblPhiSum;
}




// [d sqr phi / d delta d Tau] 
double if97_r3_PhiDeltaTau(double if97_delta, double if97_tau) {
	
	int i;
	double dblPhiSum = 0.0;
	
	
	#pragma omp parallel for reduction(+:dblPhiSum) 	//handle loop multithreaded
	for (i=2; i <= MAX_COEFFS_PHI_R3 ; i++) { 
	
		dblPhiSum +=   PHI_COEFFS_R3[i].ni *  PHI_COEFFS_R3[i].Ii  * pow (if97_delta, PHI_COEFFS_R3[i].Ii -1) * PHI_COEFFS_R3[i].Ji * pow (if97_tau, PHI_COEFFS_R3[i].Ji - 1.0) ;
		}		 
	
return   dblPhiSum;
}






