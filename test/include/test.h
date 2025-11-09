#ifndef __TEST__H
#define __TEST__H
#include "test_suite.h"

// Write data to file
void save_data(const char* filename, const char* data);

// Run all available tests
void testSuite_run(tTestSuite* test_suite, const char* input, const char* readme);

#endif // __TEST__H