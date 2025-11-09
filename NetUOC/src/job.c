#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "job.h"
#include "company.h"

// Parse input from CSVEntry
void job_parse(tJob* data, char *companyCif, tCSVEntry entry) {
    // Check input data
    assert(data != NULL);

    // Check entry fields
    assert(csv_numFields(entry) == NUM_FIELDS_JOB);

    int pos = 0; // Allow to easy incremental position of the income data

    // Copy job's id data
    data->id = csv_getAsInteger(entry, pos);

	// Copy role
	data->role = (char*)malloc(sizeof(char) * (strlen(entry.fields[++pos]) + 1));
	assert(data->role != NULL);
	strcpy(data->role, entry.fields[pos]);

    // Copy number of positions data
    data->nPos = csv_getAsInteger(entry, ++pos);

    // Parse start date
    assert(strlen(entry.fields[++pos]) == DATE_LENGTH);
    date_parse(&(data->startDate), entry.fields[pos]);

    // Parse end date
    assert(strlen(entry.fields[++pos]) == DATE_LENGTH);
    date_parse(&(data->endDate), entry.fields[pos]);

    // Copy salary data
    data->minSalary = csv_getAsInteger(entry, ++pos);
    data->maxSalary = csv_getAsInteger(entry, ++pos);
	
	// Copy the company CIF
    strcpy(companyCif, entry.fields[++pos]);
}

// Copy the data from the source to destination (individual data)
void job_cpy(tJob* destination, tJob source) {
    // Copy job's id data
    destination->id = source.id;

    // Copy role data
	destination->role = (char*)malloc(sizeof(char) * (strlen(source.role) + 1));
	assert(destination->role!= NULL);
	strcpy(destination->role, source.role);

    // Copy number of positions data
    destination->nPos = source.nPos;

    // Copy start/end date
    date_cpy(&(destination->startDate), source.startDate);
    date_cpy(&(destination->endDate), source.endDate);

    // Copy salary data
    destination->minSalary = source.minSalary;
    destination->maxSalary = source.maxSalary;
	
	// Copy the pointer to the company
	destination->company = source.company;
}

// Get job data using a string
void job_get(tJob data, char* buffer)
{
    // Print all data at same time
    sprintf(buffer,"%d;%s;%d;%02d/%02d/%04d;%02d/%02d/%04d;%d;%d;%s",
        data.id,
        data.role,
        data.nPos,
        data.startDate.day, data.startDate.month, data.startDate.year,
        data.endDate.day, data.endDate.month, data.endDate.year,
        data.minSalary,
        data.maxSalary,
		data.company->info.cif);
}

// Release all the allocated memory
void job_free(tJob *data) {
	// Check preconditions
	assert(data != NULL);
	
	if (data->role != NULL) {
		free(data->role);
		data->role = NULL;
	}
}

// Initialize jobs data
void jobs_init(tJobs* data) {
    // Check input data
    assert(data != NULL);

	data->elems = NULL;
    data->count = 0;
}

// Add a new job
void jobs_add(tJobs* data, tJob job) {
    // Check input data
    assert(data != NULL);
    assert(job.id >= 1);
    assert(job.nPos >= 1);
    assert(date_cmp(job.startDate, job.endDate) == -1);
    assert(job.minSalary <= job.maxSalary || job.minSalary == EMPTY_SALARY || job.maxSalary == EMPTY_SALARY );

    // Check that salary is legal or not informed. It is not a precondition, we could receive this data
    // It is recommended to check first salary, find if exists could need more computational time
    if ((job.minSalary == EMPTY_SALARY || job.minSalary >= LEGAL_MIN_SALARY) &&
        (job.maxSalary == EMPTY_SALARY || job.maxSalary >= LEGAL_MIN_SALARY)) {
        // If job does not exist add it
        if (jobs_find(*data, job.id) < 0) {
			// Allocate the memory
			if (data->count == 0) {
				data->elems = (tJob*)malloc(sizeof(tJob));
			} else {
				data->elems = (tJob*)realloc(data->elems, sizeof(tJob) * (data->count + 1));
			}
			
			// Check the allocation
			assert(data->elems != NULL);
			
            // Copy the data to the new position if there is space
            job_cpy(&(data->elems[data->count]), job);

            // Increase the number of elements
            data->count++;
        }
    }
}

// Get job data of position index using a string
void jobs_get(tJobs data, int index, char* buffer)
{
    assert(index >= 0 && index < data.count);
    job_get(data.elems[index], buffer);
}

// Returns the position of a job looking for id's job. -1 if it does not exist
int jobs_find(tJobs data, int id) {
    // An alternative solution compared to people_find
    int i = 0;
    int result = -1;
    while (i < data.count && result == -1) {
        if (data.elems[i].id == id) {
            result = i;
        }
        i++;
    }

    return result;
}

// Release all the jobs
void jobs_free(tJobs* data) {
	// Check preconditions
    assert(data != NULL);
	
	// Release allocated memory for every job
    for (int i = 0; i < data->count; ++i) {
        job_free(&data->elems[i]);
    }
	
	// Release all the vector
    free(data->elems);
	
    // Initialize again to prepare data
	jobs_init(data);
}
