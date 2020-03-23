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
#include "solve.h"
#include "IF97_B23_test.h"
#include "IF97_Region1_test.h"
#include "IF97_Region2_test.h"
#include "IF97_Region3_test.h"
#include "IF97_Region4_test.h"
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
	
	
	// *** Testing  if97_pt_h  ******
	intermediateResult = TEST_PASS;
	intermediateResult = intermediateResult | fprintf ( pTestLog, "\n\n *** Testing  if97_pt_h  *** \n\n" );	
	intermediateResult = intermediateResult | testDoubleInput (if97_pt_h, 3.0, 300.0, 1.15331273e02, TEST_ACCURACY, SIG_FIG, "if97_pt_h", pTestLog);
	intermediateResult = intermediateResult | testDoubleInput (if97_pt_h, 80.0, 300.0, 1.84142828e02, TEST_ACCURACY, SIG_FIG,"if97_pt_h", pTestLog);
	intermediateResult = intermediateResult | testDoubleInput (if97_pt_h, 0.0035, 300.0, 2.54991145e03,TEST_ACCURACY,  SIG_FIG,"if97_pt_h", pTestLog);
	fprintf ( pTestLog, "if97_pt_h Region 3 not yet implemented \t FAIL\n");
	intermediateResult = intermediateResult | TEST_INCOMPLETE;
	intermediateResult = intermediateResult | testDoubleInput (if97_pt_h, 30.0, 1500.0, 5.16723514e03,TEST_ACCURACY, SIG_FIG,"if97_pt_h", pTestLog);
	
	resultSummary ("if97_pt_h", pTestLog, intermediateResult);


	// *** Testing iapws_surftens  ******
	const double If97_surf_acc = 0.07;  // Highest difference in table 1 of IAPWS R1-76(2014)
	intermediateResult = TEST_PASS;
	
	fprintf ( pTestLog, "\n\n *** Testing  iapws_surftens  *** \n\n" );	
	intermediateResult = intermediateResult | testSingleInput (iapws_surftens, 0.01, 75.65, If97_surf_acc, ABS, "iapws_surftens", pTestLog);
	intermediateResult = intermediateResult | testSingleInput (iapws_surftens, 20.0, 72.74, If97_surf_acc,  ABS, "iapws_surftens", pTestLog);
	intermediateResult = intermediateResult | testSingleInput (iapws_surftens, 100.0, 58.91, If97_surf_acc, ABS,  "iapws_surftens", pTestLog);
	intermediateResult = intermediateResult | testSingleInput (iapws_surftens, 200.0, 37.67, If97_surf_acc, ABS, "iapws_surftens", pTestLog);
	intermediateResult = intermediateResult | testSingleInput (iapws_surftens, 300.0, 14.36, If97_surf_acc, ABS, "iapws_surftens", pTestLog);
	intermediateResult = intermediateResult | testSingleInput (iapws_surftens, 370.0, 0.39, If97_surf_acc, ABS, "iapws_surftens", pTestLog);
	intermediateResult = intermediateResult | testSingleInput (iapws_surftens, IF97_TC - IF97_T_TRIP, 0.0, If97_surf_acc, ABS, "iapws_surftens", pTestLog);
	resultSummary ("iapws_surftens", pTestLog, intermediateResult);
	

	
	
	/* try to solve p(h, t) using secant solver.  one from table 18.
	 * 
	 * @ 450 K 1.5 MPa, enthalpy should be 2.72134539e03 kJ/kg
	 */
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
	 
	fprintf ( pTestLog, "\n\n *** Testing Winsteam Compatibility  ****** " );
	
	fprintf ( pTestLog, "\n\n *** StmPT\n" );
	fprintf ( pTestLog, "StmPT (100 [bar], ""SI" " ) should be 310.99 °C:  Calculated as %.8f\n", StmPT(100, "SI"));  //OK
	fprintf ( pTestLog, "StmPT (100 [bar], ""1" " ) should be 310.99 °C:  Calculated as %.8f\n", StmPT(100, "1"));  //OK
	fprintf ( pTestLog, "StmPT (100 [psi], ""eng" " ) should be 327.82 °F:  Calculated as %.8f\n", StmPT(100, "enG"));  //OK
	fprintf ( pTestLog, "StmPT (100 (psig), ""engg" " ) should be 337.88 °F:  Calculated as %.8f\n", StmPT(100, "engg"));  //OK
	fprintf ( pTestLog, "StmPT (100 (psig), ""enggo" " ) should fail:  Calculated as %.8f\n", StmPT(100, "enggo"));	//NO ERROR CODE 
	fprintf ( pTestLog, "StmPT (10 (MPa), ""SIF" " ) should be 584.149 K °C:  Calculated as %.8f\n", StmPT(10, "SIF"));	  //OK
	fprintf ( pTestLog, "StmPT (100 (kPa), ""SIK" " ) should be 99.61 °C:  Calculated as %.8f\n", StmPT(100, "Sik"));  //OK
	fprintf ( pTestLog, "StmPT (100 (bar), ""MET" " ) should be 310.99 °C:  Calculated as %.8f\n", StmPT(100, "met"));  //OK
	fprintf ( pTestLog, "StmPT (100 (ata), ""METF" " ) should be 309.57 °C:  Calculated as %.8f\n", StmPT(100, "metf"));  //OK
	
	
	fprintf ( pTestLog, "\n\n *** StmTP\n" );
	fprintf ( pTestLog, "StmTP (100 [°C], ""SI" " ) should be 1.01417978 bar:  Calculated as %.8f\n", StmTP(100, "SI"));  //OK
	fprintf ( pTestLog, "StmTP (100 [°C], ""1" " ) should be 1.01417978 bar  Calculated as %.8f\n", StmTP(100, "1"));  //OK
	fprintf ( pTestLog, "StmTP (100 [°F], ""eng" " ) should be 0.95043933:  Calculated as %.8f\n", StmTP(100, "enG"));  //OK
	fprintf ( pTestLog, "StmTP (100 (°F), ""engg" " ) should be -13.74550945 psig:  Calculated as %.8f\n", StmTP(100, "engg"));  //ERROR ?
	fprintf ( pTestLog, "StmTP (100 (°F), ""enggo" " ) should fail:  Calculated as %.8f\n", StmTP(100, "enggo"));	//NO ERROR CODE 
	fprintf ( pTestLog, "StmTP (300 (°K), ""SIF" " ) should be 0.00353659 MPa:  Calculated as %.8f\n", StmTP(300, "SIF"));	  //OK
	fprintf ( pTestLog, "StmTP (100 (°C), ""SIK" " ) should be 101.417978 kPa:  Calculated as %.8f\n", StmTP(100, "Sik"));  //OK
	fprintf ( pTestLog, "StmTP (100 (°C), ""MET" " ) should be 1.01417978 bar:  Calculated as %.8f\n", StmTP(100, "met"));  //OK
	fprintf ( pTestLog, "StmTP (100 (°C), ""METF" " ) should be 1.03417556 ata:  Calculated as %.8f\n", StmTP(100, "metf"));  //OK
	
	fprintf ( pTestLog, "\n\n *** StmPTH\n" );
	fprintf ( pTestLog, "StmPTH (100 [bar],  [500 [°C], ""SI" " ) should be 3375.05844 kJ/kg:  Calculated as %.8f\n", StmPTH(100, 500, "SI"));  //OK
	fprintf ( pTestLog, "StmPTH (100 [bar], 500 [°C], ""1" " ) should be 3375.05844 kJ/kg:  Calculated as %.8f\n", StmPTH(100, 500, "1"));   //OK
	fprintf ( pTestLog, "StmPTH (100 [psia], 500 [°F], ""eng" " ) should be 1279.32095 BTU/lb:  Calculated as %.8f\n", StmPTH(100, 500, "enG"));  //OK
	fprintf ( pTestLog, "StmPTH (100 [psig], 500 (°F), ""engg" " ) should be 1277.89101 BTU/lb:  Calculated as %.8f\n", StmPTH(100, 500, "engg"));  //ERROR ??
	fprintf ( pTestLog, "StmPTH (100 [psig] 500 (°F), ""enggo" " ) should fail:  Calculated as %.8f\n", StmPTH(100, 500, "enggo"));	 //NO ERROR CODE 
	fprintf ( pTestLog, "StmPTH (100 [MPa], 500 (°K), ""SIF" " ) should be 1015.39368 kJ/kg:  Calculated as %.8f\n", StmPTH(100, 500, "SIF"));	  //OK
	fprintf ( pTestLog, "StmPTH (100 [kPa], 500 (°C), ""SIK" " ) should be 3488.70857 kJ/kg:  Calculated as %.8f\n", StmPTH(100, 500, "Sik"));  //OK
	fprintf ( pTestLog, "StmPTH (100 [bar], 500 (°C), ""MET" " ) should be 806.118860 kCal/kg:  Calculated as %.8f\n", StmPTH(100, 500, "met"));  //OK
	fprintf ( pTestLog, "StmPTH (100 [ata], 500 (°C), ""METF" " ) should be 806.688261 kCal/kg:  Calculated as %.8f\n", StmPTH(100, 500, "metf"));	  //OK
	
	fprintf ( pTestLog, "\n\n *** StmPTS\n" );
	fprintf ( pTestLog, "StmPTS (100 [bar],  [500 [°C], ""SI" " ) should be 6.59932253 kJ/kg K:  Calculated as %.8f\n", StmPTS(100, 500, "SI"));  //OK
	fprintf ( pTestLog, "StmPTS (100 [bar], 500 [°C], ""1" " ) should be 6.59932253 kJ/kg K:  Calculated as %.8f\n", StmPTS(100, 500, "1"));   //OK
	fprintf ( pTestLog, "StmPTS (100 [psia], 500 [°F], ""eng" " ) should be 1.70889598 BTU/lb R:  Calculated as %.8f\n", StmPTS(100, 500, "enG"));   //OK
	fprintf ( pTestLog, "StmPTS (100 [psig], 500 (°F), ""engg" " ) should be 1.69268747 BTU/lb R:  Calculated as %.8f\n", StmPTS(100, 500, "engg"));  //ERROR ??
	fprintf ( pTestLog, "StmPTS (100 [psig] 500 (°F), ""enggo" " ) should fail:  Calculated as %.8f\n", StmPTS(100, 500, "enggo"));		 //NO ERROR CODE 
	fprintf ( pTestLog, "StmPTS (100 [MPa], 500 (°K), ""SIF" " ) should be 2.43662387 kJ/kg K:  Calculated as %.8f\n", StmPTS(100, 500, "SIF"));  //OK
	fprintf ( pTestLog, "StmPTS (100 [kPa], 500 (°C), ""SIK" " ) should be 8.83606364 kJ/kg K:  Calculated as %.8f\n", StmPTS(100, 500, "Sik"));  //OK
	fprintf ( pTestLog, "StmPTS (100 [bar], 500 (°C), ""MET" " ) should be 1.57622111 kCal/kg C:  Calculated as %.8f\n", StmPTS(100, 500, "met"));  //OK
	fprintf ( pTestLog, "StmPTS (100 [ata], 500 (°C), ""METF" " ) should be 1.57893849 kCal/kg C:  Calculated as %.8f\n", StmPTS(100, 500, "metf"));  //OK

	
	fprintf ( pTestLog, "\n\n *** StmPTV\n" );
	fprintf ( pTestLog, "StmPTV (100 [bar],  [500 [°C], ""SI" " ) should be 0.0328128630 m3/kg:  Calculated as %.8f\n", StmPTV(100, 500, "SI")); //OK
	fprintf ( pTestLog, "StmPTV (100 [bar], 500 [°C], ""1" " ) should be 0.0328128630 m3/kg:  Calculated as %.8f\n", StmPTV(100, 500, "1")); //OK
	fprintf ( pTestLog, "StmPTV (100 [psia], 500 [°F], ""eng" " ) should be 5.58753886 ft3/lbm:  Calculated as %.8f\n", StmPTV(100, 500, "enG"));  // ERROR??
	fprintf ( pTestLog, "StmPTV (100 [psig], 500 (°F), ""engg" " ) should be XXX:  Calculated as %.8f\n", StmPTV(100, 500, "engg"));  //ERROR 
	fprintf ( pTestLog, "StmPTV (100 [psig] 500 (°F), ""enggo" " ) should fail:  Calculated as %.8f\n", StmPTV(100, 500, "enggo"));	//NO ERROR CODE 
	fprintf ( pTestLog, "StmPTV (100 [MPa], 500 (°K), ""SIF" " ) should be 0.00111209398 m3/kg:  Calculated as %.8f\n", StmPTV(100, 500, "SIF"));	 //OK
	fprintf ( pTestLog, "StmPTV (100 [kPa], 500 (°C), ""SIK" " ) should be 3.56558322 m3/kg:  Calculated as %.8f\n", StmPTV(100, 500, "Sik"));   //OK
	fprintf ( pTestLog, "StmPTV (100 [bar], 500 (°C), ""MET" " ) should be 0.0328128630 m3/kg:  Calculated as %.8f\n", StmPTV(100, 500, "met"));
	fprintf ( pTestLog, "StmPTV (100 [ata], 500 (°C), ""METF" " ) should be 0.0335198514 m3/kg:  Calculated as %.8f\n", StmPTV(100, 500, "metf"));  //OK

	fprintf ( pTestLog, "\n\n *** StmPTC\n" );
	fprintf ( pTestLog, "StmPTC (100 [bar],  [500 [°C], ""SI" " ) should be 2.58333874 kJ/kg/C:  Calculated as %.8f\n", StmPTC(100, 500, "SI"));  //OK
	fprintf ( pTestLog, "StmPTC (100 [bar], 500 [°C], ""1" " ) should be 2.58333874 kJ/kg/C:  Calculated as %.8f\n", StmPTC(100, 500, "1"));  //OK
	fprintf ( pTestLog, "StmPTC (100 [psia], 500 [°F], ""eng" " ) should be XXX btu/lbm/F:  Calculated as %.8f\n", StmPTC(100, 500, "enG")); 
	fprintf ( pTestLog, "StmPTC (100 [psig], 500 (°F), ""engg" " ) should be XXX btu/lbm/F:  Calculated as %.8f\n", StmPTC(100, 500, "engg"));
	fprintf ( pTestLog, "StmPTC (100 [psig] 500 (°F), ""enggo" " ) should fail:  Calculated as %.8f\n", StmPTC(100, 500, "enggo"));	//NO ERROR CODE 
	fprintf ( pTestLog, "StmPTC (100 [MPa], 500 (°K), ""SIF" " ) should be XXX kJ/kg/K:  Calculated as %.8f\n", StmPTC(100, 500, "SIF"));
	fprintf ( pTestLog, "StmPTC (100 [kPa], 500 (°C), ""SIK" " ) should be XXX kJ/kg/C:  Calculated as %.8f\n", StmPTC(100, 500, "Sik"));
	fprintf ( pTestLog, "StmPTC (100 [bar], 500 (°C), ""MET" " ) should be XXX kJ/kg/C:  Calculated as %.8f\n", StmPTC(100, 500, "met"));
	fprintf ( pTestLog, "StmPTC (100 [ata], 500 (°C), ""METF" " ) should be XXX kJ/kg/C:  Calculated as %.8f\n", StmPTC(100, 500, "metf"));
	
	fprintf ( pTestLog, "\n\n *** StmPTK\n" );																							//ERROR 
	fprintf ( pTestLog, "StmPTK (100 [bar],  [500 [°C], ""SI" " ) should be 0.0759223503 W/m/C:  Calculated as %.8f\n", StmPTK(100, 500, "SI"));
	fprintf ( pTestLog, "StmPTK (100 [bar], 500 [°C], ""1" " ) should be 0.0759223503 W/m/C:  Calculated as %.8f\n", StmPTK(100, 500, "1"));
	fprintf ( pTestLog, "StmPTK (100 [psia], 500 [°F], ""eng" " ) should be XXX btu/h/ft/F:  Calculated as %.8f\n", StmPTK(100, 500, "enG")); 
	fprintf ( pTestLog, "StmPTK (100 [psig], 500 (°F), ""engg" " ) should be XXX btu/h/ft/F:  Calculated as %.8f\n", StmPTK(100, 500, "engg"));
	fprintf ( pTestLog, "StmPTK (100 [psig] 500 (°F), ""enggo" " ) should fail:  Calculated as %.8f\n", StmPTK(100, 500, "enggo"));	//NO ERROR CODE 
	fprintf ( pTestLog, "StmPTK (100 [MPa], 500 (°K), ""SIF" " ) should be XXX W/m/K:  Calculated as %.8f\n", StmPTK(100, 500, "SIF"));
	fprintf ( pTestLog, "StmPTK (100 [kPa], 500 (°C), ""SIK" " ) should be XXX W/m/C:  Calculated as %.8f\n", StmPTK(100, 500, "Sik"));
	fprintf ( pTestLog, "StmPTK (100 [bar], 500 (°C), ""MET" " ) should be 0.0759223503 W/m/C:  Calculated as %.8f\n", StmPTK(100, 500, "met"));
	fprintf ( pTestLog, "StmPTK (100 [ata], 500 (°C), ""METF" " ) should be XXX kCal/m/h/C:  Calculated as %.8f\n", StmPTK(100, 500, "metf"));

	
	fprintf ( pTestLog, "\n\n *** StmPTM\n" );																							//ERROR 
	fprintf ( pTestLog, "StmPTM (100 [bar],  [500 [°C], ""SI" " ) should be XXX Centipoise:  Calculated as %.8f\n", StmPTM(100, 500, "SI"));
	fprintf ( pTestLog, "StmPTM (100 [bar], 500 [°C], ""1" " ) should be XXX m/s:  Calculated as %.8f\n", StmPTM(100, 500, "1"));
	fprintf ( pTestLog, "StmPTM (100 [psia], 500 [°F], ""eng" " ) should be XXX lbm/ft/h:  Calculated as %.8f\n", StmPTM(100, 500, "enG")); 
	fprintf ( pTestLog, "StmPTM (100 [psig], 500 (°F), ""engg" " ) should be XXX lbm/ft/h:  Calculated as %.8f\n", StmPTM(100, 500, "engg"));
	fprintf ( pTestLog, "StmPTM (100 [psig] 500 (°F), ""enggo" " ) should fail:  Calculated as %.8f\n", StmPTM(100, 500, "enggo"));	//NO ERROR CODE 
	fprintf ( pTestLog, "StmPTM (100 [MPa], 500 (°K), ""SIF" " ) should be XXX Pa.s:  Calculated as %.8f\n", StmPTM(100, 500, "SIF"));
	fprintf ( pTestLog, "StmPTM (100 [kPa], 500 (°C), ""SIK" " ) should be XXX Centipoise:  Calculated as %.8f\n", StmPTM(100, 500, "Sik"));
	fprintf ( pTestLog, "StmPTM (100 [bar], 500 (°C), ""MET" " ) should be XXX Centipoise:  Calculated as %.8f\n", StmPTM(100, 500, "met"));
	fprintf ( pTestLog, "StmPTM (100 [ata], 500 (°C), ""METF" " ) should be XXX Centipoise:  Calculated as %.8f\n", StmPTM(100, 500, "metf"));
	
	fprintf ( pTestLog, "\n\n *** StmPTW\n" );
	fprintf ( pTestLog, "StmPTW (100 [bar],  [500 [°C], ""SI" " ) should be 647.885589 m/s:  Calculated as %.8f\n", StmPTW(100, 500, "SI"));  //OK
	fprintf ( pTestLog, "StmPTW (100 [bar], 500 [°C], ""1" " ) should be 647.885589 m/s:  Calculated as %.8f\n", StmPTW(100, 500, "1"));  //OK
	fprintf ( pTestLog, "StmPTW (100 [psia], 500 [°F], ""eng" " ) should be XXX ft/s:  Calculated as %.8f\n", StmPTW(100, 500, "enG")); 
	fprintf ( pTestLog, "StmPTW (100 [psig], 500 (°F), ""engg" " ) should be XXX ft/s:  Calculated as %.8f\n", StmPTW(100, 500, "engg"));
	fprintf ( pTestLog, "StmPTW (100 [psig] 500 (°F), ""enggo" " ) should fail:  Calculated as %.8f\n", StmPTW(100, 500, "enggo"));	//NO ERROR CODE 
	fprintf ( pTestLog, "StmPTW (100 [MPa], 500 (°K), ""SIF" " ) should be XXX m/s:  Calculated as %.8f\n", StmPTW(100, 500, "SIF"));
	fprintf ( pTestLog, "StmPTW (100 [kPa], 500 (°C), ""SIK" " ) should be XXX m/s:  Calculated as %.8f\n", StmPTW(100, 500, "Sik"));
	fprintf ( pTestLog, "StmPTW (100 [bar], 500 (°C), ""MET" " ) should be 647.885589 m/s:  Calculated as %.8f\n", StmPTW(100, 500, "met"));  //OK
	fprintf ( pTestLog, "StmPTW (100 [ata], 500 (°C), ""METF" " ) should be XXX m/s:  Calculated as %.8f\n", StmPTW(100, 500, "metf"));	
	
	fprintf ( pTestLog, "\n\n *** StmPTG\n" );
	fprintf ( pTestLog, "StmPTG (100 [bar],  [500 [°C], ""SI" " ) should be XXX :  Calculated as %.8f\n", StmPTG(100, 500, "SI"));
	fprintf ( pTestLog, "StmPTG (100 [bar], 500 [°C], ""1" " ) should be XXX :  Calculated as %.8f\n", StmPTG(100, 500, "1"));
	fprintf ( pTestLog, "StmPTG (100 [psia], 500 [°F], ""eng" " ) should be XXX :  Calculated as %.8f\n", StmPTG(100, 500, "enG")); 
	fprintf ( pTestLog, "StmPTG (100 [psig], 500 (°F), ""engg" " ) should be XXX :  Calculated as %.8f\n", StmPTG(100, 500, "engg"));
	fprintf ( pTestLog, "StmPTG (100 [psig] 500 (°F), ""enggo" " ) should fail:  Calculated as %.8f\n", StmPTG(100, 500, "enggo"));	//NO ERROR CODE 
	fprintf ( pTestLog, "StmPTG (100 [MPa], 500 (°K), ""SIF" " ) should be XXX :  Calculated as %.8f\n", StmPTG(100, 500, "SIF"));
	fprintf ( pTestLog, "StmPTG (100 [kPa], 500 (°C), ""SIK" " ) should be XXX :  Calculated as %.8f\n", StmPTG(100, 500, "Sik"));
	fprintf ( pTestLog, "StmPTG (100 [bar], 500 (°C), ""MET" " ) should be XXX :  Calculated as %.8f\n", StmPTG(100, 500, "met"));
	fprintf ( pTestLog, "StmPTG (100 [ata], 500 (°C), ""METF" " ) should be XXX :  Calculated as %.8f\n", StmPTG(100, 500, "metf"));	
	**/
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