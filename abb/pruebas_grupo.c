#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "testing.h"
#include "abb.h"

/* Funciones de prueba para el iterador interno */

bool imprimir(const char* clave, void* dato, void* extra) {
	printf("%s ", clave);
	return true;
}

bool contar(const char* clave, void* dato, void* extra) {
	*(int*)extra = *(int*)extra + 1;
	return true;
}

/* Función de comparación de prueba */
int comparacion(const char* clave1, const char* clave2) {
	if (atoi(clave1) == atoi(clave2))
		return 0;
	if (atoi(clave1) > atoi(clave2))
		return 1;
	return -1;
}

void pruebas_abb_vacio() {
	printf("~~~ PRUEBAS CON ÁRBOL VACÍO ~~~\n");
	abb_t* arbol = abb_crear(strcmp, NULL);
	print_test("abb_obtener es NULL en árbol recién creado", abb_obtener(arbol, "abc") == NULL);
	print_test("abb_pertenece devuelve false", !abb_pertenece(arbol, "def"));
	print_test("abb_cantidad es 0 en árbol vacío", abb_cantidad(arbol) == 0);
	abb_destruir(arbol);
}

void pruebas_abb_un_nodo() {
	printf("~~~ PRUEBAS CON ÚNICO NODO ~~~\n");
	abb_t* arbol = abb_crear(strcmp, NULL);
	int valor = 3;
	print_test("abb_guardar es true", abb_guardar(arbol, "clave", &valor));
	print_test("abb_pertenece devuelve true", abb_pertenece(arbol, "clave"));
	print_test("abb_obtener devuelve el valor correcto", abb_obtener(arbol, "clave") == &valor);
	print_test("abb_cantidad devuelve 1", abb_cantidad(arbol) == 1);
	print_test("abb_obtener devuelve NULL con clave inexistente", abb_obtener(arbol, "def") == NULL);
	print_test("abb_borrar devuelve el valor correcto", abb_borrar(arbol, "clave") == &valor);
	print_test("abb_pertenece es false luego de quitar el nodo", !abb_pertenece(arbol, "clave"));
	print_test("abb_cantidad es 0", abb_cantidad(arbol) == 0);
	abb_destruir(arbol);
}

void pruebas_abb_insertar_borrar() {
	printf("~~~ PRUEBAS INSERTANDO Y BORRANDO NODOS ~~~\n");
	abb_t* arbol = abb_crear(strcmp, NULL);
	char* valor1 = "abcdef";
	char* valor2 = "ghijkl";
	print_test("abb_cantidad devuelve 0", abb_cantidad(arbol) == 0);
	print_test("abb_guardar devuelve true", abb_guardar(arbol, "abc", valor1));
	print_test("abb_cantidad devuelve 1", abb_cantidad(arbol) == 1);
	print_test("abb_pertenece devuelve true", abb_pertenece(arbol, "abc"));
	print_test("abb_cantidad devuelve 1", abb_cantidad(arbol) == 1);
	print_test("abb_obtener devuelve el valor", abb_obtener(arbol, "abc") == valor1);
	print_test("abb_cantidad devuelve 1", abb_cantidad(arbol) == 1);
	print_test("abb_borrar devuelve el valor", abb_borrar(arbol, "abc"));
	print_test("abb_cantidad devuelve 0", abb_cantidad(arbol) == 0);
	print_test("abb_pertenece devuelve false", !abb_pertenece(arbol, "abc"));
	print_test("abb_borrar devuelve NULL", abb_borrar(arbol, "abc") == NULL);
	print_test("abb_guardar devuelve true", abb_guardar(arbol, "def", valor2));
	print_test("abb_cantidad devuelve 1", abb_cantidad(arbol) == 1);
	abb_destruir(arbol);
}

void pruebas_abb_multiples_nodos() {
	printf("~~~ PRUEBAS CON MÚLTIPLES NODOS ~~~\n");
	abb_t* arbol = abb_crear(strcmp, NULL);
	char* claves[7] = {"uno", "dos", "tres", "cuatro", "cinco", "seis", "siete"};
	char* valores[7] = {"abc", "def", "ghi", "jkl", "mno", "pqr", "stu"};
	for (int i = 0; i < 7; i++){
		abb_guardar(arbol, claves[i], valores[i]);
	}
	print_test("abb_pertenece devuelve true para cada clave guardada", abb_pertenece(arbol, claves[0]));
	print_test("abb_cantidad devuelve 7", abb_cantidad(arbol) == 7);
	print_test("abb_pertenece devuelve false con clave inexistente", !abb_pertenece(arbol, "hola"));
	print_test("abb_borrar devuelve el valor correcto", abb_borrar(arbol, claves[0]) == valores[0]);
	print_test("abb_pertenece devuelve false para la clave borrada", !abb_pertenece(arbol, claves[0]));
	print_test("abb_borrar devuelve NULL para clave inexistente", abb_borrar(arbol, "chau") == NULL);
	print_test("abb_cantidad devuelve 6", abb_cantidad(arbol) == 6);
	print_test("abb_borrar devuelve el valor correcto", abb_borrar(arbol, claves[4]) == valores[4]);
	print_test("abb_cantidad devuelve 5", abb_cantidad(arbol) == 5);
	abb_destruir(arbol);
}

void pruebas_abb_borrado() {
	printf("~~~ PRUEBAS BORRADOS TODOS LOS CASOS ~~~\n");
	abb_t* arbol = abb_crear(comparacion, NULL);
	char* claves[10] = {"35", "72", "12", "22", "50", "4", "98", "61", "52", "63"};
	char* valores[10] = {"abc", "def", "ghi", "jkl", "mno", "pqr", "stu", "vwx", "yz", "123"};
	for (int i = 0; i < 10; i++) {
		abb_guardar(arbol, claves[i], valores[i]);
	}
	printf("Recorrido in order del árbol: ");
	abb_in_order(arbol, imprimir, NULL);
	printf("\n");
	print_test("Prueba borrado de hoja", abb_borrar(arbol, claves[3]) == valores[3]);
	printf("Recorrido luego de borrar: ");
	abb_in_order(arbol, imprimir, NULL);
	printf("\n");
	print_test("Prueba borrado de nodo con hijo izquierdo", abb_borrar(arbol, claves[2]) == valores[2]);
	printf("Recorrido luego de borrar: ");
	abb_in_order(arbol, imprimir, NULL);
	printf("\n");
	print_test("Prueba borrado de nodo con hijo derecho", abb_borrar(arbol, claves[4]) == valores[4]);
	printf("Recorrido luego de borrar: ");
	abb_in_order(arbol, imprimir, NULL);
	printf("\n");
	print_test("Prueba borrado de nodos con ambos hijos", abb_borrar(arbol, claves[7]) == valores[7]);
	printf("Recorrido luego de borrar: ");
	abb_in_order(arbol, imprimir, NULL);
	printf("\n");
	print_test("Prueba borrado de raiz", abb_borrar(arbol, claves[0]) == valores[0]);
	printf("Recorrido luego de borrar: ");
	abb_in_order(arbol, imprimir, NULL);
	printf("\n");
	abb_destruir(arbol);
}

void pruebas_abb_vacio_iterar() {
	printf("~~~ PRUEBAS ITERADORES SOBRE ÁRBOL VACÍO ~~~\n");
	abb_t* arbol = abb_crear(strcmp, NULL);
	abb_in_order(arbol, imprimir, NULL);
	abb_iter_t* iter = abb_iter_in_crear(arbol);
	print_test("El iterador externo está al final", abb_iter_in_al_final(iter));
	print_test("abb_iter_in_avanzar devuelve false", !abb_iter_in_avanzar(iter));
	print_test("abb_iter_in_ver_actual devuelve NULL", abb_iter_in_ver_actual(iter) == NULL);
	abb_iter_in_destruir(iter);
	abb_destruir(arbol);
}

void pruebas_abb_iterar() {
	printf("~~~ PRUEBAS ITERADORES SOBRE ÁRBOL NO VACÍO ~~~\n");
	abb_t* arbol = abb_crear(comparacion, NULL);
	char* claves[7] = {"35", "72", "12", "22", "50", "4", "98"};
	char* valores[7] = {"abc", "def", "ghi", "jkl", "mno", "pqr", "stu"};
	for (int i = 0; i < 7; i++)
		abb_guardar(arbol, claves[i], valores[i]);
	printf("Imprimir claves usando iterador interno: ");
	abb_in_order(arbol, imprimir, NULL);
	printf("\n");
	abb_iter_t* iter = abb_iter_in_crear(arbol);
	print_test("iter_in_al_final devuelve false", !abb_iter_in_al_final(iter));
	print_test("iter_in_ver_actual devuelve la primer clave", strcmp(abb_iter_in_ver_actual(iter), "4") == 0);
	print_test("iter_in_ver_actual devuelve copia de la clave", abb_iter_in_ver_actual(iter) != claves[5]);
	printf("Imprimir claves usando iterador externo: ");
	while (!abb_iter_in_al_final(iter)) {
		printf("%s ", abb_iter_in_ver_actual(iter));
		abb_iter_in_avanzar(iter);
	}
	printf("\n");
	print_test("El iterador está al final", abb_iter_in_al_final(iter));
	print_test("iter_in_avanzar devuelve false", !abb_iter_in_avanzar(iter));
	print_test("iter_in_ver_actual devuelve NULL", abb_iter_in_ver_actual(iter) == NULL);
	abb_iter_in_destruir(iter);
	abb_destruir(arbol);
}

void pruebas_abb_free() {
	printf("~~~ PRUEBAS CON FUNCIÓN DE DESTRUCCIÓN ~~~\n");
	abb_t* arbol = abb_crear(strcmp, free);
	char* claves[10] = {"89", "11", "50", "43", "90", "67", "190", "134", "5", "31"};
	int** arreglo = malloc(10 * sizeof(int*));
	for (int i = 0; i < 10; i++) {
		arreglo[i] = malloc(sizeof(int));
		abb_guardar(arbol, claves[i], arreglo[i]);
	}
	print_test("La cantidad de nodos es 10", abb_cantidad(arbol) == 10);
	print_test("abb_obtener devuelve NULL con clave inexistente", abb_obtener(arbol, "abc") == NULL);
	print_test("abb_pertenece devuelve false", !abb_pertenece(arbol, "def"));
	print_test("abb_guardar devuelve true", abb_guardar(arbol, "7", arreglo));
	print_test("La cantidad de nodos es 11", abb_cantidad(arbol) == 11);
	abb_destruir(arbol);
}

void pruebas_abb_volumen(unsigned int n) {
	printf("~~~ PRUEBAS DE VOLUMEN ~~~\n");
	abb_t* arbol = abb_crear(strcmp, free);
	char (*claves)[10] = malloc(n * 10);
	unsigned* valores[n];
	for (unsigned i = 0; i < n; i++) {
		sprintf(claves[i], "%08d", i);
		valores[i] = malloc(sizeof(int));
		*valores[i] = i;
		abb_guardar(arbol, claves[i], valores[i]);
	}
	print_test("abb_cantidad devuelve la cantidad correcta", abb_cantidad(arbol) == n);
	int i = 0;
	abb_in_order(arbol, contar, &i);
	print_test("Iterador interno recorrió todo el largo", i == n);
	i = 0;
	abb_iter_t* iter = abb_iter_in_crear(arbol);
	while (!abb_iter_in_al_final(iter)) {
		i++;
		abb_iter_in_avanzar(iter);
	}
	print_test("Iterador externo recorrió todo el largo", i == n);
	abb_iter_in_destruir(iter);
	abb_destruir(arbol);
	free(claves);
}

void pruebas_abb_alumno() {
	pruebas_abb_vacio();
	pruebas_abb_un_nodo();
	pruebas_abb_insertar_borrar();
	pruebas_abb_multiples_nodos();
	pruebas_abb_borrado();
	pruebas_abb_vacio_iterar();
	pruebas_abb_iterar();
	pruebas_abb_free();
	pruebas_abb_volumen(1000);
}