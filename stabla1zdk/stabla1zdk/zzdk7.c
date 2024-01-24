#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_NAME_LENGTH (100)
struct _directory;
typedef struct _directory* PositionDir;
typedef struct _directory 
{
	char name[MAX_NAME_LENGTH];
	PositionDir subDirectory;
	PositionDir next;
}Directory;

struct _levelStack;
typedef struct _levelStack* PositionLevelStack;
typedef struct _levelStack {
	PositionDir directoryLevel;
	PositionLevelStack next;
} LevelStack;

PositionDir createDir(char dirName[MAX_NAME_LENGTH]);
PositionDir createSubDirectory(char dirName[MAX_NAME_LENGTH], PositionDir currentDirectory);
PositionDir changeDir(char dirName[MAX_NAME_LENGTH], PositionDir currentDir);
PositionDir pop(PositionLevelStack headLevelStack);
int push(PositionLevelStack headLevelStack, PositionDir directoryLevel);
PositionLevelStack createNewLevelStackElement(PositionDir directoryLevel);
int listDir(PositionDir currentDir);

int main()
{
	Directory headDir = { .name = {0}, .subDirectory = NULL, .next = NULL };

	PositionDir rootDir = createDir("C:");
	headDir.next = rootDir;

	PositionDir currentDir = rootDir;

	LevelStack headLevelStack = {
	   .directoryLevel = NULL,
	   .next = NULL
	};
	push(&headLevelStack, currentDir);

	while (1)
	{
		printf("\nMenu:\n");
		printf("1 - md (Create Directory)\n");
		printf("2 - cd dir (Change Directory)\n");
		printf("3 - cd.. (Go Up)\n");
		printf("4 - dir (List Contents)\n");
		printf("5 - exit\n");

		int num = 0;
		scanf(" %d", &num);

		switch (num)
		{
		case 1:
		{
			char dirName[MAX_NAME_LENGTH];
			printf("Enter directory name\n");
			scanf(" %s", dirName);
			createSubDirectory(dirName, currentDir);
		}
		case 2:
		{
			char dirName[MAX_NAME_LENGTH];
			printf("Enter directory name: \n");
			scanf("%s", dirName);
			currentDir = changeDir(dirName, currentDir);
			push(&headLevelStack, currentDir);
		}
		case 3:
		{
			if (currentDir != rootDir)
			{
				currentDir = pop(&headLevelStack);
				printf("\033[0;32mCurrently in '%s' \033[0m\n", currentDir->name);
			}
		}
		case 4:
			listDir(currentDir);
		case 5:
			break;
		default:
			printf("Wrong number!\nTry again.\n");
		}
	}
	return 0;
}
PositionDir createDir(char dirName[MAX_NAME_LENGTH])
{
	PositionDir newDir = NULL;
	newDir = (PositionDir)malloc(sizeof(Directory));

	if (!newDir)
	{
		printf("Can t allocat memory\n");
		return NULL;
	}

	strcpy(newDir->name, dirName);

	newDir->subDirectory = NULL;
	newDir->next = NULL;
	return newDir;
}
PositionDir createSubDirectory(char dirName[MAX_NAME_LENGTH], PositionDir currentDirectory)
{
	PositionDir newDir = NULL;
	newDir = createDir(dirName);
	if (!newDir)
	{
		printf("new directory wasn t created\n");
		return NULL;
	}
	
	newDir->next = currentDirectory->subDirectory;
	currentDirectory->subDirectory = newDir;

	return newDir;
}
PositionDir changeDir(char dirName[MAX_NAME_LENGTH], PositionDir currentDir)
{
	PositionDir subdirectory = currentDir->subDirectory;
	while (subdirectory != NULL) {
		if (strcmp(subdirectory->name, dirName) == 0) {
			return subdirectory;
		}
		subdirectory = subdirectory->next;
	}
	printf("Directory '%s' not found.\n", dirName);
	return currentDir;
}
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
int listDir(PositionDir currentDir)
{
	printf("Contents of %s", currentDir->name);
	
	PositionDir subDir = currentDir->subDirectory;

	if (currentDir->subDirectory == NULL)
	{
		printf("Empty\n");
	}

	while (subDir != NULL)
	{
		printf("%s", subDir->name);
		subDir = subDir->next;
	}
	return EXIT_SUCCESS;
}