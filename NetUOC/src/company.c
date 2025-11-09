#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "company.h"

#include <time.h>

#include "csv.h"

//////////////////////////////////
// Company info functions
//////////////////////////////////

// Initialize a company info
void companyInfo_init(tCompanyInfo *info, const char *cif, const char *name, tDate foundation, int minSize, int maxSize, bool hasAIEnabled) {
	// Check input data
	assert(info != NULL);
	assert(cif != NULL);
	assert(name != NULL);
	assert(minSize > 0);
	assert(maxSize >= minSize);

	// CIF
	info->cif = (char*)malloc(sizeof(char) * (strlen(cif) + 1));
	assert(info->cif != NULL);
	strcpy(info->cif, cif);

	// Name
	info->name = (char*)malloc(sizeof(char) * (strlen(name) + 1));
	assert(info->name != NULL);
	strcpy(info->name, name);

	// Foundation
	date_cpy(&(info->foundation), foundation);

	// Size of the company
	info->minSize = minSize;
	info->maxSize = maxSize;

	// Has AI enabled
	info->hasAIEnabled = hasAIEnabled;
}

// Release the company info
void companyInfo_free(tCompanyInfo *info) {
	// Check input data
	assert(info != NULL);

	if (info->cif != NULL) {
		free(info->cif);
		info->cif = NULL;
	}

	if (info->name != NULL) {
		free(info->name);
		info->name = NULL;
	}
}

//////////////////////////////////
// Company functions
//////////////////////////////////

// Parse input from CSVEntry
void company_parse(tCompany *data, char *founderDocument, tCSVEntry entry) {
	// Check input data
	assert(data != NULL);

	// Check entry fields
    assert(csv_numFields(entry) == NUM_FIELDS_COMPANY);

	int pos = 0;

    // CIF
    const char *cif = entry.fields[pos++];

    // Name
    const char *name = entry.fields[pos++];

    // Foundation date
    assert(strlen(entry.fields[pos]) == DATE_LENGTH);
    tDate foundation;
    date_parse(&foundation, entry.fields[pos++]);

    // minSize
    int minSize = csv_getAsInteger(entry, pos++);

    // maxSize
    int maxSize = csv_getAsInteger(entry, pos++);

    // hasAIEnabled
    bool hasAIEnabled = csv_getAsInteger(entry, pos++) == 1;

    // Initialize its info
    companyInfo_init(&(data->info), cif, name, foundation, minSize, maxSize, hasAIEnabled);

    // Initialize the founder
    data->founder = NULL;

	// Initialize the jobs
	jobs_init(&(data->jobs));

    // Copy the founder document
    strcpy(founderDocument, entry.fields[pos++]);
}

// Get company data using a string
void company_get(tCompany data, char* buffer) {
	// Print all data at same time
	sprintf(buffer, "%s;%s;%02d/%02d/%04d;%d;%d;%d;%s",
		data.info.cif,
		data.info.name,
		data.info.foundation.day, data.info.foundation.month, data.info.foundation.year,
		data.info.minSize,
		data.info.maxSize,
		data.info.hasAIEnabled,
		data.founder->document
	);
}

// Release all the allocated memory
void company_free(tCompany *data) {
	// Check preconditions
	assert(data != NULL);

	// Release the allocated memory from company info
	companyInfo_free(&(data->info));

    // Release the allocated memory from jobs
    jobs_free(&(data->jobs));
}

//////////////////////////////////
// Company list functions
//////////////////////////////////

// Initialize a list of companies
tApiError companyList_init(tCompanyList *list) {
	/////////////////////////////////
	// PR1_2a
	/////////////////////////////////
	//Check if list is not empty
	assert(list != NULL);

	//Initialize first node as empty
	list->first = NULL;
	//Initialize last node as empty
	list->last = NULL;
	//Initialize size as 0
	list->size = 0;
	/////////////////////////////////
	//return E_NOT_IMPLEMENTED;
	return E_SUCCESS;
}

// Return the number of companies of the list
int companyList_len(tCompanyList list) {
	/////////////////////////////////
	// PR1_2b
	/////////////////////////////////
	// Just return the size field in the list structure
	return list.size;
	/////////////////////////////////
	//return -1;
}

// Search a company by cif
tCompany* companyList_find(tCompanyList list, const char *cif) {
	/////////////////////////////////
	// PR1_2c
	/////////////////////////////////

	tCompanyListNode* currentNode = list.first;

	// Loops til the end of the list
	while (currentNode != NULL) {
		// Compares CIFs
		if (strcmp(currentNode->elem.info.cif, cif) == 0) {
			return &(currentNode->elem);
		}
		// Move to next node
		currentNode = currentNode->next;
	}

	/*
	tCompanyListNode* currentNode = list.first;

	while (currentNode != NULL) {
		if (strcmp(currentNode->elem.info.cif, cif) == 0) {
			return &(currentNode->elem);
		}
	}
	currentNode = currentNode->next;
	*/

	/////////////////////////////////
	return NULL;
}

// Add a company into the list
tApiError companyList_add(tCompanyList *list, tCompany company, tPerson *founder) {
	/////////////////////////////////
	// PR1_2d
	/////////////////////////////////

	// Validate input pointers
	if (list == NULL || company.info.cif == NULL) {
		return E_MEMORY_ERROR;
	}

	// Verify if company exists already in the list
	if (companyList_find(*list, company.info.cif) != NULL) {
		return E_COMPANY_DUPLICATED;
	}

	// Creates a new node to the list
	tCompanyListNode *newNode = (tCompanyListNode *)malloc(sizeof(tCompanyListNode));
	if (newNode == NULL) {
		return E_MEMORY_ERROR;
	}

	// Paso 4: Inicializar el nuevo nodo
	// Initialize and copy data from company to the new node
	newNode->elem = company;

	// Sets the founder to new node
	newNode->elem.founder = founder;

	// Initialize the next pointer
	newNode->next = NULL;

	// Inserts the new node to the end of the list
	if (list->first == NULL) {
		// If empty list the new node is first not last
		list->first = newNode;
		list->last = newNode;
	} else {
		// If list is not empty it links to the end
		if (list->last == NULL) {
			free(newNode);
			return E_MEMORY_ERROR;
		}
		list->last->next = newNode;
		list->last = newNode;
	}

	// Updates memory size
	list->size++;

	return E_SUCCESS;
	/////////////////////////////////
	//return E_NOT_IMPLEMENTED;
}

// Remove a company from the list
tApiError companyList_del(tCompanyList *list, const char* cif) {
	/////////////////////////////////
	// PR1_2e
	/////////////////////////////////

	// Validate input pointers
	if (list == NULL || cif == NULL) {
		return E_MEMORY_ERROR;
	}

	// In case of list is empty
	if (list->first == NULL) {
		return E_COMPANY_NOT_FOUND;
	}

	tCompanyListNode *current = list->first;
	tCompanyListNode *previous = NULL;

	// Search for company to delete
	while (current != NULL) {
		// If company exists
		if (strcmp(current->elem.info.cif, cif) == 0) {
			// Set next node with next node
			tCompanyListNode *nextNode = current->next;

			// Update pointers
			if (previous == NULL) {
				// First node
				list->first = nextNode;
				if (list->first == NULL) {
					list->last = NULL;
				}
			} else {
				// Middle node or last one
				previous->next = nextNode;
				if (nextNode == NULL) {
					list->last = previous;
				}
			}

			free(current);

			// Update current size
			list->size--;

			return E_SUCCESS;
		}

		// Move toward
		previous = current;
		current = current->next;
	}

	return E_COMPANY_NOT_FOUND;
	/////////////////////////////////
	//return E_NOT_IMPLEMENTED;
}

// Remove all the companies from the list
tApiError companyList_free(tCompanyList *list) {
	/////////////////////////////////
	// PR1_2f
	/////////////////////////////////

	assert(list != NULL);

	// If list is empty
	if (list == NULL) {
		return E_MEMORY_ERROR;
	}

	tCompanyListNode *current = list->first;
	tCompanyListNode *nextNode = NULL;

	// // Loop the list til it is empty
	while (current != NULL) {
		// Save next node
		nextNode = current->next;

		// Free current allocated memory
		free(current);

		// Move toward
		current = nextNode;
	}

	// Reinitilize list
	list->first = NULL;
	list->last = NULL;
	list->size = 0;

	return E_SUCCESS;

	/////////////////////////////////
	//return E_NOT_IMPLEMENTED;
}
