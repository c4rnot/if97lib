//          Copyright Martin Lord 2014-2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


//    IAPWS-IF97 Steam Tables

 
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
#include "IF97_Region1.h"
#include "IF97_Region2.h"
#include "IF97_B23.h"
#include "IF97_Region3.h"
#include "IF97_Region4.h"
#include "IF97_Region5.h"
#include "solve.h"
#include <math.h> // for pow, log


#include <stdio.h>  //used for debugging only



// ******  Used internally   *******


/* returns the region for a given pressure and temperature based on Fig 1
* 0 = out of bounds.  integers represent regions
* cannot return region 4. It is impossible to sit on the knife edge.
*/
int region_pt(double p_MPa, double t_K) {
	
	if (p_MPa > IF97_R1_UPRESS )  return 0 ; // valid also for R2, R3
	
	else if (t_K > IF97_R5_UTEMP)  return 0 ; // outside valid bounds
	
	else if (t_K < IF97_R1_LTEMP )  return 0 ; // outside valid bounds
	
	else if (p_MPa < IF97_R1_LPRESS )  return 0 ; // valid also for R2
			
	else if (t_K > IF97_R2_UTEMP) {
		if (p_MPa > IF97_R5_UPRESS) return 0;
		else return 5;
	}

	else if (p_MPa < IF97_B23_LPRESS){
		if (if97_r4_ts (p_MPa)> t_K) return 1;
		else return 2;	
	} 
	

	else if (t_K < IF97_R1_UTEMP) {
		return 1;
		}
	else if  (IF97_B23T(p_MPa) < t_K) return 2;
	
	else return 3;
}





// ******  External   *******



// SATURATION LINE

// saturation temperature for a given pressure */
double if97_Ps_t(double Ps_MPa) {return if97_r4_ts (Ps_MPa ); }

// saturation pressure for a given temperature */
double if97_Ts_p(double Ts_K) {return if97_r4_ps (Ts_K);}



// Known Pressure and Temperature

/* specific enthalpy for a given p_MPa and t_K */
double if97_pt_h(double p_MPa, double t_K){
	typSolvResult slvResult;

switch (region_pt(p_MPa, t_K)) {
	case 1 :
		return if97_r1_h(p_MPa, t_K);
		break;
	case 2 :
		return if97_r2_h(p_MPa, t_K);
		break;
	case 3:
		if (!(isNearCritical(p_MPa, t_K))) return if97_r3_h( 1/if97_R3bw_v_pt (p_MPa, t_K), t_K); // use backwards equations if not in the Auxiliary zone
		else{  // near critical.  Needs iteration
			slvResult = secant_solv(if97_r3_p, t_K, false,  p_MPa, 1/if97_R3bw_v_pt (p_MPa, t_K), 0.05, TEST_ACCURACY, SIG_FIG, 100 );
			slvResult.dSolution;
			return if97_r3_h (slvResult.dSolution , t_K );
		break;
		}
	case 5: 
		return if97_r5_h(p_MPa, t_K);
		break;
	}
return -9998.0;  //error region not valid
}


/* specific enthalpy for a given p_MPa and t_K */
double if97_pt_u(double p_MPa, double t_K){
	typSolvResult slvResult;

switch (region_pt(p_MPa, t_K)) {
	case 1 :
		return if97_r1_u(p_MPa, t_K);
		break;
	case 2 :
		return if97_r2_u(p_MPa, t_K);
		break;
	case 3:
		if (!(isNearCritical(p_MPa, t_K))) return if97_r3_u( 1/if97_R3bw_v_pt (p_MPa, t_K), t_K); // use backwards equations if not in the Auxiliary zone
		else{  // near critical.  Needs iteration
			slvResult = secant_solv(if97_r3_p, t_K, false,  p_MPa, 1/if97_R3bw_v_pt (p_MPa, t_K), 0.05, TEST_ACCURACY, SIG_FIG, 100 );
			slvResult.dSolution;
			return if97_r3_u (slvResult.dSolution , t_K );
		break;
		}
	case 5: 
		return if97_r5_h(p_MPa, t_K);
		break;
	}
return -9998.0;  //error region not valid
}


/* specific entropy for a given p_MPa and t_K */
double if97_pt_s(double p_MPa, double t_K){
	typSolvResult slvResult;
	switch (region_pt(p_MPa, t_K)) {
	case 1 :
		return if97_r1_s(p_MPa, t_K);
		break;
	case 2 :
		return if97_r2_s(p_MPa, t_K);
		break;
	case 3:
		// use backwards equations to determine rho if not in the Auxiliary zone
		if (!(isNearCritical(p_MPa, t_K))) return if97_r3_s( 1/(if97_R3bw_v_pt (p_MPa, t_K)), t_K); 
		else{  // near critical.  Needs iteration
			slvResult = secant_solv(if97_r3_p, t_K, false,  p_MPa, 1/if97_R3bw_v_pt (p_MPa, t_K), 0.05, TEST_ACCURACY, SIG_FIG, 100 );
			slvResult.dSolution;
			return if97_r3_s (slvResult.dSolution , t_K );
		break;
		}
	case 5: //practically impossible
		return if97_r5_s(p_MPa, t_K);
		break;
	}
return -9998.0; //error region not valid
	
}


/* specific volume for a given p_MPa and t_K */
double if97_pt_v(double p_MPa, double t_K){
	typSolvResult slvResult;
	switch (region_pt(p_MPa, t_K)) {	
	case 1 :
		return if97_r1_v(p_MPa, t_K);
		break;
	case 2 :
		return if97_r2_v(p_MPa, t_K);
		break;
	case 3:
		// use backwards equations to determine rho if not in the Auxiliary zone
		if (!(isNearCritical(p_MPa, t_K))) return  1/(if97_R3bw_v_pt (p_MPa, t_K)); 
		else{  // near critical.  Needs iteration
			slvResult = secant_solv(if97_r3_p, t_K, false,  p_MPa, 1/if97_R3bw_v_pt (p_MPa, t_K), 0.05, TEST_ACCURACY, SIG_FIG, 100 );
			slvResult.dSolution;
			return 1/slvResult.dSolution;
		break;
		}
	case 5: //practically impossible
		return if97_r5_v(p_MPa, t_K);
		break;
	}
return -9998.0;  //error region not valid
}


/* specific isochoric heat capacity for a given p_MPa and t_K */
double if97_pt_Cv(double p_MPa, double t_K){
	typSolvResult slvResult;
	switch (region_pt(p_MPa, t_K)) {
	case 1 :
		return if97_r1_Cv(p_MPa, t_K);
		break;
	case 2 :
		return if97_r2_Cv(p_MPa, t_K);
		break;
	case 3:
		// use backwards equations to determine rho if not in the Auxiliary zone
		if (!(isNearCritical(p_MPa, t_K))) return if97_r3_Cv( 1/(if97_R3bw_v_pt (p_MPa, t_K)), t_K); 
		else{  // near critical.  Needs iteration
			slvResult = secant_solv(if97_r3_p, t_K, false,  p_MPa, 1/if97_R3bw_v_pt (p_MPa, t_K), 0.05, TEST_ACCURACY, SIG_FIG, 100 );
			slvResult.dSolution;
			return if97_r3_Cv (slvResult.dSolution , t_K );
		break;
		}
	case 5: //practically impossible
		return if97_r5_Cv(p_MPa, t_K);
		break;
	}
return -9998.0; //error region not valid
}




/* specific isochoric heat capacity for a given p_MPa and t_K */
double if97_pt_Cp(double p_MPa, double t_K){
	typSolvResult slvResult;
	switch (region_pt(p_MPa, t_K)) {
	case 1 :
		return if97_r1_Cp(p_MPa, t_K);
		break;
	case 2 :
		return if97_r2_Cp(p_MPa, t_K);
		break;
	case 3:
		// use backwards equations to determine rho if not in the Auxiliary zone
		if (!(isNearCritical(p_MPa, t_K))) return if97_r3_Cp( 1/(if97_R3bw_v_pt (p_MPa, t_K)), t_K); 
		else{  // near critical.  Needs iteration
			slvResult = secant_solv(if97_r3_p, t_K, false,  p_MPa, 1/if97_R3bw_v_pt (p_MPa, t_K), 0.05, TEST_ACCURACY, SIG_FIG, 100 );
			slvResult.dSolution;
			return if97_r3_Cp (slvResult.dSolution , t_K );
		break;
		}
	case 5: //practically impossible
		return if97_r5_Cp(p_MPa, t_K);
		break;
	}
return -9998.0; //error region not valid
}




/* thermal conductivity for a given p_MPa and t_K TODO */
double if97_pt_k(double p_MPa, double t_K){
return -9999.0; // TODO
}

/* dynamic viscosity for a given p_MPa and t_K  TODO */
double if97_pt_mu(double p_MPa, double t_K){
return -9999.0; // TODO
}


/** speed of sound for a given p_MPa and t_K */
double if97_pt_Vs(double p_MPa, double t_K){
	typSolvResult slvResult;
	switch (region_pt(p_MPa, t_K)) {
	case 1 :
		return if97_r1_w(p_MPa, t_K);
		break;
	case 2 :
		return if97_r2_w(p_MPa, t_K);
		break;
	case 3:
		// use backwards equations to determine rho if not in the Auxiliary zone
		if (!(isNearCritical(p_MPa, t_K))) return if97_r3_Cp( 1/(if97_R3bw_v_pt (p_MPa, t_K)), t_K); 
		else{  // near critical.  Needs iteration
			slvResult = secant_solv(if97_r3_p, t_K, false,  p_MPa, 1/if97_R3bw_v_pt (p_MPa, t_K), 0.05, TEST_ACCURACY, SIG_FIG, 100 );
			slvResult.dSolution;
			return if97_r3_w (slvResult.dSolution , t_K );
		break;
		}
	case 5: //practically impossible
		return if97_r5_w(p_MPa, t_K);
		break;
	}
return -9998.0; //error region not valid
}



/** isentropic expansion coefficient for a given p_MPa and t_K */
double if97_pt_gamma(double p_MPa, double t_K){
	typSolvResult slvResult;
	switch (region_pt(p_MPa, t_K)) {
	case 1 :
		return (if97_r1_Cp(p_MPa, t_K) / if97_r1_Cv(p_MPa, t_K));
		break;
	case 2 :
		return (if97_r2_Cp(p_MPa, t_K)/if97_r2_Cv(p_MPa, t_K));
		break;
	case 3:
		// use backwards equations to determine rho if not in the Auxiliary zone
		if (!(isNearCritical(p_MPa, t_K))){
			return if97_r3_Cp( 1/(if97_R3bw_v_pt (p_MPa, t_K)), t_K)/if97_r3_Cv( 1/(if97_R3bw_v_pt (p_MPa, t_K)), t_K);
		}
		else{  // near critical.  Needs iteration
			slvResult = secant_solv(if97_r3_p, t_K, false,  p_MPa, 1/if97_R3bw_v_pt (p_MPa, t_K), 0.05, TEST_ACCURACY, SIG_FIG, 100 );
			slvResult.dSolution;
			return if97_r3_Cp (slvResult.dSolution , t_K )/if97_r3_Cv (slvResult.dSolution , t_K );
		break;
		}
	case 5: //practically impossible
		return (if97_r5_Cp(p_MPa, t_K) / if97_r5_Cv(p_MPa, t_K));
		break;
	}
return -9998.0; //error region not valid;
}


/** full steam state for a given p_MPa and t_K */
typSteamState if97_pt_state(double p_MPa, double t_K){
	typSteamState returnState;
	
	returnState.p_MPa  = -9999.0;
	returnState.t_K  = -9999.0;
	
	// TODO
	
	
	return returnState;
}



