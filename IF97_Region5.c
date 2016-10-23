//          Copyright Martin Lord 2014-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


//    IAPWS-IF97 Region 5:  High Temperature Region equations
/* *********************************************************************
 * *******             VALIDITY                             ************
 * 
 * Region 5 of IAPWS-IF97 is valid for the following temperature and 
 * pressure range:
 * 1073.15 K <= T <= 2273.15 K
 * 0 < p <= 50 MPa.
 * 
 * It is only valid for pure undissociated water; any dissociation will 
 * have to be taken into account separately.
 * 
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
#include "IF97_Region5.h"
#include <math.h> // for pow, log
/* #ifdef _OPENMP // multithreading via libgomp
 # include <omp.h>
 #endif
*/
// #include <stdio.h>  used for debugging only


//***************************************************************
//****** REGION 5 *******************************




// See Table 37
const typIF97Coeffs_Jn  GIBBS_COEFFS_R5_O[] = {
	{0,    0.0}  // not used
	,{0,    -0.13179983674201E2} // 1
	,{1,    0.68540841634434E1}
	,{-3,    -0.24805148933466E-1}
	,{-2,    0.36901534980333}
	,{-1,    -0.31161318213925E1}
	,{2,    -0.32961626538917} //6
};

const int MAX_GIBBS_COEFFS_R5_O = 6;


// ideal gas part of dimensionless gibbs free energy in Region5 :   See Equation 33
double if97_r5_Gamma_o (double if97_pi, double  if97_tau) {
	
	int i;
	double dblGammaSum = 0.0;		
	
	#pragma omp parallel for reduction(+:dblGammaSum) 	//handle loop multithreaded		
	for (i=1; i <= MAX_GIBBS_COEFFS_R5_O; i++) {
		
		dblGammaSum += GIBBS_COEFFS_R5_O[i].ni * pow( if97_tau, GIBBS_COEFFS_R5_O[i].Ji);
	}
return log(if97_pi) + dblGammaSum;
}
 



// See Table 38
const typIF97Coeffs_IJn  GIBBS_COEFFS_R5_R[] = {
	 {0,    0,    0.0}  // not used
	,{1,    1,    0.15736404855259E-2}  //1
	,{1,    2,    0.90153761673944E-3}
	,{1,    3,    -0.50270077677648E-2}
	,{2,    3,    0.22440037409485E-5}
	,{2,    9,    -0.41163275453471E-5}
	,{3,    7,    0.37919454822955E-7} //6
};

const int MAX_GIBBS_COEFFS_R5_R = 6;

// residual part of dimensionless gibbs free energy in Region5 :   See Equation 34
double if97_r5_Gamma_r (double if97_pi, double  if97_tau) {
	
	int i;
	double dblGammaSum = 0.0;
	
	
	#pragma omp parallel for reduction(+:dblGammaSum) 	//handle loop multithreaded
	for (i=1; i <= MAX_GIBBS_COEFFS_R5_R; i++) {
		
		dblGammaSum +=   GIBBS_COEFFS_R5_R[i].ni *  pow(if97_pi, GIBBS_COEFFS_R5_R[i].Ii)*  pow(if97_tau, GIBBS_COEFFS_R5_R[i].Ji)	;
		}		 
	
return dblGammaSum;
}

/* dimensionless gibbs free energy in Region 5 = g/RT:  
 * The fundamental equation of region 2.   See Equation 32 */
double if97_r5_Gamma (double if97_pi, double  if97_tau) {  
			
		return if97_r5_Gamma_o (if97_pi, if97_tau) + if97_r5_Gamma_r (if97_pi, if97_tau);
}



// [d gamma_o / d pi] keeping tau constant
double if97_r5_GammaPi_o (double if97_pi) {
		double GammaPi_o = 1.0 / if97_pi;
		return GammaPi_o;
}


// [d squared gamma_o / d pi squared] keeping tau constant
double if97_r5_GammaPiPi_o (double if97_pi) {
	return -1.0 / sqr (if97_pi);
}


// [d gamma_o / d tau] keeping pi constant
// Checked OK
double if97_r5_GammaTau_o (double if97_tau) {
	
	int i;
	double dblGammaSum = 0.0;	
		
	#pragma omp parallel for reduction(+:dblGammaSum) 	//handle loop multithreaded		
	for (i=1; i <= MAX_GIBBS_COEFFS_R5_O; i++) {
		dblGammaSum += GIBBS_COEFFS_R5_O[i].ni * GIBBS_COEFFS_R5_O[i].Ji * pow(if97_tau, ( GIBBS_COEFFS_R5_O[i].Ji  - 1.0));
	}
	
return dblGammaSum;	
}


// [d squared gamma_o / d tau squared] keeping pi constant
double if97_r5_GammaTauTau_o (double if97_tau) {
	
	int i;
	double dblGammaSum = 0.0;		

	#pragma omp parallel for reduction(+:dblGammaSum) 	//handle loop multithreaded		
	for (i=1; i <= MAX_GIBBS_COEFFS_R5_O; i++) {
		
		dblGammaSum += GIBBS_COEFFS_R5_O[i].ni * GIBBS_COEFFS_R5_O[i].Ji * ( GIBBS_COEFFS_R5_O[i].Ji  - 1.0) * pow(if97_tau, ( GIBBS_COEFFS_R5_O[i].Ji  - 2.0));
	}		 

return dblGammaSum;	
}



// [d squared gamma_o / d pi d tau] 
const double if97_r5_GammaPiTau_o = 0.0;




// [d gamma_r / d pi] keeping tau constant
double if97_r5_GammaPi_r (double if97_pi, double if97_tau) {
	
	int i;
	double dblGammaSum = 0.0;		

	#pragma omp parallel for reduction(+:dblGammaSum) 	//handle loop multithreaded		
	for (i=1; i <= MAX_GIBBS_COEFFS_R5_R; i++) {
		
		dblGammaSum += GIBBS_COEFFS_R5_R[i].ni	* GIBBS_COEFFS_R5_R[i].Ii
								* pow( if97_pi, (GIBBS_COEFFS_R5_R[i].Ii - 1.0)) * pow(if97_tau, GIBBS_COEFFS_R5_R[i].Ji);
	}
	
return dblGammaSum;
}



// [d squared gamma_r / d pi squared] keeping tau constant
double if97_r5_GammaPiPi_r (double if97_pi, double if97_tau) {
	
	int i;
	double dblGammaSum = 0.0;		
	
	#pragma omp parallel for reduction(+:dblGammaSum) 	//handle loop multithreaded	
	for (i=1; i <= MAX_GIBBS_COEFFS_R5_R; i++) {
		
		dblGammaSum += GIBBS_COEFFS_R5_R[i].ni	* GIBBS_COEFFS_R5_R[i].Ii * (GIBBS_COEFFS_R5_R[i].Ii - 1.0)
								* pow(if97_pi, (GIBBS_COEFFS_R5_R[i].Ii - 2.0)) * pow(if97_tau, GIBBS_COEFFS_R5_R[i].Ji);
	}

return dblGammaSum;
}



// [d gamma_r / d tau] keeping pi constant
double if97_r5_GammaTau_r (double if97_pi, double if97_tau) {
	
	int i;
	double dblGammaSum = 0.0;	
		
	#pragma omp parallel for reduction(+:dblGammaSum) 	//handle loop multithreaded		
	for (i=1; i <= MAX_GIBBS_COEFFS_R5_R; i++) {
		
		dblGammaSum += GIBBS_COEFFS_R5_R[i].ni	* pow( if97_pi, GIBBS_COEFFS_R5_R[i].Ii)
							* GIBBS_COEFFS_R5_R[i].Ji * pow(if97_tau, (GIBBS_COEFFS_R5_R[i].Ji - 1.0)); 
	}
	
return dblGammaSum;
}


// [d squared gamma_r / d tau squared] keeping pi constant
double if97_r5_GammaTauTau_r (double if97_pi, double if97_tau) {
	
	int i;
	double dblGammaSum = 0.0;		

	#pragma omp parallel for reduction(+:dblGammaSum) 	//handle loop multithreaded		
	for (i=1; i <= MAX_GIBBS_COEFFS_R5_R; i++) {
		
		dblGammaSum += GIBBS_COEFFS_R5_R[i].ni	* pow( if97_pi, GIBBS_COEFFS_R5_R[i].Ii) * GIBBS_COEFFS_R5_R[i].Ji
								* (GIBBS_COEFFS_R5_R[i].Ji - 1.0) * pow(if97_tau, (GIBBS_COEFFS_R5_R[i].Ji - 2.0)); 
	}

return dblGammaSum;
}



// [d squared gamma_r / d tau squared] keeping pi constant
double if97_r5_GammaPiTau_r (double if97_pi, double if97_tau) {
	
	int i;
	double dblGammaSum = 0.0;		

	#pragma omp parallel for reduction(+:dblGammaSum) 	//handle loop multithreaded		
	for (i=1; i <= MAX_GIBBS_COEFFS_R5_R; i++) {
		
		dblGammaSum += GIBBS_COEFFS_R5_R[i].ni	* GIBBS_COEFFS_R5_R[i].Ii * pow(if97_pi, (GIBBS_COEFFS_R5_R[i].Ii - 1.0))
							* GIBBS_COEFFS_R5_R[i].Ji * pow(if97_tau, ( GIBBS_COEFFS_R5_R[i].Ji - 1.0)); 
	}

return dblGammaSum;
}




//**********************************************************
//********* REGION 5 PROPERTY EQUATIONS*********************

// specific Gibbs free energy in region 5 (kJ / kg)
double if97_r5_g (double p_MPa , double t_Kelvin) {  
	
	double if97pi = p_MPa / PSTAR_R5;
	double if97tau = TSTAR_R5 / t_Kelvin;
	
return IF97_R * t_Kelvin * if97_r5_Gamma(if97pi, if97tau);
}


// specific volume in region 5  (metres cubed per kilogram)  
// inputs need to convert to pure SI, hence the ´magic´ numbers
// Checked OK
double if97_r5_v (double p_MPa , double t_Kelvin ){
	
	double if97pi = p_MPa / PSTAR_R5;
	double if97tau = TSTAR_R5 / t_Kelvin;
	  
	return (IF97_R *1000 * t_Kelvin / (p_MPa * 1e6) ) * if97pi * ( if97_r5_GammaPi_o(if97pi) + if97_r5_GammaPi_r(if97pi, if97tau));
}


// specific internal energy in region 5 (KJ / Kg)
// Checked OK
double if97_r5_u (double p_MPa , double t_Kelvin ){
	
	double if97pi = p_MPa / PSTAR_R5;
	double if97tau = TSTAR_R5/t_Kelvin;
	
	return (IF97_R* t_Kelvin ) * ((if97tau * (if97_r5_GammaTau_o(if97tau) +  if97_r5_GammaTau_r(if97pi, if97tau))) 
								  - (if97pi * (if97_r5_GammaPi_o(if97pi) + if97_r5_GammaPi_r(if97pi, if97tau)))
								  );
}



// specific entropy in region 5 (KJ / Kg.K)
// Checked OK
double if97_r5_s (double p_MPa , double t_Kelvin ){
	
	double if97pi = p_MPa / PSTAR_R5;
	double if97tau = TSTAR_R5/t_Kelvin;
	
	return (IF97_R ) * (if97tau * (if97_r5_GammaTau_o(if97tau) + if97_r5_GammaTau_r(if97pi,if97tau)) - if97_r5_Gamma(if97pi, if97tau))  ;
}


// specific enthalpy in region 5 (KJ / Kg)
// Checked OK
double if97_r5_h (double p_MPa , double t_Kelvin ){
	
	double if97pi = p_MPa / PSTAR_R5;
	double if97tau = TSTAR_R5/t_Kelvin;
	
	return IF97_R * t_Kelvin * if97tau * (if97_r5_GammaTau_o(if97tau) + if97_r5_GammaTau_r(if97pi, if97tau))   ;
}





// specific isobaric heat capacity in region 5 (KJ / Kg.K)
// Checked OK
double if97_r5_Cp (double p_MPa , double t_Kelvin ){
	
	double if97pi = p_MPa / PSTAR_R5;
	double if97tau = TSTAR_R5/t_Kelvin;
	
	return (-IF97_R * sqr(if97tau) * (if97_r5_GammaTauTau_o(if97tau) + if97_r5_GammaTauTau_r(if97pi, if97tau)))   ;
}



// specific isochoric heat capacity in region 5 (KJ / Kg.K)
double if97_r5_Cv (double p_MPa , double t_Kelvin ){
	
	double if97pi = p_MPa / PSTAR_R5;
	double if97tau = TSTAR_R5 / t_Kelvin;
		
	return IF97_R * ((- sqr(if97tau) * (if97_r5_GammaTauTau_o(if97tau) + if97_r5_GammaTauTau_r(if97pi, if97tau))) - (
	                sqr ( 1.0 + if97pi * if97_r5_GammaPi_r(if97pi, if97tau) - if97tau * if97pi * if97_r5_GammaPiTau_r(if97pi, if97tau))
	                / (1.0 - sqr(if97pi) * if97_r5_GammaPiPi_r (if97pi, if97tau)))
	                ) ;
}


// speed of sound in region 5 (m/s)
// inputs need to convert to pure SI, hence the ´magic´ number 1000
// checked OK
double if97_r5_w (double p_MPa , double t_Kelvin ){
	
	double if97pi = p_MPa / PSTAR_R5;
	double if97tau = TSTAR_R5/t_Kelvin;
	

	
	return sqrt( IF97_R * 1000 * t_Kelvin * ((1.0 + 2.0 * if97pi * if97_r5_GammaPi_r(if97pi, if97tau) + sqr(if97pi) * sqr(if97_r5_GammaPi_r(if97pi, if97tau))) /
					((1.0 - sqr(if97pi) * if97_r5_GammaPiPi_r(if97pi, if97tau)) + 
						( sqr ( 1.0 + if97pi * if97_r5_GammaPi_r(if97pi, if97tau) - if97tau * if97pi * if97_r5_GammaPiTau_r(if97pi, if97tau)) /
							( sqr(if97tau) * (if97_r5_GammaTauTau_o(if97tau) + if97_r5_GammaTauTau_r(if97pi, if97tau)))
						)
					)
				 )
			   );
}

 


