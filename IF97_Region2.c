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




// see Table 10
const typIF97Coeffs_Jn  GIBBS_COEFFS_R2_O[] = {
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





// See table 11
const typIF97Coeffs_IJn  GIBBS_COEFFS_R2_R[] = {
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

 
 
/* *******************************************************************
 * Region 2 Backwards Equations
 */
 
 // Region 2a up to 4 MPa
 // Region 2b  above 5.85 kJ/kg.K
 // Region 2c Below 5.85  kJ/kg.K
 
 
 
 /* B2bc equation   determines whether we 
  * are in region 2b or c for a 
  * 
  */

const double IF97_B2bc_n[] = {
	0						//n0 unused
	, 0.90584278514723e3	//n1
	, -0.67955786399241
	, 0.12809002730136e-3
	, 0.26526571908428e4
	, 0.45257578905948e1}	;	//n5
  
/* returns the pressure of the 2b-2c boundary in MPa for a given 
 * enthalpy in kJ/kg */
double IF97_B2bc_p (double h_kJperKg){
	//h_star = 1 kJ/kg, so this constant is ignored
	return IF97_B2bc_n[1] + IF97_B2bc_n[2] * h_kJperKg + IF97_B2bc_n[3] * sqr(h_kJperKg) ;
}

/* returns the enthalpy of the 2b-2c boundary in kJ/kg for a given 
 * pressure in MPa */
double IF97_B2bc_h (double p_MPa){
	// p_star = 1
	return IF97_B2bc_n[4] + sqrt((p_MPa - IF97_B2bc_n[5]) / IF97_B2bc_n[3]);
}






// See table 20
const typIF97Coeffs_IJn  COEFFS_R2A_BW_PH[] = {
	{0,		0,		0.0}   // 0 not used
	,{0,    0,    0.10898952318288E4}  //1
	,{0,    1,    0.84951654495535E3}
	,{0,    2,    -0.10781748091826E3}
	,{0,    3,    0.33153654801263E2}
	,{0,    7,    -0.74232016790248E1}
	,{0,    20,    0.11765048724356E2}
	,{1,    0,    0.18445749355790E1}
	,{1,    1,    -0.41792700549624E1}
	,{1,    2,    0.62478196935812E1}
	,{1,    3,    -0.17344563108114E2}
	,{1,    7,    -0.20058176862096E3}
	,{1,    9,    0.27196065473796E3}
	,{1,    11,    -0.45511318285818E3}
	,{1,    18,    0.30919688604755E4}
	,{1,    44,    0.25226640357872E6}
	,{2,    0,    -0.61707422868339E-2}
	,{2,    2,    -0.31078046629583}
	,{2,    7,    0.11670873077107E2}
	,{2,    36,    0.12812798404046E9}
	,{2,    38,    -0.98554909623276E9}
	,{2,    40,    0.28224546973002E10}
	,{2,    42,    -0.35948971410703E10}
	,{2,    44,    0.17227349913197E10}
	,{3,    24,    -0.13551334240775E5}
	,{3,    44,    0.12848734664650E8}
	,{4,    12,    0.13865724283226E1}
	,{4,    32,    0.23598832556514E6}
	,{4,    44,    -0.13105236545054E8}
	,{5,    32,    0.73999835474766E4}
	,{5,    36,    -0.55196697030060E6}
	,{5,    42,    0.37154085996233E7}
	,{6,    34,    0.19127729239660E5}
	,{6,    44,    -0.41535164835634E6}
	,{7,    28,    -0.62459855192507E2}  //34
};

const int MAX_COEFFS_R2A_BW_PH = 34;

//returns temperature (K) in region 2a for a given pressure and enhalpy
//
double if97_r2a_t_ph (double p_MPa, double h_kJperkg ) {
	double eta = h_kJperkg / HSTAR_R2_BW ;
	double pi = p_MPa ;

	int i;
	double dblTSum =0.0;

	#pragma omp parallel for reduction(+:dblTSum) 	//handle loop multithreaded	
	for (i=1; i <= MAX_COEFFS_R2A_BW_PH; i++) {
		
	dblTSum += COEFFS_R2A_BW_PH[i].ni * pow(pi, COEFFS_R2A_BW_PH[i].Ii) * pow(eta - 2.1, COEFFS_R2A_BW_PH[i].Ji)  ;
	}	
	
// no need to multiply by TSTAR since it is 1	
return dblTSum ;
}


// See table 21
const typIF97Coeffs_IJn  COEFFS_R2B_BW_PH[] = {
	{0,     0,    0.0 }  // unused
	,{0,    0,    0.14895041079516E4} // 1
	,{0,    1,    0.74307798314034E3}
	,{0,    2,    -0.97708318797837E2}
	,{0,    12,    0.24742464705674E1}
	,{0,    18,    -0.63281320016026}
	,{0,    24,    0.11385952129658E1}
	,{0,    28,    -0.47811863648625}
	,{0,    40,    0.85208123431544E-2}
	,{1,    0,    0.93747147377932}
	,{1,    2,    0.33593118604916E1}
	,{1,    6,    0.33809355601454E1}
	,{1,    12,    0.16844539671904}
	,{1,    18,    0.73875745236695}
	,{1,    24,    -0.47128737436186}
	,{1,    28,    0.15020273139707}
	,{1,    40,    -0.21764114219750E-2}
	,{2,    2,    -0.21810755324761E-1}
	,{2,    8,    -0.10829784403677}
	,{2,    18,    -0.46333324635812E-1}
	,{2,    40,    0.71280351959551E-4}
	,{3,    1,    0.11032831789999E-3}
	,{3,    2,    0.18955248387902E-3}
	,{3,    12,    0.30891541160537E-2}
	,{3,    24,    0.13555504554949E-2}
	,{4,    2,    0.28640237477456E-6}
	,{4,    12,    -0.10779857357512E-4}
	,{4,    18,    -0.76462712454814E-4}
	,{4,    24,    0.14052392818316E-4}
	,{4,    28,    -0.31083814331434E-4}
	,{4,    40,    -0.10302738212103E-5}
	,{5,    18,    0.28217281635040E-6}
	,{5,    24,    0.12704902271945E-5}
	,{5,    40,    0.73803353468292E-7}
	,{6,    28,    -0.11030139238909E-7}
	,{7,    2,    -0.81456365207833E-13}
	,{7,    28,    -0.25180545682962E-10}
	,{9,    1,    -0.17565233969407E-17}
	,{9,    40,    0.86934156344163E-14} //38
};

const int MAX_COEFFS_R2B_BW_PH = 38;

//returns temperature (K) in region 2b for a given pressure and enhalpy
//
double if97_r2b_t_ph (double p_MPa, double h_kJperkg ) {
	double eta = h_kJperkg / HSTAR_R2_BW ;
	double pi = p_MPa ;

	int i;
	double dblTSum =0.0;

	#pragma omp parallel for reduction(+:dblTSum) 	//handle loop multithreaded	
	for (i=1; i <= MAX_COEFFS_R2B_BW_PH; i++) {
		
	dblTSum += COEFFS_R2B_BW_PH[i].ni * pow(pi - 2.0, COEFFS_R2B_BW_PH[i].Ii) * pow(eta - 2.6, COEFFS_R2B_BW_PH[i].Ji) ;
	}	
	
// no need to multiply by TSTAR since it is 1	
return dblTSum ;
}





// See table 22
const typIF97Coeffs_IJn  COEFFS_R2C_BW_PH[] = {
	 {0,     0,    0.0}
	,{-7,    0,    -0.32368398555242E13}
	,{-7,    4,    0.73263350902181E13}
	,{-6,    0,    0.35825089945447E12}
	,{-6,    2,    -0.58340131851590E12}
	,{-5,    0,    -0.10783068217470E11}
	,{-5,    2,    0.20825544563171E11}
	,{-2,    0,    0.61074783564516E6}
	,{-2,    1,    0.85977722535580E6}
	,{-1,    0,    -0.25745723604170E5}
	,{-1,    2,    0.31081088422714E5}
	,{0,    0,    0.12082315865936E4}
	,{0,    1,    0.48219755109255E3}
	,{1,    4,    0.37966001272486E1}
	,{1,    8,    -0.10842984880077E2}
	,{2,    4,    -0.45364172676660E-1}
	,{6,    0,    0.14559115658698E-12}
	,{6,    1,    0.11261597407230E-11}
	,{6,    4,    -0.17804982240686E-10}
	,{6,    10,    0.12324579690832E-6}
	,{6,    12,    -0.11606921130984E-5}
	,{6,    16,    0.27846367088554E-4}
	,{6,    20,    -0.59270038474176E-3}
	,{6,    22,    0.12918582991878E-2}
};

const int MAX_COEFFS_R2C_BW_PH = 22;

//returns temperature (K) in region 2c for a given pressure and enhalpy
//
double if97_r2c_t_ph (double p_MPa, double h_kJperkg ) {
	double eta = h_kJperkg / HSTAR_R2_BW ;
	double pi = p_MPa ;

	int i;
	double dblTSum =0.0;

	#pragma omp parallel for reduction(+:dblTSum) 	//handle loop multithreaded	
	for (i=1; i <= MAX_COEFFS_R2C_BW_PH; i++) {
		
	dblTSum += COEFFS_R2C_BW_PH[i].ni * pow(pi + 25.0, COEFFS_R2C_BW_PH[i].Ii) * pow(eta - 1.8, COEFFS_R2C_BW_PH[i].Ji) ;
	}	
	
// no need to multiply by TSTAR since it is 1	
return dblTSum ;
}
