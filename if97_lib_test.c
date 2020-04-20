//       Copyright Martin Lord 2014-2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)



/* *****************************************************************************
* A SHORT PROGRAMME TO CHECK THE LIBRARY
* *******************************************************************************/

#include "if97_lib_test.h"
#include "if97_lib.h"
#include "IF97_common.h"
#include "iapws_surftens.h"
#include "solve_test.h"
#include "IF97_Region1_test.h"
#include "IF97_Region2_test.h"
#include "IF97_Region3_test.h"
#include "IF97_Region4_test.h"
#include "IF97_B23_test.h"
#include <stdio.h>
#include <math.h>  // for fabs
#include "winsteam_compatibility.h"

int main (int argc, char **argv)
{

int intermediateResult = TEST_FAIL;

FILE *pTestLog;  // create pointer to a file.  the FILE type is defined in <stdio.h>

pTestLog = fopen(TESTLOGLOC, "w"); // open an empty file to write test results to. pTestLog is a pointer to it


// check threading using OpenMP test
	fprintf ( pTestLog, "\n\n************************************************************ \n" );
	fprintf ( pTestLog, "*** CHECK FOR MULTITHREADDING ***\n\n" );
#pragma omp parallel
	{
	fprintf ( pTestLog, "thread\n");
	}
	
	// *** test region 1 module ***
	intermediateResult = 	if97_region1_test (pTestLog);
	resultSummary ("IF97_Region1 module", pTestLog, intermediateResult);

	// *** test region 2 module ***
	intermediateResult = 	if97_region2_test (pTestLog);
	resultSummary ("IF97_Region2 module", pTestLog, intermediateResult);


	// *** test region 2-3 boundary line equation ***
	intermediateResult = if97_B23_test (pTestLog);
	resultSummary ("IF97_B23 module", pTestLog, intermediateResult);
	
	
	// *** test region 3 module ***
	intermediateResult = 	if97_region3_test (pTestLog);
	resultSummary ("IF97_Region3 module", pTestLog, intermediateResult);	


	// *** test region 4 module ***
	intermediateResult = 	if97_region4_test (pTestLog);
	resultSummary ("IF97_Region4 module", pTestLog, intermediateResult);	


	// *** test region 5 module ***
	intermediateResult = 	if97_region5_test (pTestLog);
	resultSummary ("IF97_Region5 module", pTestLog, intermediateResult);	
	
	//****  test the solver *****
	//intermediateResult = 	solve_test (pTestLog);
	//resultSummary ("solver module", pTestLog, intermediateResult);	
	
	
	// *** Testing iapws_surftens  ******
	intermediateResult = 	if97_surftens_test (pTestLog);
	resultSummary ("iapws_surftens", pTestLog, intermediateResult);	
		
	
	// *** test library module ***
	intermediateResult = 	if97_lib_test (pTestLog);
	resultSummary ("IF97_lib library", pTestLog, intermediateResult);	
	
	
	
		
	

	fclose (pTestLog);  //close the test log file.  note result = EOF on error
	printf ("Test log results can be found in "); printf( TESTLOGLOC );
	
return 0;
}

// test a double input function. Pass = 0. See IF97_Common.h for failure codes.
//Function outputs more detail to logfile if VERBOSE_TEST is set to true
int testSingleInput ( double (*func) (double), double input, double expectedOutput, double tol, int tolType, char* funcName, FILE *logFile)
{
	int testResult = TEST_FAIL;  // initialize as a fail. 
	double error;
	
	switch (tolType) {
		case SIG_FIG:
			error = fabs(((*func)(input) - expectedOutput)/expectedOutput);
			if ( error <= pow(10,-tol )){
					testResult = TEST_PASS;
					if (VERBOSE_TEST)
						fprintf ( logFile, "%s ( %.8g) = %.8g \t Expected: %.8g \t Error ratio : %e \t PASS\n", funcName, input,  (*func)(input), expectedOutput, error );
				}
			else {
				testResult = testResult | TEST_INCORRECT;
				if (VERBOSE_TEST)
					fprintf ( logFile, "%s ( %.8g ) = %.8g \t Expected: %.8g \t Error ratio : %e \t FAIL\n", funcName, input, (*func)(input), expectedOutput, error );
			}
			break;
		case ABS:
			error = fabs((*func)(input) - expectedOutput);
			if ( error <= tol ){
					testResult = TEST_PASS;
					if (VERBOSE_TEST)
						fprintf ( logFile, "%s ( %.8g ) = %.8g \t Expected: %.8g \t Error : %e \t PASS\n", funcName, input, (*func)(input), expectedOutput, error );
				}
			else {
				testResult = testResult | TEST_INCORRECT;
				if (VERBOSE_TEST)
					fprintf ( logFile, "%s ( %.8g ) = %.8g \t Expected: %.8g \t Error : %e \t FAIL\n", funcName, input,  (*func)(input), expectedOutput, error );
			}		
			break;
		case PERCENT:
			error = fabs(((*func)(input) - expectedOutput)/expectedOutput)*100;
			if ( error <= tol ){
					testResult = TEST_PASS;
					if (VERBOSE_TEST)
						fprintf ( logFile, "%s ( %.8g ) = %.8g \t Expected: %.4g \t Error : %e % \t PASS\n", funcName, input, (*func)(input), expectedOutput, error );
				}
			else {
				testResult = testResult | TEST_INCORRECT;
				if (VERBOSE_TEST)
					fprintf ( logFile, "%s ( %.8g ) = %.8g \t Expected: %.4g \t Error : %e % \t FAIL\n", funcName, input, (*func)(input), expectedOutput, error );
			}		
			break;
	}
	
	
	return testResult;
}


// test a double input function. Pass = 0. See IF97_Common.h for failure codes. 
//Function outputs more detail to logfile if VERBOSE_TEST is true
int testDoubleInput ( double (*func) (double, double), double input1, double input2, double expectedOutput, double tol, int tolType, char* funcName, FILE *logFile)
{
	int testResult = TEST_FAIL; // initialize as a fail.
	
	double error;
	
	switch (tolType) {
		case SIG_FIG:
			error = fabs(((*func)(input1, input2) - expectedOutput)/expectedOutput);
			if ( error <= pow(10,-tol )){
					testResult = TEST_PASS;
					if (VERBOSE_TEST)
						fprintf ( logFile, "%s ( %.8g, %.8g ) = %.8g \t Expected: %.8g \t Error ratio : %e \t PASS\n", funcName, input1, input2, (*func)(input1, input2), expectedOutput, error );
				}
			else {
				testResult = testResult | TEST_INCORRECT;
				if (VERBOSE_TEST)
					fprintf ( logFile, "%s ( %.8g, %.8g ) = %.8g \t Expected: %.8g \t Error ratio : %e \t FAIL\n", funcName, input1, input2, (*func)(input1, input2), expectedOutput, error );
			}
			break;
		case ABS:
			error = fabs((*func)(input1, input2) - expectedOutput);
			if ( error <= tol ){
					testResult = TEST_PASS;
					if (VERBOSE_TEST)
						fprintf ( logFile, "%s ( %.8g, %.8g ) = %.8g \t Expected: %.8g \t Error : %e \t PASS\n", funcName, input1, input2, (*func)(input1, input2), expectedOutput, error );
				}
			else {
				testResult = testResult | TEST_INCORRECT;
				if (VERBOSE_TEST)
					fprintf ( logFile, "%s ( %.8g, %.8g ) = %.8g \t Expected: %.8g \t Error : %e \t FAIL\n", funcName, input1, input2, (*func)(input1, input2), expectedOutput, error );
			}		
			break;
		case PERCENT:
			error = fabs(((*func)(input1, input2) - expectedOutput)/expectedOutput)*100;
			if ( error <= tol ){
					testResult = TEST_PASS;
					if (VERBOSE_TEST)
						fprintf ( logFile, "%s ( %.8g, %.8g ) = %.8g \t Expected: %.4g \t Error : %e % \t PASS\n", funcName, input1, input2, (*func)(input1, input2), expectedOutput, error );
				}
			else {
				testResult = testResult | TEST_INCORRECT;
				if (VERBOSE_TEST)
					fprintf ( logFile, "%s ( %.8g, %.8g ) = %.8g \t Expected: %.4g \t Error : %e % \t FAIL\n", funcName, input1, input2, (*func)(input1, input2), expectedOutput, error );
			}		
			break;
	}
	
	
	return testResult;
}


// test a double input function with boolean output. Pass = 0. See IF97_Common.h for failure codes.
//Function outputs more detail to logfile if VERBOSE_TEST is set to true
int testBoolDoubleInput ( bool (*func) (double, double), double input1, double input2, bool expectedOutput, char* funcName, FILE *logFile){
	int testResult = TEST_FAIL; // initialize as a fail.
	if ((*func)(input1, input2) == expectedOutput){
		testResult = TEST_PASS;
		if (VERBOSE_TEST)
			fprintf ( logFile, "%s ( %.8g, %.8g ) = %s \t Expected: %s \t  \t PASS\n", funcName, input1, input2, (*func)(input1, input2) ? "true" : "false", expectedOutput ? "true" : "false");
	}
	else{
		testResult = testResult | TEST_INCORRECT;
		if (VERBOSE_TEST)
			fprintf ( logFile, "%s ( %.8g, %.8g ) = %s \t Expected: %s \t  \t FAIL\n", funcName, input1, input2, (*func)(input1, input2) ? "true" : "false", expectedOutput ? "true" : "false");
	}
	
	return testResult;
}		
	


// test a double input function with boolean output. Pass = 0. See IF97_Common.h for failure codes.
//Function outputs more detail to logfile if VERBOSE_TEST is set to true
int testCharDoubleInput ( char (*func) (double, double), double input1, double input2, char expectedOutput, char* funcName, FILE *logFile){
	int testResult = TEST_FAIL; // initialize as a fail.
	if ((*func)(input1, input2) == expectedOutput){
		testResult = TEST_PASS;
		if (VERBOSE_TEST)
			fprintf ( logFile, "%s ( %.8g, %.8g ) = %c \t Expected: %c \t  \t PASS\n", funcName, input1, input2, (*func)(input1, input2), expectedOutput);
	}
	else{
		testResult = testResult | TEST_INCORRECT;
		if (VERBOSE_TEST)
			fprintf ( logFile, "%s ( %.8g, %.8g ) = %c \t Expected: %c \t  \t FAIL\n", funcName, input1, input2, (*func)(input1, input2), expectedOutput);
	}
	
	return testResult;
}	
	

// prints a unit test summary to the log based on the test code
void resultSummary (char* funcName, FILE *logFile, int testCode)
{
	fprintf (logFile, "\nResults summary for %s : Error code %i \n", funcName, testCode);
	
	if (testCode == TEST_PASS)
		fprintf(logFile, "PASS");
	else
	{
		if (testCode & TEST_FAIL)
			fprintf(logFile, "FAIL\n");
		if (testCode & TEST_INCORRECT)
			fprintf(logFile, "Some or all elements generate incorrect results\n");
		if (testCode & TEST_INCOMPLETE)
			fprintf(logFile, "Some or all elements are not yet complete\n");
	}
	fprintf (logFile, "\n\n");
}



	// *** Testing iapws_surftens  ******
int if97_surftens_test (FILE *logFile){
	const double If97_surf_acc = 0.07;  // Highest difference in table 1 of IAPWS R1-76(2014)
	int intermediateResult = TEST_PASS;
	
	fprintf ( logFile, "\n\n *** Testing  iapws_surftens  *** \n\n" );	
	intermediateResult = intermediateResult | testSingleInput (iapws_surftens, 0.01, 75.65, If97_surf_acc, ABS, "iapws_surftens", logFile);
	intermediateResult = intermediateResult | testSingleInput (iapws_surftens, 20.0, 72.74, If97_surf_acc,  ABS, "iapws_surftens", logFile);
	intermediateResult = intermediateResult | testSingleInput (iapws_surftens, 100.0, 58.91, If97_surf_acc, ABS,  "iapws_surftens", logFile);
	intermediateResult = intermediateResult | testSingleInput (iapws_surftens, 200.0, 37.67, If97_surf_acc, ABS, "iapws_surftens", logFile);
	intermediateResult = intermediateResult | testSingleInput (iapws_surftens, 300.0, 14.36, If97_surf_acc, ABS, "iapws_surftens", logFile);
	intermediateResult = intermediateResult | testSingleInput (iapws_surftens, 370.0, 0.39, If97_surf_acc, ABS, "iapws_surftens", logFile);
	intermediateResult = intermediateResult | testSingleInput (iapws_surftens, IF97_TC - IF97_T_TRIP, 0.0, If97_surf_acc, ABS, "iapws_surftens", logFile);
	
	if (intermediateResult != 0)
		intermediateResult= intermediateResult | TEST_FAIL;
	return intermediateResult;
}


int if97_lib_test (FILE *logFile){	
	int intermediateResult;
	
	
		// *** Testing  if97_pt_h  ******
	intermediateResult = TEST_PASS;
	fprintf ( logFile, "\n\n *** Testing  if97_pt_h  *** \n\n" );	
	
	intermediateResult = intermediateResult | testDoubleInput (if97_pt_h, 3.0, 300.0, 1.15331273e02, TEST_ACCURACY, SIG_FIG, "if97_pt_h", logFile);
	intermediateResult = intermediateResult | testDoubleInput (if97_pt_h, 80.0, 300.0, 1.84142828e02, TEST_ACCURACY, SIG_FIG,"if97_pt_h", logFile);
	fprintf ( logFile, "\n1/if97_R3bw_v_pt(%.9g, %.9g) = %.9g", IF97_PC + 0.000001, IF97_TC + 0.000001, 1/if97_R3bw_v_pt(IF97_PC + 0.000001, IF97_TC + 0.000001));
	fprintf ( logFile, "\n1/if97_pt_v(%.9g, %.9g) = %.9g\n", IF97_PC + 0.000001, IF97_TC + 0.000001, 1/if97_pt_v(IF97_PC + 0.000001, IF97_TC + 0.000001));
	intermediateResult = intermediateResult | testDoubleInput (if97_pt_v, IF97_PC + 0.000001, IF97_TC + 0.000001, 1/324.23752306, TEST_ACCURACY,  SIG_FIG,"if97_pt_v", logFile);
	intermediateResult = intermediateResult | testDoubleInput (if97_pt_h, IF97_PC + 0.000001, IF97_TC + 0.000001, 2083.817978619541, TEST_ACCURACY,  SIG_FIG,"if97_pt_h", logFile);
	intermediateResult = intermediateResult | testDoubleInput (if97_pt_h, 30.0, 1500.0, 5.16723514e03,TEST_ACCURACY, SIG_FIG,"if97_pt_h", logFile);
	
	intermediateResult = intermediateResult | testDoubleInput (if97_pt_u, IF97_PC + 0.000001, IF97_TC + 0.000001, 2015.769096450988, TEST_ACCURACY,  SIG_FIG,"if97_pt_u", logFile);
	intermediateResult = intermediateResult | testDoubleInput (if97_pt_s, IF97_PC + 0.000001, IF97_TC + 0.000001, 4.406259014859, TEST_ACCURACY,  SIG_FIG,"if97_pt_s", logFile);
	intermediateResult = intermediateResult | testDoubleInput (if97_pt_Cv, IF97_PC + 0.000001, IF97_TC + 0.000001, 4.527598110108, TEST_ACCURACY,  SIG_FIG,"if97_pt_Cv", logFile);
	intermediateResult = intermediateResult | testDoubleInput (if97_pt_Cp, IF97_PC + 0.000001, IF97_TC + 0.000001, 4.54022408404e5, TEST_ACCURACY,  SIG_FIG,"if97_pt_Cp", logFile);
	intermediateResult = intermediateResult | testDoubleInput (if97_pt_Vs, IF97_PC + 0.000001, IF97_TC + 0.000001, 314.252078309417, TEST_ACCURACY,  SIG_FIG,"if97_pt_Vs", logFile);
	
	
	
	resultSummary ("if97_pt_h", logFile, intermediateResult);
	
	
	if (intermediateResult != 0)
		intermediateResult= intermediateResult | TEST_FAIL;
	return intermediateResult;

}