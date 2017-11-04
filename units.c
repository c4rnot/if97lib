
//          Copyright Martin Lord 2014-2014.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


//  Unit conversions

// Converts input to SI then SI to output


#include "units.h"
#include <string.h>
#include <ctype.h>  // for tolower
#include <stdbool.h>
#include <stdio.h>
#include <float.h>




// set up the table of units:  import from "built_in_units.dat"
const typUnit builtInUnits[] = {
	 { "void", "-", DIMENSIONLESS, { {0.0, 1.0 }, 0.0}, "void", false} //dummy to make sure include file consistency
	#include "built_in_units.dat" 
};



double applyCoeffs (double input, typConvCoeffs coeffs) {
	return coeffs.dblPow[0] + input * coeffs.dblPow[1];
}


char *lowercase (char *strOutput, const char *strInput, int maxLen){
	
//	char *ptrInChar = strInput; // point to first character of input
	char *ptrOutChar = strOutput; // point to first character of output

	int i = 0;
	
	while (*strInput != 0) { // while the character is not the end of string marker
		if (i >= maxLen) break;
		i++;
		*ptrOutChar = tolower(*strInput);  // place lowercase letter in output
		ptrOutChar++; strInput++;  //point to the next letter in the output and input
	}
	
	*ptrOutChar = 0;  // put a tring terminator on the end of the copied string

	
/*
 * char *
strcpy(char *s1, const char *s2)
{
    char *s = s1;
    while ((*s++ = *s2++) != 0)
	;
    return (s1);
}
 * 
 * 
 */	

	
	return (strOutput);  //return a pointer to the lowercase string
}
				
int getUnitIndex ( char strUnit[]){

	int i = 0;

	
	int endIndex = sizeof (builtInUnits) / sizeof (typUnit) -1;
//	printf ("endIndex = %i\n", endIndex);

	// try matching the unit symbol with the input text
	while ((strcmp(builtInUnits[i].strSymbol, strUnit) != 0) && (i <= endIndex) ){       
      i++;       
	}	  
	if (i <= endIndex) return i ;
  
	i = 0;  // reset i and try again with the full names
	while ((strcmp(builtInUnits[i].strName, strUnit) != 0) && (i <= endIndex) ){       
      i++;        
	}	  
	if (i <= endIndex) return i ;
	
	
	i = 0; // reset i and try again with lowercase names
	if (strlen(strUnit) < 5) return -1 ;  // but only for long strigs to avaid mismatching mPa with MPA mREM with MREM etc.
	char strLowercase [NAMESTRLEN] ="";
	while ((strcmp(lowercase(strLowercase, builtInUnits[i].strName, NAMESTRLEN), strUnit) != 0) && (i <= endIndex) ){       
      i++;        
	}	  
	if (i <= endIndex) return i ;


	
return -1; //failed to find the unit	
}



typConvCoeffs getCoeffs (int UnitIndex){
	
	return builtInUnits[UnitIndex].tConvCoeffs;
	
}



double convertUnit (double inVal, int inUnit, int outUnit){
	return -1.0;
// TODO
}


double convertNamedUnit (double inVal, char strInUnit[], char strOutUnit[]){
	if (strcmp(strInUnit, strOutUnit) == 0) return inVal; //if the input unit is the output unit return unchanged
	
	int inUnitIndex = -1;
	int outUnitIndex = -1;
	char strInUnitSI[NAMESTRLEN] = "void";
	char strOutUnitSI[NAMESTRLEN] = "void";
	double valSI = DBL_MIN;
	

	
	inUnitIndex = getUnitIndex( strInUnit);
	strcpy ( strInUnitSI, builtInUnits[inUnitIndex].strConvertsTo);
	// printf ("SI Unit is = %s\n", strInUnitSI);

	outUnitIndex = getUnitIndex( strOutUnit);
	strcpy ( strOutUnitSI, builtInUnits[outUnitIndex].strConvertsTo);
	
	if(strcmp(strInUnitSI, strOutUnitSI) == 0) {  // OK - compatible units
		
		// First convert to the SI unit
		valSI = inVal * builtInUnits[inUnitIndex].tConvCoeffs.dblPow[1] + builtInUnits[inUnitIndex].tConvCoeffs.dblPow[0];
		
		
		printf( "%f  %s\n", valSI, strInUnitSI);
		
		// then convert from the SI unit to the output unit
		return (valSI - builtInUnits[outUnitIndex].tConvCoeffs.dblPow[0]) / builtInUnits[outUnitIndex].tConvCoeffs.dblPow[1] ;
		
		}
		
	return DBL_MIN;  //Error  incompatible units.  TODO may need code to go through an intermediate conversion step
}


double convertNamedUnitp (double inVal, char* strInUnit[], char* strOutUnit[]){
	if (strcmp(strInUnit, strOutUnit) == 0) return inVal; //if the input unit is the output unit return unchanged
	
	int inUnitIndex = -1;
	int outUnitIndex = -1;
	char strInUnitSI[NAMESTRLEN] = "void";
	char strOutUnitSI[NAMESTRLEN] = "void";
	double valSI = DBL_MIN;
	

	
	inUnitIndex = getUnitIndex( strInUnit);
	strcpy ( strInUnitSI, builtInUnits[inUnitIndex].strConvertsTo);
	// printf ("SI Unit is = %s\n", strInUnitSI);

	outUnitIndex = getUnitIndex( strOutUnit);
	strcpy ( strOutUnitSI, builtInUnits[outUnitIndex].strConvertsTo);
	
	if(strcmp(strInUnitSI, strOutUnitSI) == 0) {  // OK - compatible units
		
		// First convert to the SI unit
		valSI = inVal * builtInUnits[inUnitIndex].tConvCoeffs.dblPow[1] + builtInUnits[inUnitIndex].tConvCoeffs.dblPow[0];
		
		//
		 printf( "%f  %s\n", valSI, strInUnitSI);
		
		// then convert from the SI unit to the output unit
		return (valSI - builtInUnits[outUnitIndex].tConvCoeffs.dblPow[0]) / builtInUnits[outUnitIndex].tConvCoeffs.dblPow[1] ;
		
		}
		
		return DBL_MIN;  //Error  incompatible units.  TODO may need code to go through an intermediate conversion step
}



double isCompatible (int inUnit, int outUnit){
		return -1.0;
	// TODO
}

/*  pending definition of typDimensions
typDimensions getDimensions (int unit){
	// TODO
}
*/


void getUnitType (int unit, char strUnitType[]){
// TODO
}			
				
	


