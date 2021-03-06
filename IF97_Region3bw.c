//          Copyright Martin Lord 2014-2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


//    IAPWS-IF97 Region 3 Backwards Equations: low temperature supercritical region 
/* *********************************************************************
 * *******             VALIDITY                             ************
 * 623.15 K <=T <= T ( p ) [B23 temperature equation]
 * p ( T ) [B23 temperature equation] <= p <= 100 MPa .
 * 
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
#include "IF97_Region4.h" // saturation line used in determining subregion
#include "IF97_B23.h"   // not strictly required but used in v(p,t) subregion selector as an error detector
#include <math.h> // for pow, log
/* #ifdef _OPENMP // multithreading via libgomp
 # include <omp.h>
 #endif
*/
 #include <stdio.h>  //used for debugging only

typedef struct sctR3RedCoefs {
	double vStar;
	double pStar;
	double tStar;
	int N;
	double a;
	double b;
	double c;
	double d;
	double e;
} typR3RedCoefs;
	


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



const typIF97Coeffs_Jn T3UV_P_R3_COEFFS[] = {
	{0,	 0.0},				   //0  i starts at 1, so 0th i is not used
	{0, 0.528199646263062e3},
	{1, 0.890579602135307e1},
	{2, -.222814134903755},
	{3, 0.286791682263697e-2}
};

const typIF97Coeffs_Jn T3WX_P_R3_COEFFS[] = {
	{0,	 0.0},				   //0  i starts at 1, so 0th i is not used
	{0, 0.728052609145380e1},
	{1, 0.973505869861952e2},
	{2, 0.147370491183191e2},
	{-1, 0.329196213998375e3},
	{-2, 0.873371668682417e3}
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



	
/* ****   REGION BOUNDARIES CLOSE TO CRITICAL REGIION ******* */


//  Region 3u/3v boundary. see equation 1.
double if97_r3uv_p_t (double p_MPa){
	int i;
	double dblPhiSum = 0.0;
	
	for (i=1; i <= (int)(sizeof(T3UV_P_R3_COEFFS)/sizeof(typIF97Coeffs_Jn) -1 ) ; i++) {
		dblPhiSum += T3UV_P_R3_COEFFS[i].ni * pow( p_MPa, T3UV_P_R3_COEFFS[i].Ji)	;
	}	
	
return dblPhiSum;
}


//  Region 3w/3x boundary. see equation 2.  
double if97_r3wx_p_t (double p_MPa){
	int i;
	double dblPhiSum = 0.0;

	for (i=1; i <= (int)(sizeof(T3WX_P_R3_COEFFS)/sizeof(typIF97Coeffs_Jn) -1 ) ; i++) {
		dblPhiSum += (double)T3WX_P_R3_COEFFS[i].ni  * pow( log(p_MPa), T3WX_P_R3_COEFFS[i].Ji );
	}	
	
return dblPhiSum;
}


// Region 1=3a, 2=3b, 3=3c etc...    100 = near critical   0 = error: not region 3
// this function assumes you are already know you are in region 3
// see table 2
char if97_r3_pt_subregion(double p_MPa, double t_K){
	const double P3_CD = 1.900881189173929e01; // bottom of table 2
	
	// First, make sure we are in R3
	if ((p_MPa > IF97_R3_UPRESS) || (p_MPa < IF97_B23_LPRESS)) return 0 ; // not R3
	else if ((t_K < IF97_R3_LTEMP)  ||  (IF97_B23T(p_MPa) < t_K)) return 0 ;  // not R3
	
	// now check through table 2 by pressure range
	if (p_MPa > 40.0) {   // already checked below 100 MPa (R3_UPRESS)
		if (t_K >  if97_r3ab_p_t(p_MPa)) return 'b';  //3b
		else return 'a'; //3a
	}
	
	else if (p_MPa > 25.0) {   // already checked below or equal to 40 MPa 
		if (if97_r3ab_p_t(p_MPa)  < t_K ){ // right of T3ab line dividing 3d & 3e fig 3
			if (t_K <= if97_r3ef_p_t(p_MPa)) return 'e';  //3e
			else return 'f';  // 3f
		}	
		// now on or left of T3ab line dividing 3d & 3e fig 3
		else if (t_K <= if97_r3cd_p_t(p_MPa)) return 'c';  //3c
		else return 'd';  // 3d		
	}
	
	else if (p_MPa > 23.0) {   // already checked below or equal to 25 MPa 
		if (if97_r3ef_p_t(p_MPa) < t_K ){ // right of ef line fig 4
			if (t_K > if97_r3jk_p_t(p_MPa)) return 'k';  //3k
			else if (t_K > if97_r3ij_p_t(p_MPa)) return 'j'; //3j
			else return 'i';  //3i
		}	
		// now on or left of ef line
		else if (t_K <= if97_r3cd_p_t(p_MPa)) return 'c';  //3c
		else if (t_K <= if97_r3gh_p_t(p_MPa)){
			if (p_MPa > 23.5) return 'g';  // 3g   23.5 MPa < p <= 25 MPa
			else return 'l'; // 3l :  23 MPa < p <= 23.5 MPa
		}
		else return 'h';	// 3h
	}	

	else if (p_MPa > 22.5) {   // already checked below or equal to 23.0 MPa 
		if (if97_r3ef_p_t(p_MPa) < t_K ){ // right of ef line fig 4
			if (t_K > if97_r3jk_p_t(p_MPa)) return 'k';  //3k
			else if (t_K > if97_r3ij_p_t(p_MPa)) return 'j'; //3j
			else if (t_K > if97_r3op_p_t(p_MPa)) return 'p'; //3p
			else return 'o';  //3o
		}	
		// now on or left of ef line
		else if (t_K <= if97_r3cd_p_t(p_MPa)) return 'c';  //3c
		else if (t_K <= if97_r3gh_p_t(p_MPa)) return 'l';  // 3l
		else if (t_K <= if97_r3mn_p_t(p_MPa)) return 'm';  //3m
		else return 'n';	// 3n
	}	

	// // above Psat(643.15 K) 21.0434 MPa to  below or equal 22.5 MPa (already checked)   See figure 5 & Figure 3
	else if (p_MPa > if97_r4_ps(643.15)) {   
		if (if97_r3rx_p_t(p_MPa)  < t_K ){ // right of rx line fig 4
			if (t_K > if97_r3jk_p_t(p_MPa)) return 'k';  //3k
			else return 'r';  //3r
		}	
		else if ( t_K <= if97_r3qu_p_t(p_MPa)){ // on or left of of qu line fig 4
			if (t_K <= if97_r3cd_p_t(p_MPa)) return 'c';  //3c
			else return 'q';  //3q
		}
		
		// ***  now dealing with near supercritical region Fig 5, Table 10 ***
		//First supercritical near critical
		else if (22.11 < p_MPa){    // already checked below or equal to 22.6 MPa above
			if (if97_r3ef_p_t(p_MPa) < t_K){  // already checked <= 3rx
				if (if97_r3wx_p_t(p_MPa) < t_K) return 'x'; //3x (Auxiliary)
				else return 'w'; //3w (Auxiliary)
			}
			else if (t_K <= if97_r3uv_p_t(p_MPa)) return 'u'; // 3u (Auxiliary. Already checked above 3qu
			else return 'v'; //3v (Auxiliary
		}
		
		else if (IF97_PC < p_MPa){  // already checked less than or equal to 22.11 MPa
			if (if97_r3ef_p_t(p_MPa) < t_K){  // already checked <= 3rx
				if (if97_r3wx_p_t(p_MPa) < t_K) return 'x'; //3x (Auxiliary)
				else return 'z'; //3z (Auxiliary)
			}
			else if (t_K <= if97_r3uv_p_t(p_MPa)) return 'u'; // 3u (Auxiliary. Already checked above 3qu
			else return 'y'; //3y (Auxiliary
		}
		
		// now subcritical near critical
		else if (t_K <= if97_r4_ts(p_MPa)){  // water phase near critical
			if (p_MPa <= 2.193161551e1) return 'u'; // 3u /Auxiliary // see note e of table 10  // already checked above Psat(643.15K)
			else if (if97_r3uv_p_t(p_MPa) < t_K) return 'y'; // 3y (Auxiliary) // already dealt with 3q above
			else return 'u'; // 3u (Auxiliary)
			}
		// already checked that is is either steam or saturated, near critical
		if (p_MPa <= 2.190096265e1 ) return 'x'; // 3x /Auxiliary // see note f of table 10  // already checked above Psat(643.15K) 
		else if (if97_r3wx_p_t(p_MPa) < t_K) return 'x'; // 3x (Auxiliary)
		else return 'z'; //3z (Auxiliary)
	}
			

	// back to normal regions
	// see figure 3.  Now all below Psat(643.15 K) = 21.0434 MPa
	else if (p_MPa > 20.5) {   // above 20.5 to  below or equal Psat(643.15 K)
		if (if97_r4_ts(p_MPa) <= t_K ){ // on or right of saturation line fig 3
			if (t_K > if97_r3jk_p_t(p_MPa)) return 'k';  //3k
			else return 'r';  //3r
		}	
		
		// left of saturation line fig 3
		else if (t_K <= if97_r3cd_p_t(p_MPa)) return 'c';  //3c
		else return 'q';  //3q
	}		
	
	else if (p_MPa > P3_CD) {   // above P3cd to  below or equal 20.5 MPa
		if (if97_r4_ts(p_MPa) <= t_K ) return  't'; //3t  : on or right of saturation line fig 3
	
		// left of saturation line fig 3
		else if (t_K <= if97_r3cd_p_t(p_MPa)) return 'c';  //3c
		else return 's';  //3s
	}		
	
	else if (p_MPa > if97_r4_ps(IF97_R3_LTEMP)) {   // above Psat(623.15 K) to  below or equal P3cd 
		if (if97_r4_ts(p_MPa) <= t_K ) return  't'; //3t  : on or right of saturation line fig 3
	
		// left of saturation line fig 3
		else return 'c';  //3c
	}		
	
	else return 0 ; //  this should never execute
}	



// checks if the region is handled by lower accuracy auxiliary equations.
// For best accuracy use the aux equation result as a starting iteration using the main equation
bool isNearCritical (double p_MPa, double t_K){
	if (((p_MPa <= 22.5) && (if97_r4_ps(643.15) < p_MPa)) && ((if97_r3qu_p_t(p_MPa) < t_K) && (t_K <= if97_r3rx_p_t(p_MPa))))
		return true;
	else return false;
}


// The following sets of coefficients from Appendix A1 .	
	

// specific volume (m3/kg) in region 3a for a given temperature (K) and pressure (MPa)
// equation 4
double if97_r3a_v_pt (double p_MPa, double t_K){

	const typR3RedCoefs V3A_PT_RC = {
		0.0024, 100.0, 760.0, 30, 0.085, 0.817, 1.0, 1.0, 1.0
	};

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
	
	int i = 1;
	double omegasum = 0;
	
	double pi = p_MPa / V3A_PT_RC.pStar;
	double theta = t_K / V3A_PT_RC.tStar;
	
	#pragma omp parallel for reduction (+:omegasum)
	for (i = 1; i <= V3A_PT_RC.N; i++) {
		
		omegasum += V3A_PT_COEFFS[i].ni * 
			pow(pow((pi - V3A_PT_RC.a), V3A_PT_RC.c) , V3A_PT_COEFFS[i].Ii)
			* pow(pow((theta - V3A_PT_RC.b), V3A_PT_RC.d ), V3A_PT_COEFFS[i].Ji);
	}


	return V3A_PT_RC.vStar * pow(omegasum , V3A_PT_RC.e );
}




// specific volume (m3/kg) in region 3b for a given temperature (K) and pressure (MPa)
// equation 4
double if97_r3b_v_pt (double p_MPa, double t_K){

	const typR3RedCoefs V3B_PT_RC = {
		0.0041, 100.0, 860.0, 32, 0.280, 0.779, 1, 1, 1
	};

	const typIF97Coeffs_IJn V3B_PT_COEFFS[] = {
		 {0,	0,	0.0} 				   //0  i starts at 1, so 0th i is not used
    	,{-12,	10,	-0.827670470003621e-1}
		,{-12,	12,	 0.416887126010565e02}
		,{-10,	8,	 0.483651982197059e-1}
		,{-10,	14,	-0.291032084950276e05}	
		,{-8,	8,	-0.111422582236948e03} //5	
		
		,{-6,	5,	-0.202300083904014e-1}
		,{-6,	6,	 0.294002509338515e03}
		,{-6,	8,	 0.140244997609658e03}
		,{-5,	5,	-0.344384158811459e03}
		,{-5,	8,	 0.361182452612149e03}	//10

		,{-5,	10,	-0.140699677420738e04}
		,{-4,	2,	-0.202023902676481e-2}
		,{-4,	4,	 0.171346792457471e03}
		,{-4,	5,	-0.425597804058632e01}
		,{-3,	0,	 0.691346085000334e-5} //15

		,{-3,	1,	 0.151140509678925e-2}
		,{-3,	2,	-0.416375290166236e-1}
		,{-3,	3,	-0.413754957011042e02}
		,{-3,	5,	-0.506673295721637e02}
		,{-2,	0,	-0.572212965569023e-3} //20
		
		,{-2,	2,	 0.608817368401785e01}
		,{-2,	5,	 0.239600660256161e02}
		,{-1,	0,	 0.122261479925384e-1}
		,{-1,	2,	 0.216356057692938e01}
		,{0,	0,	 0.398198903368642e00} //25
		
		,{0,	1,	-0.116892827834085e00}
		,{1,	0,	-0.102845919373532e00}
		,{1,	2,	-0.492676637589284e00}
		,{2,	0,	 0.655540456406790e-1}
		,{3,	2,	-0.240462535078530e00} //30

		,{4,	0,	-0.269798180310075e-1}
		,{4,	1,	 0.128369435967012e00} //32
	};
	
	int i = 1;
	double omegasum = 0;
	
	double pi = p_MPa / V3B_PT_RC.pStar;
	double theta = t_K / V3B_PT_RC.tStar;
	
	#pragma omp parallel for reduction (+:omegasum)
	for (i = 1; i <= V3B_PT_RC.N; i++) {
		
		omegasum += V3B_PT_COEFFS[i].ni * 
			pow(pow((pi - V3B_PT_RC.a), V3B_PT_RC.c) , V3B_PT_COEFFS[i].Ii)
			* pow(pow((theta - V3B_PT_RC.b), V3B_PT_RC.d ), V3B_PT_COEFFS[i].Ji);
	}


	return V3B_PT_RC.vStar * pow(omegasum , V3B_PT_RC.e );
}


// specific volume (m3/kg) in region 3c for a given temperature (K) and pressure (MPa)
// equation 4
double if97_r3c_v_pt (double p_MPa, double t_K){

	const typR3RedCoefs V3C_PT_RC = {
		0.0022, 40.0, 690.0, 35, 0.259, 0.903, 1.0, 1.0, 1.0
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
	
	int i = 1;
	double omegasum = 0;
	
	double pi = p_MPa / V3C_PT_RC.pStar;
	double theta = t_K / V3C_PT_RC.tStar;
	
	#pragma omp parallel for reduction (+:omegasum)
	for (i = 1; i <= V3C_PT_RC.N; i++) {
		
		omegasum += V3C_PT_COEFFS[i].ni * 
			pow(pow((pi - V3C_PT_RC.a), V3C_PT_RC.c) , V3C_PT_COEFFS[i].Ii)
			* pow(pow((theta - V3C_PT_RC.b), V3C_PT_RC.d ), V3C_PT_COEFFS[i].Ji);
	}

	return V3C_PT_RC.vStar * pow(omegasum , V3C_PT_RC.e );
}


// specific volume (m3/kg) in region 3d for a given temperature (K) and pressure (MPa)
// equation 4
double if97_r3d_v_pt (double p_MPa, double t_K){

	const typR3RedCoefs V3D_PT_RC = {
		0.0029, 40.0, 690.0, 38, 0.559, 0.939, 1.0, 1.0, 4.0
	};

	const typIF97Coeffs_IJn V3D_PT_COEFFS[] = {
		 {0,	0,	0.0} 				   //0  i starts at 1, so 0th i is not used
		,{-12,	4,	-0.452484847171645e-9}
		,{-12,	6,	0.315210389538801e-4}	
		,{-12,	7,	-0.214991352047545e-2}
		,{-12,	10,	0.508058874808345e03}
		,{-12,	12,	-0.127123036845932e08} //5

		,{-12,	16,	0.115371133120497e13}
		,{-10,	0,	-0.197805728776273e-15}
		,{-10,	2,	0.241554806033972e-10}
		,{-10,	4,	-0.156481703640525e-5}
		,{-10,	6,	0.277211346836625e-2} //10

		,{-10,	8,	-0.203578994462286e2}
		,{-10,	10,	0.144369489909053e7}
		,{-10,	14,	-0.411254217946539e11}
		,{-8,	3,	0.623449786243773e-5}
		,{-8,	7,	-0.221774281146038e2} //15

		,{-8,	8,	-0.689315087933158e5}
		,{-8,	10,	-0.195419525060713e8}
		,{-6,	6,	0.316373510564015e4}
		,{-6,	8,	0.224040754426988e7}
		,{-5,	1,	-0.436701347922356e-5} //20

		,{-5,	2,	-0.404213852833996e-3}
		,{-5,	5,	-0.348153203414663e3}
		,{-5,	7,	-0.385294213555289e6}
		,{-4,	0,	0.135203700099403e-6}
		,{-4,	1,	0.134648383271089e-3} //25

		,{-4,	7,	0.125031835351736e6}
		,{-3,	2,	0.968123678455841e-1}
		,{-3,	4,	0.225660517512438e3}
		,{-2,	0,	-0.190102435341872e-3}
		,{-2,	1,	-0.299628410819229e-1} //30

		,{-1,	0,	0.500833915372121e-2}
		,{-1,	1,	0.387842482998411}
		,{-1,	5,	-0.138535367777182e4}
		,{0,	0,	0.870745245971773}
		,{0,	2,	0.171946252068742e1} //35

		,{1,	0,	-0.326650121426383e-1}
		,{1,	6,	0.498044171727877e4}
		,{3,	0,	0.551478022765087e-2}  //38*/
	};
	
	int i = 1;
	double omegasum = 0;
	
	double pi = p_MPa / V3D_PT_RC.pStar;
	double theta = t_K / V3D_PT_RC.tStar;
	
	#pragma omp parallel for reduction (+:omegasum)
	for (i = 1; i <= V3D_PT_RC.N; i++) {
		
		omegasum += V3D_PT_COEFFS[i].ni * 
			pow(pow((pi - V3D_PT_RC.a), V3D_PT_RC.c) , V3D_PT_COEFFS[i].Ii)
			* pow(pow((theta - V3D_PT_RC.b), V3D_PT_RC.d ), V3D_PT_COEFFS[i].Ji);
			
	}

	return V3D_PT_RC.vStar * pow(omegasum , V3D_PT_RC.e );
}


// specific volume (m3/kg) in region 3e for a given temperature (K) and pressure (MPa)
// equation 4
double if97_r3e_v_pt (double p_MPa, double t_K){

	const typR3RedCoefs V3E_PT_RC = {
		0.0032, 40.0, 710.0, 29, 0.587, 0.918, 1.0, 1.0, 1.0
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
	
	int i = 1;
	double omegasum = 0;
	
	double pi = p_MPa / V3E_PT_RC.pStar;
	double theta = t_K / V3E_PT_RC.tStar;
	
	#pragma omp parallel for reduction (+:omegasum)
	for (i = 1; i <= V3E_PT_RC.N; i++) {
		
		omegasum += V3E_PT_COEFFS[i].ni * 
			pow(pow((pi - V3E_PT_RC.a), V3E_PT_RC.c) , V3E_PT_COEFFS[i].Ii)
			* pow(pow((theta - V3E_PT_RC.b), V3E_PT_RC.d ), V3E_PT_COEFFS[i].Ji);
			
	}

	return V3E_PT_RC.vStar * pow(omegasum , V3E_PT_RC.e );
}


// specific volume (m3/kg) in region 3f for a given temperature (K) and pressure (MPa)
// equation 4
double if97_r3f_v_pt (double p_MPa, double t_K){

	const typR3RedCoefs V3F_PT_RC = {
		0.0064, 40.0, 730.0, 42, 0.587, 0.891, 0.5, 1.0, 4.0
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
	
	int i = 1;
	double omegasum = 0;
	
	double pi = p_MPa / V3F_PT_RC.pStar;
	double theta = t_K / V3F_PT_RC.tStar;
	
	#pragma omp parallel for reduction (+:omegasum)
	for (i = 1; i <= V3F_PT_RC.N; i++) {
		
		omegasum += V3F_PT_COEFFS[i].ni * 
			pow(pow((pi - V3F_PT_RC.a), V3F_PT_RC.c) , V3F_PT_COEFFS[i].Ii)
			* pow(pow((theta - V3F_PT_RC.b), V3F_PT_RC.d ), V3F_PT_COEFFS[i].Ji);
			
	}

	return V3F_PT_RC.vStar * pow(omegasum , V3F_PT_RC.e );
}


// specific volume (m3/kg) in region 3g for a given temperature (K) and pressure (MPa)
// equation 4
double if97_r3g_v_pt (double p_MPa, double t_K){

	const typR3RedCoefs V3G_PT_RC = {
		0.0027, 25.0, 660.0, 38, 0.872, 0.971, 1.0, 1.0, 4.0
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
	
	int i = 1;
	double omegasum = 0;
	
	double pi = p_MPa / V3G_PT_RC.pStar;
	double theta = t_K / V3G_PT_RC.tStar;
	
	#pragma omp parallel for reduction (+:omegasum)
	for (i = 1; i <= V3G_PT_RC.N; i++) {
		
		omegasum += V3G_PT_COEFFS[i].ni * 
			pow(pow((pi - V3G_PT_RC.a), V3G_PT_RC.c) , V3G_PT_COEFFS[i].Ii)
			* pow(pow((theta - V3G_PT_RC.b), V3G_PT_RC.d ), V3G_PT_COEFFS[i].Ji);
			
	}

	return V3G_PT_RC.vStar * pow(omegasum , V3G_PT_RC.e );
}


// specific volume (m3/kg) in region 3h for a given temperature (K) and pressure (MPa)
// equation 4
double if97_r3h_v_pt (double p_MPa, double t_K){

	const typR3RedCoefs V3H_PT_RC = {
		0.0032, 25.0, 660.0, 29, 0.898, 0.983, 1.0, 1.0, 4.0
	};
	
	//From Hummeling if97 Java
	// http://sourceforge.net/p/if97/git/ci/master/tree/src/main/java/com/hummeling/if97/Region3.java#l1801	
	const typIF97Coeffs_IJn V3H_PT_COEFFS[] = {
		{0,	0,	0.0}, //0  i starts at 1, so 0th i is not used
		{-12, 8, .561379678887577e-1},
		{-12, 12, .774135421587083e10},
		{-10, 4, .111482975877938e-8},
		{-10, 6, -.143987128208183e-2},
		{-10, 8, .193696558764920e4},
		{-10, 10, -.605971823585005e9},
		{-10, 14, .171951568124337e14},
		{-10, 16, -.185461154985145e17},
		{-8, 0, .387851168078010e-16},
		{-8, 1, -.395464327846105e-13},
		{-8, 6, -.170875935679023e3},
		{-8, 7, -.212010620701220e4},
		{-8, 8, .177683337348191e8},
		{-6, 4, .110177443629575e2},
		{-6, 6, -.234396091693313e6},
		{-6, 8, -.656174421999594e7},
		{-5, 2, .156362212977396e-4},
		{-5, 3, -.212946257021400e1},
		{-5, 4, .135249306374858e2},
		{-4, 2, .177189164145813},
		{-4, 4, .139499167345464e4},
		{-3, 1, -.703670932036388e-2},
		{-3, 2, -.152011044389648},
		{-2, 0, .981916922991113e-4},
		{-1, 0, .147199658618076e-2},
		{-1, 2, .202618487025578e2},
		{0, 0, .899345518944240},
		{1, 0, -.211346402240858},
		{1, 2, .249971752957491e2}
	};
		 
	int i = 1;
	double omegasum = 0;
	
	double pi = p_MPa / V3H_PT_RC.pStar;
	double theta = t_K / V3H_PT_RC.tStar;
	
	#pragma omp parallel for reduction (+:omegasum)
	for (i = 1; i <= V3H_PT_RC.N; i++) {
		
		omegasum += V3H_PT_COEFFS[i].ni * 
			pow(pow((pi - V3H_PT_RC.a), V3H_PT_RC.c) , V3H_PT_COEFFS[i].Ii)
			* pow(pow((theta - V3H_PT_RC.b), V3H_PT_RC.d ), V3H_PT_COEFFS[i].Ji);
			
	}

	return V3H_PT_RC.vStar * pow(omegasum , V3H_PT_RC.e );
}



// specific volume (m3/kg) in region 3i for a given temperature (K) and pressure (MPa)
// equation 4
double if97_r3i_v_pt (double p_MPa, double t_K){

	const typR3RedCoefs V3I_PT_RC = {
		0.0041, 25.0, 660.0, 42, 0.910, 0.984, 0.5, 1.0, 4.0
	};
	
	const typIF97Coeffs_IJn V3I_PT_COEFFS[] = {
		 
		{0,	0,	0.0},   //0  i starts at 1, so 0th i is not used
		{0, 0, .106905684359136e1},
		{0, 1, -.148620857922333e1},
		{0, 10, .259862256980408e15},
		{1, -4, -.446352055678749e-11},
		{1, -2, -.566620757170032e-6},
		{1, -1, -.235302885736849e-2},
		{1, 0, -.269226321968839},
		{2, 0, .922024992944392e1},
		{3, -5, .357633505503772e-11},
		{3, 0, -.173942565562222e2},
		{4, -3, .700681785556229e-5},
		{4, -2, -.267050351075768e-3},
		{4, -1, -.231779669675624e1},
		{5, -6, -.753533046979752e-12},
		{5, -1, .481337131452891e1},
		{5, 12, -.223286270422356e22},
		{7, -4, -.118746004987383e-4},
		{7, -3, .646412934136496e-2},
		{8, -6, -.410588536330937e-9},
		{8, 10, .422739537057241e20},
		{10, -8, .313698180473812e-12},
		{12, -12, .164395334345040e-23},
		{12, -6, -.339823323754373e-5},
		{12, -4, -.135268639905021e-1},
		{14, -10, -.723252514211625e-14},
		{14, -8, .184386437538366e-8},
		{14, -4, -.463959533752385e-1},
		{14, 5, -.992263100376750e14},
		{18, -12, .688169154439335e-16},
		{18, -10, -.222620998452197e-10},
		{18, -8, -.540843018624083e-7},
		{18, -6, .345570606200257e-2},
		{18, 2, .422275800304086e11},
		{20, -12, -.126974478770487e-14},
		{20, -10, .927237985153679e-9},
		{22, -12, .612670812016489e-13},
		{24, -12, -.722693924063497e-11},
		{24, -8, -.383669502636822e-3},
		{32, -10, .374684572410204e-3},
		{32, -5, -.931976897511086e5},
		{36, -10, -.247690616026922e-1},
		{36, -8, .658110546759474e2}  //42
	};
		 
	int i = 1;
	double omegasum = 0;
	
	double pi = p_MPa / V3I_PT_RC.pStar;
	double theta = t_K / V3I_PT_RC.tStar;
	
	#pragma omp parallel for reduction (+:omegasum)
	for (i = 1; i <= V3I_PT_RC.N; i++) {
		
		omegasum += V3I_PT_COEFFS[i].ni * 
			pow(pow((pi - V3I_PT_RC.a), V3I_PT_RC.c) , V3I_PT_COEFFS[i].Ii)
			* pow(pow((theta - V3I_PT_RC.b), V3I_PT_RC.d ), V3I_PT_COEFFS[i].Ji);
			
	}

	return V3I_PT_RC.vStar * pow(omegasum , V3I_PT_RC.e );
}



// specific volume (m3/kg) in region 3j for a given temperature (K) and pressure (MPa)
// equation 4
double if97_r3j_v_pt (double p_MPa, double t_K){

	const typR3RedCoefs V3J_PT_RC = {
		0.0054, 25.0, 670.0, 29, 0.875, 0.964, 0.5, 1.0, 4.0
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
	,{4,	-2,	-0.259680385227130e-2} //10

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
	
	int i = 1;
	double omegasum = 0;
	
	double pi = p_MPa / V3J_PT_RC.pStar;
	double theta = t_K / V3J_PT_RC.tStar;
	
	#pragma omp parallel for reduction (+:omegasum)
	for (i = 1; i <= V3J_PT_RC.N; i++) {
		
		omegasum += V3J_PT_COEFFS[i].ni * 
			pow(pow((pi - V3J_PT_RC.a), V3J_PT_RC.c) , V3J_PT_COEFFS[i].Ii)
			* pow(pow((theta - V3J_PT_RC.b), V3J_PT_RC.d ), V3J_PT_COEFFS[i].Ji);
			
	}

	return V3J_PT_RC.vStar * pow(omegasum , V3J_PT_RC.e );
}



// specific volume (m3/kg) in region 3k for a given temperature (K) and pressure (MPa)
// equation 4
double if97_r3k_v_pt (double p_MPa, double t_K){

	const typR3RedCoefs V3K_PT_RC = {
		0.0077, 25.0, 680.0, 34, 0.802, 0.935, 1.0, 1.0, 1.0
	};
	
const typIF97Coeffs_IJn V3K_PT_COEFFS[] = {
	{0,		0,		0.0},				   //0  i starts at 1, so 0th i is not used
	{-2, 	10, 	-.401215699576099e9},
	{-2,	12,		.484501478318406e11},
	{-1, 	-5, 	.394721471363678e-14},
	{-1, 	6, 		.372629967374147e5},
	{0, 	-12, 	-.369794374168666e-29}, //5
	
	{0, 	-6, 	-.380436407012452e-14},
	{0, 	-2, 	.475361629970233e-6},
	{0, 	-1, 	-.879148916140706e-3},
	{0, 	0, 		.844317863844331},
	{0, 	1, 		.122433162656600e2}, //10
	
	{0, 	2, 		-.104529634830279e3},
	{0, 	3, 		.589702771277429e3},
	{0, 	14, 	-.291026851164444e14},
	{1, 	-3, 	.170343072841850e-5},
	{1, 	-2, 	-.277617606975748e-3}, //15
	
	{1, 	0, 		-.344709605486686e1},
	{1, 	1, 		.221333862447095e2},
	{1, 	2, 		-.194646110037079e3},
	{2, 	-8, 	.808354639772825e-15},
	{2, 	-6, 	-.180845209145470e-10}, //20
	
	{2, 	-3, 	-.696664158132412e-5},
	{2, 	-2, 	-.181057560300994e-2},
	{2, 	0, 		.255830298579027e1},
	{2, 	4, 		.328913873658481e4},
	{5, 	-12, 	-.173270241249904e-18}, //25
	
	{5, 	-6, 	-.661876792558034e-6},
	{5, 	-3, 	-.395688923421250e-2},
	{6, 	-12, 	.604203299819132e-17},
	{6, 	-10, 	-.400879935920517e-13},
	{6, 	-8, 	.160751107464958e-8}, //30
	
	{6, 	-5, 	.383719409025556e-4},
	{8, 	-12, 	-.649565446702457e-14},
	{10, 	-12, 	-.149095328506000e-11},
	{12, 	-10, 	.541449377329581e-8}  //34
	};
	
	int i = 1;
	double omegasum = 0;
	
	double pi = p_MPa / V3K_PT_RC.pStar;
	double theta = t_K / V3K_PT_RC.tStar;
	
	#pragma omp parallel for reduction (+:omegasum)
	for (i = 1; i <= V3K_PT_RC.N; i++) {
		
		omegasum += V3K_PT_COEFFS[i].ni * 
			pow(pow((pi - V3K_PT_RC.a), V3K_PT_RC.c) , V3K_PT_COEFFS[i].Ii)
			* pow(pow((theta - V3K_PT_RC.b), V3K_PT_RC.d ), V3K_PT_COEFFS[i].Ji);
			
	}

	return V3K_PT_RC.vStar * pow(omegasum , V3K_PT_RC.e );
}



// specific volume (m3/kg) in region 3l for a given temperature (K) and pressure (MPa)
// equation 4
double if97_r3l_v_pt (double p_MPa, double t_K){

	const typR3RedCoefs V3L_PT_RC = {
		0.0026, 24.0, 650.0, 43, 0.908, 0.989, 1.0, 1.0, 4.0
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
	
	int i = 1;
	double omegasum = 0;
	
	double pi = p_MPa / V3L_PT_RC.pStar;
	double theta = t_K / V3L_PT_RC.tStar;
	
	#pragma omp parallel for reduction (+:omegasum)
	for (i = 1; i <= V3L_PT_RC.N; i++) {
		
		omegasum += V3L_PT_COEFFS[i].ni * 
			pow(pow((pi - V3L_PT_RC.a), V3L_PT_RC.c) , V3L_PT_COEFFS[i].Ii)
			* pow(pow((theta - V3L_PT_RC.b), V3L_PT_RC.d ), V3L_PT_COEFFS[i].Ji);
			
	}

	return V3L_PT_RC.vStar * pow(omegasum , V3L_PT_RC.e );
}
	
	
// specific volume (m3/kg) in region 3m for a given temperature (K) and pressure (MPa)
// equation 4
double if97_r3m_v_pt (double p_MPa, double t_K){

	const typR3RedCoefs V3M_PT_RC = {
		0.0028, 23.0, 650.0, 40, 1.0, 0.997, 1.0, 0.25, 1.0
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
	
	int i = 1;
	double omegasum = 0;
	
	double pi = p_MPa / V3M_PT_RC.pStar;
	double theta = t_K / V3M_PT_RC.tStar;
	
	#pragma omp parallel for reduction (+:omegasum)
	for (i = 1; i <= V3M_PT_RC.N; i++) {
		
		omegasum += V3M_PT_COEFFS[i].ni * 
			pow(pow((pi - V3M_PT_RC.a), V3M_PT_RC.c) , V3M_PT_COEFFS[i].Ii)
			* pow(pow((theta - V3M_PT_RC.b), V3M_PT_RC.d ), V3M_PT_COEFFS[i].Ji);
			
	}

	return V3M_PT_RC.vStar * pow(omegasum , V3M_PT_RC.e );
}



// specific volume (m3/kg) in region 3n for a given temperature (K) and pressure (MPa)
// equation 5
double if97_r3n_v_pt (double p_MPa, double t_K){

	const typR3RedCoefs V3N_PT_RC = {
		0.0031, 23.0, 650.0, 39, 0.976, 0.997, 0.0, 0.0, 0.0
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
	
	int i = 1;
	double omegasum = 0;
	
	double pi = p_MPa / V3N_PT_RC.pStar;
	double theta = t_K / V3N_PT_RC.tStar;
	
	#pragma omp parallel for reduction (+:omegasum)
	for (i = 1; i <= V3N_PT_RC.N; i++) {
		
		omegasum += V3N_PT_COEFFS[i].ni * 
			pow((pi - V3N_PT_RC.a), V3N_PT_COEFFS[i].Ii)
			* pow((theta - V3N_PT_RC.b), V3N_PT_COEFFS[i].Ji );
			
	}

	return V3N_PT_RC.vStar * exp(omegasum);
}


// specific volume (m3/kg) in region 3o for a given temperature (K) and pressure (MPa)
// equation 4
double if97_r3o_v_pt (double p_MPa, double t_K){

	const typR3RedCoefs V3O_PT_RC = {
		0.0034, 23.0, 650.0, 24, 0.974, 0.996, 0.5, 1.0, 1.0
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
	int i = 1;
	double omegasum = 0;
	
	double pi = p_MPa / V3O_PT_RC.pStar;
	double theta = t_K / V3O_PT_RC.tStar;
	
	#pragma omp parallel for reduction (+:omegasum)
	for (i = 1; i <= V3O_PT_RC.N; i++) {
		
		omegasum += V3O_PT_COEFFS[i].ni * 
			pow(pow((pi - V3O_PT_RC.a), V3O_PT_RC.c) , V3O_PT_COEFFS[i].Ii)
			* pow(pow((theta - V3O_PT_RC.b), V3O_PT_RC.d ), V3O_PT_COEFFS[i].Ji);
			
	}

	return V3O_PT_RC.vStar * pow(omegasum , V3O_PT_RC.e );
}


// specific volume (m3/kg) in region 3p for a given temperature (K) and pressure (MPa)
// equation 4
double if97_r3p_v_pt (double p_MPa, double t_K){

	const typR3RedCoefs V3P_PT_RC = {
		0.0041, 23.0, 650.0, 27, 0.972, 0.997, 0.5, 1.0, 1.0
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
	int i = 1;
	double omegasum = 0;
	
	double pi = p_MPa / V3P_PT_RC.pStar;
	double theta = t_K / V3P_PT_RC.tStar;
	
	#pragma omp parallel for reduction (+:omegasum)
	for (i = 1; i <= V3P_PT_RC.N; i++) {
		
		omegasum += V3P_PT_COEFFS[i].ni * 
			pow(pow((pi - V3P_PT_RC.a), V3P_PT_RC.c) , V3P_PT_COEFFS[i].Ii)
			* pow(pow((theta - V3P_PT_RC.b), V3P_PT_RC.d ), V3P_PT_COEFFS[i].Ji);
			
	}

	return V3P_PT_RC.vStar * pow(omegasum , V3P_PT_RC.e );
}



// specific volume (m3/kg) in region 3q for a given temperature (K) and pressure (MPa)
// equation 4
double if97_r3q_v_pt (double p_MPa, double t_K){

	const typR3RedCoefs V3Q_PT_RC = {
		0.0022, 23.0, 650.0, 24, 0.848, 0.983, 1.0, 1.0, 4.0
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
	int i = 1;
	double omegasum = 0;
	
	double pi = p_MPa / V3Q_PT_RC.pStar;
	double theta = t_K / V3Q_PT_RC.tStar;
	
	#pragma omp parallel for reduction (+:omegasum)
	for (i = 1; i <= V3Q_PT_RC.N; i++) {
		
		omegasum += V3Q_PT_COEFFS[i].ni * 
			pow(pow((pi - V3Q_PT_RC.a), V3Q_PT_RC.c) , V3Q_PT_COEFFS[i].Ii)
			* pow(pow((theta - V3Q_PT_RC.b), V3Q_PT_RC.d ), V3Q_PT_COEFFS[i].Ji);
			
	}

	return V3Q_PT_RC.vStar * pow(omegasum , V3Q_PT_RC.e );
}


// specific volume (m3/kg) in region 3r for a given temperature (K) and pressure (MPa)
// equation 4
double if97_r3r_v_pt (double p_MPa, double t_K){

	const typR3RedCoefs V3R_PT_RC = {
		0.0054, 23.0, 650.0, 27, 0.874, 0.982, 1.0, 1.0, 1.0
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
	int i = 1;
	double omegasum = 0;
	
	double pi = p_MPa / V3R_PT_RC.pStar;
	double theta = t_K / V3R_PT_RC.tStar;
	
	#pragma omp parallel for reduction (+:omegasum)
	for (i = 1; i <= V3R_PT_RC.N; i++) {
		
		omegasum += V3R_PT_COEFFS[i].ni * 
			pow(pow((pi - V3R_PT_RC.a), V3R_PT_RC.c) , V3R_PT_COEFFS[i].Ii)
			* pow(pow((theta - V3R_PT_RC.b), V3R_PT_RC.d ), V3R_PT_COEFFS[i].Ji);
			
	}

	return V3R_PT_RC.vStar * pow(omegasum , V3R_PT_RC.e );
}


// specific volume (m3/kg) in region 3s for a given temperature (K) and pressure (MPa)
// equation 4
double if97_r3s_v_pt (double p_MPa, double t_K){

	const typR3RedCoefs V3S_PT_RC = {
		0.0022, 21.0, 640.0, 29, 0.886, 0.990, 1.0, 1.0, 4.0
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
	int i = 1;
	double omegasum = 0;
	
	double pi = p_MPa / V3S_PT_RC.pStar;
	double theta = t_K / V3S_PT_RC.tStar;
	
	#pragma omp parallel for reduction (+:omegasum)
	for (i = 1; i <= V3S_PT_RC.N; i++) {
		
		omegasum += V3S_PT_COEFFS[i].ni * 
			pow(pow((pi - V3S_PT_RC.a), V3S_PT_RC.c) , V3S_PT_COEFFS[i].Ii)
			* pow(pow((theta - V3S_PT_RC.b), V3S_PT_RC.d ), V3S_PT_COEFFS[i].Ji);
			
	}

	return V3S_PT_RC.vStar * pow(omegasum , V3S_PT_RC.e );
}


// specific volume (m3/kg) in region 3t for a given temperature (K) and pressure (MPa)
// equation 4
double if97_r3t_v_pt (double p_MPa, double t_K){

	const typR3RedCoefs V3T_PT_RC = {
		0.0088, 20.0, 650.0, 33, 0.803, 1.02, 1.0, 1.0, 1.0
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
	int i = 1;
	double omegasum = 0;
	
	double pi = p_MPa / V3T_PT_RC.pStar;
	double theta = t_K / V3T_PT_RC.tStar;
	
	#pragma omp parallel for reduction (+:omegasum)
	for (i = 1; i <= V3T_PT_RC.N; i++) {
		
		omegasum += V3T_PT_COEFFS[i].ni * 
			pow(pow((pi - V3T_PT_RC.a), V3T_PT_RC.c) , V3T_PT_COEFFS[i].Ii)
			* pow(pow((theta - V3T_PT_RC.b), V3T_PT_RC.d ), V3T_PT_COEFFS[i].Ji);
			
	}

	return V3T_PT_RC.vStar * pow(omegasum , V3T_PT_RC.e );
}	
	


/* ****   AUXILLIARY CONSTANTS FOR CLOSE TO CRITICAL REGIION ******* */



// specific volume (m3/kg) in near critical region 3u for a given temperature (K) and pressure (MPa)
// equation 4
double if97_r3u_v_pt (double p_MPa, double t_K){

	const typR3RedCoefs V3U_PT_RC = {
		0.0026, 23.0, 650.0, 38, 0.902, 0.988, 1.0, 1.0, 1.0
	};
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
	int i = 1;
	double omegasum = 0;
	
	double pi = p_MPa / V3U_PT_RC.pStar;
	double theta = t_K / V3U_PT_RC.tStar;
	
	#pragma omp parallel for reduction (+:omegasum)
	for (i = 1; i <= V3U_PT_RC.N; i++) {
		
		omegasum += V3U_PT_COEFFS[i].ni * 
			pow(pow((pi - V3U_PT_RC.a), V3U_PT_RC.c) , V3U_PT_COEFFS[i].Ii)
			* pow(pow((theta - V3U_PT_RC.b), V3U_PT_RC.d ), V3U_PT_COEFFS[i].Ji);
			
	}

	return V3U_PT_RC.vStar * pow(omegasum , V3U_PT_RC.e );
}	


// specific volume (m3/kg) in near critical region 3v for a given temperature (K) and pressure (MPa)
// equation 4
double if97_r3v_v_pt (double p_MPa, double t_K){

	const typR3RedCoefs V3V_PT_RC = {
		0.0031, 23.0, 650.0, 39, 0.960, 0.995, 1.0, 1.0, 1.0
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
	int i = 1;
	double omegasum = 0;
	
	double pi = p_MPa / V3V_PT_RC.pStar;
	double theta = t_K / V3V_PT_RC.tStar;
	
	#pragma omp parallel for reduction (+:omegasum)
	for (i = 1; i <= V3V_PT_RC.N; i++) {
		
		omegasum += V3V_PT_COEFFS[i].ni * 
			pow(pow((pi - V3V_PT_RC.a), V3V_PT_RC.c) , V3V_PT_COEFFS[i].Ii)
			* pow(pow((theta - V3V_PT_RC.b), V3V_PT_RC.d ), V3V_PT_COEFFS[i].Ji);
			
	}

	return V3V_PT_RC.vStar * pow(omegasum , V3V_PT_RC.e );
}	


// specific volume (m3/kg) in near critical region 3w for a given temperature (K) and pressure (MPa)
// equation 4
double if97_r3w_v_pt (double p_MPa, double t_K){

	const typR3RedCoefs V3W_PT_RC = {
		0.0039, 23.0, 650.0, 35, 0.959, 0.995, 1.0, 1.0, 4.0
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
		{10, -8, .858133791857099e-5} //35
	};
	int i = 1;
	double omegasum = 0;
	
	double pi = p_MPa / V3W_PT_RC.pStar;
	double theta = t_K / V3W_PT_RC.tStar;
	
	#pragma omp parallel for reduction (+:omegasum)
	for (i = 1; i <= V3W_PT_RC.N; i++) {
		
		omegasum += V3W_PT_COEFFS[i].ni * 
			pow(pow((pi - V3W_PT_RC.a), V3W_PT_RC.c) , V3W_PT_COEFFS[i].Ii)
			* pow(pow((theta - V3W_PT_RC.b), V3W_PT_RC.d ), V3W_PT_COEFFS[i].Ji);
			
	}

	return V3W_PT_RC.vStar * pow(omegasum , V3W_PT_RC.e );
}	


// specific volume (m3/kg) in near critical region 3x for a given temperature (K) and pressure (MPa)
// equation 4
double if97_r3x_v_pt (double p_MPa, double t_K){

	const typR3RedCoefs V3X_PT_RC = {
		0.0049, 23.0, 650.0, 36, 0.910, 0.988, 1.0, 1.0, 1.0
	};
	//From Hummeling if97 Java
	// http://sourceforge.net/p/if97/git/ci/master/tree/src/main/java/com/hummeling/if97/Region3.java#l1801
	const typIF97Coeffs_IJn V3X_PT_COEFFS[] = {
		{0,	0,	0.},			   //0  i starts at 1, so 0th i is not used
		{-8, 14, .377373741298151e19},
		{-6, 10, -.507100883722913e13},
		{-5, 10, -.103363225598860e16},
		{-4, 1, .184790814320773e-5},
		{-4, 2, -.924729378390945e-3}, //5
		
		{-4, 14, -.425999562292738e24},
		{-3, -2, -.462307771873973e-12},
		{-3, 12, .107319065855767e22},
		{-1, 5, .648662492280682e11},
		{0, 0, .244200600688281e1}, //10
		
		{0, 4, -.851535733484258e10},
		{0, 10, .169894481433592e22},
		{1, -10, .215780222509020e-26},
		{1, -1, -.320850551367334},
		{2, 6, -.382642448458610e17}, //15
		
		{3, -12, -.275386077674421e-28},
		{3, 0, -.563199253391666e6},
		{3, 8, -.326068646279314e21},
		{4, 3, .397949001553184e14},
		{5, -6, .100824008584757e-6}, //20
		
		{5, -2, .162234569738433e5},
		{5, 1, -.432355225319745e11},
		{6, 1, -.592874245598610e12},
		{8, -6, .133061647281106e1},
		{8, -3, .157338197797544e7}, //25
		
		{8, 1, .258189614270853e14},
		{8, 8, .262413209706358e25},
		{10, -8, -.920011937431142e-1},
		{12, -10, .220213765905426e-2},
		{12, -8, -.110433759109547e2}, //30
		
		{12, -5, .847004870612087e7},
		{12, -4, -.592910695762536e9},
		{14, -12, -.183027173269660e-4},
		{14, -10, .181339603516302},
		{14, -8, -.119228759669889e4}, //35
		
		{14, -6, .430867658061468e7} //36
	};
	int i = 1;
	double omegasum = 0;
	
	double pi = p_MPa / V3X_PT_RC.pStar;
	double theta = t_K / V3X_PT_RC.tStar;
	
	#pragma omp parallel for reduction (+:omegasum)
	for (i = 1; i <= V3X_PT_RC.N; i++) {
		
		omegasum += V3X_PT_COEFFS[i].ni * 
			pow(pow((pi - V3X_PT_RC.a), V3X_PT_RC.c) , V3X_PT_COEFFS[i].Ii)
			* pow(pow((theta - V3X_PT_RC.b), V3X_PT_RC.d ), V3X_PT_COEFFS[i].Ji);
			
	}

	return V3X_PT_RC.vStar * pow(omegasum , V3X_PT_RC.e );
}	


// specific volume (m3/kg) in near critical region 3y for a given temperature (K) and pressure (MPa)
// equation 4
double if97_r3y_v_pt (double p_MPa, double t_K){

	const typR3RedCoefs V3Y_PT_RC = {
		0.0031, 22.0, 650.0, 20, 0.996, 0.994, 1.0, 1.0, 4.0
	};
	//From Hummeling if97 Java
	// http://sourceforge.net/p/if97/git/ci/master/tree/src/main/java/com/hummeling/if97/Region3.java#l1801
	const typIF97Coeffs_IJn V3Y_PT_COEFFS[] = {
		{0,	0,	0.},			   //0  i starts at 1, so 0th i is not used
		{0, -3, -.525597995024633e-9},
		{0, 1, .583441305228407e4},
		{0, 5, -.134778968457925e17},
		{0, 8, .118973500934212e26},
		{1, 8, -.159096490904708e27}, //5
		
		{2, -4, -.315839902302021e-6},
		{2, -1, .496212197158239e3},
		{2, 4, .327777227273171e19},
		{2, 5, -.527114657850696e22},
		{3, -8, .210017506281863e-16}, //10
		
		{3, 4, .705106224399834e21},
		{3, 8, -.266713136106469e31},
		{4, -6, -.145370512554562e-7},
		{4, 6, .149333917053130e28},
		{5, -2, -.149795620287641e8}, //15
		
		{5, 1, -.381881906271100e16},
		{8, -8, .724660165585797e-4},
		{8, -2, -.937808169550193e14},
		{10, -5, .514411468376383e10},
		{12, -8, -.828198594040141e5} //20
	};
	int i = 1;
	double omegasum = 0;
	
	double pi = p_MPa / V3Y_PT_RC.pStar;
	double theta = t_K / V3Y_PT_RC.tStar;
	
	#pragma omp parallel for reduction (+:omegasum)
	for (i = 1; i <= V3Y_PT_RC.N; i++) {
		
		omegasum += V3Y_PT_COEFFS[i].ni * 
			pow(pow((pi - V3Y_PT_RC.a), V3Y_PT_RC.c) , V3Y_PT_COEFFS[i].Ii)
			* pow(pow((theta - V3Y_PT_RC.b), V3Y_PT_RC.d ), V3Y_PT_COEFFS[i].Ji);
			
	}

	return V3Y_PT_RC.vStar * pow(omegasum , V3Y_PT_RC.e );
}	


// specific volume (m3/kg) in near critical region 3z for a given temperature (K) and pressure (MPa)
// equation 4
double if97_r3z_v_pt (double p_MPa, double t_K){

	const typR3RedCoefs V3Z_PT_RC = {
		0.0038, 22.0, 650.0, 23, 0.993, 0.994, 1.0, 1.0, 4.0
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
	int i = 1;
	double omegasum = 0;
	
	double pi = p_MPa / V3Z_PT_RC.pStar;
	double theta = t_K / V3Z_PT_RC.tStar;
	
	#pragma omp parallel for reduction (+:omegasum)
	for (i = 1; i <= V3Z_PT_RC.N; i++) {
		
		omegasum += V3Z_PT_COEFFS[i].ni * 
			pow(pow((pi - V3Z_PT_RC.a), V3Z_PT_RC.c) , V3Z_PT_COEFFS[i].Ii)
			* pow(pow((theta - V3Z_PT_RC.b), V3Z_PT_RC.d ), V3Z_PT_COEFFS[i].Ji);
			
	}

	return V3Z_PT_RC.vStar * pow(omegasum , V3Z_PT_RC.e );
}



//  Region 3 specific volume (m3/kg) using backwards equations.  These meet the
//   criteria of 0.001% on enthalpy and entropy specific volume and 0.01% on Cp
//   outside the near critical region, which can be checked with "isNearCritical"
//   in the near critical region the backwards equations should provide a good starting 
//   guess for iteration 
double if97_R3bw_v_pt (double p_MPa, double t_K){
	
	//array of pointers to region functions
	double (*ptrR3bw_v[26])(double, double) = {
		if97_r3a_v_pt,
		if97_r3b_v_pt,
		if97_r3c_v_pt,
		if97_r3d_v_pt,
		if97_r3e_v_pt,
		if97_r3f_v_pt,
		if97_r3g_v_pt,
		if97_r3h_v_pt,
		if97_r3i_v_pt,
		if97_r3j_v_pt,
		if97_r3k_v_pt,
		if97_r3l_v_pt,
		if97_r3m_v_pt,
		if97_r3n_v_pt,
		if97_r3o_v_pt,
		if97_r3p_v_pt,
		if97_r3q_v_pt,
		if97_r3r_v_pt,
		if97_r3s_v_pt,
		if97_r3t_v_pt,
		if97_r3u_v_pt,
		if97_r3v_v_pt,
		if97_r3w_v_pt,
		if97_r3x_v_pt,
		if97_r3y_v_pt,
		if97_r3z_v_pt
	};
	
	int i = 0;
	
	char R3_bw_region = if97_r3_pt_subregion(p_MPa, t_K);

	i =(int)R3_bw_region - (int)'a';
	
	if ((i < 0) || (i >25)) return 0.00; //ERROR
	
	else return ptrR3bw_v[i] (p_MPa, t_K);
}