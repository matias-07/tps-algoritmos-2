#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strutil.h"
#include "sistema.h"

// Posiciones de cada dato en el csv
#define CODIGO 0
#define PRIORIDAD 5
#define HORA 6

vuelo_t* procesar_linea(char* linea) {
	char** datos = split(linea, ',');
	char* codigo = datos[CODIGO];
	int prioridad = atoi(datos[PRIORIDAD]);
	char* hora = datos[HORA];
	char* info = linea;
	vuelo_t* vuelo = vuelo_crear(codigo, prioridad, hora, info);
	free_strv(datos);
	return vuelo;
}

bool agregar_archivo(sistema_t* sistema, char* comando[]) {
	FILE* archivo = fopen(comando[1], "r");
	if (!archivo) return false;
	char* linea = NULL;
	size_t tam = 0;
	while (getline(&linea, &tam, archivo) > 0) {
		vuelo_t* vuelo = procesar_linea(linea);
		sistema_agregar_vuelo(sistema, vuelo);
	}
	free(linea);
	fclose(archivo);
	return true;
}

bool info_vuelo(const sistema_t* sistema, char* comando[]) {
	vuelo_t* vuelo = sistema_ver_vuelo(sistema, comando[1]);
	if (!vuelo) return false;
	printf("%s\n", vuelo_obtener_info(vuelo));
	return true;
}

bool procesar_comando(sistema_t* sistema, char* entrada) {
	if (!entrada) return false;
	bool ok = false;
	char** comando = split(entrada, ' ');
	if (strcmp(comando[0], "agregar_archivo") == 0)
		ok = agregar_archivo(sistema, comando);
	else if (strcmp(comando[0], "ver_tablero") == 0)
		ok = ver_tablero(sistema, comando);
	else if (strcmp(comando[0], "info_vuelo") == 0)
		ok = info_vuelo(sistema, comando);
	else if (strcmp(comando[0], "prioridad_vuelos") == 0)
		ok = prioridad_vuelos(sistema, comando);
	else if (strcmp(comando[0], "borrar") == 0)
		ok = borrar(sistema, comando);
	free_strv(comando);
	return ok;
}

/* Función principal del programa. */
int main(void) {
	sistema_t* sistema = sistema_crear();
	char* entrada = NULL;
	size_t tam = 0;
	size_t cant;
	while ((cant = getline(&entrada, &tam, stdin)) != EOF) {
		if (entrada[cant-1] == '\n') // Agrego esto porque getline da problemas si hay un \n al final
			entrada[cant-1] = '\0';
		if (procesar_comando(sistema, entrada)) {
			printf("OK\n");
		} else {
			fprintf(stderr, "Error en comando %s\n", entrada);
		}
	}
	sistema_destruir(sistema);
	return 0;
}