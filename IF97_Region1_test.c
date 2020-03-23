
//          Copyright Martin Lord 2014-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)



/* *****************************************************************************
* A SHORT PROGRAMME TO CHECK REGION 1 PROPERTY EQUATIONS AND 1 BACKWARDS EQUATIONS
* *******************************************************************************/


#include "IF97_Region1.h"
#include "IF97_common.h"
#include "if97_lib_test.h"



int if97_region1_test (FILE *logFile){	
	int intermediateResult;
	
	fprintf(logFile, "\n\n*** IF97 REGION 1 MODULE CHECK ***\n" );
	
	fprintf(logFile, "*** IF97 REGION 1 PROPERTY (FORWARDS) EQUATIONS CHECK ***\n\n" );
	
/* PROPERY (FORWARDS) EQUATIONS
* see IAPWS-IF97 Table 5
*
* The results should be as follows
*
*				T= 300 K				T= 300 K				T= 500 K
*				p= 3 MPa				p= 80 MPa				p= 3 MPa
* g  (kJ/kg)	-2.35716470e+00 		7.35736720e+01		-3.14667321e+02 	Using Freesteam
* v  (m3/kg) 	1.00215168e-3			9.71180894e-4			1.20241800e-3
* h  (kJ/kg)	1.15331273e2			1.84142828e2			9.75542239e2
* u  (kJ/kg)	1.12324818e2			1.06448356e2			9.71934985e2
* s  (kJ/kg.K)	3.92294792e-1			3.68563852e-1			2.58041912e0
* Cp (kJ/kg.K)	4.17301218e0			4.01008987e0			4.65580682e0
* Cv (kJ/kg.K)	4.12120160e0			3.91736606e0			3.22139223e0	From Moscow Power Institute check page
* w  (m/s)		1.50773921e3			1.63469054e3			1.24071337e3

* see http://twt.mpei.ac.ru/mcs/worksheets/iapws/IAPWS-IF97-Region1.xmcd
*/

	intermediateResult = TEST_PASS; //clear flags.  
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r1_g, 3.0, 300.0, -2.35716470e+00, TEST_ACCURACY, SIG_FIG, "if97_r1_g", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r1_g, 80.0, 300.0, 7.35736720e+01, TEST_ACCURACY, SIG_FIG, "if97_r1_g", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r1_g, 3.0, 500.0, -3.14667321e+02, TEST_ACCURACY, SIG_FIG, "if97_r1_g", logFile);

	intermediateResult = intermediateResult | testDoubleInput ( if97_r1_v, 3.0, 300.0, 1.00215168e-3, TEST_ACCURACY, SIG_FIG, "if97_r1_v", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r1_v, 80.0, 300.0, 9.71180894e-4, TEST_ACCURACY, SIG_FIG, "if97_r1_v", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r1_v, 3.0, 500.0, 1.20241800e-3, TEST_ACCURACY, SIG_FIG, "if97_r1_v", logFile);

	intermediateResult = intermediateResult | testDoubleInput ( if97_r1_h, 3.0, 300.0, 1.15331273e2, TEST_ACCURACY, SIG_FIG, "if97_r1_h", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r1_h, 80.0, 300.0, 1.84142828e2, TEST_ACCURACY, SIG_FIG, "if97_r1_h", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r1_h, 3.0, 500.0, 9.75542239e2, TEST_ACCURACY, SIG_FIG, "if97_r1_h", logFile);	
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r1_u, 3.0, 300.0, 1.12324818e2, TEST_ACCURACY, SIG_FIG, "if97_r1_u", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r1_u, 80.0, 300.0, 1.06448356e2, TEST_ACCURACY, SIG_FIG, "if97_r1_u", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r1_u, 3.0, 500.0, 9.71934985e2, TEST_ACCURACY, SIG_FIG, "if97_r1_u", logFile);		

	intermediateResult = intermediateResult | testDoubleInput ( if97_r1_s, 3.0, 300.0, 3.92294792e-1, TEST_ACCURACY, SIG_FIG, "if97_r1_s", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r1_s, 80.0, 300.0, 3.68563852e-1, TEST_ACCURACY, SIG_FIG, "if97_r1_s", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r1_s, 3.0, 500.0, 2.58041912e0, TEST_ACCURACY, SIG_FIG, "if97_r1_s", logFile);	

	intermediateResult = intermediateResult | testDoubleInput ( if97_r1_Cp, 3.0, 300.0, 4.17301218e0, TEST_ACCURACY, SIG_FIG, "if97_r1_Cp", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r1_Cp, 80.0, 300.0, 4.01008987e0, TEST_ACCURACY, SIG_FIG, "if97_r1_Cp", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r1_Cp, 3.0, 500.0, 4.65580682e0, TEST_ACCURACY, SIG_FIG, "if97_r1_Cp", logFile);	

	intermediateResult = intermediateResult | testDoubleInput ( if97_r1_Cv, 3.0, 300.0, 4.12120160e0, TEST_ACCURACY, SIG_FIG, "if97_r1_Cv", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r1_Cv, 80.0, 300.0, 3.91736606e0, TEST_ACCURACY, SIG_FIG, "if97_r1_Cv", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r1_Cv, 3.0, 500.0, 3.22139223e0, TEST_ACCURACY, SIG_FIG, "if97_r1_Cv", logFile);	

	intermediateResult = intermediateResult | testDoubleInput ( if97_r1_w, 3.0, 300.0, 1.50773921e3, TEST_ACCURACY, SIG_FIG, "if97_r1_w", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r1_w, 80.0, 300.0, 1.63469054e3, TEST_ACCURACY, SIG_FIG, "if97_r1_w", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r1_w, 3.0, 500.0, 1.24071337e3, TEST_ACCURACY, SIG_FIG, "if97_r1_w", logFile);		



/* *****************************************************************************
*  BACKWARDS EQUATIONS: T(p,h)
* see IAPWS-IF97 Table 7
*
* The results should be as follows
*
* p (MPa)	h (kj/kg)	T (K)
*  3		500			391.798509
* 80		500			378.108626
* 80		1500		611.041229
*/
	fprintf(logFile, "\n\n*** IF97 REGION 1 PROPERTY BACKWARDS EQUATIONS: T(p,h) CHECK ***\n\n" );
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r1_t_ph, 3.0, 500.0, 391.798509, TEST_ACCURACY, SIG_FIG, "if97_r1_t_ph", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r1_t_ph, 80.0, 500.0, 378.108626, TEST_ACCURACY, SIG_FIG, "if97_r1_t_ph", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r1_t_ph, 80.0, 1500.0, 611.041229, TEST_ACCURACY, SIG_FIG, "if97_r1_t_ph", logFile);

/* *****************************************************************************
*  BACKWARDS EQUATIONS: T(p,s)
* see IAPWS-IF97 Table 9
*
* The results should be as follows
*
*  p (MPa)	s (kj/kg.K)	T (K)
*  3		0.5			307.842258
* 80		0.5			309.979785
* 80		3			565.899909
*/	
	fprintf(logFile, "\n\n*** IF97 REGION 1 PROPERTY BACKWARDS EQUATIONS: T (p, s) CHECK ***\n\n" );

	intermediateResult = intermediateResult | testDoubleInput ( if97_r1_t_ps, 3.0, 0.5, 307.842258, TEST_ACCURACY, SIG_FIG, "if97_r1_t_ps", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r1_t_ps, 80.0, 0.5, 309.979785, TEST_ACCURACY, SIG_FIG, "if97_r1_t_ps", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r1_t_ps, 80.0, 3.0, 565.899909, TEST_ACCURACY, SIG_FIG, "if97_r1_t_ps", logFile);
	
	
	if (intermediateResult != 0)
		intermediateResult= intermediateResult | TEST_FAIL;
	return intermediateResult;

}
