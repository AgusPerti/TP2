#include "cola.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct nodo_cola {
    void *dato;
    struct nodo_cola *sig;
} nodo_cola_t;

struct cola {
    nodo_cola_t *prim;
    nodo_cola_t *ult;
    size_t cant;
};

cola_t* cola_crear(void) {
    cola_t* cola = malloc(sizeof(cola_t));
    if (cola == NULL) {
        return NULL;
    }
    cola->prim = NULL;
    cola->ult = cola->prim;
    cola->cant = 0;
    return cola;
}

nodo_cola_t* nodo_crear_cola(void* valor) {
    nodo_cola_t* nodo = malloc(sizeof(nodo_cola_t));
    if (nodo == NULL) {
        return NULL;
    }
    nodo->dato = valor;
    nodo->sig = NULL;
    return nodo;
}

bool cola_encolar(cola_t *cola, void* valor) {
    nodo_cola_t* nodo = nodo_crear_cola(valor);
    if (nodo == NULL) {
        return false;
    }
    if (cola_esta_vacia(cola)) {
        cola->prim = nodo;
        cola->ult = cola->prim;
    } else {
        cola->ult->sig = nodo;
        cola->ult = cola->ult->sig;
    }
    cola->ult->sig = NULL;
    cola->cant++;
    return true;
}

bool cola_esta_vacia(const cola_t *cola) {
    return cola->prim == NULL && cola->ult == NULL;
}

void* cola_ver_primero(const cola_t *cola) {
    if (cola_esta_vacia(cola)) {
        return NULL;
    }
    return cola->prim->dato;
}

void* cola_desencolar(cola_t *cola) {
    if (cola_esta_vacia(cola)) {
        return NULL;
    }
    void* valor = cola->prim->dato;
    nodo_cola_t* nuevo_prim = cola->prim->sig;
    free(cola->prim);
    cola->prim = nuevo_prim;
    if (cola->prim == NULL) {
        cola->ult = NULL;
    }
    cola->cant--;
    return valor;
}

void cola_destruir(cola_t *cola, void (*destruir_dato)(void*)) {
    while (!cola_esta_vacia(cola)) {
        void* dato = cola_desencolar(cola);
        if (destruir_dato != NULL) {
            destruir_dato(dato);
        }
    }
    free(cola);
}

size_t cola_cantidad(cola_t* cola) {
    return cola->cant;
}
