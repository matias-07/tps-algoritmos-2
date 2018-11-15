#include "pila.h"
#include <stdlib.h>
#define CAPACIDAD_INICIAL 10

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void** datos;
    size_t cantidad;  // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* Función auxiliar. Recibe una pila y un nuevo tamaño
 * y redimensiona la pila. */
bool redimensionar(pila_t* pila, size_t nuevo_tam) {
    void** datos_nuevo = realloc(pila->datos, nuevo_tam * sizeof(void*));
    if (datos_nuevo == NULL)
        return false;
    pila->capacidad = nuevo_tam;
    pila->datos = datos_nuevo;
    return true;
}

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

pila_t* pila_crear(void) {
    pila_t* pila = malloc(sizeof(pila_t));
    if (pila == NULL)
        return NULL;
    pila->capacidad = CAPACIDAD_INICIAL;
    pila->datos = malloc(pila->capacidad * sizeof(void*));
    if (pila->datos == NULL) {
        free(pila);
        return NULL;
    }
    pila->cantidad = 0;
    return pila;
}

void pila_destruir(pila_t* pila) {
    free(pila->datos);
    free(pila);
}

bool pila_esta_vacia(const pila_t* pila) {
    return pila->cantidad == 0;
}

bool pila_apilar(pila_t* pila, void* valor) {
    if (pila->cantidad == pila->capacidad) {
        if (!redimensionar(pila, pila->capacidad * 2))
            return false;
    }
    pila->datos[pila->cantidad] = valor;
    pila->cantidad++;
    return true;
}

void* pila_ver_tope(const pila_t* pila) {
    if (pila_esta_vacia(pila))
        return NULL;
    void* tope = pila->datos[pila->cantidad - 1];
    return tope;
}

void* pila_desapilar(pila_t* pila) {
    if (pila_esta_vacia(pila))
        return NULL;
    if (pila->cantidad < pila->capacidad*1/4 && pila->cantidad >= CAPACIDAD_INICIAL*2) {
        if (!redimensionar(pila, pila->capacidad / 2))
            return false;
    }
    void* ultimo = pila->datos[pila->cantidad - 1];
    pila->cantidad--;
    return ultimo;
}
