#include <stdbool.h>
#include "heap.h"

/* Primitivas del tda vuelo */

typedef struct vuelo vuelo_t;

vuelo_t* vuelo_crear(char* codigo, int prioridad, char* fecha, char* datos[]);

const char* vuelo_codigo(const vuelo_t* vuelo);

int vuelo_prioridad(const vuelo_t* vuelo);

const char* vuelo_info(const vuelo_t* vuelo);


/* Primitivas del tda sistema */

typedef struct sistema sistema_t;

sistema_t* sistema_crear(void);

bool sistema_agregar_vuelo(sistema_t* sistema, vuelo_t* vuelo);

vuelo_t* sistema_ver_vuelo(const sistema_t* sistema, const char* codigo);

heap_t* sistema_prioridades(const sistema_t* sistema, int k);

void sistema_destruir(sistema_t* sistema);