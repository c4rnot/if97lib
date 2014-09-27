//          Copyright Martin Lord 2014-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


//    IAPWS-IF97 Region 2: metastable vapour region <= 10 MPa equations
/* *********************************************************************
 * *******             VALIDITY                             ************
 * 
 * Valid in the metastable vapour region  from the saturated vapour line 
 * to the 5% equilibrium moisture line (determined from the equilibrium 
 * h' and h'' values calculated at the given pressure) for pressures 
 * from the triple poiint to 10 MPa

 *  611.657 Pa <= p <= 10 MPa
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
  * ***************************************************************** */

#include "IF97_constants.h"  //PSTAR TSTAR & sqr
#include "IF97_Region2_met.h"
#include <math.h> // for pow, log
/* #ifdef _OPENMP // multithreading via libgomp
 # include <omp.h>
 #endif
*/
 #include <stdio.h>  //used for debugging only



//***************************************************************
//****** REGION 2 GIBBS FREE ENERGY AND DERIVATIVES**************

typedef struct sctGibbsCoeff_o {
	int Ji;
	double ni;
} typR2coeff_o;


// see Table 10
const typR2coeff_o  GIBBS_COEFFS_R2MET_O[] = {
	 {0,		 0.0} 			 //0  i starts at 1, so 0th i is not used
	,{ 0 , -9.6937268393049}   // 1  Different from r2
	,{ 1 , 10.087275970006 }  // 2 Different from r2
	,{ -5 , -0.0056087911283 }
	,{ -4 , 0.0714527380815 }
	,{ -3 , -0.4071049822393 }
	,{ -2 , 1.4240819171444 }
	,{ -1 , -4.383951131945 }
	,{ 2 , -0.2840863246077 }
	,{ 3 , 0.0212684637533 } //9
};

const int MAX_GIBBS_COEFFS_R2MET_O = 9;


// ideal gas part of dimensionless gibbs free energy in Region2 :   See Equation 16
// checked OK
double if97_r2met_Gamma_o (double if97_pi, double  if97_tau) {
	
	int i;
	double dblGammaSum = 0.0;		
	
	#pragma omp parallel for reduction(+:dblGammaSum) 	//handle loop multithreaded		
	for (i=1; i <= MAX_GIBBS_COEFFS_R2MET_O; i++) {
		

		
		dblGammaSum += GIBBS_COEFFS_R2MET_O[i].ni * pow( if97_tau, GIBBS_COEFFS_R2MET_O[i].Ji);
	}		 
	


	
return log(if97_pi) + dblGammaSum;
}



typedef struct sctGibbsCoeff_r {
	int Ii;
	int Ji;
	double ni;
} typR2coeff_r;


// See table 16
const typR2coeff_r  GIBBS_COEFFS_R2MET_R[] = {
	{0,	 0,	 0.0} 				   //0  i starts at 1, so 0th i is not used
	,{1,     0,     -0.73362260186506E-2}
	,{1,     2,     -0.88223831943146E-1}
	,{1,     5,     -0.72334555213245E-1}
	,{1,     11,     -0.40813178534455E-2}
	,{2,     1,     0.20097803380207E-2}
	,{2,     7,     -0.53045921898642E-1}
	,{2,     16,     -0.76190409086970E-2}
	,{3,     4,     -0.63498037657313E-2}
	,{3,     16,     -0.86043093028588E-1}
	,{4,     7,     0.75321581522770E-2}
	,{4,     10,     -0.79238375446139E-2}
	,{5,     9,     -0.22888160778447E-3}
	,{5,     10,     -0.26456501482810E-2}   //13
};

const int MAX_GIBBS_COEFFS_R2MET_R = 13;

// residual part of dimensionless gibbs free energy in Region2 :   See Equation 17
// Checked OK
double if97_r2met_Gamma_r (double if97_pi, double  if97_tau) {
	
	int i;
	double dblGammaSum = 0.0;
	
	
	#pragma omp parallel for reduction(+:dblGammaSum) 	//handle loop multithreaded
	for (i=1; i <= MAX_GIBBS_COEFFS_R2MET_R; i++) {
		
		dblGammaSum +=   GIBBS_COEFFS_R2MET_R[i].ni *  pow(if97_pi, GIBBS_COEFFS_R2MET_R[i].Ii)*  pow((if97_tau - 0.5), GIBBS_COEFFS_R2MET_R[i].Ji)	;
		}		 
	
return dblGammaSum;
}



// dimensionless gibbs free energy in Region 2 = g/RT:  The fundamental equation of region 2.   See Equation 15
double if97_r2met_Gamma (double if97_pi, double  if97_tau) {  
			
		return if97_r2met_Gamma_o (if97_pi, if97_tau) + if97_r2met_Gamma_r (if97_pi, if97_tau);
}



// [d gamma_o / d pi] keeping tau constant
double if97_r2met_GammaPi_o (double if97_pi) {
		double GammaPi_o = 1.0 / if97_pi;
		return GammaPi_o;
}


// [d squared gamma_o / d pi squared] keeping tau constant
double if97_r2met_GammaPiPi_o (double if97_pi) {
	return -1.0 / sqr (if97_pi);
}


// [d gamma_o / d tau] keeping pi constant
// Checked OK
double if97_r2met_GammaTau_o (double if97_tau) {
	
	int i;
	double dblGammaSum = 0.0;	
	double chunk = 0.0;
		
	#pragma omp parallel for reduction(+:dblGammaSum) 	//handle loop multithreaded		
	for (i=1; i <= MAX_GIBBS_COEFFS_R2MET_O; i++) {
		
		chunk = GIBBS_COEFFS_R2MET_O[i].ni * GIBBS_COEFFS_R2MET_O[i].Ji * pow(if97_tau, ( GIBBS_COEFFS_R2MET_O[i].Ji  - 1.0));
		
		dblGammaSum += chunk;
		//printf ("i\t%d\tchunk\t%e\tni\t%e\n", i, chunk, GIBBS_COEFFS_R2MET_O[i].ni);
	}
	
		//printf ("gammatautauo\t%.8e\t\n", dblGammaSum );
	
return dblGammaSum;	
}


// [d squared gamma_o / d tau squared] keeping pi constant
double if97_r2met_GammaTauTau_o (double if97_tau) {
	
	int i;
	double dblGammaSum = 0.0;		

	#pragma omp parallel for reduction(+:dblGammaSum) 	//handle loop multithreaded		
	for (i=1; i <= MAX_GIBBS_COEFFS_R2MET_O; i++) {
		
		dblGammaSum += GIBBS_COEFFS_R2MET_O[i].ni * GIBBS_COEFFS_R2MET_O[i].Ji * ( GIBBS_COEFFS_R2MET_O[i].Ji  - 1.0) * pow(if97_tau, ( GIBBS_COEFFS_R2MET_O[i].Ji  - 2.0));
	}		 

return dblGammaSum;	
}



// [d squared gamma_o / d pi d tau] 
const double if97_r2met_GammaPiTau_o = 0.0;





// [d gamma_r / d pi] keeping tau constant
// Checked OK
double if97_r2met_GammaPi_r (double if97_pi, double if97_tau) {
	
	int i;
	double dblGammaSum = 0.0;		

	#pragma omp parallel for reduction(+:dblGammaSum) 	//handle loop multithreaded		
	for (i=1; i <= MAX_GIBBS_COEFFS_R2MET_R; i++) {
		
		dblGammaSum += GIBBS_COEFFS_R2MET_R[i].ni	* GIBBS_COEFFS_R2MET_R[i].Ii
								* pow( if97_pi, (GIBBS_COEFFS_R2MET_R[i].Ii - 1.0)) * pow((if97_tau - 0.5), GIBBS_COEFFS_R2MET_R[i].Ji);
	}

return dblGammaSum;
}




// [d squared gamma_r / d pi squared] keeping tau constant

double if97_r2met_GammaPiPi_r (double if97_pi, double if97_tau) {
	
	int i;
	double dblGammaSum = 0.0;		
	
	#pragma omp parallel for reduction(+:dblGammaSum) 	//handle loop multithreaded	
	for (i=1; i <= MAX_GIBBS_COEFFS_R2MET_R; i++) {
		
		dblGammaSum += GIBBS_COEFFS_R2MET_R[i].ni	* GIBBS_COEFFS_R2MET_R[i].Ii * (GIBBS_COEFFS_R2MET_R[i].Ii - 1.0)
								* pow(if97_pi, (GIBBS_COEFFS_R2MET_R[i].Ii - 2.0)) * pow((if97_tau - 0.5), GIBBS_COEFFS_R2MET_R[i].Ji);
	}

return dblGammaSum;
}




// [d gamma_r / d tau] keeping pi constant
// Checked OK
double if97_r2met_GammaTau_r (double if97_pi, double if97_tau) {
	
	int i;
	double dblGammaSum = 0.0;	
		
	#pragma omp parallel for reduction(+:dblGammaSum) 	//handle loop multithreaded		
	for (i=1; i <= MAX_GIBBS_COEFFS_R2MET_R; i++) {
		
		dblGammaSum += GIBBS_COEFFS_R2MET_R[i].ni	* pow( if97_pi, GIBBS_COEFFS_R2MET_R[i].Ii)
							* GIBBS_COEFFS_R2MET_R[i].Ji * pow((if97_tau - 0.5), (GIBBS_COEFFS_R2MET_R[i].Ji - 1.0)); 
	}
	
return dblGammaSum;
}



// [d squared gamma_r / d tau squared] keeping pi constant
// Checked OK
double if97_r2met_GammaTauTau_r (double if97_pi, double if97_tau) {
	
	int i;
	double dblGammaSum = 0.0;		

	#pragma omp parallel for reduction(+:dblGammaSum) 	//handle loop multithreaded		
	for (i=1; i <= MAX_GIBBS_COEFFS_R2MET_R; i++) {
		
		dblGammaSum += GIBBS_COEFFS_R2MET_R[i].ni	* pow( if97_pi, GIBBS_COEFFS_R2MET_R[i].Ii) * GIBBS_COEFFS_R2MET_R[i].Ji
								* (GIBBS_COEFFS_R2MET_R[i].Ji - 1.0) * pow((if97_tau - 0.5), (GIBBS_COEFFS_R2MET_R[i].Ji - 2.0)); 
	}

return dblGammaSum;
}



// [d squared gamma_r / d tau squared] keeping pi constant
double if97_r2met_GammaPiTau_r (double if97_pi, double if97_tau) {
	
	int i;
	double dblGammaSum = 0.0;		

	#pragma omp parallel for reduction(+:dblGammaSum) 	//handle loop multithreaded		
	for (i=1; i <= MAX_GIBBS_COEFFS_R2MET_R; i++) {
		
		dblGammaSum += GIBBS_COEFFS_R2MET_R[i].ni	* GIBBS_COEFFS_R2MET_R[i].Ii * pow(if97_pi, (GIBBS_COEFFS_R2MET_R[i].Ii - 1.0))
							* GIBBS_COEFFS_R2MET_R[i].Ji * pow((if97_tau - 0.5), ( GIBBS_COEFFS_R2MET_R[i].Ji - 1.0)); 
	}

return dblGammaSum;
}



//**********************************************************
//********* REGION 2 PROPERTY EQUATIONS*********************

// specific Gibbs free energy in region 2 (kJ / kg)
double if97_r2met_g (double p_MPa , double t_Kelvin) {  
	
	double if97pi = p_MPa / PSTAR_R2;
	double if97tau = TSTAR_R2 / t_Kelvin;
	
return IF97_R * t_Kelvin * if97_r2met_Gamma(if97pi, if97tau);
}


// specific volume in region 2  (metres cubed per kilogram)  
// inputs need to convert to pure SI, hence the ´magic´ numbers
// Checked OK
double if97_r2met_v (double p_MPa , double t_Kelvin ){
	
	double if97pi = p_MPa / PSTAR_R2;
	double if97tau = TSTAR_R2 / t_Kelvin;
	  
	return (IF97_R *1000 * t_Kelvin / (p_MPa * 1e6) ) * if97pi * ( if97_r2met_GammaPi_o(if97pi) + if97_r2met_GammaPi_r(if97pi, if97tau));
}


// specific internal energy in region 2 (KJ / Kg)
// Checked OK
double if97_r2met_u (double p_MPa , double t_Kelvin ){
	
	double if97pi = p_MPa / PSTAR_R2;
	double if97tau = TSTAR_R2/t_Kelvin;
	
	return (IF97_R* t_Kelvin ) * ((if97tau * (if97_r2met_GammaTau_o(if97tau) +  if97_r2met_GammaTau_r(if97pi, if97tau))) 
								  - (if97pi * (if97_r2met_GammaPi_o(if97pi) + if97_r2met_GammaPi_r(if97pi, if97tau)))
								  );
}



// specific entropy in region 2 (KJ / Kg.K)
// Checked OK
double if97_r2met_s (double p_MPa , double t_Kelvin ){
	
	double if97pi = p_MPa / PSTAR_R2;
	double if97tau = TSTAR_R2/t_Kelvin;
	
	return (IF97_R ) * (if97tau * (if97_r2met_GammaTau_o(if97tau) + if97_r2met_GammaTau_r(if97pi,if97tau)) - if97_r2met_Gamma(if97pi, if97tau))  ;
}



// specific enthalpy in region 2 (KJ / Kg)
// Checked OK
double if97_r2met_h (double p_MPa , double t_Kelvin ){
	
	double if97pi = p_MPa / PSTAR_R2;
	double if97tau = TSTAR_R2/t_Kelvin;
	
	return IF97_R * t_Kelvin * if97tau * (if97_r2met_GammaTau_o(if97tau) + if97_r2met_GammaTau_r(if97pi, if97tau))   ;
}



// specific isobaric heat capacity in region 2 (KJ / Kg.K)
// Checked OK
double if97_r2met_Cp (double p_MPa , double t_Kelvin ){
	
	double if97pi = p_MPa / PSTAR_R2;
	double if97tau = TSTAR_R2/t_Kelvin;
	
	return (-IF97_R * sqr(if97tau) * (if97_r2met_GammaTauTau_o(if97tau) + if97_r2met_GammaTauTau_r(if97pi, if97tau)))   ;
}



// specific isochoric heat capacity in region 2 (KJ / Kg.K)
// error in Moscow Power Institute page??
double if97_r2met_Cv (double p_MPa , double t_Kelvin ){
	
	double if97pi = p_MPa / PSTAR_R2;
	double if97tau = TSTAR_R2 / t_Kelvin;
		
	return IF97_R * ((- sqr(if97tau) * (if97_r2met_GammaTauTau_o(if97tau) + if97_r2met_GammaTauTau_r(if97pi, if97tau))) - (
	                sqr ( 1.0 + if97pi * if97_r2met_GammaPi_r(if97pi, if97tau) - if97tau * if97pi * if97_r2met_GammaPiTau_r(if97pi, if97tau))
	                / (1.0 - sqr(if97pi) * if97_r2met_GammaPiPi_r (if97pi, if97tau)))
	                ) ;
}




// speed of sound in region 2 (m/s)
// inputs need to convert to pure SI, hence the ´magic´ number 1000
// checked OK
double if97_r2met_w (double p_MPa , double t_Kelvin ){
	
	double if97pi = p_MPa / PSTAR_R2;
	double if97tau = TSTAR_R2/t_Kelvin;
	

	
	return sqrt( IF97_R * 1000 * t_Kelvin * ((1.0 + 2.0 * if97pi * if97_r2met_GammaPi_r(if97pi, if97tau) + sqr(if97pi) * sqr(if97_r2met_GammaPi_r(if97pi, if97tau))) /
					((1.0 - sqr(if97pi) * if97_r2met_GammaPiPi_r(if97pi, if97tau)) + 
						( sqr ( 1.0 + if97pi * if97_r2met_GammaPi_r(if97pi, if97tau) - if97tau * if97pi * if97_r2met_GammaPiTau_r(if97pi, if97tau)) /
							( sqr(if97tau) * (if97_r2met_GammaTauTau_o(if97tau) + if97_r2met_GammaTauTau_r(if97pi, if97tau)))
						)
					)
				 )
			   );
}

 
 








