//          Copyright Martin Lord 2014-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)



/* ********************************************************************
* A SHORT PROGRAMME TO CHECK REGION 5 PROPERTY EQUATIONS 
* *********************************************************************/

//TODO  insert correct conditions for g and Cv and check table

#include "IF97_Region5.h"
#include <stdio.h>
#include "IF97_common.h"
#include "if97_lib_test.h"


int if97_region5_test (FILE *logFile){	
	int intermediateResult= TEST_PASS; //initialise with clear flags.  
	
	fprintf(logFile, "\n\n*** IF97 REGION 5 MODULE CHECK ***\n" );

/* PROPERY (FORWARDS) EQUATIONS
* see IAPWS-IF97 Table 42
*
* The results should be as follows
*
*				T= 1500 K				T= 1500 K				T= 2000 K
*				p= 0.5 MPa				p= 30 MPa				p= 30 MPa
* g  (kJ/kg)	?? 						??						??				Using Freesteam
* v  (m3/kg) 	1.38455090e00 			2.30761299e–02 		3.11385219e-02
* h  (kJ/kg)	5.21976855e03 			5.16723514e03		6.57122604e03
* u  (kJ/kg)	4.52749310e03			4.47495124e03		5.63707038e03
* s  (kJ/kg.K)	9.65408875e00			7.72970133e00		8.53640523e00
* Cp (kJ/kg.K)	2.61609445e00			2.72724317e00		2.88569882e00
* Cv (kJ/kg.K)	?????					??????				?????			From Moscow Power Institute check page
* w  (m/s)		9.17068690e02			9.28548002e02		1.06736948e03

* see http://twt.mpei.ac.ru/mcs/worksheets/iapws/IAPWS-IF97-Region2.xmcd
*/



	fprintf(logFile, "*** IF97 REGION 5 (high temperature steam) PROPERTY (FORWARDS) EQUATIONS CHECK ***\n" );
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_g, 0.5, 1500.0, -9261.36457876123, TEST_ACCURACY, SIG_FIG, "if97_r5_g", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_g, 30.0, 1500.0, -6427.31684918484, TEST_ACCURACY, SIG_FIG, "if97_r5_g", logFile);	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_g, 30.0, 2000.0, -10501.5844236579, TEST_ACCURACY, SIG_FIG, "if97_r5_g", logFile);
		
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_v, 0.5, 1500.0, 1.38455090e00, TEST_ACCURACY, SIG_FIG, "if97_r5_v", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_v, 30.0, 1500.0, 2.30761299e-02, TEST_ACCURACY, SIG_FIG, "if97_r5_v", logFile);	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_v, 30.0, 2000.0, 3.11385219e-02, TEST_ACCURACY, SIG_FIG, "if97_r5_v", logFile);
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_h, 0.5, 1500.0, 5.21976855e03, TEST_ACCURACY, SIG_FIG, "if97_r5_h", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_h, 30.0, 1500.0, 5.16723514e03, TEST_ACCURACY, SIG_FIG, "if97_r5_h", logFile);	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_h, 30.0, 2000.0, 6.57122604e03, TEST_ACCURACY, SIG_FIG, "if97_r5_h", logFile);	

	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_u, 0.5, 1500.0, 4.52749310e03, TEST_ACCURACY, SIG_FIG, "if97_r5_u", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_u, 30.0, 1500.0, 4.47495124e03, TEST_ACCURACY, SIG_FIG, "if97_r5_u", logFile);	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_u, 30.0, 2000.0, 5.63707038e03, TEST_ACCURACY, SIG_FIG, "if97_r5_u", logFile);
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_s, 0.5, 1500.0, 9.654088753313, TEST_ACCURACY, SIG_FIG, "if97_r5_s", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_s, 30.0, 1500.0, 7.729701326183, TEST_ACCURACY, SIG_FIG, "if97_r5_s", logFile);	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_s, 30.0, 2000.0, 8.536405231138, TEST_ACCURACY, SIG_FIG, "if97_r5_s", logFile);	
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_Cp, 0.5, 1500.0, 2.61609445e00, TEST_ACCURACY, SIG_FIG, "if97_r5_Cp", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_Cp, 30.0, 1500.0, 2.72724317e00, TEST_ACCURACY, SIG_FIG, "if97_r5_Cp", logFile);	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_Cp, 30.0, 2000.0, 2.88569882e00, TEST_ACCURACY, SIG_FIG, "if97_r5_Cp", logFile);
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_Cv, 0.5, 1500.0, 2.1533778351, TEST_ACCURACY, SIG_FIG, "if97_r5_Cv", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_Cv, 30.0, 1500.0, 2.192748293665, TEST_ACCURACY, SIG_FIG, "if97_r5_Cv", logFile);	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_Cv, 30.0, 2000.0, 2.395894362358, TEST_ACCURACY, SIG_FIG, "if97_r5_Cv", logFile);
		
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_w, 0.5, 1500.0, 9.17068690e02, TEST_ACCURACY, SIG_FIG, "if97_r5_w", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_w, 30.0, 1500.0, 9.28548002e02, TEST_ACCURACY, SIG_FIG, "if97_r5_w", logFile);	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_w, 30.0, 2000.0, 1.06736948e03, TEST_ACCURACY, SIG_FIG, "if97_r5_w", logFile);
	

	if (intermediateResult != 0)
		intermediateResult= intermediateResult | TEST_FAIL;
	return intermediateResult;	
}
















