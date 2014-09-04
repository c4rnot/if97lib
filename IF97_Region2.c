//          Copyright Martin Lord 2014-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


//    IAPWS-IF97 Region 2 Equations
// VALIDITY x K <= T <= x K     x <= p <= x MPa


#include "IF97_constants.h"
#include "IF97_Region1.h"
#include <math.h> // for pow, log


const double PSTAR_R2 = 1.0;  // MPa
const double TSTAR_R2 = 540.0 ; // K

// squares a double without using pow
double sqr (double dblArg){	return dblArg * dblArg; }


//***************************************************************
//****** REGION 2 GIBBS FREE ENERGY AND DERIVATIVES**************

typedef struct sctGibbsCoeff_o {
	int Ji;
	double ni;
} typR2coeff_o;


// see Table 10
const typR2coeff_o  GIBBS_COEFFS_R2_O[] = {
	 {0,		 0.0} 				   //0  i starts at 1, so 0th i is not used
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
double if97_r2_Gamma_o (double if97_pi, double  if97_tau) {
	
	int i;
	double dblGammaSum = 0.0;		
		
	for (i=1; i <= MAX_GIBBS_COEFFS_R2_O; i++) {
		dblGammaSum = dblGammaSum + GIBBS_COEFFS_R2_O[i].ni * pow( if97_tau, GIBBS_COEFFS_R2_O[i].Ji);
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
	,{1,   0,   -0.0017731742}  //1
	,{1,   1,   -0.0178348623}
	,{1,   2,   -0.0459960137}
	,{1,   3,   -0.0575812591}
	,{1,   6,   -0.0503252787}
	,{2,   1,   -3.3032641670203E-005}
	,{2,   2,   -0.0001894899}
	,{2,   4,   -0.0039392777}
	,{2,   7,   -0.0437972957}
	,{2,   36,   -2.6674547914087E-005}
	,{3,   0,   2.0481737692309E-008}
	,{3,   1,   4.3870667284435E-007}
	,{3,   3,   -3.227767723857E-005}
	,{3,   6,   -0.0015033925}
	,{3,   35,   -0.0406682536}
	,{4,   1,   -7.8847309559367E-010}
	,{4,   2,   1.2790717852285E-008}
	,{4,   3,   4.8225372718507E-007}
	,{5,   7,   2.2922076337661E-006}
	,{6,   3,   -1.6714766451061E-011}
	,{6,   16,   -0.0021171472}
	,{6,   35,   -23.8957419341}
	,{7,   0,   -5.905956432427E-018}
	,{7,   11,   -1.2621808899101E-006}
	,{7,   25,   -0.0389468424}
	,{8,   8,   1.1256211360459E-011}
	,{8,   36,   -8.2311340898}
	,{9,   13,   1.9809712802088E-008}
	,{10,   4,   1.0406965210174E-019}
	,{10,   10,   -1.0234747095929E-013}
	,{10,   14,   -0.000000001}
	,{16,   29,   -8.0882908646985E-011}
	,{16,   50,   0.1069303188}
	,{18,   57,   -0.3366225057}
	,{20,   20,   8.9185845355421E-025}
	,{20,   35,   3.0629316876232E-013}
	,{20,   48,   -4.2002467698208E-006}
	,{21,   21,   -5.9056029685639E-026}
	,{22,   53,   3.7826947613457E-006}
	,{23,   39,   -1.2768608934681E-015}
	,{24,   26,   7.3087610595061E-029}
	,{24,   40,   5.5414715350778E-017}
	,{24,   58,   -9.436970724121E-007}  //43
};

const int MAX_GIBBS_COEFFS_R2_R = 43;

// residual part of dimensionless gibbs free energy in Region2 :   See Equation 17
double if97_r2_Gamma_r (double if97_pi, double  if97_tau) {
	
	int i;
	double dblGammaSum = 0.0;		
		
	for (i=1; i <= MAX_GIBBS_COEFFS_R2_R; i++) {
		dblGammaSum = dblGammaSum + GIBBS_COEFFS_R2_R[i].ni * GIBBS_COEFFS_R2_R[i].Ii * pow( (if97_tau - 0.5), GIBBS_COEFFS_R2_R[i].Ji);
	}		 
		
return dblGammaSum;
}



// dimensionless gibbs free energy in Region 2 = g/RT:  The fundamental equation of region 2.   See Equation 15
double if97_r2_Gamma (double if97_pi, double  if97_tau) {  
		return if97_r2_Gamma_o (if97_pi, if97_tau) + if97_r2_Gamma_r (if97_pi, if97_tau);
}



// [d gamma_o / d pi] keeping tau constant
double if97_r2_GammaPi_o (double if97_pi) {
	return 1.0 / if97_pi;
}


// [d squared gamma_o / d pi squared] keeping tau constant
double if97_r2_GammaPiPi_o (double if97_pi) {
	return -1.0 / sqr (if97_pi);
}


// [d gamma_o / d tau] keeping pi constant
double if97_r2_GammaTau_o (double if97_tau) {
	
	int i;
	double dblGammaSum = 0.0;		
		
	for (i=1; i <= MAX_GIBBS_COEFFS_R2_O; i++) {
		dblGammaSum = dblGammaSum + GIBBS_COEFFS_R2_O[i].ni * GIBBS_COEFFS_R2_O[i].Ji * pow(if97_tau, ( GIBBS_COEFFS_R2_O[i].Ji  - 1.0));
	}		 
		
return dblGammaSum;	
}


// [d squared gamma_o / d tau squared] keeping pi constant
double if97_r2_GammaTauTau_o (double if97_tau) {
	
	int i;
	double dblGammaSum = 0.0;		
		
	for (i=1; i <= MAX_GIBBS_COEFFS_R2_O; i++) {
		dblGammaSum = dblGammaSum + GIBBS_COEFFS_R2_O[i].ni * GIBBS_COEFFS_R2_O[i].Ji * ( GIBBS_COEFFS_R2_O[i].Ji  - 1.0) * pow(if97_tau, ( GIBBS_COEFFS_R2_O[i].Ji  - 2.0));
	}		 
		
return dblGammaSum;	
}



// [d squared gamma_o / d pi d tau] 
const double if97_r2_GammaPiTau_o = 0.0;





// [d gamma_r / d pi] keeping tau constant
double if97_r2_GammaPi_r (double if97_pi, double if97_tau) {
	
	int i;
	double dblGammaSum = 0.0;		
		
	for (i=1; i <= MAX_GIBBS_COEFFS_R2_R; i++) {
		dblGammaSum = dblGammaSum + GIBBS_COEFFS_R2_R[i].ni	* GIBBS_COEFFS_R2_R[i].Ii
								* pow( if97_pi, (GIBBS_COEFFS_R2_R[i].Ii - 1.0)) * pow((if97_tau - 0.5), GIBBS_COEFFS_R2_R[i].Ji);
	}
	
return dblGammaSum;
}




// [d squared gamma_r / d pi squared] keeping tau constant
double if97_r2_GammaPiPi_r (double if97_pi, double if97_tau) {
	
	int i;
	double dblGammaSum = 0.0;		
		
	for (i=1; i <= MAX_GIBBS_COEFFS_R2_R; i++) {
		dblGammaSum = dblGammaSum + GIBBS_COEFFS_R2_R[i].ni	* GIBBS_COEFFS_R2_R[i].Ii * (GIBBS_COEFFS_R2_R[i].Ii - 1.0)
								* pow(if97_pi, (GIBBS_COEFFS_R2_R[i].Ii - 2.0)) * pow((if97_tau - 0.5), GIBBS_COEFFS_R2_R[i].Ji);
	}
	
return dblGammaSum;
}




// [d gamma_r / d tau] keeping pi constant
double if97_r2_GammaTau_r (double if97_pi, double if97_tau) {
	
	int i;
	double dblGammaSum = 0.0;		
		
	for (i=1; i <= MAX_GIBBS_COEFFS_R2_R; i++) {
		dblGammaSum = dblGammaSum + GIBBS_COEFFS_R2_R[i].ni	* pow( if97_pi, GIBBS_COEFFS_R2_R[i].Ii)
								* pow((if97_tau - 0.5), (GIBBS_COEFFS_R2_R[i].Ji - 1.0)); 
	}
	
return dblGammaSum;
}



// [d squared gamma_r / d tau squared] keeping pi constant
double if97_r2_GammaTauTau_r (double if97_pi, double if97_tau) {
	
	int i;
	double dblGammaSum = 0.0;		
		
	for (i=1; i <= MAX_GIBBS_COEFFS_R2_R; i++) {
		dblGammaSum = dblGammaSum + GIBBS_COEFFS_R2_R[i].ni	* pow( if97_pi, GIBBS_COEFFS_R2_R[i].Ii) * GIBBS_COEFFS_R2_R[i].Ji
								* pow((if97_tau - 0.5), (GIBBS_COEFFS_R2_R[i].Ji - 2.0)); 
	}
	
return dblGammaSum;
}



// [d squared gamma_r / d tau squared] keeping pi constant
double if97_r2_GammaPiTau_r (double if97_pi, double if97_tau) {
	
	int i;
	double dblGammaSum = 0.0;		
		
	for (i=1; i <= MAX_GIBBS_COEFFS_R2_R; i++) {
		dblGammaSum = dblGammaSum + GIBBS_COEFFS_R2_R[i].ni	* GIBBS_COEFFS_R2_R[i].Ii * pow(if97_pi, (GIBBS_COEFFS_R2_R[i].Ii - 1.0))
							* GIBBS_COEFFS_R2_R[i].Ji * pow((if97_tau - 0.5), ( GIBBS_COEFFS_R2_R[i].Ji - 1.0)); 
	}
	
return dblGammaSum;
}



//**********************************************************
//********* REGION 2 PROPERTY EQUATIONS*********************

// specific Gibbs free energy in region 2 (kJ / kg)
double if97_r1_g (double p_MPa , double t_Kelvin) {  
	
	double if97pi = p_MPa / PSTAR_R2;
	double if97tau = TSTAR_R2 / t_Kelvin;
	
return IF97_R * t_Kelvin * if97_r2_Gamma(if97pi, if97tau);
}


// specific volume in region 2  (metres cubed per kilogram)  
// inputs need to convert to pure SI, hence the ´magic´ numbers
double if97_r2_v (double p_MPa , double t_Kelvin ){
	
	double if97pi = p_MPa / PSTAR_R2;
	double if97tau = TSTAR_R2 / t_Kelvin;
	  
	return (IF97_R *1000 * t_Kelvin / (p_MPa * 1e6) ) * if97pi * ( if97_r2_GammaPi_o(if97pi) + if97_r2_GammaPi_r(if97pi, if97tau));
}


// specific internal energy in region 2 (KJ / Kg)
double if97_r2_u (double p_MPa , double t_Kelvin ){
	
	double if97pi = p_MPa / PSTAR_R2;
	double if97tau = TSTAR_R2/t_Kelvin;
	
	return (IF97_R* t_Kelvin ) * (if97tau * (if97_r2_GammaTau_o(if97tau) +  if97_r2_GammaTau_r(if97pi, if97tau)) 
								  - if97pi * (if97_r2_GammaPi_o(if97pi) - if97_r2_GammaPi_r(if97pi, if97tau))
								  );
}




// specific entropy in region 2 (KJ / Kg.K)
double if97_r2_s (double p_MPa , double t_Kelvin ){
	
	double if97pi = p_MPa / PSTAR_R2;
	double if97tau = TSTAR_R2/t_Kelvin;
	
	return (IF97_R ) * (if97tau * (if97_r2_GammaTau_o(if97tau) - if97_r2_GammaTau_r(if97pi,if97tau)) - if97_r2_Gamma(if97pi, if97tau))  ;
}



// specific enthalpy in region 2 (KJ / Kg)
double if97_r2_h (double p_MPa , double t_Kelvin ){
	
	double if97pi = p_MPa / PSTAR_R2;
	double if97tau = TSTAR_R2/t_Kelvin;
	
	return IF97_R * t_Kelvin * if97tau * (if97_r2_GammaTau_o(if97tau) + if97_r2_GammaTau_r(if97pi, if97tau))   ;
}




// specific isobaric heat capacity in region 2 (KJ / Kg.K)
double if97_r2_Cp (double p_MPa , double t_Kelvin ){
	
	double if97pi = p_MPa / PSTAR_R2;
	double if97tau = TSTAR_R2/t_Kelvin;
	
	return (-IF97_R * sqr(if97tau) * (if97_r2_GammaTauTau_o(if97tau) + if97_r2_GammaTauTau_r(if97pi, if97tau)))   ;
}



// specific isochoric heat capacity in region 2 (KJ / Kg.K)
double if97_r2_Cv (double p_MPa , double t_Kelvin ){
	
	double if97pi = p_MPa / PSTAR_R2;
	double if97tau = TSTAR_R2 / t_Kelvin;
		
	return IF97_R * (-(sqr(if97tau) * (if97_r2_GammaTauTau_o(if97tau) + if97_r2_GammaTauTau_r(if97pi, if97tau)) - (
	                sqr ( 1.0 + if97pi * if97_r2_GammaPi_r(if97pi, if97tau) - if97tau * if97pi * if97_r2_GammaPiTau_r(if97pi, if97tau))
	                / (1.0 - sqr(if97pi * if97_r2_GammaPiPi_r (if97pi, if97tau))))
	                )) ;
}


// speed of sound in region 2 (m/s)
// inputs need to convert to pure SI, hence the ´magic´ number
double if97_r2_w (double p_MPa , double t_Kelvin ){
	
	double if97pi = p_MPa / PSTAR_R2;
	double if97tau = TSTAR_R2/t_Kelvin;
	

	
	return sqrt( (IF97_R * 1000 * t_Kelvin * (1.0 + 2.0 * if97pi * if97_r2_GammaPi_r(if97pi, if97tau) + sqr(if97pi) * sqr(if97_r2_GammaPi_r(if97pi, if97tau))) /
					((1.0 - sqr(if97pi) * if97_r2_GammaPiPi_r(if97pi, if97tau)) + 
						( sqr ( 1.0 + if97pi * if97_r2_GammaPi_r(if97pi, if97tau) - if97tau * if97pi * if97_r2_GammaPiTau_r(if97pi, if97tau)) /
							sqr(if97tau) * (if97_r2_GammaTauTau_o(if97tau) + if97_r2_GammaTauTau_r(if97pi, if97tau))
						)
					)
				 )
			   );
}









