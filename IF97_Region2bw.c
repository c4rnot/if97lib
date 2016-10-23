//          Copyright Martin Lord 2014-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


//    IAPWS-IF97 Region 2: Single phase vapour region backwards equations
/* *********************************************************************
 * *******             VALIDITY                             ************
 * 
 * 273.15 K <= T <=  623.15 K    0 <= p <= Ps(T) (Eq 30 "Saturation Pressure basic Equation")
 * 623.15 K <= T <=  863.15 K    0 <= p <= p(T)  (Eq 5 "B23 Region 2 - 3 boundry equation")
 * 863.15 K <= T <= 1073.15 K    0 <= p <= 100 MPa
 * 
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
#include "IF97_Region2bw.h"
#include <math.h> // for pow, log
/* #ifdef _OPENMP // multithreading via libgomp
 # include <omp.h>
 #endif
*/
 #include <stdio.h>  //used for debugging only

 
 
/* *******************************************************************
 * *****          Region 2 Backwards Equations    ********************
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

const int MAX_COEFFS_R2C_BW_PH = 23;

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


// See table 25
const typIF97Coeffs_IdJn  COEFFS_R2A_BW_PS[] = {
	 {0,     0,    0.0}	 
	,{-1.5,    -24,    -0.39235983861984e06}
	,{-1.5,    -23,     0.51526573827270e06}
	,{-1.5,    -19,     0.40482443161048e05}
	,{-1.5,    -13,    -0.32193790923902e03}
	,{-1.5,    -11,     0.96961424218694e02 }
	,{-1.5,    -10,    -0.22867846371773e02}
	,{-1.25,   -19,    -0.44942914124357e06}
	,{-1.25,   -15,    -0.50118336020166e04}
	,{-1.25,   -6,      0.35684463560015}
	,{-1.0,    -26,     0.44235335848190e05}
	,{-1.0,    -21,    -0.13673388811708e05}
	,{-1.0,    -17,     0.42163260207864e06}
	,{-1.0,    -16,     0.22516925837475e05}
	,{-1.0,    -9,      0.47442144865646e03}
	,{-1.0,    -8,     -0.14931130797647e03}
	,{-0.75,   -15,    -0.19781126320452e06}
	,{-0.75,   -14,    -0.23554399470760e05}
	,{-0.5,    -26,    -0.19070616302076e05}
	,{-0.5,    -13,     0.55375669883164e05}  
	,{-0.5,    -9,      0.38293691437363e04}
	,{-0.5,    -7,     -0.60391860580567e03}
	,{-0.25,   -27,     0.19363102620331e04}
	,{-0.25,   -25,     0.42660643698610e04}
	,{-0.25,   -11,    -0.59780638872718e04}
 	,{-0.25,   -6,     -0.70401463926862e03}  
	,{ 0.25,    1,      0.33836784107553e03}
	,{ 0.25,    4,      0.20862786635187e02}
	,{ 0.25,    8,      0.33834172656196e-1}
 	,{ 0.25,    11,    -0.43124428414893e-4}
 	,{ 0.5,     0,      0.16653791356412e03}
 	,{ 0.5,     1,     -0.13986292055898e03}
 	,{ 0.5,     5,     -0.78849547999872}
 	,{ 0.5,     6,      0.72132411753872e-1} 	
 	,{ 0.5,     10,    -0.59754839398283e-2} 	
 	,{ 0.5,     14,    -0.12141358953904e-4} 	
 	,{ 0.5,     16,     0.23227096733871e-6} 
	,{ 0.75,     0,    -0.10538463566194e02} 
	,{ 0.75,     4,     0.20718925496502e01} 
	,{ 0.75,     9,    -0.72193155260427e-1} 
	,{ 0.75,     17,    0.20749887081120e-6} 
	,{ 1.0,      7,    -0.18340657911379e-1} 
	,{ 1.0,     18,     0.29036272348696e-6}
	,{ 1.25,     3,     0.21037527893619}
	,{ 1.25,    15,     0.25681239729999e-3}
	,{ 1.5,      5,    -0.12799002933781e-1}
	,{ 1.5,     18,    -0.82198102652018e-5}
};

const int MAX_COEFFS_R2A_BW_PS = 46;

//returns temperature (K) in region 2a for a given pressure (MPa) and entropy (kJ/kg.K)
double if97_r2a_t_ps (double p_MPa, double s_kJperkgK ) {
	double sigma = s_kJperkgK / SSTAR_R2A_BW ;
	double pi = p_MPa ;  // no need to divide: Pstar =1

	int i;
	double dblTSum =0.0;
	
	#pragma omp parallel for reduction(+:dblTSum) 	//handle loop multithreaded	
	for (i=1; i <= MAX_COEFFS_R2A_BW_PS; i++) {

	dblTSum += COEFFS_R2A_BW_PS[i].ni * pow(pi, COEFFS_R2A_BW_PS[i].Ii) * pow((sigma - 2.0), COEFFS_R2A_BW_PS[i].Ji) ;
	}	
	
// no need to multiply by TSTAR since it is 1	
return dblTSum ;
}



// See table 26
const typIF97Coeffs_IJn  COEFFS_R2B_BW_PS[] = {
	 {0,     0,    0.0}	 
	,{   -6,      0,      0.31687665083497E6 }
	,{   -6,      11,      0.20864175881858E2 }
	,{   -5,      0,      -0.39859399803599E6 }
	,{   -5,      11,      -0.21816058518877E2 }
	,{   -4,      0,      0.22369785194242E6 }
	,{   -4,      1,      -0.27841703445817E4 }
	,{   -4,      11,      0.99207436071480E1 }
	,{   -3,      0,      -0.75197512299157E5 }
	,{   -3,      1,      0.29708605951158E4 }
	,{   -3,      11,      -0.34406878548526E1 }
	,{   -3,      12,      0.38815564249115 }
	,{   -2,      0,      0.17511295085750E5 }
	,{   -2,      1,      -0.14237112854449E4 }
	,{   -2,      6,      0.10943803364167E1 }
	,{   -2,      10,      0.89971619308495 }
	,{   -1,      0,      -0.33759740098958E4 }
	,{   -1,      1,      0.47162885818355E3 }
	,{   -1,      5,      -0.19188241993679E1 }
	,{   -1,      8,      0.41078580492196 }
	,{   -1,      9,      -0.33465378172097 }
	,{   0,      0,      0.13870034777505E4 }
	,{   0,      1,      -0.40663326195838E3 }
	,{   0,      2,      0.41727347159610E2 }
	,{   0,      4,      0.21932549434532E1 }
	,{   0,      5,      -0.10320050009077E1 }
	,{   0,      6,      0.35882943516703 }
	,{   0,      9,      0.52511453726066E-2 }
	,{   1,      0,      0.12838916450705E2 }
	,{   1,      1,      -0.28642437219381E1 }
	,{   1,      2,      0.56912683664855 }
	,{   1,      3,      -0.99962954584931E-1 }
	,{   1,      7,      -0.32632037778459E-2 }
	,{   1,      8,      0.23320922576723E-3 }
	,{   2,      0,      -0.15334809857450 }
	,{   2,      1,      0.29072288239902E-1 }
	,{   2,      5,      0.37534702741167E-3 }
	,{   3,      0,      0.17296691702411E-2 }
	,{   3,      1,      -0.38556050844504E-3 }
	,{   3,      3,      -0.35017712292608E-4 }
	,{   4,      0,      -0.14566393631492E-4 }
	,{   4,      1,      0.56420857267269E-5 }
	,{   5,      0,      0.41286150074605E-7 }
	,{   5,      1,      -0.20684671118824E-7 }
	,{   5,      2,      0.16409393674725E-8 }
};

const int MAX_COEFFS_R2B_BW_PS = 44;

//returns temperature (K) in region 2b for a given pressure (MPa) and entropy (kJ/kg.K)
double if97_r2b_t_ps (double p_MPa, double s_kJperkgK ) {
	double sigma = s_kJperkgK / SSTAR_R2B_BW ;
	double pi = p_MPa ;  // no need to divide: Pstar =1

	int i;
	double dblTSum =0.0;

	#pragma omp parallel for reduction(+:dblTSum) 	//handle loop multithreaded	
	for (i=1; i <= MAX_COEFFS_R2B_BW_PS; i++) {
		
	dblTSum += COEFFS_R2B_BW_PS[i].ni * pow(pi, COEFFS_R2B_BW_PS[i].Ii) * pow((10.0 - sigma), COEFFS_R2B_BW_PS[i].Ji) ;
	}	
	
// no need to multiply by TSTAR since it is 1	
return dblTSum ;
}



// See table 27
const typIF97Coeffs_IJn  COEFFS_R2C_BW_PS[] = {
	 {0,     0,    0.0}	 
	,{   -2,    0,    0.90968501005365E3 }
	,{   -2,    1,    0.24045667088420E4 }
	,{   -1,    0,    -0.59162326387130E3 }
	,{   0,    0,    0.54145404128074E3 }
	,{   0,    1,    -0.27098308411192E3 }
	,{   0,    2,    0.97976525097926E3 }
	,{   0,    3,    -0.46966772959435E3 }
	,{   1,    0,    0.14399274604723E2 }
	,{   1,    1,    -0.19104204230429E2 }
	,{   1,    3,    0.53299167111971E1 }
	,{   1,    4,    -0.21252975375934E2 }
	,{   2,    0,    -0.31147334413760 }
	,{   2,    1,    0.60334840894623 }
	,{   2,    2,    -0.42764839702509E-1 }
	,{   3,    0,    0.58185597255259E-2 }
	,{   3,    1,    -0.14597008284753E-1 }
	,{   3,    5,    0.56631175631027E-2 }
	,{   4,    0,    -0.76155864584577E-4 }
	,{   4,    1,    0.22440342919332E-3 }
	,{   4,    4,    -0.12561095013413E-4 }
	,{   5,    0,    0.63323132660934E-6 }
	,{   5,    1,    -0.20541989675375E-5 }
	,{   5,    2,    0.36405370390082E-7 }
	,{   6,    0,    -0.29759897789215E-8 }
	,{   6,    1,    0.10136618529763E-7 }
	,{   7,    0,    0.59925719692351E-11 }
	,{   7,    1,    -0.20677870105164E-10 }
	,{   7,    3,    -0.20874278181886E-10 }
	,{   7,    4,    0.10162166825089E-9 }
	,{   7,    5,    -0.16429828281347E-9 }	 
};

const int MAX_COEFFS_R2C_BW_PS = 30;

//returns temperature (K) in region 2c for a given pressure (MPa) and entropy (kJ/kg.K)
double if97_r2c_t_ps (double p_MPa, double s_kJperkgK ) {
	double sigma = s_kJperkgK / SSTAR_R2C_BW ;
	double pi = p_MPa ;  // no need to divide: Pstar =1

	int i;
	double dblTSum =0.0;

	#pragma omp parallel for reduction(+:dblTSum) 	//handle loop multithreaded	
	for (i=1; i <= MAX_COEFFS_R2C_BW_PS; i++) {
		
	dblTSum += COEFFS_R2C_BW_PS[i].ni * pow(pi, COEFFS_R2C_BW_PS[i].Ii) * pow((2.0 - sigma), COEFFS_R2C_BW_PS[i].Ji) ;
	}	
	
// no need to multiply by TSTAR since it is 1	
return dblTSum ;
}

