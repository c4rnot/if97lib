//          Copyright Martin Lord 2014-2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)



 
//    IAPWS-IF97 Region 3: *** name  ****
/* *********************************************************************
 * *******             VALIDITY                             ************
 * 
 * ?????????????????
 * 

 * ****************************************************************** */
/* ********************************************************************
  *         COMPILE AND LINK INSTRUCTIONS    (gcc)                          *
  * 
  * This library uses math.h, so must have the -lm  link flag
  * 
  * The library is programmed to be able to use OpenMP multithreading   
  * use the -fopenmp complie flag to enable multithreadded code
  * 
  * ***************************************************************** */
  
/**
 * @copyright
 * Copyright Martin Lord 2014-2015. \n
 * Distributed under the Boost Software License, Version 1.0. \n
 * (See accompanying file LICENSE_1_0.txt or copy at \n
 * http://www.boost.org/LICENSE_1_0.txt) \n
 * 
 * @file IF97_Region3.h
 * @author Martin Lord
 * @date 21 May 2015
 * @brief IAPWS-IF97 Region 3: vapour: saturation to critical region equations
 *
 * @details 
  * VALIDITY \n
 * 623.15 K <=T <= T ( p ) [B23 temperature equation] \n
 * p ( T ) [B23 temperature equation] <= p <= 100 MPa.\n
 * \n
 * In addition, region 3 yields reasonable values inthe metastable regions\n
 * (superheated liquid and subcooleed steam close to the saturated liquid\n
 * and saturated vapor line\n

 
 * @see http://www.iapws.org/relguide/IF97-Rev.html
 */



#ifndef IF97_REGION3_H
#define IF97_REGION3_H

#include "IF97_common.h"
#include <math.h> 




//**************************************************************
//********* REGION 3 PROPERTY EQUATIONS (FORWARDS) *************

	/** specific Helmholz free energy in region 3 (kJ / kg)
	// broken */
//	double if97_r3_hhf (double p_MPa , double t_Kelvin);
	
	






#endif // IF97_REGION2_H
