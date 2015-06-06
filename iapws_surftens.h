//          Copyright Martin Lord 2014-2015.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


/*    Surface tension according to 
 * IAPWS Revised Release on Surface Tension of Ordinary Water Substance 
 * Moscow June 2014  */

 
 /* ********************************************************************
  *         COMPILE AND LINK INSTRUCTIONS    (gcc)                          *
  * 
  * This library uses math.h, so must have the -lm  link flag
  * 
  * The library is programmed to be able to use OpenMP multithreading   
  * use the -fopenmp complie flag to enable multithreadded code
  * 
  * ****************************************************************** */
   

/**
 * @copyright
 * Copyright Martin Lord 2014-2015. \n
 * Distributed under the Boost Software License, Version 1.0. \n
 * (See accompanying file LICENSE_1_0.txt or copy at \n
 * http://www.boost.org/LICENSE_1_0.txt) \n
 * 
 * @file iapws_surftens.h
 * @author Martin Lord
 * @date 06 June 2015
 * @brief IAPWS Surface Tension of Ordinary Water
 * @details Surface tension according to 
 * IAPWS Revised Release on Surface Tension of Ordinary Water Substance 
 * Moscow June 2014 \n
 * \n
 * Validity:  From 0.01°C (273.16 °K) to critical Temperature (647.096 °K)\n
 * It also gives reasonably accurate values when extrapolated into the 
 * supercooled region to temperatures as low as -25 °C\n
 * \n 
 * @see http://www.iapws.org/relguide/Surf-H2O.html
 */


#ifndef IAPWS_SURFTENS_H
#define IAPWS_SURFTENS_H

#include <math.h> // for pow


/** surface tension in mN/m for a given temperature °C according to ITS-90 */
double iapws_surftens(double t_Celcius);

#endif // IAPWS_SURFTENS_H


