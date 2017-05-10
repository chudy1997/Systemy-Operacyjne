 #include <stdio.h>

 int main(){
	 printf("%ld %ld\n",sizeof(int),sizeof(char));
 }
// #include <stdlib.h>
// #include <pthread.h>
// #include <unistd.h>
//
// void *func(void *p){
// 	char* c=p;
// 	//printf("%c\n",*c);
// 	pthread_exit(NULL);
// }
//
// int main(int argc,char** argv){
// 	int N=atoi(argv[1]);
// 	const int lettersNo='z'-'a'+'Z'-'A'+2;
// 	char letters[lettersNo];
// 	int i,j;
// 	char c;
// 	pthread_t inc_x_thread[N];
//
// 	for(i=0;i<lettersNo/2;i++)
// 		letters[i]='a'+i;
// 	for(j=0;i<lettersNo;i++,j++)
// 		letters[i]='A'+j;
//
// 	for(i=0;i<N;i++){
// 		c=letters[i];
// 		printf("%c\n",c);
// 		pthread_create(&(inc_x_thread[i]), NULL, func ,&c);
// 	}
// 	sleep(3);
// }
//
// #include <pthread.h>
// #include <stdio.h>
// #include <stdlib.h>
//
// typedef struct _thread_data_t {
//     int tid;
//     char *letters;
// } thread_data_t;
//
// void *thread_function(void *arg) {
//     thread_data_t *data = (thread_data_t *) arg;
//     printf("pid: %d, letter: %c\n", data->tid, data->letters[data->tid]);
//
//     pthread_exit(NULL);
// }
//
// int main(int argc, char *argv[]) {
//     int character_count = ('z' - 'a' + 1) + ('Z' - 'A' + 1);
//     char letters[character_count];
//     for (int i = 0; i < character_count; ++i) {
//         if (i < ('z' - 'a') + 1) {
//             letters[i] = (char) ('a' + i);
//         } else {
//             letters[i] = (char) ('A' + i - ('z' - 'a' + 1));
//         }
//     }
//
//     if (argc != 2) {
//         perror("Not enough args");
//         exit(1);
//     } else if (atoi(argv[1]) <= 0) {
//         perror("arg must be positive integer");
//         exit(1);
//     }
//
//     int thread_count = atoi(argv[1]);
//     pthread_t threads[thread_count];
//     int rc;
//     thread_data_t thread_data[thread_count];
//
//     for (int i = 0; i < thread_count; ++i) {
//         thread_data[i].tid = i;
//         thread_data[i].letters = letters;
//         if ((rc = pthread_create(&threads[i], NULL, thread_function, &thread_data[i]))) {
//             fprintf(stderr, "error: pthread_create, rc: %d\n", rc);
//             return EXIT_FAILURE;
//         }
//     }
//
//     for (int i = 0; i < thread_count; ++i) {
//         pthread_join(threads[i], NULL);
//     }
//
//     return 0;
// }
