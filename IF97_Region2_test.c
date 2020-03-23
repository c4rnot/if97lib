//          Copyright Martin Lord 2014-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)



/* *****************************************************************************
* A SHORT PROGRAMME TO CHECK REGION 2 PROPERTY EQUATIONS AND 1 BACKWARDS EQUATIONS
* *******************************************************************************/


#include "IF97_Region2.h"
#include "IF97_Region2_met.h"
#include <stdio.h>



int if97_region2_test (FILE *logFile){	
	int intermediateResult= TEST_PASS; //initialise with clear flags.  
	
	fprintf(logFile, "\n\n*** IF97 REGION 2 MODULE CHECK ***\n" );
	

	
/* PROPERY (FORWARDS) EQUATIONS
* see IAPWS-IF97 Table 15
*
* The results should be as follows
*
*				T= 300 K				T= 700 K				T= 700 K
*				p= 0.0035 MPa			p= 0.0035 MPa			p= 30 MPa
* g  (kJ/kg)	?? 						??						??				Using Freesteam
* v  (m3/kg) 	3.94913866e01			9.23015898e01		5.42946619e-3
* h  (kJ/kg)	2.54991145e03			3.33568375e03		2.63149474e03
* u  (kJ/kg)	2.41169160e03			3.01262819e03		2.46861076e03
* s  (kJ/kg.K)	8.52238967e00			1.01749996e01		5.17540298e00
* Cp (kJ/kg.K)	1.91300162e00			2.08141274e00		1.03505092e01
* Cv (kJ/kg.K)	1.441326618975			1.6197833256		2.975538368909				From Moscow Power Institute check page
* w  (m/s)		4.27920172e02			6.44289068e02		4.80386523e02

* see http://twt.mpei.ac.ru/mcs/worksheets/iapws/IAPWS-IF97-Region2.xmcd
*/

	fprintf(logFile, "*** IF97 REGION 2 PROPERTY (FORWARDS) EQUATIONS CHECK ***\n\n" );
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2_g, 0.0035, 300.0, -1, TEST_ACCURACY, SIG_FIG, "if97_r2_g", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2_g, 0.0035, 700.0, -1, TEST_ACCURACY, SIG_FIG, "if97_r2_g", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2_g, 30.0, 700.0, -1, TEST_ACCURACY, SIG_FIG, "if97_r2_g", logFile);
	fprintf(logFile, "TODO Determine correct gibbs free energies for region 2 to check against\n\n");

	intermediateResult = intermediateResult | testDoubleInput ( if97_r2_v, 0.0035, 300.0, 3.94913866e01, TEST_ACCURACY, SIG_FIG, "if97_r2_v", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2_v, 0.0035, 700.0, 9.23015898e01, TEST_ACCURACY, SIG_FIG, "if97_r2_v", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2_v, 30.0, 700.0, 5.42946619e-3, TEST_ACCURACY, SIG_FIG, "if97_r2_v", logFile);
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2_h, 0.0035, 300.0, 2.54991145e03, TEST_ACCURACY, SIG_FIG, "if97_r2_h", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2_h, 0.0035, 700.0, 3.33568375e03, TEST_ACCURACY, SIG_FIG, "if97_r2_h", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2_h, 30.0, 700.0, 2.63149474e03, TEST_ACCURACY, SIG_FIG, "if97_r2_h", logFile);	

	intermediateResult = intermediateResult | testDoubleInput ( if97_r2_u, 0.0035, 300.0, 2.41169160e03, TEST_ACCURACY, SIG_FIG, "if97_r2_u", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2_u, 0.0035, 700.0, 3.01262819e03, TEST_ACCURACY, SIG_FIG, "if97_r2_u", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2_u, 30.0, 700.0, 2.46861076e03, TEST_ACCURACY, SIG_FIG, "if97_r2_u", logFile);

	intermediateResult = intermediateResult | testDoubleInput ( if97_r2_s, 0.0035, 300.0, 8.52238967e00, TEST_ACCURACY, SIG_FIG, "if97_r2_s", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2_s, 0.0035, 700.0, 1.01749996e01, TEST_ACCURACY, SIG_FIG, "if97_r2_s", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2_s, 30.0, 700.0, 5.17540298e00, TEST_ACCURACY, SIG_FIG, "if97_r2_s", logFile);
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2_Cp, 0.0035, 300.0, 1.91300162e00, TEST_ACCURACY, SIG_FIG, "if97_r2_Cp", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2_Cp, 0.0035, 700.0, 2.08141274e00, TEST_ACCURACY, SIG_FIG, "if97_r2_Cp", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2_Cp, 30.0, 700.0, 1.03505092e01, TEST_ACCURACY, SIG_FIG, "if97_r2_Cp", logFile);
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2_Cv, 0.0035, 300.0, 1.441326618975, TEST_ACCURACY, SIG_FIG, "if97_r2_Cv", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2_Cv, 0.0035, 700.0, 1.6197833256, TEST_ACCURACY, SIG_FIG, "if97_r2_Cv", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2_Cv, 30.0, 700.0, 2.975538368909, TEST_ACCURACY, SIG_FIG, "if97_r2_Cv", logFile);	
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2_w, 0.0035, 300.0, 4.27920172e02, TEST_ACCURACY, SIG_FIG, "if97_r2_w", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2_w, 0.0035, 700.0, 6.44289068e02, TEST_ACCURACY, SIG_FIG, "if97_r2_w", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2_w, 30.0, 700.0, 4.80386523e02, TEST_ACCURACY, SIG_FIG, "if97_r2_w", logFile);

	

	
/* PROPERY (FORWARDS) EQUATIONS: Metastable Region
* see IAPWS-IF97 Table 18
*
* The results should be as follows
*
*				T= 450 K			T= 440 K			T= 450 K
*				p= 1 MPa			p= 1 MPa			p= 1.5 MPa
* g  (kJ/kg)	?? 					??			 		??
* v  (m3/kg)	0.192516540			0.186212297			0.121685206
* h  (kJ/kg)	2.76881115e03		2.74015123e03		2.72134539e03
* u  (kJ/kg)	2.57629461e03		2.55393894e03		2.53881758e03
* s  (kJ/kg.K)	6.56660377e00		6.50218759e00		6.29170440e00
* Cp (kJ/kg.K)	2.76349265e00		2.98166443e00		3.62795578e00
* Cv (kJ/kg.K)	1.95830730e00		2.08622142e00		2.41213708e00	From Moscow Power Institute
* w  (m/s)		4.98408101e02		4.89363295e02		4.81941819e02


* see http://twt.mpei.ac.ru/mcs/worksheets/iapws/IAPWS-IF97-metastable-vapor.xmcd
*/

	fprintf(logFile, "\n\n*** IF97 REGION 2 METASTABLE PROPERTY (FORWARDS) EQUATIONS CHECK ***\n\n" );

	intermediateResult = intermediateResult | testDoubleInput ( if97_r2met_g, 1.0,  450.0 , -1, TEST_ACCURACY, SIG_FIG, "if97_r2met_g", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2met_g, 1.0,  440.0 , -1, TEST_ACCURACY, SIG_FIG, "if97_r2met_g", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2met_g, 1.5,  450.0 , -1, TEST_ACCURACY, SIG_FIG, "if97_r2met_g", logFile);
	fprintf(logFile, "TODO Determine correct gibbs free energies for region 2 to check against\n\n");
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2met_v, 1.0,  450.0 , 0.192516540, TEST_ACCURACY, SIG_FIG, "if97_r2met_v", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2met_v, 1.0,  440.0 , 0.186212297, TEST_ACCURACY, SIG_FIG, "if97_r2met_v", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2met_v, 1.5,  450.0 , 0.121685206, TEST_ACCURACY, SIG_FIG, "if97_r2met_v", logFile);

	intermediateResult = intermediateResult | testDoubleInput ( if97_r2met_h, 1.0,  450.0 , 2.76881115e03, TEST_ACCURACY, SIG_FIG, "if97_r2met_h", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2met_h, 1.0,  440.0 , 2.74015123e03, TEST_ACCURACY, SIG_FIG, "if97_r2met_h", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2met_h, 1.5,  450.0 , 2.72134539e03, TEST_ACCURACY, SIG_FIG, "if97_r2met_h", logFile);	

	intermediateResult = intermediateResult | testDoubleInput ( if97_r2met_u, 1.0,  450.0 , 2.57629461e03, TEST_ACCURACY, SIG_FIG, "if97_r2met_u", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2met_u, 1.0,  440.0 , 2.55393894e03, TEST_ACCURACY, SIG_FIG, "if97_r2met_u", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2met_u, 1.5,  450.0 , 2.53881758e03, TEST_ACCURACY, SIG_FIG, "if97_r2met_u", logFile);
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2met_s, 1.0,  450.0 , 6.56660377e00, TEST_ACCURACY, SIG_FIG, "if97_r2met_s", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2met_s, 1.0,  440.0 , 6.50218759e00, TEST_ACCURACY, SIG_FIG, "if97_r2met_s", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2met_s, 1.5,  450.0 , 6.29170440e00, TEST_ACCURACY, SIG_FIG, "if97_r2met_s", logFile);
			
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2met_Cp, 1.0,  450.0 , 2.76349265e00, TEST_ACCURACY, SIG_FIG, "if97_r2met_Cp", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2met_Cp, 1.0,  440.0 , 2.98166443e00, TEST_ACCURACY, SIG_FIG, "if97_r2met_Cp", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2met_Cp, 1.5,  450.0 , 3.62795578e00, TEST_ACCURACY, SIG_FIG, "if97_r2met_Cp", logFile);	
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2met_Cv, 1.0,  450.0 , 1.95830730e00		, TEST_ACCURACY, SIG_FIG, "if97_r2met_Cv", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2met_Cv, 1.0,  440.0 , 2.08622142e00, TEST_ACCURACY, SIG_FIG, "if97_r2met_Cv", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2met_Cv, 1.5,  450.0 , 2.41213708e00, TEST_ACCURACY, SIG_FIG, "if97_r2met_Cv", logFile);	

	intermediateResult = intermediateResult | testDoubleInput ( if97_r2met_w, 1.0,  450.0 , 4.98408101e02, TEST_ACCURACY, SIG_FIG, "if97_r2met_w", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2met_w, 1.0,  440.0 , 4.89363295e02, TEST_ACCURACY, SIG_FIG, "if97_r2met_w", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2met_w, 1.5,  450.0 , 4.81941819e02, TEST_ACCURACY, SIG_FIG, "if97_r2met_w", logFile);	
	



	
// B2bc Boundary Equation check
// For verification the equations below must meet the following p-h point
// see IAPWS-IF97 paragraph following Table 19
//
// p= 1.00000000 E2 MPa
// h= 3.516004323 E3 kJ/kg

	fprintf(logFile, "*** IF97 REGION 2b -2c BOUNDARY FUNCTION CHECK  ***\n\n" );
	
	intermediateResult = intermediateResult | testSingleInput ( IF97_B2bc_p, 3.516004323E3, 1.00000000E2, TEST_ACCURACY, SIG_FIG, "IF97_B2bc_p", logFile);
	intermediateResult = intermediateResult | testSingleInput ( IF97_B2bc_h, 1.00000000E2, 3.516004323E3, TEST_ACCURACY, SIG_FIG, "IF97_B2bc_h", logFile);
	



/* Region 2 Backwards Equations T(p,h) check:
* see IAPWS-IF97 Table 24
*
* The results should be as follows
*
*	Region		p(MPa)			h(kJ/kg)		T(K)
*	-------------------------------------------------
*	2a			0.001			3000			5.34 433 241 E02
*	2a			3				3000			5.75 373 370 E02
*	2a			3				4000			1.01 077 577 E03
*	-------------------------------------------------
*	2b			5				3500			8.01 299 102 E02
*	2b			5				4000			1.01 531 583 E03
*	2b			25				3500			8.75 279 054 E02
*	-------------------------------------------------
*	2c			40				2700			7.43 056 411 E02
*	2c			60				2700			7.91 137 067 E02
*	2c			60				3200			8.82 756 860 E02
*/

	fprintf(logFile, "\n\n*** IF97 REGION 2 T(p, h) BACKWARDS EQUATIONS CHECK  ***\n\n" );

	intermediateResult = intermediateResult | testDoubleInput ( if97_r2a_t_ph, 0.001,  3000.0,  5.34433241E02, TEST_ACCURACY, SIG_FIG, "if97_r2a_t_ph", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2a_t_ph, 3.0,  3000.0, 5.75373370E02, TEST_ACCURACY, SIG_FIG, "if97_r2a_t_ph", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2a_t_ph, 3.0,  4000.0, 1.01077577E03, TEST_ACCURACY, SIG_FIG, "if97_r2a_t_ph", logFile);
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2b_t_ph, 5.0,  3500.0,  8.01299102E02, TEST_ACCURACY, SIG_FIG, "if97_r2b_t_ph", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2b_t_ph, 5.0,  4000.0, 1.01531583E03, TEST_ACCURACY, SIG_FIG, "if97_r2b_t_ph", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2b_t_ph, 25.0,  3500.0, 8.75279054E02, TEST_ACCURACY, SIG_FIG, "if97_r2b_t_ph", logFile);
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2c_t_ph, 40.0,  2700.0,  7.43056411E02, TEST_ACCURACY, SIG_FIG, "if97_r2c_t_ph", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2c_t_ph, 60.0,  2700.0, 7.91137067E02, TEST_ACCURACY, SIG_FIG, "if97_r2c_t_ph", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2c_t_ph, 60.0,  3200.0, 8.82756860E02, TEST_ACCURACY, SIG_FIG, "if97_r2c_t_ph", logFile);
	



/* Region 2 Backwards Equations T(p,s) check:
* see IAPWS-IF97 Table 29
*
* The results should be as follows
*
*	Region		p(MPa)		s(kJ/kg.K)		T(K)
*	-------------------------------------------------
*	2a			0.1			7.5				3.99 517 097 E02
*	2a			0.1			8				5.14 127 081 E02
*	2a			2.5			8				1.03 984 917 E03
*	-------------------------------------------------
*	2b			8			6				6.00 484 040 E02
*	2b			8			7.5				1.06 495 556 E03
*	2b			90			6				1.03 801 126 E03
*	-------------------------------------------------
*	2c			20			5.75			6.97 992 849 E02
*	2c			80			5.25			8.54 011 484 E02
*	2c			80			5.75			9.49 017 998 E02
*/

	fprintf(logFile, "\n\n*** IF97 REGION 2 T(p, s) BACKWARDS EQUATIONS CHECK  ***\n\n" );
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2a_t_ps, 0.1,  7.5,  3.99517097E02, TEST_ACCURACY, SIG_FIG, "if97_r2a_t_ps", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2a_t_ps, 0.1,  8.0, 5.14127081E02, TEST_ACCURACY, SIG_FIG, "if97_r2a_t_ps", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2a_t_ps, 2.5,  8.0, 1.03984917E03, TEST_ACCURACY, SIG_FIG, "if97_r2a_t_ps", logFile);
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2b_t_ps, 8.0,  6.0,  6.00484040E02, TEST_ACCURACY, SIG_FIG, "if97_r2b_t_ps", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2b_t_ps, 8.0,  7.5, 1.06495556E03, TEST_ACCURACY, SIG_FIG, "if97_r2b_t_ps", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2b_t_ps, 90.0,  6.0, 1.03801126E03, TEST_ACCURACY, SIG_FIG, "if97_r2b_t_ps", logFile);
	
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2c_t_ps, 20.0,  5.75,  6.97992849E02, TEST_ACCURACY, SIG_FIG, "if97_r2c_t_ps", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2c_t_ps, 80.0,  5.25, 8.54011484E02, TEST_ACCURACY, SIG_FIG, "if97_r2c_t_ps", logFile);
	intermediateResult = intermediateResult | testDoubleInput ( if97_r2c_t_ps, 80.0,  5.75, 9.49017998E02, TEST_ACCURACY, SIG_FIG, "if97_r2c_t_ps", logFile);


	if (intermediateResult != 0)
		intermediateResult= intermediateResult | TEST_FAIL;
	return intermediateResult;
}

















