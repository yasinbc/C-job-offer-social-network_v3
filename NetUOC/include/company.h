#ifndef __COMPANY_H__
#define __COMPANY_H__
#include <stdbool.h>
#include "date.h"
#include "job.h"
#include "person.h"

#define NUM_FIELDS_COMPANY 7

typedef struct _tCompanyInfo {
	char *cif;
	char *name;
	tDate foundation;
	int minSize;
	int maxSize;
	bool hasAIEnabled;
} tCompanyInfo;

typedef struct _tCompany {
	tCompanyInfo info;
	tPerson *founder;
	tJobs jobs;
} tCompany;

typedef struct _tCompanyListNode {
	tCompany elem;
	struct _tCompanyListNode *next;
} tCompanyListNode;

typedef struct _tCompanyList {
	tCompanyListNode *first;
	tCompanyListNode *last;
	int size;
} tCompanyList;

//////////////////////////////////
// Available functions
//////////////////////////////////

// Initialize a company info
void companyInfo_init(tCompanyInfo *info, const char *cif, const char *name, tDate foundation, int minSize, int maxSize, bool hasAIEnabled);

// Release the company info
void companyInfo_free(tCompanyInfo *info);

// Parse input from CSVEntry
void company_parse(tCompany *data, char *founderDocument, tCSVEntry entry);

// Get company data using a string
void company_get(tCompany data, char* buffer);

// Release all the allocated memory
void company_free(tCompany *data);

// Initialize a list of companies
tApiError companyList_init(tCompanyList *list);

// Return the number of companies of the list
int companyList_len(tCompanyList list);

// Search a company by cif
tCompany* companyList_find(tCompanyList list, const char *cif);

// Add a company into the list
tApiError companyList_add(tCompanyList *list, tCompany company, tPerson *founder);

// Remove a company from the list
tApiError companyList_del(tCompanyList *list, const char* cif);

// Remove all the companies from the list
tApiError companyList_free(tCompanyList *list);

#endif
