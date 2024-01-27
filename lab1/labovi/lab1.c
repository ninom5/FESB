#define _CRT_SECURE_NO_WARNINGS
#define N 50
#include<stdio.h>
#include<stdlib.h>
typedef struct
{
	char name[N];
	char surname[N];
	double points;
}student;
int main()
{
	int noRows = 0;
	int maxPoints = 70;
	int i;
	float relatPoints = 0;

	student* stud;

	FILE* p;

	p = fopen("vj1.txt", "r");

	if (p == NULL)
	{
		printf("error");
		exit(1);
	}

	while (!feof(p))
	{
		if (fgetc(p) == '\n')
			noRows++;
	}
	
	stud = (student*)malloc(noRows * sizeof(student));

	rewind(p);
	
	for (i = 0; i <= noRows; i++)
	{
		fscanf(p, "%s %s %lf", stud[i].name, stud[i].surname, &stud[i].points);
	}
	puts("\n");
	for (i = 0; i < noRows; i++)
	{
		relatPoints = stud[i].points / maxPoints * 100;
		printf("%s   %s    %lf\t %f", stud[i].name, stud[i].surname, stud[i].points, relatPoints);
		printf("\n");
	}

	fclose(p);

	return 0;
}