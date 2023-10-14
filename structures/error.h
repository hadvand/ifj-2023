#ifndef ERROR_HEADER_G
#define ERROR_HEADER_G

typedef enum error{
    ER_NONE          = 0,
    ER_LEX           = 1,
    ER_SYNTAX        = 2,
    ER_SEMAN         = 3,
    ER_PARAMS        = 4,
    ER_UNDEF_VAR     = 5,
    ER_FUNC_RETURN   = 6,
    ER_TYPE_COMP     = 7,
    ER_OTHER_SEM     = 8,
    ER_INTERNAL      = 99
} error_t;

/**
 * @brief external variable for assigning the error type
 * 
 */
extern error_t err_type;

/**
 * @brief external variable for line counter
 * 
 */
extern unsigned line_cnt;

#endif