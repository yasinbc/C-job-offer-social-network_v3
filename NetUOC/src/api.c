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

	// Check if data is empty
	if (data == NULL) {
		return E_MEMORY_ERROR;
	}

	// Initialize people
	tApiError error = people_init(&(data->people));
	if (error != E_SUCCESS) {
		// If it fails, does not continue
		return error;
	}

	// Initilize company
	error = companyList_init(&(data->companies));
	if (error != E_SUCCESS) {
		// If it fails, clear what is initilized already
		people_free(&(data->people));
		return error;
	}

	return E_SUCCESS;
    /////////////////////////////////
	//return E_NOT_IMPLEMENTED;
}

// Add a person into the data if it does not exist
tApiError api_addPerson(tApiData* data, tCSVEntry entry) {
	/////////////////////////////////
	// PR1_3c
	/////////////////////////////////

	// Get type CSV entry
	const char* entryType = csv_getType(&entry);
	// Verify the PERSON type
	if (strcmp(entryType, "PERSON") != 0) {
		return E_INVALID_ENTRY_TYPE;
	}

	// Verify it has the right number of fields (8)
	if (csv_numFields(entry) != NUM_FIELDS_PERSON) {
		return E_INVALID_ENTRY_FORMAT;
	}

	char document[256];
	csv_getAsString(entry, 0, document, sizeof(document));

	// Search for if it exists already a person with this document
	if (people_find(data->people, document) >= 0) {
		return E_PERSON_DUPLICATED;
	}

	tPerson newPerson;
	person_parse(&newPerson, entry);
	tApiError error = people_add(&(data->people), newPerson);
	if (error != E_SUCCESS) {
		person_free(&newPerson);
		return error;
	}
	return E_SUCCESS;
	/////////////////////////////////
    //return E_NOT_IMPLEMENTED;
}

// Add a company if it does not exist
tApiError api_addCompany(tApiData* data, tCSVEntry entry) {
	/////////////////////////////////
	// PR1_3d
	/////////////////////////////////
	///
	// Get type CSV entry
	const char* entryType = csv_getType(&entry);
	// Verify the COMPANY type
	if (strcmp(entryType, "COMPANY") != 0) {
		return E_INVALID_ENTRY_TYPE;
	}

	// Verify it has the right number of fields (7)
	if (csv_numFields(entry) != NUM_FIELDS_COMPANY) {
		return E_INVALID_ENTRY_FORMAT;
	}

	// Get the CIF of the company (first field)
	char cif[256];
	csv_getAsString(entry, 0, cif, sizeof(cif));

	// Search if the company already exists
	if (companyList_find(data->companies, cif) != NULL) {
		return E_COMPANY_DUPLICATED;
	}

	// Get the founder document (last field, position 6)
	char founderDocument[256];
	csv_getAsString(entry, 6, founderDocument, sizeof(founderDocument));

	// Find the founder in the people list
	int founderIndex = people_find(data->people, founderDocument);
	if (founderIndex < 0) {
		return E_PERSON_NOT_FOUND;
	}

	// Parse the company data
	tCompany newCompany;
	company_parse(&newCompany, founderDocument, entry);

	// Get the founder pointer from the people array
	tPerson* founder = &(data->people.elems[founderIndex]);

	// Add the company to the list with the founder pointer
	tApiError error = companyList_add(&(data->companies), newCompany, founder);
	if (error != E_SUCCESS) {
		// If adding fails, clean up the new company
		company_free(&newCompany);
		return error;
	}

	return E_SUCCESS;
	/////////////////////////////////
    //return E_NOT_IMPLEMENTED;
}

// Add a job into the company if it does not exist
tApiError api_addJob(tApiData* data, tCSVEntry entry) {
	/////////////////////////////////
	// PR1_3e
	/////////////////////////////////

	// Validate type of entry JOB
	const char* entryType = csv_getType(&entry);
	if (strcmp(entryType, "JOB") != 0) {
		return E_INVALID_ENTRY_TYPE;
	}

	// Validate format of entry  (8)
	if (csv_numFields(entry) != NUM_FIELDS_JOB) {
		return E_INVALID_ENTRY_FORMAT;
	}

	// Get CIF's company
	char companyCif[256];
	csv_getAsString(entry, 7, companyCif, sizeof(companyCif));

	// Search for company in data app
	tCompany* company = companyList_find(data->companies, companyCif);
	if (company == NULL) {
		return E_COMPANY_NOT_FOUND;
	}

	// Parses and add to jobs
	tJob newJob;
	job_parse(&newJob, companyCif, entry);

	// Set the company pointer to a job
	newJob.company = company;

	// Add new job
	jobs_add(&(company->jobs), newJob);

	return E_SUCCESS;

	/////////////////////////////////
    //return E_NOT_IMPLEMENTED;
}

// Get the number of people registered on the application
int api_peopleCount(tApiData data) {
	/////////////////////////////////
	// PR1_3f
	/////////////////////////////////
	// Return number of people in data
	return people_count(data.people);
	/////////////////////////////////
    //return -1;
}

// Get the number of companies registered on the application
int api_companiesCount(tApiData data) {
	/////////////////////////////////
	// PR1_3g
	/////////////////////////////////
	// Return number of companies in data
	return companyList_len(data.companies);
	/////////////////////////////////
    //return -1;
}

// Get the number of jobs registered on the application
int api_jobsCount(tApiData data) {
	/////////////////////////////////
	// PR1_3h
	/////////////////////////////////

	// Initialize aux var to return
	int totalJobs = 0;
	tCompanyListNode* current = data.companies.first;

	// Loops company list til last node
	while (current != NULL) {
		// Count total jobs
		totalJobs += current->elem.jobs.count;
		// Move forwards
		current = current->next;
	}

	return totalJobs;
	/////////////////////////////////
    //return -1;
}

// Free all used memory
tApiError api_freeData(tApiData* data) {
	/////////////////////////////////
	// PR1_3i
	/////////////////////////////////

	// Check if data is not empty
	if (data == NULL) {
		return E_MEMORY_ERROR;
	}

	// Release all memory allocated for people data
	tApiError error = people_free(&(data->people));
	if (error != E_SUCCESS) {
		return error;
	}

	// Release all memory allocated for companies data
	error = companyList_free(&(data->companies));
	if (error != E_SUCCESS) {
		return error;
	}

	// Reinitialize data structure
	error = api_initData(data);
	if (error != E_SUCCESS) {
		return error;
	}

	return E_SUCCESS;
    /////////////////////////////////
    //return E_NOT_IMPLEMENTED;
}

// Add a new entry
tApiError api_addDataEntry(tApiData* data, tCSVEntry entry) {
	/////////////////////////////////
	// PR1_3j
	/////////////////////////////////

	// Check if data is not empty
	if (data == NULL) {
		return E_MEMORY_ERROR;
	}

	// Get type of CSV entry
	const char* entryType = csv_getType(&entry);

	// Check if entry type is PERSON
	if (strcmp(entryType, "PERSON") == 0) {
		return api_addPerson(data, entry);
	}
	// Check if entry type is COMPANY
	else if (strcmp(entryType, "COMPANY") == 0) {
		return api_addCompany(data, entry);
	}
	// Check if entry type is JOB
	else if (strcmp(entryType, "JOB") == 0) {
		return api_addJob(data, entry);
	}
	// Invalid entry type
	else {
		return E_INVALID_ENTRY_TYPE;
	}
    /////////////////////////////////
    //return E_NOT_IMPLEMENTED;
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
