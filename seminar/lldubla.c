#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WAARINGS
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
	struct nodLD* next, *prev;
} nodLD;

nodLD* inserareLD(nodLD* cap, nodLD** coada, student s)
{
	nodLD* nou = (nodLD*)malloc(sizeof(nodLD));

	nou->info.id = s.id;
	nou->info.nume = (char*)malloc((strlen(s.nume) + 1) * sizeof(char));
	strcpy(nou->info.nume, s.nume);
	nou->info.medie = s.medie;

	nou->next = NULL;
	nou->prev = NULL;

	if (cap == NULL)
	{
		cap = nou;
		*coada = nou;
	}
	else
	{
		nodLD* temp = cap;
		while (temp->next != NULL)
			temp = temp->next;
		temp->next = nou;
		nou->prev = temp;
		*coada = nou;
	}
	return cap;
}

void traversareLD(nodLD* cap)
{
	nodLD* temp = cap;
	while (temp)
	{
		printf("\nid student: %d, nume: %s, medie: %5.2f", temp->info.id, temp->info.nume, temp->info.medie);
		temp = temp->next;
	}
}

void traversareLD_invers(nodLD* coada)
{
	nodLD* temp = coada;
	while (temp)
	{
		printf("\nid student: %d, nume: %s, medie: %5.2f", temp->info.id, temp->info.nume, temp->info.medie);
		temp = temp->prev;
	}
}

void dezalocareLD(nodLD* cap)
{
	nodLD* temp = cap;
	while (temp)
	{
		nodLD* temp2 = temp->next;
		free(temp->info.nume);
		free(temp);
		temp = temp2;
	}
}

void stergereNod(nodLD** cap, nodLD** coada, nodLD* sters)
{
	if (*cap == NULL || *coada == NULL || sters == NULL)
		return;
	if (*cap == sters)
	{
		*cap = (*cap)->next;
		if (*cap != NULL)
			(*cap)->prev = NULL;
		free(sters->info.nume);
		free(sters);
		return;
	}
	if (*coada == sters)
	{
		*coada = (*coada)->prev;
		(*coada)->next = NULL;
		free(sters->info.nume);
		free(sters);
		return;
	}
	nodLD* urmator = sters->next;
	nodLD* anterior = sters->prev;
	anterior->next = urmator;
	urmator->prev = anterior;
	free(sters->info.nume);
	free(sters);
	return;
}

void stergereDupaNume(nodLD** cap, nodLD** coada, char* nume)
{
	nodLD* temp = *cap;
	while (temp)
	{
		if (strcmp(nume, temp->info.nume) == 0)
		{
			nodLD* sters = temp;
			temp = temp->next;
			stergereNod(cap, coada, sters);
		}
		else
		{
			temp = temp->next;
		}
	}
}

void salvareVectorStudentiMedie(nodLD* cap, student* vect, int* nr)
{
	nodLD* temp = cap;
	while (temp)
	{
		if (temp->info.medie >= 8)
		{
			vect[*nr].id = temp->info.id;
			vect[*nr].nume = (char*)malloc((strlen(temp->info.nume) + 1) * sizeof(char));
			strcpy(vect[*nr].nume, temp->info.nume);
			vect[*nr].medie = temp->info.medie;
			(*nr)++;
		}
		temp = temp->next;
	}
}

void main()
{
	int n;

	FILE* f = fopen("studenti.txt", "r");
	fscanf(f, "%d", &n);

	student s;
	char buffer[20];
	nodLD* cap = NULL;
	nodLD* coada = NULL;

	for (int i = 0; i < n; i++)
	{
		fscanf(f, "%d", &s.id);
		fscanf(f, "%s", buffer);
		s.nume = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(s.nume, buffer);
		fscanf(f, "%f", &s.medie);

		cap = inserareLD(cap, &coada, s);
		free(s.nume);
	}
	fclose(f);

	traversareLD(cap);
	printf("\n");
	traversareLD_invers(coada);


	//----------------------------

	printf("\n\nsalvate ca vector: ");
	student* vect = (student*)malloc(n * sizeof(student));
	int nr = 0;
	salvareVectorStudentiMedie(cap, vect, &nr);
	for (int i = 0; i < nr; i++)
		printf("\nid student: %d, nume: %s, medie: %5.2f", vect[i].id, vect[i].nume, vect[i].medie);
	for (int i = 0; i < nr; i++)
		free(vect[i].nume);
	free(vect);
	//----------------------------

	printf("\n\nstergere: ");
	stergereDupaNume(&cap, &coada, "Andreea");
	traversareLD(cap);
	printf("\n");
	traversareLD_invers(coada);

	dezalocareLD(cap);
}