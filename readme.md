# External Function for COMSOL
COMSOL has the capacity of using external functions created in C. This Repository will contain an example of how to implement your own external function for a capacitance calculation application. To set up the process of using external functions please go to the [COMSOL documentation](https://doc.comsol.com/6.4/doc/com.comsol.help.comsol/comsol_ref_definitions.21.063.html#:~:text=An%20External%20function%20(%20)%20interfaces%20with,functions%20defined%20in%20COMSOL%20Multiphysics.). 

## The structure 
In `src` there is a single C file which contains the example which as the following structure:

```c
#include <stdlib.h>
#include <string.h>

#ifdef _MSC_VER
#define EXPORT __declspec(dllexport)
#else
#define EXPORT
#endif

static const char *error = NULL;

// Initializes the process, this can be left alone 
EXPORT int init(const char *str){
  return 1;
}

// This will pass the latest available error message to COMSOL
EXPORT const char* getLastError(){
  return error;
}

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

  if (strcmp(func, "my_custom_name") == 0){
    // Checking that the called function has the correct name from COMSOL
    
    if (nArgs != 2){
      // This guard is checking that the number of arguments received from COMSOL is 2 but the number is arbitrary
      return 0;
    }

    double length = (*inReal[0]); // Unpacking the 1st argument
    double width = (*inReal[1]); // Unpacking the 2nd argument
    
    double area = length * width;

    outReal[0] = area; // Storing the calculated result in the output array

    return 1;
  }else{
    // If an error occurred the message can be passed up to COMSOL 
    error = "Called but was given the wrong name";
    return 0;
  }
}
``` 

As you can see the structure of the C file is made for exporting the compiled functions into a shareable object. Which are libraries that can be used by other systems, instead of creating an executable. This is what allows the integration of our custom function into COMSOL.

>[!note]
> Depending on your system you may have a different extension for a shareable object. On windows these are *.dll files, or dynamic link libraries.

## Making the dynamic link library
To make the project you'll need to have installed on your system Make. This will allow you to use the already existing `makefile` inside this project to compile your code into the shared object. To build just run the following command.

```bash
make all
```

This will generate the a shared object for each C file within the src folders.
