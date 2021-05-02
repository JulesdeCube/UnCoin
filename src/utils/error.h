#ifndef UNCOIN__UTILS__ERROR_H_
#define UNCOIN__UTILS__ERROR_H_

// Task is a success
#define SUCCESS 0
// Error that is not normaly possible
#define INTERNAL_ERROR 1
// Not enough space
#define NO_SPACE 2
// The main object is null
#define NO_SELF 3
// The provided index is greater than the size
#define OUT_OF_RANGE 4
// The value is not valid
#define ERROR_VALUE 5

/**
* call the function and on error call the clean function and return the error
*
* @param function the function that need to be call
* @param clear function to clear all the alloced memory;
*
* @throw the error code thow by the function
*/
#define TRY_CATCH(function, clear) \
    {                              \
        int error = function;      \
        if (error != SUCCESS)      \
        {                          \
            clear;                 \
            return error;          \
        }                          \
    }

/**
* call the function and on error return the error
*
* @param function the function that need to be call
*
* @throw the error code thow by the function
*/
#define TRY(function) \
    TRY_CATCH(function, {})

#endif // UNCOIN__UTILS__ERROR_H_
