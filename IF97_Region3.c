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

const typIF97Coeffs_Jn T3AB_P_R3_COEFFS[] = {
	{0,	 0.0} 				   //0  i starts at 1, so 0th i is not used
	,{0,	0.154793642129415e04}	
	,{1,    -0.187661219490113e03}
	,{2,	0.213144632222113e02}
	,{-1,	-0.191887498864292e04}
	,{-2,	0.918419702359447e03}
};


const typIF97Coeffs_Jn T3CD_P_R3_COEFFS[] = {
	{0,	 0.0} 				   //0  i starts at 1, so 0th i is not used
	,{0,   0.585276966696349e03}	
	,{1,    0.278233532206915e01}
	,{2,	-0.127283549295878e-1}
	,{3,	0.159090746562729e-3}
};



const typIF97Coeffs_Jn T3GH_P_R3_COEFFS[] = {
	{0,	 0.0} 				   //0  i starts at 1, so 0th i is not used
	,{0,   -0.249284240900418e05}	
	,{1,    0.428143584791546e04}
	,{2,	-0.269029173140130e03}
	,{3,	0.751608051114157e01}
	,{4,	-0.787105249910383e-1}
};



const typIF97Coeffs_Jn T3IJ_P_R3_COEFFS[] = {
	{0,	 0.0} 				   //0  i starts at 1, so 0th i is not used
	,{0,	0.584814781649163e03}	
	,{1,    -0.616179320924617e00}
	,{2,	0.260763050899562e00}
	,{3,	-0.587071076864459e-2}
	,{4,	0.515308185433082e-4}
};



const typIF97Coeffs_Jn T3JK_P_R3_COEFFS[] = {
	{0,	 0.0} 				   //0  i starts at 1, so 0th i is not used
	,{0,	0.617229772068439e03}	
	,{1,    -0.770600270141675e01}
	,{2,	0.697072596851896e00}
	,{3,	-0.157391839848015e-1}
	,{4,	0.137897492684194e-3}
};



const typIF97Coeffs_Jn T3MN_P_R3_COEFFS[] = {
	{0,	 0.0} 				   //0  i starts at 1, so 0th i is not used
	,{0,	0.535339483742384e03}	
	,{1,    0.761978122720128e01}
	,{2,	-0.158365725441648e00}
	,{3,	0.192871054508108e-2}
};



const typIF97Coeffs_Jn T3OP_P_R3_COEFFS[] = {
	{0,	 0.0} 				   //0  i starts at 1, so 0th i is not used
	,{0,	0.969461372400213e03}	
	,{1,    -0.332500170441278e03}
	,{2,	0.642859598466067e02}
	,{-1,	0.773845935768222e03}
	,{-2,	-0.152313732937084e04}
};


const typIF97Coeffs_Jn T3QU_P_R3_COEFFS[] = {
	{0,	 0.0} 				   //0  i starts at 1, so 0th i is not used
	,{0,	0.565603648239126e03}	
	,{1,    0.529062258221122e01}
	,{2,	-0.102020639611016e00}
	,{3,	0.122240301070145e-2}
};



const typIF97Coeffs_Jn T3RX_P_R3_COEFFS[] = {
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





// The following sets of coefficients from Appendix A1 .	
	

const typIF97Coeffs_IJn V3A_PT_COEFFS[] = {
	 {0,	0,	0.0} 				   //0  i starts at 1, so 0th i is not used
	,{-12,	5,	0.110879558823853e-2}
	,{-12,	10,	0.572616740810616e03}
	,{-12,	12,	-0.767051948380852e05}	
	,{-10,	5,	-0.253321069529674e-1}	
	,{-10,	10,	0.628008049345689e04} //5
		
	,{-10,	12,	0.234105654131876e06}
	,{-8,	5,	0.216867826045856e00}	
	,{-8,	8,	-0.156237904341963e03}	
	,{-8,	10,	-0.269893956176613e05}
	,{-6,	1,	-0.180407100085505e-3} //10

	,{-5,	1,	0.116732227668261e-2}
	,{-5,	5,	0.266987040856040e02}
	,{-5,	10,	0.282776617243286e05}
	,{-4,	8,	-0.242431520029523e04}
	,{-3,	0,	0.435217323022733e-3} //15
	
	,{-3,	1,	-0.122494831387441e-1}
	,{-3,	3,	0.179357604019989e01}
	,{-3,	6,	0.442729521058314e02}
	,{-2,	0,	-0.593223489018342e-2}
	,{-2,	2,	0.453186261685774e00} //20

	,{-2,	3,	0.135825703129140e01}
	,{-1,	0,	0.408748415856745e-1}
	,{-1,	1,	0.474686397863312e00}
	,{-1,	2,	0.118646814997915e01}
	,{0,	0,	0.546987265727549e00} //25

	,{0,	1,	0.195266770452643e00}
	,{1,	0,	-0.502268790869663e-1}
	,{1,	2,	-0.369645308193377e00}
	,{2,	0,	0.633828037528420e-2}
	,{2,	2,	0.797441793901017e-1} //30
};



const typIF97Coeffs_IJn V3B_PT_COEFFS[] = {
	 {0,	0,	0.0} 				   //0  i starts at 1, so 0th i is not used
	,{-12,	10,	-0.827670470003621e-1}
	,{-12,	12,	0.416887126010565e02}
	,{-10,	8,	0.483651982197059e-1}
	,{-10,	14,	-0.291032084950276e05}	
	,{-8,	8,	-0.111422582236948e03} //5	
	
	,{-6,	5,	-0.202300083904014e-1}
	,{-6,	6,	0.294002509338515e03}
	,{-6,	8,	0.140244997609658e03}
	,{-5,	5,	-0.344384158811459e03}
	,{-5,	8,	0.361182452612149e03}	//10

	,{-5,	10,	-0.140699677420738e04}
	,{-4,	2,	-0.202023902676481e-2}
	,{-4,	4,	0.171346792457471e03}
	,{-4,	5,	-0.425597804058632e01}
	,{-3,	0,	0.691346085000334e-5} //15

	,{-3,	1,	0.151140509678925e-2}
	,{-3,	2,	-0.416375290166236e-1}
	,{-3,	3,	-0.413754957011042e02}
	,{-3,	5,	-0.506673295721637e02}
	,{-2,	0,	-0.572212965569023e-3} //20
	
	,{-2,	2,	0.608817368401785e01}
	,{-2,	5,	0.239600660256161e02}
	,{-1,	0,	0.122261479925384e-1}
	,{-1,	2,	0.216356057692938e01}
	,{0,	0,	0.398198903368642e00} //25
	
	,{0,	1,	-0.116892827834085e00}
	,{1,	0,	-0.102845919373532e00}
	,{1,	2,	-0.492676637589284e00}
	,{2,	0,	0.655540456406790e-1}
	,{3,	2,	-0.240462535078530e00} //30

	,{4,	0,	-0.269798180310075e-1}
	,{4,	1,	0.128369435967012e00} //32
};



typIF97Coeffs_IJn V3C_PT_COEFFS[] = {
	 {0,	0,	0.0} 				   //0  i starts at 1, so 0th i is not used
	,{-12,	6,	0.311967788763030e01}
	,{-12,	8,	0.276713458847564e05}
	,{-12,	10,	0.322583103403269e08}
	,{-10,	6,	-0.342416065095363e03}
	,{-10,	8,	-0.899732529907377e06} //5

	,{-10,	10,	-0.793892049821251e08}
	,{-8,	5,	0.953193003217388e02}
	,{-8,	6,	0.229784742345072e04}
	,{-8,	7,	0.175336675322499e06}
	,{-6,	8,	0.791214365222792e07}//10

	,{-5,	1,	0.319933345844209e-4}
	,{-5,	4,	-0.659508863555767e02}
	,{-5,	7,	-0.833426563212851e06}
	,{-4,	2,	0.645734680583292e-1}
	,{-4,	8,	-0.382031020570813e07} //15

	,{-3,	0,	0.406398848470079e-4}
	,{-3,	3,	0.310327498492008e02}
	,{-2,	0,	-0.892996718483724e-3}
	,{-2,	4,	0.234604891591616e03}
	,{-2,	5,	0.377515668966951e04} //20

	,{-1,	0,	0.158646812591361e-1}
	,{-1,	1,	0.707906336241843e00}
	,{-1,	2,	0.126016225146570e02}
	,{0,	0,	0.736143655772152e00}
	,{0,	1,	0.676544268999101e00} //25

	,{0,	2,	-0.178100588189137e02}
	,{1,	0,	-0.156531975531713e00}
	,{1,	2,	0.117707430048158e02}
	,{2,	0,	0.840143653860447e-1}
	,{2,	1,	-0.186442467471949e00} //30

	,{2,	3,	-0.440170203949645e02}
	,{2,	7,	0.123290423502494e07}
	,{3,	0,	-0.240650039730845e-1}
	,{3,	7,	-0.107077716660869e07}
	,{8,	1,	0.438319858566475e-1} //35
};



const typIF97Coeffs_IJn V3D_PT_COEFFS[] = {
	 {0,	0,	0.0} 				   //0  i starts at 1, so 0th i is not used
	,{-12,	4,	-0.452484847171645e-9}
	,{-12,	6,	0.315210389538801e-4}	
	,{-12,	7,	-0.214991352047545e-2}
	,{-12,	10,	0.508058874808345e03}
	,{-12,	2,	-0.127123036845932e08} //5

	,{-12,	16,	0.115371133120497e13}
	,{-10,	0,	-0.197805728776273e-15}
	,{-10,	2,	0.241554806033972e-10}
	,{-10,	4,	-0.156481703640525e-5}
	,{-10,	6,	0.277211346836625e-2} //10

	,{-10,	8,	-0.203578994462286e02}
	,{-10,	10,	0.144369489909053e07}
	,{-10,	14,	-0.411254217946539e11}
	,{-8,	3,	0.623449786243773e-5}
	,{-8,	7,	-0.221774281146038e02} //15

	,{-8,	8,	-0.689315087933158e05}
	,{-8,	10,	-0.195419525060713e08}
	,{-6,	6,	0.316373510564015e04}
	,{-6,	8,	0.224040754426988e07}
	,{-5,	1,	-0.436701347922356e-5} //20

	,{-5,	2,	-0.404213852833996e-3}
	,{-5,	5,	-0.348153203414663e03}
	,{-5,	7,	-0.385294213555289e06}
	,{-4,	0,	0.135203700099403e-6}
	,{-4,	1,	0.134648383271089e-3} //25

	,{-4,	7,	0.125031835351736e06}
	,{-3,	2,	0.968123678455841e-1}
	,{-3,	4,	0.225660517512438e03}
	,{-2,	0,	-0.190102435341872e-3}
	,{-2,	1,	-0.299628410819229e-1} //30

	,{-1,	0,	0.500833915372121e-2}
	,{-1,	1,	0.387842482998411e00}
	,{-1,	5,	-0.138535367777182e04}
	,{0,	0,	0.870745245971773e00}
	,{0,	2,	0.171946252068742e01} //35

	,{1,	0,	-0.326650121426383e-1}
	,{1,	6,	0.498044171727877e04}
	,{3,	0,	0.551478022765087e-2}
};



const typIF97Coeffs_IJn V3E_PT_COEFFS[] = {
	 {0,	0,	0.0} 				   //0  i starts at 1, so 0th i is not used
	,{-12,	14,	0.715815808404721e09}
	,{-12,	16,	-0.114328360753449e12}
	,{-10,	3,	0.376531002015720e-11}
	,{-10,	6,	-0.903983668691157e-4}
	,{-10,	10,	0.665695908836252e06}  // 5

	,{-10,	14,	0.535364174960127e10}
	,{-10,	16,	0.794977402335603e11}
	,{-8,	7,	0.922230563421437e02}
	,{-8,	8,	-0.142586073991215e06}
	,{-8,	10,	-0.111796381424162e07} //10

	,{-6,	6,	0.896121629640760e04}
	,{-5,	6,	-0.669989239070491e04}
	,{-4,	2,	0.451242538486834e-2}
	,{-4,	4,	-0.339731325977713e02}
	,{-3,	2,	-0.120523111552278e01}  //15

	,{-3,	6,	0.475992667717124e05}
	,{-3,	7,	-0.266627750390341e06}
	,{-2,	0,	-0.153314954386524e-3}
	,{-2,	1,	0.305638404828265e00}
	,{-2,	3,	0.123654999499486e03}  //20

	,{-2,	4,	-0.104390794213011e04}
	,{-1,	0,	-0.157496516174308e-1}
	,{0,	0,	0.685331118940253e00}
	,{0,	1,	0.178373462873903e01}
	,{1,	0,	-0.544674124878910e00} //25

	,{1,	4,	0.204529931318843e04}
	,{1,	6,	-0.228342359328752e05}
	,{2,	0,	0.413197481515899e00}
	,{2,	2,	-0.3419311835910405e02}  //29
};



const typIF97Coeffs_IJn V3F_PT_COEFFS[] = {
	 {0,	0,	0.0} 				   //0  i starts at 1, so 0th i is not used
	,{0,	-3,	-0.251756547792325e-7}
	,{0,	-2,	0.601307193668763e-5}
	,{0,	-1,	-0.100615977450049e-2}
	,{0,	0,	0.999969140252192e00}
	,{0,	1,	0.214107759236486e01}  //5

	,{0,	2,	-0.165175571959086e02}
	,{1,	-1,	-0.141987303638727e-2}
	,{1,	1,	0.269251915156554e01}
	,{1,	2,	0.349741815858722e02}
	,{1,	3,	-0.300208695771783e02}  //10

	,{2,	0,	-0.131546288252539e01}
	,{2,	1,	-0.839091277286169e01}
	,{3,	-5,	0.181545608337015e-9}
	,{3,	-2,	-0.591099206478909e-3}
	,{3,	0,	0.152115067087106e01}  //15

	,{4,	-3,	0.252956470663225e-4}
	,{5,	-8,	0.100726265203786e-14}
	,{5,	1,	-0.149774533860650e01}
	,{6,	-6,	-0.793940970562969e-9}
	,{7,	-4,	-0.150290891264717e-3}  //20

	,{7,	1,	0.151205531275133e01}
	,{10,	-6,	0.470942606221652e-5}
	,{12,	-10,	0.195049710391712e-12}
	,{12,	-8,	-0.911627886266077e-8}
	,{12,	-4,	0.604374640201265e-3}  //25

	,{14,	-12,	-0.225132933900136e-15}
	,{14,	-10,	0.610916973582981e-11}
	,{14,	-8,	-0.303063908043404e-6}
	,{14,	-6,	-0.137796070798409e-4}
	,{14,	-4,	-0.919296736666106e-3}  //30

	,{16,	-10,	0.639288223132545e-9}
	,{16,	-8,	0.753259479898699e-6}
	,{18,	-12,	-0.400321478682929e-12}
	,{18,	-10,	0.756140294351614e-8}
	,{20,	-12,	-0.912082054034891e-11}  //35

	,{20,	-10,	-0.237612381140539e-7}
	,{20,	-6,	0.269586010591874e-4}
	,{22,	-12,	-0.732828135157839e-10}
	,{24,	-12,	0.241995578306660e-9}
	,{24,	-4,	-0.405735532730322e-3}  //40

	,{28,	-12,	0.189424143498011e-9}
	,{32,	-12,	-0.486632965074563e-9}  //42
};



const typIF97Coeffs_IJn V3G_PT_COEFFS[] = {
	 {0,	0,	0.0} 				   //0  i starts at 1, so 0th i is not used
	,{-12,	7,	0.412209020652996e-4}
	,{-12,	12,	-0.114987238280587e07}
	,{-12,	14,	0.948180885032080e10}
	,{-12,	18,	-0.195788865718971e18}
	,{-12,	22,	0.496250704871300e25} //5

	,{-12,	24,	-0.105549884548496e29}
	,{-10,	14,	-0.758642165988278e12}
	,{-10,	20,	-0.922172769596101e23}
	,{-10,	24,	0.725379072059348e30}
	,{-8,	7,	-0.617718249205859e02} //10

	,{-8,	8,	0.107555033344858e05}
	,{-8,	10,	-0.379545802336487e08}
	,{-8,	12,	0.228646846221831e12}
	,{-6,	8,	-0.499741093010619e07}
	,{-6,	22,	-0.280214310054101e31} //15

	,{-5,	7,	0.104915406769586e07}
	,{-5,	20,	0.613754229168619e28}
	,{-4,	22,	0.802056715528387e32}
	,{-3,	7,	-0.298617819828065e08}
	,{-2,	3,	-0.910782540134681e02} //20

	,{-2,	5,	0.135033227281565e06}
	,{-2,	14,	-0.712949383408211e19}
	,{-2,	24,	-0.104578785289542e37}
	,{-1,	2,	0.304331584444093e02}
	,{-1,	8,	0.593250797959445e10} //25

	,{-1,	18,	-0.364174062110798e28}
	,{0,	0,	0.921791403532461e00}
	,{0,	1,	-0.337693609657471e00}
	,{0,	2,	-0.724644143758508e02}
	,{1,	0,	-0.110480239272601e00} //30

	,{1,	1,	0.536516031875059e01}
	,{1,	3,	-0.291441872156205e04}
	,{3,	24,	0.616338176535305e40}
	,{5,	22,	-0.120889175861180e39}
	,{6,	12,	0.818396024524612e23} //35

	,{8,	3,	0.940781944835829e09}
	,{10,	0,	-0.367279669545448e05}
	,{10,	6,	-0.837513931798655e16} //38
};



const typIF97Coeffs_IJn V3H_PT_COEFFS[] = {
	 {0,	0,	0.0} 				   //0  i starts at 1, so 0th i is not used
	,{-12,	8,	0.561379678887577e-1}
	,{-12,	12,	0.774135421587083e10}
	,{-10,	4,	0.111482975877938e-8}
	,{-10,	6,	-0.143987128208183e-2}
	,{-10,	8,	0.193696558764920e04} //5

	,{-10,	10,	-0.605971823585005e09}
	,{-10,	14,	0.171951568124337e14}
	,{-10,	16,	-0.185461154985145e17}
	,{-8,	0,	0.387851168078010e-16}
	,{-8,	1,	-0.395464327846105e-13}  //10

	,{-8,	6,	-0.170875935679023e03}
	,{-8,	7,	-0.212010620701220e04}
	,{-8,	8,	0.177683337348191e08}	
	,{-6,	4,	0.110177443629575e02}
	,{-6,	6,	-0.234396091693313e06}  //15

	,{-6,	8,	-0.656174421999594e07}
	,{-5,	2,	0.156362212977396e-4}
	,{-5,	3,	-0.212946257021400e01}
	,{-5,	4,	0.135249306374858e02}
	,{-4,	2,	0.177189164145813e00} //20

	,{-4,	4,	0.139449167345464e04}
	,{-3,	1,	-0.703670932036388e-2}
	,{-3,	2,	-0.152011044389648e00}
	,{-2,	0,	0.981916922991113e-4}
	,{-1,	0,	0.147199658618076e-2} //25

	,{-1,	2,	0.202618487025578e02}
	,{0,	0,	0.899345518944240e00}
	,{1,	0,	-0.211346402240858e00}
	,{1,	2,	0.249971752957491e02} //29
};



const typIF97Coeffs_IJn V3I_PT_COEFFS[] = {
	 {0,	0,	0.0} 				   //0  i starts at 1, so 0th i is not used
	,{0,	0,	0.106905684359136e01}
	,{0,	1,	-0.148620857922333e01}
	,{0,	10,	0.259862256980408e15}
	,{1,	-4,	-0.446352055678749e-11}
	,{1,	-2,	-0.566620757170032e-6} //5

	,{1,	-1,	-0.235302885736849e-2}
	,{1,	0,	-0.269226321968839e00}
	,{2,	0,	0.922024992944392e01}
	,{3,	-5,	0.357633505503772e-11}
	,{3,	0,	-0.173942565562222e02} //10

	,{4,	-3,	0.700681785556229e-5}
	,{4,	-2,	-0.267050351075768e-3}
	,{4,	-1,	-0.231779669675624e01}
	,{5,	-6,	-0.753533046979752e-12}
	,{5,	-1,	0.481337131452891e01} //15

	,{5,	12,	-0.223286270422356e22}
	,{7,	-4,	-0.118746004987383e-4}
	,{7,	-3,	0.646412934136496e-2}
	,{8,	-6,	-0.410588536330937e-9}
	,{8,	10,	0.422739537057241e20} //20

	,{10,	-8,	0.313698180473812e-12}
	,{12,	-12,	0.164395334345040e-23}
	,{12,	-6,	-0.339823323754373e-5}
	,{12,	-4,	-0.135268639905021e-1}
	,{14,	-10,	-0.723252514211625e-14} //25

	,{14,	-8,	0.184386437538366e-8}
	,{14,	-4,	-0.463959533752385e-1}
	,{14,	5,	-0.922263100376750e14}
	,{18,	-12,	0.688169154439335e-16}
	,{18,	-10,	-0.222620998452197e-10} //30

	,{18,	-8,	-0.540843018624083e-7}
	,{18,	-6,	0.345570606200257e-2}
	,{18,	2,	0.422275800304086e11}
	,{20,	-12,	-0.126974478770487e-14}
	,{20,	-10,	0.927237985153679e-9} //35

	,{22,	-12,	0.612670812016489e-13}
	,{24,	-12,	-0.722693924063497e-11}
	,{24,	-8,	-0.383669502636822e-3}
	,{32,	-10,	0.374684572410204e-3}
	,{32,	-5,	-0.931976897511086e05}  //40

	,{36,	-10,	-0.247690616026922e-1}
	,{36,	-8,	0.658110546759474e02} //42
};



const typIF97Coeffs_IJn V3J_PT_COEFFS[] = {
	 {0,	0,	0.0} 				   //0  i starts at 1, so 0th i is not used
	,{0,	-1,	-0.111371317395540e-3}
	,{0,	0,	0.100342892423685e01}
	,{0,	1,	0.530615581928979e01}
	,{1,	-2,	0.179058760078792e-5}
	,{1,	-1,	-0.728541958464774e-3} //5

	,{1,	1,	-0.187576133371704e02}
	,{2,	-1,	0.199060874071849e-2}
	,{2,	1,	0.243574755377290e02}
	,{3,	-2,	-0.177040785499444e-3}
	,{4,	-2,	-0.198704578406823e03} //10

	,{4,	2,	-0.198704578406823e03}
	,{5,	-3,	0.738627790224287e-4}
	,{5,	-2,	-0.236264692844138e-2}
	,{5,	0,	-0.161023121314333e01}
	,{6,	3,	0.622322971786473e04} //15

	,{10,	-6,	-0.960754116701669e-8}
	,{12,	-8,	-0.510572269720488e-10}
	,{12,	-3,	0.767373781404211e-2}
	,{14,	-10,	0.663855469485254e-14}
	,{14,	-8,	-0.717590735526745e-9} //20

	,{14,	-5,	0.146564542926508e-4}
	,{16,	-10,	0.309029474277013e-11}
	,{18,	-12,	-0.464216300971708e-15}
	,{20,	-12,	-0.390499637961161e-13}
	,{20,	-10,	-0.236716126781431e-9} //25

	,{24,	-12,	0.454652854268717e-11}
	,{24,	-6,	-0.422271787482497e-2}
	,{28,	-12,	0.283911742354706e-10}
	,{28,	-5,	0.270929002720228e01} //29
};



const typIF97Coeffs_IJn V3K_PT_COEFFS[] = {
	 {0,	0,	0.0} 				   //0  i starts at 1, so 0th i is not used
	,{-2,	10,	-0.401215699576099e09}
	,{-2,	12,	0.484501487318406e11}
	,{-1,	-5,	0.394721471363678e-14}
	,{-1,	6,	0.372629967374147e05}
	,{0,	-12,	-0.369794374168666e-29}  //5

	,{0,	-6,	-0.380436407012452e-14}
	,{0,	-2,	0.475361629970233e-6}
	,{0,	-1,	-0.879148916140706e-3}
	,{0,	0,	0.844317863844331e00}
	,{0,	1,	0.122433162656600e02}  //10

	,{0,	2,	-0.104529634830279e03}
	,{0,	3,	0.589702771277429e03}
	,{0,	1,	-0.291026851164444e14}
	,{1,	-3,	0.170343072841850e-5}
	,{1,	1-2,	-0.277617606975748e-3}  //15

	,{1,	0,	-0.344709605486686e01}
	,{1,	1,	0.221333862447095e02}
	,{1,	2,	-0.194646110037079e03}
	,{2,	-8,	0.808354639772825e-15}
	,{2,	-6,	-0.180845209145470e-10}  //20

	,{2,	-3,	-0.696644158132412e-5}
	,{2,	-2,	-0.181057560300994e-2}
	,{2,	0,	0.255830298579027e01}
	,{2,	4,	0.328913873658481e04}
	,{5,	-12,	-0.173270241249904e-18}  //25

	,{5,	-6,	-0.661876792558034e-6}
	,{5,	-3,	-0.395688923421250e-2}
	,{6,	-12,	0.604203299819132e-17}
	,{6,	-10,	-0.400879935920517e-13}
	,{6,	-8,	0.160751107464958e-8}  //30

	,{6,	-5,	0.383719409025556e-4}
	,{8,	-12,	-0.649565446702457e-14}
	,{10,	-12,	-0.149095328506000e-11}
	,{12,	-10,	0.541449377329581e-8} //34
};



const typIF97Coeffs_IJn V3L_PT_COEFFS[] = {
	 {0,	0,	0.0} 				   //0  i starts at 1, so 0th i is not used
	,{-12,	14,	0.260702058647537e10}
	,{-12,	16,	-0.188277213604704e15}
	,{-12,	18,	0.554923870289667e19}
	,{-12,	20,	-0.758966946387758e23}
	,{-12,	22,	0.413865186848908e27} //5

	,{-10,	14,	-0.815038000738060e12}
	,{-10,	24,	-0.381458260489955e33}
	,{-8,	6,	-0.123239564600519e-1}
	,{-8,	10,	0.226095631437174e08}
	,{-8,	12,	-0.495017809506720e12} //10

	,{-8,	14,	0.529482996422863e16}
	,{-8,	18,	-0.444359478746295e23}
	,{-8,	24,	0.521635864527315e35}
	,{-8,	36,	-0.487095672740742e55}
	,{-6,	8,	-0.714430209937547e06} //15

	,{-5,	4,	0.127868634615495e00}
	,{-5,	5,	-0.100752127917598e02}
	,{-4,	7,	0.777451437960990e07}
	,{-4,	16,	-0.108105480796471e25}
	,{-3,	1,	-0.357578581169659e-5} //20

	,{-3,	3,	-0.212857169423484e01}
	,{-3,	18,	0.270706111085238e30}
	,{-3,	20,	-0.695953622348829e33}
	,{-2,	2,	0.110609027472280e00}
	,{-2,	3,	0.721559163361354e02} //25

	,{-2,	10,	-0.306367307532219e15}
	,{-1,	0,	0.265839618885530e-4}
	,{-1,	1,	0.253392392889754e-1}
	,{-1,	3,	-0.214443041836579e03}
	,{0,	0,	0.937846601489667e00} //30

	,{0,	1,	0.223184043101700e01}
	,{0,	2,	0.338401222509191e02}
	,{0,	12,	0.494237237179718e21}
	,{1,	0,	-0.198068404154028e00}
	,{1,	16,	-0.141415349881140e31} //35

	,{2,	1,	-0.993862421613651e02}
	,{4,	0,	0.125070534142731e03}
	,{5,	0,	-0.996473529004439e03}
	,{5,	1,	0.473137909872765e05}
	,{6,	14,	0.116662121219322e33}  //40

	,{10,	4,	-0.315874976271533e16}
	,{10,	12,	-0.445703369196945e33}
	,{14,	10,	0.642794932373694e33} //43
};
	
	

const typIF97Coeffs_IJn V3M_PT_COEFFS[] = {
	 {0,	0,	0.0} 				   //0  i starts at 1, so 0th i is not used
	,{0,	0,	0.811384363481847e00}
	,{3,	0,	-0.568199310990094e04}
	,{8,	0,	-0.178657198172556e11}
	,{20,	2,	0.795537657613427e32}
	,{1,	5,	-0.814568209346872e05} //5

	,{3,	5,	-0.659774567602874e08}
	,{4,	5,	-0.152861148659302e11}
	,{5,	5,	-0.560165667510446e12}
	,{1,	6,	0.458384828593949e06}
	,{6,	6,	-0.385754000383848e14} //10

	,{2,	7,	0.453735800004273e08}
	,{4,	8,	0.939454935735563e12}
	,{14,	8,	0.266572856432938e28}
	,{2,	10,	-0.547578313899097e10}
	,{5,	10,	0.200725701112386e15} //15

	,{3,	12,	0.185007245563239e13}
	,{0,	14,	0.185135446828337e09}
	,{1,	14, -0.170451090076385e12}
	,{1,	18,	0.157890366037614e15}
	,{1,	20,	-0.202530509748774e16} //20

	,{28,	20,	0.368193926183570e60}
	,{2,	22,	0.170215539458936e18}
	,{16,	22,	0.639234909918741e42}
	,{0,	24,	-0.821698160721956e15}
	,{5,	24,	-0.795260241872306e24} //25

	,{0,	28,	0.233415869478510e18}
	,{3,	28,	-0.600079934586803e23}
	,{4,	28,	0.594584382273384e25}
	,{12,	28,	0.189461279349492e40}
	,{16,	28,	-0.810093428842645e46} //30

	,{1,	32,	0.188813911076809e22}
	,{8,	32,	0.111052244098768e36}
	,{14,	32,	0.291133958602503e46}
	,{0,	36,	-0.329421923951460e22}
	,{2,	36,	-0.137570282536969e26} //35

	,{3,	36,	0.181508996303902e28}
	,{4,	36,	-0.346865122768353e30}
	,{8,	36,	-0.211961148774260e38}
	,{14,	36,	-0.128617899887675e49}
	,{24,	36,	0.479817895699239e65} //40
};


//From Hummeling if97 Java
// http://sourceforge.net/p/if97/git/ci/master/tree/src/main/java/com/hummeling/if97/Region3.java#l1801
const typIF97Coeffs_IJn V3N_PT_COEFFS[] = {
	 {0,	0,	0.0},				   //0  i starts at 1, so 0th i is not used
	{0, -12, .280967799943151e-38},
	{3, -12, .614869006573609e-30},
	{4, -12, .582238667048942e-27},
	{6, -12, .390628369238462e-22},
	{7, -12, .821445758255119e-20},
	{10, -12, .402137961842776e-14},
	{12, -12, .651718171878301e-12},
	{14, -12, -.211773355803058e-7},
	{18, -12, .264953354380072e-2},
	{0, -10, -.135031446451331e-31},
	{3, -10, -.607246643970893e-23},
	{5, -10, -.402352115234494e-18},
	{6, -10, -.744938506925544e-16},
	{8, -10, .189917206526237e-12},
	{12, -10, .364975183508473e-5},
	{0, -8, .177274872361946e-25},
	{3, -8, -.334952758812999e-18},
	{7, -8, -.421537726098389e-8},
	{12, -8, -.391048167929649e-1},
	{2, -6, .541276911564176e-13},
	{3, -6, .705412100773699e-11},
	{4, -6, .258585887897486e-8},
	{2, -5, -.493111362030162e-10},
	{4, -5, -.158649699894543e-5},
	{7, -5, -.525037427886100},
	{4, -4, .220019901729615e-2},
	{3, -3, -.643064132636925e-2},
	{5, -3, .629154149015048e2},
	{6, -3, .135147318617061e3},
	{0, -2, .240560808321713e-6},
	{0, -1, -.890763306701305e-3},
	{3, -1, -.440209599407714e4},
	{1, 0, -.302807107747776e3},
	{0, 1, .159158748314599e4},
	{1, 1, .232534272709876e6},
	{0, 2, -.792681207132600e6},
	{1, 4, -.869871364662769e11},
	{0, 5, .354542769185671e12},
	{1, 6, .400849240129329e15} //39
};

//From Hummeling if97 Java
// http://sourceforge.net/p/if97/git/ci/master/tree/src/main/java/com/hummeling/if97/Region3.java#l1801
const typIF97Coeffs_IJn V3O_PT_COEFFS[] = {
	{0,	0,	0.},			   //0  i starts at 1, so 0th i is not used
	{0, -12, .128746023979718e-34},
	{0, -4, -.735234770382342e-11},
	{0, -1, .289078692149150e-2},
	{2, -1, .244482731907223},
	{3, -10, .141733492030985e-23},
	{4, -12, -.354533853059476e-28},
	{4, -8, -.594539202901431e-17},
	{4, -5, -.585188401782779e-8},
	{4, -4, .201377325411803e-5},
	{4, -1, .138647388209306e1},
	{5, -4, -.173959365084772e-4},
	{5, -3, .137680878349369e-2},
	{6, -8, .814897605805513e-14},
	{7, -12, .425596631351839e-25},
	{8, -10, -.387449113787755e-17},
	{8, -8, .139814747930240e-12},
	{8, -4, -.171849638951521e-2},
	{10, -12, .641890529513296e-21},
	{10, -8, .118960578072018e-10},
	{14, -12, -.155282762571611e-17},
	{14, -8, .233907907347507e-7},
	{20, -12, -.174093247766213e-12},
	{20, -10, .377682649089149e-8},
	{24, -12, -.516720236575302e-10} //24
};



//From Hummeling if97 Java
// http://sourceforge.net/p/if97/git/ci/master/tree/src/main/java/com/hummeling/if97/Region3.java#l1801
const typIF97Coeffs_IJn V3P_PT_COEFFS[] = {
	{0,	0,	0.},			   //0  i starts at 1, so 0th i is not used
	{0, -1, -.982825342010366e-4},
	{0, 0, .105145700850612e1},
	{0, 1, .116033094095084e3},
	{0, 2, .324664750281543e4},
	{1, 1, -.123592348610137e4},
	{2, -1, -.561403450013495e-1},
	{3, -3, .856677401640869e-7},
	{3, 0, .236313425393924e3},
	{4, -2, .972503292350109e-2},
	{6, -2, -.103001994531927e1},
	{7, -5, -.149653706199162e-8},
	{7, -4, -.215743778861592e-4},
	{8, -2, -.834452198291445e1},
	{10, -3, .586602660564988},
	{12, -12, .343480022104968e-25},
	{12, -6, .816256095947021e-5},
	{12, -5, .294985697916798e-2},
	{14, -10, .711730466276584e-16},
	{14, -8, .400954763806941e-9},
	{14, -3, .107766027032853e2},
	{16, -8, -.409449599138182e-6},
	{18, -8, -.729121307758902e-5},
	{20, -10, .677107970938909e-8},
	{22, -10, .602745973022975e-7},
	{24, -12, -.382323011855257e-10},
	{24, -8, .179946628317437e-2},
	{36, -12, -.345042834640005e-3} //27
};





//From Hummeling if97 Java
// http://sourceforge.net/p/if97/git/ci/master/tree/src/main/java/com/hummeling/if97/Region3.java#l1801
const typIF97Coeffs_IJn V3Q_PT_COEFFS[] = {
	{0,	0,	0.},			   //0  i starts at 1, so 0th i is not used
	{-12, 10, -.820433843259950e5},
	{-12, 12, .473271518461586e11},
	{-10, 6, -.805950021005413e-1},
	{-10, 7, .328600025435980e2},
	{-10, 8, -.356617029982490e4},
	{-10, 10, -.172985781433335e10},
	{-8, 8, .351769232729192e8},
	{-6, 6, -.775489259985144e6},
	{-5, 2, .710346691966018e-4},
	{-5, 5, .993499883820274e5},
	{-4, 3, -.642094171904570},
	{-4, 4, -.612842816820083e4},
	{-3, 3, .232808472983776e3},
	{-2, 0, -.142808220416837e-4},
	{-2, 1, -.643596060678456e-2},
	{-2, 2, -.428577227475614e1},
	{-2, 4, .225689939161918e4},
	{-1, 0, .100355651721510e-2},
	{-1, 1, .333491455143516},
	{-1, 2, .109697576888873e1},
	{0, 0, .961917379376452},
	{1, 0, -.838165632204598e-1},
	{1, 1, .247795908411492e1},
	{1, 3, -.319114969006533e4} //24
};




//From Hummeling if97 Java
// http://sourceforge.net/p/if97/git/ci/master/tree/src/main/java/com/hummeling/if97/Region3.java#l1801
const typIF97Coeffs_IJn V3R_PT_COEFFS[] = {
	{0,	0,	0.},			   //0  i starts at 1, so 0th i is not used
	{-8, 6, .144165955660863e-2},
	{-8, 14, -.701438599628258e13},
	{-3, -3, -.830946716459219e-16},
	{-3, 3, .261975135368109},
	{-3, 4, .393097214706245e3},
	{-3, 5, -.104334030654021e5},
	{-3, 8, .490112654154211e9},
	{0, -1, -.147104222772069e-3},
	{0, 0, .103602748043408e1},
	{0, 1, .305308890065089e1},
	{0, 5, -.399745276971264e7},
	{3, -6, .569233719593750e-11},
	{3, -2, -.464923504407778e-1},
	{8, -12, -.535400396512906e-17},
	{8, -10, .399988795693162e-12},
	{8, -8, -.536479560201811e-6},
	{8, -5, .159536722411202e-1},
	{10, -12, .270303248860217e-14},
	{10, -10, .244247453858506e-7},
	{10, -8, -.983430636716454e-5},
	{10, -6, .663513144224454e-1},
	{10, -5, -.993456957845006e1},
	{10, -4, .546491323528491e3},
	{10, -3, -.143365406393758e5},
	{10, -2, .150764974125511e6},
	{12, -12, -.337209709340105e-9},
	{14, -12, .377501980025469e-8} //27
};


//From Hummeling if97 Java
// http://sourceforge.net/p/if97/git/ci/master/tree/src/main/java/com/hummeling/if97/Region3.java#l1801
const typIF97Coeffs_IJn V3S_PT_COEFFS[] = {
	{0,	0,	0.},			   //0  i starts at 1, so 0th i is not used
	{-12, 20, -.532466612140254e23},
	{-12, 24, .100415480000824e32},
	{-10, 22, -.191540001821367e30},
	{-8, 14, .105618377808847e17},
	{-6, 36, .202281884477061e59},
	{-5, 8, .884585472596134e8},
	{-5, 16, .166540181638363e23},
	{-4, 6, -.313563197669111e6},
	{-4, 32, -.185662327545324e54},
	{-3, 3, -.624942093918942e-1},
	{-3, 8, -.504160724132590e10},
	{-2, 4, .187514491833092e5},
	{-1, 1, .121399979993217e-2},
	{-1, 2, .188317043049455e1},
	{-1, 3, -.167073503962060e4},
	{0, 0, .965961650599775},
	{0, 1, .294885696802488e1},
	{0, 4, -.653915627346115e5},
	{0, 28, .604012200163444e50},
	{1, 0, -.198339358557937},
	{1, 32, -.175984090163501e58},
	{3, 0, .356314881403987e1},
	{3, 1, -.575991255144384e3},
	{3, 2, .456213415338071e5},
	{4, 3, -.109174044987829e8},
	{4, 18, .437796099975134e34},
	{4, 24, -.616552611135792e46},
	{5, 4, .193568768917797e10},
	{14, 24, .950898170425042e54} //29
};



//From Hummeling if97 Java
// http://sourceforge.net/p/if97/git/ci/master/tree/src/main/java/com/hummeling/if97/Region3.java#l1801
const typIF97Coeffs_IJn V3T_PT_COEFFS[] = {
	{0,	0,	0.},			   //0  i starts at 1, so 0th i is not used
	{0, 0, .155287249586268e1},
	{0, 1, .664235115009031e1},
	{0, 4, -.289366236727210e4},
	{0, 12, -.385923202309848e13},
	{1, 0, -.291002915783761e1},
	{1, 10, -.829088246858083e12},
	{2, 0, .176814899675218e1},
	{2, 6, -.534686695713469e9},
	{2, 14, .160464608687834e18},
	{3, 3, .196435366560186e6},
	{3, 8, .156637427541729e13},
	{4, 0, -.178154560260006e1},
	{4, 10, -.229746237623692e16},
	{7, 3, .385659001648006e8},
	{7, 4, .110554446790543e10},
	{7, 7, -.677073830687349e14},
	{7, 20, -.327910592086523e31},
	{7, 36, -.341552040860644e51},
	{10, 10, -.527251339709047e21},
	{10, 12, .245375640937055e24},
	{10, 14, -.168776617209269e27},
	{10, 16, .358958955867578e29},
	{10, 22, -.656475280339411e36},
	{18, 18, .355286045512301e39},
	{20, 32, .569021454413270e58},
	{22, 22, -.700584546433113e48},
	{22, 36, -.705772623326374e65},
	{24, 24, .166861176200148e53},
	{28, 28, -.300475129680486e61},
	{32, 22, -.668481295196808e51},
	{32, 32, .428432338620678e69},
	{32, 36, -.444227367758304e72},
	{36, 36, -.281396013562745e77} //33
};


/* ****   AUXILLIARY CONSTANTS FOR CLOSE TO CRITICAL REGIION ******* */

//From Hummeling if97 Java
// http://sourceforge.net/p/if97/git/ci/master/tree/src/main/java/com/hummeling/if97/Region3.java#l1801
const typIF97Coeffs_IJn V3U_PT_COEFFS[] = {
	{0,	0,	0.},			   //0  i starts at 1, so 0th i is not used
	{-12, 14, .122088349258355e18},
	{-10, 10, .104216468608488e10},
	{-10, 12, -.882666931564652e16},
	{-10, 14, .259929510849499e20},
	{-8, 10, .222612779142211e15},
	{-8, 12, -.878473585050085e18},
	{-8, 14, -.314432577551552e22},
	{-6, 8, -.216934916996285e13},
	{-6, 12, .159079648196849e21},
	{-5, 4, -.339567617303423e3},
	{-5, 8, .884387651337836e13},
	{-5, 12, -.843405926846418e21},
	{-3, 2, .114178193518022e2},
	{-1, -1, -.122708229235641e-3},
	{-1, 1, -.106201671767107e3},
	{-1, 12, .903443213959313e25},
	{-1, 14, -.693996270370852e28},
	{0, -3, .648916718965575e-8},
	{0, 1, .718957567127851e4},
	{1, -2, .105581745346187e-2},
	{2, 5, -.651903203602581e15},
	{2, 10, -.160116813274676e25},
	{3, -5, -.510254294237837e-8},
	{5, -4, -.152355388953402},
	{5, 2, .677143292290144e12},
	{5, 3, .276378438378930e15},
	{6, -5, .116862983141686e-1},
	{6, 2, -.301426947980171e14},
	{8, -8, .169719813884840e-7},
	{8, 8, .104674840020929e27},
	{10, -4, -.108016904560140e5},
	{12, -12, -.990623601934295e-12},
	{12, -4, .536116483602738e7},
	{12, 4, .226145963747881e22},
	{14, -12, -.488731565776210e-9},
	{14, -10, .151001548880670e-4},
	{14, -6, -.227700464643920e5},
	{14, 6, -.781754507698846e28} //38
};



//From Hummeling if97 Java
// http://sourceforge.net/p/if97/git/ci/master/tree/src/main/java/com/hummeling/if97/Region3.java#l1801
const typIF97Coeffs_IJn V3V_PT_COEFFS[] = {
	{0,	0,	0.},			   //0  i starts at 1, so 0th i is not used
	{-10, -8, -.415652812061591e-54},
	{-8, -12, .177441742924043e-60},
	{-6, -12, -.357078668203377e-54},
	{-6, -3, .359252213604114e-25},
	{-6, 5, -.259123736380269e2},
	{-6, 6, .594619766193460e5},
	{-6, 8, -.624184007103158e11},
	{-6, 10, .313080299915944e17},
	{-5, 1, .105006446192036e-8},
	{-5, 2, -.192824336984852e-5},
	{-5, 6, .654144373749937e6},
	{-5, 8, .513117462865044e13},
	{-5, 10, -.697595750347391e19},
	{-5, 14, -.103977184454767e29},
	{-4, -12, .119563135540666e-47},
	{-4, -10, -.436677034051655e-41},
	{-4, -6, .926990036530639e-29},
	{-4, 10, .587793105620748e21},
	{-3, -3, .280375725094731e-17},
	{-3, 10, -.192359972440634e23},
	{-3, 12, .742705723302738e27},
	{-2, 2, -.517429682450605e2},
	{-2, 4, .820612048645469e7},
	{-1, -2, -.188214882341448e-8},
	{-1, 0, .184587261114837e-1},
	{0, -2, -.135830407782663e-5},
	{0, 6, -.723681885626348e17},
	{0, 10, -.223449194054124e27},
	{1, -12, -.111526741826431e-34},
	{1, -10, .276032601145151e-28},
	{3, 3, .134856491567853e15},
	{4, -6, .652440293345860e-9},
	{4, 3, .510655119774360e17},
	{4, 10, -.468138358908732e32},
	{5, 2, -.760667491183279e16},
	{8, -12, -.417247986986821e-18},
	{10, -2, .312545677756104e14},
	{12, -3, -.100375333864186e15},
	{14, 1, .247761392329058e27} //39
};




//From Hummeling if97 Java
// http://sourceforge.net/p/if97/git/ci/master/tree/src/main/java/com/hummeling/if97/Region3.java#l1801
const typIF97Coeffs_IJn V3W_PT_COEFFS[] = {
	{0,	0,	0.},			   //0  i starts at 1, so 0th i is not used
	{-12, 8, -.586219133817016e-7},
	{-12, 14, -.894460355005526e11},
	{-10, -1, .531168037519774e-30},
	{-10, 8, .109892402329239},
	{-8, 6, -.575368389425212e-1},
	{-8, 8, .228276853990249e5},
	{-8, 14, -.158548609655002e19},
	{-6, -4, .329865748576503e-27},
	{-6, -3, -.634987981190669e-24},
	{-6, 2, .615762068640611e-8},
	{-6, 8, -.961109240985747e8},
	{-5, -10, -.406274286652625e-44},
	{-4, -1, -.471103725498077e-12},
	{-4, 3, .725937724828145},
	{-3, -10, .187768525763682e-38},
	{-3, 3, -.103308436323771e4},
	{-2, 1, -.662552816342168e-1},
	{-2, 2, .579514041765710e3},
	{-1, -8, .237416732616644e-26},
	{-1, -4, .271700235739893e-14},
	{-1, 1, -.907886213483600e2},
	{0, -12, -.171242509570207e-36},
	{0, 1, .156792067854621e3},
	{1, -1, .923261357901470},
	{2, -1, -.597865988422577e1},
	{2, 2, .321988767636389e7},
	{3, -12, -.399441390042203e-29},
	{3, -5, .493429086046981e-7},
	{5, -10, .812036983370565e-19},
	{5, -8, -.207610284654137e-11},
	{5, -6, -.340821291419719e-6},
	{8, -12, .542000573372233e-17},
	{8, -10, -.856711586510214e-12},
	{10, -12, .266170454405981e-13},
	{10, -8, .858133791857099e-5} //34
};



//From Hummeling if97 Java
// http://sourceforge.net/p/if97/git/ci/master/tree/src/main/java/com/hummeling/if97/Region3.java#l1801
const typIF97Coeffs_IJn V3X_PT_COEFFS[] = {
	{0,	0,	0.},			   //0  i starts at 1, so 0th i is not used
	{-8, 14, .377373741298151e19},
	{-6, 10, -.507100883722913e13},
	{-5, 10, -.103363225598860e16},
	{-4, 1, .184790814320773e-5},
	{-4, 2, -.924729378390945e-3},
	{-4, 14, -.425999562292738e24},
	{-3, -2, -.462307771873973e-12},
	{-3, 12, .107319065855767e22},
	{-1, 5, .648662492280682e11},
	{0, 0, .244200600688281e1},
	{0, 4, -.851535733484258e10},
	{0, 10, .169894481433592e22},
	{1, -10, .215780222509020e-26},
	{1, -1, -.320850551367334},
	{2, 6, -.382642448458610e17},
	{3, -12, -.275386077674421e-28},
	{3, 0, -.563199253391666e6},
	{3, 8, -.326068646279314e21},
	{4, 3, .397949001553184e14},
	{5, -6, .100824008584757e-6},
	{5, -2, .162234569738433e5},
	{5, 1, -.432355225319745e11},
	{6, 1, -.592874245598610e12},
	{8, -6, .133061647281106e1},
	{8, -3, .157338197797544e7},
	{8, 1, .258189614270853e14},
	{8, 8, .262413209706358e25},
	{10, -8, -.920011937431142e-1},
	{12, -10, .220213765905426e-2},
	{12, -8, -.110433759109547e2},
	{12, -5, .847004870612087e7},
	{12, -4, -.592910695762536e9},
	{14, -12, -.183027173269660e-4},
	{14, -10, .181339603516302},
	{14, -8, -.119228759669889e4},
	{14, -6, .430867658061468e7} //34
};



//From Hummeling if97 Java
// http://sourceforge.net/p/if97/git/ci/master/tree/src/main/java/com/hummeling/if97/Region3.java#l1801
const typIF97Coeffs_IJn V3Y_PT_COEFFS[] = {
	{0,	0,	0.},			   //0  i starts at 1, so 0th i is not used
	{0, -3, -.525597995024633e-9},
	{0, 1, .583441305228407e4},
	{0, 5, -.134778968457925e17},
	{0, 8, .118973500934212e26},
	{1, 8, -.159096490904708e27},
	{2, -4, -.315839902302021e-6},
	{2, -1, .496212197158239e3},
	{2, 4, .327777227273171e19},
	{2, 5, -.527114657850696e22},
	{3, -8, .210017506281863e-16},
	{3, 4, .705106224399834e21},
	{3, 8, -.266713136106469e31},
	{4, -6, -.145370512554562e-7},
	{4, 6, .149333917053130e28},
	{5, -2, -.149795620287641e8},
	{5, 1, -.381881906271100e16},
	{8, -8, .724660165585797e-4},
	{8, -2, -.937808169550193e14},
	{10, -5, .514411468376383e10},
	{12, -8, -.828198594040141e5} //19
};


//From Hummeling if97 Java
// http://sourceforge.net/p/if97/git/ci/master/tree/src/main/java/com/hummeling/if97/Region3.java#l1801
const typIF97Coeffs_IJn V3Z_PT_COEFFS[] = {
	{0,	0,	0.},			   //0  i starts at 1, so 0th i is not used
	{-8, 3, .244007892290650e-10},
	{-6, 6, -.463057430331242e7},
	{-5, 6, .728803274777712e10},
	{-5, 8, .327776302858856e16},
	{-4, 5, -.110598170118409e10},
	{-4, 6, -.323899915729957e13},
	{-4, 8, .923814007023245e16},
	{-3, -2, .842250080413712e-12},
	{-3, 5, .663221436245506e12},
	{-3, 6, -.167170186672139e15},
	{-2, 2, .253749358701391e4},
	{-1, -6, -.819731559610523e-20},
	{0, 3, .328380587890663e12},
	{1, 1, -.625004791171543e8},
	{2, 6, .803197957462023e21},
	{3, -6, -.204397011338353e-10},
	{3, -2, -.378391047055938e4},
	{6, -6, .972876545938620e-2},
	{6, -5, .154355721681459e2},
	{6, -4, -.373962862928643e4},
	{6, -1, -.682859011374572e11},
	{8, -8, -.248488015614543e-3},
	{8, -4, .394536049497068e7}  //22
};



