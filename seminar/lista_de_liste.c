#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>

typedef struct
{
	int id;
	char* nume;
	char* forma;
} student;

typedef struct
{
	student info;
	struct nodLS* next;
} nodLS;

typedef struct
{
	struct nodLS* info;
	struct nodLP* next;
} nodLP;

void inserareLS(nodLS** capLS, student s)
{
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));

	nou->info.id = s.id;
	nou->info.nume = (char*)malloc((strlen(s.nume) + 1) * sizeof(char));
	strcpy(nou->info.nume, s.nume);
	nou->info.forma = (char*)malloc((strlen(s.forma) + 1) * sizeof(char));
	strcpy(nou->info.forma, s.forma);

	nou->next = NULL;

	if (*capLS == NULL)
		*capLS = nou;
	else
	{
		nodLS* temp = *capLS;
		while (temp->next)
			temp = temp->next;
		temp->next = nou;
	}
}

void inserareLP(nodLP** capLP, nodLS* capLS)
{
	nodLP* nou = (nodLP*)malloc(sizeof(nodLP));

	nou->info = capLS;
	nou->next = NULL;

	if (*capLP == NULL)
		*capLP = nou;
	else
	{
		nodLP* temp = *capLP;
		while (temp->next)
			temp = temp->next;
		temp->next = nou;
	}
}

void traversareLS(nodLS* capLS)
{
	nodLS* temp = capLS;
	while (temp)
	{
		printf("\nid student: %d, nume: %s, forma invatamant: %s", temp->info.id, temp->info.nume, temp->info.forma);
		temp = temp->next;
	}
}

void traversareLP(nodLP* capLP)
{
	nodLP* temp = capLP;
	int i = 1;
	while (temp)
	{
		printf("\nsublista: %d", i);
		traversareLS(temp->info);
		temp = temp->next;
		i++;
	}
}

void dezalocareLS(nodLS* capLS)
{
	nodLS* temp = capLS;
	while (temp)
	{
		nodLS* temp2 = temp->next;
		free(temp->info.nume);
		free(temp->info.forma);
		free(temp);
		temp = temp2;
	}
}

void dezalocareLP(nodLP* capLP)
{
	nodLP* temp = capLP;
	while (temp)
	{
		nodLP* temp2 = temp->next;
		dezalocareLS(temp->info);
		free(temp);
		temp = temp2;
	}
}

void main()
{
	int n;
	char buffer[20];
	nodLP* capLP = NULL;
	nodLS* capLS_IF = NULL;
	nodLS* capLS_ID = NULL;
	student s;

	FILE* f = fopen("studenti_hash.txt", "r");
	fscanf(f, "%d", &n);

	for (int i = 0; i < n; i++)
	{
		fscanf(f, "%d", &s.id);
		fscanf(f, "%s", buffer);
		s.nume = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(s.nume, buffer);
		fscanf(f, "%s", buffer);
		s.forma = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(s.forma, buffer);

		if (strcmp(s.forma, "IF") == 0)
			inserareLS(&capLS_IF, s);
		else
			inserareLS(&capLS_ID, s);

		free(s.nume);
		free(s.forma);
	}
	fclose(f);

	inserareLP(&capLP, capLS_IF);
	inserareLP(&capLP, capLS_ID);

	traversareLP(capLP);
	dezalocareLP(capLP);
}