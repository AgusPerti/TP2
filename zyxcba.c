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
#define POS_CAMPO_NOMBRE_PAC 0 // posicion del nombre del paciente
#define POS_CAMPO_ESPECIALIDAD 1 // posicion del nombre de la especialidad
#define POS_CAMPO_URGENCIA 2 // posicion de la urgencia
#define POS_CAMPO_NOMBRE_DOC 0 // posicion del nombre del doctor
#define POS_CAMPO_INICIO 0 // posicion del nombre del doctor inicial
#define POS_CAMPO_FIN 1 // posicion del nombre del doctor final

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

bool verificar_atender(char** parametros, abb_t* doctores, hash_t* especialidades_regulares, hash_t* especialidades_urgentes, const char* comando){
	if (!strcmp(parametros[0], "")) {
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

bool verificar_informe(char** parametros, abb_t* doctores, const char* comando) {
	if (!parametros[POS_CAMPO_INICIO] || !parametros[POS_CAMPO_FIN]) {
		printf(ENOENT_PARAMS, comando);
		return false;
	}
	
	return true;
}

void actualizar_historial_doc(lista_t* doc_datos){
	size_t* cant_pacien_atendidos = lista_ver_primero(doc_datos);
	(*cant_pacien_atendidos)++;
}

void atender(char** parametros, abb_t* doctores, hash_t* especialidades_regulares, hash_t* especialidades_urgentes){
	lista_t* doc_datos = abb_obtener(doctores, parametros[0]);
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

int main(int argc, char** argv) {
	hash_t* pacientes = hash_crear(free);
	hash_t* regulares = hash_crear(NULL);
	hash_t* urgentes = hash_crear(NULL);
	abb_t* doctores = abb_crear(strcmp, NULL);
	
	csv_leer_archivo(argv[1], 0, pacientes, regulares, urgentes, doctores);
	csv_leer_archivo(argv[2], 1, pacientes, regulares, urgentes, doctores);

	procesar_entrada(doctores, pacientes, regulares, urgentes);

	destruir_estructuras(pacientes, regulares, urgentes, doctores);

	return 0;
}
