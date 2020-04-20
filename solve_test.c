
//          Copyright Martin Lord 2014-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)



/* *****************************************************************************
* A SHORT PROGRAMME TO CHECK SOLVERS 
* *******************************************************************************/

#include "solve_test.h"
#include "solve.h"
#include "IF97_common.h"
#include "if97_lib_test.h"
#include "if97_Region3.h"
#include <stdio.h>

int solve_test (FILE *logFile){	
	int intermediateResult;
	typSolvResult thisSolution;
	
	fprintf(logFile, "\n\n*** SOLVERS CHECK ***\n" );
	fprintf(logFile, "*** Testing secant solver   *** \n\n" );


    //if97_r3w_v_pt ( 22.15, 647.5 ) = 0.0036940323
	fprintf ( logFile, "if97_r3_p (1/0.003694034494534 ,  647.5) = %.10g to match backwards check if97_r3w_v_pt ( 22.15, 647.5 ) = 0.0036940323 \n", if97_r3_p (1/0.003694034494534 ,  647.5) );
	thisSolution = secant_solv(if97_r3_p, 647.5, false,  22.150000, 1/0.0036940323, 0.05, TEST_ACCURACY, SIG_FIG, 10 );
	intermediateResult = intermediateResult | testSolver ( thisSolution, 1/0.003694034494534, TEST_ACCURACY, SIG_FIG, "if97_r3_p(x,647.5)", logFile);
	
	thisSolution = secant_solv(if97_r3_p, 647.5, false,  22.150000, 1/0.003, 0.05, TEST_ACCURACY, SIG_FIG, 100 );
	intermediateResult = intermediateResult | testSolver ( thisSolution, 1/0.003694034494534, TEST_ACCURACY, SIG_FIG, "if97_r3_p(x,647.5)", logFile);

	thisSolution = secant_solv(if97_r3_p, 647.5, false,  22.150000, 1/0.003, 5.0, TEST_ACCURACY, SIG_FIG, 100 );
	intermediateResult = intermediateResult | testSolver ( thisSolution, 1/0.003694034494534, TEST_ACCURACY, SIG_FIG, "if97_r3_p(x,647.5)", logFile);


	if (intermediateResult != 0)
		intermediateResult= intermediateResult | TEST_FAIL;
	return intermediateResult;

}


	
	/* try to solve p(h, t) using secant solver.  one from table 18.
	 * 
	 * @ 450 K 1.5 MPa, enthalpy should be 2.72134539e03 kJ/kg
	 
	fprintf ( logFile, "\n\n *** Testing secant solver   *** \n\n" );	
	//intermediateResult = intermediateResult | testDoubleInput (if97_pt_h, 3.0, 300.0, 1.15331273e02, TEST_ACCURACY, SIG_FIG, "if97_pt_h", logFile);
	 
	 
	 
	 /**
	fprintf ( pTestLog, "\n\n *** Testing secant solver  ****** \n\n" );
	fprintf ( pTestLog, "The output should be the following: \n\n");
	fprintf ( pTestLog, "p (MPa)h (Kj/kg)	T (K)	\n");
	fprintf ( pTestLog, "1.5	2.72134539e03	450.0\n");		
	fprintf ( pTestLog, "\n\n**********RESULTS******\n");	  
	fprintf ( pTestLog, "p (MPa)	h (Kj/kg)	T (K)\n");	
	
	fprintf ( pTestLog, "first result should be 	%.8f\n", if97_pt_h(30.0,650));	
	
	typSolvResult solution =  secant_solv(&if97_pt_h, 30.0, true,  2.63149474e03, 710.0, 0.05, 0.000000001, 20 );
	fprintf ( pTestLog, "1.5	2.72134539e03	%.8f\n", solution.dSolution);
	fprintf ( pTestLog, "iterations	%li\n", solution.lIterations);		
	fprintf ( pTestLog, "error code	%i\n", solution.iErrCode);		
	 
		fprintf  ("\n\n\n");
	 **/
	
	
	
	
// unlike the other test methods, the solver should be called separately (they have various input requirements). This routeine checks the result of an already calculated solution.
// put the values into funcname
int testSolver (typSolvResult result, double expectedOutput, double tol, int tolType, char* funcName, FILE *logFile){
	int testResult = TEST_FAIL; // initialize as a fail.
	
	double error;
	
	switch (tolType) {
		case SIG_FIG:
			error = fabs((result.dSolution - expectedOutput)/expectedOutput);
			if ( error <= pow(10,-tol ) && (result.iErrCode == SOLVE_CONVERGE) ){
					testResult = TEST_PASS;
					if (VERBOSE_TEST)
						fprintf ( logFile, "%s  = %.10g \t Expected: %.10g \tError ratio: %e \tIterations %i  \t PASS\n", funcName, result.dSolution, expectedOutput, error, result.lIterations );
				}
			else {
				testResult = testResult | TEST_INCORRECT;
				if (VERBOSE_TEST)
					fprintf ( logFile, "%s  = %.10g \t Expected: %.10g \tError ratio: %e \tIterations %i  \t FAIL\n", funcName, result.dSolution, expectedOutput, error, result.lIterations );
			}
			break;
		case ABS:
			error = fabs(result.dSolution - expectedOutput);
			if ( error <= tol && (result.iErrCode == SOLVE_CONVERGE)){
					testResult = TEST_PASS;
					if (VERBOSE_TEST)
						fprintf ( logFile, "%s  = %.10g \t Expected: %.10g \tError: %e \tIterations %i  \t PASS\n", funcName, result.dSolution, expectedOutput, error, result.lIterations );
				}
			else {
				testResult = testResult | TEST_INCORRECT;
				if (VERBOSE_TEST)
					fprintf ( logFile, "%s  = %.10g \t Expected: %.10g \tError: %e \tIterations %i  \t FAIL\n", funcName, result.dSolution, expectedOutput, error, result.lIterations );
			}		
			break;
		
		case PERCENT:
			error = fabs((result.dSolution - expectedOutput)/expectedOutput)*100;
			if ( (error <= tol) && (result.iErrCode == SOLVE_CONVERGE) ){
					testResult = TEST_PASS;
					if (VERBOSE_TEST)
						fprintf ( logFile, "%s  = %.10g \t Expected: %.10g \tError: %e % \tIterations %i  \t PASS\n", funcName, result.dSolution, expectedOutput, error, result.lIterations );
				}
			else {
				testResult = testResult | TEST_INCORRECT;
				if (VERBOSE_TEST)
					fprintf ( logFile, "%s  = %.10g \t Expected: %.10g \tError: %e % \tIterations %i  \t FAIL\n", funcName, result.dSolution, expectedOutput, error, result.lIterations );
			}		
			break;
	}
	
	return testResult;
}