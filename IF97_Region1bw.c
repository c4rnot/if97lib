
//          Copyright Martin Lord 2014-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


/**    IAPWS-IF97 Region 1 Equations
 *  VALIDITY  273.15 K <= T <= 623.15 K     Psat <= p <= 100 MPa
 * Exception:  Backwards equations not valid in metastable (superheated liquid) region
 */


#include "IF97_common.h"  //PSTAR TSTAR sqr
#include "IF97_Region1.h"
#include <math.h> //  pow




//**********************************************************
//********* REGION 1 BACKWARDS EQUATIONS********************



//returns temperature (K) in region 1 for a given pressure and enthalpy
// Checked OK
double if97_r1_t_ph (double p_MPa , double h_kJperKg ){

		// see Table 6
	const typIF97Coeffs_IJn  BW_COEFFS_R1_TPH[] = {
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
	const typIF97Coeffs_IJn  BW_COEFFS_R1_TPH[] = {
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





