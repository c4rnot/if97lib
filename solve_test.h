
//          Copyright Martin Lord 2014-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)



/* *****************************************************************************
* A SHORT PROGRAMME TO CHECK SOLVERS 
* *******************************************************************************/

#ifndef SOLVE_TEST_H
#define SOLVE_TEST_H

#include "solve.h"
#include "IF97_common.h"
#include "if97_lib_test.h"
#include "if97_Region3.h"
#include <stdio.h>

/** run the tests on the solver module  **/ 
int solve_test (FILE *logFile);

/** module test routine for checking and displaying solver results  **/
int testSolver (typSolvResult result, double expectedOutput, double tol, int tolType, char* funcName, FILE *logFile);

#endif // SOLVE_TEST_H