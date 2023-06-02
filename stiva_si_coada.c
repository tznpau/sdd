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
	struct nodStiva* next;
} nodStiva;

typedef struct
{
	student info;
	struct nodCoada* next;
} nodCoada;

nodStiva* push(nodStiva* varf, student s)
{
	nodStiva* nou = (nodStiva*)malloc(sizeof(nodStiva));

	nou->info.id = s.id;
	nou->info.nume = (char*)malloc((strlen(s.nume) + 1) * sizeof(char));
	strcpy(nou->info.nume, s.nume);
	nou->info.medie = s.medie;

	nou->next = NULL;

	if (varf == NULL)
		varf = nou;
	else
	{
		nou->next = varf;
		varf = nou;
	}
	return varf;
}

int pop(nodStiva** varf, student* s)
{
	if (*varf == NULL)
		return -1;
	else
	{
		(*s).id = (*varf)->info.id;
		(*s).nume = (char*)malloc((strlen((*varf)->info.nume) + 1) * sizeof(char));
		strcpy((*s).nume, (*varf)->info.nume);
		(*s).medie = (*varf)->info.medie;
		
		nodStiva* temp = *varf;
		*varf = (*varf)->next;
		free(temp->info.nume);
		free(temp);
		return 0;
	}
}

void put(nodCoada** prim, nodCoada** ultim, student s)
{
	nodCoada* nou = (nodCoada*)malloc(sizeof(nodCoada));

	nou->info.id = s.id;
	nou->info.nume = (char*)malloc((strlen(s.nume) + 1) * sizeof(char));
	strcpy(nou->info.nume, s.nume);
	nou->info.medie = s.medie;
	nou->next = NULL;
	if (*prim == NULL || *ultim == NULL)
	{
		*prim = nou;
		*ultim = nou;
	}
	else
	{
		(*ultim)->next = nou;
		*ultim = nou;
	}
}

int get(nodCoada** prim, nodCoada** ultim, student* s)
{
	if (*prim != NULL && *ultim != NULL)
	{
		(*s).id = (*prim)->info.id;
		(*s).nume = (char*)malloc((strlen((*prim)->info.nume) + 1) * sizeof(char));
		strcpy((*s).nume, (*prim)->info.nume);
		(*s).medie = (*prim)->info.medie;

		nodCoada* temp = *prim;
		*prim = (*prim)->next;
		free(temp->info.nume);
		free(temp);
		return 0;
	}
	if (*prim == NULL)
	{
		*ultim = NULL;
		return -1;
	}
}

void afisareStiva(nodStiva* varf)
{
	nodStiva* temp = varf;
	while (temp)
	{
		printf("\nid student: %d, nume: %s, medie: %5.2f", temp->info.id, temp->info.nume, temp->info.medie);
		temp = temp->next;
	}
}

void afisareCoada(nodCoada* prim)
{
	nodCoada* temp = prim;
	while (temp)
	{
		printf("\nid student: %d, nume: %s, medie: %5.2f", temp->info.id, temp->info.nume, temp->info.medie);
		temp = temp->next;
	}
}

void salvareStivaCaVector(nodStiva* varf, student* vect, int* nr)
{
	student s;
	while (pop(&varf, &s) == 0)
	{
		vect[*nr] = s;
		(*nr)++;
	}
}

void salvareStivaCaCoada(nodStiva* varf, nodCoada** prim, nodCoada** ultim)
{
	student s;
	while (pop(&varf, &s) == 0)
		put(prim, ultim, s);
}

void main()
{
	int n;
	student s;
	char buffer[20];
	nodStiva* varf = NULL;
	nodCoada* prim = NULL;
	nodCoada* ultim = NULL;

	FILE* f = fopen("studenti.txt", "r");
	fscanf(f, "%d", &n);
	
	for (int i = 0; i < n; i++)
	{
		fscanf(f, "%d", &s.id);
		fscanf(f, "%s", buffer);
		s.nume = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(s.nume, buffer);
		fscanf(f, "%f", &s.medie);

		varf = push(varf, s);
		//put(&prim, &ultim, s);
		free(s.nume);
	}
	fclose(f);//

	afisareStiva(varf);
	printf("\n");

	/*printf("\nconversie stiva -> vector: \n");
	student* vect = (student*)malloc(n * sizeof(student));
	int nr = 0;
	salvareStivaCaVector(varf, vect, &nr);
	for (int i = 0; i < nr; i++)
		printf("\nid student: %d, nume: %s, medie: %5.2f", vect[i].id, vect[i].nume, vect[i].medie);
	for (int i = 0; i < nr; i++)
		free(vect[i].nume);
	free(vect);

	while (pop(&varf, &s) == 0)
		free(s.nume);*/


	printf("\nconversie stiva -> coada: \n");
	salvareStivaCaCoada(varf, &prim, &ultim);
	afisareCoada(prim);
	while (get(&prim, &ultim, &s) == 0)
		free(s.nume);
}