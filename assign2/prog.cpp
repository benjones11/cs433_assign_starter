
/**
 * Assignment 2: Simple UNIX Shell
 * @file pcbtable.h
 * @author Benjamin Jones 
 * @brief This is the main function of a simple UNIX Shell. You may add additional functions in this file for your implementation
 * @version 0.1
 */
// You must complete the all parts marked as "TODO". Delete "TODO" after you are done.
// Remember to add sufficient and clear comments to your code

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fcntl.h>
#include <cstring>
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

#define MAX_LINE 80 // The maximum length command

/**
 * @brief parse out the command and arguments from the input command separated by spaces
 *
 * @param command input buffer; motified by strtok
 * @param args Array of string pointers ro each token 
 * @return int number of tokens found in command 
 */
int parse_command(char command[], char *args[])
{
    // Remove newline character
    //strcspn counts char until a new line is found and makes the string end before the new line 
    command[strcspn(command, "\n")] = '\0';
    
    int arg_count = 0;
    char *token = strtok(command, " "); //this splits the string into tokens at spaces
    
    while (token != NULL && arg_count < MAX_LINE / 2) { //while the token isn't null and the count hasn't reached max
        args[arg_count] = token; //store in each token
        arg_count++;
        token = strtok(NULL, " "); //continue until end of the string 
    }
    
    args[arg_count] = NULL; // Null-terminate for execvp
    return arg_count; //returns count of arguments 
}

// Helper function to check for redirection and modify args array
bool check_redirection(char *args[], int &num_args, char *input_file, char *output_file) {
    input_file[0] = '\0';
    output_file[0] = '\0';
    
    for (int i = 0; i < num_args - 1; i++) {
        if (strcmp(args[i], "<") == 0) {
            strcpy(input_file, args[i + 1]);
            // Remove < and filename from args
            for (int j = i; j < num_args - 2; j++) {
                args[j] = args[j + 2];
            }
            args[num_args - 2] = NULL;
            num_args -= 2;
            return true;
        }
        else if (strcmp(args[i], ">") == 0) {
            strcpy(output_file, args[i + 1]);
            // Remove > and filename from args
            for (int j = i; j < num_args - 2; j++) {
                args[j] = args[j + 2];
            }
            args[num_args - 2] = NULL;
            num_args -= 2;
            return true;
        }
    }
    return false;
}

// Helper function to check for pipe and split command
bool check_pipe(char *args[], int &num_args, char *second_command[], int &second_num_args) {
    for (int i = 0; i < num_args; i++) {
        if (strcmp(args[i], "|") == 0) {
            // Split at pipe
            args[i] = NULL;  // End first command at pipe
            second_num_args = 0;
            
            // Copy second command
            for (int j = i + 1; j < num_args; j++) {
                second_command[second_num_args] = args[j];
                second_num_args++;
            }
            second_command[second_num_args] = NULL;
            
            num_args = i;  // Update first command length
            return true;
        }
    }
    return false;
}

/**
 * @brief The main function of a simple UNIX Shell. You may add additional functions in this file for your implementation
 * @param argc The number of arguments
 * @param argv The array of arguments
 * @return The exit status of the program
 */
int main(int argc, char *argv[])
{
    char command[MAX_LINE];       // the command that was entered
    char *args[MAX_LINE / 2 + 1]; // hold parsed out command line arguments
    int should_run = 1;           /* flag to determine when to exit program */
    char history[MAX_LINE]; 
    // TODO: Add additional variables for the implementation.

    while (should_run)
    {
        printf("osh>");
        fflush(stdout);
        // Read the input command
        fgets(command, MAX_LINE, stdin);

        char commandCopy[MAX_LINE];
        strcpy(commandCopy, command);
        // Parse the input command; copy of what the user entered
        int num_args = parse_command(command, args);

        // TODO: Add your code for the implementation
        /**
         * After reading user input, the steps are:
         * (1) fork a child process using fork()
         * (2) the child process will invoke execvp()
         * (3) parent will invoke wait() unless command included &
         */

        if(num_args == 1 && strcmp(args[0], "!!") == 0){
            if(history[0] == '\0'){
                printf("No commands\n"); 
                continue; 
            }
            strcpy(command, history);
            printf("%s", command);
            num_args = parse_command(command, args);
        }

        if(num_args > 0 && strcmp(args[0], "exit") == 0){
            should_run = 0; 
            continue; 
        }

        if(num_args == 0){
        continue; 
        }

        // Check for pipe first (before redirection)
        char *second_command[MAX_LINE / 2 + 1];
        int second_num_args = 0;
        bool has_pipe = check_pipe(args, num_args, second_command, second_num_args);
        
        // Check for redirection (only if no pipe)
        char input_file[MAX_LINE], output_file[MAX_LINE];
        bool has_redirection = false;
        if (!has_pipe) {
            has_redirection = check_redirection(args, num_args, input_file, output_file);
        }
        
        // Check for background command (&)
        bool background = false;
        if (num_args > 0 && strcmp(args[num_args - 1], "&") == 0) {
            background = true;
            args[num_args - 1] = NULL;  // Remove & from args
            num_args--;
        }

        if (has_pipe) {
            // Handle pipe: ls -l | less
            int pipefd[2];
            if (pipe(pipefd) == -1) {
                perror("pipe failed");
                continue;
            }
            
            // Fork first child (writes to pipe)
            pid_t pid1 = fork();
            if (pid1 == 0) {
                // First child - execute first command, write to pipe
                close(pipefd[0]);  // Close read end
                dup2(pipefd[1], STDOUT_FILENO);  // Redirect stdout to pipe
                close(pipefd[1]);
                
                execvp(args[0], args);
                perror("execvp failed");
                exit(1);
            }
            
            // Fork second child (reads from pipe)
            pid_t pid2 = fork();
            if (pid2 == 0) {
                // Second child - execute second command, read from pipe
                close(pipefd[1]);  // Close write end
                dup2(pipefd[0], STDIN_FILENO);   // Redirect stdin to pipe
                close(pipefd[0]);
                
                execvp(second_command[0], second_command);
                perror("execvp failed");
                exit(1);
            }
            
            // Parent closes pipe and waits for both children
            close(pipefd[0]);
            close(pipefd[1]);
            
            if (!background) {
                wait(NULL);  // Wait for first child
                wait(NULL);  // Wait for second child
            }
        }
        else {
            // Regular command (no pipe)
            pid_t pid = fork(); 

            if (pid == 0) {
                // Child process - handle redirection
                if (has_redirection && input_file[0] != '\0') {
                    // Input redirection
                    int fd = open(input_file, O_RDONLY);
                    if (fd == -1) {
                        perror("open input file failed");
                        exit(1);
                    }
                    dup2(fd, STDIN_FILENO);
                    close(fd);
                }
                
                if (has_redirection && output_file[0] != '\0') {
                    // Output redirection
                    int fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                    if (fd == -1) {
                        perror("open output file failed");
                        exit(1);
                    }
                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }
                
                execvp(args[0], args); 
                perror("execvp failed");
                exit(1); 
            }

            if(pid < 0 ){
                perror("fork failed");
            }
            else if (pid > 0) { 
                if (!background) {
                    wait(NULL);  // Only wait if not background
                }
            }
        }
    

        if(num_args > 0 && strcmp(args[0], "!!") != 0) {
            strcpy(history, commandCopy);
            }
    
    }
    return 0;
}
