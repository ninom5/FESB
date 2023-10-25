#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX 50
#define _CRT_SECURE_NO_WARNINGS
typedef Person* Position;
typedef struct person
{
	char name[MAX];
	char surname[MAX];
	int birth_year;
	Position Next;
}Person;
int appendList(Position Head);
int addToEndOfList(Position Head);
Position createPerson();
Position findLast(Position Head);
int printing(Position P);
int menu(Position Head);

int main()
{
	int x = 2;
	Person Head;
	menu(&Head);

	return EXIT_SUCCESS;
}
Position createPerson()
{
	Position newPerson = NULL;
	char name[MAX] = { 0 };
	char surname[MAX] = { 0 };
	int birth_year = 0;

	newPerson = (Position)malloc(sizeof(Person));
	if (!newPerson)
	{
		perror("error");
		return NULL;
	}
	printf("Enter name:\n");
	scanf(" %s", name);
	printf("Enter surname:\n");
	scanf(" %s", surname);
	printf("Enter birth year:\n");
	scanf(" %d", &birth_year);

	strcpy(newPerson->name, name);
	strcpy(newPerson->surname, surname);
	newPerson->birth_year = birth_year;

	return newPerson;
}
int appendList(Position Head)
{
	Position newPerson = NULL;

	newPerson = createPerson();

	if (newPerson)
	{
		newPerson->Next = Head->Next;
		Head->Next = newPerson;
	}

	return EXIT_SUCCESS;
}
Position findLast(Position Head)
{
	while (Head->Next != NULL)
	{
		Head = Head->Next;
	}
	return EXIT_SUCCESS;
}
int addToEndOfList(Position Head)
{
	Position newPerson = NULL;

	newPerson = createPerson();

	if (newPerson)
	{
		Head = findLast(Head);
		newPerson->Next = Head->Next;
		Head->Next = newPerson;
	}
	return EXIT_SUCCESS;
}
int printing(Position P)
{
	if (!P)
	{
		perror("Empty list!\n");
	}
	for (; P != NULL; P = P->Next)
	{
		printf("Name: %s\t Surname: %s\t Birth year: %d\t\n", P->name, P->surname, P->birth_year);
	}

	return EXIT_SUCCESS;
}
int menu(Position Head)
{
	char choice = '\0';
	while (1) {
		printf("Enter A(Append list), E(Put at the end of list), P(Print list), X(Exit)\n");
		scanf(" %c", &choice);
		if (tolower(choice) == 'a')
			appendList(Head);
		else if (tolower(choice) == 'e')
			addToEndOfList(Head);
		else if (tolower(choice) == 'p')
			printList(Head->Next);
		else if (tolower(choice) == 'x')
			break;
		else
			perror("Wrong letter!\n");
	}
	return EXIT_SUCCESS;
}
