#ifndef _LIBCONTACT_H
#define _LIBCONTACT_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

struct Contact{
	char* firstname;
	char* surname;
	char* dateOfBirth;
	char* email;
	char* phoneNumber;
	char* address;
};

void makeContact(struct Contact* contact,char* firstname,char* surname, char* dateOfBirth,char* email,char* phoneNumber, char* address);
void deleteContact(struct Contact* contact);

int countChars(struct Contact* c);
char* contactToString(struct Contact* contact);

#endif
