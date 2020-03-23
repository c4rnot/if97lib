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
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_v, 0.5, 1500.0, -1, TEST_ACCURACY, SIG_FIG, "if97_r5_v", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_v, 30.0, 1500.0, -1, TEST_ACCURACY, SIG_FIG, "if97_r5_v", logFile);	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_v, 30.0, 2000.0, -1, TEST_ACCURACY, SIG_FIG, "if97_r5_v", logFile);
	fprintf(logFile, "TODO  obtain expected g to crosscheck\n\n" );
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_v, 0.5, 1500.0, 1.38455090e00, TEST_ACCURACY, SIG_FIG, "if97_r5_v", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_v, 30.0, 1500.0, 2.30761299e-02, TEST_ACCURACY, SIG_FIG, "if97_r5_v", logFile);	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_v, 30.0, 2000.0, 3.11385219e-02, TEST_ACCURACY, SIG_FIG, "if97_r5_v", logFile);
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_h, 0.5, 1500.0, 5.21976855e03, TEST_ACCURACY, SIG_FIG, "if97_r5_h", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_h, 30.0, 1500.0, 5.16723514e03, TEST_ACCURACY, SIG_FIG, "if97_r5_h", logFile);	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_h, 30.0, 2000.0, 6.57122604e03, TEST_ACCURACY, SIG_FIG, "if97_r5_h", logFile);	

	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_u, 0.5, 1500.0, 4.52749310e03, TEST_ACCURACY, SIG_FIG, "if97_r5_u", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_u, 30.0, 1500.0, 4.47495124e03, TEST_ACCURACY, SIG_FIG, "if97_r5_u", logFile);	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_u, 30.0, 2000.0, 5.63707038e03, TEST_ACCURACY, SIG_FIG, "if97_r5_u", logFile);
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_s, 0.5, 1500.0, 4.52749310e03, TEST_ACCURACY, SIG_FIG, "if97_r5_s", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_s, 30.0, 1500.0, 4.47495124e03, TEST_ACCURACY, SIG_FIG, "if97_r5_s", logFile);	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_s, 30.0, 2000.0, 5.63707038e03, TEST_ACCURACY, SIG_FIG, "if97_r5_s", logFile);	
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_Cp, 0.5, 1500.0, 2.61609445e00, TEST_ACCURACY, SIG_FIG, "if97_r5_Cp", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_Cp, 30.0, 1500.0, 2.72724317e00, TEST_ACCURACY, SIG_FIG, "if97_r5_Cp", logFile);	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_Cp, 30.0, 2000.0, 2.88569882e00, TEST_ACCURACY, SIG_FIG, "if97_r5_Cp", logFile);
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_Cv, 0.5, 1500.0, -1, TEST_ACCURACY, SIG_FIG, "if97_r5_Cv", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_Cv, 30.0, 1500.0, -1, TEST_ACCURACY, SIG_FIG, "if97_r5_Cv", logFile);	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_Cv, 30.0, 2000.0, -1, TEST_ACCURACY, SIG_FIG, "if97_r5_Cv", logFile);
	fprintf(logFile, "TODO  obtain expected Cv to crosscheck\n\n" );
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_w, 0.5, 1500.0, 9.17068690e02, TEST_ACCURACY, SIG_FIG, "if97_r5_w", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_w, 30.0, 1500.0, 9.28548002e02, TEST_ACCURACY, SIG_FIG, "if97_r5_w", logFile);	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r5_w, 30.0, 2000.0, 1.06736948e03, TEST_ACCURACY, SIG_FIG, "if97_r5_w", logFile);
	
	
	printf ("\n\n************************************************************ \n" );
	printf ("IF97 REGION 5 (high temperature steam) PROPERTY (FORWARDS) EQUATIONS CHECK \n\n" );
	printf ("The output should be the following: \n\n");
	printf ("\t\tT= 1500 K \tT= 1500 K \tT= 2000 K\n");
	printf ("\t\tp= 0.5 MPa \tp= 30 MPa \tp= 30 MPa\n\n");

	printf ("g  (kJ/kg)\t?????????\t?????????\t???????????\t(1)\n");	
	printf ("v  (m3/kg)\t1.38455090e+00\t2.30761299e–02\t3.11385219e-02\n");
	printf ("h  (kJ/kg)\t5.21976855e+03\t5.16723514e+03\t6.57122604e+03\n");
	printf ("u  (kJ/kg)\t4.52749310e+03\t4.47495124e+03\t5.63707038e+03\n");
	printf ("s  (kJ/kg.K)\t9.65408875e+00\t7.72970133e+00\t8.53640523e+00\n");
	printf ("Cp (kJ/kg.K)\t2.61609445e+00\t2.72724317e+00\t2.88569882e+00\n");
	printf ("Cv (kJ/kg.K)\t?????????\t?????????\t???????????\\t(2)\n");
	printf ("w  (m/s)\t9.17068690e+02\t9.28548002e+02\t1.06736948e+03\n\n");
	
	printf ("\n(1) Using Freesteam \n(2) Using Moscow Power Institute calculation \n    http://twt.mpei.ac.ru/mcs/worksheets/iapws/IAPWS-IF97-metastable-vapor.xmcd\n\n");
	printf ("*******OUTPUT************\n");
	

	
	
	
	if (intermediateResult != 0)
		intermediateResult= intermediateResult | TEST_FAIL;
	return intermediateResult;	
}
















