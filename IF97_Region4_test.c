//          Copyright Martin Lord 2014-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)



/* *****************************************************************************
* A SHORT PROGRAMME TO CHECK REGION 4 (SATURATION LINE) EQUATIONS 
* *****************************************************************************/


#include "IF97_Region4.h"
#include <stdio.h>
#include "IF97_common.h"
#include "if97_lib_test.h"


int if97_region4_test (FILE *logFile){	
	int intermediateResult= TEST_PASS; //initialise with clear flags.  
	
	fprintf(logFile, "\n\n*** IF97 REGION 4 MODULE CHECK ***\n" );

/* **************************************
 * SATURATION LINE EQUATION:  Psat from T
* see IAPWS-IF97 Table 35
*
* The results should be as follows
* 
* T (K)			Psat (MPa)
* 300			0.353 658 941 E -2
* 500			0.263 889 776 E 1
* 600			0.123 443 146 E 2
*/ 
	
	fprintf(logFile, "*** IF97 REGION 4 (SATURATION LINE) EQUATIONS CHECK Psat(T) ***\n" );
	
	
	intermediateResult = intermediateResult | testSingleInput( if97_r4_ps, 300.0 , 0.353658941e-2 , TEST_ACCURACY, SIG_FIG, "if97_r4_ps", logFile);
	intermediateResult = intermediateResult | testSingleInput( if97_r4_ps, 500.0 , 0.263889776e1 , TEST_ACCURACY, SIG_FIG, "if97_r4_ps", logFile);
	intermediateResult = intermediateResult | testSingleInput( if97_r4_ps, 600.0 , 0.123443146e2 , TEST_ACCURACY, SIG_FIG, "if97_r4_ps", logFile);
	 
 
 
/* ****************************************
 * SATURATION LINE EQUATION:  Psat from T
 * see IAPWS-IF97 Table 36
 *
 * The results should be as follows
 * 
 * P (MPa)	Ts (K)
 * 0.1		0.372 755 919 E 3
 * 1		0.453 035 632 E 3
 * 10		0.584 149 488 E 3
 */
	fprintf(logFile, "\n\n*** IF97 REGION 4 (SATURATION LINE) EQUATIONS CHECK Tsat(P) ***\n" );
	
	intermediateResult = intermediateResult | testSingleInput( if97_r4_ts, 0.1 , 0.372755919E3 , TEST_ACCURACY, SIG_FIG, "if97_r4_ts", logFile);
	intermediateResult = intermediateResult | testSingleInput( if97_r4_ts, 1.0 , 0.453035632E3 , TEST_ACCURACY, SIG_FIG, "if97_r4_ts", logFile);
	intermediateResult = intermediateResult | testSingleInput( if97_r4_ts, 10.0 , 0.584149488E3 , TEST_ACCURACY, SIG_FIG, "if97_r4_ts", logFile);
	
	
	if (intermediateResult != 0)
		intermediateResult= intermediateResult | TEST_FAIL;
	return intermediateResult;
}

