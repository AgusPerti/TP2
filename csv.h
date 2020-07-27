#ifndef __CSV__H_
#define __CSV__H_

#include "abb.h"
#include "hash.h"
#include "strutil.h"

typedef enum {
	ARCHIVO_DOC,
	ARCHIVO_PAC,
} tipo_archivo_t;

/*
Luego de ser invocada por la funcion csv_leer_archivo crea 3 estructuras utilizando la linea separada por split,
un arbol con clave el nombre del doctor y como valor una lista con la cantidad de pacientes atendidos y su especialdiad,
un hash con clave el nombre de la especialidad y valor un heap y un hash con clave el nombre de la especialidad y valor
una cola.
*/
bool csv_crear_estructuras(char** campos, abb_t* arbol, hash_t* regulares, hash_t* urgentes);

/*
Haciendo uso del strutil (split) lee un archivo csv y, linea a linea, va llamando al constructor de estructuras.
Dicho constructor se invoca con la linea separada por split, sin fines de linea ni ninguna otra consideración.
*/
bool csv_leer_archivo(const char* ruta_csv, tipo_archivo_t tipo_archivo, hash_t* pacientes, hash_t* regulares, hash_t* urgentes, abb_t* doctores);

#endif
