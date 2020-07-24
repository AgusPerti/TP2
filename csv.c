#define _POSIX_C_SOURCE 200809L //getline
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv.h"
#include "heap.h"
#include "lista.h"
#include "cola.h"
#define SEPARADOR ','

/*typedef enum {
	ARCHIVO_DOC,
	ARCHIVO_PAC,
} tipo_archivo_t;*/

int comparacion(const void* elem1, const void* elem2) {
    if (*(int*)lista_ver_ultimo(elem1) < *(int*)lista_ver_ultimo(elem2)) {
        return -1;
    }

    if (*(int*)lista_ver_ultimo(elem1) > *(int*)lista_ver_ultimo(elem2)) {
        return 1;
    }
	
    return 0;
}

static void eliminar_fin_linea(char* linea) {
	size_t len = strlen(linea);
	if (linea[len - 1] == '\n') {
		linea[len - 1] = '\0';
	}
}

bool csv_leer_archivo(const char* ruta_csv, tipo_archivo_t tipo_archivo, hash_t* pacientes, hash_t* regulares, hash_t* urgentes, abb_t* doctores) {
	FILE* archivo = fopen(ruta_csv, "r");
	if (!archivo) return false;

	char* linea = NULL;
	size_t c = 0;

	while (getline(&linea, &c, archivo) > 0) {
		eliminar_fin_linea(linea);
		char** campos = split(linea, SEPARADOR);
		if (tipo_archivo == ARCHIVO_DOC) {
			if (!csv_crear_estructura_doctores(campos, doctores, regulares, urgentes)) {
				fclose(archivo);
				hash_destruir(pacientes);
				hash_destruir(regulares);
				hash_destruir(urgentes);
				abb_destruir(doctores);
			}
		} else {
			/*if (!csv_crear_estructuras_hash(campos, pacientes, regulares, urgentes)) {
				fclose(archivo);
			}*/
			hash_guardar(pacientes, campos[0], campos[1]);
		}
		free_strv(campos);
	}
	free(linea);
	fclose(archivo);
	return true;
}

bool csv_crear_estructura_doctores(char** campos, abb_t* arbol, hash_t* regulares, hash_t* urgentes) {
	lista_t* lista = lista_crear();
	if (!lista) return false;

	lista_insertar_primero(lista, campos[1]);
	//lista_insertar_ultimo();
	abb_guardar(arbol, campos[0], lista); 

	heap_t* heap = heap_crear(comparacion);
	if (!heap) return false;
	hash_guardar(regulares, campos[1], heap); 
	
	cola_t* cola = cola_crear();
	if (!cola) {
		heap_destruir(heap, NULL);
		return false;
	}
	hash_guardar(urgentes, campos[1], cola);

	return true;
}

/*bool csv_crear_estructuras_hash(char** campos, hash_t* pacientes) {
	hash_guardar(pacientes, campos[0], campos[1]);

	return true;
}*/
