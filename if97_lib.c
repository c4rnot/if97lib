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

// saturation pressure for a given temperatrue */
double if97_Ts_p(double Ts_K) {return if97_r4_ps (Ts_K);}



// Known Pressure and Temperature

/* specific enthalpy for a given p_MPa and t_K */
double if97_pt_h(double p_MPa, double t_K){

switch (region_pt(p_MPa, t_K)) {
	case 1 :
		printf ("\n Wrong region 1!\n");	
		return if97_r1_h(p_MPa, t_K);
		break;
	case 2 :
		printf ("\n found the right region\n");	
		return if97_r2_h(p_MPa, t_K);
		break;
	case 3:
		//return if97_r3_h(rho_kgperM3, t_K);
		//TODO.
		printf ("\n Wrong region 3!\n");	
		return -9999.0;
		break;
	case 5: 
		printf ("\n Wrong region 5!\n");	
		return if97_r5_h(p_MPa, t_K);
		break;
	}
return -9998.0;  //error region not valid
};



/* specific entropy for a given p_MPa and t_K */
double if97_pt_s(double p_MPa, double t_K){
	switch (region_pt(p_MPa, t_K)) {
	case 1 :
		return if97_r1_s(p_MPa, t_K);
		break;
	case 2 :
		return if97_r2_s(p_MPa, t_K);
		break;
	case 3:
		//return if97_r3_s(rho_kgperM3, t_K);
		//TODO.
		return -9999.0;
		break;
	case 5: //practically impossible
		return if97_r5_s(p_MPa, t_K);
		break;
	}
return -9998.0; //error region not valid
	
};


/* specific volume for a given p_MPa and t_K */
double if97_pt_v(double p_MPa, double t_K){
	switch (region_pt(p_MPa, t_K)) {	
	case 1 :
		return if97_r1_v(p_MPa, t_K);
		break;
	case 2 :
		return if97_r2_v(p_MPa, t_K);
		break;
	case 3:
		//return if97_r3_v(rho_kgperM3, t_K);
		//TODO.
		return -9999.0;
		break;
	case 5: //practically impossible
		return if97_r5_v(p_MPa, t_K);
		break;
	}
return -9998.0;  //error region not valid
};


/* specific isochoric heat capacity for a given p_MPa and t_K */
double if97_pt_Cv(double p_MPa, double t_K){
		switch (region_pt(p_MPa, t_K)) {
	case 1 :
		return if97_r1_Cv(p_MPa, t_K);
		break;
	case 2 :
		return if97_r2_Cv(p_MPa, t_K);
		break;
	case 3:
		//return if97_r3_Cv(rho_kgperM3, t_K);
		//TODO.
		return -9999.0;
		break;
	case 5: //practically impossible
		return if97_r5_Cv(p_MPa, t_K);
		break;
	}
return -9998.0; //error region not valid
};




/* specific isochoric heat capacity for a given p_MPa and t_K */
double if97_pt_Cp(double p_MPa, double t_K){
		switch (region_pt(p_MPa, t_K)) {
	case 1 :
		return if97_r1_Cp(p_MPa, t_K);
		break;
	case 2 :
		return if97_r2_Cp(p_MPa, t_K);
		break;
	case 3:
		//return if97_r3_Cp(rho_kgperM3, t_K);
		//TODO.
		return -9999.0;
		break;
	case 5: //practically impossible
		return if97_r5_Cp(p_MPa, t_K);
		break;
	}
return -9998.0; //error region not valid
};




/* thermal conductivity for a given p_MPa and t_K TODO */
double if97_pt_k(double p_MPa, double t_K){
return -9999.0; // TODO
};

/* dynamic viscosity for a given p_MPa and t_K  TODO */
double if97_pt_mu(double p_MPa, double t_K){
return -9999.0; // TODO
};


/** speed of sound for a given p_MPa and t_K */
double if97_pt_Vs(double p_MPa, double t_K){
		switch (region_pt(p_MPa, t_K)) {
	case 1 :
		return if97_r1_w(p_MPa, t_K);
		break;
	case 2 :
		return if97_r2_w(p_MPa, t_K);
		break;
	case 3:
		//return if97_r3_Cp(rho_kgperM3, t_K);
		//TODO.
		return -9999.0;
		break;
	case 5: //practically impossible
		return if97_r5_w(p_MPa, t_K);
		break;
	}
return -9998.0; //error region not valid
};



/** isentropic expansion coefficient for a given p_MPa and t_K */
double if97_pt_gamma(double p_MPa, double t_K){
		switch (region_pt(p_MPa, t_K)) {
	case 1 :
		return (if97_r1_Cp(p_MPa, t_K) / if97_r1_Cv(p_MPa, t_K));
		break;
	case 2 :
		return (if97_r2_Cp(p_MPa, t_K)/if97_r2_Cv(p_MPa, t_K));
		break;
	case 3:
		//return if97_r3_Cp(rho_kgperM3, t_K);
		//TODO.
		return -9999.0;
		break;
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



