#pragma once

// File system node
typedef struct Node
{
    // Name of the node
    char name[64];

    // Node type, 'D' for directory, 'F' for file
    char type;

    // Points to the oldest child
    struct Node *child;
    
    // Points to the oldest sibling
    struct Node *sibling;

    // Points to the parent node
    struct Node *parent;
} Node;

// Initialize root node of the file system tree
void initializeTree();

// Find the index number of a given command name
int findCommand(char *command);

// ? Have a single int callCommand(char *command, char *pathname) function as a public interface

// Commands

// Make a new directory for a given pathname
int mkdir(char *pathname);

// Remove the directory, if it is empty
int rmdir(char *pathname);

// Change CWD to pathname, or / (root) if no pathname
int cd(char *pathname);

// List the directory contents of pathname, or CWD if no pathname
int ls(char *pathname);

// Print the absolute pathname of CWD (does not use pathname argument)
int pwd(char *pathname);

// Create a FILE node on the given pathname
int creat(char *pathname);

// Remove the FILE node of the given pathname
int rm(char *pathname);

// Save the current file system three as a file
int save(char *filename);

// Construct a file system tree from the given file
int reload(char *filename);

// Show a menu of valid commands (does not use pathname argument)
int menu(char *pathname);

// Save the file system tree, then terminate the program (does not use pathname argument)
int quit(char *pathname);

// ? Use switch satement to call functions instead?
// Table of function pointers, to easily call commands
static int (*functionPtr[])(char *) = {
    mkdir,
    rmdir,
    ls,
    cd,
    pwd,
    creat,
    rm,
    reload,
    save,
    menu,
    quit
};
