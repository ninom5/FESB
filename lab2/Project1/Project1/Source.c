#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_LENGTH (50)
struct Person;
typedef struct Person* Position;
typedef struct Person
{
	char name[MAX_LENGTH];
	char surname[MAX_LENGTH];
	int birthYear;
	Position next;
}person;
int addToBeginning(Position head);
int printList(Position head);
int addToEnd(Position head);
int menu(Position head);
int findSurname(Position head);
int printPerson(Position person);
int deletePerson(Position head);
Position createPerson();
Position findLast(Position head);
int main()
{
	person head = { .next = NULL, .name = {0}, .surname = {0}, .birthYear = 0 };

	menu(&head);

	return EXIT_SUCCESS;
}
int addToBeginning(Position head)
{
	Position newPerson = NULL;
	newPerson = createPerson();

	if (newPerson)
	{
		newPerson->next = head->next;
		head->next = newPerson;
	}

	return EXIT_SUCCESS;
}
int addToEnd(Position head)
{
	Position newPerson = NULL;
	newPerson = createPerson;

	if (newPerson)
	{
		head = findLast(head);
		newPerson->next = head->next;
		head->next = newPerson;
	}
	return EXIT_SUCCESS;
}
int findSurname(Position head)
{
	char surname[MAX_LENGTH] = { 0 };
	printf("Enter the surname");
	scanf("%s", surname);
	if (head->next)
	{
		while (head->next != NULL && strcmp(head->next->surname, surname) != 0)
		{
			head = head->next;
		}
		if (head->next)
		{
			printPerson(head->next);
		}
		else
		{
			printf("cant find person\n");
			return -1;
		}
	}
	else
		perror("List is empty\n");

	return EXIT_SUCCESS;
}
int printPerson(Position person)
{
	printf("Name: %s\t Surname: %s\tBirth year: %d\tadress: %p\n", person->name, person->surname, person->birthYear, person);

	return EXIT_SUCCESS;
}
int deletePerson(Position head)
{
	char surname[MAX_LENGTH] = { 0 };
	printf("Enter the surname");
	scanf("%s", surname);

	if (head->next)
	{
		Position prev = NULL;
		while (head->next && strcmp(head->surname, surname) != 0)
		{
			prev = head;
			head = head->next;
		}
		if (prev->next && strcmp(head->surname, surname) == 0)
		{
			printPerson(head);
			prev->next = head->next;
			free(head);
			printf("Deleted!\n");
		}
		else
		{
			perror("Can't find person with that surname!\n");
			return -1;
		}
	}
	else
	{
		perror("Empty list!\n");
	}

	return EXIT_SUCCESS;
}
int printList(Position head)
{

	if (!head)
	{
		perror("Empty list!\n");
	}
	for (; head != NULL; head = head->next)
	{
		printf("Name: %s\t Surname: %s\t Birth year: %d\t\n", head->name, head->surname, head->birthYear);
	}

	return EXIT_SUCCESS;


	return;
}
Position createPerson()
{
	Position newPerson = NULL;
	char name[MAX_LENGTH] = { 0 };
	char surname[MAX_LENGTH] = { 0 };
	int birthYear = 0;

	newPerson = (Position)malloc(sizeof(person));
	if (!newPerson)                              //provjera jeli alociranje uspjesno
	{
		perror("cant allocate memory");
		return NULL;
	}
	printf("Enter name:\n");
	scanf("%s", name);
	printf("Enter surname:\n");
	scanf("%s", surname);
	printf("Enter birth year:\n");
	scanf("%d", &birthYear);

	strcpy(newPerson->name, name);
	strcpy(newPerson->surname, surname);
	newPerson->birthYear = birthYear;

	return newPerson;
}
Position findLast(Position head)
{
	while (head->next != NULL)
	{
		head = head->next;
	}
	return head;
}
int menu(Position head)
{
	char choice;

	printf("Enter: A to add on the beginning, E to add on the end of the list ");
	scanf("%c", choice);

	char choice = '\0';
	while (1) {
		printf("Enter A(Append list), E(Put at the end of list), P(Print list), S(Search), D(Delete), X(Exit)\n");
		scanf(" %c", &choice);
		if (tolower(choice) == 'a')
			addToBeginning(head);
		else if (tolower(choice) == 'e')
			addToEnd(head);
		else if (tolower(choice) == 'p')
			printList(head->next);
		else if (tolower(choice) == 's')
		{
			while (findPerson(head) == -1)
			{
				findSurname(head);
			}
		}
		else if (tolower(choice) == 'd')
		{
			while (deletePerson(head) == -1)
			{
				deletePerson(head);
			}
		}
		else if (tolower(choice) == 'x')
			break;
		else
			perror("Wrong letter!\n");
	}
	return EXIT_SUCCESS;
}