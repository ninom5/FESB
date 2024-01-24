#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_SIZE (50)
#define MAX_LINE (1024)

struct town;
typedef struct town* PositionTown;
typedef struct town
{
	char name[MAX_SIZE];
	int population;
	PositionTown left;
	PositionTown right;
	PositionTown next;
}Town;

struct country;
typedef struct country* PositionCountry;
typedef struct country
{
	char name[MAX_SIZE];
	PositionCountry next;
	PositionCountry left;
	PositionCountry right;
	PositionTown townRootTree;
	Town townListHead;

}Country;


int main()
{
	Country CountryHeadList = 
	{ .name = {0}, .next = NULL, .left = NULL, .right = NULL, .townRootTree = NULL, .townListHead = 
		{
		.name = {0}, .left = NULL, .next = NULL, .population = 0, .right = NULL
		}
	};

	PositionCountry countryRootTree = NULL;
	PositionCountry countryCurrentList = NULL;
	PositionCountry countryCurrentTree = NULL;
	PositionCountry countries[] = { NULL, NULL };

	countryRootTree = readAndFillCountries(&CountryHeadList, countryRootTree);
	printf("list\n");
	printCountryList(&CountryHeadList);
	printf("\n________________\n");
	printf("tree\n");
	printCountryTree(countryRootTree);


	return 0;
}
PositionCountry readAndFillCountries(PositionCountry countryHeadList, PositionCountry countryRootTree)
{
	FILE* filePointer;
	char countryName[MAX_SIZE];
	char countryFile[MAX_SIZE];
	PositionCountry countries[] = {NULL, NULL, NULL};
	PositionCountry newCountryList = NULL;
	PositionCountry newCountryTree = NULL;
	PositionCountry newCountryHash = NULL;
	PositionCountry countries[] = { NULL, NULL, NULL };
	char nullString[MAX_LINE] = { 0 };

	filePointer = fopen("drzave.txt", "r");
	if (!filePointer)
	{
		printf("cant open the file!\n");
		return -1;
	}

	while (!feof(filePointer))
	{
		fscanf("%s %s", countryName, countryFile);
		CreateNewCountry(countryName, countryFile, countries);
		newCountryList = countries[0];
		newCountryTree = countries[1];
		newCountryHash = countries[2];
		insertSortedNewCountryList(countryHeadList, newCountryList);
		countryRootTree = insertNewCountryTree(countryRootTree, newCountryTree);
		strcpy(countryName, nullString);
		strcpy(countryFile, nullString);
	}
	fclose(filePointer);

	return countryRootTree;
}
PositionCountry CreateNewCountry(char* countryName, char* countryFile, PositionCountry* countries)
{
	char countryLocation[MAX_SIZE] = {0};
	FILE* countryFilePointer = NULL;
	char townName[MAX_SIZE];
	int population;
	PositionCountry newCountryList = NULL;
	PositionCountry newCountryTree = NULL;
	PositionTown newTownList = NULL;
	PositionTown newTownTree = NULL;

	strcpy(countryLocation, countryFile);

	countryFilePointer = fopen(countryLocation, "r");
	if (!countryFilePointer)
	{
		perror("Cant open file");
		return -1;
	}

	newCountryList = createCountry(countryName);
	newCountryTree = createCountry(countryName);


	while (!feof(countryFilePointer))
	{
		fscanf(countryFilePointer, "%s %d", townName, &population);
		newTownList = createNewTown(townName, population);
		newTownTree = createNewTown(townName, population);
		insertSortedTownList(&newCountryTree->townListHead, newTownList);
		newCountryList->townRootTree = insertNewTownTree(newCountryList->townRootTree, newTownTree);

		newTownList = NULL;
	}

	countries[0] = newCountryList;
	countries[1] = newCountryTree;

	fclose(countryFilePointer);

	return 0;
}
PositionCountry createCountry(char * countryName)
{
	PositionCountry newCountry = NULL;
	newCountry = (PositionCountry)malloc(sizeof(Country));

	if (!newCountry)
	{
		printf("cant allocate memory\n");
		return NULL;
	}

	strcpy(newCountry->name, countryName);
	newCountry->next = NULL;
	newCountry->left = NULL;
	newCountry->right = NULL;
	newCountry->townRootTree = NULL;

	newCountry->townListHead.population = 0;
	newCountry->townListHead.next = NULL;
	newCountry->townListHead.left = NULL;
	newCountry->townListHead.right = NULL;

	return newCountry;
}
PositionTown createNewTown(char *townName, int population)
{
	PositionTown newTown = NULL;
	if (!newTown)
	{
		printf("cant allocate memory\n");
		return NULL;
	}

	strcpy(newTown->name, townName);
	newTown->population = population;
	newTown->next = NULL;
	newTown->left = NULL;
	newTown->right = NULL;
	
	return newTown;
}
int insertSortedTownList(PositionTown townHeadList, PositionTown newTown)
{
	PositionTown townCurrent = townHeadList;

	while (townCurrent->next != NULL && townCurrent->next->population > newTown->population)
	{
		townCurrent = townCurrent->next;
	}

	insertTownAfter(townCurrent, newTown);

	return EXIT_SUCCESS;
}
int insertTownAfter(PositionTown townCurrent, PositionTown newTown)
{
	newTown->next = townCurrent->next;
	townCurrent->next = newTown;
	
	return EXIT_SUCCESS;
}
PositionTown insertNewTownTree(PositionTown townRootTree, PositionTown newTown)
{
	if (townRootTree == NULL)
		return townRootTree;

	if (newTown->population > townRootTree->population)
		townRootTree->left = insertNewTownTree(townRootTree->left, newTown);
	else if (newTown->population <= townRootTree->population)
		townRootTree->right = insertNewTownTree(townRootTree->right, newTown);

	return townRootTree;
}
int insertSortedNewCountryList(PositionCountry townHeadList, PositionCountry newCountry)
{
	PositionCountry currentCountry = townHeadList;
	while (currentCountry->next != NULL && strcmp(currentCountry->next->name, newCountry->name) < 0)
	{
		currentCountry = currentCountry->next;
	}
	insertCountryAfter(currentCountry, newCountry);

	return 0;
}
int insertCountryAfter(PositionCountry currentCountry, PositionCountry newCountry) 
{
	newCountry->next = currentCountry->next;
	currentCountry->next = newCountry;

	return EXIT_SUCCESS;
}
PositionCountry insertNewCountryTree(PositionCountry countryRootTree, PositionCountry newCountry) {
	if (countryRootTree == NULL)
		return newCountry;

	if (strcmp(countryRootTree->name, newCountry->name) > 0)
		countryRootTree->left = insertNewCountryTree(countryRootTree->left, newCountry);
	else if (strcmp(countryRootTree->name, newCountry->name) <= 0)
		countryRootTree->right = insertNewCountryTree(countryRootTree->right, newCountry);

	return countryRootTree;
}
