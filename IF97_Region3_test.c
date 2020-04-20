
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
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3_hhz, 500.0, 650.0 , -823.014490474163, TEST_ACCURACY, SIG_FIG, "if97_r3_hhz", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3_hhz, 200.0, 650.0 , -891.693463667219, TEST_ACCURACY, SIG_FIG, "if97_r3_hhz", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3_hhz, 500.0, 750.0 , -1250.21997453598, TEST_ACCURACY, SIG_FIG, "if97_r3_hhz", logFile);
		
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

	intermediateResult = intermediateResult | testDoubleInput ( if97_r3_Cv, 500.0, 650.0 , 3.191317871889, TEST_ACCURACY, SIG_FIG, "if97_r3_Cv", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3_Cv, 200.0, 650.0 , 4.04118075955, TEST_ACCURACY, SIG_FIG, "if97_r3_Cv", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3_Cv, 500.0, 750.0 , 2.71701677121, TEST_ACCURACY, SIG_FIG, "if97_r3_Cv", logFile);
	
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
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3d_v_pt, 26.0, 656.0, 2.245587720e-3, TEST_ACCURACY, SIG_FIG, "if97_r3d_v_pt", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3d_v_pt, 30.0, 670.0, 2.506897702e-3, TEST_ACCURACY, SIG_FIG, "if97_r3d_v_pt", logFile);

	intermediateResult = intermediateResult | testDoubleInput ( if97_r3e_v_pt, 26.0, 661.0, 2.970225962e-3, TEST_ACCURACY, SIG_FIG, "if97_r3e_v_pt", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3e_v_pt, 30.0, 675.0, 3.004627086e-3, TEST_ACCURACY, SIG_FIG, "if97_r3e_v_pt", logFile);	

	intermediateResult = intermediateResult | testDoubleInput ( if97_r3f_v_pt, 26.0, 671.0, 5.019029401e-3, TEST_ACCURACY, SIG_FIG, "if97_r3f_v_pt", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3f_v_pt, 30.0, 690.0, 4.656470142e-3, TEST_ACCURACY, SIG_FIG, "if97_r3f_v_pt", logFile);	

	intermediateResult = intermediateResult | testDoubleInput ( if97_r3g_v_pt, 23.6, 649.0, 2.163198378e-3, TEST_ACCURACY, SIG_FIG, "if97_r3g_v_pt", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3g_v_pt, 24.0, 650.0, 2.166044161e-3, TEST_ACCURACY, SIG_FIG, "if97_r3g_v_pt", logFile);	

	intermediateResult = intermediateResult | testDoubleInput ( if97_r3h_v_pt, 23.6, 652.0, 2.651081407e-3, TEST_ACCURACY, SIG_FIG, "if97_r3h_v_pt", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3h_v_pt, 24.0, 654.0, 2.967802335e-3, TEST_ACCURACY, SIG_FIG, "if97_r3h_v_pt", logFile);	
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3i_v_pt, 23.6, 653.0, 3.273916816e-3, TEST_ACCURACY, SIG_FIG, "if97_r3i_v_pt", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3i_v_pt, 24.0, 655.0, 3.550329864e-3, TEST_ACCURACY, SIG_FIG, "if97_r3i_v_pt", logFile);
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3j_v_pt, 23.5, 655.0, 4.545001142e-3, TEST_ACCURACY, SIG_FIG, "if97_r3j_v_pt", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3j_v_pt, 24.0, 660.0, 5.100267704e-3, TEST_ACCURACY, SIG_FIG, "if97_r3j_v_pt", logFile);

	intermediateResult = intermediateResult | testDoubleInput ( if97_r3k_v_pt, 23.0, 660.0, 6.109525997e-3, TEST_ACCURACY, SIG_FIG, "if97_r3k_v_pt", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3k_v_pt, 24.0, 670.0, 6.427325645e-3, TEST_ACCURACY, SIG_FIG, "if97_r3k_v_pt", logFile);	
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3l_v_pt, 22.6, 646.0, 2.117860851e-3, TEST_ACCURACY, SIG_FIG, "if97_r3l_v_pt", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3l_v_pt, 23.0, 646.0, 2.062374674e-3, TEST_ACCURACY, SIG_FIG, "if97_r3l_v_pt", logFile);	

	intermediateResult = intermediateResult | testDoubleInput ( if97_r3m_v_pt, 22.6, 648.6, 2.533063780e-3, TEST_ACCURACY, SIG_FIG, "if97_r3m_v_pt", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3m_v_pt, 22.8, 649.3, 2.572971781e-3, TEST_ACCURACY, SIG_FIG, "if97_r3m_v_pt", logFile);	

	intermediateResult = intermediateResult | testDoubleInput ( if97_r3n_v_pt, 22.6, 649.0, 2.923432711e-3, TEST_ACCURACY, SIG_FIG, "if97_r3n_v_pt", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3n_v_pt, 22.8, 649.7, 2.913311494e-3, TEST_ACCURACY, SIG_FIG, "if97_r3n_v_pt", logFile);	

	intermediateResult = intermediateResult | testDoubleInput ( if97_r3o_v_pt, 22.6, 649.1, 3.131208996e-3, TEST_ACCURACY, SIG_FIG, "if97_r3o_v_pt", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3o_v_pt, 22.8, 649.9, 3.221160278e-3, TEST_ACCURACY, SIG_FIG, "if97_r3o_v_pt", logFile);	

	intermediateResult = intermediateResult | testDoubleInput ( if97_r3p_v_pt, 22.6, 649.4, 3.715596186e-3, TEST_ACCURACY, SIG_FIG, "if97_r3p_v_pt", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3p_v_pt, 22.8, 650.2, 3.664754790e-3, TEST_ACCURACY, SIG_FIG, "if97_r3p_v_pt", logFile);	

	intermediateResult = intermediateResult | testDoubleInput ( if97_r3q_v_pt, 21.1, 640.0, 1.970999272e-3, TEST_ACCURACY, SIG_FIG, "if97_r3q_v_pt", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3q_v_pt, 21.8, 643.0, 2.043919161e-3, TEST_ACCURACY, SIG_FIG, "if97_r3q_v_pt", logFile);	

	intermediateResult = intermediateResult | testDoubleInput ( if97_r3r_v_pt, 21.1, 644.0, 5.251009921e-3, TEST_ACCURACY, SIG_FIG, "if97_r3r_v_pt", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3r_v_pt, 21.8, 648.0, 5.256844741e-3, TEST_ACCURACY, SIG_FIG, "if97_r3r_v_pt", logFile);	

	intermediateResult = intermediateResult | testDoubleInput ( if97_r3s_v_pt, 19.1, 635.0, 1.932829079e-3, TEST_ACCURACY, SIG_FIG, "if97_r3s_v_pt", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3s_v_pt, 20.0, 638.0, 1.985387227e-3, TEST_ACCURACY, SIG_FIG, "if97_r3s_v_pt", logFile);	

	intermediateResult = intermediateResult | testDoubleInput ( if97_r3t_v_pt, 17.0, 626.0, 8.483262001e-3, TEST_ACCURACY, SIG_FIG, "if97_r3t_v_pt", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3t_v_pt, 20.0, 640.0, 6.227528101e-3, TEST_ACCURACY, SIG_FIG, "if97_r3t_v_pt", logFile);	
	
	
	fprintf(logFile, "\n\n*** IF97 REGION 3 AUXILIARY (NEAR CRITICAL) v(p,T) SUBREGION BOUNDARIES CHECK ***\n\n" );
	
	intermediateResult = intermediateResult | testSingleInput ( if97_r3uv_p_t, 22.3, 6.477996121e02, TEST_ACCURACY, SIG_FIG, "if97_r3uv_p_t", logFile);
	
	intermediateResult = intermediateResult | testSingleInput ( if97_r3wx_p_t, 22.3, 6.482049480e02, TEST_ACCURACY, SIG_FIG, "if97_r3wx_p_t", logFile);
	
	fprintf(logFile, "\n\n*** IF97 REGION 3 AUXILIARY (NEAR CRITICAL) v(p,T) CHECK ***\n\n" );
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3u_v_pt, 21.5, 644.6, 2.268366647e-3, TEST_ACCURACY, SIG_FIG, "if97_r3u_v_pt", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3u_v_pt, 22.0, 646.1, 2.296350553e-3, TEST_ACCURACY, SIG_FIG, "if97_r3u_v_pt", logFile);	

	intermediateResult = intermediateResult | testDoubleInput ( if97_r3v_v_pt, 22.5, 648.6, 2.832373260e-3, TEST_ACCURACY, SIG_FIG, "if97_r3v_v_pt", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3v_v_pt, 22.3, 647.9, 2.811424405e-3, TEST_ACCURACY, SIG_FIG, "if97_r3v_v_pt", logFile);	

	intermediateResult = intermediateResult | testDoubleInput ( if97_r3w_v_pt, 22.15, 647.5, 3.694032281e-3, TEST_ACCURACY, SIG_FIG, "if97_r3w_v_pt", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3w_v_pt, 22.3, 648.1, 3.622226305e-3, TEST_ACCURACY, SIG_FIG, "if97_r3w_v_pt", logFile);	

	intermediateResult = intermediateResult | testDoubleInput ( if97_r3x_v_pt, 22.11, 648.0, 4.528072649e-3, TEST_ACCURACY, SIG_FIG, "if97_r3x_v_pt", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3x_v_pt, 22.3, 649.0, 4.556905799e-3, TEST_ACCURACY, SIG_FIG, "if97_r3x_v_pt", logFile);	

	intermediateResult = intermediateResult | testDoubleInput ( if97_r3y_v_pt, 22.0, 646.84, 2.698354719e-3, TEST_ACCURACY, SIG_FIG, "if97_r3y_v_pt", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3y_v_pt, 22.064, 647.05, 2.717655648e-3, TEST_ACCURACY, SIG_FIG, "if97_r3y_v_pt", logFile);	

	intermediateResult = intermediateResult | testDoubleInput ( if97_r3z_v_pt, 22.0, 646.89, 3.798732962e-3, TEST_ACCURACY, SIG_FIG, "if97_r3z_v_pt", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r3z_v_pt, 22.064, 647.15, 3.701940010e-3, TEST_ACCURACY, SIG_FIG, "if97_r3z_v_pt", logFile);	

	fprintf(logFile, "\n\n*** IF97 REGION 3 REGION SELECTION CHECK ***\n\n" );
	
	intermediateResult = intermediateResult | testBoolDoubleInput ( isNearCritical, 21.93, 646.0, true, "isNearCritical", logFile);
	intermediateResult = intermediateResult | testBoolDoubleInput ( isNearCritical, 21.93, 649.0, false, "isNearCritical", logFile);
	intermediateResult = intermediateResult | testBoolDoubleInput ( isNearCritical, 22.2, 647.0, true, "isNearCritical", logFile);
	intermediateResult = intermediateResult | testBoolDoubleInput ( isNearCritical, 22.51, 647.0, false, "isNearCritical", logFile);
	intermediateResult = intermediateResult | testBoolDoubleInput ( isNearCritical, 21.0, 647.0, false, "isNearCritical", logFile);
	fprintf(logFile, "\n" );
	
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 50.0, 630.0, 'a', "if97_r3_pt_subregion", logFile);
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 80.0, 670.0, 'a', "if97_r3_pt_subregion", logFile);
	
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 50.0, 710.0, 'b', "if97_r3_pt_subregion", logFile);
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 80.0, 750.0, 'b', "if97_r3_pt_subregion", logFile);
	
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 20.0, 630.0, 'c', "if97_r3_pt_subregion", logFile);
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 30.0, 650.0, 'c', "if97_r3_pt_subregion", logFile);
	
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 26.0, 656.0, 'd', "if97_r3_pt_subregion", logFile);
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 30.0, 670.0, 'd', "if97_r3_pt_subregion", logFile);
	
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 26.0, 661.0, 'e', "if97_r3_pt_subregion", logFile);
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 30.0, 675.0, 'e', "if97_r3_pt_subregion", logFile);
	
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 26.0, 671.0, 'f', "if97_r3_pt_subregion", logFile);
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 30.0, 690.0, 'f', "if97_r3_pt_subregion", logFile);
	
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 23.6, 649.0, 'g', "if97_r3_pt_subregion", logFile);
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 24.0, 650.0, 'g', "if97_r3_pt_subregion", logFile);
	
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 23.6, 652.0, 'h', "if97_r3_pt_subregion", logFile);
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 24.0, 654.0, 'h', "if97_r3_pt_subregion", logFile);
	
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 23.6, 653.0, 'i', "if97_r3_pt_subregion", logFile);
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 24.0, 655.0, 'i', "if97_r3_pt_subregion", logFile);
	
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 23.5, 655.0, 'j', "if97_r3_pt_subregion", logFile);
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 24.0, 660.0, 'j', "if97_r3_pt_subregion", logFile);
	
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 23.0, 660.0, 'k', "if97_r3_pt_subregion", logFile);
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 24.0, 670.0, 'k', "if97_r3_pt_subregion", logFile);
	
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 22.6, 646.0, 'l', "if97_r3_pt_subregion", logFile);
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 23.0, 646.0, 'l', "if97_r3_pt_subregion", logFile);
	
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 22.6, 648.6, 'm', "if97_r3_pt_subregion", logFile);
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 22.8, 649.3, 'm', "if97_r3_pt_subregion", logFile);
	
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 22.6, 649.0, 'n', "if97_r3_pt_subregion", logFile);
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 22.8, 649.7, 'n', "if97_r3_pt_subregion", logFile);
	
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 22.6, 649.1, 'o', "if97_r3_pt_subregion", logFile);
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 22.8, 649.9, 'o', "if97_r3_pt_subregion", logFile);

	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 22.6, 649.4, 'p', "if97_r3_pt_subregion", logFile);
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 22.8, 650.2, 'p', "if97_r3_pt_subregion", logFile);

	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 21.1, 640.0, 'q', "if97_r3_pt_subregion", logFile);
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 21.8, 643.0, 'q', "if97_r3_pt_subregion", logFile);

	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 21.1, 644.0, 'r', "if97_r3_pt_subregion", logFile);
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 21.8, 648.0, 'r', "if97_r3_pt_subregion", logFile);

	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 19.1, 635.0, 's', "if97_r3_pt_subregion", logFile);
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 20.0, 638.0, 's', "if97_r3_pt_subregion", logFile);

	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 17.0, 626.0, 't', "if97_r3_pt_subregion", logFile);
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 20.0, 640.0, 't', "if97_r3_pt_subregion", logFile);

	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 21.5, 644.6, 'u', "if97_r3_pt_subregion", logFile);
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 22.0, 646.1, 'u', "if97_r3_pt_subregion", logFile);

	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 22.5, 648.6, 'v', "if97_r3_pt_subregion", logFile);
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 22.3, 647.9, 'v', "if97_r3_pt_subregion", logFile);

	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 22.15, 647.5, 'w', "if97_r3_pt_subregion", logFile);
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 22.3, 648.1, 'w', "if97_r3_pt_subregion", logFile);

	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 22.11, 648.0, 'x', "if97_r3_pt_subregion", logFile);
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 22.3, 649.0, 'x', "if97_r3_pt_subregion", logFile);

	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 22.0, 646.84, 'y', "if97_r3_pt_subregion", logFile);
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 22.064, 647.05, 'y', "if97_r3_pt_subregion", logFile);	

	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 22.0, 646.89, 'z', "if97_r3_pt_subregion", logFile);
	intermediateResult = intermediateResult | testCharDoubleInput ( if97_r3_pt_subregion, 22.064, 647.15, 'z', "if97_r3_pt_subregion", logFile);
	
	fprintf(logFile, "\n\n*** IF97 REGION 3 BACKWARDS v(P, T) CHECK ***\n\n" );
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_R3bw_v_pt, 50.0, 630.0, 1.470853100e-3, TEST_ACCURACY, SIG_FIG, "if97_R3bw_v_pt", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_R3bw_v_pt, 80.0, 670.0, 1.503831359e-3, TEST_ACCURACY, SIG_FIG, "if97_R3bw_v_pt", logFile);
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_R3bw_v_pt, 50.0, 710.0, 2.204728587e-3, TEST_ACCURACY, SIG_FIG, "if97_R3bw_v_pt", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_R3bw_v_pt, 80.0, 750.0, 1.973692940e-3, TEST_ACCURACY, SIG_FIG, "if97_R3bw_v_pt", logFile);
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_R3bw_v_pt, 22.0, 646.84, 2.698354719e-3, TEST_ACCURACY, SIG_FIG, "if97_R3bw_v_pt", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_R3bw_v_pt, 22.064, 647.05, 2.717655648e-3, TEST_ACCURACY, SIG_FIG, "if97_R3bw_v_pt", logFile);	

	intermediateResult = intermediateResult | testDoubleInput ( if97_R3bw_v_pt, 22.0, 646.89, 3.798732962e-3, TEST_ACCURACY, SIG_FIG, "if97_R3bw_v_pt", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_R3bw_v_pt, 22.064, 647.15, 3.701940010e-3, TEST_ACCURACY, SIG_FIG, "if97_R3bw_v_pt", logFile);	

	
	if (intermediateResult != 0)
		intermediateResult= intermediateResult | TEST_FAIL;
	return intermediateResult;
}
