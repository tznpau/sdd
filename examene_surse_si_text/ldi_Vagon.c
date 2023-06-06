#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARINGS
#include <stdio.h>
#include <malloc.h>


//1.
typedef struct
{
	int numarVagon;
	char* firmaTransport;
	int numarBileteVandute;
	int capacitateVagon;
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

//2.
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
void stergereLD(nodLD** cap, nodLD** coada, nodLD* sters)
{
	if (*cap == NULL || *coada == NULL || sters == NULL)
		return;
	if (*cap == sters)
	{
		*cap = (*cap)->next;
		if (*cap != NULL)
			(*cap)->prev = NULL;
		free(sters->info.firmaTransport);
		free(sters);
		return;
	}
	if (*coada == sters)
	{
		*coada = (*coada)->prev;
		if (*coada != NULL)
			(*coada)->next = NULL;
		free(sters->info.firmaTransport);
		free(sters);
		return;
	}
	nodLD* anterior = sters->prev;
	nodLD* urmator = sters->next;
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

//4. - incomplet
//5.
typedef struct
{
	Vagon info;
	struct nodCoada* next;
} nodCoada;

void put(nodCoada** prim, nodCoada** ultim, Vagon v)
{
	nodCoada* nou = (nodCoada*)malloc(sizeof(nodCoada));
	nou->info.numarVagon = v.numarVagon;
	nou->info.firmaTransport = (char*)malloc((strlen(v.firmaTransport) + 1) * sizeof(char));
	strcpy(nou->info.firmaTransport, v.firmaTransport);
	nou->info.numarBileteVandute = v.numarBileteVandute;
	nou->info.capacitateVagon = v.capacitateVagon;

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

int get(nodCoada** prim, nodCoada** ultim, Vagon* v)
{
	if (*prim != NULL && *ultim != NULL)
	{
		(*v).numarVagon = (*prim)->info.numarVagon;
		(*v).firmaTransport = (char*)malloc((strlen((*prim)->info.firmaTransport) + 1) * sizeof(char));
		strcpy((*v).firmaTransport, (*prim)->info.firmaTransport);
		(*v).numarBileteVandute = (*prim)->info.numarBileteVandute;
		(*v).capacitateVagon = (*prim)->info.capacitateVagon;

		nodCoada* temp = *prim;
		*prim = (*prim)->next;
		free(temp->info.firmaTransport);
		free(temp);
		return 0;
	}
	if (*prim == NULL)
	{
		*ultim = NULL;
		return -1;
	}
}

void afisareCoada(nodCoada* prim)
{
	nodCoada* temp = prim;
	while (temp)
	{
		printf("\nnr vagon: %d, firma transport: %s, nr bilete vandute: %d, capacitate: %d", temp->info.numarVagon, temp->info.firmaTransport, temp->info.numarBileteVandute, temp->info.capacitateVagon);
		temp = temp->next;
	}
}

float calculGO(Vagon v)
{
	return v.numarBileteVandute / v.capacitateVagon;
}

float minGO(nodLD* cap)
{
	float min = calculGO(cap->info);
	nodLD* temp = cap;
	while (temp)
	{
		if (min > calculGO(temp->info))
			min = calculGO(temp->info);
		temp = temp->next;
	}
	return min;
}

//void salvareLDCoada(nodCoada** prim, nodCoada** ultim, nodLD* cap)
//{
//	Vagon v;
//	
//}

void modifNrBilete(nodCoada* prim, int nrVagon, int nrNouBilete)
{
	nodCoada* temp = prim;
	while (temp)
	{
		if (temp->info.numarVagon == nrVagon)
		{
			temp->info.numarBileteVandute = nrNouBilete;
			printf("\nvagonul cu numarul %d are vandute %d bilete!", nrVagon, nrNouBilete);
		}
		temp = temp->next;
	}
}


void main()
{
	Vagon v;
	char buffer[20];
	int nr;
	nodLD* cap = NULL;
	nodLD* coada = NULL;
	nodCoada* prim = NULL;
	nodCoada* ultim = NULL;


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

		cap = inserareLD(cap, &coada, v);

		put(&prim, &ultim, v);

		free(v.firmaTransport);
	}
	fclose(f);

	printf("traversare ld: \n");
	traversareLD(cap);
	printf("\n\ntraversare ld INVERS: \n");
	traversareLDInvers(coada);

	printf("\n\nstergere: \n");
	stergereMinimLD(&cap, &coada);
	printf("\ntraversare ld: \n");
	traversareLD(cap);
	printf("\n\ntraversare ld INVERS: \n");
	traversareLDInvers(coada);

	printf("\n\ncoada: \n");
	afisareCoada(prim);
	modifNrBilete(prim, 1, 15);

	dezalocareLD(cap);
}