#ifndef _LIBTREE_H
#define _LIBTREE_H

#include "libcontact.h"
#include "liblist.h"

struct TreeNode{
	struct Contact* contact;
	struct TreeNode *left,*right;
};
struct TreeBook{
	struct TreeNode* root;
};

void createTreeBook(struct TreeBook* tree);
void deleteTreeBook(struct TreeBook* tree);
void addContactToTreeBookD(struct TreeBook* tree,struct TreeNode* node,char* firstname,char* surname, char* dateOfBirth,char* email,char* phoneNumber, char* address);
void addContactToTreeBookC(struct TreeBook* tree,struct TreeNode* contact);
void deleteContactFromTreeBook(struct TreeBook* tree,char* firstname,char* surname);
struct Contact* findContactInTreeBook(struct TreeBook* tree,char* firstname,char* surname);
void sortTreeBook(struct TreeBook **book,int n);
void sortTreeBookRec(struct TreeBook* res,struct TreeNode *tmp,int (*wsk)(struct TreeNode* x,struct TreeNode* y));

int printTree(struct TreeNode* node,int i);
int countElemsInTree(struct TreeNode *node);
void deleteTreeBookRec(struct TreeNode* node);
void deleteContactFromTreeBookRec(struct TreeNode* node,char* firstname,char* surname);
void addContactToTreeBookRec(struct TreeNode* root,struct TreeNode* node);
struct Contact* findContactInTreeBookRec(struct TreeNode* node,char* firstname,char* surname);
int treef1(struct TreeNode* x,struct TreeNode* y);
int treef2 (struct TreeNode* x,struct TreeNode* y);
int treef3 (struct TreeNode* x,struct TreeNode* y);
int treef4 (struct TreeNode* x,struct TreeNode* y);
int treef5 (struct TreeNode* x,struct TreeNode* y);
int treef6 (struct TreeNode* x,struct TreeNode* y);
void addContactToTreeBookInOrder(struct TreeBook* node,struct Contact *contact,int (*wsk)(struct TreeNode* x,struct TreeNode* y));
void addContactToTreeBookInOrderRec(struct TreeNode* node,struct TreeNode *myNode,int (*wsk)(struct TreeNode* x,struct TreeNode* y));
void flattenTree(struct ListBook *res,struct TreeNode* node);
#endif
