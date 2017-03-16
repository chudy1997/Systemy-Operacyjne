#include "lib.h"
#include "sys/times.h"

struct tms *run_time;
struct tms previous;

double previousUserTimeInMilis;
double previousSystemTimeInMilis;

void print_time();
void reset_time();

char* firstNames[100]={"Nasim","Yoshi","Jennifer","Xerxes","Audra","Steven","Stacy","Haley","Janna","Lance","Tarik","Taylor","Riley","Indigo","Clio","Kyle","Brian","Xander","Caryn","Hyatt","Alexander","Kalia","Yasir","Myra","Clementine","Cheryl","Noah","Duncan","Inez","Diana","Jade","Sigourney","Gemma","Gil","Ocean","Jack","Nero","Halla","Indira","Kristen","Nadine","Maya","Ruth","Susan","Paloma","Tasha","Kuame","Denise","Dara","Moana","Randall","Kennan","Keely","Bevis","Oleg","Larissa","Jerome","Graiden","Cody","Dorothy","Sybill","Ariel","Caleb","Abel","August","Nicholas","Cruz","Forrest","Felix","Daria","Yuri","Jermaine","Rigel","Sophia","Barbara","Kato","Isaiah","Riley","Kiara","Cara","Miranda","Gail","Ila","Hadley","Cade","Charity","Beverly","Jennifer","Anastasia","Ivor","Fuller","Ian","Eliana","Galvin","Constance","Lionel","Justin","Janna","Francis","Danielle"};
char* surNames[100]={"Harrison","Vance","Reilly","Mccoy","Arnold","Butler","Sanders","Weeks","Roy","Joyner","Lawson","Dillon","Thornton","Vaughn","Lowery","Cline","Lynch","Diaz","Ratliff","Wilkerson","Madden","Gutierrez","Carroll","Patton","Stark","Newman","Trujillo","Tate","Phillips","Miles","Rich","Gillespie","Hooper","Kramer","Mcguire","English","Horton","Leonard","Jordan","Hopkins","Wise","Gallagher","Mckee","William","Cash","Ayala","Schroeder","Daniel","Mcclain","Chaney","Bryan","Blair","Santiago","Combs","Malone","Larson","Guy","Glover","Rose","Whitaker","Bruce","Ballard","Drake","Holt","Sears","Mays","House","Holmes","Middleton","Lowery","Edwards","Kirk","Wall","Gibson","Butler","Ware","Davidson","Combs","Schneider","Vaughn","Evans","Battle","Andrews","Ewing","Spears","Bradley","Emerson","Bean","Horton","King","Mcintosh","Odom","Workman","Baird","Cotton","Walton","Booker","Benson","Reeves","Woods"};

char* generateFirstname(int i);
char* generateSurname(int i);
char* generateDateOfBirth(int i);
char* generateEmail(int i);
char* generatePhoneNumber(int i);
char* generateAddress(int i);

int main(){
	previousSystemTimeInMilis = previousUserTimeInMilis = 0.0;
	printf("Clocks per second: %ld\n",CLOCKS_PER_SEC);
	run_time = (struct tms *) malloc(sizeof(struct tms));

	times(run_time);

	struct ListBook* list=malloc(sizeof(struct ListBook));
	struct TreeBook* tree=malloc(sizeof(struct TreeBook));
	struct Contact* contact;

	for(int i=0;i<100;i++)
		for(int j=0;j<10000;j++){
			list=malloc(sizeof(struct ListBook));
			createListBook(list);
			free(list);
		}
	printf("Creating 1000000 contacts books (list):\n\n");
	print_time();

	for(int i=0;i<100;i++)
		for(int j=0;j<10000;j++){
			tree=malloc(sizeof(struct TreeBook));
			createTreeBook(tree);
			free(tree);
		}
	printf("Creating 1000000 contacts books (tree):\n\n");
	print_time();

	list=malloc(sizeof(struct ListBook));
	tree=malloc(sizeof(struct TreeBook));

for(int i=0;i<10000;i++){
	contact=malloc(sizeof(struct Contact));
	makeContact(contact,generateFirstname(i),generateSurname(i),generateDateOfBirth(i),
		generateEmail(i),generatePhoneNumber(i),generateAddress(i));
	struct ListNode* node=malloc(sizeof(struct ListNode));
	node->contact=contact;
	addContactToListBookC(list,node);
}
printf("Adding 10000 contacts to list:\n\n");
print_time();

for(int i=0;i<10000;i++){
	contact=malloc(sizeof(struct Contact));
	makeContact(contact,generateFirstname(i),generateSurname(i),generateDateOfBirth(i),
		generateEmail(i),generatePhoneNumber(i),generateAddress(i));
	struct TreeNode* node=malloc(sizeof(struct TreeNode));
	node->contact=contact;
	addContactToTreeBookC(tree,node);
}
printf("Adding 10000 contacts to tree:\n\n");
print_time();
for(int i=9999;i>=0;i--)
	deleteContactFromListBook(list,generateFirstname(i),generateSurname(i));
printf("Deleting 10000 contacts from list:\n\n");
print_time();

for(int i=9999;i>=0;i--){deleteContactFromTreeBook(tree,generateFirstname(i),generateSurname(i));}
	printf("Deleting 10000 contacts from tree:\n\n");
	print_time();

list=malloc(sizeof(struct ListBook));
createListBook(list);
tree=malloc(sizeof(struct TreeBook));
createTreeBook(tree);

for(int i=0;i<10000;i++){
	contact=malloc(sizeof(struct Contact));
	makeContact(contact,generateFirstname(i),generateSurname(i),generateDateOfBirth(i),
		generateEmail(i),generatePhoneNumber(i),generateAddress(i));
	struct ListNode* node1=malloc(sizeof(struct ListNode));
	node1->contact=contact;
	addContactToListBookC(list,node1);
	contact=malloc(sizeof(struct Contact));
	makeContact(contact,generateFirstname(i),generateSurname(i),generateDateOfBirth(i),generateEmail(i),generatePhoneNumber(i),generateAddress(i));
	struct TreeNode* node2=malloc(sizeof(struct TreeNode));
	node2->contact=contact;
	addContactToTreeBookC(tree,node2);
}
reset_time();

sortListBookByNthElement(list,2);
printf("Sorting Listbook with 10000 contacts:\n\n");
print_time();

sortTreeBook(&tree,1);
printf("Sorting Listbook with 10000 contacts:\n\n");
print_time();

for(int i=0;i<10000;i++)
	findContactInTreeBook(tree,generateFirstname(i),generateSurname(i));
printf("Finding 10000 contacts in Treebook with 10000 contacts:\n\n");
print_time();

for(int i=0;i<10000;i++)
	findContactInListBook(list,generateFirstname(i),generateSurname(i));
printf("Finding 10000 contacts in Listbook with 10000 contacts:\n\n");
print_time();

deleteListBook(list);
printf("Deleting listbook with 10000 contacts:\n\n");
print_time();

deleteTreeBook(tree);
printf("Deleting treebook with 10000 contacts:\n\n");
print_time();

}


void print_time()
{
    previous = *run_time;
    times(run_time);

    double totalUserTime = (double) run_time -> tms_utime;
    double totalSystemTime = (double) run_time -> tms_stime;

    double totalUserTimeInMilis = (totalUserTime / CLOCKS_PER_SEC) * 10000.0d;
    double totalSystemTimeInMilis = (totalSystemTime / CLOCKS_PER_SEC) * 10000.0d;

    printf("\tTotal time:  \t(+%4.2lf) s\n", totalUserTimeInMilis + totalSystemTimeInMilis - previousUserTimeInMilis - previousSystemTimeInMilis);
    printf("\tUser time:   \t(+%4.2lf) s\n", totalUserTimeInMilis - previousUserTimeInMilis);
    printf("\tSystem time: \t(+%4.2lf) s\n\n", totalSystemTimeInMilis - previousSystemTimeInMilis);

    previousUserTimeInMilis = totalUserTimeInMilis;
    previousSystemTimeInMilis = totalSystemTimeInMilis;
}
void reset_time(){
	previous = *run_time;
	times(run_time);

	double totalUserTime = (double) run_time -> tms_utime;
	double totalSystemTime = (double) run_time -> tms_stime;

	double totalUserTimeInMilis = (totalUserTime / CLOCKS_PER_SEC) * 10000.0d;
	double totalSystemTimeInMilis = (totalSystemTime / CLOCKS_PER_SEC) * 10000.0d;

	previousUserTimeInMilis = totalUserTimeInMilis;
	previousSystemTimeInMilis = totalSystemTimeInMilis;
}

char* generateFirstname(int i){
	return firstNames[i%100];
}
char* generateSurname(int i){
	return surNames[(i%1000)/100];
}
char* generateDateOfBirth(int i){
	int iday,imonth,iyear;
	char* sday=malloc(sizeof(char)*3),*smonth=malloc(sizeof(char)*3),*syear=malloc(sizeof(char)*5),*res=malloc(sizeof(char)*11);
	iyear=1960+rand()%20+i%20;imonth=1+rand()%6+i%7;iday=1+rand()%14+i%15;
	sprintf(syear,"%d",iyear);sprintf(smonth,"%d",imonth);sprintf(sday,"%d",iday);
	strcat(res,syear);strcat(res,"/");strcat(res,smonth);strcat(res,"/");strcat(res,sday);
	return res;
}
char* generateEmail(int i){
	char* res=malloc(sizeof(char)*40);
	strcat(res,generateFirstname(i));
	strcat(res,generateSurname(i));
	strcat(res,"@example.com");
	return res;
}
char* generatePhoneNumber(int i){
	char* res=malloc(sizeof(char)*11);
	strcat(res,"+44");
	for(int j=2;j<9;j++){
		char* tmp=malloc(sizeof(char)*2);
		sprintf(tmp,"%d",i%j+1);
		strcat(res,tmp);
	}
	return res;
}
char* generateAddress(int i){
	char* tmp=malloc(sizeof(char)*2);
	sprintf(tmp,"%d",i);
	char* res=malloc(sizeof(char)*35);
	strcat(res,"USA,Washington,7th Street, no. ");
	return strcat(res,tmp);
}
