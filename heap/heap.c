#include <stdlib.h>
#include "heap.h"

#define CAPACIDAD_INICIAL 10
#define FACTOR_REDIMENSION 2

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
	return 2 * pos - 1;
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