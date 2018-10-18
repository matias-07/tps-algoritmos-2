#define _POSIX_C_SOURCE 200809L
#include <stdlib.h>
#include <string.h>
#include "lista.h"
#include "hash.h"
#define LARGO_INICIAL 17

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

//Funcion de hashing

unsigned long
funcion_hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

// PRIMITIVAS DEL CAMPO DE HASH

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
    size_t indice = funcion_hashing(clave) % hash->largo;
    lista_iter_t* iter = lista_iter_crear(hash->tabla[indice]);
    // Se recorre la lista; si la clave ya existe, sólo se reemplaza el valor
    while (!lista_iter_al_final(iter)) {
        hash_campo_t* actual = lista_iter_ver_actual(iter);
        if (strcmp(actual->clave, clave) == 0) {
            actual->valor = dato;
            lista_iter_destruir(iter);
            return true;
        }
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
    hash_campo_t* campo = hash_campo_crear(clave, dato);
    if (!campo) return false;
    lista_insertar_ultimo(hash->tabla[indice], campo);
    return true;
}

void* hash_borrar(hash_t* hash, const char* clave) {
    size_t indice = funcion_hashing(clave) % hash->largo;
    void* dato = NULL;
    lista_iter_t* iter = lista_iter_crear(hash->tabla[indice]);
    while (!lista_iter_al_final(iter)) {
        hash_campo_t* actual = lista_iter_ver_actual(iter);
        if (strcmp(actual->clave, clave) == 0) {
            dato = actual->valor;
            hash_campo_destruir(lista_iter_borrar(iter));
            break;
        }
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
    return dato;
}

void* hash_obtener(const hash_t* hash, const char* clave) {
    size_t indice = funcion_hashing(clave) % hash->largo;
    void* dato = NULL;
    lista_iter_t* iter = lista_iter_crear(hash->tabla[indice]);
    while (!lista_iter_al_final(iter)) {
        hash_campo_t* actual = lista_iter_ver_actual(iter);
        if (strcmp(actual->clave, clave) == 0) {
            dato = actual->valor;
            break;
        }
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);
    return dato;
}

bool hash_pertenece(const hash_t *hash, const char *clave){
    size_t indice = funcion_hashing(clave) % hash->largo;
	bool pertenece = false;
	lista_iter_t* iter = lista_iter_crear(hash->tabla[indice]);
	while (!lista_iter_al_final(iter)) {
		hash_campo_t* actual = lista_iter_ver_actual(iter);
		if (strcmp(actual->clave, clave) == 0) {
			pertenece = true;
			break;
		}
	}
	lista_iter_destruir(iter);
	return pertenece;
}

size_t hash_cantidad(const hash_t *hash){
	return hash->cantidad;
}

void hash_destruir(hash_t *hash){
	for (size_t i=0; i<(hash->cantidad); i++){
		//size_t indice = obtener_balde_con_lista_no_vacia;
		while (!lista_esta_vacia(hash->tabla[indice])){
			void* borrado = lista_borrar_primero(hash->tabla[indice]);
			if (!(hash->destruir_dato)){
				hash->destruir_dato(borrado);
			}
			hash_campo_destruir((hash_campo_t*) borrado); //casteo el campo borrado de void* a hash_campo_t*
		}
	free(hash->tabla);		
	free(hash);	
}
