#include <stdio.h>
#include <dirent.h>


int main(int argc,char** argv){
  DIR *dir;
  struct dirent *ent;
  if ((dir = opendir (argv[1])) != NULL) {
    /* print all the files and directories within directory */
    while ((ent = readdir (dir)) != NULL) {
      printf ("%s\n", ent->d_name);
    }
    closedir (dir);
  } else {
    /* could not open directory */
    perror ("");
    return -1;
  }
}

  /*
  if(argc<2){
    printf("Usage: zaj <dirname> <size>");
    return -1;
  }
  struct dirent *pDir;
  char* dirName=argv[1];
  int size=atoi(argv[2]);
  DIR* directory=opendir(dirName);
  if(directory==NULL){
    printf("Cannot open directory '%s'\n",dirName);
    return -2;
  }
  while((pDir=readdir(directory))!=NULL){
    printf("[%s]\n\n",pDir->d_name);
  }
  close(directory);*/
//}
