#ifndef _PROCESAR_PACIENTES_H
#define _PROCESAR_PACIENTES_H

#include "hash.h"
#include <stdbool.h>

/*
Recibe los campos separados por split, en caso de que la urgencia sea URGENTE se procede a encolar en la cola de la especialidad
(indicada en el comando) dentro del hash correspondiente, si al urgencia es REGULAR se procede crear una lista con los datos del paciente
siendo estos datos su nombre y su anio de inscripcion, esa lista se la encola en el heap de la especialidad dentro del hash correspondiente.
*/
void pedir_turno(char** parametros, hash_t* pacientes, hash_t* especialidades_regulares, hash_t* especialidades_urgentes);

/* 
Verifica si los campos ingresados son validos, en caso de serlos devuelve True sino devuelve False.
*/
bool verificar_turno(char** parametros, hash_t* pacientes, hash_t* especialidades_regulares, const char* comando);


#endif // _PROCESAR_PACIENTES_H