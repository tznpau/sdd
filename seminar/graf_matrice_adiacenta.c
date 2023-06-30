#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>

typedef struct
{
	int id;
	char* den;
} oras;

typedef struct
{
	int info;
	struct nodStiva* next;
} nodStiva;

typedef struct
{
	int info;
	struct nodCoada* next;
} nodCoada;

typedef struct
{
	int idNodStart, idNodFinal, greutateArc;
	struct nodLS* next;
} nodLS;

void push(nodStiva** varf, int val)
{
	nodStiva* nou = (nodStiva*)malloc(sizeof(nodStiva));
	nou->info = val;
	nou->next = NULL;

	if (*varf == NULL)
		*varf = nou;
	else
	{
		nou->next = *varf;
		*varf = nou;
	}
}

void put(nodCoada** prim, nodCoada** ultim, int val)
{
	nodCoada* nou = (nodCoada*)malloc(sizeof(nodCoada));
	nou->info = val;
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

int pop(nodStiva** varf, int* val)
{
	if (*varf == NULL)
		return -1;
	else
	{
		(*val) = (*varf)->info;

		nodStiva* temp = *varf;
		*varf = (*varf)->next;
		free(temp);
		return 0;
	}
}

int get(nodCoada** prim, nodCoada** ultim, int* val)
{
	if (*prim != NULL && *ultim != NULL)
	{
		(*val) = (*prim)->info;

		nodCoada* temp = *prim;
		*prim = (*prim)->next;
		free(temp);
		return 0;
	}
	if (*prim == NULL)
	{
		*ultim = NULL;
		return -1;
	}
}

//adancime
void parcurgereDF(int** mat, int nrNoduri, oras* vect, int* vizitat, int idNodStart)
{
	for (int i = 0; i < nrNoduri; i++)
		vizitat[i] = 0; 

	nodStiva* varf = NULL;
	push(&varf, idNodStart); //se pune pe stiva nodurilor de vizitat
	vizitat[idNodStart] = 1;

	while (varf)
	{
		pop(&varf, &idNodStart); //se scoate de pe stiva
		printf("\n%d.%s ", idNodStart + 1, vect[idNodStart].den); //nodul a fost vizitat si se afiseaza
		for (int k = 0; k < nrNoduri; k++)
			if (mat[idNodStart][k] != 0 && vizitat[k] == 0)
			{
				push(&varf, k);
				vizitat[k] = 1;
			}
	}
}

//latime
void parcurgereBF(int** mat, int nrNoduri, oras* vect, int* vizitat, int idNodStart)
{
	for (int i = 0; i < nrNoduri; i++)
		vizitat[i] = 0;

	nodCoada* prim, * ultim = NULL;
	put(&prim, &ultim, idNodStart); //se pun in coada
	vizitat[idNodStart] = 1;

	while (prim)
	{
		get(&prim, &ultim, &idNodStart);
		printf("\n%d.%s ", idNodStart + 1, vect[idNodStart].den);
		for (int k = 0; k < nrNoduri; k++)
			if (mat[idNodStart][k] != 0 && vizitat[k] == 0)
			{
				put(&prim, &ultim, k);
				vizitat[k] = 1;
			}
	}
}

nodLS* inserareLS(nodLS* cap, int idNodStart, int idNodFinal, int greutateArc)
{
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->idNodStart = idNodStart;
	nou->idNodFinal = idNodFinal;
	nou->greutateArc = greutateArc;
	nou->next = NULL;

	if (cap == NULL)
		cap = nou;
	else
	{
		nodLS* temp = cap;
		while (temp->next)
			temp = temp->next;
		temp->next = nou;
	}
	return cap;
}

void afisareLS(nodLS* cap)
{
	nodLS* temp = cap;
	while (temp->next)
	{
		printf("(%d, %d, %d) -> ", temp->idNodStart, temp->idNodFinal, temp->greutateArc);
		temp = temp->next;
	}
	printf("(%d, %d, %d)\n", temp->idNodStart, temp->idNodFinal, temp->greutateArc); //sa nu apara ->
}

void trasformaListaArce(nodLS** cap, int** mat, int nrNoduri)
{
	for (int i = 0; i < nrNoduri; i++)
		for (int j = 0; j < nrNoduri; j++)
			if (mat[i][j] != 0)
				*cap = inserareLS(*cap, i + 1, j + 1, mat[i][j]);
}

nodLS* arcGreutateMinima(nodLS* cap)
{
	nodLS* temp = cap;
	nodLS* min = cap;
	while (temp)
	{
		if (min->greutateArc > temp->greutateArc)
			min = temp;
		temp = temp->next;
	}
	return min;
}

nodLS* arcGreutateMaxima(nodLS* cap)
{
	nodLS* temp = cap;
	nodLS* max = cap;
	while (temp)
	{
		if (max->greutateArc < temp->greutateArc)
			max = temp;
		temp = temp->next;
	}
	return max;
}

void main()
{
	FILE* f = fopen("matriceAd.txt", "r");
	int nrNoduri, nrArce;

	fscanf(f, "%d", &nrNoduri);

	oras* vect = (oras*)malloc(nrNoduri * sizeof(oras));
	char buffer[20];

	for (int i = 0; i < nrNoduri; i++)
	{
		fscanf(f, "%d", &vect[i].id);
		fscanf(f, "%s", buffer);
		vect[i].den = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(vect[i].den, buffer);
	}

	//matricea de adiacenta
	int** mat = (int**)malloc(nrNoduri * sizeof(int*));
	for (int i = 0; i < nrNoduri; i++)
		mat[i] = (int*)malloc(nrNoduri * sizeof(int));
	for (int i = 0; i < nrNoduri; i++)
		for (int j = 0; j < nrNoduri; j++)
			mat[i][j] = 0;

	fscanf(f, "%d", &nrArce);
	int ii, jj, greutate;
	for (int i = 0; i < nrArce; i++)
	{
		fscanf(f, "%d", &ii);
		fscanf(f, "%d", &jj);
		fscanf(f, "%d", &greutate);
		mat[ii - 1][jj - 1] = greutate;
		mat[jj - 1][ii - 1] = greutate; //cu adaugarea acestei linii => matricea va fi simetrica => graf neorientat
	}
	fclose(f);

	//pentru functia de parcurgere - vector prin care etichetam daca am trecut sau nu prin fiecare
	int* vizitat = (int*)malloc(nrNoduri * sizeof(int));

	int idNodStart;
	printf("\nparcurgere in adancime (DF) de la nodul: ");
	scanf("%d", &idNodStart);
	parcurgereDF(mat, nrNoduri, vect, vizitat, idNodStart);

	printf("\n\nparcurgere in latime (BF) de la nodul: ");
	scanf("%d", &idNodStart);
	parcurgereBF(mat, nrNoduri, vect, vizitat, idNodStart);

	printf("\n\nlista de arce: ");
	nodLS* cap = NULL;
	trasformaListaArce(&cap, mat, nrNoduri);
	afisareLS(cap);

	nodLS* arcMinim = arcGreutateMinima(cap);
	printf("\n\narcul cu greutate minima: (%d %d %d)", arcMinim->idNodStart, arcMinim->idNodFinal, arcMinim->greutateArc);
}