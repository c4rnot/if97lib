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



int main (int argc, char **argv)
{

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

	printf ("\n\n************************************************************ \n" );
	printf ("IF97 REGION 2 PROPERTY (FORWARDS) EQUATIONS CHECK \n\n" );
	printf ("The output should be the following: \n\n");
	printf ("\t\tT= 300 K \tT= 700 K \tT= 700 K\n");
	printf ("\t\tp= 0.0035 MPa \tp= 0.0035 MPa \tp= 30 MPa\n\n");

	printf ("g  (kJ/kg)\t?????????\t?????????\t???????????\t(1)\n");	
	printf ("v  (m3/kg)\t3.94913866e01\t9.23015898e01\t5.42946619e-3\n");
	printf ("h  (kJ/kg)\t2.54991145e03\t3.33568375e03\t2.63149474e03\n");
	printf ("u  (kJ/kg)\t2.41169160e03\t3.01262819e03\t2.46861076e03\n");
	printf ("s  (kJ/kg.K)\t8.52238967e00\t1.01749996e01\t5.17540298e00\n");
	printf ("Cp (kJ/kg.K)\t1.91300162e00\t2.08141274e00\t1.03505092e01\n");
	printf ("Cv (kJ/kg.K)\t1.44132662e00\t1.61978333e00\t2.97553837e00\t(2)\n");
	printf ("w  (m/s)\t4.27920172e02\t6.44289068e02\t4.80386523e02\n\n");
	
	printf ("\n(1) Using Freesteam \n(2) Using Moscow Power Institute calculation \n    http://twt.mpei.ac.ru/mcs/worksheets/iapws/IAPWS-IF97-metastable-vapor.xmcd\n\n");
	printf ("*******OUTPUT************\n");

	printf ("g  (kJ/kg)\t%.8e\t", if97_r2_g(0.0035, 300.0) );
		printf("%.8e\t", if97_r2_g(0.0035, 700.0) );
			printf("%.8e\t\n", if97_r2_g(30.0, 700.0) );

	printf ("v  (m3/kg)\t%.8e\t", if97_r2_v(0.0035, 300.0) );
		printf("%.8e\t", if97_r2_v(0.0035, 700.0) );
			printf("%.8e\t\n", if97_r2_v(30.0, 700.0) );
			
	printf ("h  (kJ/kg)\t%.8e\t", if97_r2_h(0.0035, 300.0) );
		printf("%.8e\t", if97_r2_h(0.0035, 700.0) );
			printf("%.8e\t\n", if97_r2_h(30.0, 700.0) );
			
	printf ("u  (kJ/kg)\t%.8e\t", if97_r2_u(0.0035, 300.0) );
		printf("%.8e\t", if97_r2_u(0.0035, 700.0) );
			printf("%.8e\t\n", if97_r2_u(30.0, 700.0) );		
	
	printf ("s  (kJ/kg.K)\t%.8e\t", if97_r2_s(0.0035, 300.0) );
		printf("%.8e\t", if97_r2_s(0.0035, 700.0) );
			printf("%.8e\t\n", if97_r2_s(30.0, 700.0) );			
	
	printf ("Cp  (kJ/kg.K)\t%.8e\t", if97_r2_Cp(0.0035, 300.0) );
		printf("%.8e\t", if97_r2_Cp(0.0035, 700.0) );
			printf("%.8e\t\n", if97_r2_Cp(30.0, 700.0) );		

	printf ("Cv  (kJ/kg.K)\t%.8e\t", if97_r2_Cv(0.0035, 300.0) );
		printf("%.8e\t", if97_r2_Cv(0.0035, 700.0) );
			printf("%.8e\t\n", if97_r2_Cv(30.0, 700.0) );		
	
	printf ("w  (m/s)\t%.8e\t", if97_r2_w(0.0035, 300.0) );
		printf("%.8e\t", if97_r2_w(0.0035, 700.0) );
			printf("%.8e\t\n", if97_r2_w(30.0, 700.0) );		
	
	
	
	
	
/* PROPERY (FORWARDS) EQUATIONS: Metastable Region
* see IAPWS-IF97 Table 18
*
* The results should be as follows
*
*				T= 450 K			T= 440 K			T= 450 K
*				p= 1 MPa			p= 1 MPa			p= 1.5 MPa
* g  (kJ/kg)	?? 					0.186212297 		0.121685206
* h  (kJ/kg)	2.76881115e03		2.74015123e03		2.72134539e03
* u  (kJ/kg)	2.57629461e03		2.55393894e03		2.53881758e03
* s  (kJ/kg.K)	6.56660377e00		6.50218759e00		6.29170440e00
* Cp (kJ/kg.K)	2.76349265e00		2.98166443e00		3.62795578e00
* Cv (kJ/kg.K)	1.95830730e00		2.08622142e00		2.41213708e00	From Moscow Power Institute
* w  (m/s)		4.98408101e02		4.89363295e02		4.81941819e02


* see http://twt.mpei.ac.ru/mcs/worksheets/iapws/IAPWS-IF97-metastable-vapor.xmcd
*/

	printf ("\n\n************************************************************ \n" );
	printf ("IF97 REGION 2 METASTABLE PROPERTY (FORWARDS) EQUATIONS CHECK \n\n" );
	printf ("The output should be the following: \n\n");
	printf ("\t\tT= 450 K \tT= 440 K \tT= 450 K\n");
	printf ("\t\tp= 1 MPa \tp= 1 MPa \tp= 1.5 MPa\n\n");

	printf ("g  (kJ/kg)\t?????????\t?????????\t???????????\t(1)\n");	
	printf ("v  (m3/kg)\t0.192516540\t0.186212297\t0.121685206\n");
	printf ("h  (kJ/kg)\t2.76881115e03\t2.74015123e03\t2.72134539e03\n");
	printf ("u  (kJ/kg)\t2.57629461e03\t2.55393894e03\t2.53881758e03\n");
	printf ("s  (kJ/kg.K)\t6.56660377e00\t6.50218759e00\t6.29170440e00\n");
	printf ("Cp (kJ/kg.K)\t2.76349265e00\t2.98166443e00\t3.62795578e00\n");
	printf ("Cv (kJ/kg.K)\t1.95830730e00\t2.08622142e00\t2.41213708e00\t(2)\n");
	printf ("w  (m/s)\t4.98408101e02\t4.89363295e02\t4.81941819e02\n\n");
	
	printf ("\n(1) Using Freesteam \n(2) Using Moscow Power Institute calculation \n    http://twt.mpei.ac.ru/mcs/worksheets/iapws/IAPWS-IF97-metastable-vapor.xmcd\n\n");
	
	printf ("*******OUTPUT************\n");

	printf ("g  (kJ/kg)\t%.8e\t", if97_r2met_g(1.0,  450.0  ) );
		printf("%.8e\t", if97_r2met_g(1.0,  440.0  ) );
			printf("%.8e\t\n", if97_r2met_g(1.5,  450.0  ) );

	printf ("v  (m3/kg)\t%.8e\t", if97_r2met_v(1.0,  450.0  ) );
		printf("%.8e\t", if97_r2met_v(1.0,  440.0  ) );
			printf("%.8e\t\n", if97_r2met_v(1.5, 450.0) );
			
	printf ("h  (kJ/kg)\t%.8e\t", if97_r2met_h(1.0,  450.0  ) );
		printf("%.8e\t", if97_r2met_h(1.0,  440.0  ) );
			printf("%.8e\t\n", if97_r2met_h(1.5, 450.0) );
			
	printf ("u  (kJ/kg)\t%.8e\t", if97_r2met_u(1.0,  450.0  ) );
		printf("%.8e\t", if97_r2met_u(1.0,  440.0  ) );
			printf("%.8e\t\n", if97_r2met_u(1.5, 450.0) );		
	
	printf ("s  (kJ/kg.K)\t%.8e\t", if97_r2met_s(1.0,  450.0  ) );
		printf("%.8e\t", if97_r2met_s(1.0,  440.0  ) );
			printf("%.8e\t\n", if97_r2met_s(1.5, 450.0) );			
	
	printf ("Cp  (kJ/kg.K)\t%.8e\t", if97_r2met_Cp(1.0,  450.0  ) );
		printf("%.8e\t", if97_r2met_Cp(1.0,  440.0  ) );
			printf("%.8e\t\n", if97_r2met_Cp(1.5, 450.0) );		

	printf ("Cv  (kJ/kg.K)\t%.8e\t", if97_r2met_Cv(1.0,  450.0  ) );
		printf("%.8e\t", if97_r2met_Cv(1.0,  440.0  ) );
			printf("%.8e\t\n", if97_r2met_Cv(1.5, 450.0) );		
	
	printf ("w  (m/s)\t%.8e\t", if97_r2met_w(1.0,  450.0  ) );
		printf("%.8e\t", if97_r2met_w(1.0,  440.0  ) );
			printf("%.8e\t\n", if97_r2met_w(1.5, 450.0) );		
	



// B2bc Boundary Equation check
// For verification the equations below must meet the following p-h point
// see IAPWS-IF97 paragraph following Table 19
//
// p= 1.00000000 E2 MPa
// h= 3.516004323 E3 kJ/kg

	printf ("\n\n************************************************************ \n" );
	printf("IF97 REGION 2b -2c BOUNDARY FUNCTION CHECK \n\n" );
	printf ("The output should be the following: \n");
	printf("Pressure: \t\t1.00000000e02 MPa \n");	
	printf("Temperature: \t\t3.516004323e03 kJ/kg \n\n*******OUTPUT************\n");	
	
	printf ( "IF97_B2bc_p pressure \t%.8e MPa\n" , IF97_B2bc_p (3.516004323E3) ); 
	printf ( "IF97_B2bc_h Enthalpy \t%.8e kJ/kg\n\n" , IF97_B2bc_h (1.0e2) ); 



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



	printf ("\n\n************************************************************ \n" );
	printf ("IF97 REGION 2 T(p, h) BACKWARDS EQUATIONS CHECK \n\n" );
	printf ("The output should be the following: \n\n");
	
	printf ("Region\tp(MPa)\th(kJ/kg)\tT(K)\n");
	printf ("-------------------------------------------------\n");
	printf ("2a\t0.001\t3000\t\t5.34433241E02\n");
	printf ("2a\t3\t3000\t\t5.75373370E02\n");
	printf ("2a\t3\t4000\t\t1.01077577E03\n");
	printf ("-------------------------------------------------\n");
	printf ("2b\t5\t3500\t\t8.01299102E02\n");
	printf ("2b\t5\t4000\t\t1.01531583E03\n");
	printf ("2b\t25\t3500\t\t8.75279054E02\n");
	printf ("-------------------------------------------------\n");
	printf ("2c\t40\t2700\t\t7.43056411E02\n");
	printf ("2c\t60\t2700\t\t7.91137067E02\n");
	printf ("2c\t60\t3200\t\t8.82756860E02\n\n");	
	
	printf ("*******OUTPUT************\n");
	printf ("Region\tp(MPa)\th(kJ/kg)\tT(K)\n");
	printf ("-------------------------------------------------\n");
	
	printf ("2a\t0.001\t3000\t\t%.8e\n", if97_r2a_t_ph(0.001,  3000.0) );
	printf ("2a\t3\t3000\t\t%.8e\n", if97_r2a_t_ph(3.0,  3000.0) );
	printf ("2a\t3\t4000\t\t%.8e\n", if97_r2a_t_ph(3.0,  4000.0) );
		printf ("-------------------------------------------------\n");
	printf ("2b\t5\t3500\t\t%.8e\n", if97_r2b_t_ph(5.0,  3500.0) );
	printf ("2b\t5\t4000\t\t%.8e\n", if97_r2b_t_ph(5.0,  4000.0) );
	printf ("2b\t25\t3500\t\t%.8e\n", if97_r2b_t_ph(25.0,  3500.0) );
		printf ("-------------------------------------------------\n");
	printf ("2c\t40\t2700\t\t%.8e\n", if97_r2c_t_ph(40.0,  2700.0) );
	printf ("2c\t60\t2700\t\t%.8e\n", if97_r2c_t_ph(60.0,  2700.0) );
	printf ("2c\t60\t3200\t\t%.8e\n", if97_r2c_t_ph(60.0,  3200.0) );		
		
		
	
// check threading using OpenMP test
	printf ("\n\n************************************************************ \n" );
	printf ("CHECK FOR MULTITHREADDING \n\n" );
#pragma omp parallel
	{
	printf ("thread\n");
	}
	
	
	printf ("\n*********************** END ***************************** \n" );	
	
	return 0;
}

















