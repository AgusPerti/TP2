#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "csv.h"
#include "procesar_entrada.h"

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
