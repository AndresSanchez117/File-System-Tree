#include <stdio.h>
#include <string.h>
#include "tree.h"

// ? Should these global variables be static

// Root of the file system tree
Node *root;

// Current working directory on the file system tree
Node *cwd;

// Command table, for use on the findCommand function
char *commands[] = {
    "mkdir",
    "rmdir",
    "ls",
    "cd",
    "pwd",
    "creat",
    "rm",
    "reload",
    "save",
    "menu",
    "quit",
    NULL
};

// Initialize root node of the file system tree
void initializeTree()
{

}

// Find the index number of a given command name
int findCommand(char *command)
{
    for (int i = 0; commands[i] != NULL; i++)
    {
        if (strcmp(commands[i], command) == 0)
        {
            return i;
        }
    }
    return -1;
}

// Commands

// Make a new directory for a given pathname
int mkdir(char *pathname)
{

}

// Remove the directory, if it is empty
int rmdir(char *pathname)
{

}

// Change CWD to pathname, or / (root) if no pathname
int cd(char *pathname)
{

}

// List the directory contents of pathname, or CWD if no pathname
int ls(char *pathname)
{

}

// Print the absolute pathname of CWD
int pwd(char *pathname)
{

}

// Create a FILE node on the given pathname
int creat(char *pathname)
{

}

// Remove the FILE node of the given pathname
int rm(char *pathname)
{

}

// Save the current file system three as a file
int save(char *filename)
{

}

// Construct a file system tree from the given file
int reload(char *filename)
{

}

// Show a menu of valid commands
int menu(char *pathname)
{
    
}

// Save the file system tree, then terminate the program
int quit(char *pathname)
{

}
