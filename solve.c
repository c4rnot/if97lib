//          Copyright Martin Lord 2014-2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


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
   


#include <math.h> // for pow, log, fabs
#include <stdbool.h>

#include "solve.h"

#include <stdio.h>  //used for debugging only

/*  Assuming Secant method most securely finds roots.
 * Use this if Newton Raphson doesn't solve or if the derivatives are not known.
 *  
 * result tolerance is the error from perfect solution as a SLV_PERCENTage of the 
 * value to be found.  See chapter 12 of IAPWS-IF97
 * i.e. guess is an acceptabel solution when  
 *  result_tol_pct <= ((*func(guess) - funcConst)/ funcConst) * 100
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
							int solutionTolType, //	SLV_SIG_FIG, SLV_ABS, SLV_PERCENT, - in this case defined in IF97_common.h
							long int max_iterations){   // maximum number of iterations to attempt before declaring an error
	
	double lastGuessResult, thisGuessResult, nextGuessResult;
	double lastGuess = in_guess ;
	double thisGuess = in_guess * (1 + guess_tol_pct/100.0) ;
	double nextGuess = in_guess * (1 - guess_tol_pct/100.0);
	double lastErr, thisErr, nextErr; // deviation from goal seek as a ratio of goal seek
	long int i =0;
	int eCode = 0;  // error code.  Initialised with no error
	typSolvResult solution;
	
	// first, check the initial guess.  Maybe it's right first time.
	if (isFuncConstPos1 == true) lastGuessResult = (*func)(funcConst, lastGuess);
	else                         lastGuessResult = (*func)(lastGuess, funcConst) ;
		
	lastErr = fabs(lastGuessResult - seek_result);
	
	switch (solutionTolType){
		case SLV_PERCENT:		
			if (fabs(lastErr/seek_result) <= solutionTol/100.0){
				solution.dSolution = lastGuess;
				solution.lIterations = i;
				solution.iErrCode = eCode;
				return solution ;
			}
			break;
		case SLV_ABS:
			if (lastErr <= solutionTol){
				solution.dSolution = lastGuess;
				solution.lIterations = i;
				solution.iErrCode = eCode;
				return solution ;
			}
			break;			
		case SLV_SIG_FIG:
			if (lastErr <= pow(10,-solutionTol )){
				solution.dSolution = lastGuess;
				solution.lIterations = i;
				solution.iErrCode = eCode;
				return solution ;
			}
			break;		
	}
	
	
	// Not right first time, so calculate the guesses either side of the initial (last) guess
	if (isFuncConstPos1 == true){
		thisGuessResult = (*func)(funcConst, thisGuess);
		nextGuessResult = (*func)(funcConst, nextGuess);
	}
	else {
		thisGuessResult = (*func)(thisGuess,  funcConst) ;
		nextGuessResult = (*func)(nextGuess,  funcConst) ;
	}

	// make closest tol the next guess and discard the other
	if (fabs(thisGuessResult - seek_result) > fabs(nextGuessResult - seek_result)) {
		thisGuess = nextGuess;
		thisGuessResult = nextGuessResult ;
	}

		thisErr = fabs((thisGuessResult - seek_result));
		// now we have a lastGuess, lastGuessResult and lastErr based on the initial guess
		// and a thisGuess, thisGuessResult and thisErr based on the best of the two guesses either side of the initial guess.


	// Iterate to find solution
	for (i=1; i <= max_iterations ; i++) {
		
		// next guess
		// based on http://mathworld.wolfram.com/SecantMethod.html
		nextGuess =  thisGuess - ( (thisErr * ( thisGuess - lastGuess)) 
									/ (thisErr - lastErr)      );
				
		// try function with new next guess
		if (isFuncConstPos1 == true) nextGuessResult = (*func)(funcConst, nextGuess);
		else                         nextGuessResult = (*func)(nextGuess, funcConst) ;

		nextErr = fabs((nextGuessResult - seek_result));

		//  check if the result of the try meets the solution acceptance criterion
		switch (solutionTolType){
			case SLV_PERCENT:		
				if (fabs(nextErr/seek_result) <= solutionTol/100.0){
					solution.dSolution = nextGuess;
					solution.lIterations = i;
					solution.iErrCode = eCode;
					return solution ;
				}
				break;
			case SLV_ABS:
				if (nextErr <= solutionTol){
					solution.dSolution = nextGuess;
					solution.lIterations = i;
					solution.iErrCode = eCode;
					return solution ;
				}
				break;			
			case SLV_SIG_FIG:
				if (nextErr <= pow(10,-solutionTol )){
					solution.dSolution = nextGuess;
					solution.lIterations = i;
					solution.iErrCode = eCode;
					return solution ;
				}
				break;		
		}
	
/* if we reach here on this iteration, we do not have a solution so
 * 	move guesses and results back a place ready for the next iteration */

		lastGuess = thisGuess;
		lastGuessResult = thisGuessResult ;
		lastErr = thisErr;
		
		thisGuess = nextGuess;
		thisGuessResult = nextGuessResult ;
		thisErr = nextErr;
	} // end of main loop

	eCode = SOLVE_NO_CONVERGE;  // if this executes, we have exceeded the maximum number of iterations without converging
	
	solution.dSolution = nextGuess;
	solution.lIterations = i;
	solution.iErrCode = eCode;
return solution ;
}










/*  Use Newton Raphson if the first derivatives are know and are known not
 * to have discontinuities.  Do NOT use it near discontinuities. 
 * i.e. not v(p,t) near critical point.  not straddling the saturation 
 * line on the P, T plane   
*/



