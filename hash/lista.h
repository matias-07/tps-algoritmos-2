#include <stddef.h>
#include <stdbool.h>

/************************* NODO ***************************/

// Estructura de nodo. Posee un dato y un nodo próximo.
typedef struct nodo nodo_t;

// Crea un nodo cuyo dato es el pasado por parámetro.
// Post: devuelve un nodo.
nodo_t* nodo_crear(void* dato);

/********************* LISTA ENLAZADA *********************/

// La lista está planteada como una estructura de
// nodos enlazados.
typedef struct lista lista_t;

// Crea una lista.
// Post: devuelve una lista vacía.
lista_t* lista_crear(void);

// Devuelve true si la lista tiene al menos un
// elemento, false en caso contrario.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t* lista);

// Agrega un nuevo elemento al inicio de la lista.
// Devuelve false en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, que
// se encuentra en la primera posición.
bool lista_insertar_primero(lista_t* lista, void* dato);

// Agrega un nuevo elemento al final de la lista.
// Devuelve false en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, que
// se encuentra en la última posición.
bool lista_insertar_ultimo(lista_t* lista, void* dato);

// Quita el elemento en la primera posición de la
// lista, si no está vacía, y lo devuelve.
// Si está vacía, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el elemento que se encontraba en la
// primera posición o NULL si la lista no tenía elementos.
void* lista_borrar_primero(lista_t* lista);

// Obtiene el elemento que se encuentra en la primera
// posición de la lista, si no está vacía.
// Si está vacía, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el elemento que se encuentra en la
// primera posición de la lista, si no está vacía.
void* lista_ver_primero(const lista_t* lista);

// Obtiene el elemento que se encuentra en la última posición
// de la lista, si no está vacía. Si está vacía, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el elemento que se encuentra en la última
// posición de la lista, si no está vacía.
void* lista_ver_ultimo(const lista_t* lista);

// Obtiene el largo de la lista.
// Pre: la lista fue creada.
// Post: se devolvió el largo de la lista.
size_t lista_largo(const lista_t* lista);

// Destruye la lista. Si se recibe la función destruir_dato
// por parámetro, la aplica para cada elemento de la lista.
// Pre: la lista fue creada. destruir_dato es una función capaz
// de destruir los elementos de la lista, o NULL en caso de que
// no sea necesaria.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t* lista, void destruir_dato(void*));

/************* PRIMITIVA DEL ITERADOR INTERNO *************/

// Recorre la lista y llama a la función visitar en cada
// iteración.
// Pre: la lista fue creada. visitar es una función que devuelve
// un booleano que decide si se sigue recorriendo la lista.
// Post: se llamó a la función visitar al menos una vez.
void lista_iterar(lista_t* lista, bool visitar(void* dato, void* extra), void* extra);

/******************* ITERADOR EXTERNO *********************/

// El iterador externo está implementado como una estructura
// de nodos enlazados.
typedef struct lista_iter lista_iter_t;

// Crea un iterador inicializado en el primer elemento de la lista
// pasada por parámetro.
// Pre: la lista fue creada.
// Post: se devolvió un iterador que apunta al primer elemento de
// la lista.
lista_iter_t* lista_iter_crear(lista_t* lista);

// Avanza el iterador al siguiente elemento de la lista.
// Si el iterador ya se encuentra al final de la lista,
// devuelve false.
// Pre: el iterador fue creado.
// Post: el iterador apunta al siguiente elemento, que
// ahora es el actual.
bool lista_iter_avanzar(lista_iter_t* iter);

// Obtiene el dato actual al que apunta el iterador.
// Pre: el iterador fue creado.
// Post: se devolvió el dato actual.
void* lista_iter_ver_actual(const lista_iter_t* iter);

// Devuelve true si el iterador se encuentra al
// final de la lista, false en caso contrario.
// Pre: el iterador fue creado.
bool lista_iter_al_final(const lista_iter_t* iter);

// Destruye el iterador.
// Pre: el iterador fue creado.
// Post: se destruyó el iterador.
void lista_iter_destruir(lista_iter_t* iter);

// Agrega un elemento en la posición actual de la lista.
// Devuelve false en caso de error.
// Pre: el iterador fue creado.
// Post: se insertó el nuevo elemento en la posición actual
// de la lista. El iterador ahora apunta a este nuevo elemento.
bool lista_iter_insertar(lista_iter_t* iter, void* dato);

// Quita el elemento en la posición actual de la lista
// y lo devuelve.
// Pre: el iterador fue creado.
// Post: se devolvió el elemento eliminado de la lista
// y el iterador ahora apunta al siguiente elemento.
void* lista_iter_borrar(lista_iter_t* iter);

/************************ PRUEBAS**************************/

// Ejecuta todas las pruebas.
void pruebas_lista_alumno(void);