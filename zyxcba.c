#define _POSIX_C_SOURCE 200809L //getline
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include "funciones_tp2.h"
#include "csv.h"
#include "heap.h"
#include "pila.h"
#include "cola.h"
#include "lista.h"
#include "strutil.h"
#include "mensajes.h"

#define COMANDO_PEDIR_TURNO "PEDIR_TURNO"
#define COMANDO_ATENDER "ATENDER_SIGUIENTE"
#define COMANDO_INFORME "INFORME"
#define PRIORIDAD1 "URGENTE"
#define PRIORIDAD2 "REGULAR"
#define ANIO_ACTUAL 2020

bool verificar_turno(char** parametros, abb_t* pacientes, hash_t* especialidades_regulares) {
	if (!abb_pertenece(pacientes, parametros[0])) {
		printf(ENOENT_PACIENTE, parametros[0]);
		return false;
	}

	if (!hash_pertenece(especialidades_regulares, parametros[1])) {
		printf(ENOENT_ESPECIALIDAD, parametros[1]);
		return false;
	}

	if (strcmp(parametros[2], PRIORIDAD1) != 0 && strcmp(parametros[2], PRIORIDAD2) != 0) {
		printf("%sx\n", parametros[2]);
		printf(ENOENT_URGENCIA, parametros[2]);
		return false;
	}

	return true;
}

void pedir_turno(char** parametros, abb_t* pacientes, hash_t* especialidades_regulares, hash_t* especialidades_urgentes) {
	size_t cantidad_pacientes = 0;
	if (strcmp(parametros[2], PRIORIDAD1) == 0) {
		cola_encolar(hash_obtener(especialidades_urgentes, parametros[1]), parametros[0]);
	} else {
		lista_t* datos_paciente = lista_crear();
		if (!datos_paciente) return;
		lista_insertar_primero(datos_paciente, abb_obtener(pacientes, parametros[0]));
		lista_insertar_primero(datos_paciente, parametros[0]);
		heap_encolar(hash_obtener(especialidades_regulares, parametros[1]), datos_paciente);
	}
	cantidad_pacientes = heap_cantidad(hash_obtener(especialidades_regulares, parametros[1])) + cola_cantidad(hash_obtener(especialidades_urgentes, parametros[1]));
	printf(PACIENTE_ENCOLADO, parametros[0]);
	printf(CANT_PACIENTES_ENCOLADOS, cantidad_pacientes, parametros[1]);
}

void procesar_comando(const char* comando, char** parametros, abb_t* doctores, abb_t* pacientes, hash_t* especialidades_regulares, hash_t* especialidades_urgentes) {
	if (strcmp(comando, COMANDO_PEDIR_TURNO) == 0) {
		if (!verificar_turno(parametros, pacientes, especialidades_regulares)) return;
		pedir_turno(parametros, pacientes, especialidades_regulares, especialidades_urgentes);
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

void procesar_entrada(abb_t* doctores, abb_t* pacientes, hash_t* especialidades_regulares, hash_t* especialidades_urgentes) {
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
		procesar_comando(campos[0], parametros, doctores, pacientes, especialidades_regulares, especialidades_urgentes);
		free_strv(parametros);
		free_strv(campos);
	}
	free(linea);
}

int main(int argc, char** argv) {
	abb_t* doctores = csv_crear_estructura(argv[1]);
	abb_t* pacientes = csv_crear_estructura(argv[2]);
	hash_t* especialidades_regulares = csv_crear_especialidades_regulares(argv[1]);
	hash_t* especialidades_urgentes = csv_crear_especialidades_urgentes(argv[1]);
	procesar_entrada(doctores, pacientes, especialidades_regulares, especialidades_urgentes);
	return 0;
}
