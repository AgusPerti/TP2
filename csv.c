#define _POSIX_C_SOURCE 200809L //getline
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csv.h"
#include "heap.h"
#include "lista.h"
#include "cola.h"
#include "mensajes.h"
#include <ctype.h>

#define SEPARADOR ','
#define POS_CAMPO_PACIENTE 0
#define POS_CAMPO_DOCTOR 0 
#define POS_CAMPO_ANIO 1
#define POS_CAMPO_ESPECIALIDAD 1

int comparacion(const void* elem1, const void* elem2) {
	int num1 = atoi((char*)lista_ver_ultimo(elem1));
	int num2 = atoi((char*)lista_ver_ultimo(elem2));
    if (num1 < num2) {
        return 1;
    }

    if (num1 > num2) {
        return -1;
    }
	
    return 0;
}

bool es_numero_y_entero(char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (isdigit(str[i]) == 0) {
            return false;
        }
    }

    return true;
}

static void eliminar_fin_linea(char* linea) {
	size_t len = strlen(linea);
	if (linea[len - 1] == '\n') {
		linea[len - 1] = '\0';
	}
}

bool csv_leer_archivo(const char* ruta_csv, tipo_archivo_t tipo_archivo, hash_t* pacientes, hash_t* regulares, hash_t* urgentes, abb_t* doctores) {
	FILE* archivo = fopen(ruta_csv, "r");
	if (!archivo) {
		printf(ENOENT_ARCHIVO, ruta_csv);
		return false;
	} 
		
	char* linea = NULL;
	size_t c = 0;

	while (getline(&linea, &c, archivo) > 0) {
		eliminar_fin_linea(linea);
		char** campos = split(linea, SEPARADOR);

		if (tipo_archivo == ARCHIVO_DOC) {
			if (!csv_crear_estructuras(campos, doctores, regulares, urgentes)) {
				fclose(archivo);
				hash_destruir(pacientes);
				hash_destruir(regulares);
				hash_destruir(urgentes);
				abb_destruir(doctores);
			}
		} else {
			if (!es_numero_y_entero(campos[POS_CAMPO_ANIO])) {
				printf(ENOENT_ANIO, campos[POS_CAMPO_ANIO]);
			} else {
				hash_guardar(pacientes, campos[POS_CAMPO_PACIENTE], strdup(campos[POS_CAMPO_ANIO]));
			}
		}
		free_strv(campos);
	}
	free(linea);
	fclose(archivo);
	return true;
}

bool csv_crear_estructuras(char** campos, abb_t* arbol, hash_t* regulares, hash_t* urgentes) {
	lista_t* lista = lista_crear();
	if (!lista) return false;

	lista_insertar_ultimo(lista, strdup(campos[POS_CAMPO_ESPECIALIDAD]));
	size_t* cant_pac = malloc(sizeof(size_t));
	*cant_pac = 0;
	lista_insertar_primero(lista, cant_pac);
	abb_guardar(arbol, campos[POS_CAMPO_DOCTOR], lista); 

	heap_t* heap = heap_crear(comparacion);
	if (!heap) return false;
	if (hash_pertenece(regulares, campos[POS_CAMPO_ESPECIALIDAD])) {
		heap_destruir(heap, NULL);
	} else {
		hash_guardar(regulares, campos[POS_CAMPO_ESPECIALIDAD], heap); 
	}
	
	cola_t* cola = cola_crear();
	if (!cola) {
		heap_destruir(heap, NULL);
		return false;
	}
	if (hash_pertenece(urgentes, campos[POS_CAMPO_ESPECIALIDAD])) {
		cola_destruir(cola, NULL);
		return true;
	} 
	hash_guardar(urgentes, campos[POS_CAMPO_ESPECIALIDAD], cola);

	return true;
}

