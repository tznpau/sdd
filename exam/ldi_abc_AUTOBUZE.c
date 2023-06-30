#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>

typedef struct
{
	int id;
	char* marca;
	float* km;
	int nrCurse;
	int nrReparatii;
} Autobuz;

typedef struct
{
	Autobuz info;
	struct nodLD* next;
	struct nodLD* prev;
} nodLD;

typedef struct
{
	Autobuz info;
	struct nodArb* left;
	struct nodArb* right;
} nodArb;

Autobuz deepCopyAutobuz(Autobuz a)
{
	Autobuz copie;
	copie.id = a.id;
	copie.marca = (char*)malloc((strlen(a.marca) + 1) * sizeof(char));
	strcpy(copie.marca, a.marca);
	copie.km = (float*)malloc(sizeof(float));
	*copie.km = *a.km;
	copie.nrCurse = a.nrCurse;
	copie.nrReparatii = a.nrReparatii;
	return copie;
}

void afisareAutobuz(Autobuz a)
{
	printf("%d %s %5.2f %d %d\n", a.id, a.marca, *a.km, a.nrCurse, a.nrReparatii);
}

nodLD* inserareLD(nodLD* cap, nodLD** coada, Autobuz a)
{
	nodLD* nou = (nodLD*)malloc(sizeof(nodLD));
	nou->info = deepCopyAutobuz(a);
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
		afisareAutobuz(temp->info);
		temp = temp->next;
	}
}

void traversareLD_invers(nodLD* coada)
{
	nodLD* temp = coada;
	while (temp)
	{
		afisareAutobuz(temp->info);
		temp = temp->prev;
	}
}

void dezalocareLD(nodLD* cap)
{
	nodLD* temp = cap;
	while (temp)
	{
		nodLD* temp2 = temp->next;
		free(temp->info.marca);
		free(temp->info.km);
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
		free(sters->info.marca);
		free(sters->info.km);
		free(sters);
		return;
	}
	if (*coada == sters)
	{
		*coada = (*coada)->prev;
		(*coada)->next = NULL;
		free(sters->info.marca);
		free(sters->info.km);
		free(sters);
		return;
	}
	nodLD* anterior = sters->prev;
	nodLD* urmator = sters->next;
	anterior->next = urmator;
	urmator->prev = anterior;
	free(sters->info.marca);
	free(sters->info.km);
	free(sters);
	return;
}

void stergeNodDupaId(nodLD** cap, nodLD** coada, int id)
{
	nodLD* temp = *cap;
	while (temp)
	{
		if (temp->info.id == id)
		{
			nodLD* sters = temp;
			temp = temp->next;
			stergereNod(cap, coada, sters);
		}
		else
			temp = temp->next;
	}
}

nodArb* creareNod(nodArb* st, nodArb* dr, Autobuz a)
{
	nodArb* nou = (nodArb*)malloc(sizeof(nodArb));
	nou->info = deepCopyAutobuz(a);
	nou->left = st;
	nou->right = dr;
	return nou;
}

nodArb* inserareABC(nodArb* rad, Autobuz a)
{
	if (rad)
	{
		if (a.nrCurse < rad->info.nrCurse)
		{
			rad->left = inserareABC(rad->left, a);
			return rad;
		}
		else if (a.nrCurse > rad->info.nrCurse)
		{
			rad->right = inserareABC(rad->right, a);
			return rad;
		}
		else
			return rad;
	}
	else
		return creareNod(NULL, NULL, a);
}

void inordine(nodArb* rad)
{
	if (rad)
	{
		inordine(rad->left);
		afisareAutobuz(rad->info);
		inordine(rad->right);
	}
}

void dezalocareABC(nodArb* rad)
{
	if (rad)
	{
		nodArb* st = rad->left;
		nodArb* dr = rad->right;
		free(rad->info.marca);
		free(rad->info.km);
		free(rad);
		dezalocareABC(st);
		dezalocareABC(dr);
	}
}

void copiereInArbore(nodLD* cap, nodArb** rad, int prag)
{
	nodLD* temp = cap;
	while (temp)
	{
		if (temp->info.nrCurse < prag)
			*rad = inserareABC(*rad, temp->info);
		temp = temp->next;
	}
}

int maxim(int a, int b)
{
	int max = a;
	if (max < b)
		max = b;
	return max;
}

int inaltimeArb(nodArb* rad)
{
	if (rad)
		return 1 + maxim(inaltimeArb(rad->left), inaltimeArb(rad->right));
	else
		return 0;
}

void determinareNrNoduriCuFii(nodArb* rad, int* nr)
{
	if (rad)
	{
		if (rad->left && rad->right)
			(*nr)++;
		determinareNrNoduriCuFii(rad->left, nr);
		determinareNrNoduriCuFii(rad->right, nr);
	}
}

void afisareNivel(nodArb* rad, int level)
{
	if (!rad)
		return;
	if (level == 1)
		afisareAutobuz(rad->info);
	if (level > 1)
	{
		afisareNivel(rad->left, level - 1);
		afisareNivel(rad->right, level -1);
	}
}

void afisarePeNivel(nodArb* rad, int inaltimeArb)
{
	for (int i = 0; i < inaltimeArb; i++)
	{
		printf("Nivel: %d\n", i + 1);
		afisareNivel(rad, i + 1);
	}
}

void main()
{
	Autobuz a;
	int nrAutobuze;
	nodLD* cap = NULL;
	nodLD* coada = NULL;
	char buffer[20];
	float aux;

	FILE* f = fopen("fisier_Autobuz.txt", "r");
	fscanf(f, "%d", &nrAutobuze);
	Autobuz* vectorAutobuze = (Autobuz*)malloc(nrAutobuze * sizeof(Autobuz));
	for (int i = 0; i < nrAutobuze; i++)
	{
		fscanf(f, "%d", &a.id);
		fscanf(f, "%s", buffer);
		a.marca = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(a.marca, buffer);
		fscanf(f, "%f", &aux);
		a.km = (float*)malloc(sizeof(float));
		*a.km = aux;
		fscanf(f, "%d", &a.nrCurse);
		fscanf(f, "%d", &a.nrReparatii);

		vectorAutobuze[i] = deepCopyAutobuz(a);
	}
	for (int i = 0; i < nrAutobuze - 1; i++)
		for (int j = i + 1; j < nrAutobuze; j++)
			if (vectorAutobuze[i].id > vectorAutobuze[j].id)
			{
				Autobuz aux = vectorAutobuze[i];
				vectorAutobuze[i] = vectorAutobuze[j];
				vectorAutobuze[j] = aux;
			}
	for (int i = 0; i < nrAutobuze; i++)
	{
		cap = inserareLD(cap, &coada, vectorAutobuze[i]);
		free(vectorAutobuze[i].km);
		free(vectorAutobuze[i].marca);
	}

	fclose(f);

	//afisarea listei duble
	//lista este sortata crescator dupa un anumit atribut
	printf("\nAFISARE LISTA AUTOBUZE (SORTATA): \n");
	traversareLD(cap);
	printf("\nAFISARE LISTA AUTOBUZE INVERS (SORTATA DESCRESCATOR): \n");
	traversareLD_invers(coada);

	//functia pentru stergerea unui autobuz dupa un anumit atribut
	printf("\nAFISARE LISTA AUTOBUZE DUPA STERGEREA AUTOBUZULUI CU ID = 1: \n");
	stergeNodDupaId(&cap, &coada, 1);
	traversareLD(cap);
	printf("\nAFISARE LISTA AUTOBUZE (SORTATA DESCRESCATOR) DUPA STERGEREA AUTOBUZULUI CU ID = 1: \n");
	traversareLD_invers(coada);

	//copiaza o parte din autobuzele din LDI intr-o structura Arbore Binar de Cautare, cheia fiind un atribut unic al autobuzului
	printf("\n\nARBORE\n");
	nodArb* rad = NULL;
	copiereInArbore(cap, &rad, 30);
	printf("\nAFISARE ABC (AUTOBUZE CU nrCurse < 30): \n");
	printf("inordine\n");
	inordine(rad);

	//functie ce afiseaza nodurile pe fiecare nivel
	printf("\nNODURI/NIVEL: \n");
	int h = inaltimeArb(rad);
	afisarePeNivel(rad, h);

	//functia pentru determinarea numarului de noduri care au atat fiu stanga cat si fiu dreapta
	int nrNoduriCuFii = 0;
	determinareNrNoduriCuFii(rad, &nrNoduriCuFii);
	printf("\n\nNumarul de noduri care au atat fiu stanga cat si fiu dreapta este urmatorul: %d", nrNoduriCuFii);

	printf("\n\n");
	//dezalocarea structurilor create
	dezalocareABC(rad);
	dezalocareLD(cap);
}