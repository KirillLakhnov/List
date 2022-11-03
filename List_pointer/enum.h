#ifndef ENUM_H
#define ENUM_H

enum ERROR_FUNCTION {
    GOOD_WORKING     = 0,
    
    ERROR_CALLOC     = 1,
    ERROR_REALLOC    = 2,

    ERROR_FILE_OPEN  = 3,
    ERROR_FILE_CLOSE = 4,
};

enum LIST_ERROR {
    LIST_ERROR_POINTER_STRUCT_NULL        = 1 << 0,
    LIST_ERROR_HEAD_NULL                  = 1 << 1,
    LIST_ERROR_TAIL_NULL                  = 1 << 2,
    LIST_ERROR_SIZE_SMALLER_ZERO          = 1 << 3,
};

#endif // ENUM_H