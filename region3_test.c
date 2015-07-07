
//          Copyright Martin Lord 2014-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)



/* *****************************************************************************
* A SHORT PROGRAMME TO CHECK REGION 3 PROPERTY EQUATIONS 
* *******************************************************************************/


#include "IF97_Region3.h"
#include <stdio.h>


int main (int argc, char **argv)
{

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
	
	printf ("\n\nIF97 REGION 3 PROPERTY (FORWARDS) EQUATIONS CHECK \n\n" );
	printf ("The output should be the following: \n\n");
	printf ("\t\tT= 650 K \tT= 650 K \tT= 750 K\n");
	printf ("\t\trho= 500 kg/m3 \trho= 200 kg/m3\trho= 500 kg/m3\n\n");
	
	printf ("hholz (kJ/kg)\t??\t??\t??\tUsing freesteam\n");
	printf ("P  (MPa)\t2.55837018e01\t2.22930643e01\t7.83095639e01\n");	
	printf ("h  (kJ/kg)\t1.86343019e03\t2.37512401e03\t2.25868845e03\n");
	printf ("u  (kJ/kg)\t1.81226279e03\t2.26365868e03\t2.10206932e03\n");
	printf ("s  (kJ/kg.K)\t4.05427273e00\t4.85438792e00\t4.46971906e00\n");
	printf ("Cp (kJ/kg.K)\t1.38935717e01\t4.46579342e01\t6.34165359e00\n");
	printf ("Cv (kJ/kg.K)\t3.19131787e00\t4.04118076e00\t2.71701677e00\tFrom Moscow Power Institute check page\n");
	printf ("w  (m/s)\t5.02005554e02\t3.83444594e02\t7.60696041e02\n\n*******OUTPUT************\n");
	
	
	printf ("hholz (kJ/kg)\t%.8e\t", if97_r3_hhz(500.0, 650.0) );
		printf("%.8e\t", if97_r3_hhz(200.0, 650.0) );
			printf("%.8e\t\n", if97_r3_hhz(500.0, 750.0) );

	printf ("P  (MPa)\t%.8e\t", if97_r3_p(500.0, 650.0) );
		printf("%.8e\t", if97_r3_p(200.0, 650.0) );
			printf("%.8e\t\n", if97_r3_p(500.0, 750.0) );
			
	printf ("h  (kJ/kg)\t%.8e\t", if97_r3_h(500.0, 650.0) );
		printf("%.8e\t", if97_r3_h(200.0, 650.0) );
			printf("%.8e\t\n", if97_r3_h(500.0, 750.0) );
			
	printf ("u  (kJ/kg)\t%.8e\t", if97_r3_u(500.0, 650.0) );
		printf("%.8e\t", if97_r3_u(200.0, 650.0) );
			printf("%.8e\t\n", if97_r3_u(500.0, 750.0) );		
	
	printf ("s  (kJ/kg.K)\t%.8e\t", if97_r3_s(500.0, 650.0) );
		printf("%.8e\t", if97_r3_s(200.0, 650.0) );
			printf("%.8e\t\n", if97_r3_s(500.0, 750.0) );			
	
	printf ("Cp  (kJ/kg.K)\t%.8e\t", if97_r3_Cp(500.0, 650.0) );
		printf("%.8e\t", if97_r3_Cp(200.0, 650.0) );
			printf("%.8e\t\n", if97_r3_Cp(500.0, 750.0) );		

	printf ("Cv  (kJ/kg.K)\t%.8e\t", if97_r3_Cv(500.0, 650.0) );
		printf("%.8e\t", if97_r3_Cv(200.0, 650.0) );
			printf("%.8e\t\n", if97_r3_Cv(500.0, 750.0) );		
	
	printf ("w  (m/s)\t%.8e\t", if97_r3_w(500.0, 650.0) );
		printf("%.8e\t", if97_r3_w(200.0, 650.0) );
			printf("%.8e\t\n", if97_r3_w(500.0, 750.0) );		
	
	
	
	
	
	/* BACKWARDS v (p, t)  EQUATIONS
*/
	printf ("\n\nIF97 REGION 3 BACKWARDS v(p,T) SUBREGION BOUNDARIES CHECK \n" );
	printf ("\nTable 3 of supplementary release for v(p, T) for region3: June 2014\n\n" );
	printf ("The output should be the following: \n\n");
	printf ("Equation\tP (MPa)\tT (Kelvin)\n");

	printf ("T3ab(p)\t40\t6.930341408e02\n");
	printf ("T3cd(p)\t25\t6.493659208e02\n");
	printf ("T3ef(p)\t40\t7.139593992e02\n");
	printf ("T3gh(p)\t23\t6.498873759e02\n");
	printf ("T3ij(p)\t23\t6.515778091e02\n");
	
	printf ("T3jk(p)\t23\t6.558338344e02\n");	
	printf ("T3mn(p)\t22.8\t6.496054133e02\n");	
	printf ("T3op(p)\t22.8\t6.500106943e02\n");	
	printf ("T3qu(p)\t22\t6.456355027e02\n");	
	printf ("T3rx(p)\t22\t6.482622754e02\n");
	
	printf ("\n\n*******OUTPUT************\n");
	printf ("Equation\tP (MPa)\tT (Kelvin)\n");	
	
	printf ("T3ab(p)\t40\t%.9e\n", if97_r3ab_p_t(40.0));	
	printf ("T3cd(p)\t25\t%.9e\n", if97_r3cd_p_t(25.0));		
	printf ("T3ef(p)\t40\t%.9e\n", if97_r3ef_p_t(40.0));		
	printf ("T3gh(p)\t23\t%.9e\n", if97_r3gh_p_t(23.0));
	printf ("T3ij(p)\t23\t%.9e\n", if97_r3ij_p_t(23.0));	
	
	printf ("T3jk(p)\t23\t%.9e\n", if97_r3jk_p_t(23.0));	
	printf ("T3mn(p)\t22.8\t%.9e\n", if97_r3mn_p_t(22.8));	
	printf ("T3op(p)\t22.8\t%.9e\n", if97_r3op_p_t(22.8));	
	printf ("T3qu(p)\t22\t%.9e\n", if97_r3qu_p_t(22.0));	
	printf ("T3rx(p)\t22\t%.9e\n", if97_r3rx_p_t(22.0));




	printf ("\n\nIF97 REGION 3 BACKWARDS v(p,T) CHECK \n" );
	printf ("\nTable 3 of supplementary release for v(p, T) for region3: June 2014\n\n" );
	printf ("The output should be the following: \n\n");
	printf ("Equation\tP (MPa)\tT (Kelvin)\tv (m3/kg\n");
	printf ("v3a(p,t)\t50\t630\t1.470853100e-3\n");	
	printf ("v3a(p,t)\t80\t670\t1.503831359e-3\n");	
	printf ("-----------------------------------------\n");	
	printf ("v3b(p,t)\t50\t710\t2.204728587e-3\n");	
	printf ("v3b(p,t)\t80\t750\t1.973692940e-3\n");	
	printf ("-----------------------------------------\n");	
	printf ("v3c(p,t)\t20\t630\t1.761696406e-3\n");	
	printf ("v3c(p,t)\t30\t650\t1.819560617e-3\n");	
	printf ("-----------------------------------------\n");	
	printf ("v3d(p,t)\t20\t656\t2.245587720e-3\n");	
	printf ("v3d(p,t)\t30\t670\t2.506897702e-3\n");	
	printf ("-----------------------------------------\n");	


	printf ("\n\n*******OUTPUT************\n");
	printf ("Equation\tP (MPa)\tT (Kelvin)\tv (m3/kg\n");
	printf ("v3a(p,t)\t50\t630\t%.9e\n", if97_r3a_v_pt(50.0, 630.0));	
	printf ("v3a(p,t)\t80\t670\t%.9e\n", if97_r3a_v_pt(80.0, 670.0));
	printf ("-----------------------------------------\n");	
	printf ("v3b(p,t)\t50\t710\t%.9e\n", if97_r3b_v_pt(50.0, 710.0));	
	printf ("v3b(p,t)\t80\t750\t%.9e\n", if97_r3b_v_pt(80.0, 750.0));
	printf ("-----------------------------------------\n");
	printf ("v3c(p,t)\t20\t630\t%.9e\n", if97_r3c_v_pt(20.0, 630.0));	
	printf ("v3c(p,t)\t30\t650\t%.9e\n", if97_r3c_v_pt(30.0, 650.0));
	printf ("-----------------------------------------\n");
	printf ("v3d(p,t)\t26\t656\t%.9e\n", if97_r3d_v_pt(26.0, 656.0));	
	printf ("v3d(p,t)\t30\t670\t%.9e\n", if97_r3d_v_pt(30.0, 670.0));

	return 0;
}

