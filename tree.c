#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libgen.h>
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

// Create a new node for the file system tree
Node* createNode(char* name, char type, Node* parent)
{
    Node *node = (Node *)malloc(sizeof(Node));

    strcpy(node->name, name);
    node->type = type;
    node->parent = parent;
    node->child = NULL;
    node->sibling = NULL;

    return node;
}

// Initialize root node of the file system tree
void initializeTree()
{
    // TODO: Deallocate memory of previous tree, maybe with the rm and rmDir functions with post order traversal

    root = createNode("/", 'D', NULL);
    root->parent = root;
    root->sibling = root;
    cwd = root;
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

// Returns 1 if pathname is absolute, 0 otherwise
int isAbsolute(char *pathname)
{
    return pathname[0] == '/';
}

// Separates given pathname, storing the dirname and basename in given char *
void dirAndBaseName(char *pathname, char *dname, char *bname)
{
    char temp[128];
    strcpy(temp, pathname);
    strcpy(dname, dirname(temp));
    strcpy(temp, pathname);
    strcpy(bname, basename(temp));
}

// Find a sibling node of start with the given name
Node* searchSibling(char* name, Node* start)
{
    Node *p = start;
    while (p != NULL)
    {
        if (strcmp(p->name, name) == 0)
        {
            return p;
        }
        p = p->sibling;
    }
    return NULL;
}

// Search a directory of the given dirname
Node* searchDir(char* dname)
{
    Node *start = NULL;
    if (isAbsolute(dname))
    {
        start = root;
    }
    else
    {
        start = cwd;
        if (strcmp(dname, ".") == 0)
        {
            return start;
        }
    }

    char temp[64];
    strcpy(temp, dname);
    char *nextDir = strtok(temp, "/");

    while (nextDir != NULL && start != NULL)
    {
        start = searchSibling(nextDir, start->child);
        nextDir = strtok(NULL, "/");
    }

    if (start != NULL && start->type == 'D')
    {
        return start;
    }
    return NULL;
}

// Insert a new silbing node for the given node
void insertSibling(Node **start, Node* sibling)
{
    Node *q = *start;

    // If list is empty, insert sibling as first element
    if (q == NULL)
    {
        *start = sibling;
    }
    // List is not empty
    else
    {
        // Go to last element
        while (q->sibling)
        {
            q = q->sibling;
        }
        // Last element points to sibling
        q->sibling = sibling;
    }
}

// Delete sibling node of start with the given name, returns deleted node
Node* deleteSibling(Node **start, char *name)
{
    Node *p, *q;
    // If no siblings
    if (*start == NULL)
    {
        return NULL;
    }
    p = *start;
    // If sibling is first element
    if (strcmp(p->name, name) == 0)
    {
        *start = p->sibling;
        return p;
    }

    // Element is not the first one
    q = p->sibling;
    while (q != NULL)
    {
        if (strcmp(q->name, name) == 0)
        {
            p->sibling = q->sibling;
            return q;
        }
        p = q;
        q = q->sibling;
    }
    return NULL;
}

// Commands

// Make a new directory for a given pathname
int mkdir(char *pathname)
{
    // Separate pathname into a directoryname and basename
    char dname[64], bname[64];
    dirAndBaseName(pathname, dname, bname);

    // Search for directory
    Node *dir = searchDir(dname);
    if (dir == NULL)
    {
        printf("ERROR: Directory does not exist.\n");
        return 1;
    }

    // Check if a node with the same basename already exists
    Node *bnameNode = searchSibling(bname, dir->child);
    if (bnameNode != NULL)
    {
        printf("ERROR: A directory or file of the same name already exists.\n");
        return 1;
    }

    // Add directory node
    Node *newDir = createNode(bname, 'D', dir);
    insertSibling(&dir->child, newDir);
    return 0;
}

// Remove the directory, if it is empty
int rmdir(char *pathname)
{
    // Search directory
    Node *dir = searchDir(pathname);

    // Checks
    if (dir == NULL)
    {
        printf("ERROR: Directory does not exist.\n");
        return 1;
    }
    if (dir->child != NULL)
    {
        printf("ERROR: Directory is not empty.\n");
        return 1;
    }

    // Delete direcory
    dir = deleteSibling(&(dir->parent->child), dir->name);
    free(dir);
    return 0;
}

// Change CWD to pathname, or / (root) if no pathname
int cd(char *pathname)
{
    // TODO: If no pathname, cwd should point to root

    // Search directory
    Node *dir = searchDir(pathname);

    // Check if directory found
    if (dir == NULL)
    {
        printf("ERROR: Directory does not exist.\n");
        return 1;
    }

    // cd
    cwd = dir;
    return 0;
}

// List the directory contents of pathname, or CWD if no pathname
int ls(char *pathname)
{
    Node *dir = searchDir(pathname);

    if (dir == NULL)
    {
        printf("ERROR: Directory does not exist.\n");
        return 1;
    }

    // List contents
    Node *child = dir->child;
    while (child != NULL)
    {
        printf("%c %s\n", child->type, child->name);
        child = child->sibling;
    }
    return 0;
}

void pwdRecurse(Node* current)
{
    // Base case
    if (strcmp(current->name, "/") == 0)
    {
        printf("/");
        return;
    }
    pwdRecurse(current->parent);
    printf("%s/", current->name);
}

// Print the absolute pathname of CWD (does not use pathname argument)
int pwd(char *pathname)
{
    pwdRecurse(cwd);
    printf("\n");
}

// Create a FILE node on the given pathname
int creat(char *pathname)
{
    // Separate pathname into a directory name and basename
    char dname[64], bname[64];
    dirAndBaseName(pathname, dname, bname);

    // Search for directory
    Node *dir = searchDir(dname);
    if (dir == NULL)
    {
        printf("ERROR: Directory does not exist.\n");
        return 1;
    }

    // Check if a node with the same basename already exists
    Node *bnameNode = searchSibling(bname, dir->child);
    if (bnameNode != NULL)
    {
        printf("ERROR: A directory or file of the same name already exists.\n");
        return 1;
    }

    // Add file node
    Node *newFile = createNode(bname, 'F', dir);
    insertSibling(&dir->child, newFile);
    return 0;
}

// Remove the FILE node of the given pathname
int rm(char *pathname)
{
    // Separate pathname into a directoryname and basename
    char dname[64], bname[64];
    dirAndBaseName(pathname, dname, bname);

    // Search for directory
    Node *dir = searchDir(dname);
    if (dir == NULL)
    {
        printf("ERROR: Directory does not exist.\n");
        return 1;
    }

    // Search and checks
    Node *file = searchSibling(bname, dir->child);
    if (file == NULL || file->type == 'D')
    {
        printf("ERROR: File does not exist.\n");
        return 1;
    }

    // Delete file
    file = deleteSibling(&(dir->child), bname);
    free(file);
    return 0;
}

void treeTraversal(FILE *fp, Node *node, char *dname)
{
    // Base case
    if (node == NULL)
    {
        return;
    }

    // TODO: This is not very efficient, or safe
    char pathname[128];
    pathname[0] = '\0';
    strcpy(pathname, dname);
    strcat(pathname, "/");
    strcat(pathname, node->name);

    // Visit
    fprintf(fp, "%c %s\n", node->type, pathname);

    // Children
    treeTraversal(fp, node->child, pathname);

    // Sibling
    treeTraversal(fp, node->sibling, dname);
}

// Save the current file system tree as a file
int save(char *filename)
{
    FILE *fp = fopen(filename, "w");
    treeTraversal(fp, root->child, "");
    fclose(fp);
}

// Construct a file system tree from the given file
int reload(char *filename)
{
    // Open file
    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        printf("ERROR: File not found.\n");
        return 1;
    }
        
    // Reset tree
    initializeTree();

    // Read file line by line
    char line[128], type[2], pathname[128];
    while (fgets(line, 128, fp))
    {
        line[strlen(line) - 1] = '\0';

        // Separate line in type of node and pathname
        sscanf(line, "%s %s", type, pathname);

        // Insert node
        if (type[0] == 'D')
        {
            mkdir(pathname);
        }
        else
        {
            creat(pathname);
        }
    }
    fclose(fp);
    return 0;
}

// Show a menu of valid commands, does not use pathname argument
int menu(char *pathname)
{
    printf("mkdir [pathname]\n");
    printf("rmdir [pathname]\n");
    printf("ls [pathname]?\n");
    printf("cd [pathname]?\n");
    printf("pwd\n");
    printf("creat [pathname]\n");
    printf("rm [pathname]\n");
    printf("save [filename]\n");
    printf("reload [filename]\n");
    printf("menu\n");
    printf("quit\n");
    return 0;
}

// Save the file system tree, then terminate the program
int quit(char *pathname)
{
    save("fileSystemTree.txt");
    exit(0);
}
