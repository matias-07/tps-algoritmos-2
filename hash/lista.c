#include <stdlib.h>
#include <stdbool.h>
#include "lista.h"

/********************** DEFINICIÓN DE NODO *********************/

typedef struct nodo {
    struct nodo* prox;
    void* dato;
} nodo_t;

nodo_t* nodo_crear(void* dato) {
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if (!nodo) return NULL;
    nodo->prox = NULL;
    nodo->dato = dato;
    return nodo;
}

/*************** DEFINICIÓN DE LA LISTA ENLAZADA ***************/

typedef struct lista {
    nodo_t* prim;
    nodo_t* ult;
    size_t largo;
} lista_t;

/*************** PRIMITIVAS DE LA LISTA ENLAZADA ***************/

lista_t* lista_crear(void) {
    lista_t* lista = malloc(sizeof(lista_t));
    if (!lista) return NULL;
    lista->prim = NULL;
    lista->ult = NULL;
    lista->largo = 0;
    return lista;
}

bool lista_esta_vacia(const lista_t* lista) {
    return lista->largo == 0;
}

bool lista_insertar_primero(lista_t* lista, void* dato) {
    nodo_t* nuevo = nodo_crear(dato);
    if (!nuevo) return false;
    nodo_t* prim_anterior = lista->prim;
    if (!prim_anterior)
        lista->ult = nuevo;
    lista->prim = nuevo;
    lista->prim->prox = prim_anterior;
    lista->largo++;
    return true;
}

bool lista_insertar_ultimo(lista_t* lista, void* dato) {
    nodo_t* nuevo = nodo_crear(dato);
    if (!nuevo) return false;
    if (!lista->ult) {
        lista->prim = nuevo;
        lista->ult = nuevo;
    } else {
        lista->ult->prox = nuevo;
        lista->ult = nuevo;
    }
    lista->largo++;
    return true;
}

void* lista_borrar_primero(lista_t* lista) {
    if (lista_esta_vacia(lista))
        return NULL;
    nodo_t* elemento = lista->prim;
    void* dato = elemento->dato;
    lista->prim = lista->prim->prox;
    if (!lista->prim)
        lista->ult = NULL;
    lista->largo--;
    free(elemento);
    return dato;
}

void* lista_ver_primero(const lista_t* lista) {
    if (lista_esta_vacia(lista))
        return NULL;
    return lista->prim->dato;
}

void* lista_ver_ultimo(const lista_t* lista) {
    if (lista_esta_vacia(lista))
        return NULL;
    return lista->ult->dato;
}

size_t lista_largo(const lista_t* lista) {
    return lista->largo;
}

void lista_destruir(lista_t* lista, void destruir_dato(void*)) {
    nodo_t* actual = lista->prim;
    while (actual) {
        nodo_t* siguiente = actual->prox;
        if (destruir_dato)
            destruir_dato(actual->dato);
        free(actual);
        actual = siguiente;
    }
    free(lista);
}

/************** PRIMITIVA DEL ITERADOR INTERNO ****************/

void lista_iterar(lista_t* lista, bool visitar(void* dato, void* extra), void* extra) {
    nodo_t* actual = lista->prim;
    while (actual) {
        if (!visitar(actual->dato, extra))
            break;
        actual = actual->prox;
    }
}

/************** PRIMITIVAS DEL ITERADOR EXTERNO ***************/

typedef struct lista_iter {
	lista_t* lista;
    nodo_t* anterior;
    nodo_t* actual;
} lista_iter_t;

lista_iter_t* lista_iter_crear(lista_t* lista) {
    lista_iter_t* iter = malloc(sizeof(lista_iter_t));
    if (!iter) return NULL;
    iter->lista = lista;
    iter->anterior = NULL;
    iter->actual = lista->prim;
    return iter;
}

bool lista_iter_avanzar(lista_iter_t* iter) {
    if (!iter->actual) return false;
    iter->anterior = iter->actual;
    iter->actual = iter->actual->prox;
    return true;
}

void* lista_iter_ver_actual(const lista_iter_t* iter) {
    if (!iter->actual) return NULL;
    return iter->actual->dato;
}

bool lista_iter_al_final(const lista_iter_t* iter) {
    return iter->actual == NULL;
}

void lista_iter_destruir(lista_iter_t* iter) {
    free(iter);
}

bool lista_iter_insertar(lista_iter_t* iter, void* dato) {
    nodo_t* nuevo = nodo_crear(dato);
    if (!nuevo) return false;
    nuevo->prox = iter->actual;
    if (!nuevo->prox)
        iter->lista->ult = nuevo;
    if (iter->anterior)
        iter->anterior->prox = nuevo;
    else
    	iter->lista->prim = nuevo;
    iter->actual = nuevo;
    iter->lista->largo++;
    return true;
}

void* lista_iter_borrar(lista_iter_t* iter) {
    if (!iter->actual) return NULL;
    nodo_t* a_borrar = iter->actual;
    iter->actual = iter->actual->prox;
    if (iter->anterior)
        iter->anterior->prox = iter->actual;
    else
        iter->lista->prim = iter->actual;
    if (!iter->actual)
        iter->lista->ult = iter->anterior;
    void* dato = a_borrar->dato;
    free(a_borrar);
    iter->lista->largo--;
    return dato;
}