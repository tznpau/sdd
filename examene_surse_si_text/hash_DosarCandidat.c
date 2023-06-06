#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>

//1.
//tabela de dispersie
//mecanism tratare coliziuni: chaining

typedef struct
{
	char* numeCandidat;
	char* programStudiu;
	float medieBac;
	int codDosar;
} DosarCandidat;

typedef struct
{
	DosarCandidat info;
	struct nodLS* next;
} nodLS;

typedef struct
{
	struct nodLS** vect;
	int nrElem;
} hashT;

int functieDispersie(hashT tabela, char cheie[20])
{
	return cheie[0] % tabela.nrElem;
}

void inserareTabela(hashT tabela, DosarCandidat dc)
{
	int poz = 0;
	if (tabela.vect != NULL)
	{
		poz = functieDispersie(tabela, dc.numeCandidat);
		nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
		nou->info.numeCandidat = (char*)malloc((strlen(dc.numeCandidat) + 1) * sizeof(char));
		strcpy(nou->info.numeCandidat, dc.numeCandidat);
		nou->info.programStudiu = (char*)malloc((strlen(dc.programStudiu) + 1) * sizeof(char));
		strcpy(nou->info.programStudiu, dc.programStudiu);
		nou->info.medieBac = dc.medieBac;
		nou->info.codDosar = dc.codDosar;

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
		printf("\nnume candidat: %s, program de studiu: %s, medie bac: %5.2f, cod candidat: %d", temp->info.numeCandidat, temp->info.programStudiu, temp->info.medieBac, temp->info.codDosar);
		temp = temp->next;
	}
}

//2.
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
		free(temp->info.numeCandidat);
		free(temp->info.programStudiu);
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
				dezalocareLS(tabela.vect[i]);
		free(tabela.vect);
	}
}

void stergereNod(nodLS* nod)
{
	free(nod->info.numeCandidat);
	free(nod->info.programStudiu);
	free(nod);
}

int stergere(hashT tabela, char cheie[20])
{
	int poz = 0;
	if (tabela.vect != NULL)
	{
		poz = functieDispersie(tabela, cheie);
		nodLS* temp = tabela.vect[poz];
		if (temp == NULL)
			return -1;
		else
		{
			if (strcmp(temp->info.numeCandidat, cheie) == 0)
			{
				if (temp->next == NULL)
				{
					stergereNod(temp);
					tabela.vect[poz] = NULL;
				}
				else
				{
					nodLS* temp2 = temp;
					tabela.vect[poz] = temp->next;
					stergereNod(temp2);
				}
			}
			else
			{
				while (temp->next != NULL)
				{
					nodLS* temp2 = temp->next;
					if (strcmp(temp2->info.numeCandidat, cheie) == 1)
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
						stergereNod(temp3);
					}
					else
					{
						temp->next = NULL;
						stergereNod(temp3);
					}
				}
				else
					return -1;
				
			}
		}
	}
	return poz;
}

//3.
int nrStudentiProgram(hashT tabela, char programStudiu[20])
{
	int nr = 0;
	if (tabela.vect != NULL)
	{
		for (int i = 0; i < tabela.nrElem; i++)
			if (tabela.vect[i] != NULL)
			{
				nodLS* temp = tabela.vect[i];
				while (temp)
				{
					if (strcmp(temp->info.programStudiu, programStudiu) == 0)
						nr++;
					temp = temp->next;
				}
			}
	}
	return nr;
}

//4.
typedef struct
{
	struct nodLS* info;
	struct nodLP* next;
} nodLP;

void inserareLP(nodLP** capLP, nodLS* capLS)
{
	nodLP* nou = (nodLP*)malloc(sizeof(nodLP));
	nou->info = capLS;
	nou->next = NULL;

	if (*capLP == NULL)
		*capLP = nou;
	else
	{
		nodLP* temp = *capLP;
		while (temp->next)
			temp = temp->next;
		temp->next = nou;
	}
}

void traversareLP(nodLP* cap)
{
	nodLP* temp = cap;
	int i = 1;
	while (temp)
	{
		printf("\nsublsita: %d", i);
		traversareLS(temp->info);
		temp = temp->next;
		i++;
	}
}

void dezalocareLP(nodLP* cap)
{
	nodLP* temp = cap;
	while (temp)
	{
		nodLP* temp2 = temp->next;
		dezalocareLS(temp->info);
		free(temp);
		temp = temp2;
	}
}

void inserareLSMedieBac(nodLS** capLS, DosarCandidat dc, float prag)
{
	if (dc.medieBac < prag)
	{
		nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
		nou->info.numeCandidat = (char*)malloc((strlen(dc.numeCandidat) + 1) * sizeof(char));
		strcpy(nou->info.numeCandidat, dc.numeCandidat);
		nou->info.programStudiu = (char*)malloc((strlen(dc.programStudiu) + 1) * sizeof(char));
		strcpy(nou->info.programStudiu, dc.programStudiu);
		nou->info.medieBac = dc.medieBac;
		nou->info.codDosar = dc.codDosar;

		nou->next = NULL;

		if (*capLS == NULL)
			*capLS = nou;
		else
		{
			nodLS* temp = *capLS;
			while (temp->next)
				temp = temp->next;
			temp->next = nou;
		}
	}
}

//5.
//functia care sterge din lista de liste candidatii cu cea mai mica medie de bac
//NU functioneaza aceasta cerinta
nodLS* candidatMedieMinima(nodLS* cap)
{
	nodLS* temp = cap;
	nodLS* min = cap;
	while (temp)
	{
		if (min->info.medieBac > temp->info.medieBac)
			min = temp;
		temp = temp->next;
	}
	return min;
}

nodLS* medieMin(nodLS* a, nodLS* b)
{
	nodLS* min = a;
	if (min > b)
		min = b;
	return min;
}

void stergereLS(nodLS** cap, char nume[20])
{
	if (*cap == NULL)
		return;
	if (strcmp((*cap)->info.numeCandidat, nume) == 0)
	{
		nodLS* temp = *cap;
		*cap = (*cap)->next;
		free(temp->info.numeCandidat);
		free(temp->info.programStudiu);
		free(temp);
		printf("\ncandidatul %s a fost sters\n", nume);
		return;
	}
	nodLS* curent = *cap;
	nodLS* anterior = NULL;
	while (curent != NULL && (strcmp(curent->info.numeCandidat, nume) == 1))
	{
		anterior = curent;
		curent = curent->next;
	}
	if (curent == NULL)
	{
		printf("\neroare - nu a fost gasit candidatul in lista\n");
		return;
	}
	anterior->next = curent->next;
	free(curent->info.numeCandidat);
	free(curent->info.programStudiu);
	free(curent);
	printf("\ncandidatul %s a fost sters\n", nume);
}

void main()
{
	hashT tabela;
	tabela.nrElem = 31;
	tabela.vect = (nodLS**)malloc(tabela.nrElem * sizeof(nodLS*));
	for (int i = 0; i < tabela.nrElem; i++)
		tabela.vect[i] = NULL;

	int n;
	char buffer[20];
	DosarCandidat dc;

	nodLP* capLP = NULL;
	nodLS* capLS_Info = NULL;
	nodLS* capLS_Stat = NULL;

	FILE* f = fopen("fisier_Dosare.txt", "r");
	fscanf(f, "%d", &n);
	for (int i = 0; i < n; i++)
	{
		fscanf(f, "%s", buffer);
		dc.numeCandidat = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(dc.numeCandidat, buffer);
		fscanf(f, "%s", buffer);
		dc.programStudiu = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(dc.programStudiu, buffer);
		fscanf(f, "%f", &dc.medieBac);
		fscanf(f, "%d", &dc.codDosar);

		inserareTabela(tabela, dc);

		if (strcmp(dc.programStudiu, "Informatica") == 0)
			inserareLSMedieBac(&capLS_Info, dc, 9.3);
		else
			inserareLSMedieBac(&capLS_Stat, dc, 9.3);

		free(dc.numeCandidat);
		free(dc.programStudiu);
	}
	fclose(f);

	printf("\ntabela de dispersie: \n");

	traversareTabela(tabela);

	printf("\n\nnumarul de candidati cu programul de studiu Statistica licenta: %d", nrStudentiProgram(tabela, "Statistica"));

	printf("\n\nlista de liste\n");
	inserareLP(&capLP, capLS_Info);
	inserareLP(&capLP, capLS_Stat);
	traversareLP(capLP);

	nodLS* minim = medieMin(candidatMedieMinima(capLS_Info), candidatMedieMinima(capLS_Stat));
	printf("\n\ncandidatul cu medie de bac minima este: %s, cu media %5.2f", minim->info.numeCandidat, minim->info.medieBac);
	//if (strcmp(minim->info.programStudiu, "Informatica") == 0)
	//	stergereLS(&capLS_Info, minim->info.numeCandidat);
	//else
	//	stergereLS(&capLS_Stat, minim->info.numeCandidat);


	dezalocareTabela(tabela);
	dezalocareLP(capLP);
}