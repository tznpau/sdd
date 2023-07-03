#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>

typedef struct
{
	int cod;
	char* denumire;
	char* proprietar;
	int nrFolosiri;
	float pret;
} Platforma;

typedef struct
{
	Platforma* vector;
	int nrElem;
} MinHEAP;

typedef struct
{
	Platforma info;
	struct nodArb* left;
	struct nodArb* right;
} nodArb;

Platforma deepCopyPlatforma(Platforma p)
{
	Platforma copie;
	copie.cod = p.cod;
	copie.denumire = (char*)malloc((strlen(p.denumire) + 1) * sizeof(char));
	strcpy(copie.denumire, p.denumire);
	copie.proprietar = (char*)malloc((strlen(p.proprietar) + 1) * sizeof(char));
	strcpy(copie.proprietar, p.proprietar);
	copie.nrFolosiri = p.nrFolosiri;
	copie.pret = p.pret;
	return copie;
}

void afisarePlatforma(Platforma p)
{
	printf("Cod: %d, Denumire: %s, Proprietar: %s, Nr folosiri: %d, Pret: %5.2f\n", p.cod, p.denumire, p.proprietar, p.nrFolosiri, p.pret);
}

void filtrare(MinHEAP h, int index)
{
	int indexRad = index;
	int indexSt = 2 * index + 1;
	int indexDr = 2 * index + 2;

	if (indexSt < h.nrElem && h.vector[indexSt].pret < h.vector[indexRad].pret)
		indexRad = indexSt;
	if (indexDr < h.nrElem && h.vector[indexDr].pret < h.vector[indexRad].pret)
		indexRad = indexDr;

	if (indexRad != index)
	{
		Platforma aux = h.vector[index];
		h.vector[index] = h.vector[indexRad];
		h.vector[indexRad] = aux;

		filtrare(h, indexRad);
	}
}

void inserareMinHEAP(MinHEAP* h, Platforma noua)
{
	Platforma* vectNouPlatforme = (Platforma*)malloc((h->nrElem + 1) * sizeof(Platforma));
	for (int i = 0; i < h->nrElem; i++)
		vectNouPlatforme[i] = h->vector[i];
	vectNouPlatforme[h->nrElem] = noua;
	h->nrElem++;
	free(h->vector);
	h->vector = vectNouPlatforme;
	for (int i = (h->nrElem - 1) / 2; i >= 0; i--)
		filtrare(*h, i);
}

void extragereMinHEAP(MinHEAP* h, Platforma* extrasa)
{
	Platforma* vectNouPlatforme = (Platforma*)malloc((h->nrElem - 1) * sizeof(Platforma));
	Platforma aux = h->vector[0]; //dorim sa il extragem pe primul, deci il interschimbam cu ultimul si apoi *extrasa ia valoarea lui
	h->vector[0] = h->vector[h->nrElem - 1];
	h->vector[h->nrElem - 1] = aux;
	*extrasa = h->vector[h->nrElem - 1];
	h->nrElem--;
	for (int i = 0; i < h->nrElem; i++)
		vectNouPlatforme[i] = h->vector[i];
	free(h->vector);
	h->vector = vectNouPlatforme;
	for (int i = (h->nrElem - 1) / 2; i >= 0; i--)
		filtrare(*h, i);
}

void afisareMinHEAP(MinHEAP h)
{
	for (int i = 0; i < h.nrElem; i++)
		afisarePlatforma(h.vector[i]);
}

void dezalocareMinHEAP(MinHEAP h)
{
	for (int i = 0; i < h.nrElem; i++)
	{
		free(h.vector[i].denumire);
		free(h.vector[i].proprietar);
	}
	free(h.vector);
}

void modificaCuPretMinim(MinHEAP h, float pretNou)
{
	h.vector[0].pret = pretNou;
	for (int i = (h.nrElem - 1) / 2; i >= 0; i--)
		filtrare(h, i);
}


nodArb* creareNod(nodArb* st, nodArb* dr, Platforma p)
{
	nodArb* nou = (nodArb*)malloc(sizeof(nodArb));
	nou->info = deepCopyPlatforma(p);
	nou->left = st;
	nou->right = dr;
	return nou;
}

nodArb* inserareABC(nodArb* rad, Platforma p)
{
	if (rad)
	{
		if (p.nrFolosiri < rad->info.nrFolosiri)
		{
			rad->left = inserareABC(rad->left, p);
			return rad;
		}
		else if (p.nrFolosiri > rad->info.nrFolosiri)
		{
			rad->right = inserareABC(rad->right, p);
			return rad;
		}
		else
			return rad;
	}
	else
		return creareNod(NULL, NULL, p);
}

//se copiaza primele n platforme
void copiere_n_platforme_inABC(MinHEAP h, nodArb** rad, int n)
{
	for (int i = 0; i < n; i++)
	{
		printf("[%d]\n", i);
		afisarePlatforma(h.vector[i]);
		*rad = inserareABC(*rad, h.vector[i]);
	}
	printf("\n");
}

void inordine(nodArb* rad)
{
	if (rad)
	{
		inordine(rad->left);
		afisarePlatforma(rad->info);
		inordine(rad->left);
	}
}

void dezalocareABC(nodArb* rad)
{
	if (rad)
	{
		nodArb* st = rad->left;
		nodArb* dr = rad->right;
		free(rad->info.denumire);
		free(rad->info.proprietar);
		free(rad);
		dezalocareABC(st);
		dezalocareABC(dr);
	}
}

void afisarePlatformeNodCu1Fiu(nodArb* rad)
{
	if (rad)
	{
		if ((rad->left && !rad->right) || (!rad->left && rad->right))
			afisarePlatforma(rad->info);
		afisarePlatformeNodCu1Fiu(rad->left);
		afisarePlatformeNodCu1Fiu(rad->right);
	}
}



void main()
{
	MinHEAP h;
	char buffer[20];
	Platforma p;


	//1. crearea structurii de MinHEAP
	//prioritatea data de pret
	//inserarea unei platforme se implementeaza intr-o functie care se apeleaza in secventa de creare a structurii MinHEAP
	FILE* f = fopen("fisier_Platforme.txt", "r");
	h.nrElem = 0;
	h.vector = NULL;
	fscanf(f, "%d", &p.cod);
	while (!feof(f))
	{
		fscanf(f, "%s", buffer);
		p.denumire = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(p.denumire, buffer);
		fscanf(f, "%s", buffer);
		p.proprietar = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(p.proprietar, buffer);
		fscanf(f, "%d", &p.nrFolosiri);
		fscanf(f, "%f", &p.pret);
		inserareMinHEAP(&h, p);
		fscanf(f, "%d", &p.cod);
	}
	for (int i = (h.nrElem - 1) / 2; i >= 0; i--)
		filtrare(h, i);
	printf("\nAFISARE HEAP DUPA INSERARE:\n\n");
	afisareMinHEAP(h);

	//2. functia care modifica pretul platformei cu pretul cel mai mic din cadrul structurii MinHEAP
	//platforma e reintrodusa in cadrul structurii cu noul pret
	printf("\n\nAFISARE HEAP DUPA MODIFICAREA PRETULUI MINIM:\n\n");
	modificaCuPretMinim(h, 190);
	afisareMinHEAP(h);

	//3. sunt copiate primele n = 5 platforme din heap intr-un arbore binar de cautare
	//inserarea in arbore se realizeaza pe baza unui camp definit in Platforma diferit de pret
	nodArb* rad = NULL;
	printf("\n\nELEMENTE COPIATE DIN HEAP IN ARBORE:\n\n");
	copiere_n_platforme_inABC(h, &rad, 5);
	printf("\n\nPARCURGERE PRIN INORDINE:\n\n");
	inordine(rad);

	//4. functia care afiseaza platformele aflate in nodurile care au un singur nod descendent
	printf("\n\nPLATFORME DIN NODURI CU 1 FIU:\n\n");
	afisarePlatformeNodCu1Fiu(rad);

	dezalocareMinHEAP(h);
	dezalocareABC(rad);
}