#define _POSIX_C_SOURCE 200809L
#include "procesar_pacientes.h"
#include "mensajes.h"
#include "lista.h"
#include "hash.h"
#include "heap.h"
#include "cola.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


bool verificar_turno(char** parametros, hash_t* pacientes, hash_t* especialidades_regulares, const char* comando) {
	if (!strcmp(parametros[POS_CAMPO_NOMBRE_PAC], "") || !strcmp(parametros[POS_CAMPO_ESPECIALIDAD], "") || !strcmp(parametros[POS_CAMPO_URGENCIA], "")) {
		printf(ENOENT_PARAMS, comando);
		return false;
	}

	if (!hash_pertenece(pacientes, parametros[POS_CAMPO_NOMBRE_PAC])) {
		printf(ENOENT_PACIENTE, parametros[POS_CAMPO_NOMBRE_PAC]);
		return false;
	}

	if (!hash_pertenece(especialidades_regulares, parametros[POS_CAMPO_ESPECIALIDAD])) {
		printf(ENOENT_ESPECIALIDAD, parametros[POS_CAMPO_ESPECIALIDAD]);
		return false;
	}

	if (strcmp(parametros[POS_CAMPO_URGENCIA], PRIORIDAD1) != 0 && strcmp(parametros[POS_CAMPO_URGENCIA], PRIORIDAD2) != 0) {
		printf(ENOENT_URGENCIA, parametros[POS_CAMPO_URGENCIA]);
		return false;
	}

	return true;
}

void pedir_turno(char** parametros, hash_t* pacientes, hash_t* especialidades_regulares, hash_t* especialidades_urgentes) {
	size_t cantidad_pacientes = 0;
	char* campo_nombre = strdup(parametros[POS_CAMPO_NOMBRE_PAC]);

	if (strcmp(parametros[POS_CAMPO_URGENCIA], PRIORIDAD1) == 0) {
		cola_encolar(hash_obtener(especialidades_urgentes, parametros[POS_CAMPO_ESPECIALIDAD]), campo_nombre); 
	} else {
		lista_t* datos_paciente = lista_crear();
		if (!datos_paciente) return;
		lista_insertar_primero(datos_paciente, strdup(hash_obtener(pacientes, campo_nombre)));
		lista_insertar_primero(datos_paciente, campo_nombre);
		heap_encolar(hash_obtener(especialidades_regulares, parametros[POS_CAMPO_ESPECIALIDAD]), datos_paciente); 
	}

	cantidad_pacientes = heap_cantidad(hash_obtener(especialidades_regulares, parametros[POS_CAMPO_ESPECIALIDAD])) + cola_cantidad(hash_obtener(especialidades_urgentes, parametros[POS_CAMPO_ESPECIALIDAD]));
	printf(PACIENTE_ENCOLADO, campo_nombre);
	printf(CANT_PACIENTES_ENCOLADOS, cantidad_pacientes, parametros[POS_CAMPO_ESPECIALIDAD]);
}