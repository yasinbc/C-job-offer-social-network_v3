#ifndef __JOB_H__
#define __JOB_H__
#include "date.h"
#include "csv.h"

#define LEGAL_MIN_SALARY 16576
#define EMPTY_SALARY -1
#define NUM_FIELDS_JOB 8

// Define the struct despite its incompleteness to prevent including recursion error
typedef struct _tCompany tCompany;

typedef struct _tJob {
    int id;
    char *role;
    int nPos;
    tDate startDate;
    tDate endDate;
    int minSalary;
    int maxSalary;
	tCompany *company;
} tJob;

typedef struct _tJobs {
    tJob *elems;
    int count;
} tJobs;

// Parse input from CSVEntry
void job_parse(tJob* data, char *companyCif, tCSVEntry entry);

// Copy the data from the source to destination (individual data)
void job_cpy(tJob* destination, tJob source);

// Get job data using a string
void job_get(tJob data, char* buffer);

// Release all the allocated memory
void job_free(tJob *data);

// Initialize jobs data
void jobs_init(tJobs* data);

// Add a new job
void jobs_add(tJobs* data, tJob job);

// Get job data of position index using a string
void jobs_get(tJobs data, int index, char* buffer);

// Returns the position of a job looking for id's job. -1 if it does not exist
int jobs_find(tJobs data, int id);

// Release all the jobs
void jobs_free(tJobs* data);

#endif
