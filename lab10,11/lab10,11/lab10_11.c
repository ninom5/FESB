#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_SIZE (50)
#define MAX_LINE (1024)
#define HASH_TABLE_SIZE 11

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
struct _hash_table {
	PositionCountry buckets[HASH_TABLE_SIZE];
};
typedef struct _hash_table HashTable;

HashTable* createHashTable() {
	HashTable* hashTable = (HashTable*)malloc(sizeof(HashTable));
	if (!hashTable) {
		printf("Can't allocate memory for hash table!\n");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < HASH_TABLE_SIZE; i++) {
		hashTable->buckets[i] = NULL;
	}

	return hashTable;
}

// Function to calculate the hash value for a given country name
unsigned int calculateHash(char* countryName) {
	unsigned int hashValue = 0;
	int i = 0;
	while (i < 5 && countryName[i] != '\0') {
		hashValue += (unsigned int)countryName[i];
		i++;
	}
	return hashValue % HASH_TABLE_SIZE;
}

// Function to insert a country into the hash table
void insertCountryIntoHashTable(HashTable* hashTable, PositionCountry newCountry) {
	unsigned int index = calculateHash(newCountry->name);

	if (hashTable->buckets[index] == NULL) {
		hashTable->buckets[index] = newCountry;
		newCountry->next = NULL;
	}
	else {
		// Handle collision by adding the new country to the linked list at this index
		newCountry->next = hashTable->buckets[index];
		hashTable->buckets[index] = newCountry;
	}
}

// Function to search for a country in the hash table
PositionCountry searchCountryInHashTable(HashTable* hashTable, char* countryName) {
	unsigned int index = calculateHash(countryName);
	PositionCountry currentCountry = hashTable->buckets[index];

	while (currentCountry != NULL && strcmp(currentCountry->name, countryName) != 0) {
		currentCountry = currentCountry->next;
	}

	return currentCountry;
}

int printAllCountriesFromHash(HashTable* hashTable) {
	for (int index = 0; index < HASH_TABLE_SIZE; index++) {
		PositionCountry currentCountry = NULL;
		currentCountry = hashTable->buckets[index];

		while (currentCountry != NULL) {
			printf("\n%s", currentCountry->name);
			printTownTree(currentCountry->townRootTree);
			currentCountry = currentCountry->next;
		}
	}
}
PositionCountry searchCountryTree(PositionCountry countryRootTree, char* countryName);
int findCountries(PositionCountry countryHeadList, PositionCountry countryRootTree, PositionCountry* countries);
int printTownList(PositionTown townHeadList);
int printCountryTree(PositionCountry rootCountryTree);
int printTownTree(PositionTown townRootTree);
int printCountryList(PositionCountry headListCountry);
PositionCountry insertNewCountryTree(PositionCountry countryRootTree, PositionCountry newCountry);
int insertCountryAfter(PositionCountry currentCountry, PositionCountry newCountry);
int insertSortedNewCountryList(PositionCountry townHeadList, PositionCountry newCountry);
PositionTown insertNewTownTree(PositionTown townRootTree, PositionTown newTown);
int insertTownAfter(PositionTown townCurrent, PositionTown newTown);
PositionTown createNewTown(char* townName, int population);
PositionCountry createCountry(char* countryName);
PositionCountry CreateNewCountry(char* countryName, char* countryFile, PositionCountry* countries);
PositionCountry readAndFillCountries(PositionCountry countryHeadList, PositionCountry countryRootTree);
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
PositionTown createNewTown(char *townName, int population)
{
	PositionTown town = NULL;
	town = (PositionTown)malloc(sizeof(Town));
	if (!town) {
		printf("Can't allocate memory!\n");
		return -1;
	}

	strcpy(town->name, townName);
	town->population = population;
	town->next = NULL;
	town->left = NULL;
	town->right = NULL;

	return town;
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
int printCountryList(PositionCountry headListCountry)
{
	PositionCountry currentCountry = headListCountry->next;

	while (currentCountry != NULL)
	{
		printf("%s\n", currentCountry->name);
		printTownTree(currentCountry->townRootTree);
		currentCountry = currentCountry->next;
	}
	return EXIT_SUCCESS;
}
int printTownTree(PositionTown townRootTree)
{
	if (townRootTree)
	{
		printTownTree(townRootTree->left);
		printf("%s %d\n", townRootTree->name, townRootTree->population);
		printTownTree(townRootTree->right);
	}
	return EXIT_SUCCESS;
}
int printCountryTree(PositionCountry rootCountryTree)
{
	if (rootCountryTree)
	{
		printCountryTree(rootCountryTree->left);
		printf("%s\n", rootCountryTree->name);
		printTownList(&rootCountryTree->townListHead);
		printCountryTree(rootCountryTree->right);
	}
	return EXIT_SUCCESS;
}
int printTownList(PositionTown townHeadList)
{
	PositionTown currentTown = townHeadList->next;

	while (currentTown != NULL)
	{
		printf("%s %d", currentTown->name, currentTown->population);

		currentTown = currentTown->next;
	}

	return EXIT_SUCCESS;
}
int findCountries(PositionCountry countryHeadList, PositionCountry countryRootTree, PositionCountry* countries) {
	PositionCountry countryCurrentList = countryHeadList->next;
	PositionCountry countryCurrentTree = countryRootTree;
	char countryName[MAX_SIZE] = { 0 };
	printf("\n\nEnter country name: ");
	scanf(" %s", countryName);

	while (countryCurrentList != NULL && strcmp(countryCurrentList->name, countryName)) 
	{
		countryCurrentList = countryCurrentList->next;
	}

	countryCurrentTree = searchCountryTree(countryRootTree, countryName);

	countries[0] = countryCurrentList;
	countries[1] = countryCurrentTree;

	return EXIT_SUCCESS;
}
PositionCountry searchCountryTree(PositionCountry countryRootTree, char* countryName)
{
	if (countryRootTree == NULL || strcmp(countryRootTree->name, countryName) == 0)
		return countryRootTree;

	if (strcmp(countryRootTree->name, countryName) > 0)
		return searchCountryTree(countryRootTree->left, countryName);

	return searchCountryTree(countryRootTree->right, countryName);
}