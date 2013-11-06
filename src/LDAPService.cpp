/*
 * LDAPService.cpp
 *
 *  Created on: 15.10.2013
 *      Author: matthias
 */

#include <ldap.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include "LDAPService.h"

#define LDAP_HOST "ldap.technikum-wien.at"
#define LDAP_PORT 389
#define SEARCHBASE "dc=technikum-wien,dc=at"
#define SCOPE LDAP_SCOPE_SUBTREE
#define BIND_USER "uid=if12b046,ou=People,dc=technikum-wien,dc=at"		/* anonymous bind with user and pw NULL */
#define BIND_PW "Kreizi_90"

LDAPService::LDAPService() {
	// TODO Auto-generated destructor stub
}

LDAPService::~LDAPService() {
	// TODO Auto-generated destructor stub
}

bool LDAPService::login(std::string username, std::string password) {
	LDAP *ld; /* LDAP resource handle */
	LDAPMessage *result, *e; /* LDAP result handle */

	std::stringstream usernamefilter;
	usernamefilter << "(uid=" << username << ")";

	char *attribs[2]; /* attribute array for search */

	attribs[0] = strdup("uid");
	attribs[1] = NULL; /* array must be NULL terminated */

	/* setup LDAP connection */
	if ((ld = ldap_init(LDAP_HOST, LDAP_PORT)) == NULL) {
		perror("ldap_init failed");
		return false;
	}

	printf("connected to LDAP server %s on port %d\n", LDAP_HOST, LDAP_PORT);

	/* anonymous bind */
	int rc = ldap_simple_bind_s(ld, BIND_USER, BIND_PW);

	if (rc != LDAP_SUCCESS) {
		fprintf(stderr, "LDAP error: %s\n", ldap_err2string(rc));
		return false;
	}

	/* perform ldap search */
	rc = ldap_search_s(ld, SEARCHBASE, SCOPE, usernamefilter.str().c_str(),
			attribs, 0, &result);

	if (rc != LDAP_SUCCESS) {
		fprintf(stderr, "LDAP search error: %s\n", ldap_err2string(rc));
		return false;
	}

	if (ldap_count_entries(ld, result) != 1) {
		std::cout << "Es wurde nicht genau ein Ergebnis gefunden" << std::endl;
		return false;
	}

	for (e = ldap_first_entry(ld, result); e != NULL;
			e = ldap_next_entry(ld, e)) {
		//printf("DN: %s\n", ldap_get_dn(ld, e));
		rc = ldap_simple_bind_s(ld, ldap_get_dn(ld, e), password.c_str());

		if (rc != LDAP_SUCCESS) {
			return false;
		} else {
			return true;
		}
	}

	/* free memory used for result */
	ldap_msgfree(result);
	free(attribs[0]);
	printf("LDAP search suceeded\n");

	ldap_unbind(ld);
	return true;
}

