//          Copyright Martin Lord 2014-2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


//    IAPWS-IF97 Region 3: low temperature supercritical and metastable region equations
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
  * use the -fopenmp compile flag to enable multithreadded code
  * 
  * ****************************************************************** */
   

#include "IF97_common.h"  //PSTAR TSTAR & sqr
#include "IF97_Region3.h"
#include "IF97_Region3bw.h"
#include <math.h> // for pow, log

#include <stdio.h>  //used for debugging only


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
double if97_r3_Phi (double if97_delta, double  if97_tau) {
	
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
	
		dblPhiSum += PHI_COEFFS_R3[i].ni * pow( if97_delta, PHI_COEFFS_R3[i].Ii) * PHI_COEFFS_R3[i].Ji 
						* (PHI_COEFFS_R3[i].Ji  - 1.0) * pow(if97_tau, PHI_COEFFS_R3[i].Ji  - 2.0);
		}		 
return   dblPhiSum;
}




// [d sqr phi / d delta d Tau] 
double if97_r3_PhiDeltaTau(double if97_delta, double if97_tau) {
	
	int i;
	double dblPhiSum = 0.0;
	
	
	#pragma omp parallel for reduction(+:dblPhiSum) 	//handle loop multithreaded
	for (i=2; i <= MAX_COEFFS_PHI_R3 ; i++) { 
	
		dblPhiSum +=   PHI_COEFFS_R3[i].ni *  PHI_COEFFS_R3[i].Ii  * pow (if97_delta, PHI_COEFFS_R3[i].Ii -1) 
							* PHI_COEFFS_R3[i].Ji * pow (if97_tau, PHI_COEFFS_R3[i].Ji - 1.0) ;
		}		 
	
return   dblPhiSum;
}






//**********************************************************
//********* REGION 3 PROPERTY EQUATIONS*********************

// specific Helmholz free energy in region 3 (kJ / kg)
double if97_r3_hhz (double rho_kgPerM3 , double t_Kelvin) {  
	
	double if97delta = rho_kgPerM3 / IF97_RHOC;
	double if97tau = IF97_TC / t_Kelvin;
	
return IF97_R * t_Kelvin * if97_r3_Phi(if97delta, if97tau);
}



// pressure (MPa) in region 3 for a given density (kg/m3) and temperature (K)
double if97_r3_p (double rho_kgPerM3 , double t_Kelvin ) {
		
	double if97delta = rho_kgPerM3 / IF97_RHOC;
	double if97tau =  IF97_TC / t_Kelvin;
	
return  0.001 * rho_kgPerM3 *  IF97_R * t_Kelvin * if97delta * if97_r3_PhiDelta(if97delta, if97tau);  // factor of 1000 because R  needs to go from kJ/kg to J/kg
}	


// specific internal energy (kJ/kg) in region 3 for a given density (kg/m3) and temperature (K)
double if97_r3_u (double rho_kgPerM3 , double t_Kelvin ) {
		
	double if97delta = rho_kgPerM3 / IF97_RHOC;
	double if97tau = IF97_TC / t_Kelvin;
	
return IF97_R * t_Kelvin * if97tau * if97_r3_PhiTau(if97delta, if97tau);
}	


// specific entropy (kJ/kg K) in region 3 for a given density (kg/m3) and temperature (K)
double if97_r3_s (double rho_kgPerM3 , double t_Kelvin ) {
		
	double if97delta = rho_kgPerM3 / IF97_RHOC;
	double if97tau = IF97_TC / t_Kelvin;
	
return IF97_R * ( if97tau * if97_r3_PhiTau(if97delta, if97tau) - if97_r3_Phi(if97delta, if97tau)) ;
}	


// specific enthalpy (kJ/kg) in region 3 for a given density (kg/m3) and temperature (K)
double if97_r3_h (double rho_kgPerM3 , double t_Kelvin ) {
		
	double if97delta = rho_kgPerM3 / IF97_RHOC;
	double if97tau = IF97_TC / t_Kelvin;
	
return IF97_R * t_Kelvin * ( if97tau * if97_r3_PhiTau(if97delta, if97tau) + if97delta * if97_r3_PhiDelta(if97delta, if97tau)) ;
}	



// specific isochoric heat capacity Cv (kJ/kg K) in region 3 for a given density (kg/m3) and temperature (K)
double if97_r3_Cv (double rho_kgPerM3 , double t_Kelvin ) {
		
	double if97delta = rho_kgPerM3 / IF97_RHOC;
	double if97tau =  IF97_TC / t_Kelvin;
	
return  - IF97_R * ( sqr (if97tau ) * if97_r3_PhiTauTau(if97delta, if97tau) ) ;
}	



// specific isobaric heat capacity Cp (kJ/kg K) in region 3 for a given density (kg/m3) and temperature (K)
double if97_r3_Cp (double rho_kgPerM3 , double t_Kelvin ) {
		
	double if97delta = rho_kgPerM3 / IF97_RHOC;
	double if97tau = IF97_TC / t_Kelvin;
	
		
return  IF97_R * (-sqr(if97tau) * if97_r3_PhiTauTau(if97delta, if97tau) 
		
		+  sqr (if97delta * if97_r3_PhiDelta(if97delta, if97tau) - if97delta * if97tau * if97_r3_PhiDeltaTau(if97delta, if97tau)) 
			/ (2.0 * if97delta * if97_r3_PhiDelta(if97delta, if97tau) + sqr(if97delta) * if97_r3_PhiDeltaDelta(if97delta, if97tau))
					);
}	



// speed of sound w (m/s) in region 3 for a given density (kg/m3) and temperature (K)
double if97_r3_w (double rho_kgPerM3 , double t_Kelvin ) {
		
	double if97delta = rho_kgPerM3 / IF97_RHOC;
	double if97tau =  IF97_TC / t_Kelvin;
	
	double part1 = 2.0 * if97delta * if97_r3_PhiDelta( if97delta, if97tau) ;
	
	double part2 = sqr(if97delta) * if97_r3_PhiDeltaDelta(if97delta, if97tau);
	
	double part3num = sqr (if97delta * if97_r3_PhiDelta(if97delta, if97tau) 
			- if97delta * if97tau * if97_r3_PhiDeltaTau(if97delta, if97tau)) ;
	
	
	double part3denom = sqr(if97tau) * if97_r3_PhiTauTau(if97delta, if97tau);
	
  return  sqrt(IF97_R * 1000.0 * t_Kelvin * (part1 + part2 - part3num / part3denom)) ;  // 1000 because R in in KJ / Kg.K  not J / Kg.K 
}	




// TODO Phase Equilibrium equations from table 31



/*  TODO: Joule Thompson Coefficient, isothermal throttling coefficient
 *  and isentropic temperature - pressure coefficient in the Moscow Power 
 * Institute page for region 3
 * 
 * http://twt.mpei.ac.ru/mcs/worksheets/iapws/IAPWS-IF97-Region3.xmcd
 */




