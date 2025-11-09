#include <stdio.h>
#include <assert.h>
#include "csv.h"
#include "api.h"
#include <string.h>

// Get the API version information
const char* api_version()
{
    return "UOC PP 20251";
}

// Load data from a CSV file. If reset is true, remove previous data
tApiError api_loadData(tApiData* data, const char* filename, bool reset)
{
    tApiError error;
    FILE *fin;    
    char buffer[FILE_READ_BUFFER_SIZE];
    tCSVEntry entry;
    
    // Check input data
    assert( data != NULL );
    assert(filename != NULL);
    
    // Reset current data    
    if (reset) {
        // Remove previous information
        error = api_freeData(data);
        if (error != E_SUCCESS) {
            return error;
        }
        
        // Initialize the data
        error = api_initData(data);
        if (error != E_SUCCESS) {
            return error;
        }
    }

    // Open the input file
    fin = fopen(filename, "r");
    if (fin == NULL) {
        return E_FILE_NOT_FOUND;
    }
    
    // Read file line by line
    while (fgets(buffer, FILE_READ_BUFFER_SIZE, fin)) {
        // Remove new line character     
        buffer[strcspn(buffer, "\n\r")] = '\0';
        
        csv_initEntry(&entry);
        csv_parseEntry(&entry, buffer, NULL);
        // Add this new entry to the api Data
        error = api_addDataEntry(data, entry);
        if (error != E_SUCCESS) {
			csv_freeEntry(&entry);
			fclose(fin);
            return error;
        }
        csv_freeEntry(&entry);

    }
    
    fclose(fin);
    
    return E_SUCCESS;
}

// Initialize the data structure
tApiError api_initData(tApiData* data) {
	/////////////////////////////////
	// PR1_3b
	/////////////////////////////////
    
    /////////////////////////////////
	return E_NOT_IMPLEMENTED;
}

// Add a person into the data if it does not exist
tApiError api_addPerson(tApiData* data, tCSVEntry entry) {
	/////////////////////////////////
	// PR1_3c
	/////////////////////////////////
	
	/////////////////////////////////
    return E_NOT_IMPLEMENTED;
}

// Add a company if it does not exist
tApiError api_addCompany(tApiData* data, tCSVEntry entry) {
	/////////////////////////////////
	// PR1_3d
	/////////////////////////////////
	
	/////////////////////////////////
    return E_NOT_IMPLEMENTED;
}

// Add a job into the company if it does not exist
tApiError api_addJob(tApiData* data, tCSVEntry entry) {
	/////////////////////////////////
	// PR1_3e
	/////////////////////////////////
	
	/////////////////////////////////
    return E_NOT_IMPLEMENTED;
}

// Get the number of people registered on the application
int api_peopleCount(tApiData data) {
	/////////////////////////////////
	// PR1_3f
	/////////////////////////////////
	
	/////////////////////////////////
    return -1;
}

// Get the number of companies registered on the application
int api_companiesCount(tApiData data) {
	/////////////////////////////////
	// PR1_3g
	/////////////////////////////////
	
	/////////////////////////////////
    return -1;
}

// Get the number of jobs registered on the application
int api_jobsCount(tApiData data) {
	/////////////////////////////////
	// PR1_3h
	/////////////////////////////////
	
	/////////////////////////////////
    return -1;
}

// Free all used memory
tApiError api_freeData(tApiData* data) {
	/////////////////////////////////
	// PR1_3i
	/////////////////////////////////
	
    /////////////////////////////////
    return E_NOT_IMPLEMENTED;
}

// Add a new entry
tApiError api_addDataEntry(tApiData* data, tCSVEntry entry) {
	/////////////////////////////////
	// PR1_3j
	/////////////////////////////////
	
    /////////////////////////////////
    return E_NOT_IMPLEMENTED;
}

// Get person data
tApiError api_getPerson(tApiData data, const char *document, tCSVEntry *entry) {
	/////////////////////////////////
	// PR1_4a
	/////////////////////////////////
	
	/////////////////////////////////
    return E_NOT_IMPLEMENTED;
}

// Get company data
tApiError api_getCompany(tApiData data, const char* cif, tCSVEntry *entry) {
	/////////////////////////////////
	// PR1_4b
	/////////////////////////////////
	
	/////////////////////////////////
    return E_NOT_IMPLEMENTED;
}

// Get jobs data
tApiError api_getJobs(tApiData data, tCSVData *jobs) {
	/////////////////////////////////
	// PR1_4c
	/////////////////////////////////
	
	/////////////////////////////////
    return E_NOT_IMPLEMENTED;
}

// Get jobs data by company and salary
tApiError api_getJobsByCompanyAndSalary(tApiData data, tCSVData *jobs, const char *companyCif, int salary) {
	/////////////////////////////////
	// PR1_4d
	/////////////////////////////////
	
	/////////////////////////////////
    return E_NOT_IMPLEMENTED;
}
