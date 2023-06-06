#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNIGNS
#include <stdio.h>
#include <malloc.h>

//1.
//arbore binar de cautare

typedef struct
{
	unsigned int id;
	char* denumireHotel;
	unsigned char nrCamere;
	char* numeClient;
	float suma;
} rezervare;

typedef struct
{
	rezervare info;
	struct nodArb* left;
	struct nodArb* right;
} nodArb;

nodArb* creareNod(nodArb* st, nodArb* dr, rezervare r)
{
	nodArb* nou = (nodArb*)malloc(sizeof(nodArb));
	nou->info.id = r.id;
	nou->info.denumireHotel = (char*)malloc((strlen(r.denumireHotel) + 1) * sizeof(char));
	strcpy(nou->info.denumireHotel, r.denumireHotel);
	nou->info.nrCamere = r.nrCamere;
	nou->info.numeClient = (char*)malloc((strlen(r.numeClient) + 1) * sizeof(char));
	strcpy(nou->info.numeClient, r.numeClient);
	nou->info.suma = r.suma;

	nou->left = st;
	nou->right = dr;

	return nou;
}

nodArb* inserareArb(nodArb* rad, rezervare r)
{
	if (rad != NULL)
	{
		if (r.id < rad->info.id)
		{
			rad->left = inserareArb(rad->left, r);
			return rad;
		}
		else
		{
			if (r.id > rad->info.id)
			{
				rad->right = inserareArb(rad->right, r);
				return rad;
			}
			else
				return rad;
		}
	}
	else
		return creareNod(NULL, NULL, r);
}

void dezalocareArb(nodArb* rad)
{
	if (rad != NULL)
	{
		nodArb* st = rad->left;
		nodArb* dr = rad->right;
		free(rad->info.denumireHotel);
		free(rad->info.numeClient);
		free(rad);
		dezalocareArb(st);
		dezalocareArb(dr);
	}
}

//2.
void preordine(nodArb* rad)
{
	if (rad != NULL)
	{
		printf("\nid rezervare: %u, nume client: %s", rad->info.id, rad->info.numeClient);
		preordine(rad->left);
		preordine(rad->right);
	}
}

void inordine(nodArb* rad)
{
	if (rad != NULL)
	{
		inordine(rad->left);
		printf("\nid rezervare: %u, nume client: %s", rad->info.id, rad->info.numeClient);
		inordine(rad->right);
	}
}

void postordine(nodArb* rad)
{
	if (rad != NULL)
	{
		postordine(rad->left);
		postordine(rad->right);
		printf("\nid rezervare: %u, nume client: %s", rad->info.id, rad->info.numeClient);
	}
}

//3.
//functie care afla suma totala a unui client
float getSumaTotalaClient(nodArb* rad, char numeClient[20])
{
	if (rad != NULL)
	{
		float sumaTotala = 0;
		if (strcmp(rad->info.numeClient, numeClient) == 0)
			sumaTotala += rad->info.suma;

		sumaTotala += getSumaTotalaClient(rad->left, numeClient);
		sumaTotala += getSumaTotalaClient(rad->right, numeClient);
	}
	else
		return 0;
}

//4.
nodArb* stergeRad(nodArb* rad)
{
	nodArb* aux = rad;
	if (rad->left != NULL)
	{
		rad = aux->left;
		if (aux->right != NULL)
		{
			nodArb* max = aux->left;
			while (max->right)
				max = max->right;
			max->right = aux->right;
		}
	}
	else
		if (aux->right != NULL)
			rad = aux->right;
		else
			rad = NULL;
	free(aux->info.denumireHotel);
	free(aux->info.numeClient);
	free(aux);
	return rad;
}

//de fapt
//fac o functie care tot traverseaza partea stanga
//pana cand gaseste nodul care nu are niciun nod stang
nodArb* stergeNodMinim(nodArb* rad)
{
	if (rad == NULL)
		return NULL;
	if (rad->left == NULL) //deci cel mai mic nod e radacina; pe el il stergem
	{
		nodArb* copilDr = rad->right;
		free(rad->info.denumireHotel);
		free(rad->info.numeClient);
		free(rad);
		return copilDr;
	}
	rad->left = stergeNodMinim(rad->left);
	return rad;
}

//5.
//functie ce returneaza un vector cu nr de noduri plasate pe fiecare nivel din arbore

//mai intai calculez inaltimea arborelui
int max(int a, int b)
{
	int max = a;
	if (max < b)
		max = b;
	return max;
}

int inaltimeArb(nodArb* rad)
{
	if (rad != NULL)
	{
		return 1 + max(inaltimeArb(rad->left), inaltimeArb(rad->right));
	}
	else
		return 0;
}

void nrNoduriPeNivelRec(nodArb* nod, int* levelCounts, int nivel)
{
	if (nod == NULL)
		return;
	levelCounts[nivel]++;
	nrNoduriPeNivelRec(nod->left, levelCounts, nivel + 1);
	nrNoduriPeNivelRec(nod->right, levelCounts, nivel + 1);
}

int* vectNrNoduriPeNivel(nodArb* rad, int* nr)
{
	if (rad == NULL)
	{
		*nr = 0;
		return NULL;
	}

	int inaltime = inaltimeArb(rad);
	int* levelCounts = (int*)malloc(inaltime * sizeof(int));

	nrNoduriPeNivelRec(rad, levelCounts, 0);
	*nr = inaltime;
	return levelCounts;
}


void main()
{
	rezervare r;
	char buffer[20];
	int nr;
	nodArb* rad = NULL;

	FILE* f = fopen("fisier_Rezervare.txt", "r");
	fscanf(f, "%d", &nr);
	for (int i = 0; i < nr; i++)
	{
		fscanf(f, "%u", &r.id);
		fscanf(f, "%s", buffer);
		r.denumireHotel = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(r.denumireHotel, buffer);
		fscanf(f, "%hhu", &r.nrCamere);
		fscanf(f, "%s", buffer);
		r.numeClient = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(r.numeClient, buffer);
		fscanf(f, "%f", &r.suma);

		rad = inserareArb(rad, r);
	}
	fclose(f);

	printf("\nparcurgere in preordine: \n");
	preordine(rad);
	printf("\n\nparcurgere in inordine: \n");
	inordine(rad);
	printf("\n\nparcurgere in postordine: \n");
	postordine(rad);

	float sumaMaria = getSumaTotalaClient(rad, "Maria");
	printf("\n\ncontravaloarea rezervarilor Mariei este de %5.2f RON\n", sumaMaria);

	printf("\n\ndupa stergerea nodului minim: \n");
	rad = stergeNodMinim(rad);
	preordine(rad);

	int nrNoduri = 0;
	int* nrNoduriPeNivele = vectNrNoduriPeNivel(rad, &nrNoduri);
	printf("\n\nnr noduri pe nivele: \n");
	for (int i = 0; i < nrNoduri; i++)
		printf("nivel %d: %d noduri\n", i + 1, nrNoduriPeNivele[i]);
	free(nrNoduriPeNivele);

	dezalocareArb(rad);
}