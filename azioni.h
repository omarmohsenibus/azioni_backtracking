/**
 * @file: azioni.h
 * @brief: 
 * @name: Omar Mohseni
 * @date: 20 jun 2020
 * 
*/

#define _CRT_SECURE_NO_WARNINGS
#if !defined AZIONI_H
#define AZIONI_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef struct {
	int Numero;
	float CostoUnitario;
	float Guadagno;
} azione;

typedef struct {
	size_t n_elementi;
	char *soluzione;
	float costo;
	float guadagno;
} gruppo_azioni;

extern void MassimizzaGuadagnoAzioni(azione *azioni, size_t n_azioni, float budget);
extern void WriteAzioniBin(FILE *f, azione *a, size_t n);
extern azione *ReadAzioniBin(FILE *f, size_t *n);

#endif //AZIONI_H

