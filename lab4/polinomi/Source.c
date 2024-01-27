#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE (1024)
#define MAX_SIZE (50)
#define FILE_NOT_OPEN (-1)
#define FAILED_MEMORY_ALLOCATION (NULL)
#define EMPTY_LISTS (-2)

struct _Element;
typedef struct _Element* Position;
typedef struct _Element {
    int coefficient;
    int exponent;
    Position next;
}Element;

int readFile(Position HeadPoly1, Position HeadPoly2, char* fileName1);
int scanFile(Position HeadPoly1, FILE* filePtr, char* buff);
int addNewElement(Position HeadPoly1, int coef, int expon);
Position createElement(int coef, int expon);
int sortPoly(Position HeadPoly, Position newElement);
int toDelete(Position element);
int printPoly(char* name, Position HeadPoly);
int AddPoly(Position HeadPoly1, Position HeadPoly2, Position HeadPolyAdd);
Position helpSort(Position HeadPolyAdd, int expon);
int multiplyPoly(Position resultHead, Position firstElementPoly1, Position firstElementPoly2);

int main(void) {
    Element HeadPoly1 = { .coefficient = 0, .exponent = 0, .next = NULL };
    Element HeadPoly2 = { .coefficient = 0, .exponent = 0, .next = NULL };
    Element HeadPolyAdd = { .coefficient = 0, .exponent = 0, .next = NULL };
    Element HeadPolyMultiply = { .coefficient = 0, .exponent = 0, .next = NULL };
    char* fileName = "/Users/mosa21/programiranje/Strukture-podataka/vjezba4/vjezba4/polynomes.txt";

    if (readFile(&HeadPoly1, &HeadPoly2, fileName) == EXIT_SUCCESS)
    {
        printPoly("First polynome: ", HeadPoly1.next);
        printPoly("Second polynome: ", HeadPoly2.next);

    }
    AddPoly(&HeadPoly1, &HeadPoly2, &HeadPolyAdd);
    printPoly("Added poly: ", HeadPolyAdd.next);
    multiplyPoly(&HeadPolyMultiply, HeadPoly1.next, HeadPoly2.next);
    printPoly("Multiplied polynome: ", HeadPolyMultiply.next);
}

int readFile(Position HeadPoly1, Position HeadPoly2, char* fileName)
{
    FILE* filePtr = NULL;
    char buffer[MAX_LINE];
    filePtr = fopen(fileName, "r");
    if (!filePtr)
    {
        printf("Failed to read file\n");
        return EXIT_FAILURE;
    }
    fgets(buffer, MAX_LINE, filePtr);
    scanFile(HeadPoly1, filePtr, buffer);

    fgets(buffer, MAX_LINE, filePtr);
    scanFile(HeadPoly2, filePtr, buffer);
    return EXIT_SUCCESS;
}

int scanFile(Position HeadPoly1, FILE* filePtr, char* buff)
{
    char* buffer = buff;


    while (strlen(buffer) > 0)
    {
        int coef = 0;
        int expon = 0;
        int numBytes = 0;
        sscanf(buffer, " %dx^%d %n", &coef, &expon, &numBytes);

        buffer += numBytes;
        if (coef != 0)
        {
            addNewElement(HeadPoly1, coef, expon);
        }

    }
    return EXIT_SUCCESS;
}

int addNewElement(Position HeadPoly, int coef, int expon)
{
    Position newElement = NULL;
    while (HeadPoly->next && HeadPoly->next->exponent > expon)
    {
        HeadPoly = HeadPoly->next;
    }

    newElement = createElement(coef, expon);

    sortPoly(HeadPoly, newElement);

    return EXIT_SUCCESS;
}

Position createElement(int coef, int expon)
{
    Position newElement = NULL;

    newElement = (Position)malloc(sizeof(Element));
    if (!newElement)
    {
        printf("Failed to allocate memory\n");
        return FAILED_MEMORY_ALLOCATION;
    }

    newElement->coefficient = coef;
    newElement->exponent = expon;
    newElement->next = NULL;

    return newElement;
}
int sortPoly(Position HeadPoly, Position newElement)
{
    if (!HeadPoly->next || HeadPoly->next->exponent > newElement->exponent)
    {
        newElement->next = HeadPoly->next;
        HeadPoly->next = newElement;
    }
    else
    {
        if (HeadPoly->next->exponent == newElement->exponent)
        {
            int resultCoef = HeadPoly->next->coefficient + newElement->coefficient;
            if (resultCoef == 0)
            {
                toDelete(HeadPoly);
            }
            else
            {
                HeadPoly->next->coefficient = resultCoef;
            }
            free(newElement);
        }
        else
        {
            newElement->next = HeadPoly->next;
            HeadPoly->next = newElement;
        }
    }

    return EXIT_SUCCESS;
}

int toDelete(Position HeadPoly)
{
    Position toDelete = NULL;

    toDelete = HeadPoly->next;
    HeadPoly->next = toDelete->next;
    free(toDelete);

    return EXIT_SUCCESS;
}

int printPoly(char* name, Position HeadPoly)
{
    printf("%s", name);
    if (HeadPoly)
    {
        if (HeadPoly->exponent < 0)
        {
            if (HeadPoly->coefficient == 1)
            {
                printf("x^(%d)", HeadPoly->exponent);
            }
            else
            {
                printf("%dx^(%d)", HeadPoly->coefficient, HeadPoly->exponent);
            }
        }
        else
        {
            if (HeadPoly->coefficient == 1)
            {
                printf("x^%d", HeadPoly->exponent);
            }
            else
            {
                printf("%dx^%d", HeadPoly->coefficient, HeadPoly->exponent);
            }
        }
        HeadPoly = HeadPoly->next;
    }

    for (; HeadPoly != NULL; HeadPoly = HeadPoly->next)
    {

        if (HeadPoly->coefficient < 0)
        {
            if (HeadPoly->exponent < 0)
            {
                printf(" - %dx^(%d)", abs(HeadPoly->coefficient), HeadPoly->exponent);
            }
            else
            {
                printf(" - %dx^(%d)", abs(HeadPoly->coefficient), HeadPoly->exponent);
            }
        }
        else
        {
            if (HeadPoly->exponent < 0)
            {
                if (HeadPoly->coefficient == 1)
                {
                    printf(" + x^(%d)", HeadPoly->exponent);
                }
                else
                {
                    printf(" + %dx^(%d)", HeadPoly->coefficient, HeadPoly->exponent);
                }
            }
            else
            {
                if (HeadPoly->coefficient == 1)
                {
                    printf(" + x^%d", HeadPoly->exponent);
                }
                else
                    printf(" + %dx^%d", HeadPoly->coefficient, HeadPoly->exponent);
            }
        }
    }
    printf("\n");
    return EXIT_SUCCESS;
}

int AddPoly(Position HeadPoly1, Position HeadPoly2, Position HeadPolyAdd)
{
    for (; HeadPoly1 != NULL; HeadPoly1 = HeadPoly1->next)
    {

        Position newElement = createElement(HeadPoly1->coefficient, HeadPoly1->exponent);
        sortPoly(helpSort(HeadPolyAdd, HeadPoly1->exponent), newElement);
    }

    for (; HeadPoly2 != NULL; HeadPoly2 = HeadPoly2->next)
    {
        Position newElement = createElement(HeadPoly2->coefficient, HeadPoly2->exponent);
        sortPoly(helpSort(HeadPolyAdd, HeadPoly2->exponent), newElement);
    }

    return EXIT_SUCCESS;
}

Position helpSort(Position HeadPolyAdd, int expon)
{
    while (HeadPolyAdd->next && HeadPolyAdd->next->exponent > expon)
    {
        HeadPolyAdd = HeadPolyAdd->next;
    }
    return HeadPolyAdd;
}

int multiplyPoly(Position resultHead, Position firstElementPoly1, Position firstElementPoly2)
{
    if (firstElementPoly1 == NULL || firstElementPoly2 == NULL)
        return EMPTY_LISTS;
    for (Position currentPoly1 = firstElementPoly1; currentPoly1 != NULL; currentPoly1 = currentPoly1->next) {
        for (Position currentPoly2 = firstElementPoly2; currentPoly2 != NULL; currentPoly2 = currentPoly2->next) {
            Position newElement = createElement(currentPoly1->coefficient * currentPoly2->coefficient, currentPoly1->exponent + currentPoly2->exponent);
            if (!newElement) {
                return EXIT_FAILURE;
            }

            sortPoly(helpSort(resultHead, currentPoly1->exponent + currentPoly2->exponent), newElement);
        }
    }
    return EXIT_SUCCESS;

}