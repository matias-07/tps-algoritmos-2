#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <string.h>
#include "abb.h"
#include "pila.h"

typedef struct abb_nodo {
    struct abb_nodo* izq;
    struct abb_nodo* der;
    char* clave;
    void* dato;
} abb_nodo_t;

struct abb {
    abb_nodo_t* raiz;
    abb_comparar_clave_t cmp;
    abb_destruir_dato_t destruir_dato;
    size_t cantidad;
};

struct abb_iter {
    pila_t* pila;
};

/******************************************************************************
 *                         FUNCIONES AUXILIARES
 *****************************************************************************/

/* Busca el nodo asociado con la clave en el abb
 * y lo devuelve.
 */
abb_nodo_t* abb_nodo_buscar(const abb_t* arbol, abb_nodo_t* raiz, const char* clave) {
    if (!raiz) return NULL;
    if (!raiz->clave) return NULL;
    if (arbol->cmp(raiz->clave, clave) == 0)
        return raiz;
    if (arbol->cmp(raiz->clave, clave) > 0)
        return abb_nodo_buscar(arbol, raiz->izq, clave);
    return abb_nodo_buscar(arbol, raiz->der, clave);
}

/* Busca el padre del nodo pasado en el abb y
 * lo devuelve.
 */
abb_nodo_t* abb_buscar_padre(const abb_t* arbol, abb_nodo_t* raiz, abb_nodo_t* nodo) {
    if (!raiz) return NULL;
    if (raiz == nodo) return NULL;
    if (raiz->izq == nodo || raiz->der == nodo)
        return raiz;
    if (arbol->cmp(raiz->clave, nodo->clave) > 0)
        return abb_buscar_padre(arbol, raiz->izq, nodo);
    return abb_buscar_padre(arbol, raiz->der, nodo);
}

/* Intercambia los pares (clave, dato) entre dos nodos.
 */
void intercambiar_datos(abb_nodo_t* a, abb_nodo_t* b) {
    char* clave_aux = a->clave;
    void* dato_aux = a->dato;
    a->clave = b->clave;
    b->clave = clave_aux;
    a->dato = b->dato;
    b->dato = dato_aux;
}

/* Devuelve el nodo menor del abb.
 */
abb_nodo_t* abb_obtener_menor(abb_nodo_t* raiz) {
    if (!raiz->izq) return raiz;
    return abb_obtener_menor(raiz->izq);
}

/* Devuelve el siguiente nodo in-order de la raiz
 * pasada por parámetro.
 */
abb_nodo_t* abb_obtener_siguiente(abb_nodo_t* raiz) {
    if (!raiz->der) return NULL;
    return abb_obtener_menor(raiz->der);
}

/* Recibe un nodo y cambia su hijo por el nuevo pasado
 * por parámetro.
 */
void cambiar_nodo(abb_nodo_t* padre, abb_nodo_t* hijo, abb_nodo_t* nuevo) {
    if (!padre) return;
    if (padre->izq == hijo)
        padre->izq = nuevo;
    else
        padre->der = nuevo;
}

/* Primitivas del nodo */

/* Crea un nodo.
 */
abb_nodo_t* nodo_crear(void){
    abb_nodo_t* nodo = malloc(sizeof(abb_nodo_t));
    if (!nodo) return NULL;
    nodo->izq = NULL;
    nodo->der = NULL;
    nodo->clave = NULL;
    nodo->dato = NULL;
    return nodo;
}

/* Guarda en el nodo la clave y el dato.
 */
bool nodo_guardar(abb_nodo_t* nodo, const char* clave, void* dato) {
    nodo->clave = strdup(clave);
    nodo->dato = dato;
    return true;
}

/* Destruye el nodo.
 * Si se recibe una función de destrucción, la aplica
 * a su dato.
 */
void nodo_destruir(abb_nodo_t* nodo, abb_destruir_dato_t destruir_dato){
    free(nodo->clave);
    if (destruir_dato != NULL)
        destruir_dato(nodo->dato);
    free(nodo);
}

/******************************************************************************
 *                         PRIMITIVAS DEL ABB
 *****************************************************************************/

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato){
    abb_t* arbol = malloc(sizeof(abb_t));
    if (!arbol) return NULL;
    arbol->raiz = NULL;
    arbol->cmp = cmp;
    arbol->destruir_dato = destruir_dato;
    arbol->cantidad = 0;
    return arbol;
}

bool abb_pertenece(const abb_t* arbol, const char* clave){
    return abb_nodo_buscar(arbol, arbol->raiz, clave) != NULL;
}

void* abb_obtener(const abb_t* arbol, const char* clave){
    abb_nodo_t* nodo = abb_nodo_buscar(arbol, arbol->raiz, clave);
    if (!nodo) return NULL;
    return nodo->dato;
}

size_t abb_cantidad(const abb_t* arbol){
    return arbol->cantidad;
}

bool _abb_guardar(abb_t* arbol, abb_nodo_t* nodo, const char* clave, void* dato) {
    if (nodo->clave == NULL) {
        return nodo_guardar(nodo, clave, dato);
    }
    if (arbol->cmp(nodo->clave, clave) > 0) {
        if (!nodo->izq) {
            nodo->izq = nodo_crear();
            if (!nodo->izq) return false;
        }
        return _abb_guardar(arbol, nodo->izq, clave, dato);
    }
    if (!nodo->der) {
        nodo->der = nodo_crear();
        if (!nodo->der) return false;
    }
    return _abb_guardar(arbol, nodo->der, clave, dato);
}

bool abb_guardar(abb_t* arbol, const char* clave, void* dato) {
    if (!arbol->raiz) {
        arbol->raiz = nodo_crear();
        if (!arbol->raiz)
            return false;
        if (!nodo_guardar(arbol->raiz, clave, dato))
            return false;
        arbol->cantidad++;
        return true;
    }
    abb_nodo_t* nodo = abb_nodo_buscar(arbol, arbol->raiz, clave);
    if (nodo) {
        if (arbol->destruir_dato)
            arbol->destruir_dato(nodo->dato);
        nodo->dato = dato;
        return true;
    }
    if (!_abb_guardar(arbol, arbol->raiz, clave, dato))
        return false;
    arbol->cantidad++;
    return true;
}

void* _abb_borrar(abb_t* arbol, abb_nodo_t* raiz, abb_nodo_t* nodo) {
    void* dato = nodo->dato;
    abb_nodo_t* padre = abb_buscar_padre(arbol, raiz, nodo);
    if (!nodo->izq && !nodo->der) {
        cambiar_nodo(padre, nodo, NULL);
        if (!padre) arbol->raiz = NULL;
        nodo_destruir(nodo, NULL);
        return dato;
    }
    if (!nodo->izq) {
        cambiar_nodo(padre, nodo, nodo->der);
        if (!padre) arbol->raiz = nodo->der;
        nodo_destruir(nodo, NULL);
        return dato;
    }
    if (!nodo->der) {
        cambiar_nodo(padre, nodo, nodo->izq);
        if (!padre) arbol->raiz = nodo->izq;
        nodo_destruir(nodo, NULL);
        return dato;
    }
    abb_nodo_t* siguiente = abb_obtener_siguiente(nodo);
    padre = abb_buscar_padre(arbol, raiz, siguiente);
    intercambiar_datos(nodo, siguiente);
    return _abb_borrar(arbol, padre, siguiente);
}

void* abb_borrar(abb_t* arbol, const char* clave) {
    abb_nodo_t* nodo = abb_nodo_buscar(arbol, arbol->raiz, clave);
    if (!nodo) return NULL;
    void* borrado = _abb_borrar(arbol, arbol->raiz, nodo);
    arbol->cantidad--;
    return borrado;
}

void _abb_destruir(abb_t* arbol, abb_nodo_t* raiz) {
    if (!raiz) return;
    _abb_destruir(arbol, raiz->izq);
    _abb_destruir(arbol, raiz->der);
    nodo_destruir(raiz, arbol->destruir_dato);
}

void abb_destruir(abb_t* arbol){
    _abb_destruir(arbol, arbol->raiz);
    free(arbol);
}

void _abb_in_order(abb_nodo_t* raiz, bool visitar(const char*, void*, void*), void* extra) {
    if (!raiz) return;
    if (!raiz->clave) return;
    _abb_in_order(raiz->izq, visitar, extra);
    if (!visitar(raiz->clave, raiz->dato, extra))
        return;
    _abb_in_order(raiz->der, visitar, extra);
}

void abb_in_order(abb_t* arbol, bool visitar(const char*, void*, void*), void* extra) {
    _abb_in_order(arbol->raiz, visitar, extra);
}

/******************************************************************************
 *                    PRIMITIVAS DEL ITERADOR EXTERNO
 *****************************************************************************/

/* Función auxiliar. */
void apilar_izquierdos(pila_t* pila, abb_nodo_t* nodo) {
    if (!nodo) return;
    if (!nodo->clave) return;
    pila_apilar(pila, nodo);
    apilar_izquierdos(pila, nodo->izq);
}

abb_iter_t* abb_iter_in_crear(const abb_t* arbol) {
    abb_iter_t* iter = malloc(sizeof(abb_iter_t));
    if (!iter) return NULL;
    iter->pila = pila_crear();
    apilar_izquierdos(iter->pila, arbol->raiz);
    return iter;
}

bool abb_iter_in_avanzar(abb_iter_t* iter) {
    if (abb_iter_in_al_final(iter)) return false;
    abb_nodo_t* desapilado = pila_desapilar(iter->pila);
    apilar_izquierdos(iter->pila, desapilado->der);
    return true;
}

const char* abb_iter_in_ver_actual(const abb_iter_t* iter) {
    abb_nodo_t* actual = pila_ver_tope(iter->pila);
    if (!actual) return NULL;
    return actual->clave;
}

bool abb_iter_in_al_final(const abb_iter_t* iter) {
    return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t* iter) {
    pila_destruir(iter->pila);
    free(iter);
}