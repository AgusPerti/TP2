#ifndef _PROCESAR_INFORME_H
#define _PROCESAR_INFORME_H

#include "abb.h"
#include <stdbool.h>

/*
Itera el arbol de doctores y guarda en una lista los nombres de los doctores indicados en el comando, en caso de no indicarse ningun doctor guardara
en la lista todos los doctores dentro del arbol en orden alfabetico. Luego itera sobre esa lista imprimiendo los datos de cada doctor, es decir 
cantidad de pacientes atendidos hasta el momento y su respectiva especialidad.
*/
void hacer_informe(char** parametros, abb_t* doctores);

/*
Verifica si los campos ingresados son validos, en caso de serlos devuelve True sino devuelve False.
*/
bool verificar_informe(char** parametros, abb_t* doctores, const char* comando);


#endif // _PROCESAR_INFORME_H

