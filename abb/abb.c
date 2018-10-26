#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


typedef int (*abb_comparar_clave_t) (const char *, const char *);
typedef void (*abb_destruir_dato_t) (void *);

typedef struct abb_nodo{
	struct abb_nodo izq;
	struct abb_nodo der;
	char* clave;
	void* dato;
} abb_nodo_t;

typedef struct abb{
	abb_nodo_t* raiz;
	abb_comparar_clave_t cmp;
	abb_destruir_dato_t destruir_dato;
	size_t cantidad;
} abb_t;

abb_nodo_t* nodo_crear(char* clave, void* dato){
	abb_nodo_t* nodo = malloc(sizeof(abb_nodo_t));
	if (!nodo) return NULL;
	nodo->izq = NULL;
	nodo->der = NULL;
	nodo->clave = clave;
	nodo->dato = dato;
	return nodo;
}

abb_nodo_t* abb_nodo_buscar(abb_t* arbol, abb_nodo_t* nodo, char* clave){
	if (nodo == NULL) return NULL;
	if (arbol->cmp(nodo->clave, clave) == 0) return nodo;
	if (arbol->cmp(nodo->clave, clave) < 0)
		return abb_nodo_buscar(arbol, nodo->der, clave);
	if (arbol->cmp(nodo->clave, clave) > 0)
		return abb_nodo_buscar(arbol, nodo->izq, clave);
}

void nodo_destruir(abb_nodo_t* nodo, abb_destruir_dato_t destruir_dato){
	if (destruir_dato != NULL)
		destruir_dato(nodo->dato);
	free(nodo);
}

/*Primitivas del ABB*/

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
	abb_t* arbol = malloc(sizeof(abb_t));
	if (!arbol) return NULL;
	arbol->raiz = NULL;
	arbol->cmp = cmp;
	arbol->destruir_dato = destruir_dato;
	arbol->cantidad = 0;
}

bool abb_pertenece(const abb_t* arbol, const char* clave){
	if (abb_nodo_buscar(arbol, arbol->raiz, clave) != NULL)
		return true;
	return false;
}

void* abb_obtener(const abb_t* arbol, const char* clave){
	abb_nodo_t* nodo = abb_nodo_buscar(arbol, arbol->raiz, clave);
	return nodo->dato;
}

size_t abb_cantidad(abb_t* arbol){
	return arbol->cantidad;
}

bool abb_guardar(abb_t* arbol, const char* clave, void* dato){
	char* clave_dup = strdup(clave);
	abb_nodo_t* nodo = nodo_crear(clave_dup, dato);
	if (!nodo) return false;
	if(abb_cantidad(arbol) == 0){
		arbol->raiz = nodo;
		arbol->cantidad++;
		return true;
	}
	//Casos mas dificiles
}









