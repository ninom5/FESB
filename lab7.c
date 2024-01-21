#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50

struct _directory;
typedef struct _directory* PositionDir;
typedef struct _directory {
    char name[MAX_NAME_LENGTH];
    PositionDir subDirectories;
    PositionDir next;
} Directory;

struct _levelStack;
typedef struct _levelStack* PositionLevelStack;
typedef struct _levelStack {
    PositionDir directoryLevel;
    PositionLevelStack next;
} LevelStack;

PositionDir createDir(char name[MAX_NAME_LENGTH]);
PositionDir createSubdirectory(char name[MAX_NAME_LENGTH], PositionDir currentDirectory);
PositionDir changeDirectory(char name[MAX_NAME_LENGTH], PositionDir currentDirectory);
int listDirectoryContents(PositionDir currentDirectory);

PositionDir pop(PositionLevelStack headLevelStack);
int push(PositionLevelStack headLevelStack, PositionDir directoryLevel);
PositionLevelStack createNewLevelStackElement(PositionDir directoryLevel);

int main() {
    Directory headDirectory = {
        .name = {0},
        .subDirectories = NULL,
        .next = NULL
    };
    PositionDir rootDir = createDir("C:");
    headDirectory.next = rootDir;

    PositionDir currDir = rootDir;

    LevelStack headLevelStack = {
        .directoryLevel = NULL,
        .next = NULL
    };
    push(&headLevelStack, currDir);

    while (1) {
        printf("\nMenu:\n");
        printf("1 - md (Create Directory)\n");
        printf("2 - cd dir (Change Directory)\n");
        printf("3 - cd.. (Go Up)\n");
        printf("4 - dir (List Contents)\n");
        printf("5 - exit\n");

        char choice[10];
        printf("Enter your choice: ");
        scanf("%s", choice);

        if (strcmp(choice, "1") == 0) {
            char directoryName[MAX_NAME_LENGTH];
            printf("\033[0;32mEnter directory name: \033[0m");
            scanf("%s", directoryName);
            createSubdirectory(directoryName, currDir);
        }
        else if (strcmp(choice, "2") == 0) {
            char directoryName[MAX_NAME_LENGTH];
            printf("\033[0;32mEnter directory name: \033[0m");
            scanf("%s", directoryName);
            currDir = changeDirectory(directoryName, currDir);
            push(&headLevelStack, currDir);
        }
        else if (strcmp(choice, "3") == 0) {
            if (currDir != rootDir) {
                currDir = pop(&headLevelStack);
                printf("\033[0;32mCurrently in '%s' \033[0m\n", currDir->name);
            }
            else {
                printf("Already in the root directory.\n");
                return currDir;
            }
        }
        else if (strcmp(choice, "4") == 0) {
            listDirectoryContents(currDir);
        }
        else if (strcmp(choice, "5") == 0) {
            printf("Exiting the program.\n");
            break;
        }
        else {
            printf("\033[0;31mInvalid choice. Please enter a valid option.\033[0m\n");
        }
    }

    // Free allocated memory
    free(rootDir);

    return 0;
}

/*Directory functions*/

PositionDir createDir(char name[MAX_NAME_LENGTH]) {
    PositionDir newDirectory = NULL;
    newDirectory = (PositionDir)malloc(sizeof(Directory));
    if (!newDirectory) {
        printf("Can't allocate memory!\n");
        return NULL;
    }
    strcpy(newDirectory->name, name);
    newDirectory->subDirectories = NULL;
    newDirectory->next = NULL;
    return newDirectory;
}

PositionDir createSubdirectory(char name[MAX_NAME_LENGTH], PositionDir currentDirectory) {
    PositionDir newDirectory = NULL;
    newDirectory = createDir(name);
    if (!newDirectory) {
        printf("New directory wasn't created!\n");
        return NULL;
    }
    newDirectory->next = currentDirectory->subDirectories;
    currentDirectory->subDirectories = newDirectory;
    return newDirectory;
}

PositionDir changeDirectory(char name[MAX_NAME_LENGTH], PositionDir currentDirectory) {
    PositionDir subdirectory = currentDirectory->subDirectories;
    while (subdirectory != NULL) {
        if (strcmp(subdirectory->name, name) == 0) {
            return subdirectory;
        }
        subdirectory = subdirectory->next;
    }
    printf("Directory '%s' not found.\n", name);
    return currentDirectory;
}

int listDirectoryContents(PositionDir currentDirectory) {
    printf("\033[0;32mContents of directory '%s':\033[0m\n", currentDirectory->name);
    PositionDir subdirectory = currentDirectory->subDirectories;
    while (subdirectory != NULL) {
        printf("\033[0;32m - %s\033[0m\n", subdirectory->name);
        subdirectory = subdirectory->next;
    }
    if (currentDirectory->subDirectories == NULL) {
        printf("\033[0;32m   (empty)\033[0m\n");
    }
    return EXIT_SUCCESS;
}

/*Stack functions*/

PositionDir pop(PositionLevelStack headLevelStack) {
    PositionLevelStack toDelete = NULL;
    PositionDir directoryLevel = NULL;

    toDelete = headLevelStack->next;
    if (!toDelete) {
        printf("Stack is empty! Nothing to pop!\n");
        return NULL;
    }

    headLevelStack->next = toDelete->next;
    directoryLevel = toDelete->directoryLevel;
    free(toDelete);

    return directoryLevel;
}

int push(PositionLevelStack headLevelStack, PositionDir directoryLevel) {
    PositionLevelStack newLevelStackElement = NULL;

    newLevelStackElement = createNewLevelStackElement(directoryLevel);
    if (!newLevelStackElement) {
        perror("Error in creating new element!\n");
        return NULL;
    }

    newLevelStackElement->next = headLevelStack->next;
    headLevelStack->next = newLevelStackElement;
}

PositionLevelStack createNewLevelStackElement(PositionDir directoryLevel) {
    PositionLevelStack newLevelStackElement = NULL;

    newLevelStackElement = (PositionLevelStack)malloc(sizeof(LevelStack));
    if (!newLevelStackElement) {
        perror("Can't allocate memory!\n");
        return NULL;
    }

    newLevelStackElement->directoryLevel = directoryLevel;
    newLevelStackElement->next = NULL;

    return newLevelStackElement;
}