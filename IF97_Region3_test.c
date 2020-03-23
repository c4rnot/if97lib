
//          Copyright Martin Lord 2014-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)



/* *****************************************************************************
* A SHORT PROGRAMME TO CHECK REGION 3 PROPERTY EQUATIONS 
* *******************************************************************************/

#include "IF97_Region3.h"
#include <stdio.h>
#include "IF97_common.h"
#include "if97_lib_test.h"


int if97_region3_test (FILE *logFile){	
	int intermediateResult= TEST_PASS; //initialise with clear flags.  
	
	fprintf(logFile, "\n\n*** IF97 REGION 3 MODULE CHECK ***\n" );

/* PROPERY (FORWARDS) EQUATIONS
* see IAPWS-IF97 Table 33
*
* The results should be as follows
*
*				T= 650 K				T= 650 K				T= 750 K
*				rho= 500 kg/m3			rho= 200 kg/m3			rho= 500 kg/m3	
* hhz  (kJ/kg)	??				 		????					???? 	From freesteam
* P  (MPa) 		2.55837018e01			2.22930643e01			7.83095639e01
* h  (kJ/kg)	1.86343019e03			2.37512401e03			2.25868845e03
* u  (kJ/kg)	1.81226279e03			2.26365868e03			2.10206932e03
* s  (kJ/kg.K)	4.05427273e00			4.85438792e00			4.46971906e00
* Cp (kJ/kg.K)	1.38935717e01			4.46579342e01			6.34165359e00
* Cv (kJ/kg.K)	??						??						??		From Moscow Power Institute check page
* w  (m/s)		5.02005554e02			3.83444594e02			7.60696041e02

* see http://twt.mpei.ac.ru/mcs/worksheets/iapws/IAPWS-IF97-Region1.xmcd
*/

	fprintf(logFile, "*** IF97 REGION 3 PROPERTY (FORWARDS) EQUATIONS CHECK ***\n\n" );
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3_hhz, 500.0, 650.0 , -1, TEST_ACCURACY, SIG_FIG, "if97_r3_hhz", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3_hhz, 200.0, 650.0 , -1, TEST_ACCURACY, SIG_FIG, "if97_r3_hhz", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3_hhz, 500.0, 750.0 , -1, TEST_ACCURACY, SIG_FIG, "if97_r3_hhz", logFile);
	fprintf(logFile, "TODO Determine correct Helmholz free energies for region 3 to check against\n\n");
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3_p, 500.0, 650.0 , 2.55837018e01, TEST_ACCURACY, SIG_FIG, "if97_r3_p", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3_p, 200.0, 650.0 , 2.22930643e01, TEST_ACCURACY, SIG_FIG, "if97_r3_p", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3_p, 500.0, 750.0 , 7.83095639e01, TEST_ACCURACY, SIG_FIG, "if97_r3_p", logFile);
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3_h, 500.0, 650.0 , 1.86343019e03, TEST_ACCURACY, SIG_FIG, "if97_r3_h", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3_h, 200.0, 650.0 , 2.37512401e03, TEST_ACCURACY, SIG_FIG, "if97_r3_h", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3_h, 500.0, 750.0 , 2.25868845e03, TEST_ACCURACY, SIG_FIG, "if97_r3_h", logFile);
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3_u, 500.0, 650.0 , 1.81226279e03, TEST_ACCURACY, SIG_FIG, "if97_r3_u", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3_u, 200.0, 650.0 , 2.26365868e03, TEST_ACCURACY, SIG_FIG, "if97_r3_u", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3_u, 500.0, 750.0 , 2.10206932e03, TEST_ACCURACY, SIG_FIG, "if97_r3_u", logFile);
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3_s, 500.0, 650.0 , 4.05427273e00, TEST_ACCURACY, SIG_FIG, "if97_r3_s", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3_s, 200.0, 650.0 , 4.85438792e00, TEST_ACCURACY, SIG_FIG, "if97_r3_s", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3_s, 500.0, 750.0 , 4.46971906e00, TEST_ACCURACY, SIG_FIG, "if97_r3_s", logFile);
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3_Cp, 500.0, 650.0 , 1.38935717e01, TEST_ACCURACY, SIG_FIG, "if97_r3_Cp", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3_Cp, 200.0, 650.0 , 4.46579342e01, TEST_ACCURACY, SIG_FIG, "if97_r3_Cp", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3_Cp, 500.0, 750.0 , 6.34165359e00, TEST_ACCURACY, SIG_FIG, "if97_r3_Cp", logFile);

	intermediateResult = intermediateResult | testDoubleInput ( if97_r3_Cv, 500.0, 650.0 , -1, TEST_ACCURACY, SIG_FIG, "if97_r3_Cv", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3_Cv, 200.0, 650.0 , -1, TEST_ACCURACY, SIG_FIG, "if97_r3_Cv", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3_Cv, 500.0, 750.0 , -1, TEST_ACCURACY, SIG_FIG, "if97_r3_Cv", logFile);
	fprintf(logFile, "TODO Determine correct Cv   for region 3 to check against\n\n");

	intermediateResult = intermediateResult | testDoubleInput ( if97_r3_w, 500.0, 650.0 , 5.02005554e02, TEST_ACCURACY, SIG_FIG, "if97_r3_w", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3_w, 200.0, 650.0 , 3.83444594e02, TEST_ACCURACY, SIG_FIG, "if97_r3_w", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3_w, 500.0, 750.0 , 7.60696041e02, TEST_ACCURACY, SIG_FIG, "if97_r3_w", logFile);
	

	

/* IF97 REGION 3 BACKWARDS v(p,T) SUBREGION BOUNDARIES CHECK

Table 3 of supplementary release for v(p, T) for region3: June 2014

The output should be the following:

Equation	P (MPa)	T (Kelvin)
T3ab(p)		40		6.930341408e02
T3cd(p)		25		6.493659208e02
T3ef(p)		40		7.139593992e02
T3gh(p)		23		6.498873759e02
T3ij(p)		23		6.515778091e02
T3jk(p)		23		6.558338344e02
T3mn(p)		22.8	6.496054133e02
T3op(p)		22.8	6.500106943e02
T3qu(p)		22		6.456355027e02
T3rx(p)		22		6.482622754e02	
*/	

	fprintf(logFile, "\n\n*** IF97 REGION 3 BACKWARDS v(p,T) SUBREGION BOUNDARIES CHECK ***\n\n" );
	
	intermediateResult = intermediateResult | testSingleInput ( if97_r3ab_p_t, 40.0, 6.930341408e02, TEST_ACCURACY, SIG_FIG, "if97_r3ab_p_t", logFile);
	
	intermediateResult = intermediateResult | testSingleInput ( if97_r3cd_p_t, 25.0, 6.493659208e02, TEST_ACCURACY, SIG_FIG, "if97_r3cd_p_t", logFile);
	
	intermediateResult = intermediateResult | testSingleInput ( if97_r3ef_p_t, 40.0, 7.139593992e02, TEST_ACCURACY, SIG_FIG, "if97_r3ef_p_t", logFile);
	
	intermediateResult = intermediateResult | testSingleInput ( if97_r3gh_p_t, 23.0, 6.498873759e02, TEST_ACCURACY, SIG_FIG, "if97_r3gh_p_t", logFile);
	
	intermediateResult = intermediateResult | testSingleInput ( if97_r3ij_p_t, 23.0, 6.515778091e02, TEST_ACCURACY, SIG_FIG, "if97_r3ij_p_t", logFile);
	
	intermediateResult = intermediateResult | testSingleInput ( if97_r3jk_p_t, 23.0, 6.558338344e02, TEST_ACCURACY, SIG_FIG, "if97_r3jk_p_t", logFile);
	
	intermediateResult = intermediateResult | testSingleInput ( if97_r3mn_p_t, 22.8, 6.496054133e02, TEST_ACCURACY, SIG_FIG, "if97_r3mn_p_t", logFile);
	
	intermediateResult = intermediateResult | testSingleInput ( if97_r3op_p_t, 22.8, 6.500106943e02, TEST_ACCURACY, SIG_FIG, "if97_r3op_p_t", logFile);
	
	intermediateResult = intermediateResult | testSingleInput ( if97_r3qu_p_t, 22.0, 6.456355027e02, TEST_ACCURACY, SIG_FIG, "if97_r3qu_p_t", logFile);
	
	intermediateResult = intermediateResult | testSingleInput ( if97_r3rx_p_t, 22.0, 6.482622754e02, TEST_ACCURACY, SIG_FIG, "if97_r3rx_p_t", logFile);
	
	
	

/*  IF97 REGION 3 BACKWARDS v(p,T)

Table 3 of supplementary release for v(p, T) for region3: June 2014

The output should be the following:

Equation        P (MPa) T (Kelvin)	v (m3/kg)
v3a(p,t)        50      630     1.470853100e-3
v3a(p,t)        80      670     1.503831359e-3
-----------------------------------------
v3b(p,t)        50      710     2.204728587e-3
v3b(p,t)        80      750     1.973692940e-3
-----------------------------------------
v3c(p,t)        20      630     1.761696406e-3
v3c(p,t)        30      650     1.819560617e-3
-----------------------------------------
v3d(p,t)        20      656     2.245587720e-3
v3d(p,t)        30      670     2.506897702e-3
-----------------------------------------
*/

	fprintf(logFile, "\n\n*** IF97 REGION 3 BACKWARDS v(p,T) CHECK ***\n\n" );


	intermediateResult = intermediateResult | testDoubleInput ( if97_r3a_v_pt, 50.0, 630.0, 1.470853100e-3, TEST_ACCURACY, SIG_FIG, "if97_r3a_v_pt", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3a_v_pt, 80.0, 670.0, 1.503831359e-3, TEST_ACCURACY, SIG_FIG, "if97_r3a_v_pt", logFile);
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3b_v_pt, 50.0, 710.0, 2.204728587e-3, TEST_ACCURACY, SIG_FIG, "if97_r3b_v_pt", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3b_v_pt, 80.0, 750.0, 1.973692940e-3, TEST_ACCURACY, SIG_FIG, "if97_r3b_v_pt", logFile);	

	intermediateResult = intermediateResult | testDoubleInput ( if97_r3c_v_pt, 20.0, 630.0, 1.761696406e-3, TEST_ACCURACY, SIG_FIG, "if97_r3c_v_pt", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3c_v_pt, 30.0, 650.0, 1.819560617e-3, TEST_ACCURACY, SIG_FIG, "if97_r3c_v_pt", logFile);
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3d_v_pt, 20.0, 656.0, 2.245587720e-3, TEST_ACCURACY, SIG_FIG, "if97_r3d_v_pt", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3d_v_pt, 30.0, 670.0, 2.506897702e-3, TEST_ACCURACY, SIG_FIG, "if97_r3d_v_pt", logFile);
	fprintf(logFile, "TODO Determine error in if97_r3d_v_pt \n\n");

	
	if (intermediateResult != 0)
		intermediateResult= intermediateResult | TEST_FAIL;
	return intermediateResult;
}
