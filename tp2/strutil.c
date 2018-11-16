#define _POSIX_C_SOURCE 200809L
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strutil.h"

// Función auxiliar
int apariciones(const char* str, size_t largo, char k) {
	int resultado = 0;
	for (size_t i = 0; i < largo; i++)
		if (str[i] == k) resultado++;
	return resultado;
}

char** split(const char* str, char sep) {
	size_t largo = strlen(str);
	size_t tam = apariciones(str, largo, sep)+2;
	char** resultado = malloc(tam * sizeof(char*));
	if (!resultado) return NULL;
	size_t pos = 0, act = 0, inicio = 0;
	for (size_t i = 0; i <= largo; i++) {
		act++;
		if (str[i] == sep || str[i] == '\0') {
			resultado[pos] = strndup(str+inicio, (act-1)*sizeof(char));
			pos++;
			inicio += act;
			act = 0;
		}
	}
	resultado[pos] = NULL;
	return resultado;
}

char* join(char** strv, char sep) {
	if (sep == '\0') return NULL;
	size_t largo = 0;
	for (int i = 0; strv[i] != NULL; i++)
		largo += strlen(strv[i]) + 1;
	if (largo == 0) largo = 1;
	char* resultado = malloc(largo * sizeof(char));
	if (!resultado) return NULL;
	int j = 0, k = 0;
	for (int i = 0; i < largo-1; i++) {
		if (strv[k][j] != '\0') {
			resultado[i] = strv[k][j];
			j++;
		} else {
			k++;
			j = 0;
			resultado[i] = sep;
		}
	}
	resultado[largo-1] = '\0';
	return resultado;
}

void free_strv(char* strv[]) {
	for (int i = 0; strv[i] != NULL; i++)
		free(strv[i]);
	free(strv);
}