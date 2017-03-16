#include "libcontact.h"

void makeContact(struct Contact* contact,char* firstname,char* surname, char* dateOfBirth,char* email,char* phoneNumber, char* address){
    contact->firstname=firstname;
    contact->surname=surname;
    contact->dateOfBirth=dateOfBirth;
    contact->email=email;
    contact->phoneNumber=phoneNumber;
    contact->address=address;
}
void deleteContact(struct Contact* contact){
	free(contact);
}
char* contactToString(struct Contact* contact){
	const int length=62+countChars(contact);
	char* res = (char*)(malloc(sizeof (char)*length));
	res[0]=0;
	strcat(res,"firstname ");
	strcat(res,contact->firstname);
	strcat(res,"\nsurname ");
	strcat(res,contact->surname);
	strcat(res,"\ndateOfBirth ");
	strcat(res,contact->dateOfBirth);
	strcat(res,"\nemail ");
	strcat(res,contact->email);
	strcat(res,"\nphoneNumber ");
	strcat(res,contact->phoneNumber);
	strcat(res,"\naddress ");
	strcat(res,contact->address);
	return res;
}
int countChars(struct Contact* c){
	return strlen(c->firstname)+strlen(c->surname)+strlen(c->dateOfBirth)+strlen(c->email)+strlen(c->phoneNumber)+strlen(c->address);
}
