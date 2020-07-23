#include "lista.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct nodo {
    void *dato;
    struct nodo *sig;
} nodo_t;

struct lista {
    struct nodo* prim;
    struct nodo* ult;
    size_t largo;
};

struct lista_iter {
    struct nodo* anterior;
    struct nodo* actual;
    lista_t* lista;
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

lista_t* lista_crear(void) {
    lista_t* lista = malloc(sizeof(lista_t));
    if (lista == NULL) {
        return NULL;
    }
    lista->prim = NULL;
    lista->ult = lista->prim;
    lista->largo = 0;
    return lista;
}

nodo_t* nodo_crear(void* valor) {
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if (nodo == NULL) {
        return NULL;
    }
    nodo->dato = valor;
    nodo->sig = NULL;
    return nodo;
}

bool lista_esta_vacia(const lista_t* lista) {
    return lista->prim == NULL && lista->ult == NULL;
}

bool lista_insertar_primero(lista_t* lista, void* dato) {
    nodo_t* nodo = nodo_crear(dato);
    if (nodo == NULL) {
        return false;
    }
    if (lista_esta_vacia(lista)) {
        lista->ult= nodo;
    }
    nodo->sig = lista->prim;
    lista->prim = nodo;
    lista->largo++;
    return true;
}

bool lista_insertar_ultimo(lista_t* lista, void* dato) {
    nodo_t* nodo = nodo_crear(dato);
    if (nodo == NULL) {
        return false;
    }
    if (lista_esta_vacia(lista)) {
        lista->prim = nodo;
    } else {
        lista->ult->sig = nodo;
    }
    lista->ult = nodo;
    lista->largo++;
    return true;
}

size_t lista_largo(const lista_t* lista) {
    return lista->largo;
}

void* lista_ver_primero(const lista_t* lista) {
    if (lista_esta_vacia(lista)) {
        return NULL;
    }
    return lista->prim->dato;
}

void* lista_ver_ultimo(const lista_t* lista) {
    if (lista_esta_vacia(lista)) {
        return NULL;
    }
    return lista->ult->dato;
}

void* lista_borrar_primero(lista_t* lista) {
    if (lista_esta_vacia(lista)) {
        return NULL;
    }
    void* valor = lista->prim->dato;
    nodo_t* nuevo_prim = lista->prim->sig;
    free(lista->prim);
    lista->prim = nuevo_prim;
    if (lista->prim == NULL) {
        lista->ult = NULL;
    }
    lista->largo--;
    return valor;
}

void lista_destruir(lista_t* lista, void destruir_dato(void*)) {
    while (!lista_esta_vacia(lista)) {
        void* dato = lista_borrar_primero(lista);
        if (destruir_dato != NULL) {
            destruir_dato(dato);
        }
    }
    free(lista);
}

/* *****************************************************************
 *                    PRIMITIVA DEL ITERADOR INTERNO
 * *****************************************************************/

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra) {
    nodo_t* actual = lista->prim;
    while (actual) {
        if (!visitar(actual->dato, extra)) {
            break;
        }
        actual = actual->sig;
    }
}

/* *****************************************************************
 *                    PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

lista_iter_t* lista_iter_crear(lista_t* lista) {
    lista_iter_t* lista_iter = malloc(sizeof(lista_iter_t));
    if( lista_iter == NULL) {
        return NULL;
    }
    lista_iter->lista = lista;

    lista_iter->anterior = NULL;
    lista_iter->actual = lista_iter->lista->prim;
    return lista_iter;
}

bool lista_iter_avanzar(lista_iter_t* iter) {
    if (lista_iter_al_final(iter)) {
        return false;
    }
    iter->anterior = iter->actual;
    iter->actual = iter->actual->sig;
    return true;
}

void* lista_iter_ver_actual(const lista_iter_t* iter) {
    if (lista_esta_vacia(iter->lista) || lista_iter_al_final(iter)) {
        return NULL;
    }
    return iter->actual->dato;
}

bool lista_iter_al_final(const lista_iter_t* iter) {
    return iter->actual == NULL;
}

void lista_iter_destruir(lista_iter_t* iter) {
    free(iter);
}

bool lista_iter_insertar(lista_iter_t* iter, void* dato) {
    if (iter->actual == iter->lista->prim) {
        lista_insertar_primero(iter->lista, dato);
        iter->actual = iter->lista->prim;
        return true;
    }
    if (lista_iter_al_final(iter)) {
        lista_insertar_ultimo(iter->lista, dato);
        iter->actual = iter->lista->ult;
        return true;
    }
    nodo_t* nuevo_nodo = nodo_crear(dato);
    if (nuevo_nodo == NULL) {
        return false;
    }
    iter->anterior->sig = nuevo_nodo;
    nuevo_nodo->sig = iter->actual;
    iter->actual = nuevo_nodo;
    iter->lista->largo++;
    return true;
}

void* lista_iter_borrar(lista_iter_t* iter) {
    if (lista_iter_al_final(iter)) {
        return NULL;
    }
    if (iter->actual == iter->lista->prim) {
        void* dato = lista_borrar_primero(iter->lista);
        iter->actual = iter->lista->prim;
        return dato;
    }
    void* dato = iter->actual->dato;
    nodo_t* nuevo_actual = iter->actual->sig;
    iter->anterior->sig = nuevo_actual;
    free(iter->actual);
    iter->actual = nuevo_actual;
    if (lista_iter_al_final(iter)) {
        iter->lista->ult = iter->anterior;
    }
    iter->lista->largo--;
    return dato;
}
