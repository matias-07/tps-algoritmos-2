#include <stdbool.h>
#include <stddef.h>

/******************************************************************************
 *                        ÁRBOL BINARIO DE BÚSQUEDA
 *****************************************************************************/
struct abb;

typedef struct abb abb_t;

typedef int (*abb_comparar_clave_t) (const char*, const char*);

typedef void (*abb_destruir_dato_t) (void*);


/* Crea un abb con una función de comparación de claves
 * y una función de destrucción de datos.
 * Post: Se creó un abb vacío.
 */
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

/* Guarda un par (clave, valor) en el abb.
 * Pre: El abb fue creado.
 * Post: Devuelve true si se guardó correctamente
 * o false en caso contrario.
 */
bool abb_guardar(abb_t* arbol, const char* clave, void* dato);

/* Borra del abb el dato asociado a la clave y lo devuelve.
 * Pre: El abb fue creado.
 * Post: Se devolvió el dato o NULL si éste no se encontraba.
 */
void* abb_borrar(abb_t* arbol, const char* clave);

/* Devuelve el dato del abb asociado a la clave pasada
 * por parámetro.
 * Pre: El abb fue creado.
 * Post: Se devolvió el dato o NULL si éste no se encuentra.
 */
void* abb_obtener(const abb_t* arbol, const char* clave);

/* Devuelve true si la clave se encuentra en el abb,
 * false en caso contrario.
 * Pre: El abb fue creado.
 */
bool abb_pertenece(const abb_t* arbol, const char* clave);

/* Devuelve la cantidad de nodos del abb.
 * Pre: El abb fue creado.
 */
size_t abb_cantidad(const abb_t* arbol);

/* Destruye el abb (y sus datos si tiene una función
 * de destrucción).
 * Pre: El abb fue creado.
 * Post: El abb fue destruido.
 */
void abb_destruir(abb_t* arbol);

/* Itera sobre el abb in-order, aplicando la función
 * visitar sobre cada nodo hasta que la misma devuelva
 * false o hasta llegar al final.
 */
void abb_in_order(abb_t* arbol, bool visitar(const char*, void*, void*), void* extra);

/******************************************************************************
 *                            ITERADOR EXTERNO
 *****************************************************************************/

struct abb_iter;

typedef struct abb_iter abb_iter_t;

/* Crea un iterador in-order del abb pasado por
 * parámetro.
 * Pre: El abb fue creado.
 * Post: Se devolvió un iterador en la primera posición
 * del abb.*/
abb_iter_t* abb_iter_in_crear(const abb_t* arbol);

/* Avanza el iterador.
 * Pre: El iterador fue creado.
 * Post: Devuelve true si avanzó o false en caso
 * de estar al final.
 */
bool abb_iter_in_avanzar(abb_iter_t* iter);

/* Devuelve la clave actual.
 * Pre: El iterador fue creado.
 * Post: Se devolvió la clave actual o NULL si
 * el iterador está al final.
 */
const char* abb_iter_in_ver_actual(const abb_iter_t* iter);

/* Devuelve true si el iterador está al final
 * o false en caso contrario.
 * Pre: El iterador fue creado.
 */
bool abb_iter_in_al_final(const abb_iter_t* iter);

/* Destruye el iterador.
 * Pre: El iterador fue creado.
 * Post: El iterador fue destruido.
 */
void abb_iter_in_destruir(abb_iter_t* iter);