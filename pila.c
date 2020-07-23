#include "pila.h"
#include <stdlib.h>
#define TAM 8

/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void** datos;
    size_t cantidad;  // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arreglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/
bool pila_redimensionar (pila_t* pila, size_t capacidad_nueva); //Redimensiona la pila

pila_t* pila_crear(void) {
    pila_t* pila = malloc(sizeof(pila_t));

    if (pila == NULL) {
        return NULL;
    }

    pila->datos = malloc(TAM * sizeof(void*));

    if (pila->datos == NULL) {
        free(pila);
        return NULL;
    }
    pila->cantidad = 0;
    pila->capacidad = TAM;
    return pila;
}

void pila_destruir(pila_t *pila) {
    free(pila->datos);
    free(pila);
}

bool pila_esta_vacia(const pila_t *pila) {
    return pila->cantidad == 0;
}

bool pila_apilar(pila_t *pila, void* valor) {
    if (pila->cantidad == pila->capacidad) {
        size_t capacidad_nueva = 2 * pila->capacidad;
        if (!pila_redimensionar(pila, capacidad_nueva)) {
            return false;
        }
    }
    pila->datos[pila->cantidad] = valor;
    (pila->cantidad)++;
    return true;
}

void* pila_ver_tope(const pila_t *pila) {
    if (!pila_esta_vacia(pila)) {
        return pila->datos[pila->cantidad - 1];
    }
    return NULL;
}

void* pila_desapilar(pila_t *pila) {
    if (pila->cantidad * 4 <= pila->capacidad && pila->capacidad > TAM) {
        size_t capacidad_nueva = pila->capacidad / 2;
        if (!pila_redimensionar(pila, capacidad_nueva)) {
            return false;
        }
    }
    if (!pila_esta_vacia(pila)) {
        (pila->cantidad)--;
        return pila->datos[pila->cantidad];
    }
    return NULL;
}

bool pila_redimensionar(pila_t *pila, size_t capacidad_nueva) {
    void** datos_nuevos = realloc(pila->datos, sizeof(void*) * capacidad_nueva);
    if (datos_nuevos == NULL) {
        return false;
    }
    pila->datos = datos_nuevos;
    pila->capacidad = capacidad_nueva;
    return true;
}
