#define _POSIX_C_SOURCE 200809L //getline
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include "funciones_tp2.h"
#include "csv.h"
#include "heap.h"
#include "strutil.h"
#include "mensajes.h"

#define COMANDO_PEDIR_TURNO "PEDIR_TURNO"
#define COMANDO_ATENDER "ATENDER_SIGUIENTE"
#define COMANDO_INFORME "INFORME"
#define PRIORIDAD1 "URGENTE"
#define PRIORIDAD2 "REGULAR"

bool verificar_turno(const char** parametros, abb_t* pacientes, hash_t* especialidades) {
	if (!abb_pertenece(pacientes, parametros[0])) {
		printf(ENOENT_PACIENTE, parametros[0]);
		return false;
	}

	if (!hash_pertenece(especialidades, parametros[1])) {
		printf(ENOENT_ESPECIALIDAD, parametros[1]);
		return false;
	}

	if (strcmp(parametros[2], PRIORIDAD1) != 0 || strcmp(parametros[2], PRIORIDAD2) != 0) {
		printf(ENOENT_URGENCIA, parametros[2]);
		return false;
	}

	return true;
}

void pedir_turno(const char** parametros, abb_t* pacientes, hash_t* especialidades) {
	int* anio_inscripcion = abb_obtener(pacientes, parametros[0]);
	heap_t* cola_especialidad = hash_obtener(especialidades, parametros[1]);

}

void procesar_comando(const char* comando, const char** parametros, abb_t* doctores, abb_t* pacientes, hash_t* especialidades) {
	if (strcmp(comando, COMANDO_PEDIR_TURNO) == 0) {
		if (!verificar_turno(parametros, pacientes, especialidades)) return;
		pedir_turno(parametros, pacientes, especialidades);
	} else if (strcmp(comando, COMANDO_ATENDER) == 0) {

	} else if (strcmp(comando, COMANDO_INFORME) == 0) {

	} else {

	}
}

void eliminar_fin_linea(char* linea) {
	size_t len = strlen(linea);
	if (linea[len - 1] == '\n') {
		linea[len - 1] = '\0';
	}
}

void procesar_entrada(abb_t* doctores, abb_t* pacientes, hash_t* especialidades) {
	char* linea = NULL;
	size_t c = 0;
	while (getline(&linea, &c, stdin) > 0) {
		eliminar_fin_linea(linea);
		char** campos = split(linea, ':');
		if (campos[1] == NULL) {
			printf(ENOENT_FORMATO, linea);
			free_strv(campos);
			continue;	
		}
		char** parametros = split(campos[1], ',');
		procesar_comando(campos[0], parametros, doctores, pacientes, especialidades);
		free_strv(parametros);
		free_strv(campos);
	}
	free(linea);
}

int main(int argc, char** argv) {
	abb_t* doctores = csv_crear_estructura(argv[1]);
	abb_t* pacientes = csv_crear_estructura(argv[2]);
	hash_t* especialidades = csv_crear_especialidades(argv[1]);
	procesar_entrada(doctores, pacientes, especialidades);
	return 0;
}
