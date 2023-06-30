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

typedef struct
{
	struct nodLS** vect;
	int nrElem;
} hashT;

int functieDispersie(hashT tabela, int cheie)
{
	return cheie % tabela.nrElem;
}

void inserareTabela(hashT tabela, student s)
{
	int poz = 0;
	if (tabela.vect != NULL)
	{
		poz = functieDispersie(tabela, s.id);

		nodLS* nou = (nodLS*)malloc(sizeof(nodLS));

		nou->info.id = s.id;
		nou->info.nume = (char*)malloc((strlen(s.nume) + 1) * sizeof(char));
		strcpy(nou->info.nume, s.nume);
		nou->info.medie = s.medie;

		nou->next = NULL;

		if (tabela.vect[poz] == NULL)
			tabela.vect[poz] = nou;
		else
		{
			nodLS* temp = tabela.vect[poz];
			while (temp->next)
				temp = temp->next;
			temp->next = nou;
		}
	}
	return poz;
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

void traversareTabela(hashT tabela)
{
	if (tabela.vect != NULL)
	{
		for (int i = 0; i < tabela.nrElem; i++)
			if (tabela.vect[i] != NULL)
			{
				printf("\npozitie: %d", i);
				traversareLS(tabela.vect[i]);
			}
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

void dezalocareTabela(hashT tabela)
{
	if (tabela.vect != NULL)
	{
		for (int i = 0; i < tabela.nrElem; i++)
			if (tabela.vect[i] != NULL)
			{
				dezalocareLS(tabela.vect[i]);
			}
		free(tabela.vect);
	}
}

void stergeNod(nodLS* nod)
{
	free(nod->info.nume);
	free(nod);
}

int stergere(hashT tabela, int id)
{
	int poz = 0;
	if (tabela.vect != NULL)
	{
		poz = functieDispersie(tabela, id);
		nodLS* temp = tabela.vect[poz];
		if (temp == NULL)
			return -1;
		else
		{
			if (temp->info.id == id)
			{
				if (temp->next == NULL)
				{
					stergeNod(temp);
					tabela.vect[poz] = NULL;
				}
				else
				{
					nodLS* temp2 = temp;
					tabela.vect[poz] = temp->next;
					stergeNod(temp2);
				}
			}
			else
			{
				while (temp->next != NULL)
				{
					nodLS* temp2 = temp->next;
					if (temp2->info.id != id)
						temp = temp->next;
					else
						break;
				}
				nodLS* temp3 = temp->next;
				if (temp3 != NULL)
				{
					if (temp3->next != NULL)
					{
						temp->next = temp3->next;
						stergeNod(temp3);
					}
					else
					{
						temp->next = NULL;
						stergeNod(temp3);
					}
				}
				else
					return -1;
			}
		}
	}
	return poz;
}

void main()
{
	hashT tabela;
	tabela.nrElem = 31;
	tabela.vect = (nodLS**)malloc(tabela.nrElem * sizeof(nodLS*));
	for (int i = 0; i < tabela.nrElem; i++)
		tabela.vect[i] = NULL;

	int n;
	student s;
	char buffer[20];

	FILE* f = fopen("studenti_hash2.txt", "r");
	fscanf(f, "%d", &n);
	for (int i = 0; i < n; i++)
	{
		fscanf(f, "%d", &s.id);
		fscanf(f, "%s", buffer);
		s.nume = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(s.nume, buffer);
		fscanf(f, "%f", &s.medie);

		inserareTabela(tabela, s);

		free(s.nume);
	}
	fclose(f);

	traversareTabela(tabela);
	stergere(tabela, 400);
	printf("\n\ndupa stergere: \n");
	traversareTabela(tabela);
	dezalocareTabela(tabela);
}
