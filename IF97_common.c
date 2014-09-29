
//          Copyright Martin Lord 2014-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


//    IAPWS-IF97 constants used throughout the formulation


/**
 * @copyright
 * Copyright Martin Lord 2014-2014. \n
 * Distributed under the Boost Software License, Version 1.0. \n
 * (See accompanying file LICENSE_1_0.txt or copy at \n
 * http://www.boost.org/LICENSE_1_0.txt) \n
 * 
 * @file IF97_common.h
 * @author Martin Lord
 * @date 29 Sep 2014
 * @brief Constants and common functions used (internally) by the library.
 *
 */

/*! \mainpage 
 *
 * \section intro_sec Introduction
 *
 * if97lib is a set of IAPWS-IF97 steam tables written in C and released under the BOOST licence
 *
 * \section install_sec Installation
 *
 * \subsection step1 Getting the latest Code
 *
 *
 * if97 is hosted on bitbucket using the Mercurial revision management system.
 *
 * The bitbucket page for this project is here:
 * https://C4rnot@bitbucket.org/C4rnot/if97lib
 * 
 * 
 * In order to get the latest version of the source (assuming you have Mercurial already installed):
 * 
 * $ hg clone https://C4rnot@bitbucket.org/C4rnot/if97lib
 * 
 * 
 * Alternatively, download a zip of the source from here:
 * https://bitbucket.org/C4rnot/if97lib/downloads
 * 
 * 
 * \subsection step2 Compilation
 * 
 * The Make system has not yet been implimented.
 * 
 * A number of files use the C math library <math.h>.  -lm needs to be passed to the compiler to link this correctly.
 *
 * Some code uses OpenMP for multithreading.  Since openMP uses pragma calls to the preprocessor, the code will compile and run correctly, even if your compiler doesn't support OpenMP
 * 
 * use the  -fopenmp flag to enable multithreaded calculation in gcc.  Please consult http://openmp.org for instructions for other compilers.
 * 
 * The example below compiles and links constants.c, IF97_Region2.c, IF97_Region2_met.c, region2_test.c to produce the executable region2test_lnx
 * 
 * $ gcc -fopenmp IF97_common.c IF97_Region2.c IF97_Region2_met.c region2_test.c -o region2test_lnx -lm
 * 
 *  * \section refs References
 * 
 * This library is based on the International Association for the Properties of Water and Steam Revised Release of the IAPWS Industrial Formulation 1997 for the Thermodynamic Properties of Water and Steam, which is available here:
 * 
 * http://www.iapws.org/relguide/IF97-Rev.pdf
 * 
 */


#include "IF97_common.h"


/** squares a double without using pow */
double sqr (double dblArg) {	return dblArg * dblArg; }

/** squares a double without using pow */
double cube (double dblArg){	return dblArg * dblArg * dblArg; }

