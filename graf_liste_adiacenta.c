#pragma warning(disable:4996)
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>


typedef struct
{
	int id;
	char* den;
} oras;

//LP = lista principala
//LS = lista secundara

typedef struct nodLP; //sa fie vizibil in struct nodLS

typedef struct
{
	struct nodLP* info; //pointer catre nodLP
	struct nodLS* next; 
} nodLS;

typedef struct
{
	oras info;
	struct nodLS* vecini; //pointer catre LS
	struct nodLP* next;
} nodLP;


nodLP* inserareLP(nodLP* cap, oras o)
{
	nodLP* nou = (nodLP*)malloc(sizeof(nodLP));
	nou->info.id = o.id;
	nou->info.den = (char*)malloc((strlen(o.den) + 1) * sizeof(char));
	strcpy(nou->info.den, o.den);
	
	nou->vecini = NULL;
	nou->next = NULL;

	if (cap == NULL)
		cap = nou;
	else
	{
		nodLP* temp = cap;
		while (temp->next)
			temp = temp->next;
		temp->next = nou;
	}
	return cap;
}

nodLS* inserareLS(nodLS* cap, nodLP* info)
{
	nodLS* nou = (nodLS*)malloc(sizeof(nodLS));

	nou->info = info;
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

nodLP* cautareDupaId(nodLP* cap, int id)
{
	nodLP* temp = cap;
	while (temp && temp->info.id != id)
		temp = temp->next;
	return temp;
}

void inserareArcInGraf(nodLP* cap, int idNodStart, int idNodFinal)
{
	if (cap)
	{
		nodLP* idS = cautareDupaId(cap, idNodStart);
		nodLP* idF = cautareDupaId(cap, idNodFinal);
		if (idS && idF)
		{
			//populez listele de vecini
			//daca vreau ca graful sa fie orientat fac doar o inserare idS->...
			idS->vecini = inserareLS(idS->vecini, idF);
			idF->vecini = inserareLS(idF->vecini, idS);
		}
	}
}

void traversareLS(nodLS* cap)
{
	nodLS* temp = cap;
	while (temp)
	{
		//vreau sa afisez listele de vecini
		nodLP* aux = temp->info;
		printf("	%d.%s\n", aux->info.id + 1, aux->info.den); //+1 ptr ca incep cu 0
		temp = temp->next;
	}
}

void traversareLP(nodLP* cap)
{
	nodLP* temp = cap;
	while (temp)
	{
		printf("%d/%s are urmatorii vecini: \n", temp->info.id + 1, temp->info.den);
		traversareLS(temp->vecini);
		temp = temp->next;
	}
}

void dezalocareLS(nodLS* cap)
{
	nodLS* temp = cap;
	while (temp)
	{
		nodLS* temp2 = temp->next;
		free(temp);
		temp = temp2;
	}
}

void dezalocareLP(nodLP* cap)
{
	nodLP* temp = cap;
	while (temp)
	{
		nodLP* temp2 = temp->next;
		if (temp->vecini)
			dezalocareLS(temp->vecini);
		printf("\nam sters: %s", temp->info.den);
		free(temp->info.den);
		free(temp);
		temp = temp2;
	}
}

typedef struct
{
	int id;
	struct nodStiva* next;
} nodStiva;

typedef struct
{
	int id;
	struct nodCoada* next;
} nodCoada;

nodStiva* push(nodStiva* varf, int id)
{
	nodStiva* nou = (nodStiva*)malloc(sizeof(nodStiva));
	nou->id = id;
	nou->next = NULL;

	if (varf == NULL)
		varf = nou;
	else
	{
		nou->next = varf;
		varf = nou;
	}
	return varf;
}

int pop(nodStiva** varf)
{
	if (*varf == NULL)
		return -1;
	else
	{
		int id = (*varf)->id;
		nodStiva* temp = *varf;
		*varf = (*varf)->next;
		free(temp);
		return id;
	}
}

void parcurgereDF(nodLP* graf, int idNodStart, int nrNoduri)
{
	if (graf)
	{
		nodStiva* varf = NULL;

		int* vizitate = (int*)malloc(nrNoduri * sizeof(int));
		for (int i = 0; i < nrNoduri; i++)
			vizitate[i] = 0;
		varf = push(varf, idNodStart);
		vizitate[idNodStart] = 1;

		while (varf)
		{
			int idNodCurent = pop(&varf);
			nodLP* nodCurent = cautareDupaId(graf, idNodCurent);
			printf("%d.%s\n", nodCurent->info.id + 1, nodCurent->info.den);

			nodLS* temp = nodCurent->vecini;
			while (temp)
			{
				//caut vecinii nevizitati
				//ii pun pe stiva
				//ii marchez ca vizitati
				nodLP* aux = temp->info;
				if (vizitate[aux->info.id] == 0)
				{
					varf = push(varf, aux->info.id);
					vizitate[aux->info.id] = 1;
				}
				temp = temp->next;
			}
		}
	}
}

void put(nodCoada** prim, nodCoada** ultim, int val)
{
	nodCoada* nou = (nodCoada*)malloc(sizeof(nodStiva));
	nou->id = val;
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

int get(nodCoada** prim, nodCoada** ultim, int* val)
{
	if (*prim != NULL && *ultim != NULL)
	{
		*val = (*prim)->id;
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

void parcurgereBF(nodLP* graf, int idNodStart, int nrNoduri)
{
	if (graf)
	{
		nodCoada* prim = NULL;
		nodCoada* ultim = NULL;

		int* vizitate = (int*)malloc(nrNoduri * sizeof(int));
		for (int i = 0; i < nrNoduri; ++i)
			vizitate[i] = 0;
		
		put(&prim, &ultim, idNodStart);
		vizitate[idNodStart] = 1;

		while (prim)
		{
			int idNodCurent = 0;
			get(&prim, &ultim, &idNodCurent);
			nodLP* nodCurent = cautareDupaId(graf, idNodCurent);
			printf("%d.%s\n", nodCurent->info.id + 1, nodCurent->info.den);

			nodLS* temp = nodCurent->vecini;
			while (temp)
			{
				//caut vecinii nevizitati
				//ii pun in coada
				//ii marchez ca vizitati
				nodLP* aux = temp->info;
				if (vizitate[aux->info.id] == 0)
				{
					put(&prim, &ultim, aux->info.id);
					vizitate[aux->info.id] = 1;
				}
				temp = temp->next;
			}
		}
	}
}

void main()
{
	FILE* f = fopen("listeAd.txt", "r");
	
	nodLP* graf = NULL;
	int nrNoduri;
	char buffer[20];
	oras o;

	fscanf(f, "%d", &nrNoduri);

	for (int i = 0; i < nrNoduri; i++)
	{
		o.id = i;
		fscanf(f, "%s", buffer);
		o.den = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
		strcpy(o.den, buffer);

		graf = inserareLP(graf, o); //creez lp; momentan vecinii sunt NULL; metoda inserareArcInGraf va popula listele de vecini
	}
	int nrArce, idNodStart, idNodFinal;
	fscanf(f, "%d", &nrArce);
	for (int i = 0; i < nrArce; i++)
	{
		fscanf(f, "%d", &idNodStart);
		fscanf(f, "%d", &idNodFinal);
		inserareArcInGraf(graf, idNodStart, idNodFinal);
	}
	fclose(f);

	traversareLP(graf);

	printf("\nparcurgere in adancime de la nodul 1: \n");
	parcurgereDF(graf, 0, nrNoduri);

	printf("\nparcurgere in latime de la nodul 1: \n");
	parcurgereBF(graf, 0, nrNoduri);

	dezalocareLP(graf);
}