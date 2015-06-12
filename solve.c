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
 * result tolerance is the error from perfect solution as a percentage of the 
 * value to be found.  See chapter 12 of IAPWS-IF97
 * i.e. guess is an acceptabel solution when  
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
 
//Checked OK 
typSolvResult  secant_solv ( double (*func)(double, double), double common_in_var, 
							bool isCommon_in_pos1,   double func_seek_result_in_var, double in_guess,  
							double guess_tol_pct, double result_tol_pct,   long int max_iterations){
	
	double lastGuessResult, lastGuess = in_guess ;
	double thisGuessResult, thisGuess = in_guess * (1 + guess_tol_pct/100.0) ;
	double nextGuessResult, nextGuess = in_guess * (1 - guess_tol_pct/100.0);
	double lastErr, thisErr, nextErr; // deviation from goal seek as a ratio of goal seek
	long int i =0;
	int eCode = 0;  // error code.
	typSolvResult solution;

		printf ("fixed pressure p	%.8f\n", common_in_var);			
		
		printf ("in guess t	%.8f\n",lastGuess);	
		printf ("guess 1 t	%.8f\n",thisGuess);	
		printf ("guess 2 t	%.8f\n",nextGuess);	
				
				
	// first, check the initial guess.  If it comes from backwards equations it may well be fine.
	if (isCommon_in_pos1 == true){ 
		lastGuessResult = (*func)(common_in_var, lastGuess);
		lastErr = fabs((lastGuessResult - func_seek_result_in_var));
			printf ("first guess h	%.8f\n",lastGuessResult);
			printf ("first (result-seek)/seek	%.8f\n", fabs((lastGuessResult - func_seek_result_in_var)/func_seek_result_in_var));
		if ( lastErr/func_seek_result_in_var <= result_tol_pct/100.0){
			solution.dSolution = lastGuess;
			solution.lIterations = i;
			solution.iErrCode = eCode;
			return solution ;
		}
	}
	else { 
		lastGuessResult = (*func)(lastGuess,  common_in_var) ;
		lastErr = fabs((lastGuessResult - func_seek_result_in_var));
		if (fabs((lastErr/func_seek_result_in_var)) <= result_tol_pct/100.0){
			solution.dSolution = lastGuess;
			solution.lIterations = i;
			solution.iErrCode = eCode;
			return solution ;
		}
	}	
	
	// try each of the initial guess set
	if (isCommon_in_pos1 == true){
		thisGuessResult = (*func)(common_in_var, thisGuess);
		nextGuessResult = (*func)(common_in_var, nextGuess);
	}
	else {
		thisGuessResult = (*func)(thisGuess,  common_in_var) ;
		nextGuessResult = (*func)(nextGuess,  common_in_var) ;
	}

		printf ("guess 1 result h	%.8f\n",thisGuessResult);	
		printf ("guess 2 result h	%.8f\n",nextGuessResult);	


	// make closest tol the next guess and discard the other
	if (fabs(thisGuessResult - func_seek_result_in_var) > fabs(nextGuessResult - func_seek_result_in_var)) {
		thisGuess = nextGuess;
		thisGuessResult = nextGuessResult ;
	}

		thisErr = fabs((thisGuessResult - func_seek_result_in_var));

		printf ("prelim t	%.8f\n",thisGuess);	
		printf ("prelim h	%.8f\n",thisGuessResult);	
		printf ("prelim error	%.8f\n",thisErr);



	// Iterate to find solution
	for (i=1; i <= max_iterations ; i++) {
		
		// next guess
		// based on http://mathworld.wolfram.com/SecantMethod.html
		nextGuess =  thisGuess - ( (thisErr * ( thisGuess - lastGuess)) 
									/ (thisErr - lastErr)      );
		
		

		
		// try function with guess
		if (isCommon_in_pos1 == true){
			nextGuessResult = (*func)(common_in_var, nextGuess);
			}
		else nextGuessResult = (*func)(nextGuess,  common_in_var) ;
		
		
		printf ("guess t	%.8f\n",nextGuess);	
		printf ("result h	%.8f\n",nextGuessResult);	
		printf ("(result-seek)/seek	%.8f\n", fabs((lastGuessResult - func_seek_result_in_var)/func_seek_result_in_var));


		nextErr = fabs((nextGuessResult - func_seek_result_in_var));

		//  check if the result of the try meets the solution acceptance criteri(on)
		if (nextErr/func_seek_result_in_var <= result_tol_pct/100.0){
				solution.dSolution = nextGuess;
				solution.lIterations = i;
				solution.iErrCode = eCode;
			return solution ;
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

	eCode = 500 ;  // if this executes, we have exceeded the maximum number of iterations without converging
	
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



