#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>

typedef struct
{
	unsigned int cod;
	char* titlu;
	char* beneficiar;
	unsigned char nrExecutanti;
	float buget;
} Proiect;

typedef struct
{
	Proiect info;
	struct nodLS* next;
} nodLS;

typedef struct
{
	nodLS** vect;
	int nrElem;
} hashT;

Proiect deepCopyProiect(Proiect p)
{
	Proiect copie;
	copie.cod = p.cod;
	copie.titlu = (char*)malloc((strlen(p.titlu) + 1) * sizeof(char));
	strcpy(copie.titlu, p.titlu);
	copie.beneficiar = (char*)malloc((strlen(p.beneficiar) + 1) * sizeof(char));
	strcpy(copie.beneficiar, p.beneficiar);
	copie.nrExecutanti = p.nrExecutanti;
	copie.buget = p.buget;
	return copie;
}

void afisareProiect(Proiect p)
{
	printf("\tCod: %u, Beneficiar: %s\n", p.cod, p.beneficiar);
}

int functieDispersie(hashT tabela, char cheie[20])
{
	return cheie[0] % tabela.nrElem;
}

void inserareHash(hashT tabela, Proiect p)
{
	int poz = 0;
	if (tabela.vect != NULL)
	{
		poz = functieDispersie(tabela, p.beneficiar);

		nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
		nou->info = deepCopyProiect(p);
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
		afisareProiect(temp->info);
		temp = temp->next;
	}
}

void dezalocareLS(nodLS* cap)
{
	nodLS* temp = cap;
	while (temp)
	{
		nodLS* temp2 = temp->next;
		free(temp->info.titlu);
		free(temp->info.beneficiar);
		free(temp);
		temp = temp2;
	}
}

void traversareTabela(hashT tabela)
{
	if (tabela.vect != NULL)
	{
		for (int i = 0; i < tabela.nrElem; i++)
			if (tabela.vect[i] != NULL)
			{
				printf("\nPozitie: %d\n", i);
				traversareLS(tabela.vect[i]);
			}
	}
}

void dezalocareTabela(hashT tabela)
{
	if (tabela.vect != NULL)
	{
		for (int i = 0; i < tabela.nrElem; i++)
			dezalocareLS(tabela.vect[i]);
		free(tabela.vect);
	}
}

float bugetTotal(hashT tabela, const char* beneficiar)
{
	float suma = 0;
	if (tabela.vect != NULL)
	{
		for (int i = 0; i < tabela.nrElem; i++)
		{
			nodLS* temp = tabela.vect[i];
			while (temp)
			{
				if (strcmp(temp->info.beneficiar, beneficiar) == 0)
					suma += temp->info.buget;
				temp = temp->next;
			}
		}
	}
	return suma;
}


void schimbaBeneficiar(hashT* tabela, const char* beneficiar, const char* beneficiarNou)
{
	if ((*tabela).vect != NULL)
	{
		for (int i = 0; i < (*tabela).nrElem; i++)
		{
			nodLS* temp = (*tabela).vect[i];
			while (temp)
			{
				if (strcmp(temp->info.beneficiar, beneficiar) == 0)
				{
					free(temp->info.beneficiar);
					temp->info.beneficiar = (char*)malloc((strlen(beneficiarNou) + 1) * sizeof(char));
					strcpy(temp->info.beneficiar, beneficiarNou);
				}
				temp = temp->next;
			}
		}
	}
}

//pentru inserare in lista
nodLS* inserareLS(nodLS* cap, Proiect p)
{
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));
	nou->info = deepCopyProiect(p);
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

void copiereInLS(nodLS** cap, hashT tabela, int prag)
{
	if (tabela.vect != NULL)
	{
		for (int i = 0; i < tabela.nrElem; i++)
			if (tabela.vect[i] != NULL)
			{
				nodLS* temp = tabela.vect[i];
				while (temp)
				{
					if (temp->info.buget > prag)
						*cap = inserareLS(*cap, temp->info);
					temp = temp->next;
				}
			}
	}
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
	Proiect p;

	FILE* f = fopen("fisier_Proiecte.txt", "r");
	fscanf(f, "%d", &n);
	for (int i = 0; i < n; i++)
	{
		fscanf(f, "%u", &p.cod);
		fscanf(f, "%s", buffer);
		p.titlu = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(p.titlu, buffer);
		fscanf(f, "%s", buffer);
		p.beneficiar = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(p.beneficiar, buffer);
		fscanf(f, "%hhu", &p.nrExecutanti);
		fscanf(f, "%f", &p.buget);

		inserareHash(tabela, p);

		free(p.titlu);
		free(p.beneficiar);
	}
	fclose(f);

	//1 + 2. tabela de dispersie
	printf("\nTabela de dispersie: \n\n");
	traversareTabela(tabela);

	//3. functie care calculeaza bugetul total al unui beneficiar introdus la tastatura
	float bugetAna = bugetTotal(tabela, "Ana");
	printf("\nBuget total proiecte Ana: %5.2f\n\n", bugetAna);

	//4. functie ce schimba beneficiarul
	schimbaBeneficiar(&tabela, "Ana", "Anna");
	printf("\nSchimbarea beneficiarului Ana in Anna: \n\n");
	traversareTabela(tabela);

	//5. salveaza intr-o lista simpla poriectele cu buget peste 500 lei
	printf("\nConversie in LSI: \n\n");
	nodLS* cap = NULL;
	copiereInLS(&cap, tabela, 500);
	traversareLS(cap);

	dezalocareLS(cap);
	dezalocareTabela(tabela);
}