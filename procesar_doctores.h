#ifndef _PROCESAR_DOCTORES_H
#define _PROCESAR_DOCTORES_H

#include "abb.h"
#include "hash.h"
#include <stdbool.h>

/*
A partir del doctor recibido en el campo al que se le hizo split se atiende al paciente que pidio turno
para la especialidad del doctor indicado. En caso de haber un paciente urgente se atiende a ese paciente,
no se atenderan pacientes regulares hasta que la cantidad de pacientes urgentes para esa especialidad sea
0. Una vez atendido el paciente se actualiza la informacion del doctor sumandole 1 a la cantidad de pacientes
atendidos.
*/
void atender(char** parametros, abb_t* doctores, hash_t* especialidades_regulares, hash_t* especialidades_urgentes);

/*
Verifica si los campos ingresados son validos, en caso de serlos devuelve True sino devuelve False.
*/
bool verificar_atender(char** parametros, abb_t* doctores, hash_t* especialidades_regulares, hash_t* especialidades_urgentes, const char* comando);


#endif // _PROCESAR_DOCTORES_H