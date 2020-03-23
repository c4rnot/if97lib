
//          Copyright Martin Lord 2014-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

// ****SHORT PROGRAM FOR UNIT TESTING *******

// For verifivation the above equations must meet the following T-p point
//
// T= 0.623 150 000 E3 Kelvin
// p= 0.165 291 643 E2 MPa
//

#include "IF97_B23_test.h"
#include "IF97_B23.h"
#include "IF97_common.h"
#include "if97_lib_test.h"


int if97_B23_test (FILE *logFile){	
	int intermediateResult = TEST_PASS;
	
	fprintf(logFile, "\n\n*** IF97 REGION 2-3 BOUNDARY FUNCTION CHECK ***\n\n" );
	
	intermediateResult = intermediateResult & testSingleInput ( IF97_B23P, 0.623150000e3, 1.65291643e01, TEST_ACCURACY, SIG_FIG, "IF97_B23P", logFile);
	intermediateResult = intermediateResult & testSingleInput ( IF97_B23T, 0.165291643e2, 6.23150000e02, TEST_ACCURACY, SIG_FIG, "IF97_B23T", logFile);
	
	if (intermediateResult != 0)
		intermediateResult= intermediateResult | TEST_FAIL;
	
	return intermediateResult;
}
