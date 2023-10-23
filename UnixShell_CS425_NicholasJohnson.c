#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

//declare size for array and background processes
#define MAX_INPUT_SIZE 100
#define MAX_BACKGROUND_PROCESSES 10

int main() {
    //declare array for parsing
    char input[MAX_INPUT_SIZE];
    pid_t background_pids[MAX_BACKGROUND_PROCESSES];
    int num_background_processes = 0;

    //welcome message :)
    printf("This is my simple Unix Shell. Type commands as you would in a normal shell.\n");

    //Continuous loop for shell prompts
    while (1) {
        //print sh> before every new prompt line
        printf("sh> ");
        fflush(stdout);

        //obtain new input and parse into char array
        if (fgets(input, sizeof(input), stdin) == NULL) {
            perror("fgets");
            exit(EXIT_FAILURE);
        }

        //remove newline character at the end of the input for no mistakes
        input[strcspn(input, "\n")] = '\0';

        //exit shell if user types "exit"
        if (strcmp(input, "exit") == 0) {
            //wait for background processes before exit
	    for (int i = 0; i < num_background_processes; i++) {
                waitpid(background_pids[i], NULL, 0);
            }
            printf("Exiting shell. Thanks for using :)\n");
            break;
        }

        //check for background execution using '&'
        //'&' will make child process
        int background = 0;
        if (input[strlen(input) - 1] == '&') {
            background = 1;
            input[strlen(input) - 1] = '\0'; //remove '&' character
        }

        //fork a new child process
        pid_t child_pid = fork();

        if (child_pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (child_pid == 0) {
            //if 0, it is executed in child process
            //execute the program
            char *args[MAX_INPUT_SIZE / 2]; //max number of args
            int num_args = 0;

            char *token = strtok(input, " ");

            while (token != NULL) {
                args[num_args++] = token;
                token = strtok(NULL, " ");
            }
	    
	    args[num_args] = NULL;

            if (execvp(args[0], args) == -1) {
                perror("execvp");
                exit(EXIT_FAILURE);
            }

        } else {
            //if > 0, the code is executed in the parent process
            //if it is a background process add it to the list

            if (background) {
                background_pids[num_background_processes++] = child_pid;
                //check if we are at maximum number of background processes
                if (num_background_processes >= MAX_BACKGROUND_PROCESSES) {
                    printf("Maximum number of background processes reached.\n");
                    background = 0;
                }
            }
            //if not a background process wait for it to complete
            if (!background) {
                int status;
                waitpid(child_pid, &status, 0);
            }
        }
    }
    return 0;
}