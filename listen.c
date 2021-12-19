// Louay Ahmad (CSIT 231_01) and Dori Lerner (CSIT 231_03)/


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

//declerations
int numofctrlzs = 0;
int fd1;
// FIFO file path
char * mypath = "/tmp/mypath";
char str1[80], str2[80];
FILE *transcript;
//semaphore
int sema;
FILE *lock;
char * user;
char line[256];

//checks if user1.txt was created, 
int exist2(const char *fname)
{
    FILE *file;
    if( access( fname, F_OK ) == 0 ) {
      printf("User1 has Initialized\n");
      printf("\n");
      printf("Please enter ctrl Z to send a message:\n");
    return 1;
} 
else {
   printf("Waiting for User1 to initialize...\n");
   sleep(2);
    return 0;
}
}

//removes lock file 
int rm_file(const char *fname){
  if (remove(fname) == 0)
      printf("\n");
      
      
   else
      printf("(Unable to delete the file)\n");
  
   return 0;
}

//checks if lock file exisits
int exists(const char *fname)
{
    FILE *file;
    if( access( fname, F_OK ) == 0 ) {
    // file exists
        printf("The other user is currently transmitting. Your message will be automatically sent when incoming transmission arrives...\n");
        fd1 = open(mypath,O_RDONLY);
        transcript = fopen("transcript.txt", "a");
        read(fd1, str1, 80);

        // Print the read string and close
        printf("User1: %s\n", str1);
        close(fd1);

        //writes to recording
        fprintf(transcript,"User1: %s\n", str1);
        fclose(transcript);
    return 1;
} else {
    // file doesn't exist
    //prompt user for input
    printf("Please send Messege:\n");
    printf("%s: ", user);
    return 1;
  }
}


//repurpos ctrl z
void ctrlz ()
{
  
  if (numofctrlzs==10)
  {
    
    raise(SIGSTOP); 
  }
  
  else
  {
    //check if lock fie exisists, if yes, stay in loop
    while(sema == 0){
      //aas long aas sema = 0, the other person is transmitting.
        sema = exists("lock");
        
    }
    

        
        //append user input to fifo
        FILE * lock;
        lock = fopen("lock", "a");
        fgets(str2, 80, stdin);
        fd1 = open(mypath,O_WRONLY);
        
        write(fd1, str2, strlen(str2)+1);
        close(fd1);
        //writes to recording
        transcript = fopen("transcript.txt", "a");
        fprintf(transcript,"%s: %s\n", user, str2);
        fclose(transcript);
        //remove lock file
        rm_file("lock");
        //reset semaphore
        sema = 0;
        
  
  sleep(1);
  }
}


int main(int argc, char *argv[])
{
  //clear terminal
  system("clear");
  //ensure valid number of args
  if (argc<2)
  {
    printf("USAGE: Please enter './listen' followed by your name:\n");
    exit(1);
  }
  //assign first arg to user
  user=argv[1];
  //create file and append username
  FILE *username2;
  username2 = fopen("user2.txt", "a");
  fprintf(username2, "%s", user);
  fclose(username2);



  //repurpose ctrl z
   signal(SIGTSTP, ctrlz );
  
  
    // mkfifo(<pathname>,<permission>)
    mkfifo(mypath, 0666);
    //warn user
    printf("Hello %s\n", user);
    printf("\n");
    printf("WARNING: This Conversation WILL be recorded\n");
    printf("\n");
    printf("Result of 'ps -ef | egrep listen|speak':\n");
    printf("\n");
    system("ps -ef | egrep 'listen|speak'\n");
    printf("\n");
    

  //checks if user1.txt exisists, if not wait for it
  int check= 0;
  while(check == 0){
    check = exist2("user1.txt");
  }
  //get user1 username from file
  FILE * usernames; char str[999];
  FILE * user1;
  user1 = fopen( "user1.txt" , "r");
  if (user1) {
    while (fscanf(user1, "%s", str)!=EOF)
        printf("\n");
    fclose(user1);
    rm_file("user1.txt");

  }
  //read fifo
    while (1)
    {

      

        
        //open fifio 
        fd1 = open(mypath,O_RDONLY);
        //open transcript
        transcript = fopen("transcript.txt", "a");
        read(fd1, str1, 80);

        // Print the read string and close
        printf("%s: %s\n", str, str1);
        close(fd1);

        //writes to recording and close
        fprintf(transcript,"%s: %s\n", str, str1);
        fclose(transcript);


    }
    return 0;
  } 

