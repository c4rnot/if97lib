
//          Copyright Martin Lord 2014-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


//    IAPWS-IF97 constants used throughout the formulation

//    Methods for finding solutions

 
 /* ********************************************************************
  *         COMPILE AND LINK INSTRUCTIONS    (gcc)                          *
  * 
  * This library uses math.h, so must have the -lm  link flag
  * 
  * The library is programmed to be able to use OpenMP multithreading   
  * use the -fopenmp compile flag to enable multithreadded code
  * 
  * ****************************************************************** */

 
/**
 * @copyright
 * Copyright Martin Lord 2014-2015. \n
 * Distributed under the Boost Software License, Version 1.0. \n
 * (See accompanying file LICENSE_1_0.txt or copy at \n
 * http://www.boost.org/LICENSE_1_0.txt) \n
 * 
 * @file solve.h
 * @author Martin Lord
 * @date 08 June 2015
 * @brief solver intended for property tables
 * @details   So far, there is a Secant Solver implimented
 */


#ifndef SOLVE_H
#define SOLVE_H


#include <math.h> // for pow, log, fabs
#include <stdbool.h>


#include <stdio.h>  //used for debugging only



typedef struct sctSolvResult {
	double dSolution;
	long int lIterations;
	int iErrCode;
} typSolvResult;



/**  Secant method:
 * Use this if Newton Raphson doesn't solve or if the derivatives are not known.
 *  
 * result tolerance is the error from perfect solution as a percentage of the 
 * value to be found.  See chapter 12 of IAPWS-IF97
 * i.e. guess is an acceptable solution when  
 *  result_tol_pct <= ((*func(guess) - Common_in_var)/ Common_in_var) * 100
 * 
 * A guess tolerance is used to make a second guess.  
 * A secant is made between guess
 * and guess + guess_tol and guess - guess_tol.  The best of which is used
 * as guess 2 for the iteration.
 * 
 * Usage example using predefined function h(p,t) to find t(p,h)  
 * p is the common_in_var (it is a variable to both functions)
 * h is the func_result in var 
 * *function should point to h(p,t)  (pointer to the predefined function)
 * 
 * p is the first of the two variables in h(p,t) so isCommon_in_pos1 = true
 * (if it were set to false, the method would try to put pressure in t and
 * guess p, leading to incorrect result.  Result should be sanity checked 
 * at least in testing.
 */
 
typSolvResult  secant_solv ( double (*func)(double, double), double common_in_var, 
							bool isCommon_in_pos1,   double func_seek_result_in_var, double in_guess,  
							double guess_tol_pct, double result_tol_pct,   long int max_iterations);



#endif //SOLVE_H

