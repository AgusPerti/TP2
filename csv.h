#ifndef __CSV__H_
#define __CSV__H_

#include "abb.h"
#include "hash.h"
#include "strutil.h"

/**
Haciendo uso de strutil (split) lee un archivo csv y, línea a línea,
va llamando al constructor que se pasa por parámetro. Dicho constructor se invoca
con la línea separada por split, sin fines de línea ni ninguna otra consideración,
y con el puntero extra que se pasa por parámetro.
Importante: la función constructor no debe guardar las referencias a las cadenas
dentro de arreglo de cadenas pasado por parámetros (hacer copias en caso de ser 
necesario); luego de invocarse el constructor se invoca a free_strv.

Se devuelve una lista con todos los elementos construidos. NULL en caso que el archivo
csv (indicado por la ruta pasada por parámetro) no exista. 
**/
abb_t* csv_crear_estructura(const char* ruta_csv);

// Documentacion...
hash_t* csv_crear_especialidades_regulares(const char* ruta_csv); 


hash_t* csv_crear_especialidades_urgentes(const char* ruta_csv);


#endif
