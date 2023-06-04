#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>

typedef struct
{
	int cod;
	char* nume;
	int nrCredite;
} curs;

typedef struct
{
	int BF;
	curs info;
	struct nodArb* left, * right;
} nodArb;

typedef struct
{
	curs info;
	struct nodLS* next;
} nodLS;

nodArb* creareNod(nodArb* st, nodArb* dr, curs c)
{
	nodArb* nou = (nodArb*)malloc(sizeof(nodArb));
	nou->info.cod = c.cod;
	nou->info.nume = (char*)malloc((strlen(c.nume) + 1) * sizeof(char));
	strcpy(nou->info.nume, c.nume);
	nou->info.nrCredite = c.nrCredite;

	nou->left = st;
	nou->right = dr;

	return nou;
}

nodArb* inserareArb(nodArb* rad, curs c)
{
	if (rad != NULL)
	{
		if (c.cod < rad->info.cod)
		{
			rad->left = inserareArb(rad->left, c);
			return rad;
		}
		else
		{
			if (c.cod > rad->info.cod)
			{
				rad->right = inserareArb(rad->right, c);
				return rad;
			}
			else
				return rad;
		}
	}
	else
		return creareNod(NULL, NULL, c);
}

//RSD = rad st dr
void preordine(nodArb* rad)
{
	if (rad != NULL)
	{
		printf("\ncod curs: %d, curs: %s, nr credite: %d; BF = %d", rad->info.cod, rad->info.nume, rad->info.nrCredite, rad->BF);
		preordine(rad->left);
		preordine(rad->right);
	}
}

//SRD = st rad dr
void inordine(nodArb* rad)
{
	if (rad != NULL)
	{
		inordine(rad->left);
		printf("\ncod curs: %d, curs: %s, nr credite: %d; BF = %d", rad->info.cod, rad->info.nume, rad->info.nrCredite, rad->BF);
		inordine(rad->right);
	}
}

//SDR = st dr rad
void postordine(nodArb* rad)
{
	if (rad != NULL)
	{
		postordine(rad->left);
		postordine(rad->right);
		printf("\ncod curs: %d, curs: %s, nr credite: %d; BF = %d", rad->info.cod, rad->info.nume, rad->info.nrCredite, rad->BF);
	}
}

void dezalocare(nodArb* rad)
{
	if (rad != NULL)
	{
		nodArb* st = rad->left;
		nodArb* dr = rad->right;
		free(rad->info.nume);
		free(rad);
		dezalocare(st);
		dezalocare(dr);
	}
}

void cateNoduriFrunzaSunt(nodArb* rad, int* nr)
{
	if (rad != NULL)
	{
		if (rad->left == NULL && rad->right == NULL)
			(*nr)++;

		cateNoduriFrunzaSunt(rad->left, nr);
		cateNoduriFrunzaSunt(rad->right, nr);
	}
}

//sa se determine cursurile care au mai mult de ... credite
void cursuriNrCredite(nodArb* rad, curs* vect, int* nr, int prag)
{
	if (rad != NULL)
	{
		if (rad->info.nrCredite >= prag)
		{
			vect[*nr] = rad->info;
			(*nr)++;
		}

		cursuriNrCredite(rad->left, vect, nr, prag);
		cursuriNrCredite(rad->right, vect, nr, prag);
	}
}

nodArb* cautare(nodArb* rad, int cheie)
{
	if (rad != NULL)
	{
		if (cheie == rad->info.cod)
			return rad;
		else
			if (cheie < rad->info.cod)
				return cautare(rad->left, cheie);
			else
				return cautare(rad->right, cheie);
	}
	else
		return NULL;
}

nodLS* inserareLS(nodLS* cap, curs c)
{
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->info.cod = c.cod;
	nou->info.nume = (char*)malloc((strlen(c.nume) + 1) * sizeof(char));
	strcpy(nou->info.nume, c.nume);
	nou->info.nrCredite = c.nrCredite;
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

void traversareLS(nodLS* cap)
{
	nodLS* temp = cap;
	while (temp)
	{
		printf("\ncod curs: %d, curs: %s, nr credite: %d", temp->info.cod, temp->info.nume, temp->info.nrCredite);
		temp = temp->next;
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

void salvareLSNrCredite(nodArb* rad, nodLS** cap, int prag)
{
	if (rad != NULL)
	{
		if (rad->info.nrCredite >= prag)
			*cap = inserareLS(*cap, rad->info);

		salvareLSNrCredite(rad->left, cap, prag);
		salvareLSNrCredite(rad->right, cap, prag);
	}
}

int maxim(a, b)
{
	int max = a;
	if (max < b)
		max = b;
	return max;
}

//determinare inaltime arbore
int inaltimeArb(nodArb* rad)
{
	if (rad != NULL)
		return 1 + maxim(inaltimeArb(rad->left), inaltimeArb(rad->right));
	else
		return 0;
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
	free(aux->info.nume);
	free(aux);
	return rad;
}

nodArb* stergeDupaCheie(nodArb* rad, int cheie)
{
	if (rad != NULL)
	{
		if (cheie < rad->info.cod)
			rad->left = stergeDupaCheie(rad->left, cheie);
		else
			if (cheie > rad->info.cod)
				rad->right = stergeDupaCheie(rad->right, cheie);
			else
				rad = stergeRad(rad);
		return rad;
	}
	else
		return NULL;
}

nodArb* stergeDupaNume(nodArb* rad, char* n)
{
	if (rad != NULL)
	{
		if (strcmp(rad->info.nume, n) == 0)
			rad = stergeDupaCheie(rad, rad->info.cod);
		else
		{
			rad->left = stergeDupaNume(rad->left, n);
			rad->right = stergeDupaNume(rad->right, n);
		}
		return rad;
	}
	else
		return NULL;
}

//ECHILIBRAREA ARBORILOR
void calculBF(nodArb* rad)
{
	if (rad != NULL)
	{
		rad->BF = inaltimeArb(rad->right) - inaltimeArb(rad->left);
		calculBF(rad->left);
		calculBF(rad->right);
	}
}

//rotatii
nodArb* rotatieDr(nodArb* rad)
{
	printf("\nrotatie dreapta!");
	nodArb* nod1 = rad->left; //nod1 = pivot rotatie la dreapta
	rad->left = nod1->right;
	nod1->right = rad;
	rad = nod1;
	return rad;
}

nodArb* rotatieSt(nodArb* rad)
{
	printf("\nrotatie stanga!");
	nodArb* nod1 = rad->right; //nod1 = pivot rotatie la stanga
	rad->right = nod1->left;
	nod1->left = rad;
	rad = nod1;
	return rad;
}

nodArb* rotatieStDr(nodArb* rad)
{
	printf("\nrotatie stanga dreapta!");
	nodArb* nod1 = rad->left;
	nodArb* nod2 = nod1->right;
	nod1->right = nod2->left;
	nod2->left = nod1;

	rad->left = nod2->right;
	nod2->right = rad;
	rad = nod2;
	return rad;
}

nodArb* rotatieDrSt(nodArb* rad)
{
	printf("\nrotatie dreapta stanga!");
	nodArb* nod1 = rad->right;
	nodArb* nod2 = nod1->left;
	nod1->left = nod2->right;
	nod2->right = nod1;

	rad->right = nod2->left;
	nod2->left = rad;
	rad = nod2;
	return rad;
}

//dupa rotatie trebuie sa modific indicatorii BF pentru ca ei iau alte valori
//reechilibrare
nodArb* reechilibrare(nodArb* rad)
{
	calculBF(rad);
	nodArb* st = rad->left;
	nodArb* dr = rad->right;

	if (rad->BF <= -2 && st->BF <= -1)
	{
		rad = rotatieDr(rad);
		calculBF(rad);
	}
	else
		if (rad->BF >= 2 && dr->BF >= 1)
		{
			rad = rotatieSt(rad);
			calculBF(rad);
		}
		else
			if (rad->BF <= -2 && st->BF >= 1)
			{
				rad = rotatieStDr(rad);
				calculBF(rad);
			}
			else
				if (rad->BF >= 2 && dr->BF <= -1)
				{
					rad = rotatieDrSt(rad);
					calculBF(rad);
				}
	return rad;
}

void main()
{
	nodArb* rad = NULL;
	curs c;
	char buffer[20];
	int nrCursuri;

	FILE* f = fopen("cursuri_arbore.txt", "r");
	fscanf(f, "%d", &nrCursuri);
	for (int i = 0; i < nrCursuri; i++)
	{
		fscanf(f, "%d", &c.cod);
		fscanf(f, "%s", buffer);
		c.nume = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(c.nume, buffer);
		fscanf(f, "%d", &c.nrCredite);

		rad = inserareArb(rad, c);
		free(c.nume);

		rad = reechilibrare(rad);
	}
	fclose(f);

	//calculBF(rad);

	printf("\n\npreordine\n");
	preordine(rad);
	printf("\n\ninordine\n");
	inordine(rad->left);
	printf("\n\npostordine\n");
	postordine(rad->right);

	printf("\n\ncate frunze are arborele?\n");
	int nrFrunze = 0;
	cateNoduriFrunzaSunt(rad, &nrFrunze);
	printf("\narborele are %d frunze", nrFrunze);

	printf("\n\nin functie de credite:\n");
	int nr = 0; //index si contor
	curs* vect = (curs*)malloc(nrCursuri * sizeof(curs));
	cursuriNrCredite(rad, vect, &nr, 4);
	for (int i = 0; i < nr; i++)
		printf("\ncod curs: %d, curs: %s, nr credite: %d", vect[i].cod, vect[i].nume, vect[i].nrCredite);
	free(vect);

	printf("\n\ncautare curs:\n");
	nodArb* nodCautat = cautare(rad, 20);
	if (nodCautat != NULL)
		printf("cursul cautat se numeste %s", nodCautat->info.nume);
	else
		printf("\ncursul nu exista");

	printf("\n\nlista:\n");
	nodLS* cap = NULL;
	salvareLSNrCredite(rad, &cap, 4);
	traversareLS(cap);
	dezalocareLS(cap);

	printf("\n\ninaltimi:\n");
	printf("\ninaltime arbore: %d", inaltimeArb(rad));
	printf("\ninaltime subarbore stang: %d", inaltimeArb(rad->left));
	printf("\ninaltime subarbore drept: %d", inaltimeArb(rad->right));

	printf("\n\nstergere:\n");
	/*rad = stergeRad(rad);
	rad = stergeDupaCheie(rad, 50);*/
	rad = stergeDupaNume(rad, "PAW");
	printf("\n\npreordine\n");
	preordine(rad);
	printf("\n\ninordine\n");
	inordine(rad->left);
	printf("\n\npostordine\n");
	postordine(rad->right);

	dezalocare(rad);
}