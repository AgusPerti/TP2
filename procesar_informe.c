#define _POSIX_C_SOURCE 200809L
#include "procesar_informe.h"
#include "mensajes.h"
#include "lista.h"
#include "abb.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


bool fabricar_lista(const char* clave, void* dato, void* extra) { 
	lista_insertar_ultimo(extra, strdup(clave));
	return true;
}

bool imprimir_lista_cadenas(void* dato, void* extra, void *estructura) {
	(*(size_t*)extra)++;
	char* nombre = dato;
	lista_t* lista = abb_obtener(estructura, nombre);
	size_t cant_pacientes = *(size_t*)lista_ver_primero(lista);
	char* especialidad = lista_ver_ultimo(lista); 
	printf(INFORME_DOCTOR,*(size_t*)extra, nombre, especialidad, cant_pacientes);
	return true;
}

bool verificar_informe(char** parametros, abb_t* doctores, const char* comando) {
	if (!parametros[POS_CAMPO_INICIO] || !parametros[POS_CAMPO_FIN]) {
		printf(ENOENT_PARAMS, comando);
		return false;
	}
	
	return true;
}

void hacer_informe(char** parametros, abb_t* doctores) {
	lista_t* claves_doctores = lista_crear();
	char* inicio = parametros[POS_CAMPO_INICIO];
	char* fin = parametros[POS_CAMPO_FIN];

	if (!strcmp(parametros[POS_CAMPO_INICIO], "")) {
		inicio = NULL;
	}

	if (!strcmp(parametros[POS_CAMPO_FIN], "")) {
		fin = NULL;
	}

	abb_por_rangos(doctores, fabricar_lista, claves_doctores, inicio, fin);
	printf(DOCTORES_SISTEMA, lista_largo(claves_doctores));
	size_t cant_doctores = 0;
	lista_iterar(claves_doctores, imprimir_lista_cadenas, &cant_doctores, doctores);
	lista_destruir(claves_doctores, free);
}