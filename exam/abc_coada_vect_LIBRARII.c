#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>

typedef struct
{
	int idLibrarie;
	char* denumire;
	float profit;
} Librarie;

typedef struct
{
	Librarie info;
	struct nodArb* left;
	struct nodArb* right;
} nodArb;

typedef struct
{
	Librarie info;
	struct nodCoada* next;
} nodCoada;

Librarie deepCopyLibrarie(Librarie l)
{
	Librarie copie;
	copie.idLibrarie = l.idLibrarie;
	copie.denumire = (char*)malloc((strlen(l.denumire) + 1) * sizeof(char));
	strcpy(copie.denumire, l.denumire);
	copie.profit = l.profit;
	return copie;
}

void afisareLibrarie(Librarie l)
{
	printf("ID: %d, DENUMRIE: %s, PROFIT: %5.2f\n", l.idLibrarie, l.denumire, l.profit);
}

nodArb* creareNod(nodArb* st, nodArb* dr, Librarie l)
{
	nodArb* nou = (nodArb*)malloc(sizeof(nodArb));
	nou->info = deepCopyLibrarie(l);
	nou->left = st;
	nou->right = dr;
	return nou;
}

nodArb* inserareABC(nodArb* rad, Librarie l)
{
	if (rad)
	{
		if (strcmp(l.denumire, rad->info.denumire) == -1)
		{
			rad->left = inserareABC(rad->left, l);
			return rad;
		}
		else if (strcmp(l.denumire, rad->info.denumire) == 1)
		{
			rad->right = inserareABC(rad->right, l);
			return rad;
		}
		else
			return rad;
	}
	else
		return creareNod(NULL, NULL, l);
}

void inordine(nodArb* rad)
{
	if (rad)
	{
		inordine(rad->left);
		afisareLibrarie(rad->info);
		inordine(rad->right);
	}
}

void dezalocareABC(nodArb* rad)
{
	if (rad)
	{
		nodArb* st = rad->left;
		nodArb* dr = rad->right;
		free(rad->info.denumire);
		free(rad);
		dezalocareABC(st);
		dezalocareABC(dr);
	}
}

void copiereToFromABC(nodArb* rad, nodArb** destinatie)
{
	if (rad)
	{
		if (rad->info.idLibrarie % 2 == 0)
			*destinatie = inserareABC(*destinatie, rad->info);
		copiereToFromABC(rad->left, destinatie);
		copiereToFromABC(rad->right, destinatie);
	}
	else
		return destinatie;
}

//coada
void put(nodCoada** prim, nodCoada** ultim, Librarie l)
{
	nodCoada* nou = (nodCoada*)malloc(sizeof(nodCoada));
	nou->info = deepCopyLibrarie(l);
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

int get(nodCoada** prim, nodCoada** ultim, Librarie* l)
{
	if (*prim != NULL && *ultim != NULL)
	{
		(*l).idLibrarie = (*prim)->info.idLibrarie;
		(*l).denumire = (char*)malloc((strlen((*prim)->info.denumire) + 1) * sizeof(char));
		strcpy((*l).denumire, (*prim)->info.denumire);
		(*l).profit = (*prim)->info.profit;

		nodCoada* temp = *prim;
		*prim = (*prim)->next;
		free(temp->info.denumire);
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
		afisareLibrarie(temp->info);
		temp = temp->next;
	}
}

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
}

nodArb* stergeDupaCheie(nodArb* rad, char* cheie)
{
	if (rad)
	{
		if (strcmp(cheie, rad->info.denumire) == -1)
			rad->left = stergeDupaCheie(rad->left, cheie);
		else
			if (strcmp(cheie, rad->info.denumire) == 1)
				rad->right = stergeDupaCheie(rad->right, cheie);
			else
				rad = stergeRad(rad);
		return rad;
	}
	else
		return NULL;
}

nodArb* cautareNod(nodArb* rad, char* cheie)
{
	if (rad)
	{
		if (strcmp(cheie, rad->info.denumire) == 0)
			return rad;
		else if (strcmp(cheie, rad->info.denumire) == -1)
			return cautareNod(rad->left, cheie);
		else
			return cautareNod(rad->right, cheie);
	}
	else
		return NULL;
}

//functie ce salveaza denumirile librariilor cu profit < 2500 intr-un vector
void getDenumiri(nodArb* rad, char** vector, int* dim)
{
	if (rad)
	{
		if (rad->info.profit < 2500)
		{
			vector[*dim] = (char*)malloc((strlen(rad->info.denumire) + 1) * sizeof(char));
			strcpy(vector[*dim], rad->info.denumire);
			(*dim)++;
		}
		getDenumiri(rad->left, vector, dim);
		getDenumiri(rad->right, vector, dim);
	}
}

//se cere inserarea descrescator a cheilor pentru cei ce nu au descendenti
void getVectorSortat(nodArb* rad, Librarie* vector, int* dim)
{
	if (rad)
	{
		if (rad->left == NULL && rad->right == NULL) //e nod frunza
			vector[(*dim)++] = deepCopyLibrarie(rad->info);
		getVectorSortat(rad->left, vector, dim);
		getVectorSortat(rad->right, vector, dim);
	}
	else
	{
		for (int i = 0; i < *dim - 1; i++)
			for (int j = i + 1; j < *dim; j++)
				if (strcmp(vector[i].denumire, vector[j].denumire) == -1)
				{
					Librarie aux = vector[i];
					vector[i] = vector[j];
					vector[j] = aux;
				}
	}
}


//functia de stergere a librariilor le salveaza simultan in coada
void stergereLibrarii(nodArb** rad, char** vector, int dim, nodCoada** prim, nodCoada** ultim)
{
	nodArb* cautat;
	for (int i = 0; i < dim; i++)
	{
		cautat = cautareNod(*rad, vector[i]);
		if (cautat != NULL)
		{
			put(prim, ultim, cautat->info);
			*rad = stergeDupaCheie(*rad, vector[i]);
		}
	}
}


void main()
{
	Librarie l;
	nodArb* rad = NULL;
	char buffer[20];
	int n;

	FILE* f = fopen("fisier_Librarii.txt", "r");
	fscanf(f, "%d", &n);
	fscanf(f, "%d", &l.idLibrarie);
	while (!feof(f))
	{
		fscanf(f, "%s", buffer);
		l.denumire = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(l.denumire, buffer);
		fscanf(f, "%f", &l.profit);
		
		rad = inserareABC(rad, l);
		fscanf(f, "%d", &l.idLibrarie);
	}

	//1. ABC cu cheie de inserare alfanumerica
	printf("INORDINE: \n\n");
	inordine(rad);

	//vector Librarii frunze
	Librarie* librarii2 = (Librarie*)malloc(sizeof(Librarie) * n);
	int dimLibrarii2 = 0;
	getVectorSortat(rad, librarii2, &dimLibrarii2);
	printf("\nNODURI FRUNZA INITIALE: \n\n");
	for (int i = 0; i < dimLibrarii2; i++)
		afisareLibrarie(librarii2[i]);


	//2. functie de copiere a tuturor librariilor din arborele initial intr-un alt arbore diferit care sa partajeze zone de memorie HEAP
	//filtrarea librariilor se realizeaza pe baza unui camp definit in structura Librarie
	printf("\n\nARBORE CREAT PE BAZA ARBORELUI PRINCIPAL (COPIATE LIBRARIILE CU ID PAR): \n\n");
	nodArb* destinatie = NULL;
	copiereToFromABC(rad, &destinatie);
	printf("INORDINE: \n\n");
	inordine(destinatie);

	//3. functia care sterge librariile din repository pe baza unui anumit criteriu, diferit de cel anterior
	//le insereaza intr-o structura care sa reflecte ordinea in care au fost sterse
	//elementele sunt afisate la consola dupa ce au fost sterse
	nodCoada* prim = NULL;
	nodCoada* ultim = NULL;
	printf("\n\nSTERGERE LIBRARII CU PROFIT < 2500: REZULTAT ARBORE: \n\n");
	char** denumiri = (char**)malloc(sizeof(char*) * n);
	int dim = 0;
	getDenumiri(rad, denumiri, &dim);
	stergereLibrarii(&rad, denumiri, dim, &prim, &ultim);
	inordine(rad);

	printf("\n\nIN COADA: \n");
	afisareCoada(prim);

	//4. functia care insereaza intr-un vector in ordine descrescatoare a cheilor elementele din repository care nu au fii
	Librarie* librarii = (Librarie*)malloc(sizeof(Librarie) * n);
	int dimLibrarii = 0;
	getVectorSortat(rad, librarii, &dimLibrarii);
	printf("\nNODURI FRUNZA DESCNDENT DUPA DENUMIRE: \n\n");
	for (int i = 0; i < dimLibrarii; i++)
		afisareLibrarie(librarii[i]);

	dezalocareABC(rad);
	//dezalocare coada
}