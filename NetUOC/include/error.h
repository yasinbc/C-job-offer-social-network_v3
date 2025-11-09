#ifndef __ERROR__H
#define __ERROR__H

// Define error codes
enum _tApiError
{
    E_SUCCESS = 0, // No error
    E_NOT_IMPLEMENTED = -1, // Called method is not implemented
    E_FILE_NOT_FOUND = -2, // File not found
    E_INVALID_ENTRY_TYPE = -3, // Invalid entry type
    E_INVALID_ENTRY_FORMAT = -4, // Invalid entry format
    E_MEMORY_ERROR = -5, // Memory error 
	E_PERSON_DUPLICATED = -6, // Person duplicated
	E_PERSON_NOT_FOUND = -7, // Person not found
	E_COMPANY_DUPLICATED = -8, // Company duplicated
	E_COMPANY_NOT_FOUND = -9, // Company not found
};

// Define an error type
typedef enum _tApiError tApiError;

#endif // __ERRORS__H