//          Copyright Martin Lord 2014-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)



/* ********************************************************************
* A SHORT PROGRAMME TO CHECK THE WINSTEAM COMPATIBILITY MODULE 
* *********************************************************************/



#include "winsteam_compatibility.h"
#include <stdio.h>
#include "IF97_common.h"
#include "if97_lib_test.h"


int winsteam_compatibility_test (FILE *logFile){	
	int intermediateResult= TEST_PASS; //initialise with clear flags.  
	
	fprintf(logFile, "\n\n*** IF97 REGION 5 MODULE CHECK ***\n" );


	fprintf(logFile, "*** IF97 REGION 5 (high temperature steam) PROPERTY (FORWARDS) EQUATIONS CHECK ***\n" );
	
	//intermediateResult = intermediateResult | testDoubleInput ( StmPT, 0.5, 1500.0, -1, TEST_ACCURACY, SIG_FIG, "if97_r5_v", logFile);


/*
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
	
	
	

	
	
	
	if (intermediateResult != 0)
		intermediateResult= intermediateResult | TEST_FAIL;
	return intermediateResult;	
}
















