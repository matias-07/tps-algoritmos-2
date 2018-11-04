#include <stdio.h>
#include <stdlib.h>
#include "testing.h"
#include "heap.h"

/* Función de comparación de prueba */
int intcmp(const void* a, const void* b) {
	if (*(int*)a > *(int*)b) return 1;
	if (*(int*)a < *(int*)b) return -1;
	return 0;
}

void pruebas_heap_vacio(){
	printf("~~~ PRUEBAS HEAP VACIO ~~~\n");
	heap_t* heap = heap_crear(intcmp);
	print_test("El heap creado está vacío", heap_esta_vacio(heap));
	print_test("heap_desencolar devuelve NULL", heap_desencolar(heap) == NULL);
	print_test("heap_ver_max devuelve NULL", heap_ver_max(heap) == NULL);
	print_test("La cantidad de elementos del heap es 0", heap_cantidad(heap) == 0);
	heap_destruir(heap, NULL);
}

void pruebas_heap_encolar() {
	printf("~~~ PRUEBAS ENCOLANDO UN ELEMENTO ~~~\n");
	heap_t* heap = heap_crear(intcmp);
	int a = 3;
	print_test("heap_encolar devuelve true", heap_encolar(heap, &a));
	print_test("El heap no está vacío", !heap_esta_vacio(heap));
	print_test("La cantidad de elementos del heap es 1", heap_cantidad(heap) == 1);
	print_test("heap_ver_max devuelve el único elemento", heap_ver_max(heap) == &a);
	print_test("El heap no está vacío", !heap_esta_vacio(heap));
	print_test("heap_desencolar devuelve el único elemento", heap_desencolar(heap) == &a);
	print_test("La cantidad de elementos es 0", heap_cantidad(heap) == 0);
	print_test("heap_desencolar devuelve NULL", heap_desencolar(heap) == NULL);
	heap_destruir(heap, NULL);
}

void pruebas_heap_encolar_varios() {
	printf("~~~ PRUEBAS ENCOLANDO VARIOS ELEMENTOS ~~~\n");
	heap_t* heap = heap_crear(intcmp);
	int a = 3;
	int b = 5;
	int c = 2;
	print_test("heap_encolar devuelve true", heap_encolar(heap, &a));
	print_test("heap_cantidad devuelve 1", heap_cantidad(heap) == 1);
	print_test("heap_encolar devuelve true", heap_encolar(heap, &b));
	print_test("El heap no está vacío", !heap_esta_vacio(heap));
	print_test("heap_ver_max devuelve el elemento correcto", heap_ver_max(heap) == &b);
	print_test("heap_encolar devuelve true", heap_encolar(heap, &c));
	print_test("heap_ver_max devuelve el elemento correcto", heap_ver_max(heap) == &b);
	print_test("heap_desencolar devuelve el máximo elemento", heap_desencolar(heap) == &b);
	print_test("heap_ver_max devuelve el nuevo máximo", heap_ver_max(heap) == &a);
	print_test("El heap no está vacío", !heap_esta_vacio(heap));
	heap_destruir(heap, NULL);
}

void pruebas_heap_volumen(size_t n) {
	printf("~~~ PRUEBAS DE VOLUMEN ~~~\n");
	heap_t* heap = heap_crear(intcmp);
	int* arreglo = malloc(n * sizeof(int));
	bool ok_ver_max = true, ok_cantidad = true;
	for (int i = 0; i < n; i++) {
		arreglo[i] = i;
		heap_encolar(heap, &arreglo[i]);
		if (heap_ver_max(heap) != &arreglo[i]) ok_ver_max = false;
		if (heap_cantidad(heap) != i+1) ok_cantidad = false;
	}
	print_test("heap_ver_max devuelve los elementos correctos", ok_ver_max);
	print_test("heap_cantidad devuelve la cantidad correcta", ok_cantidad);
	heap_destruir(heap, NULL);
	free(arreglo);
}

void pruebas_crear_heap_arr() {
	printf("~~~ PRUEBA CREANDO HEAP A PARTIR DE ARREGLO ~~~\n");
	int numeros[] = {5, 2, 8, 6, 3};
	int** arreglo = malloc(5 * sizeof(int*));
	for (int i = 0; i < 5; i++) {
		arreglo[i] = malloc(sizeof(int));
		*arreglo[i] = numeros[i];
	}
	heap_t* heap = heap_crear_arr((void**)arreglo, 5, intcmp);
	print_test("La cantidad de elementos es 5", heap_cantidad(heap) == 5);
	print_test("heap_ver_max devuelve el elemento correcto", *(int*)heap_ver_max(heap) == 8);
	heap_destruir(heap, free);
	// La memoria se libera con heap_destruir
}

void pruebas_heap_sort() {
	printf("~~~ PRUEBAS HEAP SORT ~~~\n");

	int ordenado[10] = {0, 1, 2, 3, 3, 4, 6, 7, 8, 9};
	int desordenado[10] = {3, 9, 4, 6, 7, 2, 1, 3, 0, 8};

	int** arreglo = malloc(10 * sizeof(int*));
	
	for (int i = 0; i < 10; i++) {
		arreglo[i] = malloc(sizeof(int));
		*arreglo[i] = desordenado[i];
	}

	heap_sort((void**)arreglo, 10, intcmp);
	bool ok = true;
	for (int i = 0; i < 10; i++) {
		if (*arreglo[i] != ordenado[i])
			ok = false;
	}
	print_test("Prueba heap_sort con arreglo desordenado", ok);

	for (int i = 0; i < 10; i++) {
		*arreglo[i] = ordenado[i];
	}
	// Se aplica heap_sort de nuevo al arreglo, que ya está ordenado,
	// y debe quedar igual.
	heap_sort((void**)arreglo, 10, intcmp);
	ok = true;
	for (int i = 0; i < 10; i++) {
		if (*arreglo[i] != ordenado[i])
			ok = false;
	}
	print_test("Prueba heap_sort con arreglo ya ordenado", ok);

	for (int i = 0; i < 10; i++)
		free(arreglo[i]);
	free(arreglo);
}

void pruebas_heap_alumno(){
	pruebas_heap_vacio();
	pruebas_heap_encolar();
	pruebas_heap_encolar_varios();
	pruebas_heap_volumen(10000);
	pruebas_crear_heap_arr();
	pruebas_heap_sort();
}