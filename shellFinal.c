#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

//for colors in text
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

//for the sizes to initial
#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 100
bool showPID = false;

// Function to execute a command.
void execute_command(char *command, bool isBackground) {
    int status;
    pid_t childPid = fork(); // Fork a new process.

    if (childPid == -1) {
        perror("fork"); // Handle fork error.
        exit(EXIT_FAILURE);
    } else if (childPid == 0) { // In the child process.
        char *arguments[MAX_ARGS];
        char *token = strtok(command, " ");
        int argCount = 0;

        // Tokenize the command into arguments.
        while (token != NULL && argCount < MAX_ARGS - 1) {
            arguments[argCount++] = token;
            token = strtok(NULL, " ");
        }

        arguments[argCount] = NULL;
        if(showPID)
        {
        printf("Child process PID: %d\n", getpid());
    	}

        // Execute the command.
        if (execvp(arguments[0], arguments) == -1) {
            perror("execvp"); // Handle execvp error.
            exit(EXIT_FAILURE);
        }
    } else { // In the parent process.
    	if(showPID)
    	{
        	printf("Parent process PID: %d\n", getpid());
        }
        if (!isBackground) {
            waitpid(childPid, &status, 0); // Wait for the child process to finish if not in the background.
        }
    }
}

//cd function
void change_directory(char *directory) {
    if (chdir(directory) == -1) {
        perror("chdir"); // Handle chdir error.
    }
}

//toggle for showing PID for visability
void show_PID_Toggle(void) {
	if(showPID){
		showPID = false;
		return;
	}
	showPID = true;
	return;
}

//shows a help text
void show_help(void){
	printf("\nWelcome to ");
	printf(RED "C1b0" RESET);
	printf(" - Your Simple Unix Shell\n");
    printf("-------------------------------------------------\n");
    printf(RED "C1b0" RESET)
    printf(" is a basic Unix shell that supports various\n");
    printf("commands and features. Here's how you can use it:\n");
    printf("\n");
    printf("Available Commands:\n");
    printf("1. Run external programs by typing their names.\n");
    printf("   Example: 'ls -l' or 'gcc myprogram.c'\n");
    printf("2. Change the current working directory using 'cd'.\n");
    printf("   Example: 'cd /path/to/directory'\n");
    printf("3. Run commands in the background by adding '&'.\n");
    printf("   Example: 'myprogram &' or 'sleep 10 &'\n");
    printf("4. Exit the shell by typing 'exit'.\n");
    printf("5. Toggle showing the parent and child PID numbers by typing 'showPID'.\n");
    printf("\n");
    printf("Notes:\n");
    printf("- You can run most standard Unix commands.\n");
    printf("- Background commands will not wait for completion.\n");
    printf("- To run programs in the current directory, use './'.\n");
    printf("\n");
    printf("Enjoy using C1b0!\n");
    printf("-------------------------------------------------\n");
}

// Main loop for the shell.
void main_loop() {

	printf("       ____________\n");
	printf("      |.---------.|\n");
	printf("      ||         ||\n");
	printf("      ||   ");
	printf(RED "C1b0  " RESET);
	printf("||\n");
	printf("      ||         ||\n");
	printf("      |'---------'|\n");
	printf("       `)__ ____('\n");
	printf("       [=== -- o ]--.\n");
	printf("     __'---------'__ \\\n");
	printf("    [::::::::::: :::] )\n");
	printf("     `\"\"'\"\"\"\"\"\"\"`/T\\\n");
	printf("                    \\_/\n");

    printf(GRN "\nWelcome to my simple Unix shell, C1b0!" RESET);
    printf(GRN "\nI hope you enjoy, type 'help' if you need help!\n\n" RESET);

    char input[MAX_INPUT_SIZE];

    while (1) {
        printf(RED "C1b0> " RESET);
        fflush(stdout); //this will flush the output buffer 

        // Read user input.
        if (fgets(input, sizeof(input), stdin) == NULL) {
            perror("fgets"); // Handle fgets error.
            exit(EXIT_FAILURE);
        }

        input[strlen(input) - 1] = '\0'; //gets length of input and sets the last

        if (strlen(input) == 0) {
            continue; //will skip through and await next input
        }

        bool isBackground = false; //set up for if it runs in the background

        // Check if the command should run in the background.
        if (input[strlen(input) - 1] == '&') {
            isBackground = true;
            input[strlen(input) - 1] = '\0';
        }

        // Check for built-in commands.
        if (strcmp(input, "exit") == 0) {
        	printf(CYN "goodbye...\n" RESET);
            exit(EXIT_SUCCESS);
        } else if (strncmp(input, "cd ", 3) == 0) {
            char *directory = input + 3; // Get the directory argument.
            change_directory(directory);
        } else if (strcmp(input, "showPID") == 0) {
        	show_PID_Toggle();
        } else if (strcmp(input, "help") == 0) {
        	show_help();
        } else {
            execute_command(input, isBackground); // Execute the command.
        }
    }
}

int main() {
    main_loop(); // Start the main shell loop.
    return 0;
}
