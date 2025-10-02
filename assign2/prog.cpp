
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

// TODO: Add additional functions if you need

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

    // TODO: Add additional variables for the implementation.

    while (should_run)
    {
        printf("osh>");
        fflush(stdout);
        // Read the input command
        fgets(command, MAX_LINE, stdin);
        // Parse the input command
        int num_args = parse_command(command, args);

        // TODO: Add your code for the implementation
        /**
         * After reading user input, the steps are:
         * (1) fork a child process using fork()
         * (2) the child process will invoke execvp()
         * (3) parent will invoke wait() unless command included &
         */
    
    }
    return 0;
}
