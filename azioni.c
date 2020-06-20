#include "azioni.h"


bool canAddAzione(azione *a, unsigned char index, gruppo_azioni *curr, float budget) {
	if (budget - curr->costo >= a[index].CostoUnitario*a[index].Numero) {
		return true;
	}

	return false;
}


void addAzione(azione *a, unsigned char index, gruppo_azioni *curr, bool *azioni_usate) {
	curr->soluzione[curr->n_elementi] = index;
	curr->n_elementi++;
	curr->costo += a[index].CostoUnitario * a[index].Numero;
	curr->guadagno += a[index].Guadagno;
	azioni_usate[index] = true;
}

void deleteAzione(azione *a, unsigned char index, gruppo_azioni *curr, bool *azioni_usate) {
	curr->n_elementi--;
	curr->costo -= a[index].CostoUnitario * a[index].Numero;
	curr->guadagno -= a[index].Guadagno;
	azioni_usate[index] = false;	
}



void PrintSoluzione(azione *a, size_t n, gruppo_azioni *best) {
	printf("\nEcco una soluzione ottimale: \n");
	for (size_t i = 0; i < best->n_elementi; i++) {
		size_t index = best->soluzione[i];
		printf("%d)\tNumero: %d;\tCostoUnitario: %.2f; \tGuadagno: %.2f\n",
			i + 1, a[i].Numero, a[i].CostoUnitario, a[i].Guadagno);
	}
	printf("\nCostoTotale: %f\nGuadagnoTotale: %f\n", best->costo, best->guadagno);
}

void updateBest(gruppo_azioni *best, gruppo_azioni *curr, size_t n) {
	best->n_elementi = curr->n_elementi;
	best->guadagno = curr->guadagno;
	best->costo = curr->costo;
	memcpy(best->soluzione, curr->soluzione, n * sizeof(char));
}


void Backtrack(int i, int n, azione *a, gruppo_azioni *curr, gruppo_azioni *best, bool *azioni_usate, float budget) {
	if (i == n) {
		if (best->guadagno < curr->guadagno) {
			updateBest(best, curr, n);
		}
		return;
	}
	for (unsigned char j = 0; j < n; j++) {
		if (azioni_usate[j] == false) {
			if (canAddAzione(a, j, curr, budget)) {
				addAzione(a, j, curr, azioni_usate);
				Backtrack(i+1, n, a, curr, best, azioni_usate, budget);
				deleteAzione(a, j, curr, azioni_usate);
			}
			else {
				Backtrack(n, n, a, curr, best, azioni_usate, budget);
			}
		}

	}
}

void setEnviroment(bool **azioni_usate, gruppo_azioni *best, gruppo_azioni *curr, size_t n) {
	*azioni_usate = calloc(n, sizeof(bool));
	best->soluzione = malloc(n * sizeof(char));
	curr->soluzione = malloc(n * sizeof(char));

	best->costo = 0;
	best->guadagno = 0;
	best->n_elementi = 0;
	
	curr->costo = 0;
	curr->guadagno = 0;
	curr->n_elementi = 0;

}

void unsetEnviroment(bool *a, gruppo_azioni *b, gruppo_azioni *c) {
	free(a);
	free(b->soluzione);
	free(c->soluzione);
}

void WriteAzioniBin(FILE *f, azione *a, size_t n) {
	fwrite(&n, sizeof(size_t), 1, f);
	for (size_t i = 0; i < n; i++) {
		fwrite(&a[i].Numero, sizeof(int), 1, f);
		fwrite(&a[i].CostoUnitario, sizeof(float), 1, f);
		fwrite(&a[i].Guadagno, sizeof(float), 1, f);
	}
}

azione *ReadAzioniBin(FILE *f, size_t *n) {
	azione *a = NULL;
	size_t letti = fread(n, sizeof(size_t), 1, f);
	if (letti == 1) {
		a = malloc(*n * sizeof(azione));
		size_t i = 0;
		while (1) {
			letti = fread(&a[i].Numero, sizeof(int), 1, f);
			letti += fread(&a[i].CostoUnitario, sizeof(float), 1, f);
			letti += fread(&a[i].Guadagno, sizeof(float), 1, f);
			i++;
			if (letti != 3) {
				if (ferror(f)) {
					free(a);
					a = NULL;
					break;
				}
				if (feof(f)) {
					break;
				}
				break;
			}
		}

	}
	return a;
}




void MassimizzaGuadagnoAzioni(azione *a, size_t n, float budget) {
	gruppo_azioni best;
	gruppo_azioni curr;
	bool *azioni_usate = NULL;


	setEnviroment(&azioni_usate, &best, &curr, n);

	Backtrack(0, n, a, &curr, &best, azioni_usate, budget);

	PrintSoluzione(a, n, &best);

	unsetEnviroment(azioni_usate, &best, &curr);
}