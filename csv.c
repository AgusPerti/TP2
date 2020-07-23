#define _POSIX_C_SOURCE 200809L //getline
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv.h"
#include "heap.h"
#define SEPARADOR ','

int comparacion(const void* elem1, const void* elem2) {
    if (*(int*)elem1 < *(int*)elem2) {
        return -1;
    }
    if (*(int*)elem1 > *(int*)elem2) {
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

abb_t* csv_crear_estructura(const char* ruta_csv) {
	FILE* archivo = fopen(ruta_csv, "r");
	if (!archivo) {
		return NULL;
	}
	
	abb_t* arbol = abb_crear(strcmp, NULL);
	if (!arbol) {
		fclose(archivo);
		return NULL;
	}

	char* linea = NULL;
	size_t c = 0;
	while (getline(&linea, &c, archivo) > 0) {
		eliminar_fin_linea(linea);
		char** campos = split(linea, SEPARADOR);
		abb_guardar(arbol, campos[0], campos[1]); 
		free_strv(campos);
	}
	free(linea);
	fclose(archivo);
	return arbol;
}

hash_t* csv_crear_especialidades(const char* ruta_csv) { 
	FILE* archivo = fopen(ruta_csv, "r");
	if (!archivo) {
		return NULL;
	}
	
	hash_t* hash = hash_crear(NULL);
	if (!hash) {
		fclose(archivo);
		return NULL;
	}

	char* linea = NULL;
	size_t c = 0;
	while (getline(&linea, &c, archivo) > 0) {
		eliminar_fin_linea(linea);
		char** campos = split(linea, SEPARADOR);
		heap_t* cola = heap_crear(comparacion);
		hash_guardar(hash, campos[0], cola); 
		free_strv(campos);
	}
	free(linea);
	fclose(archivo);
	return hash;
}