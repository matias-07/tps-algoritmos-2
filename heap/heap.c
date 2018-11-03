#include <stdlib.h>
#include "heap.h"

#define CAPACIDAD_INICIAL 10
#define FACTOR_REDIMENSION 2
#define COND_ACHICAR 4

struct heap {
	void** datos;
	size_t capacidad;
	size_t cantidad;
	cmp_func_t cmp;
};

/******************************************************************************
 *                          FUNCIONES AUXILIARES
 *****************************************************************************/

size_t obtener_padre(size_t pos) {
	return (pos - 1) / 2;
}

size_t obtener_hijo_izq(size_t pos) {
	return 2 * pos + 1;
}

size_t obtener_hijo_der(size_t pos) {
	return 2 * pos + 2;
}

void swap(void** arreglo, size_t a, size_t b) {
	void* aux = arreglo[a];
	arreglo[a] = arreglo[b];
	arreglo[b] = aux;
}

void upheap(heap_t* heap, size_t pos) {
	if (pos == 0) return;
	size_t padre = obtener_padre(pos);
	if (heap->cmp(heap->datos[padre], heap->datos[pos]) > 0)
		return;
	swap(heap->datos, padre, pos);
	upheap(heap, padre);
}

void downheap(heap_t* heap, size_t pos){
	void** array = heap->datos;
	size_t maximo = pos;
	size_t izq = obtener_hijo_izq(pos);
	size_t der = obtener_hijo_der(pos);
	if (izq < heap->cantidad && heap->cmp(array[izq], array[maximo]) > 0)
		maximo = izq;
	if (der < heap->cantidad && heap->cmp(array[der], array[maximo]) > 0)
		maximo = der;
	if (maximo == pos) return;
	swap(array, pos, maximo);
	downheap(heap, maximo);
}

bool heap_redimensionar(heap_t* heap, size_t nueva_capacidad){
	void** datos_redim = malloc(nueva_capacidad*sizeof(void*));
	if (!datos_redim) return false;
	for(size_t i=0; i<heap->cantidad; i++){
		datos_redim[i] = heap->datos[i];
	}
	heap->datos = datos_redim;
	heap->capacidad = nueva_capacidad;
	return true;
}

/******************************************************************************
 *                          PRIMITIVAS DEL HEAP
 *****************************************************************************/

heap_t* heap_crear(cmp_func_t cmp) {
	heap_t* heap = malloc(sizeof(heap_t));
	if (!heap) return NULL;
	heap->capacidad = CAPACIDAD_INICIAL;
	heap->datos = malloc(heap->capacidad * sizeof(void*));
	if (!heap->datos) {
		free(heap);
		return NULL;
	}
	heap->cantidad = 0;
	heap->cmp = cmp;
	return heap;
}

bool heap_encolar(heap_t* heap, void* elem) {
	if (!elem) return false;
	if (heap->cantidad == heap->capacidad) {
		if(!heap_redimensionar(heap, heap->cantidad*FACTOR_REDIMENSION))
			return false;
	}
	heap->datos[heap->cantidad] = elem;
	upheap(heap, heap->cantidad);
	heap->cantidad++;
	return true;
}

size_t heap_cantidad(const heap_t* heap){
	return heap->cantidad;
}

bool heap_esta_vacio(const heap_t* heap){
	return heap->cantidad == 0;
}

void heap_destruir(heap_t* heap, void destruir_elemento(void* e)){
	if (destruir_elemento){
		for(size_t i=0; i<(heap->cantidad); i++){
			destruir_elemento(heap->datos[i]);
		}	
	}
	free(heap->datos);
	free(heap);
}

void* heap_ver_max(const heap_t* heap){
	if (heap_esta_vacio(heap)) return NULL;
	return heap->datos[0];
}

void* heap_desencolar(heap_t* heap){
	if (heap_esta_vacio(heap)) return NULL;
	void* desencolado = heap_ver_max(heap);
	heap->datos[0] = NULL;
	swap(heap->datos, 0, heap->cantidad-1);
	downheap(heap, 0);
	heap->cantidad--;
	if (COND_ACHICAR*heap->cantidad == heap->capacidad){
		if (!heap_redimensionar(heap, heap->cantidad/FACTOR_REDIMENSION))
			return false;
	}
	return desencolado;
}

