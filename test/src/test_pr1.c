#include "test_pr1.h"
#include "api.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

// Run all tests for PR1
bool run_pr1(tTestSuite *test_suite, const char *input) {
    bool ok = true;
    tTestSection* section = NULL;

    assert(test_suite != NULL);

    testSuite_addSection(test_suite, "PR1", "Tests for PR1 exercices");

    section = testSuite_getSection(test_suite, "PR1");
    assert(section != NULL);

    ok = run_pr1_ex1(section, input);
    ok = run_pr1_ex2(section, input) && ok;
    ok = run_pr1_ex3(section, input) && ok;
    ok = run_pr1_ex4(section, input) && ok;

    return ok;
}

// Run all tests for Exercice 1 of PR1
bool run_pr1_ex1(tTestSection *test_section, const char *input) {
	bool passed = true, failed = false;
	const char *version;

	/////////////////////////////
	/////  PR1 EX1 TEST 1  //////
	/////////////////////////////
	failed = false;
	start_test(test_section, "PR1_EX1_1", "Read version information.", 0.0);
	// Get the version
	version = api_version();
	if (strcmp(version, "UOC PP 20251") != 0) {
		failed = true;
		passed = false;
	}
	end_test(test_section, "PR1_EX1_1", !failed);

	return passed;
}

// Run all tests for Exercice 2 of PR1
bool run_pr1_ex2(tTestSection *test_section, const char *input) {
	tApiError error;
	tDate foundation;
	tPerson person1, person2;
	tCompany company1, company2, company3;
	tCompanyList companyList;
	tCSVData test_people;
	
	bool passed = true;
	bool failed = false;
	bool fail_all = false;
	
	// Intialize people
	const char* test_people_str = 	"00000000A;Gabriel;Ferrate;987654321;gferrate@uoc.edu;Rambla de Poblenou, 154-156;08001;03/03/1932\n" \
									"98765432J;Hendrik;Lorentz;987654321;hendrik.lorentz@example.com;his street, 5;00001;27/08/1954\n";
	csv_init(&test_people);
	csv_parse(&test_people, test_people_str, "PEOPLE");
	person_parse(&person1, csv_getEntry(test_people, 0)[0]);
	person_parse(&person2, csv_getEntry(test_people, 1)[0]);
	
	// Initialize company1
	foundation.day = 6;
	foundation.month = 10;
	foundation.year = 1994;
	company1.founder = NULL;
	jobs_init(&(company1.jobs));
	companyInfo_init(&(company1.info), "G60667813", "Universitat Oberta de Catalunya", foundation, 1000, 9999, true);
	
	// Initialize company2
	foundation.day = 1;
	foundation.month = 1;
	foundation.year = 2000;
	company2.founder = NULL;
	jobs_init(&(company2.jobs));
	companyInfo_init(&(company2.info), "N9660589D", "Company 2", foundation, 10, 99, false);
	
	// Initialize company3
	foundation.day = 10;
	foundation.month = 12;
	foundation.year = 2002;
	company3.founder = NULL;
	jobs_init(&(company3.jobs));
	companyInfo_init(&(company3.info), "J75797183", "Company 3", foundation, 100, 999, false);
	
	/////////////////////////////
	/////  PR1 EX2 TEST 1  //////
	/////////////////////////////
	failed = false;
	start_test(test_section, "PR1_EX2_1", "Initialize the company list", 0.15);
	// Initialize the company list
	error = companyList_init(&companyList);
	if (error != E_SUCCESS) {
		failed = true;
		passed = false;
		fail_all = true;
	} else {
		if (companyList.first != NULL || companyList.last != NULL || companyList.size != 0) {
			failed = true;
			passed = false;
			fail_all = true;
		}
	}
	end_test(test_section, "PR1_EX2_1", !failed);
	
	/////////////////////////////
	/////  PR1 EX2 TEST 2  //////
	/////////////////////////////
	failed = false;
	start_test(test_section, "PR1_EX2_2", "Add a company into the list", 0.5);
	if (fail_all) {
		failed = true;
	} else {
		// Add a company to an empty list
		error = companyList_add(&companyList, company1, &person1);
		if (error != E_SUCCESS) {
			failed = true;
			passed = false;
			fail_all = true;
		} else {
			if (companyList.first == NULL || companyList.last == NULL || companyList.size != 1) {
				failed = true;
				passed = false;
				fail_all = true;
			} else if (companyList.first->next != NULL || strcmp(companyList.first->elem.info.cif, company1.info.cif) != 0
					|| companyList.first->elem.founder != &person1 || companyList.first->elem.jobs.count != 0 || companyList.first->elem.jobs.elems != NULL) {
				failed = true;
				passed = false;
				fail_all = true;
			} else if (companyList.last->next != NULL || strcmp(companyList.last->elem.info.cif, company1.info.cif) != 0
					|| companyList.last->elem.founder != &person1 || companyList.last->elem.jobs.count != 0 || companyList.last->elem.jobs.elems != NULL) {
				failed = true;
				passed = false;
				fail_all = true;
			} else if (companyList.first != companyList.last) {
				failed = true;
				passed = false;
				fail_all = true;
			}
		}
	}
	end_test(test_section, "PR1_EX2_2", !failed);
	
	/////////////////////////////
	/////  PR1 EX2 TEST 3  //////
	/////////////////////////////
	failed = false;
	start_test(test_section, "PR1_EX2_3", "Add a duplicated company to the list", 0.25);
	if (fail_all) {
		failed = true;
	} else {
		// Add a duplicated company to the list
		error = companyList_add(&companyList, company1, &person1);
		if (error != E_COMPANY_DUPLICATED) {
			failed = true;
			passed = false;
			fail_all = true;
		} else {
			if (companyList.first == NULL || companyList.last == NULL || companyList.size != 1) {
				failed = true;
				passed = false;
				fail_all = true;
			} else if (companyList.first->next != NULL || strcmp(companyList.first->elem.info.cif, company1.info.cif) != 0
					|| companyList.first->elem.founder != &person1 || companyList.first->elem.jobs.count != 0 || companyList.first->elem.jobs.elems != NULL) {
				failed = true;
				passed = false;
				fail_all = true;
			} else if (companyList.last->next != NULL || strcmp(companyList.last->elem.info.cif, company1.info.cif) != 0
					|| companyList.last->elem.founder != &person1 || companyList.last->elem.jobs.count != 0 || companyList.last->elem.jobs.elems != NULL) {
				failed = true;
				passed = false;
				fail_all = true;
			} else if (companyList.first != companyList.last) {
				failed = true;
				passed = false;
				fail_all = true;
			}
		}
	}
	end_test(test_section, "PR1_EX2_3", !failed);
	
	/////////////////////////////
	/////  PR1 EX2 TEST 4  //////
	/////////////////////////////
	failed = false;
	start_test(test_section, "PR1_EX2_4", "Add a company to a non-empty list", 0.5);
	if (fail_all) {
		failed = true;
	} else {
		// Add a company to a non-empty list
		error = companyList_add(&companyList, company2, &person2);
		if (error != E_SUCCESS) {
			failed = true;
			passed = false;
			fail_all = true;
		} else {
			if (companyList.first == NULL || companyList.last == NULL || companyList.size != 2) {
				failed = true;
				passed = false;
				fail_all = true;
			} else if (companyList.first->next == NULL || strcmp(companyList.first->elem.info.cif, company1.info.cif) != 0
					|| companyList.first->elem.founder != &person1 || companyList.first->elem.jobs.count != 0 || companyList.first->elem.jobs.elems != NULL) {
				failed = true;
				passed = false;
				fail_all = true;
			} else if (companyList.first->next->next != NULL || strcmp(companyList.first->next->elem.info.cif, company2.info.cif) != 0
					|| companyList.first->next->elem.founder != &person2 || companyList.first->next->elem.jobs.count != 0 || companyList.first->next->elem.jobs.elems != NULL) {
				failed = true;
				passed = false;
				fail_all = true;
			} else if (companyList.last->next != NULL || strcmp(companyList.last->elem.info.cif, company2.info.cif) != 0
					|| companyList.last->elem.founder != &person2 || companyList.last->elem.jobs.count != 0 || companyList.last->elem.jobs.elems != NULL) {
				failed = true;
				passed = false;
				fail_all = true;
			} else if (companyList.first == companyList.last) {
				failed = true;
				passed = false;
				fail_all = true;
			}
		}
	}
	end_test(test_section, "PR1_EX2_4", !failed);
	
	/////////////////////////////
	/////  PR1 EX2 TEST 5  //////
	/////////////////////////////
	failed = false;
	start_test(test_section, "PR1_EX2_5", "Get the number of companies", 0.1);
	if (fail_all) {
		failed = true;
	} else {
		if (companyList_len(companyList) != 2) {
			failed = true;
			passed = false;
			fail_all = true;
		}
		
		// Add another company
		companyList_add(&companyList, company3, &person2);
		
		if (companyList_len(companyList) != 3) {
			failed = true;
			passed = false;
			fail_all = true;
		}
	}
	end_test(test_section, "PR1_EX2_5", !failed);
	
	/////////////////////////////
	/////  PR1 EX2 TEST 6  //////
	/////////////////////////////
	failed = false;
	start_test(test_section, "PR1_EX2_6", "Find a company by its CIF", 0.3);
	if (fail_all) {
		failed = true;
	} else {
		if (companyList_find(companyList, "NON-EXIST") != NULL) {
			failed = true;
			passed = false;
			fail_all = true;
		} else if (companyList_find(companyList, "G60667813") == NULL) {
			failed = true;
			passed = false;
			fail_all = true;
		}
	}
	end_test(test_section, "PR1_EX2_6", !failed);
	
	/////////////////////////////
	/////  PR1 EX2 TEST 7  //////
	/////////////////////////////
	failed = false;
	start_test(test_section, "PR1_EX2_7", "Remove a non-existing company from the list", 0.3);
	if (fail_all) {
		failed = true;
	} else {
		// Remove a non-existing company from the list
		error = companyList_del(&companyList, "NON-EXIST");
		if (error != E_COMPANY_NOT_FOUND) {
			failed = true;
			passed = false;
			fail_all = true;
		} else {
			if (companyList.first == NULL || companyList.last == NULL || companyList.size != 3) {
				failed = true;
				passed = false;
				fail_all = true;
			} else if (companyList.first->next == NULL || strcmp(companyList.first->elem.info.cif, company1.info.cif) != 0
					|| companyList.first->elem.founder != &person1 || companyList.first->elem.jobs.count != 0 || companyList.first->elem.jobs.elems != NULL) {
				failed = true;
				passed = false;
				fail_all = true;
			} else if (companyList.first->next->next == NULL || strcmp(companyList.first->next->elem.info.cif, company2.info.cif) != 0
					|| companyList.first->next->elem.founder != &person2 || companyList.first->next->elem.jobs.count != 0 || companyList.first->next->elem.jobs.elems != NULL) {
				failed = true;
				passed = false;
				fail_all = true;
			} else if (companyList.first->next->next->next != NULL || strcmp(companyList.first->next->next->elem.info.cif, company3.info.cif) != 0
					|| companyList.first->next->next->elem.founder != &person2 || companyList.first->next->next->elem.jobs.count != 0 || companyList.first->next->next->elem.jobs.elems != NULL) {
				failed = true;
				passed = false;
				fail_all = true;
			}  else if (companyList.last->next != NULL || strcmp(companyList.last->elem.info.cif, company3.info.cif) != 0
					|| companyList.last->elem.founder != &person2 || companyList.last->elem.jobs.count != 0 || companyList.last->elem.jobs.elems != NULL) {
				failed = true;
				passed = false;
				fail_all = true;
			} else if (companyList.first == companyList.last) {
				failed = true;
				passed = false;
				fail_all = true;
			}
		}
	}
	end_test(test_section, "PR1_EX2_7", !failed);
	
	/////////////////////////////
	/////  PR1 EX2 TEST 8  //////
	/////////////////////////////
	failed = false;
	start_test(test_section, "PR1_EX2_8", "Remove a company from the list", 0.7);
	if (fail_all) {
		failed = true;
	} else {
		// Remove a company from the list
		error = companyList_del(&companyList, "N9660589D");
		if (error != E_SUCCESS) {
			failed = true;
			passed = false;
			fail_all = true;
		} else {
			if (companyList.first == NULL || companyList.last == NULL || companyList.size != 2) {
				failed = true;
				passed = false;
				fail_all = true;
			} else if (companyList.first->next == NULL || strcmp(companyList.first->elem.info.cif, company1.info.cif) != 0
					|| companyList.first->elem.founder != &person1 || companyList.first->elem.jobs.count != 0 || companyList.first->elem.jobs.elems != NULL) {
				failed = true;
				passed = false;
				fail_all = true;
			} else if (companyList.first->next->next != NULL || strcmp(companyList.first->next->elem.info.cif, company3.info.cif) != 0
					|| companyList.first->next->elem.founder != &person2 || companyList.first->next->elem.jobs.count != 0 || companyList.first->next->elem.jobs.elems != NULL) {
				failed = true;
				passed = false;
				fail_all = true;
			} else if (companyList.last->next != NULL || strcmp(companyList.last->elem.info.cif, company3.info.cif) != 0
					|| companyList.last->elem.founder != &person2 || companyList.last->elem.jobs.count != 0 || companyList.last->elem.jobs.elems != NULL) {
				failed = true;
				passed = false;
				fail_all = true;
			} else if (companyList.first == companyList.last) {
				failed = true;
				passed = false;
				fail_all = true;
			}
		}
		
		error = companyList_del(&companyList, "J75797183");
		if (error != E_SUCCESS) {
			failed = true;
			passed = false;
			fail_all = true;
		} else {
			if (companyList.first == NULL || companyList.last == NULL || companyList.size != 1) {
				failed = true;
				passed = false;
				fail_all = true;
			} else if (companyList.first->next != NULL || strcmp(companyList.first->elem.info.cif, company1.info.cif) != 0
					|| companyList.first->elem.founder != &person1 || companyList.first->elem.jobs.count != 0 || companyList.first->elem.jobs.elems != NULL) {
				failed = true;
				passed = false;
				fail_all = true;
			} else if (companyList.last->next != NULL || strcmp(companyList.last->elem.info.cif, company1.info.cif) != 0
					|| companyList.last->elem.founder != &person1 || companyList.last->elem.jobs.count != 0 || companyList.last->elem.jobs.elems != NULL) {
				failed = true;
				passed = false;
				fail_all = true;
			} else if (companyList.first != companyList.last) {
				failed = true;
				passed = false;
				fail_all = true;
			}
		}
		
		error = companyList_del(&companyList, "G60667813");
		if (error != E_SUCCESS) {
			failed = true;
			passed = false;
			fail_all = true;
		} else {
			if (companyList.first != NULL || companyList.last != NULL || companyList.size != 0) {
				failed = true;
				passed = false;
				fail_all = true;
			}
		}
	}
	end_test(test_section, "PR1_EX2_8", !failed);
	
	/////////////////////////////
	/////  PR1 EX2 TEST 9  //////
	/////////////////////////////
	failed = false;
	start_test(test_section, "PR1_EX2_9", "Remove all films from the catalog", 0.2);
	if (fail_all) {
		failed = true;
	} else {
		// Add data to the list
		companyList_add(&companyList, company1, &person1);
		companyList_add(&companyList, company2, &person2);
		
		// Remove all the data from the list
		error = companyList_free(&companyList);
		if (error != E_SUCCESS) {
			failed = true;
			passed = false;
			fail_all = true;
		} else if (companyList.first != NULL || companyList.last != NULL || companyList.size != 0) {
			failed = true;
			passed = false;
			fail_all = true;
		}
	}
	end_test(test_section, "PR1_EX2_9", !failed);
	
	// Release all data
	companyList_free(&companyList);
	company_free(&company1);
	company_free(&company2);
	company_free(&company3);
	person_free(&person1);
	person_free(&person2);
	csv_free(&test_people);
	
	return passed;
}

// Run all tests for Exercice 3 of PR1
bool run_pr1_ex3(tTestSection *test_section, const char *input) {
	tApiData data;
	tApiError error;
	tCSVEntry entry;
	int nPeople;
	int nCompanies;
	int nJobs;
	
	bool passed = true;
	bool failed = false;
	bool fail_all = false;
	
	/////////////////////////////
	/////  PR1 EX3 TEST 1  //////
	/////////////////////////////
	failed = false;
	start_test(test_section, "PR1_EX3_1", "Initialize the API data structure", 0.25);
	// Initialize the data
	error = api_initData(&data);
	if (error != E_SUCCESS) {
		failed = true;
		passed = false;
		fail_all = true;
	}
	end_test(test_section, "PR1_EX3_1", !failed);
	
	/////////////////////////////
	/////  PR1 EX3 TEST 2  //////
	/////////////////////////////
	failed = false;
	start_test(test_section, "PR1_EX3_2", "Add an invalid person", 0.2);
	if (fail_all) {
		failed = true;
	} else {
		csv_initEntry(&entry);
		csv_parseEntry(&entry, "98765432J;Hendrik;Lorentz;987654321;hendrik.lorentz@example.com;his street, 5;00001;27/08/1954", "PERSONA");
		
		// Add an invalid person type
		error = api_addPerson(&data, entry);
		if (error != E_INVALID_ENTRY_TYPE) {
			failed = true;
			passed = false;
			fail_all = true;
		}
		
		csv_freeEntry(&entry);
		
		csv_initEntry(&entry);
		csv_parseEntry(&entry, "98765432J;Hendrik;Lorentz;987654321;hendrik.lorentz@example.com;his street, 5;00001;27/08/1954;oops", "PERSON");
		
		// Add an invalid person format
		error = api_addPerson(&data, entry);
		if (error != E_INVALID_ENTRY_FORMAT) {
			failed = true;
			passed = false;
			fail_all = true;
		}
		
		csv_freeEntry(&entry);
		
		csv_initEntry(&entry);
		csv_parseEntry(&entry, "98765432J;Hendrik;Lorentz;987654321;hendrik.lorentz@example.com;his street, 5;27/08/1954", "PERSON");
		
		// Add an invalid person format
		error = api_addPerson(&data, entry);
		if (error != E_INVALID_ENTRY_FORMAT) {
			failed = true;
			passed = false;
			fail_all = true;
		}
		
		csv_freeEntry(&entry);
	}
	end_test(test_section, "PR1_EX3_2", !failed);
	
	/////////////////////////////
	/////  PR1 EX3 TEST 3  //////
	/////////////////////////////
	failed = false;
	start_test(test_section, "PR1_EX3_3", "Add a valid person", 0.2);
	if (fail_all) {
		failed = true;
	} else {
		csv_initEntry(&entry);
		csv_parseEntry(&entry, "98765432J;Hendrik;Lorentz;987654321;hendrik.lorentz@example.com;his street, 5;00001;27/08/1954", "PERSON");
		
		// Add a valid person
		error = api_addPerson(&data, entry);
		if (error != E_SUCCESS) {
			failed = true;
			passed = false;
			fail_all = true;
		}
		
		csv_freeEntry(&entry);
	}
	end_test(test_section, "PR1_EX3_3", !failed);
	
	/////////////////////////////
	/////  PR1 EX3 TEST 4  //////
	/////////////////////////////
	failed = false;
	start_test(test_section, "PR1_EX3_4", "Add a duplicated person", 0.1);
	if (fail_all) {
		failed = true;
	} else {
		csv_initEntry(&entry);
		csv_parseEntry(&entry, "98765432J;Hendrik;Lorentz;987654321;hendrik.lorentz@example.com;his street, 5;00001;27/08/1954", "PERSON");
		
		// Add a duplicated person
		error = api_addPerson(&data, entry);
		
		if (error != E_PERSON_DUPLICATED) {
			failed = true;
			passed = false;
			fail_all = true;
		}
		
		csv_freeEntry(&entry);
	}
	end_test(test_section, "PR1_EX3_4", !failed);
	
	/////////////////////////////
	/////  PR1 EX3 TEST 5  //////
	/////////////////////////////
	failed = false;
	start_test(test_section, "PR1_EX3_5", "Add an invalid company", 0.2);
	if (fail_all) {
		failed = true;
	} else {
		csv_initEntry(&entry);
		csv_parseEntry(&entry, "G60667813;Universitat Oberta de Catalunya;06/10/1994;1000;9999;1;98765432J", "COMPANYY");
		
		// Add an invalid company type
		error = api_addCompany(&data, entry);
		if (error != E_INVALID_ENTRY_TYPE) {
			failed = true;
			passed = false;
			fail_all = true;
		}
		
		csv_freeEntry(&entry);
		
		csv_initEntry(&entry);
		csv_parseEntry(&entry, "G60667813;Universitat Oberta de Catalunya;06/10/1994;1000;9999;1;98765432J;oops", "COMPANY");
		
		// Add an invalid company format
		error = api_addCompany(&data, entry);
		if (error != E_INVALID_ENTRY_FORMAT) {
			failed = true;
			passed = false;
			fail_all = true;
		}
		
		csv_freeEntry(&entry);
		
		csv_initEntry(&entry);
		csv_parseEntry(&entry, "G60667813;Universitat Oberta de Catalunya;06/10/1994;1000;9999;1", "COMPANY");
		
		// Add an invalid company format
		error = api_addCompany(&data, entry);
		if (error != E_INVALID_ENTRY_FORMAT) {
			failed = true;
			passed = false;
			fail_all = true;
		}
		
		csv_freeEntry(&entry);
	}
	end_test(test_section, "PR1_EX3_5", !failed);
	
	/////////////////////////////
	/////  PR1 EX3 TEST 6  //////
	/////////////////////////////
	failed = false;
	start_test(test_section, "PR1_EX3_6", "Add a valid company", 0.2);
	if (fail_all) {
		failed = true;
	} else {
		csv_initEntry(&entry);
		csv_parseEntry(&entry, "G60667813;Universitat Oberta de Catalunya;06/10/1994;1000;9999;1;98765432J", "COMPANY");
		
		// Add a valid company
		error = api_addCompany(&data, entry);
		if (error != E_SUCCESS) {
			failed = true;
			passed = false;
			fail_all = true;
		}
		
		csv_freeEntry(&entry);
	}
	end_test(test_section, "PR1_EX3_6", !failed);
	
	/////////////////////////////
	/////  PR1 EX3 TEST 7  //////
	/////////////////////////////
	failed = false;
	start_test(test_section, "PR1_EX3_7", "Add a duplicated company", 0.2);
	if (fail_all) {
		failed = true;
	} else {
		csv_initEntry(&entry);
		csv_parseEntry(&entry, "G60667813;Universitat Oberta de Catalunya;06/10/1994;1000;9999;1;98765432J", "COMPANY");
		
		// Add a duplicated company
		error = api_addCompany(&data, entry);
		
		if (error != E_COMPANY_DUPLICATED) {
			failed = true;
			passed = false;
			fail_all = true;
		}
		
		csv_freeEntry(&entry);
	}
	end_test(test_section, "PR1_EX3_7", !failed);
	
	/////////////////////////////
	/////  PR1 EX3 TEST 8  //////
	/////////////////////////////
	failed = false;
	start_test(test_section, "PR1_EX3_8", "Add a company with non-existing person", 0.2);
	if (fail_all) {
		failed = true;
	} else {
		csv_initEntry(&entry);
		csv_parseEntry(&entry, "B90234567;TechNova Solutions;15/03/2010;50;300;1;NON-EXIST", "COMPANY");
		
		// Add a valid company with non-exist person
		error = api_addCompany(&data, entry);
		if (error != E_PERSON_NOT_FOUND) {
			failed = true;
			passed = false;
			fail_all = true;
		}
		
		csv_freeEntry(&entry);
	}
	end_test(test_section, "PR1_EX3_8", !failed);
	
	/////////////////////////////
	/////  PR1 EX3 TEST 9  //////
	/////////////////////////////
	failed = false;
	start_test(test_section, "PR1_EX3_9", "Add an invalid job", 0.2);
	if (fail_all) {
		failed = true;
	} else {
		csv_initEntry(&entry);
		csv_parseEntry(&entry, "1;Junior software developer;1;01/10/2025;20/11/2025;20000;35000;G60667813", "JOV");
		
		// Add an invalid job type
		error = api_addJob(&data, entry);
		if (error != E_INVALID_ENTRY_TYPE) {
			failed = true;
			passed = false;
			fail_all = true;
		}
		
		csv_freeEntry(&entry);
		
		csv_initEntry(&entry);
		csv_parseEntry(&entry, "1;Junior software developer;1;01/10/2025;20/11/2025;20000;35000;G60667813;oops", "JOB");
		
		// Add an invalid job format
		error = api_addJob(&data, entry);
		if (error != E_INVALID_ENTRY_FORMAT) {
			failed = true;
			passed = false;
			fail_all = true;
		}
		
		csv_freeEntry(&entry);
		
		csv_initEntry(&entry);
		csv_parseEntry(&entry, "1;Junior software developer;1;01/10/2025;20/11/2025;20000;35000", "JOB");
		
		// Add an invalid job format
		error = api_addJob(&data, entry);
		if (error != E_INVALID_ENTRY_FORMAT) {
			failed = true;
			passed = false;
			fail_all = true;
		}
		
		csv_freeEntry(&entry);
	}
	end_test(test_section, "PR1_EX3_9", !failed);
	
	/////////////////////////////
	/////  PR1 EX3 TEST 10  //////
	/////////////////////////////
	failed = false;
	start_test(test_section, "PR1_EX3_10", "Add a valid job", 0.3);
	if (fail_all) {
		failed = true;
	} else {
		csv_initEntry(&entry);
		csv_parseEntry(&entry, "1;Junior software developer;1;01/10/2025;20/11/2025;20000;35000;G60667813", "JOB");
		
		// Add a valid job
		error = api_addJob(&data, entry);
		if (error != E_SUCCESS) {
			failed = true;
			passed = false;
			fail_all = true;
		}
		
		csv_freeEntry(&entry);
	}
	end_test(test_section, "PR1_EX3_10", !failed);
	
	/////////////////////////////
	/////  PR1 EX3 TEST 11  //////
	/////////////////////////////
	failed = false;
	start_test(test_section, "PR1_EX3_11", "Add a job with a non-existing company", 0.3);
	if (fail_all) {
		failed = true;
	} else {
		csv_initEntry(&entry);
		csv_parseEntry(&entry, "1;Junior software developer;1;01/10/2025;20/11/2025;20000;35000;NON-EXIST", "JOB");
		
		error = api_addJob(&data, entry);
		
		if (error != E_COMPANY_NOT_FOUND) {
			failed = true;
			passed = false;
			fail_all = true;
		}
		
		csv_freeEntry(&entry);
	}
	end_test(test_section, "PR1_EX3_11", !failed);
	
	/////////////////////////////
	/////  PR1 EX3 TEST 12  //////
	/////////////////////////////
	failed = false;
	start_test(test_section, "PR1_EX3_12", "Check the number of persons", 0.1);
	if (fail_all) {
		failed = true;
	} else {
		nPeople = api_peopleCount(data);
		if (nPeople != 1) {
			failed = true;
			passed = false;
			fail_all = true;
		} else {
			// Valid person
			csv_initEntry(&entry);
			csv_parseEntry(&entry, "33365111X;Marie;Curie;+33123456789;marie.curie@example.com;Radium street, 88;54321;07/10/1867", "PERSON");
			api_addPerson(&data, entry);
			csv_freeEntry(&entry);
			
			// Valid person
			csv_initEntry(&entry);
			csv_parseEntry(&entry, "55565432Z;Nils;Gustaf Dalen;555123123;nils.gustaf@example.com;Picadilly street, 123;12345;25/11/1972", "PERSON");
			api_addPerson(&data, entry);
			csv_freeEntry(&entry);
			
			// Duplicated person
			csv_initEntry(&entry);
			csv_parseEntry(&entry, "55565432Z;Nils;Gustaf Dalen;555123123;nils.gustaf@example.com;Picadilly street, 123;12345;25/11/1972", "PERSON");
			api_addPerson(&data, entry);
			csv_freeEntry(&entry);
			
			// Invalid person
			csv_initEntry(&entry);
			csv_parseEntry(&entry, "Nils;Gustaf Dalen;555123123;nils.gustaf@example.com;Picadilly street, 123;12345;25/11/1972", "PERSON");
			api_addPerson(&data, entry);
			csv_freeEntry(&entry);
			
			// Invalid person
			csv_initEntry(&entry);
			csv_parseEntry(&entry, "55565432Z;Nils;Gustaf Dalen;555123123;nils.gustaf@example.com;Picadilly street, 123;12345;25/11/1972", "PERSONA");
			api_addPerson(&data, entry);
			csv_freeEntry(&entry);
			
			nPeople = api_peopleCount(data);
			if (nPeople != 3) {
				failed = true;
				passed = false;
				fail_all = true;
			}
		}
	}
	end_test(test_section, "PR1_EX3_12", !failed);
	
	/////////////////////////////
	/////  PR1 EX3 TEST 13  //////
	/////////////////////////////
	failed = false;
	start_test(test_section, "PR1_EX3_13", "Check the number of companies", 0.1);
	if (fail_all) {
		failed = true;
	} else {
		nCompanies = api_companiesCount(data);
		if (nCompanies != 1) {
			failed = true;
			passed = false;
			fail_all = true;
		} else {
			// Valid company
			csv_initEntry(&entry);
			csv_parseEntry(&entry, "D60487219;NeuralEdge Systems;11/07/2018;10;80;1;33365111X", "COMPANY");
			api_addCompany(&data, entry);
			csv_freeEntry(&entry);
			
			// Duplicated company
			csv_initEntry(&entry);
			csv_parseEntry(&entry, "D60487219;NeuralEdge Systems;11/07/2018;10;80;1;33365111X", "COMPANY");
			api_addCompany(&data, entry);
			csv_freeEntry(&entry);
			
			// Invalid company
			csv_initEntry(&entry);
			csv_parseEntry(&entry, "E50938476;BlueWave Robotics;03/12/2016;30;200;1;98765432J", "COMPANYA");
			api_addCompany(&data, entry);
			csv_freeEntry(&entry);
			
			// Invalid company
			csv_initEntry(&entry);
			csv_parseEntry(&entry, "E50938476;BlueWave Robotics;03/12/2016;30;200;1", "COMPANY");
			api_addCompany(&data, entry);
			csv_freeEntry(&entry);
			
			// Non-existing person
			csv_initEntry(&entry);
			csv_parseEntry(&entry, "E50938476;BlueWave Robotics;03/12/2016;30;200;1;NON-EXISTING", "COMPANY");
			api_addCompany(&data, entry);
			csv_freeEntry(&entry);
			
			nCompanies = api_companiesCount(data);
			if (nCompanies != 2) {
				failed = true;
				passed = false;
				fail_all = true;
			}
		}
	}
	end_test(test_section, "PR1_EX3_13", !failed);
	
	/////////////////////////////
	/////  PR1 EX3 TEST 14 //////
	/////////////////////////////
	failed = false;
	start_test(test_section, "PR1_EX2_14", "Check the number of jobs", 0.25);
	if (fail_all) {
		failed = true;
	} else {
		nJobs = api_jobsCount(data);
		if (nJobs != 1) {
			failed = true;
			passed = false;
			fail_all = true;
		} else {
			// Valid company
			csv_initEntry(&entry);
			csv_parseEntry(&entry, "JOB;8;Computer scientist;1;07/08/2025;01/11/2025;15000;16500;C75891234", "JOB");
			api_addCompany(&data, entry);
			csv_freeEntry(&entry);
			
			// Duplicated company
			csv_initEntry(&entry);
			csv_parseEntry(&entry, "JOB;8;Computer scientist;1;07/08/2025;01/11/2025;15000;16500;C75891234", "JOB");
			api_addCompany(&data, entry);
			csv_freeEntry(&entry);
			
			// Invalid company
			csv_initEntry(&entry);
			csv_parseEntry(&entry, "JOB;9;CTO;1;07/08/2025;01/11/2025;47000;59500;D60487219", "JOBB");
			api_addCompany(&data, entry);
			csv_freeEntry(&entry);
			
			// Invalid company
			csv_initEntry(&entry);
			csv_parseEntry(&entry, "JOB;9;CTO;1;07/08/2025;01/11/2025;47000;59500", "JOB");
			api_addCompany(&data, entry);
			csv_freeEntry(&entry);
			
			// Non-existing company
			csv_initEntry(&entry);
			csv_parseEntry(&entry, "JOB;9;CTO;1;07/08/2025;01/11/2025;47000;59500;NON-EXISTING", "JOB");
			api_addCompany(&data, entry);
			csv_freeEntry(&entry);
			
			nCompanies = api_companiesCount(data);
			if (nCompanies != 2) {
				failed = true;
				passed = false;
				fail_all = true;
			}
		}
	}
	end_test(test_section, "PR1_EX2_14", !failed);
	
	/////////////////////////////
	/////  PR1 EX3 TEST 15 //////
	/////////////////////////////
	failed = false;
	start_test(test_section, "PR1_EX2_15", "Free API data", 0.2);
	if (fail_all) {
		failed = true;
	} else {
		error = api_freeData(&data);
		nPeople = api_peopleCount(data);
		nCompanies = api_companiesCount(data);
		nJobs = api_jobsCount(data);
		
		if (error != E_SUCCESS || nPeople != 0 || nCompanies != 0 || nJobs != 0) {
			failed = true;
			passed = false;
			fail_all = true;
		}
	}
	end_test(test_section, "PR1_EX2_15", !failed);
	
	/////////////////////////////
	/////  PR1 EX3 TEST 16 //////
	/////////////////////////////
	failed = false;
	start_test(test_section, "PR1_EX2_16", "Load data from file", 1.0);
	if (fail_all) {
		failed = true;
	} else {
		error = api_loadData(&data, input, true);
		nPeople = api_peopleCount(data);
		nCompanies = api_companiesCount(data);
		nJobs = api_jobsCount(data);
		
		if (error != E_SUCCESS || nPeople != 5 || nCompanies != 5 || nJobs != 9) {
			failed = true;
			passed = false;
			fail_all = true;
		}
	}
	end_test(test_section, "PR1_EX2_16", !failed);
	
	// Release all data
	api_freeData(&data);
	
	return passed;
}

// Run all tests for Exercice 4 of PR1
bool run_pr1_ex4(tTestSection *test_section, const char *input) {
	tApiData data;
	tApiError error;
	tCSVEntry entry;
	tCSVEntry refEntry;
	tCSVData report;
	tCSVData refReport;
	
	int nPeople;
	int nCompanies;
	int nJobs;
	
	bool passed = true;
	bool failed = false;
	bool fail_all = false;
	
	// Initialize the data
	error = api_initData(&data);
	if (error != E_SUCCESS) {
		passed = false;
		fail_all = true;
	}

	if (!fail_all) {
		error = api_loadData(&data, input, true);
		nPeople = api_peopleCount(data);
		nCompanies = api_companiesCount(data);
		nJobs = api_jobsCount(data);
		
		if (error != E_SUCCESS || nPeople != 5 || nCompanies != 5 || nJobs != 9) {
			passed = false;
			fail_all = true;
		}
	}
	
	/////////////////////////////
	/////  PR1 EX4 TEST 1  //////
	/////////////////////////////
	failed = false;
	start_test(test_section, "PR1_EX4_1", "Request a missing person", 0.3);
	if (fail_all) {
		failed = true;
	} else {
		csv_initEntry(&entry);
		error = api_getPerson(data, "NON-EXIST", &entry);
		if (error != E_PERSON_NOT_FOUND) {
			failed = true;
			passed = false;
		}
		csv_freeEntry(&entry);
	}
	end_test(test_section, "PR1_EX4_1", !failed);
	
	/////////////////////////////
	/////  PR1 EX4 TEST 2  //////
	/////////////////////////////
	failed = false;
	start_test(test_section, "PR1_EX4_2", "Request a valid person", 0.3);
	if (fail_all) {
		failed = true;
	} else {
		csv_initEntry(&entry);
		csv_initEntry(&refEntry);
		csv_parseEntry(&refEntry, "47051307Z;James;Bond;707070707;bond.jamesbond@example.com;Downing street, 10;07077;12/01/1952", "PERSON");
		error = api_getPerson(data, "47051307Z", &entry);
		if (error != E_SUCCESS || !csv_equalsEntry(entry, refEntry)) {
			failed = true;
			passed = false;
		}
		csv_freeEntry(&entry);
		csv_freeEntry(&refEntry);
	}
	end_test(test_section, "PR1_EX4_2", !failed);
	
	/////////////////////////////
	/////  PR1 EX4 TEST 3  //////
	/////////////////////////////
	failed = false;
	start_test(test_section, "PR1_EX4_3", "Request a missing company", 0.3);
	if (fail_all) {
		failed = true;
	} else {
		csv_initEntry(&entry);
		error = api_getCompany(data, "NON-EXIST", &entry);
		if (error != E_COMPANY_NOT_FOUND) {
			failed = true;
			passed = false;
		}
		csv_freeEntry(&entry);
	}
	end_test(test_section, "PR1_EX4_3", !failed);
	
	/////////////////////////////
	/////  PR1 EX4 TEST 4  //////
	/////////////////////////////
	failed = false;
	start_test(test_section, "PR1_EX4_4", "Request a valid company", 0.3);
	if (fail_all) {
		failed = true;
	} else {
		csv_initEntry(&entry);
		csv_initEntry(&refEntry);
		csv_parseEntry(&refEntry, "E50938476;BlueWave Robotics;03/12/2016;30;200;1;47051307Z", "COMPANY");
		error = api_getCompany(data, "E50938476", &entry);
		if (error != E_SUCCESS || !csv_equalsEntry(entry, refEntry)) {
			failed = true;
			passed = false;
		}
		csv_freeEntry(&entry);
		csv_freeEntry(&refEntry);
	}
	end_test(test_section, "PR1_EX4_4", !failed);
	
	/////////////////////////////
	/////  PR1 EX4 TEST 5  //////
	/////////////////////////////
	failed = false;
	start_test(test_section, "PR1_EX4_5", "Get jobs", 0.9);
	if (fail_all) {
		failed = true;
	} else {
		csv_init(&report);
		csv_init(&refReport);
		
		csv_addStrEntry(&refReport, "1;Junior software developer;1;01/10/2025;20/11/2025;20000;35000;G60667813", "JOB");
		csv_addStrEntry(&refReport, "2;Chef assistant;1;15/11/2025;15/02/2026;20000;25000;G60667813", "JOB");
		csv_addStrEntry(&refReport, "3;Course instructor;2;15/10/2025;31/10/2025;19000;22000;G60667813", "JOB");
		csv_addStrEntry(&refReport, "4;Senior software developer;1;15/10/2025;15/12/2025;30000;33000;B90234567", "JOB");
		csv_addStrEntry(&refReport, "5;Data scientist;1;15/10/2025;15/12/2025;30000;35000;B90234567", "JOB");
		csv_addStrEntry(&refReport, "6;Chef;2;15/10/2025;31/10/2025;20000;45000;C75891234", "JOB");
		csv_addStrEntry(&refReport, "7;English Teacher;2;15/10/2025;31/10/2025;16576;20000;C75891234", "JOB");
		csv_addStrEntry(&refReport, "8;Computer scientist;1;07/08/2025;01/11/2025;17000;19000;C75891234", "JOB");
		csv_addStrEntry(&refReport, "9;CTO;1;07/08/2025;01/11/2025;47000;59500;D60487219", "JOB");

		error = api_getJobs(data, &report);
		if (error != E_SUCCESS || !csv_equals(report, refReport)) {
			failed = true;
			passed = false;
		}
		csv_free(&report);
		csv_free(&refReport);
	}
	end_test(test_section, "PR1_EX4_5", !failed);
	
	/////////////////////////////
	/////  PR1 EX4 TEST 6  //////
	/////////////////////////////
	failed = false;
	start_test(test_section, "PR1_EX4_6", "Get jobs by company and salary", 0.9);
	if (fail_all) {
		failed = true;
	} else {
		csv_init(&report);
		csv_init(&refReport);		

		csv_addStrEntry(&refReport, "1;Junior software developer;1;01/10/2025;20/11/2025;20000;35000;G60667813", "JOB");
		csv_addStrEntry(&refReport, "2;Chef assistant;1;15/11/2025;15/02/2026;20000;25000;G60667813", "JOB");

		error = api_getJobsByCompanyAndSalary(data, &report, "G60667813", 23000);
		if (error != E_SUCCESS || !csv_equals(report, refReport)) {
			failed = true;
			passed = false;
		}
		csv_free(&report);
		csv_free(&refReport);
		
		if (!failed) {
			csv_init(&report);
			csv_init(&refReport);		

			csv_addStrEntry(&refReport, "7;English Teacher;2;15/10/2025;31/10/2025;16576;20000;C75891234", "JOB");
			csv_addStrEntry(&refReport, "8;Computer scientist;1;07/08/2025;01/11/2025;17000;19000;C75891234", "JOB");

			error = api_getJobsByCompanyAndSalary(data, &report, "C75891234", 18000);
			if (error != E_SUCCESS || !csv_equals(report, refReport)) {
				failed = true;
				passed = false;
			}
			csv_free(&report);
			csv_free(&refReport);
		}
	}
	end_test(test_section, "PR1_EX4_6", !failed);

	// Release all data
	api_freeData(&data);
	
	return passed;
}
