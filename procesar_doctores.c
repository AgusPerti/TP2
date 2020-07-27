#include "procesar_doctores.h"
#include "mensajes.h"
#include "abb.h"
#include "lista.h"
#include "hash.h"
#include "heap.h"
#include "cola.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


bool verificar_atender(char** parametros, abb_t* doctores, hash_t* especialidades_regulares, hash_t* especialidades_urgentes, const char* comando){
	if (!strcmp(parametros[POS_CAMPO_NOMBRE_DOC], "")) {
		printf(ENOENT_PARAMS, comando);
		return false;
	}

	if(!abb_pertenece(doctores, parametros[POS_CAMPO_NOMBRE_DOC])){
		printf(ENOENT_DOCTOR, parametros[POS_CAMPO_NOMBRE_DOC]);
		return false;
	}

	lista_t* doc_datos = abb_obtener(doctores, parametros[POS_CAMPO_NOMBRE_DOC]);
	
	size_t cantidad_pacientes = heap_cantidad(hash_obtener(especialidades_regulares, lista_ver_ultimo(doc_datos))) + cola_cantidad(hash_obtener(especialidades_urgentes, lista_ver_ultimo(doc_datos)));
	
	if(cantidad_pacientes == 0){
		printf(SIN_PACIENTES);
		return false;
	}

	return true;
}

void actualizar_historial_doc(lista_t* doc_datos){
	size_t* cant_pacien_atendidos = lista_ver_primero(doc_datos);
	(*cant_pacien_atendidos)++;
}

void atender(char** parametros, abb_t* doctores, hash_t* especialidades_regulares, hash_t* especialidades_urgentes) {
	lista_t* doc_datos = abb_obtener(doctores, parametros[POS_CAMPO_NOMBRE_DOC]);
	char* doc_esp = lista_ver_ultimo(doc_datos);
	
	lista_t* datos_pacientes;
	size_t cant_urgentes = cola_cantidad(hash_obtener(especialidades_urgentes, doc_esp));
	size_t cant_regulares = heap_cantidad(hash_obtener(especialidades_regulares, doc_esp));
	char* nom_paciente;
	
	if(cant_urgentes > 0){
		nom_paciente = cola_desencolar(hash_obtener(especialidades_urgentes, doc_esp));
	 	printf(PACIENTE_ATENDIDO,nom_paciente);
		cant_urgentes--;
		free(nom_paciente);
	} else {
	 	datos_pacientes = heap_desencolar(hash_obtener(especialidades_regulares, doc_esp));
	 	nom_paciente = lista_ver_primero(datos_pacientes);
		cant_regulares--; 
	 	printf(PACIENTE_ATENDIDO, nom_paciente);
	 	
	 	lista_destruir(datos_pacientes, free);
		
	}
	
	actualizar_historial_doc(doc_datos);
	printf(CANT_PACIENTES_ENCOLADOS, cant_regulares + cant_urgentes, doc_esp);
}

