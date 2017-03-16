#ifndef _LIBLIST_H
#define _LIBLIST_H

#include "libcontact.h"

struct ListNode{
	struct Contact* contact;
	struct ListNode *prev,*next;
};
struct ListBook{
	struct ListNode *first,*last;
};

void createListBook(struct ListBook* list);
void deleteListBook(struct ListBook* list);
void addContactToListBookD(struct ListBook* list,struct ListNode *tmp,char* firstname,char* surname, char* dateOfBirth,char* email,char* phoneNumber, char* address);
void addContactToListBookC(struct ListBook* list,struct ListNode* node);
void deleteContactFromListBook(struct ListBook* list,char *firstname,char *surname);
struct ListNode* findContactInListBook(struct ListBook* list,char *firstname,char *surname);
void sortListBookByNthElement(struct ListBook* book, int n); /*n(1-6)-który element struktury do sortowania*/

char** listBookToString(struct ListBook* list);
int listf1(struct ListNode* x,struct ListNode* y);
int listf2 (struct ListNode* x,struct ListNode* y);
int listf3 (struct ListNode* x,struct ListNode* y);
int listf4 (struct ListNode* x,struct ListNode* y);
int listf5 (struct ListNode* x,struct ListNode* y);
int listf6 (struct ListNode* x,struct ListNode* y);
void sortListBook(struct ListBook* list,int (*wsk)(struct ListNode* x,struct ListNode* y));
#endif
