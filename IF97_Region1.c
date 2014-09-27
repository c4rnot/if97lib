
//          Copyright Martin Lord 2014-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


//    IAPWS-IF97 Region 1 Equations
// VALIDITY  273.15 K <= T <= 623.15 K     Psat <= p <= 100 MPa
// Exception:  Backwards equations not valid in metastable (superheated liquid) region


#include "IF97_constants.h"  //PSTAR TSTAR sqr
#include "IF97_Region1.h"
#include <math.h> //  pow



typedef struct sctGibbsCoeff {
	int Ii;
	int Ji;
	double ni;
} typR1coeff;



//***************************************************************
//****** REGION 1 GIBBS FREE ENERGY AND DERIVATIVES**************
// these were checked using  Moscow Power Engineering Institute page
// http://twt.mpei.ac.ru/mcs/worksheets/iapws/IAPWS-IF97-Region1.xmcd

// see Table 2
const typR1coeff  GIBBS_COEFFS_R1[] = {
	 {0,	0,		 0.0} 				   //0  i starts at 1, so 0th i is not used
	,{0,	-2,		 0.14632971213167E+00} //1
	,{0,	-1,		-0.84548187169114E+00} //2
	,{0,	0,		-0.37563603672040E+01} //3
	,{0,	1,		 0.33855169168385E+01} //4
	,{0,	2,		-0.95791963387872E+00} //5
	,{0,	3,		 0.15772038513228E+00} //6
	,{0,	4,		-0.16616417199501E-01} //7
	,{0,	5,		 0.81214629983568E-03} //8
	,{1,	-9,		 0.28319080123804E-03} //9
	,{1,	-7,		-0.60706301565874E-03} //10
	,{1,	-1,		-0.18990068218419E-01} //11
	,{1,	0,		-0.32529748770505E-01} //12
	,{1,	1,		-0.21841717175414E-01} //13
	,{1,	3,		-0.52838357969930E-04} //14
	,{2,	-3,		-0.47184321073267E-03} //15
	,{2,	0,		-0.30001780793026E-03} //16
	,{2,	1,		 0.47661393906987E-04} //17
	,{2,	3,		-0.44141845330846E-05} //18
	,{2,	17,		-0.72694996297594E-15} //19
	,{3,	-4,		-0.31679644845054E-04} //20
	,{3,	0,		-0.28270797985312E-05} //21
	,{3,	6,		-0.85205128120103E-09} //22
	,{4,	-5,		-0.22425281908000E-05} //23
	,{4,	-2,		-0.65171222895601E-06} //24
	,{4,	10,		-0.14341729937924E-12} //25
	,{5,	-8,		-0.40516996860117E-06} //26
	,{8,	-11,	-0.12734301741641E-08} //27
	,{8,	-6,		-0.17424871230634E-09} //28
	,{21,	-29,	-0.68762131295531E-18} //29
	,{23,	-31,	 0.14478307828521E-19} //30
	,{29,	-38,	 0.26335781662795E-22} //31
	,{30,	-39,	-0.11947622640071E-22} //32
	,{31,	-40,	 0.18228094581404E-23} //33
	,{32,	-41,	-0.93537087292458E-25} //34
};

const int MAX_GIBBS_COEFFS_R1 = 34;


// dimensionless gibbs free energy in Region1 = g/RT:  The fundamental equation of region 1.   See Equation 7
// Checked OK
double if97_r1_Gamma (double if97_pi, double  if97_tau) {  
	int i;
	double dblGammaSum =0.0;

	#pragma omp parallel for reduction(+:dblGammaSum) 	//handle loop multithreaded	
	for (i=1; i <= MAX_GIBBS_COEFFS_R1; i++) {
		
	dblGammaSum += GIBBS_COEFFS_R1[i].ni*( pow(7.1 - if97_pi, GIBBS_COEFFS_R1[i].Ii) * pow(if97_tau - 1.222, GIBBS_COEFFS_R1[i].Ji));
	}
	
return dblGammaSum;
}
	


// [d gamma/d pi]with tau constant
// Checked OK
double if97_r1_GammaPi (double if97_pi, double if97_tau) {  
	int i;
	double dblGammaSum =0.0;
	double dblGammapiI = 0.0;
	
	#pragma omp parallel for reduction(+:dblGammaSum) 	//handle loop multithreaded
	for (i=1; i <= MAX_GIBBS_COEFFS_R1; i++) {
				
	dblGammapiI = - GIBBS_COEFFS_R1[i].ni*GIBBS_COEFFS_R1[i].Ii*( pow(7.1 - if97_pi, (GIBBS_COEFFS_R1[i].Ii-1)) * pow(if97_tau - 1.222, GIBBS_COEFFS_R1[i].Ji));
	dblGammaSum += dblGammapiI;

	}
return dblGammaSum;
}


// [d squared gamma/d pi squared] with tau constant
// Checked OK
double if97_r1_GammaPiPi (double if97_pi, double if97_tau) {  
	int i;
	double dblGammaSum =0.0;

	#pragma omp parallel for reduction(+:dblGammaSum) 	//handle loop multithreaded	
	for (i=1; i <= MAX_GIBBS_COEFFS_R1; i++) {
		
	dblGammaSum += GIBBS_COEFFS_R1[i].ni*GIBBS_COEFFS_R1[i].Ii*(GIBBS_COEFFS_R1[i].Ii-1)*( pow(7.1 - if97_pi, (GIBBS_COEFFS_R1[i].Ii-2)) * pow(if97_tau - 1.222, GIBBS_COEFFS_R1[i].Ji));
	}
	
return dblGammaSum;
}



// [d gamma/d tau] with pi constant
// Checked OK
double if97_r1_GammaTau (double if97_pi, double if97_tau) {  
	int i;
	double dblGammaSum =0.0;

	#pragma omp parallel for reduction(+:dblGammaSum) 	//handle loop multithreaded	
	for (i=1; i <= MAX_GIBBS_COEFFS_R1; i++) {
		
	dblGammaSum += GIBBS_COEFFS_R1[i].ni * pow(7.1 - if97_pi, GIBBS_COEFFS_R1[i].Ii) * GIBBS_COEFFS_R1[i].Ji * pow(if97_tau - 1.222, GIBBS_COEFFS_R1[i].Ji-1);
	
	}
	
return dblGammaSum;
}



// [d squared gamma/d tau squared] with pi constant
// Checked OK
double if97_r1_GammaTauTau (double if97_pi, double if97_tau) {  
	int i;
	double dblGammaSum =0.0;

	#pragma omp parallel for reduction(+:dblGammaSum) 	//handle loop multithreaded	
	for (i=1; i <= MAX_GIBBS_COEFFS_R1; i++) {
		
	dblGammaSum += GIBBS_COEFFS_R1[i].ni*( pow(7.1 - if97_pi, GIBBS_COEFFS_R1[i].Ii) * 
								GIBBS_COEFFS_R1[i].Ji *(GIBBS_COEFFS_R1[i].Ji-1) * pow(if97_tau - 1.222, GIBBS_COEFFS_R1[i].Ji-2));
	}
return dblGammaSum;
}



// [d squared gamma/d pi . d tau ] 
// Checked OK
double if97_r1_GammaPiTau (double if97_pi, double if97_tau) {  
	int i;
	double dblGammaSum =0.0;

	#pragma omp parallel for reduction(+:dblGammaSum) 	//handle loop multithreaded	
	for (i=1; i <= MAX_GIBBS_COEFFS_R1; i++) {
		
	dblGammaSum -= GIBBS_COEFFS_R1[i].ni * GIBBS_COEFFS_R1[i].Ii * ( pow(7.1 - if97_pi, (GIBBS_COEFFS_R1[i].Ii -1)) * GIBBS_COEFFS_R1[i].Ji * pow(if97_tau - 1.222, (GIBBS_COEFFS_R1[i].Ji -1)));
	
	}
	
return dblGammaSum;
}


//**********************************************************
//********* REGION 1 PROPERTY EQUATIONS*********************

// specific Gibbs free energy in region 1 (kJ / kg)
// checked OK
double if97_r1_g (double p_MPa , double t_Kelvin) {  
	
	double if97pi = p_MPa / PSTAR_R1;
	double if97tau = TSTAR_R1 / t_Kelvin;
	
return IF97_R * t_Kelvin * if97_r1_Gamma(if97pi, if97tau);
}


// specific volume in region 1  (metres cubed per kilogram)  
// inputs need to convert to pure SI, hence the ´magic´ numbers
// checked OK
double if97_r1_v (double p_MPa , double t_Kelvin ){
	
	double if97pi = p_MPa / PSTAR_R1;
	double if97tau = TSTAR_R1 / t_Kelvin;
	  
	
	return (IF97_R *1000 * t_Kelvin / (p_MPa * 1e6) ) * if97pi * if97_r1_GammaPi(if97pi, if97tau);
}



// specific internal energy in region 1 (KJ / Kg)
// Checked OK
double if97_r1_u (double p_MPa , double t_Kelvin ){
	
	double if97pi = p_MPa / PSTAR_R1;
	double if97tau = TSTAR_R1/t_Kelvin;
	
	return (IF97_R* t_Kelvin ) * (if97tau * if97_r1_GammaTau(if97pi, if97tau) - if97pi * if97_r1_GammaPi(if97pi, if97tau) ) ;
}


// specific entropy in region 1 (KJ / Kg.K)
// Checked OK
double if97_r1_s (double p_MPa , double t_Kelvin ){
	
	double if97pi = p_MPa / PSTAR_R1;
	double if97tau = TSTAR_R1/t_Kelvin;
	
	return (IF97_R ) * (if97tau * if97_r1_GammaTau(if97pi, if97tau) - if97_r1_Gamma(if97pi, if97tau))  ;
}



// specific enthalpy in region 1 (KJ / Kg)
// Checked OK
double if97_r1_h (double p_MPa , double t_Kelvin ){
	
	double if97pi = p_MPa / PSTAR_R1;
	double if97tau = TSTAR_R1/t_Kelvin;
	
	return IF97_R * t_Kelvin * if97tau * if97_r1_GammaTau(if97pi, if97tau)   ;
}


// specific isobaric heat capacity in region 1 (KJ / Kg.K)
// Checked OK
double if97_r1_Cp (double p_MPa , double t_Kelvin ){
	
	double if97pi = p_MPa / PSTAR_R1;
	double if97tau = TSTAR_R1/t_Kelvin;
	
	return (-IF97_R * if97tau *if97tau * if97_r1_GammaTauTau(if97pi, if97tau))   ;
}


// specific isochoric heat capacity in region 1 (KJ / Kg.K)
// Checked OK
double if97_r1_Cv (double p_MPa , double t_Kelvin ){
	
	double if97pi = p_MPa / PSTAR_R1;
	double if97tau = TSTAR_R1 / t_Kelvin;
		
	return IF97_R * (-(sqr(if97tau) * if97_r1_GammaTauTau(if97pi, if97tau)) + (
	                sqr ( if97_r1_GammaPi(if97pi, if97tau) - if97tau * if97_r1_GammaPiTau(if97pi, if97tau))
	                / if97_r1_GammaPiPi (if97pi, if97tau))) ;
}



// speed of sound in region 1 (m/s)
// inputs need to convert to pure SI, hence the ´magic´ number
// checked OK
double if97_r1_w (double p_MPa , double t_Kelvin ){
	
	double if97pi = p_MPa / PSTAR_R1;
	double if97tau = TSTAR_R1/t_Kelvin;
	

	
	return sqrt(  (IF97_R * 1000 * t_Kelvin * sqr(if97_r1_GammaPi(if97pi, if97tau))) 
				  /(( sqr( if97_r1_GammaPi(if97pi, if97tau) - if97tau * if97_r1_GammaPiTau(if97pi, if97tau) ) 
				     / ( if97tau * if97tau * if97_r1_GammaTauTau(if97pi, if97tau)))   
				  - if97_r1_GammaPiPi(if97pi, if97tau)
				   )
			   );
}




//**********************************************************
//********* REGION 1 BACKWARDS EQUATIONS********************



//returns temperature (K) in region 1 for a given pressure and enthalpy
// Checked OK
double if97_r1_t_ph (double p_MPa , double h_kJperKg ){

		// see Table 6
	const typR1coeff  BW_COEFFS_R1_TPH[] = {
	 {0,	0,	 0.0} 				   //0  i starts at 1, so 0th i is not used
	,{0,	0,	-0.23872489924521E3}  //1
	,{0,	1,	 0.40421188637945E3}
	,{0,	2,	 0.11349746881718E3}
	,{0,	6,	-0.58457616048039E1}
	,{0,	22,	-0.15285482413140E-3}
	,{0,	32,	-0.10866707695377E-5}
	,{1,	0,	-0.13391744872602E2}
	,{1,	1,	 0.43211039183559E2}
	,{1,	2,	-0.54010067170506E2}
	,{1,	3,	 0.30535892203916E2}
	,{1,	4,	-0.65964749423638E1}
	,{1,	10,	 0.93965400878363E-2}
	,{1,	32,	 0.11573647505340E-6}
	,{2,	10,	-0.25858641282073E-4}
	,{2,	32,	-0.40644363084799E-8}
	,{3,	10,	 0.66456186191635E-7}
	,{3,	32,	 0.80670734103027E-10}
	,{4,	32,	-0.93477771213947E-12}
	,{5,	32,	 0.58265442020601E-14}
	,{6,	32,	-0.15020185953503E-16} //20
	};

	const int MAX_BW_COEFFS_R1_TPH = 20;
	
	const double PSTAR_R1_TPH = 1.0 ;  // MPa
	const double TSTAR_R1_TPH = 1.0 ;
	const double HSTAR_R1_TPH = 2500.0 ; // kJ / kg

	double if97pi =  p_MPa / PSTAR_R1_TPH;
	double if97eta = h_kJperKg / HSTAR_R1_TPH;

	int i;
	double dblHSum =0.0;

	#pragma omp parallel for reduction(+:dblHSum) 	//handle loop multithreaded	
	for (i=1; i <= MAX_BW_COEFFS_R1_TPH; i++) {
		
	dblHSum += BW_COEFFS_R1_TPH[i].ni * pow(if97pi, BW_COEFFS_R1_TPH[i].Ii) * pow( ( if97eta +1),  BW_COEFFS_R1_TPH[i].Ji);
	}
	
return TSTAR_R1_TPH * dblHSum;
}


//returns temperature (K) in region 1 for a given pressure and entropy
//
double if97_r1_t_ps (double p_MPa , double s_kJperKgK ){
	
	// see Table 8
	const typR1coeff  BW_COEFFS_R1_TPH[] = {
	 {0,	0,	 0.0} 				   //0  i starts at 1, so 0th i is not used			
	,{0,	0,	174.78268058307}  //1
	,{0,	1,	34.806930892873}
	,{0,	2,	6.5292584978455}
	,{0,	3,	0.33039981775489}
	,{0,	11,	-1.9281382923196E-07}
	,{0,	31,	-2.4909197244573E-23}
	,{1,	0,	-0.26107636489332}
	,{1,	1,	0.22592965981586}
	,{1,	2,	-0.06425646339523}
	,{1,	3,	0.00788762892705}
	,{1,	12,	3.5672110607366E-10}
	,{1,	31,	1.7332496994895E-24}
	,{2,	0,	0.00056608900655}
	,{2,	1,	-0.0003263548314}
	,{2,	2,	4.4778286690632E-05}
	,{2,	9,	-5.1322156908507E-10}
	,{2,	31,	-4.2522657042207E-26}
	,{3,	10,	2.6400441360689E-13}
	,{3,	32,	7.8124600459723E-29}
	,{4,	32,	-3.0732199903668E-31}  //20
	};
	
	const int MAX_BW_COEFFS_R1_TPS = 20;
	
	const double PSTAR_R1_TPS = 1.0 ; // MPa
	const double TSTAR_R1_TPS = 1.0;  // K
	const double SSTAR_R1_TPS = 1.0 ; // kJ / kgK

	double if97pi =  p_MPa / PSTAR_R1_TPS;
	double  if97sigma = s_kJperKgK / SSTAR_R1_TPS;
	
	

	int i;
	double dblHSum =0.0;

	#pragma omp parallel for reduction(+:dblHSum) 	//handle loop multithreaded	
	for (i=1; i <= MAX_BW_COEFFS_R1_TPS; i++) {
		
	dblHSum += BW_COEFFS_R1_TPH[i].ni * pow(if97pi, BW_COEFFS_R1_TPH[i].Ii) * pow( ( if97sigma + 2),  BW_COEFFS_R1_TPH[i].Ji);
	}
	
return TSTAR_R1_TPS * dblHSum;
}





