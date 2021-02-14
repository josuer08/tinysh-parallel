#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>


int COMMANDS_SIZE;
char **parse_command(char *my_line, char value)
{
    int buffer_size = 64;
    int i = 0;
    char *arg;
    char **args = malloc(buffer_size * sizeof(char*));
    char *valuee = &value;
    arg = strtok(my_line, valuee);
    while (arg != NULL) {
        args[i] = arg;
        i++;
        arg = strtok(NULL, valuee);
    }

    args[i] = NULL;
    COMMANDS_SIZE = i;
    return args;
}
void trimTrailing(char * str)
{
    int index, i;

    /* Set default index */
    index = -1;

    /* Find last index of non-white space character */
    i = 0;
    while(str[i] != '\0')
    {
        if(str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
        {
            index= i;
        }

        i++;
    }

    /* Mark next character to last non-white space character as NULL */
    str[index + 1] = '\0';
}
////////////////////EXEC PROGRAM FUNCTION/////////////
//
//
//
static int exec_prog(int argc, const char **argv) {

  pid_t pid[argc];
  int status, timeout /* unused ifdef WAIT_FOR_COMPLETION */;

  // this block executes when pid=0, (I am the father process)

  for (int i =0; i < argc; i++) {

    if ((pid[i] = fork()) < 0) {  /* fork a child process           */
      printf("*** ERROR: forking child process failed\n");
      exit(1);
     } else if (pid[i] == 0) {  /* for the child process:         */
       trimTrailing(argv[i]);
       char **value = parse_command(argv[i], ' ');
       if (strcmp(value[0], "")) {
        if (execvp(value[0], value) < 0) {  /* execute the command  */
                 printf("*** ERROR: exec failed\n");
                 exit(1);
            }
        exit(0);
       } else {
        printf("");
        exit(0);
       }
     }

  }

  for (int i =0; i < argc; i++) {
    do {
      if ((pid[i] = waitpid(pid[i], &status, WNOHANG)) == -1) {
        perror("wait() error");
      }
      else if (pid[i] == 0) {
        /*printf("child is still running ");*/
        sleep(1);
      } else {
        if (WIFEXITED(status))
        printf("child exited with status of %d\n", WEXITSTATUS(status));
        /*else puts("child did not exit successfully");*/
      }
    } while (pid[i] == 0);

  }

    return 0;
}
//
//
//
//////////////////////////////////////////////////////

////////////////////BATCH MODE PROCESS////////////////
//
//
//
int batchModeStart(const char **argv) {



}
//
//
//
//////////////////////////////////////////////////////

void promptPrint(){
printf("⌈[tinysh] \n↳>>> ");
}


char *read_command_line(void)
{
    int bufsize = 1024;
    char *buffer = malloc(sizeof(char) * bufsize);
    int c;
    int i = 0;

    while ( c != '\n' ) {
        c = getchar();
        buffer[i] = c;
        i++;
    }

    return buffer;
}


int main (int argc, char *argv[]){

  char *prompt = argv[0];
  char *filename;
  bool batchMode = false;
  if ( argc == 2) {
    filename = argv[1];
    batchMode = true;
  } else if (argc != 1) {
    printf("Wrong ammount of arguments, only one file per batch \n");
  }

  /*promptPrint();*/
  /*exec_prog(1, filename);*/
  /*exec_prog(&argv[1]);*/



//Oscar J Rodriguez B (@josuer08)
  if (!batchMode) {
    bool active = true;
    char *arguments;
    while (active) {
      promptPrint();
      arguments = read_command_line();
      char **args = parse_command(arguments, ';');
      trimTrailing(args[0]);
      if (strcmp(args[0], "exit") == 0) {/* is it an "exit"?     */
        active = false;  /*   exit if it is                */
      } else {
        exec_prog(COMMANDS_SIZE,args);
      }
      /*printf("%d", COMMANDS_SIZE);*/
      /*printf("%s and %s", args[0], args[1]);*/
    }
  } else {
    char * buffer = 0;
    long length;
    FILE * f = fopen (filename, "rb");

    if (f) {
      fseek (f, 0, SEEK_END);
      length = ftell (f);
      fseek (f, 0, SEEK_SET);
      buffer = malloc (length);
      if (buffer) {
        fread (buffer, 1, length, f);
      }
      fclose (f);
    }

    if (buffer) {
    char **args = parse_command(buffer, ';');
    trimTrailing(args[0]);
    /*printf("%s", buffer);*/
    exec_prog(COMMANDS_SIZE,args);
    }


    return 0;

  }




}
