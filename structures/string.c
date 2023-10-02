/**
 * @file string.h
 * @author Murad Mikogaziev (xmikog00)
 * @brief source code for string functions
 *
 * @date 02.10.2023
 * 
 */

#include "string.h"

string_ptr string_init()
{
    //create a new string
    string_ptr string;
    if((string = (string_ptr) malloc(sizeof(struct string_t))) == NULL){
        //alloc failed
        return NULL;
    }

    string->last_index = 0;
    string->mem_allocated = 0;
    string->string = NULL;

    return string;
}

void string_free(string_ptr string)
{
    if(string != NULL){
        free(string->string);
        string->string = NULL;
        free(string);
        string = NULL;
    }
}

bool string_append(string_ptr string, char c)
{
    if((string->last_index + 1 == string->mem_allocated) || string->last_index == 0){
        if(string->mem_allocated == 0){
            string->mem_allocated = BYTES_TO_ALLOC;
        }
        else{
            string->mem_allocated *= 2;
        }
        
        if((string->string = (char *) realloc(string->string, string->mem_allocated)) == NULL){
            //aloc failed
            return false;
        }
    }

    string->string[string->last_index] = c;
    string->last_index++;
    string->string[string->last_index] = '\0';

    return true;
}


