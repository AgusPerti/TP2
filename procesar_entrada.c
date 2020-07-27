#define _POSIX_C_SOURCE 200809L //getline
#include "procesar_entrada.h"
#include "mensajes.h"
#include "strutil.h"
#include "abb.h"
#include "hash.h"
#include "heap.h"
#include "lista.h"
#include "cola.h"
#include "procesar_pacientes.h"
#include "procesar_doctores.h"
#include "procesar_informe.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


void procesar_comando(const char* comando, char** parametros, abb_t* doctores, hash_t* pacientes, hash_t* especialidades_regulares, hash_t* especialidades_urgentes) {
	if (strcmp(comando, COMANDO_PEDIR_TURNO) == 0) {
		if (!verificar_turno(parametros, pacientes, especialidades_regulares, comando)) return;
		pedir_turno(parametros, pacientes, especialidades_regulares, especialidades_urgentes);
	} else if (strcmp(comando, COMANDO_ATENDER) == 0) {
		if (!verificar_atender(parametros, doctores, especialidades_regulares, especialidades_urgentes, comando)) return;
		atender(parametros, doctores, especialidades_regulares, especialidades_urgentes);

	} else if (strcmp(comando, COMANDO_INFORME) == 0) {
		if (!verificar_informe(parametros, doctores, comando)) return;
		hacer_informe(parametros, doctores);
	} else {
		printf(ENOENT_CMD, comando);
	}
}

void eliminar_fin_linea(char* linea) {
	size_t len = strlen(linea);

	if (linea[len - 1] == '\n') {
		linea[len - 1] = '\0';
	}
}

void procesar_entrada(abb_t* doctores, hash_t* pacientes, hash_t* especialidades_regulares, hash_t* especialidades_urgentes) {
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

bool visitar(const char* clave, void* dato, void* extra) {
	lista_destruir(dato, free);
	return true;
}

void destruir_estructuras(hash_t* pacientes, hash_t* regulares, hash_t* urgentes, abb_t* doctores) {

	hash_iter_t* iter_regulares = hash_iter_crear(regulares);
	hash_iter_t* iter_urgentes = hash_iter_crear(urgentes);

	while (!hash_iter_al_final(iter_regulares)) {
		heap_t* heap = hash_obtener(regulares, hash_iter_ver_actual(iter_regulares));
		while (!heap_esta_vacio(heap)) {
			lista_destruir(heap_desencolar(heap), free);
		}
		heap_destruir(heap, free);
		hash_iter_avanzar(iter_regulares);
	}
	while (!hash_iter_al_final(iter_urgentes)) {
		cola_destruir(hash_obtener(urgentes, hash_iter_ver_actual(iter_urgentes)), free);
		hash_iter_avanzar(iter_urgentes);
	}

	abb_in_order(doctores, visitar, NULL);

	hash_iter_destruir(iter_regulares);
	hash_iter_destruir(iter_urgentes);

	hash_destruir(regulares);
	hash_destruir(urgentes);
	hash_destruir(pacientes);
	abb_destruir(doctores);
}