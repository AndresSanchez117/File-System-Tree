#include <stdio.h>
#include <string.h>
#include "tree.h"

int main()
{
    // For user input
    char line[128], command[16], pathname[64];

    // Initialize root node of the file system tree
    initializeTree();

    printf("FILE SYSTEM TREE.\n");
    printf("Type \"menu\" for available commands.\n");

    while(1)
    {
        // Prompt
        printf("$ ");

        // Get user input
        fgets(line, 128, stdin);
        line[strlen(line) - 1] = '\0';

        // Separate input in coommand and pathaname
        sscanf(line, "%s %s", command, pathname);
        
        // Find and exeute coommand, it it exists
        int index = findCommand(command);
        if (index != -1)
        {
            int ret = functionPtr[index](pathname);
        }
        else
        {
            printf("Invalid command.\n");
        }
    }
}