#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lista.h"
#include "hash.h"

#define LARGO_INICIAL 7
#define CARGA_MAX 1.75
#define CARGA_MIN 0.25
#define FACTOR_REDIM 2

typedef struct hash_campo {
    char* clave;
    void* valor;
} hash_campo_t;

struct hash {
    lista_t** tabla;
    size_t cantidad;
    size_t largo;
    hash_destruir_dato_t destruir_dato;
};

struct hash_iter {
    const hash_t* hash;
    size_t indice;
    lista_iter_t* actual;
};

/* Función de hashing djb2.
 * Créditos a Daniel J. Bernstein.
 */
size_t funcion_hash(const char *str, size_t largo_hash) {
    size_t valor = 5381;
    size_t c;
    while ((c = *str++))
        valor = ((valor << 5) + valor) + c;
    return valor % largo_hash;
}

/* PRIMITIVAS DEL CAMPO DE HASH */

hash_campo_t* hash_campo_crear(const char* clave, void* valor) {
    hash_campo_t* campo = malloc(sizeof(hash_campo_t));
    if (!campo) return NULL;
    campo->clave = strdup(clave);
    campo->valor = valor;
    return campo;
}

void hash_campo_destruir(hash_campo_t* campo) {
    free(campo->clave);
    free(campo);
}

/******************************************************************************
 *                          FUNCIONES AUXILIARES
 *****************************************************************************/

void tabla_inicializar(lista_t** tabla, size_t largo) {
    for (size_t i = 0; i < largo; i++)
        tabla[i] = lista_crear();
}

void tabla_destruir(lista_t** tabla, size_t largo, hash_t* nuevo_hash) {
    for (size_t i = 0; i < largo; i++) {
        while(!lista_esta_vacia(tabla[i])) {
            hash_campo_t* campo = lista_borrar_primero(tabla[i]);
            hash_guardar(nuevo_hash, campo->clave, campo->valor);
            hash_campo_destruir(campo);
        }
        lista_destruir(tabla[i], NULL);
    }
    free(tabla);
}

bool redimensionar(hash_t* hash, size_t nuevo_largo){
    lista_t** tabla_anterior = hash->tabla;
    size_t largo_anterior = hash->largo;
    hash->tabla = malloc(nuevo_largo * sizeof(lista_t*));
    if (!hash->tabla) return false;
    tabla_inicializar(hash->tabla, nuevo_largo);
    hash->largo = nuevo_largo;
    hash->cantidad = 0;
    tabla_destruir(tabla_anterior, largo_anterior, hash);
    return true;
}

hash_campo_t* buscar_campo(const hash_t* hash, const char* clave) {
    size_t indice = funcion_hash(clave, hash->largo);
    hash_campo_t* campo = NULL;
    lista_iter_t* iter = lista_iter_crear(hash->tabla[indice]);
    while (!lista_iter_al_final(iter)) {
        hash_campo_t* actual = lista_iter_ver_actual(iter);
        if (strcmp(actual->clave, clave) == 0) {
            campo = actual;
            break;
        }
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
    return campo;
}

float hash_carga(const hash_t* hash) {
    return (float)hash->cantidad/(float)hash->largo;
}

/******************************************************************************
 *                           PRIMITIVAS DEL HASH
 *****************************************************************************/

hash_t* hash_crear(hash_destruir_dato_t destruir_dato) {
    hash_t* hash = malloc(sizeof(hash_t));
    if (!hash) return NULL;
    hash->tabla = malloc(LARGO_INICIAL * sizeof(lista_t*));
    if (!hash->tabla) {
        free(hash);
        return NULL;
    }
    for (size_t i = 0; i < LARGO_INICIAL; i++)
        hash->tabla[i] = lista_crear();
    hash->cantidad = 0;
    hash->largo = LARGO_INICIAL;
    hash->destruir_dato = destruir_dato;
    return hash;
}

bool hash_guardar(hash_t* hash, const char* clave, void* dato) {
    if (hash_carga(hash) > CARGA_MAX) {
        if (!redimensionar(hash, hash->largo*FACTOR_REDIM))
            return false;
    }
    size_t indice = funcion_hash(clave, hash->largo);
    hash_campo_t* campo = buscar_campo(hash, clave);
    if (campo != NULL) {
        if (hash->destruir_dato)
            hash->destruir_dato(campo->valor);
        campo->valor = dato;
        return true;
    }
    campo = hash_campo_crear(clave, dato);
    if (!campo) return false;
    lista_insertar_ultimo(hash->tabla[indice], campo);
    hash->cantidad++;
    return true;
}

void* hash_borrar(hash_t* hash, const char* clave) {
    if (hash_carga(hash) < CARGA_MIN && hash->largo > LARGO_INICIAL) {
        if (!redimensionar(hash, hash->largo/FACTOR_REDIM))
            return NULL;
    }
    size_t indice = funcion_hash(clave, hash->largo);
    void* dato = NULL;
    lista_iter_t* iter = lista_iter_crear(hash->tabla[indice]);
    while (!lista_iter_al_final(iter)) {
        hash_campo_t* actual = lista_iter_ver_actual(iter);
        if (strcmp(actual->clave, clave) == 0) {
            dato = actual->valor;
            hash_campo_destruir(lista_iter_borrar(iter));
            hash->cantidad--;
            break;
        }
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
    return dato;
}

void* hash_obtener(const hash_t* hash, const char* clave) {
    hash_campo_t* campo = buscar_campo(hash, clave);
    if (!campo) return NULL;
    return campo->valor;
}

bool hash_pertenece(const hash_t *hash, const char *clave){
    return buscar_campo(hash, clave) != NULL;
}

size_t hash_cantidad(const hash_t *hash){
    return hash->cantidad;
}

void hash_destruir(hash_t *hash){
    for (size_t i=0; i < hash->largo; i++){
        while (!lista_esta_vacia(hash->tabla[i])) {
            hash_campo_t* campo = lista_borrar_primero(hash->tabla[i]);
            if (hash->destruir_dato)
                hash->destruir_dato(campo->valor);
            hash_campo_destruir(campo);
        }
        lista_destruir(hash->tabla[i], NULL);
    }
    free(hash->tabla);
    free(hash);
}

/******************************************************************************
 *                         PRIMITIVAS DEL ITERADOR
 *****************************************************************************/

hash_iter_t* hash_iter_crear(const hash_t* hash) {
    hash_iter_t* iter = malloc(sizeof(hash_iter_t));
    if (!iter) return NULL;
    iter->hash = hash;
    iter->indice = 0;
    iter->actual = NULL;
    if (hash->cantidad == 0)
        return iter;
    while (lista_esta_vacia(hash->tabla[iter->indice]))
        iter->indice++;
    iter->actual = lista_iter_crear(hash->tabla[iter->indice]);
    return iter;
}

bool hash_iter_avanzar(hash_iter_t* iter) {
    if (hash_iter_al_final(iter))
        return false;
    lista_iter_avanzar(iter->actual);
    if (!lista_iter_al_final(iter->actual))
        return true;
    lista_iter_destruir(iter->actual);
    iter->actual = NULL;
    if (iter->indice == iter->hash->largo-1)
        return true;
    iter->indice++;
    while (lista_esta_vacia(iter->hash->tabla[iter->indice])) {
        iter->indice++;
        if (iter->indice == iter->hash->largo)
            return true;
    }
    iter->actual = lista_iter_crear(iter->hash->tabla[iter->indice]);
    return true;
}

const char* hash_iter_ver_actual(const hash_iter_t* iter) {
    if (hash_iter_al_final(iter))
        return NULL;
    return ((hash_campo_t*)lista_iter_ver_actual(iter->actual))->clave;
}

bool hash_iter_al_final(const hash_iter_t* iter) {
    return iter->actual == NULL;
}

void hash_iter_destruir(hash_iter_t* iter) {
    if (iter->actual)
        lista_iter_destruir(iter->actual);
    free(iter);
}