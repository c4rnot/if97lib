
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
	
	


	return 0;
}

