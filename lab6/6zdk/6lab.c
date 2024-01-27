#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATE_LENGTH (11)
#define MAX_LENGTH (50)
#define MAX_LINE (1024)
#define FILE_NOT_OPEN (-1)
#define FAILED_MEMORY_ALLOCATION (NULL)
#define DATE2_LATER (-1)
#define DATE2_EARLIER (1)
#define DATES_EQUAL (0)

struct _article;
typedef struct _article* ArticlePosition;
typedef struct _article
{
	char name[MAX_LENGTH];
	int quantity;
	double price;
	ArticlePosition next;
} Article;

struct _receipt;
typedef struct _receipt* ReceiptPosition;
typedef struct _receipt
{
	char receiptDate[DATE_LENGTH];
	ReceiptPosition next;
	Article articleHead;
} Receipt;

int readAndFillReceipts(ReceiptPosition receiptHead);
ReceiptPosition createNewRecipeFromBuffer(char* buffer);
ReceiptPosition createNewRecipe(char* date);
ArticlePosition createNewArticle(char* name, int quantity, double price);
int insertSortedNewArticle(ArticlePosition articleHead, ArticlePosition newArticle, int isNew);
int mergeArticleAfter(ArticlePosition articleCurrent, ArticlePosition newArticle, int isNew);
int insertArticleAfter(ArticlePosition articleCurrent, ArticlePosition newArticle);
int insertSortedNewReceipt(ReceiptPosition receiptHead, ReceiptPosition newReceipt);
int mergeAfter(ReceiptPosition receiptCurrent, ReceiptPosition newReceipt);
int insertAfter(ReceiptPosition receiptCurrent, ReceiptPosition newReceipt);
int compareDates(char* date1, char* date2);
int articleInBetweenDates(char* articleName, char* dateFrom, char* dateTo, ReceiptPosition receiptHead);

int main() {
	Receipt receiptHead = {
		.receiptDate = { 0 },
		.next = NULL,
		.articleHead = {
			.name = { 0 },
			.quantity = 0,
			.price = 0,
			.next = NULL
		}
	};

	if (readAndFillReceipts(&receiptHead) == EXIT_SUCCESS) {
		ReceiptPosition currentReceipt = NULL;
		currentReceipt = receiptHead.next;
		ArticlePosition currentArticle = NULL;
		while (currentReceipt != NULL) {
			printf("\n%s\n", currentReceipt->receiptDate);
			currentArticle = currentReceipt->articleHead.next;
			while (currentArticle != NULL) {
				printf("\t%s %d %.2lf\n", currentArticle->name, currentArticle->quantity, currentArticle->price);
				currentArticle = currentArticle->next;
			}
			currentReceipt = currentReceipt->next;
		}
	}

	articleInBetweenDates("Jabuka", "2023-11-10", "2023-11-17", receiptHead.next);

	return EXIT_SUCCESS;
}

int readAndFillReceipts(ReceiptPosition receiptHead) {
	FILE* filePointer = NULL;
	char buffer[MAX_LINE] = { 0 };
	char nullString[MAX_LINE] = { 0 };
	ReceiptPosition newReceipt = NULL;

	filePointer = fopen("racuni.txt", "r");
	if (!filePointer) {
		printf("\033[31mCan't open file!\033[0m\n");
		return FILE_NOT_OPEN;
	}

	while (!feof(filePointer)) {
		fscanf(filePointer, " %s", buffer);
		newReceipt = createNewRecipeFromBuffer(buffer);
		insertSortedNewReceipt(receiptHead, newReceipt);
		strcpy(buffer, nullString);
	}

	fclose(filePointer);

	return EXIT_SUCCESS;
}

ReceiptPosition createNewRecipeFromBuffer(char* buffer) {
	char receiptLocation[MAX_LENGTH] = { 0 };
	FILE* receiptFilePointer = NULL;
	ReceiptPosition newReceipt = NULL;
	ArticlePosition newArticle = NULL;
	char dateBuffer[DATE_LENGTH] = { 0 };
	char articleName[MAX_LENGTH] = { 0 };
	int articleQuantity = 0;
	double articlePrice = 0;

	strcpy(receiptLocation, buffer);

	receiptFilePointer = fopen(receiptLocation, "r");
	if (!receiptFilePointer) {
		printf("\033[31mCan't open file!\033[0m\n");
		return FILE_NOT_OPEN;
	}

	fgets(dateBuffer, DATE_LENGTH, receiptFilePointer);
	newReceipt = createNewRecipe(dateBuffer);
	while (!feof(receiptFilePointer)) {
		fscanf(receiptFilePointer, " %s %d %lf", articleName, &articleQuantity, &articlePrice);
		newArticle = createNewArticle(articleName, articleQuantity, articlePrice);
		insertSortedNewArticle(&newReceipt->articleHead, newArticle, 1);
		newArticle = NULL;
	}

	fclose(receiptFilePointer);

	return newReceipt;
}

ReceiptPosition createNewRecipe(char* date) {
	ReceiptPosition receipt = NULL;
	receipt = (ReceiptPosition)malloc(sizeof(Receipt));
	if (!receipt) {
		printf("Can't allocate memory!\n");
		return FAILED_MEMORY_ALLOCATION;
	}


	strcpy(receipt->receiptDate, date);
	receipt->next = NULL;
	receipt->articleHead.next = NULL;

	return receipt;
}

ArticlePosition createNewArticle(char* name, int quantity, double price) {
	ArticlePosition article = NULL;
	article = (ArticlePosition)malloc(sizeof(Article));
	if (!article) {
		printf("Can't allocate memory!\n");
		return FAILED_MEMORY_ALLOCATION;
	}


	strcpy(article->name, name);
	article->quantity = quantity;
	article->price = price;
	article->next = NULL;

	return article;
}

int insertSortedNewArticle(ArticlePosition articleHead, ArticlePosition newArticle, int isNew) {
	ArticlePosition articleCurrent = articleHead;
	while (articleCurrent->next != NULL && strcmp(articleCurrent->next->name, newArticle->name) < 0) {
		articleCurrent = articleCurrent->next;
	}

	mergeArticleAfter(articleCurrent, newArticle, isNew);

	return EXIT_SUCCESS;
}

int mergeArticleAfter(ArticlePosition articleCurrent, ArticlePosition newArticle, int isNew) {
	int resultArticleQuantity = 0;
	ArticlePosition newArticle2 = newArticle;

	if (!isNew) {
		newArticle2 = createNewArticle(newArticle2->name, newArticle2->quantity, newArticle2->price);
	}

	if (articleCurrent->next == NULL || strcmp(articleCurrent->next->name, newArticle->name) != 0) {
		insertArticleAfter(articleCurrent, newArticle2);
	}
	else {
		resultArticleQuantity = articleCurrent->next->quantity + newArticle->quantity;
		articleCurrent->next->quantity = resultArticleQuantity;
		if (isNew)
			free(newArticle2);
	}

	return EXIT_SUCCESS;
}

int insertArticleAfter(ArticlePosition articleCurrent, ArticlePosition newArticle) {
	newArticle->next = articleCurrent->next;
	articleCurrent->next = newArticle;

	return EXIT_SUCCESS;
}

int insertSortedNewReceipt(ReceiptPosition receiptHead, ReceiptPosition newReceipt) {
	ReceiptPosition receiptCurrent = receiptHead;

	while (receiptCurrent->next != NULL && compareDates(receiptCurrent->next->receiptDate, newReceipt->receiptDate) == DATE2_LATER  /* < */) {
		receiptCurrent = receiptCurrent->next;
	}

	mergeAfter(receiptCurrent, newReceipt);

	return EXIT_SUCCESS;
}

int mergeAfter(ReceiptPosition receiptCurrent, ReceiptPosition newReceipt) {
	ArticlePosition articleCurrent = NULL;
	if (receiptCurrent->next == NULL || compareDates(receiptCurrent->next->receiptDate, newReceipt->receiptDate) == DATE2_EARLIER) {
		insertAfter(receiptCurrent, newReceipt);
	}
	else {
		articleCurrent = newReceipt->articleHead.next;
		while (articleCurrent != NULL) {
			insertSortedNewArticle(&receiptCurrent->next->articleHead, articleCurrent, 0);
			articleCurrent = articleCurrent->next;
		}
		free(newReceipt);
	}

	return EXIT_SUCCESS;
}

int insertAfter(ReceiptPosition receiptCurrent, ReceiptPosition newReceipt) {
	newReceipt->next = receiptCurrent->next;
	receiptCurrent->next = newReceipt;

	return EXIT_SUCCESS;
}
int compareDates(char* date1, char* date2) {
	int year1, month1, day1;
	int year2, month2, day2;

	sscanf(date1, "%d-%d-%d", &year1, &month1, &day1);
	sscanf(date2, "%d-%d-%d", &year2, &month2, &day2);

	if (year1 < year2) {
		return DATE2_LATER;
	}
	else if (year1 > year2) {
		return DATE2_EARLIER;
	}


	if (month1 < month2) {
		return DATE2_LATER;
	}
	else if (month1 > month2) {
		return DATE2_EARLIER;
	}

	if (day1 < day2) {
		return DATE2_LATER;
	}
	else if (day1 > day2) {
		return DATE2_EARLIER;
	}


	return DATES_EQUAL;
}

int articleInBetweenDates(char* articleName, char* dateFrom, char* dateTo, ReceiptPosition receiptFirstElement) {
	ReceiptPosition receiptFrom = NULL;
	ReceiptPosition receiptTo = NULL;
	ReceiptPosition receiptCurrent = NULL;
	ArticlePosition articleCurrent = NULL;
	int articleQuantity = 0;
	double articlePrice = 0;

	receiptCurrent = receiptFirstElement;
	while (receiptCurrent != NULL) {
		if (compareDates(dateFrom, receiptCurrent) != DATE2_EARLIER && receiptFrom == NULL)
			receiptFrom = receiptCurrent;
		if ((compareDates(dateTo, receiptCurrent) != DATE2_LATER && receiptTo == NULL) || receiptCurrent->next == NULL)
			receiptTo = receiptCurrent;
		receiptCurrent = receiptCurrent->next;
	}


	receiptCurrent = receiptFrom;
	while (receiptCurrent != NULL && compareDates(receiptCurrent->receiptDate, receiptTo->receiptDate) != DATE2_EARLIER) {
		articleCurrent = receiptCurrent->articleHead.next;
		while (articleCurrent != NULL && strcmp(articleCurrent->name, articleName) < 0) {
			articleCurrent = articleCurrent->next;
		}
		if (articleCurrent != NULL && strcmp(articleCurrent->name, articleName) == 0) {
			articleQuantity += articleCurrent->quantity;
			articlePrice += articleCurrent->price * (double)articleCurrent->quantity;
		}
		receiptCurrent = receiptCurrent->next;
	}

	printf("\n %d %.2lf", articleQuantity, articlePrice);
}