#define _POSIX_C_SOURCE 200809L
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "strutil.h"
#include "sistema.h"
#include "hash.h"
#include "abb.h"
#include "heap.h"

/* Estructura del TDA vuelo */

struct vuelo{
	char* codigo;
	int	prioridad;
	char* hora;
	char* info;
};

/* Primitivas del TDA vuelo */

vuelo_t* vuelo_crear(char* codigo, int prioridad, char* hora, char* datos[]){
	vuelo_t* vuelo = malloc(vuelo_t);
	if (!vuelo) return NULL;
	vuelo->codigo = strdup(codigo);
	vuelo->prioridad = prioridad;
	vuelo->hora = strdup(hora);
	vuelo->info = join(datos, ' ');
	return vuelo;
}

const char* vuelo_info(vuelo_t* vuelo){
	return vuelo->info;
}

const char* vuelo_codigo(const vuelo_t* vuelo) {
	return vuelo->codigo;
}

int vuelo_prioridad(const vuelo_t* vuelo) {
	return vuelo->prioridad;
}

void vuelo_destruir(void* vuelo) {
	free(((vuelo_t*)vuelo)->codigo);
	free(((vuelo_t*)vuelo)->hora);
	free(((vuelo_t*)vuelo)->info);
	free(vuelo);
}

// La función de comparación está invertida para que el heap
// funcione como un heap de mínimos, pero capaz sería mejor modificar
// directamente el heap para que quede más claro
int vuelocmp(const void* a, const void* b) {
	if (((vuelo_t*)a)->prioridad > ((vuelo_t*)b)->prioridad)
		return -1;
	if (((vuelo_t*)a)->prioridad < ((vuelo_t*)b)->prioridad)
		return 1;
	if (strcmp(((vuelo_t*)a)->codigo, ((vuelo_t*)b)->codigo) > 0)
		return -1;
	return 1;
}

/* Definición del TDA sistema */

struct sistema {
	hash_t* hash;
	abb_t* abb;
};

/* Primitivas del TDA sistema */

sistema_t* sistema_crear(void) {
	sistema_t* sistema = malloc(sizeof(sistema_t));
	if (!sistema) return NULL;
	sistema->hash = hash_crear(vuelo_destruir);
	sistema->abb = abb_crear(strcmp, NULL);
	return sistema;
}

bool sistema_agregar_vuelo(sistema_t* sistema, vuelo_t* vuelo) {
	hash_guardar(sistema->hash, vuelo->codigo, vuelo);
	abb_guardar(sistema->abb, vuelo->hora, vuelo);
	return true;
}

vuelo_t* sistema_ver_vuelo(const sistema_t* sistema, const char* codigo) {
	return hash_obtener(sistema->hash, codigo);
}

heap_t* sistema_prioridades(const sistema_t* sistema, int k) {
	heap_t* heap = heap_crear(vuelocmp);
	hash_iter_t* iter = hash_iter_crear(sistema->hash);
	for (int i = 0; i < k; i++) {
		if (hash_iter_al_final(iter))
			break;
		vuelo_t* vuelo = sistema_ver_vuelo(sistema, hash_iter_ver_actual(iter));
		heap_encolar(heap, vuelo);
		hash_iter_avanzar(iter);
	}
	while (!hash_iter_al_final(iter)) {
		vuelo_t* vuelo = sistema_ver_vuelo(sistema, hash_iter_ver_actual(iter));
		if (vuelocmp(vuelo, (vuelo_t*) heap_ver_max(heap)) < 0) {
			heap_desencolar(heap);
			heap_encolar(heap, vuelo);
		}
		hash_iter_avanzar(iter);
	}
	hash_iter_destruir(iter);
	return heap;
}

void sistema_destruir(sistema_t* sistema) {
	hash_destruir(sistema->hash);
	abb_destruir(sistema->abb);
	free(sistema);
}