
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



// bitwise test error codes. //
#define SOLVE_CONVERGE 0   //Solver reports solution found
#define SOLVE_NO_CONVERGE 8 // A solution did not converge within the maximum allowed number of iterations


typedef struct sctSolvResult {
	double dSolution;
	long int lIterations;
	int iErrCode;
} typSolvResult;


enum SlvTolType{
	SLV_SIG_FIG,
	SLV_ABS,
	SLV_PERCENT,
};


/**  Secant method for two variable PDE:
 * Use this if Newton Raphson doesn't solve or if the derivatives are not known.
 *  
 * A guess tolerance is used to make a second guess.  
 * A secant is made between guess
 * and guess + guess_tol and guess - guess_tol.  The best of which is used
 * as guess 2 for the iteration.
 * 
 * Usage example using predefined function h(p,t) to find t(p,h)  
 * p is the funcConst (it is a variable to both functions)
 * h is the func_result in var 
 * *function should point to h(p,t)  (pointer to the predefined function)
 * 
 * p is the first of the two variables in h(p,t) so isFuncConstPos1 = true
 * (if it were set to false, the method would try to put pressure in t and
 * guess p, leading to incorrect result.  Result should be sanity checked 
 * at least in testing.
 */
 
typSolvResult  secant_solv ( double (*func)(double, double),  // pointer to the function. one variable is known (held constant), the other is sought.
							double funcConst, //  the function variable which is held constant
							bool isFuncConstPos1,  // is input held constant the first of the two function variables? (false if it is the second)
							double seek_result,  // the (known) result of the function when the sought variable is correct.
							double in_guess,  // initial guess of the sought variable
							double guess_tol_pct, //  
							double solutionTol, //
							int solutionTolType, //	SIG_FIG, ABS, PERCENT, - in this case defined in IF97_common.h
							long int max_iterations);   // maximum number of iterations to attempt before declaring an error



#endif //SOLVE_H

