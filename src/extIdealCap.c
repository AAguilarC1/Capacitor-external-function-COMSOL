#define _USE_MATH_DEFINES
#include "area.h"
#include <stdlib.h>
#include <string.h>

#ifdef _MSC_VER
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

static const char *error = NULL;

EXPORT int init(const char *str){
  return 1;
}

EXPORT const char* getLastError(){
  return error;
}

static inline double Capacitance(double relative_permittivity, double area, double distance){
  const double Permittivity_0 = 8.854187817e-12;
  return (Permittivity_0 * relative_permittivity * area) / distance;
}

/* This is the function that will be called by COMSOL to evaluate your custom function */

/**
 * @brief Evaluates the custom function for COMSOL
 * @details This function will be called by COMSOL to evaluate the custom function. It takes the function name, number of arguments, input arguments, block size, and output arguments. It returns 1 if successful, 0 otherwise.  
 * @param func The function name
 * @param nArgs The number of arguments
 * @param inReal The real parts of the input arguments
 * @param inImag The imaginary parts of the input arguments
 * @param blockSize The block size
 * @param outReal The real parts of the output arguments
 * @param outImag The imaginary parts of the output arguments
 * @return 1 if successful, 0 otherwise 
 */
EXPORT int eval(const char* func,
                int nArgs,
                const double** inReal,
                const double** inImag,
                int blockSize,
                double* outReal,
                double* outImag){

  if (strcmp(func, "ideal_cap") == 0){
    // This if statement is checking that the received str is "ideal_cap". Which is the name used within COMSOL to call this function. If the function name is correct, it will proceed to evaluate the function. If not, it will return an error message.
 
    // What my custom function receives from COMSOL are following arguments in this order:
    // 1. Length of plate
    // 2. Width of plate
	  // 3. Dielectric thickness
	  // 4. Substrate thickness
    // 5. Permittivity of dielectric
    // 6. Permittivity of substrate
    
    // The function will then calculate the ideal capacitance of a pill plate using the formula:
    // Output:
	  // 1. Ideal Capacitance of pill plate (F)
    // 
    // C = (Permittivity_0 * Permittivity * Area) / Distance
    
    if (nArgs != 6){
      // This guard is checking that the number of arguments received from COMSOL is 6
      error = "ideal_cap: wrong number of arguments";
      return 0;
    }

    double length = (*inReal[0]); // Unpacking the 1st argument
    double width = (*inReal[1]); // Unpacking the 2nd argument
    double dielectric_thickness = (*inReal[2]); // Unpacking the 3rd argument
    double substrate_thickness = (*inReal[3]); // Unpacking the 4th argument
    double dielectric_permittivity = (*inReal[4]); // Unpacking the 5th argument
    double substrate_permittivity = (*inReal[5]); // Unpacking the 6th argument

    if(length <= 0 || width <= 0 || dielectric_thickness <= 0 || substrate_thickness <= 0 || dielectric_permittivity <= 0 || substrate_permittivity <= 0){
      // This guard is checking that all the arguments received from COMSOL are positive. If any of the arguments are negative or zero, it will return an error message.
      error = "ideal_cap: arguments must be positive";
      return 0;
    }
    
    double distance = dielectric_thickness + 2 * substrate_thickness; // Acquiring the distance for a 3 layer structure
    double permittivity = dielectric_permittivity; // Assuming the dielectric layer is the dominant relative permittivity for the capacitance calculation
    double area = area_pill(length, width); // Calculating the area of the pill plate using the area_pill function from area.h
    double capacitance = Capacitance(permittivity, area, distance); // Calculating the ideal capacitance using the Capacitance function defined above

    outReal[0] = capacitance; // Store the calculated capacitance in the output array

    return 1;

  }else{
    // If the function name received from COMSOL does not match "ideal_cap", it will return an error message.
    error = "ideal_cap: unknown function";
    return 0;
  }
}
