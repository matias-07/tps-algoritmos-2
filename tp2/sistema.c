#define _POSIX_C_SOURCE 200809L
#include <stdbool.h>
#include <stdio.h>
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
	vuelo_t* vuelo = malloc(sizeof(vuelo_t));
	if (!vuelo) return NULL;
	vuelo->codigo = strdup(codigo);
	vuelo->prioridad = prioridad;
	vuelo->hora = strdup(hora);
	vuelo->info = join(datos, ' ');
	return vuelo;
}

const char* vuelo_info(const vuelo_t* vuelo) {
	return vuelo->info;
}

const char* vuelo_codigo(const vuelo_t* vuelo) {
	return vuelo->codigo;
}

const char* vuelo_hora(const vuelo_t* vuelo) {
	return vuelo->hora;
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

int vuelocmp(const void* a, const void* b) {
	if (((vuelo_t*)a)->prioridad > ((vuelo_t*)b)->prioridad)
		return -1;
	if (((vuelo_t*)a)->prioridad < ((vuelo_t*)b)->prioridad)
		return 1;
	if (strcmp(((vuelo_t*)a)->codigo, ((vuelo_t*)b)->codigo) > 0)
		return 1;
	return -1;
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
	return abb_guardar(sistema->abb, vuelo->hora, vuelo) && hash_guardar(sistema->hash, vuelo->codigo, vuelo);
}

vuelo_t* sistema_ver_vuelo(const sistema_t* sistema, const char* codigo) {
	return hash_obtener(sistema->hash, codigo);
}

lista_t* sistema_obtener_vuelos(sistema_t* sistema, char* desde, char* hasta, char* modo) {
	lista_t* vuelos = lista_crear();
	abb_iter_t* iter = abb_iter_in_crear(sistema->abb, desde, hasta);
	while (!abb_iter_in_al_final(iter)) {
		if (strcmp(modo, "desc")){
			lista_insertar_ultimo(vuelos, abb_iter_in_ver_actual(iter));
		} else {
			lista_insertar_primero(vuelos, abb_iter_in_ver_actual(iter));
		}
		abb_iter_in_avanzar(iter);
	}
	abb_iter_in_destruir(iter);
	return vuelos;
}

lista_t* sistema_ver_tablero(sistema_t* sistema, int k, char* modo, char* desde, char* hasta){
	lista_t* vuelos = sistema_obtener_vuelos(sistema, desde, hasta, modo);
	lista_t* vuelos_tablero = lista_crear();
	lista_iter_t* iter = lista_iter_crear(vuelos);
	int cont = 0;
	while(!lista_iter_al_final(iter)){
		lista_insertar_ultimo(vuelos_tablero, lista_iter_ver_actual(iter));
		lista_iter_avanzar(iter);
		cont++;
		if (cont >= k) break;
	}
	lista_destruir(vuelos, NULL);
	lista_iter_destruir(iter);
	return vuelos_tablero;
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

lista_t* sistema_borrar(sistema_t* sistema, char* desde, char* hasta){
	lista_t* vuelos = sistema_obtener_vuelos(sistema, desde, hasta, "desc");
	lista_iter_t* iter = lista_iter_crear(vuelos);
	while (!lista_iter_al_final(iter)) {
		vuelo_t* vuelo = lista_iter_ver_actual(iter);
		hash_borrar(sistema->hash, vuelo_codigo(vuelo));
		abb_borrar(sistema->abb, vuelo_hora(vuelo));
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);
	return vuelos;
}

void sistema_destruir(sistema_t* sistema) {
	hash_destruir(sistema->hash);
	abb_destruir(sistema->abb);
	free(sistema);
}