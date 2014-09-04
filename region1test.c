
//          Copyright Martin Lord 2014-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)



/* *****************************************************************************
* A SHORT PROGRAMME TO CHECK REGION 1 PROPERTY EQUATIONS AND 1 BACKWARDS EQUATIONS
* *******************************************************************************/


#include "IF97_Region1.h"
#include <stdio.h>


int main (int argc, char **argv)
{

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

*/
	
	printf ("\n\nIF97 REGION 1 PROPERTY (FORWARDS) EQUATIONS CHECK \n\n" );
	printf ("The output should be the following: \n\n");
	printf ("\t\tT= 300 K \tT= 300 K \tT= 500 K\n");
	printf ("\t\tp= 3 MPa \tp= 80M Pa \tp= 3 MPa\n\n");

	printf ("g  (kJ/kg)\t-2.35716470e+00\t7.35736720e+01\t-3.14667321e+02\tUsing freesteam\n");	
	printf ("v  (m3/kg)\t1.00215168e-3\t9.71180894e-4\t1.20241800e-3\n");
	printf ("h  (kJ/kg)\t1.15331273e2\t1.84142828e2\t9.75542239e2\n");
	printf ("u  (kJ/kg)\t1.12324818e2\t1.06448356e2\t9.71934985e2\n");
	printf ("s  (kJ/kg.K)\t3.92294792e-1\t3.68563852e-1\t2.58041912e0\n");
	printf ("Cp (kJ/kg.K)\t4.17301218e0\t4.01008987e0\t4.65580682e0\n");
	printf ("Cv (kJ/kg.K)\t4.12120160e0\t3.91736606e0\t3.22139223e0\tFrom Moscow Power Institute check page\n");
	printf ("w  (m/s)\t1.50773921e3\t1.63469054e3\t1.24071337e3\n\n*******OUTPUT************\n");
	
	
	printf ("g  (kJ/kg)\t%.8e\t", if97_r1_g(3.0, 300.0) );
		printf("%.8e\t", if97_r1_g(80.0, 300.0) );
			printf("%.8e\t\n", if97_r1_g(3.0, 500.0) );

	printf ("v  (m3/kg)\t%.8e\t", if97_r1_v(3.0, 300.0) );
		printf("%.8e\t", if97_r1_v(80.0, 300.0) );
			printf("%.8e\t\n", if97_r1_v(3.0, 500.0) );
			
	printf ("h  (kJ/kg)\t%.8e\t", if97_r1_h(3.0, 300.0) );
		printf("%.8e\t", if97_r1_h(80.0, 300.0) );
			printf("%.8e\t\n", if97_r1_h(3.0, 500.0) );
			
	printf ("u  (kJ/kg)\t%.8e\t", if97_r1_u(3.0, 300.0) );
		printf("%.8e\t", if97_r1_u(80.0, 300.0) );
			printf("%.8e\t\n", if97_r1_u(3.0, 500.0) );		
	
	printf ("s  (kJ/kg.K)\t%.8e\t", if97_r1_s(3.0, 300.0) );
		printf("%.8e\t", if97_r1_s(80.0, 300.0) );
			printf("%.8e\t\n", if97_r1_s(3.0, 500.0) );			
	
	printf ("Cp  (kJ/kg.K)\t%.8e\t", if97_r1_Cp(3.0, 300.0) );
		printf("%.8e\t", if97_r1_Cp(80.0, 300.0) );
			printf("%.8e\t\n", if97_r1_Cp(3.0, 500.0) );		

	printf ("Cv  (kJ/kg.K)\t%.8e\t", if97_r1_Cv(3.0, 300.0) );
		printf("%.8e\t", if97_r1_Cv(80.0, 300.0) );
			printf("%.8e\t\n", if97_r1_Cv(3.0, 500.0) );		
	
	printf ("w  (m/s)\t%.8e\t", if97_r1_w(3.0, 300.0) );
		printf("%.8e\t", if97_r1_w(80.0, 300.0) );
			printf("%.8e\t\n", if97_r1_w(3.0, 500.0) );		
	
	


	
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
	
	printf ("\n\nIF97 REGION 1 BACKWARDS EQUATIONS CHECK: T (p, h) \n\n" );
	printf ("The output should be the following: \n\n");
	printf ("p (MPa)	h (kj/kg)	T (K)\n");
	printf ("3	500	391.798509\n");	
	printf ("80	500	378.108626\n");	
	printf ("80	1500	611.041229\n\n**********RESULTS******\n");
	
	printf ("p (MPa)	h (kj/kg)	T (K)\n");
	printf ("3	500	%.6f\n", if97_r1_t_ph(3.0, 500.0));	
	printf ("80	500	%.6f\n", if97_r1_t_ph(80.0, 500.0));	
	printf ("80	1500	%.6f\n\n", if97_r1_t_ph(80.0, 1500.0));
	
	
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
	
	
	printf ("\n\nIF97 REGION 1 BACKWARDS EQUATIONS CHECK: T (p, s) \n\n" );
	printf ("The output should be the following: \n\n");
	printf ("p (MPa)	s (kj/kg.K)	T (K)\n");
	printf ("3	0.5	307.842258\n");	
	printf ("80	0.5	309.979785\n");	
	printf ("80	3	565.899909\n\n**********RESULTS******\n");
	
	printf ("p (MPa)	s (kj/kg.K)	T (K)\n");
	printf ("3	0.5	%.6f\n", if97_r1_t_ps(3.0, 0.5));	
	printf ("80	0.5	%.6f\n", if97_r1_t_ps(80.0, 0.5));	
	printf ("80	3	%.6f\n\n", if97_r1_t_ps(80.0, 3.0));
	
	return 0;
}

