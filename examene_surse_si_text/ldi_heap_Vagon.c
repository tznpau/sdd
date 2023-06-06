#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>

//1.
//ldi
//1.
typedef struct
{
	int numarVagon;
	char* firmaTransport;
	int numarBileteVandute;
	int capacitateVagon;
	int GO;
} Vagon;

typedef struct
{
	Vagon info;
	struct nodLD* next;
	struct nodLD* prev;
} nodLD;

nodLD* inserareLD(nodLD* cap, nodLD** coada, Vagon v)
{
	nodLD* nou = (nodLD*)malloc(sizeof(nodLD));
	nou->info.numarVagon = v.numarVagon;
	nou->info.firmaTransport = (char*)malloc((strlen(v.firmaTransport) + 1) * sizeof(char));
	strcpy(nou->info.firmaTransport, v.firmaTransport);
	nou->info.numarBileteVandute = v.numarBileteVandute;
	nou->info.capacitateVagon = v.capacitateVagon;

	nou->next = NULL;
	nou->prev = NULL;

	if (cap == NULL || *coada == NULL)
	{
		cap = nou;
		*coada = nou;
	}
	else
	{
		nodLD* temp = cap;
		while (temp->next)
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
		printf("\nnr vagon: %d, firma transport: %s, nr bilete vandute: %d, capacitate: %d", temp->info.numarVagon, temp->info.firmaTransport, temp->info.numarBileteVandute, temp->info.capacitateVagon);
		temp = temp->next;
	}
}

void traversareLDInvers(nodLD* coada)
{
	nodLD* temp = coada;
	while (temp)
	{
		printf("\nnr vagon: %d, firma transport: %s, nr bilete vandute: %d, capacitate: %d", temp->info.numarVagon, temp->info.firmaTransport, temp->info.numarBileteVandute, temp->info.capacitateVagon);
		temp = temp->prev;
	}
}

void dezalocareLD(nodLD* cap)
{
	nodLD* temp = cap;
	while (temp)
	{
		nodLD* temp2 = temp->next;
		free(temp->info.firmaTransport);
		free(temp);
		temp = temp2;
	}
}

//3.
//functie care sterge vagonul cu nr minim de bilete vandute
void stergereLD(nodLD** cap, nodLD** coada, nodLD* sters)
{
	if (*cap == NULL || *coada == NULL || sters == NULL)
		return;
	if (*cap == sters)
	{
		*cap = (*cap)->next;
		(*cap)->prev = NULL;
		free(sters->info.firmaTransport);
		free(sters);
		return;
	}
	if (*coada == sters)
	{
		*coada = (*coada)->prev;
		(*coada)->next = NULL;
		free(sters->info.firmaTransport);
		free(sters);
		return;
	}
	nodLD* urmator = sters->next;
	nodLD* anterior = sters->prev;
	anterior->next = urmator;
	urmator->prev = anterior;
	free(sters->info.firmaTransport);
	free(sters);
	return;
}

int cautareMinimBilete(nodLD** cap)
{
	int min = (*cap)->info.numarBileteVandute;
	nodLD* temp = *cap;
	while (temp)
	{
		if (min > temp->info.numarBileteVandute)
			min = temp->info.numarBileteVandute;
		temp = temp->next;
	}
	return min;
}

void stergereMinimLD(nodLD** cap, nodLD** coada)
{
	nodLD* temp = *cap;
	int min = cautareMinimBilete(cap);
	while (temp)
	{
		if (temp->info.numarBileteVandute == min)
		{
			nodLD* sters = temp;
			temp = temp->next;
			stergereLD(cap, coada, sters);
		}
		else
			temp = temp->next;
	}
}

//4.
//problema cu afisarea gradului de ocupare
float calculGO(Vagon v)
{
	return (float)v.numarBileteVandute / v.capacitateVagon;
}

//heap
typedef struct
{
	Vagon* vect;
	int nrElem;
} heap;

void filtrare(heap h, int index)
{
	int indexRad = index;
	int indexSt = 2 * index + 1;
	int indexDr = 2 * index + 2;

	if (indexSt < h.nrElem && h.vect[indexSt].GO < h.vect[indexRad].GO)
		indexRad = indexSt;
	if (indexDr < h.nrElem && h.vect[indexDr].GO < h.vect[indexRad].GO)
		indexRad = indexDr;

	if (indexRad != index)
	{
		Vagon temp = h.vect[index];
		h.vect[index] = h.vect[indexRad];
		h.vect[indexRad] = temp;

		filtrare(h, indexRad);
	}
}

heap inserareHeap(heap h, Vagon nou)
{
	Vagon* vectNou = (Vagon*)malloc((h.nrElem + 1) * sizeof(Vagon));
	for (int i = 0; i < h.nrElem; i++)
		vectNou[i] = h.vect[i];
	h.nrElem++;
	vectNou[h.nrElem - 1] = nou;
	free(h.vect);
	h.vect = vectNou;
	for (int i = (h.nrElem - 1) / 2; i >= 0; i--)
		filtrare(h, i);
	return h;
}

heap extragereHeap(heap h, Vagon* extras)
{
	Vagon* vectNou = (Vagon*)malloc((h.nrElem - 1) * sizeof(Vagon));

	Vagon temp = h.vect[0];
	h.vect[0] = h.vect[h.nrElem - 1];
	h.vect[h.nrElem - 1] = temp;
	*extras = h.vect[h.nrElem - 1];

	h.nrElem--;
	for (int i = 0; i < h.nrElem; i++)
		vectNou[i] = h.vect[i];
	free(h.vect);
	h.vect = vectNou;
	for (int i = (h.nrElem - 1) / 2; i >= 0; i--)
		filtrare(h, i);
	return h;
}

void traversareHeap(heap h)
{
	for (int i = 0; i < h.nrElem; i++)
		printf("\nnr vagon: %d, firma transport: %s, nr bilete vandute: %d, capacitate: %d, grad de ocupare: %5.2f", h.vect[i].numarVagon, h.vect[i].firmaTransport, h.vect[i].numarBileteVandute, h.vect[i].capacitateVagon, h.vect[i].GO);
}

void dezalocareHeap(heap h)
{
	for (int i = 0; i < h.nrElem; i++)
		free(h.vect[i].firmaTransport);
	free(h.vect);
}

//5.
void modifNrBilete(heap h, int nrVagon, int nrBilete)
{
	for (int i = 0; i < h.nrElem; i++)
	{
		if (h.vect[i].numarVagon == nrVagon)
		{
			h.vect[i].numarBileteVandute = nrBilete;
			printf("\nvagonul cu numarul %d are vandute %d bilete!", nrVagon, nrBilete);
		}
	}
}

void main()
{
	Vagon v;
	char buffer[20];
	int nr;
	nodLD* cap = NULL;
	nodLD* coada = NULL;

	FILE* f = fopen("fisier_Vagon.txt", "r");
	fscanf(f, "%d", &nr);

	for (int i = 0; i < nr; i++)
	{
		fscanf(f, "%d", &v.numarVagon);
		fscanf(f, "%s", buffer);
		v.firmaTransport = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(v.firmaTransport, buffer);
		fscanf(f, "%d", &v.numarBileteVandute);
		fscanf(f, "%d", &v.capacitateVagon);
		v.GO = calculGO(v);

		cap = inserareLD(cap, &coada, v);

		free(v.firmaTransport);
	}
	fclose(f);

	printf("traversari\n");
	traversareLD(cap);
	printf("\n");
	traversareLDInvers(coada);

	printf("\n\ndupa stergerea minimului: \n");
	stergereMinimLD(&cap, &coada);
	traversareLD(cap);
	printf("\n");
	traversareLDInvers(coada);



	heap h;
	f = fopen("fisier_Vagon.txt", "r");
	fscanf(f, "%d", &h.nrElem);
	h.vect = (Vagon*)malloc(h.nrElem * sizeof(Vagon));
	for (int i = 0; i < h.nrElem; i++)
	{
		fscanf(f, "%d", &h.vect[i].numarVagon);
		fscanf(f, "%s", buffer);
		h.vect[i].firmaTransport = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(h.vect[i].firmaTransport, buffer);
		fscanf(f, "%d", &h.vect[i].numarBileteVandute);
		fscanf(f, "%d", &h.vect[i].capacitateVagon);
		h.vect[i].GO = calculGO(h.vect[i]);
	}
	fclose(f);


	printf("\n\nheap\n");
	for (int i = (h.nrElem - 1) / 2; i >= 0; i--)
		filtrare(h, i);
	traversareHeap(h);

	printf("\n\n\n");

	modifNrBilete(h, 1, 18);
	for (int i = (h.nrElem - 1) / 2; i >= 0; i--)
		filtrare(h, i);
	traversareHeap(h);

	dezalocareLD(cap);
	dezalocareHeap(h);
}