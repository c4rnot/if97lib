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
	,{10,	6,	-0.837513931798655e16}
};
