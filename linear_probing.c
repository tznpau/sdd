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
	student** vect;
	int nrElem;
} hashT;

int functieDispersie(hashT tabela, int cheie)
{
	return cheie % tabela.nrElem;
}

void inserareTabela(hashT tabela, student* s)
{
	int poz = 0;
	if (tabela.vect != NULL)
	{
		poz = functieDispersie(tabela, (*s).id);

		if (tabela.vect[poz] == NULL)
			tabela.vect[poz] = s;
		else //coliziune
		{
			int i = 1; //index
			while (poz + i < tabela.nrElem) //pas linear probing
			{
				if (tabela.vect[poz + i] == NULL)
				{
					poz += i;
					tabela.vect[poz] = s;
					break;
				}
				else
					i++;
			}
		}
	}
}

void traversareTabela(hashT tabela)
{
	if (tabela.vect != NULL)
		for (int i = 0; i < tabela.nrElem; i++)
			if (tabela.vect[i] != NULL)
			{
				printf("\npozitie: %d", i);
				printf("\nid student: %d, nume: %s, medie: %5.2f", tabela.vect[i]->id, tabela.vect[i]->nume, tabela.vect[i]->medie);
			}
}

void dezalocareTabela(hashT tabela)
{
	if (tabela.vect != NULL)
	{
		for (int i = 0; i < tabela.nrElem; i++)
			if (tabela.vect[i] != NULL)
			{
				free(tabela.vect[i]->nume);
				free(tabela.vect[i]);
			}
		free(tabela.vect);
	}
}

int stergere(hashT tabela, int id)
{
	int poz;
	if (tabela.vect != NULL)
	{
		poz = functieDispersie(tabela, id);
		if (tabela.vect[poz] == NULL)
			return -1;
		else
		{
			if (tabela.vect[poz]->id == id)
			{
				free(tabela.vect[poz]->nume);
				free(tabela.vect[poz]);
				tabela.vect[poz] = NULL;
			}
			else
			{
				int i = 1;
				while (poz + i < tabela.nrElem)
				{
					if (tabela.vect[poz + i]->id == id)
					{
						poz += i;
						free(tabela.vect[poz]->nume);
						free(tabela.vect[poz]);
						tabela.vect[poz] = NULL;
						break;
					}
					i++;
				}
			}
		}
	}
	return poz;
}

void main()
{
	hashT tabela;
	tabela.nrElem = 31;
	tabela.vect = (student**)malloc(tabela.nrElem * sizeof(student*));
	for (int i = 0; i < tabela.nrElem; i++)
		tabela.vect[i] = NULL;

	int n;
	student* s;
	char buffer[20];

	FILE* f = fopen("studenti_hash2.txt", "r");
	fscanf(f, "%d", &n);

	for (int i = 0; i < n; i++)
	{
		s = (student*)malloc(sizeof(student));
		fscanf(f, "%d", &s->id);
		fscanf(f, "%s", buffer);
		s->nume = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(s->nume, buffer);
		fscanf(f, "%f", &s->medie);

		inserareTabela(tabela, s);
	}
	fclose(f);

	traversareTabela(tabela);
	stergere(tabela, 400);
	printf("\n\ndupa stergere: ");
	traversareTabela(tabela);
	dezalocareTabela(tabela);
}