#ifndef ENUM_H
#define ENUM_H

enum ERROR_FUNCTION {
    GOOD_WORKING     = 0,
    
    ERROR_CALLOC     = 1,
    ERROR_REALLOC    = 2,

    ERROR_FILE_OPEN  = 3,
    ERROR_FILE_CLOSE = 4,
};

#endif // ENUM_H