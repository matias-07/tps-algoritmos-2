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
	return (abb_guardar(sistema->abb, vuelo->hora, vuelo) && hash_guardar(sistema->hash, vuelo->codigo, vuelo));
}

vuelo_t* sistema_ver_vuelo(const sistema_t* sistema, const char* codigo) {
	return hash_obtener(sistema->hash, codigo);
}

vuelo_t** sistema_ver_tablero(sistema_t* sistema, int cant_vuelos, char* modo, char* fecha_desde, char* fecha_hasta){ 
	//Se me ocurrio iterar en el abb hasta encontrar una fecha mayor a fecha_desde, y ahi ir agregando k vuelos hasta fecha_hasta
	//segun asc o desc, pero para eso deberiamos recorrer (en el caso, que sean los ultimos vuelos) practicamente todo el abb
	//y no seria O(log(v)) sino 0(v) aprox, y eso seria para mostrar unos pocos K ubicados al final.

	//Como hacer para que el orden sea O(log(v)) en casos promedio?
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

vuelo_t** sistema_borrar(sistema_t* sistema, char* fecha_desde, char* fecha_hasta){
	//Parecido a ver_tablero, ya que abb_borrar es O(log(v)), para que el algoritmo sea O(K*log(v)) no deberiamos hacer mas que
	//aplicar el borrado K veces, pero para deberiamos saber cuales son las fechas dentro del rango, y para eso solo se me ocurre
	//recorrer el abb comparando, y eso terminaria siendo en el peor caso O(v), entonces si me dan dos fechas entre las cuales hay 3
	//en el medio (K=3), pero estan al final, habria que recorrer el arbol dandome O(v) en lugar de O(3*log(v))

}

void sistema_destruir(sistema_t* sistema) {
	hash_destruir(sistema->hash);
	abb_destruir(sistema->abb);
	free(sistema);
}