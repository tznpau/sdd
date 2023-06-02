#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>

typedef struct
{
	int id;
	char* nume;
	float medie;
} student;

typedef struct
{
	student info;
	struct nodLS* next;
} nodLS;

nodLS* inserareLS(nodLS* cap, student s)
{
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));

	nou->info.id = s.id;
	nou->info.nume = (char*)malloc((strlen(s.nume) + 1) * sizeof(char));
	strcpy(nou->info.nume, s.nume);
	nou->info.medie = s.medie;

	nou->next = NULL;

	if (cap == NULL)
		cap = nou;
	else
	{
		nodLS* temp = cap;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = nou;
	}
	return cap;
}

void traversareLS(nodLS* cap)
{
	nodLS* temp = cap;
	while (temp)
	{
		printf("\nid student: %d, nume: %s, medie: %5.2f", temp->info.id, temp->info.nume, temp->info.medie);
		temp = temp->next;
	}
}

void dezalocareLS(nodLS* cap)
{
	nodLS* temp = cap;
	while (temp)
	{
		nodLS* temp2 = temp->next;
		free(temp->info.nume);
		free(temp);
		temp = temp2;
	}
}

void main()
{
	int n;

	FILE* f = fopen("studenti.txt", "r");
	fscanf(f, "%d", &n);

	student s;
	char buffer[20];
	nodLS* cap = NULL;

	for (int i = 0; i < n; i++)
	{
		fscanf(f, "%d", &s.id);
		fscanf(f, "%s", buffer);
		s.nume = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(s.nume, buffer);
		fscanf(f, "%f", &s.medie);

		cap = inserareLS(cap, s);
		free(s.nume);
	}
	fclose(f);

	traversareLS(cap);
	dezalocareLS(cap);
}