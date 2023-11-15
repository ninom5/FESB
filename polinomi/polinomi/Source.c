#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_LINE (1024)
#define MAX_SIZE (50)
#define FILE_NOT_OPEN (-1)
#define FAILED_MEMORY_ALLOCATION (NULL)
#define EMPTY_LISTS (-2)

struct _Element;
typedef struct _Element* Position;
typedef struct _Element
{
	int coefficient;
	int exponent;
	Position next;
}Element;
int readFile(Position headPoly1, Position headPoly2, char* fileName);
int parseStringIntoList(headPoly1, buffer);
int insertSorted(Position head, Position newElement);
int mergeAfter(Position current, Position newElement);
Position createElement(coefficient, exponent);
int insertAfter(Position current, Position newElement);
int deleteAfter(Position prev);
int printPoly(char* polynomeName, Position first);
int main()
{
	Element headPoly1 = { .coefficient = 0, .exponent = 0, .next = NULL };
	Element headPoly2 = { .coefficient = 0, .exponent = 0, .next = NULL };
	Element headPolyAdd = { .coefficient = 0, .exponent = 0, .next = NULL };
	Element headPolyMultiply = { .coefficient = 0, .exponent = 0, .next = NULL };

	char* fileName = "polynomes.txt";

	if (readFile(&headPoly1, &headPoly2, fileName) == EXIT_SUCCESS)
	{
		printPoly("First polynome: ", headPoly1.next);



	}

	return EXIT_SUCCESS;
}
int readFile(Position headPoly1, Position headPoly2, char* fileName)
{
	FILE* filePointer = NULL;

	char buffer[MAX_LINE] = { 0 };
	int status = EXIT_SUCCESS;

	filePointer = fopen(fileName, "r");
	if (!filePointer)
	{
		printf("\033[31mCant open file\033[0m\n");
		return FILE_NOT_OPEN;
	}
	
	fgets(buffer, MAX_LINE, filePointer);
	status = parseStringIntoList(headPoly1, buffer);
	if (status != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}

	fgets(buffer, MAX_LINE, filePointer);
	status = parseStringIntoList(headPoly2, buffer);
	if (status != EXIT_SUCCESS)
	{
		return EXIT_FAILURE;
	}

	fclose(filePointer);

	return EXIT_SUCCESS;
}
int parseStringIntoList(Position head, char* buffer)
{
	char* currentBuffer = buffer;
	int coefficient = 0;
	int exponent = 0;
	int numBytes = 0;
	int status = 0;
	Position newElement = NULL;

	while (strlen(currentBuffer) > 0)
	{
		status = sscanf(currentBuffer, " %dx^%d %n", &coefficient, &exponent, &numBytes);
		if (status != 2)
		{
			printf("File not good\n");
			return EXIT_FAILURE;
		}
		newElement = createElement(coefficient, exponent);
		if (!newElement)
			return EXIT_FAILURE;

		insertSorted(head, newElement);

		currentBuffer += numBytes;
	}
	
	return EXIT_SUCCESS;
}
Position createElement(coefficient, exponent)
{
	Position element = NULL;

	element = (Position)malloc(sizeof(Element));

	if (!element)
	{
		printf("Can t allocate memory\n");
		return FAILED_MEMORY_ALLOCATION;
	}

	element->coefficient = coefficient;
	element->exponent = exponent;
	element->next = NULL;

	return element;
}
int insertSorted(Position head, Position newElement)
{
	Position current = head;

	while (current->next != NULL && current->next->exponent > newElement->exponent)
		current = current->next;

	mergeAfter(current, newElement);

	return EXIT_SUCCESS;
}
int mergeAfter(Position current, Position newElement)
{
	if (current->next == NULL || current->next->exponent != newElement->exponent)
	{
		insertAfter(current, newElement);
	}
	else {
		int resultCoefficient = current->next->coefficient + newElement->coefficient;
		if (resultCoefficient == 0)
		{
			deleteAfter(current);
		}
		else
		{
			current->next->coefficient = resultCoefficient;
		}
		free(newElement);
	}
	return EXIT_SUCCESS;
}
int insertAfter(Position current, Position newElement)
{
	newElement->next = current->next;
	current->next = newElement;

	return EXIT_SUCCESS;
}
int deleteAfter(Position prev)
{
	Position toDelete = NULL;

	toDelete = prev->next;
	prev->next = toDelete->next;
	free(toDelete);

	return EXIT_SUCCESS;
}
int printPoly(char* polynomeName, Position first)
{
	printf(" %s = ", polynomeName);

	if (first)
	{
		if (first->exponent < 0)
		{
			if (first->coefficient == 1)
			{
				printf("x^(%d)", first->exponent);
			}
			else
			{
				printf("%dx^(%d)", first->coefficient, first->exponent);
			}
		}
		else
		{
			if (first->coefficient == 1)
			{
				printf("x^%d", first->exponent);
			}
			else
			{
				printf("%dx^%d", first->coefficient, first->exponent);
			}
		}
		first = first->next;
	}
	for (; first != NULL; first = first->next)
	{
		if (first->coefficient < 0)
		{
			if (first->exponent < 0)
			{
				printf(" -%dx^(-%d) ", abs(first->coefficient), abs(first->exponent));
			}
			else
			{
				printf(" -%dx^%d ", abs(first->coefficient), first->exponent);
			}
		}
		else
		{
			if (first->exponent < 0)
			{
				if (first->coefficient == 1) {
					printf(" + x^(%d)", first->exponent);
				}
				else {
					printf(" + %dx^(%d)", first->coefficient, first->exponent);
				}
			}
			else
			{
				if (first->coefficient == 1) {
					printf(" + x^%d", first->exponent);
				}
				else {
					printf(" + %dx^%d", first->coefficient, first->exponent);
				}
			}
		}
	}
	printf("\n");

	return EXIT_SUCCESS;
}


























