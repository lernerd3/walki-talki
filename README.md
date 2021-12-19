# walki-talki
C program using fifo and mutex lock
// Louay Ahmad (CSIT 231_01) and Dori Lerner (CSIT 231_03) 

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include<errno.h> 
#include <sys/wait.h>
#include<signal.h> 

//declarations
char * user;
int numofctrlzs = 0;
int fd;
// FIFO file path
char * mypath = "/tmp/mypath";
char str1[80], str2[80];
int sema;
FILE lock;


//remove file called lock, unlocking transmission  
int rm_file(const char *fname){
  if (remove(fname) == 0)
      printf("\n");
      
      
   else
      printf("(Unable to delete the file)\n");
  
   return 0;
}

//check if lock file exisists, if it exists, keep listening for message, send message only after incoming arrives
//if lock file does does not exist, send message
int exists(const char *fname)
{
    FILE *file;
    if( access( fname, F_OK ) == 0 ) {
    // file exists
    printf("The other user is currently transmitting. Your message will be automatically sent when incoming transmission arrives..\n");
    // Open FIFO for Read only
        fd = open(mypath, O_RDONLY);
        // read from fifio
        read(fd, str1, sizeof(str1));
        // print the read message
        printf("User2: %s\n", str1);
        close(fd);
    return 1;
} else {
    // file doesn't exist
    
    printf("Please send Messege:\n");
    printf("%s: ", user);
    return 1;
  }
}
//check if user to initialized, if file calles user2 has been created
int exist2(const char *fname)
{
    FILE *file;
    if( access( fname, F_OK ) == 0 ) {
      printf("User2 has Initialized\n");
      printf("\n");
      printf("Please enter ctrl Z to send a message:\n");
    return 1;
} else {
   printf("Waiting for User2 to initialize...\n");
   printf("\n");
   sleep(2);
    return 0;
}
}


//function called when ctrlz being entered 
void ctrlz ()
{
  if (numofctrlzs==10)
  {
    
    raise(SIGSTOP); 
  }
  else
  {
    //check if lock file exisits, if exisists, stay in loop 
    while(sema == 0){
      //as long as semaphore = 0, the other person is transmitting. stay in loop, 
        sema = exists("lock");
    }
    
              
              //create file called lock
              FILE * lock;
              lock = fopen("lock", "a");
              
              //get input from user
              fgets(str2, 80, stdin);

              // Open FIFO for write only
              fd = open(mypath, O_WRONLY);

              // write input to fifo
              write(fd, str2, strlen(str2)+1);
              // and close it
              close(fd);

              //remove lock file
              rm_file("lock");

              //reset semaphore
              sema = 0;
              
  
  sleep(1);
  }
}

int main(int argc, char *argv[])
{
  //clear termial
  system("clear");
  // ensure correct num args
  if (argc<2)
  {
    printf("USAGE: Please enter ./speak, then your name:\n");
    exit(1);
  }
   // assignes ussername to user
   user=argv[1];
   //creates a file called user1.txt, appends user1s name to the file
   FILE *usernames;
   usernames = fopen("user1.txt", "a");
   fprintf(usernames, "%s", user);
   fclose(usernames);

  //repurpose ctrl z
   signal(SIGTSTP, ctrlz );

   // Creating the named file(FIFO)
    // mkfifo(<pathname>, <permission>)
    mkfifo(mypath, 0666);
    
    //warning to user
    printf("Hello %s\n", user);
    printf("\n");
    printf("WARNING: This Conversation WILL be recorded\n");
    printf("\n");
    printf("Result of 'ps -ef | egrep listen|speak':\n");
    printf("\n");
    system("ps -ef | egrep 'listen|speak'\n");
    printf("\n");
    

  int check= 0;
  //check if other username file exisits, if not wait until it does
  while(check == 0){
    check = exist2("user2.txt");
  }
  //get username
  char str[999];
  FILE * user2;
  user2 = fopen( "user2.txt" , "r");
  if (user2) {
    while (fscanf(user2, "%s", str)!=EOF)
        printf("\n");
    fclose(user2);}
    rm_file("user2.txt");

     //read from fifo
    while (1)
    {
      
    
        // Open FIFO for Read only
        fd = open(mypath, O_RDONLY);
        // Read from FIFO
        read(fd, str1, sizeof(str1));
        // Print the read message
        printf("%s: %s\n", str,  str1);
        close(fd);
    }
    return 0;
  }
