#include "azioni.h"

int main(void) {
	FILE *f = fopen("azioni.dat", "rb");
	if (f != NULL) {
		size_t n = 0;
		azione *a = ReadAzioniBin(f, &n);
		fclose(f);

		if (a != NULL) {
			float budget = 100;

			printf("Azioni disponibili nel mercato:\n");
			for (size_t i = 0; i < n; i++) {
				printf("%d)\tNumero: %d;\tCostoUnitario: %.2f; \tGuadagno: %.2f\n",
					i + 1, a[i].Numero, a[i].CostoUnitario, a[i].Guadagno);
			}
			printf("Con un budget di %f euro\n", budget);
			MassimizzaGuadagnoAzioni(a, n, budget);
			free(a);
		}
	}


	return 0;
}
