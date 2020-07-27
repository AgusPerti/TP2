#ifndef _PROCESAR_ENTRADA_H
#define _PROCESAR_ENTRADA_H

#include "abb.h"
#include "hash.h"


/*
Procesa lo recibido por entrada estandar (stdin) usando el split, en caso de ser correcto el formato ingresado procede
a llamar a la funcion siguiente pasandole el split de lo recibido, si no es correcto el formato se imprime un error y se pasa 
a la siguiente entrada.
*/
void procesar_entrada(abb_t* doctores, hash_t* pacientes, hash_t* especialidades_regulares, hash_t* especialidades_urgentes);

/*
Utiliza lo recibido por parametro de la funcion procesar_entrada, en caso de que el comando no sea el correcto se imprime un error, si esta
dentro de los comandos disponibles llama a las funciones correspondientes de hacer lo que indica el comando.
*/
void procesar_comando(const char* comando, char** parametros, abb_t* doctores, hash_t* pacientes, hash_t* especialidades_regulares, hash_t* especialidades_urgentes);

/*
Se encarga de destruir todas las estructuras utilizadas en el programa.
*/
void destruir_estructuras(hash_t* pacientes, hash_t* regulares, hash_t* urgentes, abb_t* doctores);


#endif // _PROCESAR_ENTRADA_H