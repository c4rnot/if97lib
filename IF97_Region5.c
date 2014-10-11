//          Copyright Martin Lord 2014-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


//    IAPWS-IF97 Region 5:  ##description of region## equations
/* *********************************************************************
 * *******             VALIDITY                             ************
 * 
 * Region 5 of IAPWS-IF97 is valid for the following temperature and 
 * pressure range:
 * 1073.15 K <= T <= 2273.15 K
 * 0 < p <= 50 MPa.
 * 
 * It is only valid for pure undissociated water; any dissociation will 
 * have to be taken into account separately.
 * 
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
  * ****************************************************************** */
   

#include "IF97_common.h"  //PSTAR TSTAR & sqr
#include "IF97_Region5.h"
#include <math.h> // for pow, log
/* #ifdef _OPENMP // multithreading via libgomp
 # include <omp.h>
 #endif
*/
// #include <stdio.h>  used for debugging only


//***************************************************************
//****** REGION 5 *******************************


// o
definition {
	{0,    0.0}  // not used
	,{0,    -0.13179983674201E2} // 1
	,{1,    0.68540841634434E1}
	,{-3,    -0.24805148933466E-1}
	,{-2,    0.36901534980333}
	,{-1,    -0.31161318213925E1}
	,{2,    -0.32961626538917} //6
};




// r
definition {
	 {0,    0,    0.0}
	,{1,    1,    0.15736404855259E-2}
	,{1,    2,    0.90153761673944E-3}
	,{1,    3,    -0.50270077677648E-2}
	,{2,    3,    0.22440037409485E-5}
	,{2,    9,    -0.41163275453471E-5}
	,{3,    7,    0.37919454822955E-7}
};

