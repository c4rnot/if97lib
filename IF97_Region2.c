//          Copyright Martin Lord 2014-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


//    IAPWS-IF97 Region 2: Single phase vapour region equations
/* *********************************************************************
 * *******             VALIDITY                             ************
 * 
 * 273.15 K <= T <=  623.15 K    0 <= p <= Ps(T) (Eq 30 "Saturation Pressure basic Equation")
 * 623.15 K <= T <=  863.15 K    0 <= p <= p(T)  (Eq 5 "B23 Region 2 - 3 boundry equation")
 * 863.15 K <= T <= 1073.15 K    0 <= p <= 100 MPa
 * 
 * These functions also provide reasonable values for the metastable 
 * region above 10 MPa
 * 
 * Note:  for temperatures between 273.15 and 273.16 K, the part of the 
 * range of validity between the pressures on the saturation pressure 
 * line (Eq 30) and on the sublimation line corresponds to metastable 
 * states
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
#include "IF97_Region2.h"
#include <math.h> // for pow, log
/* #ifdef _OPENMP // multithreading via libgomp
 # include <omp.h>
 #endif
*/
// #include <stdio.h>  used for debugging only


//***************************************************************
//****** REGION 2 GIBBS FREE ENERGY AND DERIVATIVES**************

typedef struct sctGibbsCoeff_o {
	int Ji;
	double ni;
} typR2coeff_o;


// see Table 10
const typR2coeff_o  GIBBS_COEFFS_R2_O[] = {
	 {0,		 0.0} 			 //0  i starts at 1, so 0th i is not used
	,{ 0 , -9.6927686500217 }  // 1
	,{ 1 , 10.086655968018 }
	,{ -5 , -0.0056087911283 }
	,{ -4 , 0.0714527380815 }
	,{ -3 , -0.4071049822393 }
	,{ -2 , 1.4240819171444 }
	,{ -1 , -4.383951131945 }
	,{ 2 , -0.2840863246077 }
	,{ 3 , 0.0212684637533 } //9
};

const int MAX_GIBBS_COEFFS_R2_O = 9;


// ideal gas part of dimensionless gibbs free energy in Region2 :   See Equation 16
// checked OK
double if97_r2_Gamma_o (double if97_pi, double  if97_tau) {
	
	int i;
	double dblGammaSum = 0.0;		
	
	#pragma omp parallel for reduction(+:dblGammaSum) 	//handle loop multithreaded		
	for (i=1; i <= MAX_GIBBS_COEFFS_R2_O; i++) {
		
		dblGammaSum += GIBBS_COEFFS_R2_O[i].ni * pow( if97_tau, GIBBS_COEFFS_R2_O[i].Ji);
	}		 
	
return log(if97_pi) + dblGammaSum;
}



typedef struct sctGibbsCoeff_r {
	int Ii;
	int Ji;
	double ni;
} typR2coeff_r;


// See table 11
const typR2coeff_r  GIBBS_COEFFS_R2_R[] = {
	{0,	 0,	 0.0} 				   //0  i starts at 1, so 0th i is not used
	,{1,       0,       -1.7731742473213E-003}
	,{1,       1,       -1.7834862292358E-002}
	,{1,       2,       -4.5996013696365E-002}
	,{1,       3,       -5.7581259083432E-002}
	,{1,       6,       -5.0325278727930E-002}
	,{2,       1,       -3.3032641670203E-005}
	,{2,       2,       -1.8948987516315E-004}
	,{2,       4,       -3.9392777243355E-003}
	,{2,       7,       -4.3797295650573E-002}
	,{2,       36,       -2.6674547914087E-005}
	,{3,       0,       2.0481737692309E-008}
	,{3,       1,       4.3870667284435E-007}
	,{3,       3,       -3.2277677238570E-005}
	,{3,       6,       -1.5033924542148E-003}
	,{3,       35,       -4.0668253562649E-002}
	,{4,       1,       -7.8847309559367E-010}
	,{4,       2,       1.2790717852285E-008}
	,{4,       3,       4.8225372718507E-007}
	,{5,       7,       2.2922076337661E-006}
	,{6,       3,       -1.6714766451061E-011}
	,{6,       16,       -2.1171472321355E-003}
	,{6,       35,       -2.3895741934104E+001}
	,{7,       0,       -5.9059564324270E-018}
	,{7,       11,       -1.2621808899101E-006}
	,{7,       25,       -3.8946842435739E-002}
	,{8,       8,       1.1256211360459E-011}
	,{8,       36,       -8.2311340897998E+000}
	,{9,       13,       1.9809712802088E-008}
	,{10,       4,       1.0406965210174E-019}
	,{10,       10,       -1.0234747095929E-013}
	,{10,       14,       -1.0018179379511E-009}
	,{16,       29,       -8.0882908646985E-011}
	,{16,       50,       1.0693031879409E-001}
	,{18,       57,       -3.3662250574171E-001}
	,{20,       20,       8.9185845355421E-025}
	,{20,       35,       3.0629316876232E-013}
	,{20,       48,       -4.2002467698208E-006}
	,{21,       21,       -5.9056029685639E-026}
	,{22,       53,       3.7826947613457E-006}
	,{23,       39,       -1.2768608934681E-015}
	,{24,       26,       7.3087610595061E-029}
	,{24,       40,       5.5414715350778E-017}
	,{24,       58,       -9.4369707241210E-007}   //43
};

const int MAX_GIBBS_COEFFS_R2_R = 43;

// residual part of dimensionless gibbs free energy in Region2 :   See Equation 17
// Checked OK
double if97_r2_Gamma_r (double if97_pi, double  if97_tau) {
	
	int i;
	double dblGammaSum = 0.0;
	
	
	#pragma omp parallel for reduction(+:dblGammaSum) 	//handle loop multithreaded
	for (i=1; i <= MAX_GIBBS_COEFFS_R2_R; i++) {
		
		dblGammaSum +=   GIBBS_COEFFS_R2_R[i].ni *  pow(if97_pi, GIBBS_COEFFS_R2_R[i].Ii)*  pow((if97_tau - 0.5), GIBBS_COEFFS_R2_R[i].Ji)	;
		}		 
	
return dblGammaSum;
}



// dimensionless gibbs free energy in Region 2 = g/RT:  The fundamental equation of region 2.   See Equation 15
double if97_r2_Gamma (double if97_pi, double  if97_tau) {  
			
		return if97_r2_Gamma_o (if97_pi, if97_tau) + if97_r2_Gamma_r (if97_pi, if97_tau);
}



// [d gamma_o / d pi] keeping tau constant
double if97_r2_GammaPi_o (double if97_pi) {
		double GammaPi_o = 1.0 / if97_pi;
		return GammaPi_o;
}


// [d squared gamma_o / d pi squared] keeping tau constant
double if97_r2_GammaPiPi_o (double if97_pi) {
	return -1.0 / sqr (if97_pi);
}


// [d gamma_o / d tau] keeping pi constant
// Checked OK
double if97_r2_GammaTau_o (double if97_tau) {
	
	int i;
	double dblGammaSum = 0.0;	
		
	#pragma omp parallel for reduction(+:dblGammaSum) 	//handle loop multithreaded		
	for (i=1; i <= MAX_GIBBS_COEFFS_R2_O; i++) {
		dblGammaSum += GIBBS_COEFFS_R2_O[i].ni * GIBBS_COEFFS_R2_O[i].Ji * pow(if97_tau, ( GIBBS_COEFFS_R2_O[i].Ji  - 1.0));
	}
	
return dblGammaSum;	
}


// [d squared gamma_o / d tau squared] keeping pi constant
double if97_r2_GammaTauTau_o (double if97_tau) {
	
	int i;
	double dblGammaSum = 0.0;		

	#pragma omp parallel for reduction(+:dblGammaSum) 	//handle loop multithreaded		
	for (i=1; i <= MAX_GIBBS_COEFFS_R2_O; i++) {
		
		dblGammaSum += GIBBS_COEFFS_R2_O[i].ni * GIBBS_COEFFS_R2_O[i].Ji * ( GIBBS_COEFFS_R2_O[i].Ji  - 1.0) * pow(if97_tau, ( GIBBS_COEFFS_R2_O[i].Ji  - 2.0));
	}		 

return dblGammaSum;	
}



// [d squared gamma_o / d pi d tau] 
const double if97_r2_GammaPiTau_o = 0.0;





// [d gamma_r / d pi] keeping tau constant
// Checked OK
double if97_r2_GammaPi_r (double if97_pi, double if97_tau) {
	
	int i;
	double dblGammaSum = 0.0;		

	#pragma omp parallel for reduction(+:dblGammaSum) 	//handle loop multithreaded		
	for (i=1; i <= MAX_GIBBS_COEFFS_R2_R; i++) {
		
		dblGammaSum += GIBBS_COEFFS_R2_R[i].ni	* GIBBS_COEFFS_R2_R[i].Ii
								* pow( if97_pi, (GIBBS_COEFFS_R2_R[i].Ii - 1.0)) * pow((if97_tau - 0.5), GIBBS_COEFFS_R2_R[i].Ji);
	}
	
return dblGammaSum;
}




// [d squared gamma_r / d pi squared] keeping tau constant

double if97_r2_GammaPiPi_r (double if97_pi, double if97_tau) {
	
	int i;
	double dblGammaSum = 0.0;		
	
	#pragma omp parallel for reduction(+:dblGammaSum) 	//handle loop multithreaded	
	for (i=1; i <= MAX_GIBBS_COEFFS_R2_R; i++) {
		
		dblGammaSum += GIBBS_COEFFS_R2_R[i].ni	* GIBBS_COEFFS_R2_R[i].Ii * (GIBBS_COEFFS_R2_R[i].Ii - 1.0)
								* pow(if97_pi, (GIBBS_COEFFS_R2_R[i].Ii - 2.0)) * pow((if97_tau - 0.5), GIBBS_COEFFS_R2_R[i].Ji);
	}

return dblGammaSum;
}




// [d gamma_r / d tau] keeping pi constant
// Checked OK
double if97_r2_GammaTau_r (double if97_pi, double if97_tau) {
	
	int i;
	double dblGammaSum = 0.0;	
		
	#pragma omp parallel for reduction(+:dblGammaSum) 	//handle loop multithreaded		
	for (i=1; i <= MAX_GIBBS_COEFFS_R2_R; i++) {
		
		dblGammaSum += GIBBS_COEFFS_R2_R[i].ni	* pow( if97_pi, GIBBS_COEFFS_R2_R[i].Ii)
							* GIBBS_COEFFS_R2_R[i].Ji * pow((if97_tau - 0.5), (GIBBS_COEFFS_R2_R[i].Ji - 1.0)); 
	}
	

	
return dblGammaSum;
}



// [d squared gamma_r / d tau squared] keeping pi constant
// Checked OK
double if97_r2_GammaTauTau_r (double if97_pi, double if97_tau) {
	
	int i;
	double dblGammaSum = 0.0;		

	#pragma omp parallel for reduction(+:dblGammaSum) 	//handle loop multithreaded		
	for (i=1; i <= MAX_GIBBS_COEFFS_R2_R; i++) {
		
		dblGammaSum += GIBBS_COEFFS_R2_R[i].ni	* pow( if97_pi, GIBBS_COEFFS_R2_R[i].Ii) * GIBBS_COEFFS_R2_R[i].Ji
								* (GIBBS_COEFFS_R2_R[i].Ji - 1.0)*pow((if97_tau - 0.5), (GIBBS_COEFFS_R2_R[i].Ji - 2.0)); 
	}

return dblGammaSum;
}



// [d squared gamma_r / d tau squared] keeping pi constant
double if97_r2_GammaPiTau_r (double if97_pi, double if97_tau) {
	
	int i;
	double dblGammaSum = 0.0;		

	#pragma omp parallel for reduction(+:dblGammaSum) 	//handle loop multithreaded		
	for (i=1; i <= MAX_GIBBS_COEFFS_R2_R; i++) {
		
		dblGammaSum += GIBBS_COEFFS_R2_R[i].ni	* GIBBS_COEFFS_R2_R[i].Ii * pow(if97_pi, (GIBBS_COEFFS_R2_R[i].Ii - 1.0))
							* GIBBS_COEFFS_R2_R[i].Ji * pow((if97_tau - 0.5), ( GIBBS_COEFFS_R2_R[i].Ji - 1.0)); 
	}

return dblGammaSum;
}



//**********************************************************
//********* REGION 2 PROPERTY EQUATIONS*********************

// specific Gibbs free energy in region 2 (kJ / kg)
double if97_r2_g (double p_MPa , double t_Kelvin) {  
	
	double if97pi = p_MPa / PSTAR_R2;
	double if97tau = TSTAR_R2 / t_Kelvin;
	
return IF97_R * t_Kelvin * if97_r2_Gamma(if97pi, if97tau);
}


// specific volume in region 2  (metres cubed per kilogram)  
// inputs need to convert to pure SI, hence the ´magic´ numbers
// Checked OK
double if97_r2_v (double p_MPa , double t_Kelvin ){
	
	double if97pi = p_MPa / PSTAR_R2;
	double if97tau = TSTAR_R2 / t_Kelvin;
	  
	return (IF97_R *1000 * t_Kelvin / (p_MPa * 1e6) ) * if97pi * ( if97_r2_GammaPi_o(if97pi) + if97_r2_GammaPi_r(if97pi, if97tau));
}


// specific internal energy in region 2 (KJ / Kg)
//Checked OK
double if97_r2_u (double p_MPa , double t_Kelvin ){
	
	double if97pi = p_MPa / PSTAR_R2;
	double if97tau = TSTAR_R2/t_Kelvin;
	
	return (IF97_R* t_Kelvin ) * ((if97tau * (if97_r2_GammaTau_o(if97tau) +  if97_r2_GammaTau_r(if97pi, if97tau))) 
								  - (if97pi * (if97_r2_GammaPi_o(if97pi) + if97_r2_GammaPi_r(if97pi, if97tau)))
								  );
}




// specific entropy in region 2 (KJ / Kg.K)
// Checked OK
double if97_r2_s (double p_MPa , double t_Kelvin ){
	
	double if97pi = p_MPa / PSTAR_R2;
	double if97tau = TSTAR_R2/t_Kelvin;
	
	return (IF97_R ) * (if97tau * (if97_r2_GammaTau_o(if97tau) + if97_r2_GammaTau_r(if97pi,if97tau)) - if97_r2_Gamma(if97pi, if97tau))  ;
}



// specific enthalpy in region 2 (KJ / Kg)
// Checked OK
double if97_r2_h (double p_MPa , double t_Kelvin ){
	
	double if97pi = p_MPa / PSTAR_R2;
	double if97tau = TSTAR_R2/t_Kelvin;
	
	return IF97_R * t_Kelvin * if97tau * (if97_r2_GammaTau_o(if97tau) + if97_r2_GammaTau_r(if97pi, if97tau))   ;
}




// specific isobaric heat capacity in region 2 (KJ / Kg.K)
// Checked OK
double if97_r2_Cp (double p_MPa , double t_Kelvin ){
	
	double if97pi = p_MPa / PSTAR_R2;
	double if97tau = TSTAR_R2/t_Kelvin;
	
	return (-IF97_R * sqr(if97tau) * (if97_r2_GammaTauTau_o(if97tau) + if97_r2_GammaTauTau_r(if97pi, if97tau)))   ;
}



// specific isochoric heat capacity in region 2 (KJ / Kg.K)
double if97_r2_Cv (double p_MPa , double t_Kelvin ){
	
	double if97pi = p_MPa / PSTAR_R2;
	double if97tau = TSTAR_R2 / t_Kelvin;
		
	return IF97_R * ((- sqr(if97tau) * (if97_r2_GammaTauTau_o(if97tau) + if97_r2_GammaTauTau_r(if97pi, if97tau))) - (
	                sqr ( 1.0 + if97pi * if97_r2_GammaPi_r(if97pi, if97tau) - if97tau * if97pi * if97_r2_GammaPiTau_r(if97pi, if97tau))
	                / (1.0 - sqr(if97pi) * if97_r2_GammaPiPi_r (if97pi, if97tau)))
	                ) ;
}


// speed of sound in region 2 (m/s)
// inputs need to convert to pure SI, hence the ´magic´ number 1000
double if97_r2_w (double p_MPa , double t_Kelvin ){
	
	double if97pi = p_MPa / PSTAR_R2;
	double if97tau = TSTAR_R2/t_Kelvin;
	

	
	return sqrt( IF97_R * 1000 * t_Kelvin * ((1.0 + 2.0 * if97pi * if97_r2_GammaPi_r(if97pi, if97tau) + sqr(if97pi) * sqr(if97_r2_GammaPi_r(if97pi, if97tau))) /
					((1.0 - sqr(if97pi) * if97_r2_GammaPiPi_r(if97pi, if97tau)) + 
						( sqr ( 1.0 + if97pi * if97_r2_GammaPi_r(if97pi, if97tau) - if97tau * if97pi * if97_r2_GammaPiTau_r(if97pi, if97tau)) /
							( sqr(if97tau) * (if97_r2_GammaTauTau_o(if97tau) + if97_r2_GammaTauTau_r(if97pi, if97tau)))
						)
					)
				 )
			   );
}

 
 


