#include "vuelo.h"

/* Estructura del TDA vuelo */

struct vuelo{
	char* codigo;
	int	prioridad;
	char* hora;
	char* info;
};

/* Primitivas del TDA vuelo */

vuelo_t* vuelo_crear(char* codigo, int prioridad, char* hora, char* info){
	vuelo_t* vuelo = malloc(vuelo_t);
	if (!vuelo) return NULL;
	vuelo->codigo = codigo;
	vuelo->prioridad = prioridad;
	vuelo->hora = hora;
	vuelo->info = info;
	return vuelo;
}

char* vuelo_obteber_info(vuelo_t* vuelo){
	return vuelo->info;
}