#include <stdbool.h>
#include "heap.h"
#include "lista.h"

/* Definicion de estructuras de los tda vuelo y sistema*/

typedef struct vuelo vuelo_t;

typedef struct sistema sistema_t;

/* Primitivas del tda vuelo */

//Crea un vuelo, recibe por parámetro el codigo del vuelo, su prioridad, su
//fecha, y vector de cadenas conteniendo toda la informacion que se guardara
//como una única cadena separada por espacios.
//Devuelve un puntero al vuelo.
//Post: el vuelo fue creado.
vuelo_t* vuelo_crear(char* codigo, int prioridad, char* fecha, char* datos[]);

//Recibe un vuelo por parametro y devuelve su codigo.
//Pre: el vuelo fue creado.
const char* vuelo_codigo(const vuelo_t* vuelo);

//Recibe un vuelo por parametro y devuelve su prioridad.
//Pre: el vuelo fue creado.
int vuelo_prioridad(const vuelo_t* vuelo);

//Recibe un vuelo por parametro y devuelve su informacion como cadena.
//Pre: el vuelo fue creado.
const char* vuelo_info(const vuelo_t* vuelo);

//Recibe un vuelo por parametro y devuelve su fecha y hora como cadena.
//Pre: el vuelo fue creado.
const char* vuelo_hora(const vuelo_t* vuelo);

//Recibe un vuelo por parametro y lo destruye.
//Pre: el vuelo fue creado.
//Post: el vuelo fue destruido
void vuelo_destruir(void* vuelo);

/* Primitivas del tda sistema */

//Crea un sistema, el cual contiene la informacion
//de los vuelos para ser utilizada cuando se necesite.
//Post: el sistema fue creado.
sistema_t* sistema_crear(void);

//Recibe por parámetro un vuelo a agregar y un sistema donde guardarlo, guarda
//copia de la informacion del vuelo en el sistema.
//Pre: el sistema y el vuelo fueron creados.
bool sistema_agregar_vuelo(sistema_t* sistema, vuelo_t* vuelo);

//Recibe por parámetro un sistema y un codigo de vuelo, y devuelve el vuelo
//asociado a ese codigo, si el codigo no está, devuelve NULL.
vuelo_t* sistema_ver_vuelo(const sistema_t* sistema, const char* codigo);

//Recibe por parametro un sistema, un entero k, un modo, y una fecha desde y
//hasta, devuelve una lista con los k vuelos ordenados por fecha segun el modo.
//Pre: el sistema fue creado.
lista_t* sistema_ver_tablero(sistema_t* sistema, int k, char* modo, char* desde, char* hasta);

//Recibe por parámetro un sistema y un entero k, y devuelve un heap con los k
//vuelos con mas prioridad.
//Pre: el sistema fue creado
heap_t* sistema_prioridades(const sistema_t* sistema, int k);

//Recibe por parámetro un sistema, fechas desde y hasta, y un modo,
//que determina el orden -ascendente o descendente- de los vuelos.
//Devuelve una lista con todos los vuelos correspondiente a ese intervalo de fechas.
//Pre: el sistema fue creado.
lista_t* sistema_obtener_vuelos(sistema_t* sistema, char* desde, char* hasta, char* modo);

//Recibe un sistema y un vuelo, y lo elimina del sistema.
//Pre: el sistema fue creado.
void sistema_eliminar_vuelo(sistema_t* sistema, vuelo_t* vuelo);

//Recibe un sistema por parámetro y lo destruye.
//Pre: el sistema fue creado.
//Post: el sistema fue destruido.
void sistema_destruir(sistema_t* sistema);
