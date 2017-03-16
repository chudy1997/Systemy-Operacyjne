#include "liblist.h"

void createListBook(struct ListBook* list){
	list->first=list->last=0;
}
void deleteListBook(struct ListBook* list){
	if(list){
		struct ListNode* tmp=list->first;
		while(tmp!=list->last){
			tmp=tmp->next;
			deleteContact(tmp->prev->contact);
			free(tmp->prev);
		}
		free(tmp);
		free(list);
	}
}

void addContactToListBookD(struct ListBook* list,struct ListNode *tmp,char* firstname,char* surname, char* dateOfBirth,char* email,char* phoneNumber, char* address){
	makeContact(tmp->contact,firstname,surname,dateOfBirth,email,phoneNumber,address);
	addContactToListBookC(list,tmp);
}
void addContactToListBookC(struct ListBook* list,struct ListNode* node){
	if(!list)return ;
	if(list->first){
		list->last->next=node;
		node->next=0;
		node->prev=list->last;
		list->last=node;
	}
	else{
		list->first=list->last=node;
		node->next=node->prev=0;
	}
}

void deleteContactFromListBook(struct ListBook* list,char *firstname,char *surname){
	if(list){
		struct ListNode *tmp=findContactInListBook(list,firstname,surname);
		if(tmp){
			 if(tmp->prev)
				tmp->prev->next=tmp->next;
			 if(tmp->next)
			 	tmp->next->prev=tmp->prev;
			 deleteContact(tmp->contact);
		}
	}
}

struct ListNode *findContactInListBook(struct ListBook* list,char *firstname,char *surname){
	struct ListNode* tmp=0;
	if(list){
		tmp=list->first;
		while(tmp && (strcmp(tmp->contact->firstname,firstname) || !strcmp(tmp->contact->surname,surname)))tmp=tmp->next;
	}
	return tmp;
}

void sortListBookByNthElement(struct ListBook* list, int n){
	switch(n){
		case 1:
		sortListBook(list,listf1);
		break;
		case 2:
		sortListBook(list,listf2);
		break;
		case 3:
		sortListBook(list,listf3);
		break;
		case 4:
		sortListBook(list,listf4);
		break;
		case 5:
		sortListBook(list,listf5);
		break;
		case 6:
		sortListBook(list,listf6);
		break;
		default:;
	}
}

char** listBookToString(struct ListBook* book){
	int i;char** res;struct ListNode* tmp;
	i=1;
	if(!book)return NULL;
	tmp=book->first;
	while(tmp!=book->last){tmp=tmp->next;i++;}
	res=(char**)(malloc(i*sizeof(char*)));
	tmp=book->first;
	i=1;
	res[0]=contactToString(tmp->contact);
	while(tmp!=book->last){
		tmp=tmp->next;
		res[i++]=contactToString(tmp->contact);
	}
	return res;
}

int listf1(struct ListNode* x,struct ListNode* y){
	if(x->contact->firstname > y->contact->firstname)return 1;
	else if(x->contact->firstname < y->contact->firstname)return -1;
	return 0;
}
int listf2(struct ListNode* x,struct ListNode* y){
if(x->contact->surname > y->contact->surname)return 1;
	else if(x->contact->surname < y->contact->surname)return -1;
	return 0;
}
int listf3 (struct ListNode* x,struct ListNode* y){
	if(x->contact->dateOfBirth > y->contact->dateOfBirth)return 1;
	else if(x->contact->dateOfBirth < y->contact->dateOfBirth)return -1;
	return 0;
}
int listf4 (struct ListNode* x,struct ListNode* y){
	if(x->contact->email > y->contact->email)return 1;
	else if(x->contact->email < y->contact->email)return -1;
	return 0;
}
int listf5 (struct ListNode* x,struct ListNode* y){
	if(x->contact->phoneNumber > y->contact->phoneNumber)return 1;
	else if(x->contact->phoneNumber < y->contact->phoneNumber)return -1;
	return 0;
}
int listf6 (struct ListNode* x,struct ListNode* y){
	if(x->contact->address > y->contact->address)return 1;
	else if(x->contact->address < y->contact->address)return -1;
	return 0;
}
void sortListBook(struct ListBook* list,int (*wsk)(struct ListNode* x,struct ListNode* y)){
	struct ListNode *tmp,*max;
	struct ListBook *res=malloc(sizeof(struct ListBook));
	res->first=list->first;
	res->last=list->last;
	list->first=list->last=0;
	while(res){
		max=res->first;
		tmp=res->first->next;
		while(tmp){
			if(wsk(tmp,max)>0)max=tmp;
			tmp=tmp->next;
		}
		if(res->first==res->last){
			res->first=res->last=0;
			res=0;
		}
		else if(max==res->first){
			res->first=res->first->next;
			list->first->prev=0;
		}
		else if(max==res->last){
			res->last=res->last->prev;
			res->last->next=0;
		}
		else{
			max->prev->next=max->next;
			max->next->prev=max->prev;
		}
		if(list->first){
			list->last->next=max;
			max->prev=list->last;
			max->next=0;
			list->last=max;
		}
		else {
			list->first=list->last=max;
			max->prev=max->next=0;
		}
	}
}
