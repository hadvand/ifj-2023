/**
 * @file string.h
 * @author Murad Mikogaziev (xmikog00)
 * @brief source code for string functions
 *
 * @date 02.10.2023
 * 
 */

#ifndef STRING_HEADER_G   //guard
#define STRING_HEADER_G

#include <stdlib.h>
#include <stdbool.h>

#define BYTES_TO_ALLOC 9    //the longest keyword is "function", which has 8 characters

/**
 * @brief struct for string
 * 
 */
typedef struct string_t{
    char *string;
    unsigned last_index;
    unsigned mem_allocated;
} *string_ptr;

/**
 * @brief string struct initialization
 * 
 * @return string_ptr pointer to created string struct, NULL on error
 */
string_ptr string_init();

/**
 * @brief release allocated memory for string
 * 
 * @param string pointer to string struct
 */
void string_free(string_ptr string);

/**
 * @brief append symbol to the end of the string
 * 
 * @param string pointer to string struct
 * @param c char to be appended
 * @return true if success, false if alloc failed
 */
bool string_append(string_ptr string, char c);

#endif    //for guard