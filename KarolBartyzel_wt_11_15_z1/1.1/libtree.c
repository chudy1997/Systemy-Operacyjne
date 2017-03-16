#include "lib.h"

void createTreeBook(struct TreeBook* tree){
	tree->root=0;
}
int i=1;
void deleteTreeBook(struct TreeBook* tree){
  deleteTreeBookRec(tree->root);
	free(tree);
}

void addContactToTreeBookD(struct TreeBook* tree,struct TreeNode* tmp,char* firstname,char* surname, char* dateOfBirth,char* email,char* phoneNumber, char* address){
	makeContact(tmp->contact,firstname,surname,dateOfBirth,email,phoneNumber,address);
	addContactToTreeBookC(tree,tmp);
}
void addContactToTreeBookC(struct TreeBook* tree,struct TreeNode* node){
	if(tree){
		time_t tt;
		srand(time(&tt));
		if(!tree->root)tree->root=node;
		else addContactToTreeBookRec(tree->root,node);
	}
}

struct TreeNode* findParentOfLeaf(struct TreeNode* root,struct Contact* leaf){
	if(root!=0)return 0;
	if(root->contact==leaf)return 0;
	if(root->left!=0 && root->right!=0)return 0;
	if(root->left->contact==leaf || root->right->contact==leaf)return root;
	struct TreeNode* tmp=findParentOfLeaf(root->left,leaf);
	if(tmp)return tmp;
	tmp=findParentOfLeaf(root->right,leaf);
	return tmp;
}
int k=1;

void deleteContactFromTreeBook(struct TreeBook* tree,char* firstname,char* surname){
	if(tree && tree->root){
		struct Contact *tmp=findContactInTreeBook(tree,firstname,surname);
		printf("%d. %s",k++,contactToString(tmp));
		if(!tmp)return;
		struct TreeNode *par=findParentOfLeaf(tree->root,tmp);
		struct TreeNode *leaf;
		if(par)leaf=par;
		else leaf=tree->root;
		while(leaf && (leaf->left || leaf->right)){
			if(leaf->left)leaf=leaf->left;
			else leaf=leaf->right;}
		struct TreeNode *par2;
		par2=findParentOfLeaf(tree->root,leaf->contact);

		if(leaf!=NULL && leaf->contact!=NULL && par2!=NULL && par2->left!=NULL && par2->left->contact!=NULL && par2->left->contact==leaf->contact){
			if(par!=NULL && par->left!=NULL && par->left->contact!=NULL && par->left->contact==tmp){
					//printf("a");
					par->left->contact=leaf->contact;
					deleteContact(par2->left->contact);
					free(par2->left);
					par2->left=0;
			}
			else{
					//printf("b");
					par->right->contact=leaf->contact;
					deleteContact(par2->left->contact);
					free(par2->left);
					par2->left=0;
			}
		}
		else if(leaf!=NULL && leaf->contact!=NULL && par2!=NULL && par2->right!=NULL && par2->right->contact!=NULL && par2->right->contact==leaf->contact){
			if(par!=NULL && par->left!=NULL && par->left->contact!=NULL && par->left->contact==tmp){
				//	printf("c");
					par->left->contact=leaf->contact;
					deleteContact(par2->right->contact);
					free(par2->right);
					par2->right=0;
			}
			else{
			//		printf("d");
					par->right->contact=leaf->contact;
					deleteContact(par2->right->contact);
					free(par2->right);
					par2->right=0;
			}
		}
	}
}


struct Contact* findContactInTreeBook(struct TreeBook* tree,char* firstname,char* surname){
	if(tree)return findContactInTreeBookRec(tree->root,firstname,surname);
	return 0;
}

void sortTreeBook(struct TreeBook **book,int n){
	struct TreeBook *tmp=*book;
	*book=malloc(sizeof(struct TreeBook));
	switch(n){
		case 1:
		sortTreeBookRec(*book,tmp->root,treef1);
		break;
		case 2:
		sortTreeBookRec(*book,tmp->root,treef2);
		break;
		case 3:
		sortTreeBookRec(*book,tmp->root,treef3);
		break;
		case 4:
		sortTreeBookRec(*book,tmp->root,treef4);
		break;
		case 5:
		sortTreeBookRec(*book,tmp->root,treef5);
		break;
		case 6:
		sortTreeBookRec(*book,tmp->root,treef6);
		break;
	}

}
void sortTreeBookRec(struct TreeBook* res,struct TreeNode *tmp,int (*wsk)(struct TreeNode* x,struct TreeNode* y)){
	if(!tmp)return;
	addContactToTreeBookInOrder(res,tmp->contact,wsk);
	sortTreeBookRec(res,tmp->left,wsk);
	sortTreeBookRec(res,tmp->right,wsk);
}

int countElemsInTree(struct TreeNode *node){
	if(!node)return 0;
	return 1+countElemsInTree(node->left)+countElemsInTree(node->right);
}
void deleteTreeBookRec(struct TreeNode* node){
	if(node){
		deleteTreeBookRec(node->left);
		deleteTreeBookRec(node->right);
		deleteContact(node->contact);
		free(node);
	}
}

int printTree(struct TreeNode* node,int i){
	if(!node)return i;
	i=printTree(node->left,i);
	printf("%d.\n%s\n\n",i++,contactToString(node->contact));
	i=printTree(node->right,i);
	return i;
}

void addContactToTreeBookInOrderRec(struct TreeNode* node,struct TreeNode *myNode,int (*wsk)(struct TreeNode* x,struct TreeNode* y)){
	if(wsk(node,myNode)>0){
		if(node->left)addContactToTreeBookInOrderRec(node->left,myNode,wsk);
		else {node->left=myNode;myNode->left=myNode->right=0;}
	}
	else{
		if(node->right)addContactToTreeBookInOrderRec(node->right,myNode,wsk);
		else {node->right=myNode;myNode->left=myNode->right=0;}
	}
}
struct Contact* findContactInTreeBookRec(struct TreeNode* node,char* firstname,char* surname){
	if(!node || !node->contact)return 0;
	else{
		struct Contact *res1,*res2;
		if(node!=NULL && node->contact!=NULL)
			if(strcmp(node->contact->firstname,firstname)==0 && !strcmp(node->contact->surname,surname)==0)return node->contact;
		if(node->left!=NULL)res1=findContactInTreeBookRec(node->left,firstname,surname);
		if(node->right!=NULL)res2=findContactInTreeBookRec(node->right,firstname,surname);
		if(node->left!=NULL && res1!=NULL)return res1;
		else if(node->right!=NULL && res2!=NULL)return res2;
		else return 0;
	}

}
int treef1(struct TreeNode* x,struct TreeNode* y){
	return strcmp(x->contact->firstname,y->contact->firstname);
}
int treef2 (struct TreeNode* x,struct TreeNode* y){
	return strcmp(x->contact->surname,y->contact->surname);
}
int treef3 (struct TreeNode* x,struct TreeNode* y){
	return strcmp(x->contact->dateOfBirth,y->contact->dateOfBirth);
}
int treef4 (struct TreeNode* x,struct TreeNode* y){
	return strcmp(x->contact->email, y->contact->email);
}
int treef5 (struct TreeNode* x,struct TreeNode* y){
	return strcmp(x->contact->phoneNumber,y->contact->phoneNumber);
}
int treef6 (struct TreeNode* x,struct TreeNode* y){
	return strcmp(x->contact->address,y->contact->address);
}
void addContactToTreeBookInOrder(struct TreeBook* node,struct Contact *contact,int (*wsk)(struct TreeNode* x,struct TreeNode* y)){
	if(!node)return;
	if(!node->root){
		node->root=malloc(sizeof(struct TreeNode));
		node->root->contact=contact;
		node->root->left=node->root->right=0;
	}
	else{
		struct TreeNode* tmp=malloc(sizeof (struct TreeNode));
		tmp->contact=contact;
		addContactToTreeBookInOrderRec(node->root,tmp,wsk);
	}
}
void addContactToTreeBookRec(struct TreeNode* root,struct TreeNode* node){
	if(root->left && root->right){
		if(rand()%2)addContactToTreeBookRec(root->left,node);
		else addContactToTreeBookRec(root->right,node);
	}
	else{
		node->left=node->right=0;
		if(!root->left && !root->right){
			if(rand()%2){root->left=node;}
			else root->right=node;
		}
		else if(!root->left)root->left=node;
		else if(!root->right)root->right=node;
	}
}
