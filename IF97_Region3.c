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
#include "IF97_Region4.h" 
#include "IF97_B23.h"   // not strictly required but used in v(p,t) subregion selector as an error detector
#include <math.h> // for pow, log
/* #ifdef _OPENMP // multithreading via libgomp
 # include <omp.h>
 #endif
*/
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



/*  TODO:  What are Joule Thompson Coefficient, isothermal throttling coefficient
 *  and isentropic temperature - pressure coefficient in the Moscow Power 
 * Institute page for region 3?
 * 
 * http://twt.mpei.ac.ru/mcs/worksheets/iapws/IAPWS-IF97-Region3.xmcd
 */



/* **********************************************************
********* REGION 3 BACKWARDS EQUATIIONS v(p,t) **************
* 
* Revised Supplementary Release on Backward Equations for Specific Volume 
* as a Function of Pressure and Temperature v(p,T) for Region 3 of the 
* IAPWS Industrial Formulation 1997 for the Thermodynamic Properties
* of Water and Steam.
* 
*  
* http://iapws.org/relguide/Supp-VPT3-2014.pdf
*/


// The following sets of coefficients from table 1.

typIF97Coeffs_Jn T3AB_P_R3_COEFFS[] = {
	{0,	 0.0} 				   //0  i starts at 1, so 0th i is not used
	,{0,	0.154793642129415e04}	
	,{1,    -0.187661219490113e03}
	,{2,	0.213144632222113e02}
	,{-1,	-0.191887498864292e04}
	,{-2,	0.918419702359447e03}
};


typIF97Coeffs_Jn T3CD_P_R3_COEFFS[] = {
	{0,	 0.0} 				   //0  i starts at 1, so 0th i is not used
	,{0,   0.585276966696349e03}	
	,{1,    0.278233532206915e01}
	,{2,	-0.127283549295878e-1}
	,{3,	0.159090746562729e-3}
};



typIF97Coeffs_Jn T3GH_P_R3_COEFFS[] = {
	{0,	 0.0} 				   //0  i starts at 1, so 0th i is not used
	,{0,   -0.249284240900418e05}	
	,{1,    0.428143584791546e04}
	,{2,	-0.269029173140130e03}
	,{3,	0.751608051114157e01}
	,{4,	-0.787105249910383e-1}
};



typIF97Coeffs_Jn T3IJ_P_R3_COEFFS[] = {
	{0,	 0.0} 				   //0  i starts at 1, so 0th i is not used
	,{0,	0.584814781649163e03}	
	,{1,    -0.616179320924617e00}
	,{2,	0.260763050899562e00}
	,{3,	-0.587071076864459e-2}
	,{4,	0.515308185433082e-4}
};



typIF97Coeffs_Jn T3JK_P_R3_COEFFS[] = {
	{0,	 0.0} 				   //0  i starts at 1, so 0th i is not used
	,{0,	0.617229772068439e03}	
	,{1,    -0.770600270141675e01}
	,{2,	0.697072596851896e00}
	,{3,	-0.157391839848015e-1}
	,{4,	0.137897492684194e-3}
};



typIF97Coeffs_Jn T3MN_P_R3_COEFFS[] = {
	{0,	 0.0} 				   //0  i starts at 1, so 0th i is not used
	,{0,	0.535339483742384e03}	
	,{1,    0.761978122720128e01}
	,{2,	-0.158365725441648e00}
	,{3,	0.192871054508108e-2}
};



typIF97Coeffs_Jn T3OP_P_R3_COEFFS[] = {
	{0,	 0.0} 				   //0  i starts at 1, so 0th i is not used
	,{0,	0.969461372400213e03}	
	,{1,    -0.332500170441278e03}
	,{2,	0.642859598466067e02}
	,{-1,	0.773845935768222e03}
	,{-2,	-0.152313732937084e04}
};


typIF97Coeffs_Jn T3QU_P_R3_COEFFS[] = {
	{0,	 0.0} 				   //0  i starts at 1, so 0th i is not used
	,{0,	0.565603648239126e03}	
	,{1,    0.529062258221122e01}
	,{2,	-0.102020639611016e00}
	,{3,	0.122240301070145e-2}
};



typIF97Coeffs_Jn T3RX_P_R3_COEFFS[] = {
	{0,	 0.0} 				   //0  i starts at 1, so 0th i is not used
	,{0,	0.584561202520006e03}	
	,{1,    -0.102961025163669e01}
	,{2,	0.243293362700452e00}
	,{3,	-0.294905044740799e-2}
};



bool isNearCritical(double p_MPa, double t_K){

//TODO
return -1;	
};

//  Region 3a/3b boundary. see equation 2.  critical isentrope from 25MPa to 100 MPa
double if97_r3ab_p_t (double p_MPa){
	int i;
	double dblPhiSum = 0.0;

	for (i=1; i <= (int)(sizeof(T3AB_P_R3_COEFFS)/sizeof(typIF97Coeffs_Jn) -1 ) ; i++) {
		dblPhiSum += (double)T3AB_P_R3_COEFFS[i].ni  * pow( log(p_MPa), T3AB_P_R3_COEFFS[i].Ji );
	}	
	
return dblPhiSum;
};

// Region 3c/3d boundary.  See equation 1. valid: 25 - 40 MPa
double if97_r3cd_p_t (double p_MPa){
	int i;
	double dblPhiSum = 0.0;
	
	for (i=1; i <= (int)(sizeof(T3CD_P_R3_COEFFS)/sizeof(typIF97Coeffs_Jn) -1 ) ; i++) {
		dblPhiSum += T3CD_P_R3_COEFFS[i].ni * pow( p_MPa, T3CD_P_R3_COEFFS[i].Ji)	;
	}	
	
return dblPhiSum;
};


// Region 3e/3f boundary.  see equation 3.  valid 22.5 - 40 MPa
double if97_r3ef_p_t (double p_MPa){
	
	const double DPhiDPi = 3.727888004;
	
return 	DPhiDPi * (p_MPa- 22.064) + 647.096;
};


// Region 3g/3h boundary.  See equation 1. Valid 22.5 - 25 MPa
double if97_r3gh_p_t (double p_MPa){
	int i;
	double dblPhiSum = 0.0;
	
	for (i=1; i <= (int)(sizeof(T3GH_P_R3_COEFFS)/sizeof(typIF97Coeffs_Jn) -1 ) ; i++) {
		dblPhiSum += T3GH_P_R3_COEFFS[i].ni * pow( p_MPa, T3GH_P_R3_COEFFS[i].Ji)	;
	}	
	
return dblPhiSum;
};



// Region 3i/3j boundary. See equation 1.  valid 22.5 - 25 MPa  ~v= 0.0041 m3/kg
double if97_r3ij_p_t (double p_MPa){
	int i;
	double dblPhiSum = 0.0;
	
	for (i=1; i <= (int)(sizeof(T3GH_P_R3_COEFFS)/sizeof(typIF97Coeffs_Jn) -1 ) ; i++) {
		dblPhiSum += T3IJ_P_R3_COEFFS[i].ni * pow( p_MPa, T3IJ_P_R3_COEFFS[i].Ji)	;
	}	
	
return dblPhiSum;
};



// Region 3j/3k boundary. See equation 1. Valid 20.5 - 25 MPa.  ~ v = v"(20.5 MPa)
double if97_r3jk_p_t (double p_MPa){
	int i;
	double dblPhiSum = 0.0;
	
	for (i=1; i <= (int)(sizeof(T3JK_P_R3_COEFFS)/sizeof(typIF97Coeffs_Jn) -1 ) ; i++) {
		dblPhiSum += T3JK_P_R3_COEFFS[i].ni * pow( p_MPa, T3JK_P_R3_COEFFS[i].Ji)	;
	}	
	
return dblPhiSum;
};


// Region 3m/3n boundary. See equation 1. valid: 22.5 - 23 MPa. ~v=0.0028 m3/kg 
double if97_r3mn_p_t (double p_MPa){
	int i;
	double dblPhiSum = 0.0;
	
	for (i=1; i <= (int)(sizeof(T3MN_P_R3_COEFFS)/sizeof(typIF97Coeffs_Jn) -1 ) ; i++) {
		dblPhiSum += T3MN_P_R3_COEFFS[i].ni * pow( p_MPa, T3MN_P_R3_COEFFS[i].Ji)	;
	}	
	
return dblPhiSum;
};



// Region 3o/3p boundary. see equation 2. valid: 22.5 - 23 MPa. ~v=0.0034 m3/kg 
double if97_r3op_p_t (double p_MPa){
	int i;
	double dblPhiSum = 0.0;
	
	for (i=1; i <= (int)(sizeof(T3OP_P_R3_COEFFS)/sizeof(typIF97Coeffs_Jn) -1 ) ; i++) {
		dblPhiSum += T3OP_P_R3_COEFFS[i].ni * pow( log(p_MPa), T3OP_P_R3_COEFFS[i].Ji)	;
	}	
	
return dblPhiSum;
};


// Region 3q/3u boundary. See equation 1. valid: Psat(643.15 K) - 22.5 MPa
double if97_r3qu_p_t (double p_MPa){
	int i;
	double dblPhiSum = 0.0;
	
	for (i=1; i <= (int)(sizeof(T3QU_P_R3_COEFFS)/sizeof(typIF97Coeffs_Jn) -1 ) ; i++) {
		dblPhiSum += T3QU_P_R3_COEFFS[i].ni * pow( p_MPa, T3QU_P_R3_COEFFS[i].Ji)	;
	}	
	
return dblPhiSum;
};



// Region 3r/3x boundary. See equation 1.  valid: Psat(643.15 K) - 22.5 MPa
double if97_r3rx_p_t (double p_MPa){
	int i;
	double dblPhiSum = 0.0;
	
	for (i=1; i <= (int)(sizeof(T3RX_P_R3_COEFFS)/sizeof(typIF97Coeffs_Jn) -1 ) ; i++) {
		dblPhiSum += T3RX_P_R3_COEFFS[i].ni * pow( p_MPa, T3RX_P_R3_COEFFS[i].Ji)	;
	}	
	
return dblPhiSum;
};



// Region 1=3a, 2=3b, 3=3c etc...    100 = near critical   0 = error: not region 3
// this function assumes you are already know you are in region 3
// see table 2
int if97_r3_pt_subregion(double p_MPa, double t_K){
	const double P3_CD = 1.900881189173929e01; // bottom of table 2
	
	// First, make sure we are in R3
	if ((p_MPa > IF97_R3_UPRESS) || (p_MPa < IF97_B23_LPRESS)) return 0 ; // not R3
	else if ((t_K < IF97_R3_LTEMP)  ||  (IF97_B23T(p_MPa) < t_K)) return 0 ;  // not R3
	
	// now check through table 2 by pressure range
	if (p_MPa > 40.0) {   // already checked below 100 MPa (R3_UPRESS)
		if (t_K >  if97_r3ab_p_t(p_MPa)) return 2;  //3b
		else return 1; //3a
	}
	
	else if (p_MPa > 25.0) {   // already checked below or equal to 40 MPa 
		if (if97_r3ab_p_t(p_MPa)  < t_K ){ // right of de/ab line fig 3
			if (t_K <= if97_r3ef_p_t(p_MPa)) return 5;  //3e
			else return 6;  // 3f
		}	
		// now on or left of de/ab line
		else if (t_K <= if97_r3cd_p_t(p_MPa)) return 3;  //3c
		else return 4;  // 3d		
	}
	
	else if (p_MPa > 23.0) {   // already checked below or equal to 25 MPa 
		if (if97_r3ef_p_t(p_MPa) < t_K ){ // right of ef line fig 4
			if (t_K > if97_r3jk_p_t(p_MPa)) return 11;  //3k
			else if (t_K > if97_r3ij_p_t(p_MPa)) return 10; //3j
			else return 9;  //3i
		}	
		// now on or left of ef line
		else if (t_K <= if97_r3cd_p_t(p_MPa)) return 3;  //3c
		else if (t_K <= if97_r3gh_p_t(p_MPa)){
			if (p_MPa > 23.5) return 7;  // 3g   23.5 MPa < p <= 25 MPa
			else return 12; // 3l :  23 MPa < p <= 23.5 MPa
		}
		else return 8;	// 3h
	}	

	else if (p_MPa > 22.5) {   // already checked below or equal to 23.0 MPa 
		if (if97_r3ef_p_t(p_MPa) < t_K ){ // right of ef line fig 4
			if (t_K > if97_r3jk_p_t(p_MPa)) return 11;  //3k
			else if (t_K > if97_r3ij_p_t(p_MPa)) return 10; //3j
			else if (t_K > if97_r3op_p_t(p_MPa)) return 16; //3p
			else return 15;  //3o
		}	
		// now on or left of ef line
		else if (t_K <= if97_r3cd_p_t(p_MPa)) return 3;  //3c
		else if (t_K <= if97_r3gh_p_t(p_MPa)) return 12;  // 3l
		else if (t_K <= if97_r3mn_p_t(p_MPa)) return 13;  //3m
		else return 14;	// 3n
	}	


	else if (p_MPa > if97_r4_ps(643.15)) {   // above Psat(643.15 K) to  below or equal 22.5 MPa 
		if (if97_r3rx_p_t(p_MPa)  < t_K ){ // right of rx line fig 4
			if (t_K > if97_r3jk_p_t(p_MPa)) return 11;  //3k
			else return 18;  //3r
		}	
		else if ( t_K <= if97_r3qu_p_t(p_MPa)){ // on or left of of qu line fig 4
			if (t_K <= if97_r3cd_p_t(p_MPa)) return 3;  //3c
			else return 17;  //3q
		}			
		else return 100;	// near supercritical region
	}	


	else if (p_MPa > 20.5) {   // above 20.5 to  below or equal Psat(643.15 K)
		if (if97_r4_ts(p_MPa) <= t_K ){ // on or right of saturation line fig 3
			if (t_K > if97_r3jk_p_t(p_MPa)) return 11;  //3k
			else return 18;  //3r
		}	
		
		// left of saturation line fig 3
		else if (t_K <= if97_r3cd_p_t(p_MPa)) return 3;  //3c
		else return 17;  //3q
	}		
	
	else if (p_MPa > P3_CD) {   // above P3cd to  below or equal 20.5 MPa
		if (if97_r4_ts(p_MPa) <= t_K ) return  20; //3t  : on or right of saturation line fig 3
	
		// left of saturation line fig 3
		else if (t_K <= if97_r3cd_p_t(p_MPa)) return 3;  //3c
		else return 19;  //3s
	}		
	
	else if (p_MPa > if97_r4_ps(IF97_R3_LTEMP)) {   // above Psat(623.15 K) to  below or equal P3cd 
		if (if97_r4_ts(p_MPa) <= t_K ) return  20; //3t  : on or right of saturation line fig 3
	
		// left of saturation line fig 3
		else return 3;  //3c
	}		
	
	else return 0 ; //  this should never execute
}	


	
	





