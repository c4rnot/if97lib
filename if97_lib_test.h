//       Copyright Martin Lord 2014-2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)



/* *****************************************************************************
* A SHORT PROGRAMME TO CHECK THE LIBRARY
* *******************************************************************************/

#ifndef IF97_LIB_TEST_H
#define IF97_LIB_TEST_H

#include "if97_lib.h"
#include "IF97_common.h"
#include "iapws_surftens.h"
#include "solve_test.h"
#include <stdio.h>
#include <math.h>  // for fabs
#include "winsteam_compatibility.h"

// test a single input function. Pass = 0. See IF97_Common.h for failure codes. 
//Function outputs more detail to logfile if VERBOSE_TEST is true
int testSingleInput ( double (*func) (double), double input, double expectedOutput, double tol, int tolType, char* funcName, FILE *logFile);

// test a double input function. Pass = 0. See IF97_Common.h for failure codes.
//Function outputs more detail to logfile if VERBOSE_TEST is set to true
int testDoubleInput ( double (*func) (double, double), double input1, double input2, double expectedOutput, double tol, int tolType, char* funcName, FILE *logFile);

// test a double input function with boolean output. Pass = 0. See IF97_Common.h for failure codes.
//Function outputs more detail to logfile if VERBOSE_TEST is set to true
int testBoolDoubleInput ( bool (*func) (double, double), double input1, double input2, bool expectedOutput, char* funcName, FILE *logFile);

// test a double input function with boolean output. Pass = 0. See IF97_Common.h for failure codes.
//Function outputs more detail to logfile if VERBOSE_TEST is set to true
int testCharDoubleInput ( char (*func) (double, double), double input1, double input2, char expectedOutput, char* funcName, FILE *logFile);


// prints a unit test summary to the log based on the test code
void resultSummary (char* funcName, FILE *logFile, int testCode);

// unit tests for the functions in the library file itself
int if97_lib_test (FILE *logFile);

#endif // IF97_LIB_TEST_H