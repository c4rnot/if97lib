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
   

/**
 * @copyright
 * Copyright Martin Lord 2014-2015. \n
 * Distributed under the Boost Software License, Version 1.0. \n
 * (See accompanying file LICENSE_1_0.txt or copy at \n
 * http://www.boost.org/LICENSE_1_0.txt) \n
 * 
 * @file if97_lib.h
 * @author Martin Lord
 * @date 28 May 2015
 * @brief IAPWS-IF97 Steam Tables
 * @details 
  * NOT YET COMPLETE!


 
 * @see http://www.iapws.org/relguide/IF97-Rev.html
 */


#ifndef IF97_LIB_H
#define IF97_LIB_H



#include "IF97_common.h"  //PSTAR TSTAR & sqr, structures
#include "IF97_Region1.h"
#include "IF97_Region2.h"
#include "IF97_Region3.h"
#include "IF97_Region4.h"
#include "IF97_Region5.h"
#include <math.h> // for pow, log


// SATURATION LINE


/** saturation temperature (K) for a given pressure (MPa) */
double if97_Ps_t(double Ps_MPa);

/** saturation pressure (MPa) for a given temperature (K) */
double if97_Ts_p(double Ts_K);


// PT

/** specific enthalpy (kJ/kg) for a given p_(MPa) and t_(K) */
double if97_pt_h(double p_MPa, double t_K);

/** specific entropy (kJ/kg/K)for a given p_(MPa) and t_(K) */
double if97_pt_s(double p_MPa, double t_K);

/** specific volume (kg/m3)for a given p_(MPa) and t_(K) */
double if97_pt_v(double p_MPa, double t_K);

/** specific isochoric heat capacity for a given p_MPa and t_K */
double if97_pt_Cv(double p_MPa, double t_K);

/** specific isobaric heat capacity for a given p_MPa and t_K */
double if97_pt_Cp(double p_MPa, double t_K);

/** thermal conductivity for a given p_MPa and t_K  TODO */
double if97_pt_k(double p_MPa, double t_K);

/** dynamic viscosity for a given p_MPa and t_K  TODO */
double if97_pt_mu(double p_MPa, double t_K);

/** speed of sound (m/s)for a given p_(MPa) and t_(K) */
double if97_pt_Vs(double p_MPa, double t_K);

/** isentropic expansion coefficient (-) for a given p_MPa and t_K */
double if97_pt_gamma(double p_MPa, double t_K);

/** full steam state for a given p_MPa and t_K */
typSteamState if97_pt_state(double p_MPa, double t_K);


// PH  TODO

/** t_K for a given p_MPa and h_KJperKg */
double if97_ph_t(double p_MPa, double h_KJperKg);

/** specific h_KJperKgK for a given p_MPa and h_KJperKg */
double if97_ph_s(double p_MPa, double h_KJperKg);

/** specific volume for a given p_MPa and h_KJperKg */
double if97_ph_v(double p_MPa, double h_KJperKg);

/** qual_pct for a given p_MPa and h_KJperKg */
double if97_ph_q(double p_MPa, double h_KJperKg);

/** specific heat capacity #TODO - Which one# for a given p_MPa and h_KJperKg */
double if97_ph_Cp(double p_MPa, double h_KJperKg);

/** speed of sound for a given p_MPa and h_KJperKg */
double if97_ph_Vs(double p_MPa, double h_KJperKg);

/** isentropic expansion coefficient for a given p_MPa and h_KJperKg */
double if97_ph_gamma(double p_MPa, double h_KJperKg);


// PS  TODO

/** t_K for a given p_MPa and h_KJperKgK */
double if97_ps_t(double p_MPa, double h_KJperKgK);

/** specific h_KJperKg for a given p_MPa and h_KJperKgK */
double if97_ps_h(double p_MPa, double h_KJperKgK);

/** specific volume for a given p_MPa and h_KJperKgK */
double if97_ps_v(double p_MPa, double h_KJperKgK);

/** qual_pct for a given p_MPa and h_KJperKgK */
double if97_ps_q(double p_MPa, double h_KJperKgK);

/** specific heat capacity #TODO - Which one# for a given p_MPa and h_KJperKgK */
double if97_ps_Cp(double p_MPa, double h_KJperKgK);

/** speed of sound for a given p_MPa and h_KJperKgK */
double if97_ps_Vs(double p_MPa, double h_KJperKgK);

/** isentropic expansion coefficient for a given p_MPa and h_KJperKgK */
double if97_ps_gamma(double p_MPa, double h_KJperKgK);


/** full steam state for a givenp_MPa and h_KJperKgK */
typSteamState if97_ps_state(double p_MPa, double h_KJperKgK);



// TQ   TODO

/** specific h_KJperKg for a given t_K and qual_pct */
double if97_tq_h(double t_K, double qual_pct);

/** specific h_KJperKgK for a given t_K and qual_pct */
double if97_tq_s(double t_K, double qual_pct);

/** specific volume for a given t_K and qual_pct */
double if97_tq_v(double t_K, double qual_pct);

/** specific heat capacity #TODO - Which one#  for a given t_K and qual_pct */
double if97_tq_Cp(double t_K, double qual_pct);

/** thermal conductivity for a given t_K and qual_pct */
double if97_tq_k(double t_K, double qual_pct);

/** dynamic viscosity for a given t_K and qual_pct */
double if97_tq_mu(double t_K, double qual_pct);

/** sonic speed (m/s)for a given t_(K) and quality_(percent) */
double if97_tq_Vs(double t_K, double qual_pct);

/** isentropic expansion coefficient # TODO - what is this? # for a given t_K and qual_pct */
double if97_tq_gamma(double t_K, double qual_pct);

/** full steam state for a given t_K and qual_pct */
typSteamState if97_tq_state(double t_K, double qual_pct);



//PQ  TODO

/** specific h_(KJ/Kg) for a given p_(MPa) and steam quality_(percent) */
double if97_pq_h(double p_MPa, double qual_pct);

/** specific h_KJperKgK for a given p_MPa and qual_pct */
double if97_pq_s(double p_MPa, double qual_pct);

/** specific volume for a given p_MPa and qual_pct */
double if97_pq_v(double p_MPa, double qual_pct);

/** specific heat capacity #TODO - which one?# for a given p_MPa and qual_pct */
double if97_pq_Cp(double p_MPa, double qual_pct);

/** thermal conductivity for a given p_MPa and qual_pct */
double if97_pq_k(double p_MPa, double qual_pct);


/** dynamic viscosity for a given p_MPa and qual_pct */
double if97_pq_mu(double p_MPa, double qual_pct);

/** speed of sound for a given p_MPa and qual_pct */
double if97_pq_Vs(double p_MPa, double qual_pct);

/** dynamic viscosity for a given p_MPa and qual_pct */
double if97_pq_gamma(double p_MPa, double qual_pct);

/** full steam state for a given p_MPa  and qual_pct */
typSteamState if97_tq_state(double p_MPa, double qual_pct);







#endif // IF97_LIB_H
