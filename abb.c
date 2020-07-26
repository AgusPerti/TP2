#define _POSIX_C_SOURCE 200809L
#include "pila.h"
#include "abb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    IZQUIERDO,
    DERECHO,
} tipos_t;

typedef struct nodo_abb {
    struct nodo_abb* izq;
    struct nodo_abb* der;
    char* clave;
    void* dato;
} nodo_abb_t;

struct abb {
    nodo_abb_t* raiz;
    size_t cant;
    abb_destruir_dato_t destruir;
    abb_comparar_clave_t comparar;
};

struct abb_iter {
    pila_t* pila;
};

/* *****************************************************************
 *                    PRIMITIVAS DEL ABB
 * *****************************************************************/

abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato) {
    abb_t* abb = malloc(sizeof(abb_t));
    if (abb == NULL) {
        return NULL;
    }
    abb->raiz = NULL;
    abb->cant = 0;
    abb->destruir = destruir_dato;
    abb->comparar = cmp;
    return abb;
}

nodo_abb_t *nodo_abb_crear(char *clave, void *dato) {
    nodo_abb_t* nodo_abb = malloc(sizeof(nodo_abb_t));
    if (nodo_abb == NULL) {
        return NULL;
    }
    nodo_abb->clave = clave;
    nodo_abb->dato = dato;
    nodo_abb->izq = NULL;
    nodo_abb->der = NULL;
    return nodo_abb;
}

void encontrar_elemento(nodo_abb_t *actual, const char *clave, nodo_abb_t **padre, nodo_abb_t **elemento, const abb_t* arbol) {
    if (actual == NULL) {
        return;
    }
    if (arbol->comparar(clave, actual->clave) == 0) {
        *elemento = actual;
    }
    if (arbol->comparar(clave, actual->clave) < 0) {
        *padre = actual;
        encontrar_elemento(actual->izq, clave, padre, elemento, arbol);
    }
    if (arbol->comparar(clave, actual->clave) > 0) {
        *padre = actual;
        encontrar_elemento(actual->der, clave, padre, elemento, arbol);
    }
    return;
}

void tipo_de_hijos(nodo_abb_t *padre, nodo_abb_t *elemento, int *tipo_hijo, abb_t *arbol, int *cantidad_hijos, int * tipo_hijo_elemento) {
    // si es 1 seria hijo izq, si es 2 seria hijo der.
    if (padre != NULL) {
        if (arbol->comparar(padre->clave, elemento->clave) > 0) {
            *tipo_hijo = IZQUIERDO;
        } else {
            *tipo_hijo = DERECHO;
        }
    }
    // Veo la cantidad de hijos del elemento y de que tipo son los hijos.
    // Veo si el hijo del elemento es derecho o izquierdo para saber con cual reemplazar en el borrar.
    *cantidad_hijos = 0;
    *tipo_hijo_elemento = 0;
    if (elemento->izq != NULL) {
        *cantidad_hijos += 1;
        *tipo_hijo_elemento = IZQUIERDO;
    }
    if (elemento->der != NULL) {
        *cantidad_hijos += 1;
        *tipo_hijo_elemento = DERECHO;
    }
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato) {
    if (arbol->cant == 0) {
        char* copia_clave = strdup(clave);
        nodo_abb_t* nodo_abb = nodo_abb_crear(copia_clave, dato);
        arbol->raiz = nodo_abb;
        arbol->cant++;
        return true;
    }
    nodo_abb_t* padre = NULL;
    nodo_abb_t* elemento = NULL;
    encontrar_elemento(arbol->raiz, clave, &padre, &elemento, arbol);
    if (elemento != NULL) {
        if (arbol->destruir != NULL) {
            arbol->destruir(elemento->dato);
        }
        elemento->dato = dato;
    } else {
        char* copia_clave = strdup(clave);
        nodo_abb_t* nodo_abb = nodo_abb_crear(copia_clave, dato);
        if (nodo_abb == NULL) {
            free(copia_clave);
            return false;
        }
        if (arbol->comparar(clave, padre->clave) < 0) {
            padre->izq = nodo_abb;
        } else {
            padre->der = nodo_abb;
        }
        arbol->cant++;
    }
    return true;
}

void *abb_borrar(abb_t *arbol, const char *clave) {
    if (arbol->cant == 0) {
        return NULL;
    }
    nodo_abb_t* padre = NULL;
    nodo_abb_t* elemento = NULL;
    int tipo_hijo;
    int cantidad_hijos;
    int tipo_hijo_elemento;
    encontrar_elemento(arbol->raiz, clave, &padre, &elemento, arbol);
    /*Me fijo si el elemento a borrar se encuentra en el arbol*/
    if (elemento == NULL) {
        return NULL;
    }
    void* valor = elemento->dato;
    tipo_de_hijos(padre, elemento, &tipo_hijo, arbol, &cantidad_hijos, &tipo_hijo_elemento);
    // Caso 1: nodo sin hijos y Caso 2: nodo con 1 hijo
    if (cantidad_hijos < 2) {
        // Me fijo si el elemento a borrar es la raiz del ABB.
        if (padre == NULL) {
            if (tipo_hijo_elemento == IZQUIERDO) {
                arbol->raiz = elemento->izq;
            } else {
                arbol->raiz = elemento->der;
            }
        } else if (tipo_hijo == IZQUIERDO) {
                if (tipo_hijo_elemento == IZQUIERDO) {
                    padre->izq = elemento->izq;
                } else {
                    padre->izq = elemento->der;
                }
        } else if (tipo_hijo == DERECHO) {
            if (tipo_hijo_elemento == IZQUIERDO) {
                padre->der = elemento->izq;
            } else {
                padre->der = elemento->der;
            }
        }
        free(elemento->clave);
        free(elemento);
        arbol->cant--;
    }
    // Caso 3: nodo con 2 hijos. Tengo que buscar un reemplazante que puede ser el mas grande del lado izquierdo o el mas chico del derecho.
    // En mi caso voy a buscar el mas chico del lado derecho.
    // Voy a llamar a abb_borrar para el nodo reemplazante y luego reemplazo los datos del nodo que queria borrar por los del reemplazante.
    else {
        nodo_abb_t* elemento_2 = NULL;
        nodo_abb_t* padre_2 = NULL;
        encontrar_elemento(elemento->der, elemento->clave, &padre_2, &elemento_2, arbol);
        char* reemplazante_clave = strdup(padre_2->clave);
        void* reemplazante_dato = abb_borrar(arbol, padre_2->clave);
        free(elemento->clave);
        elemento->clave = reemplazante_clave;
        elemento->dato = reemplazante_dato;
    }
    return valor;
}

void *abb_obtener(const abb_t *arbol, const char *clave) {
    nodo_abb_t* padre = NULL;
    nodo_abb_t* elemento = NULL;
    encontrar_elemento(arbol->raiz, clave, &padre, &elemento, arbol);
    if (elemento == NULL) {
        return NULL;
    }
    return elemento->dato;
}

bool abb_pertenece(const abb_t *arbol, const char *clave) {
    nodo_abb_t* padre = NULL;
    nodo_abb_t* elemento = NULL;
    encontrar_elemento(arbol->raiz, clave, &padre, &elemento, arbol);
    if (elemento != NULL) {
        return true;
    }
    return false;
}

size_t abb_cantidad(abb_t *arbol) {
    return arbol->cant;
}

void recursivo_post_order(nodo_abb_t *actual, abb_t *arbol) {
    if (actual == NULL) {
        return;
    }
    recursivo_post_order(actual->izq, arbol);
    recursivo_post_order(actual->der, arbol);
    if (arbol->destruir != NULL) {
        arbol->destruir(actual->dato);
    }
    free(actual->clave);
    free(actual);
}

void abb_destruir(abb_t *arbol) {
    recursivo_post_order(arbol->raiz, arbol);
    free(arbol);
}

/* *****************************************************************
 *                    PRIMITIVA DEL ITERADOR INTERNO
 * *****************************************************************/

bool recorrido_in_order(nodo_abb_t *actual, bool visitar(const char *, void *, void *), void *extra) {
    if (actual == NULL) {
        return true;
    }
    if (!recorrido_in_order(actual->izq, visitar, extra)) {
        return false;
    }
    if (!visitar(actual->clave, actual->dato, extra)) {
        return false;
    }
    if (!recorrido_in_order(actual->der, visitar, extra)) {
        return false;
    }
    return true;
}

bool recorrido_por_rangos(nodo_abb_t *actual, bool visitar(const char *, void *, void *), void *extra, const char *inicio, const char *fin, abb_t* arbol) {
    if (actual == NULL) {
        return true;
    }
    if (inicio != NULL) {
        if (arbol->comparar(actual->clave, inicio) < 0) {
            recorrido_por_rangos(actual->der, visitar, extra, inicio, fin, arbol);
            return true;
        } 
    }
    if (fin != NULL) {
        if (arbol->comparar(actual->clave, fin) > 0) {
            recorrido_por_rangos(actual->izq, visitar, extra, inicio, fin, arbol);   
            return true;
        }
    } 
    /*Recorrido in order*/
    if (!recorrido_por_rangos(actual->izq, visitar, extra, inicio, fin, arbol)) {
        return false;
    }
    if (!visitar(actual->clave, actual->dato, extra)) {
        return false;
    }
    if (!recorrido_por_rangos(actual->der, visitar, extra, inicio, fin, arbol)) {
        return false;
    }

    return true;
}

void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra) {
    nodo_abb_t* actual = arbol->raiz;
    recorrido_in_order(actual, visitar, extra);
}

void abb_por_rangos(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra, const char *inicio, const char *fin) {
    nodo_abb_t* actual = arbol->raiz;
    recorrido_por_rangos(actual, visitar, extra, inicio, fin, arbol);
}

/* *****************************************************************
 *                    PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

void apilar_raiz_e_hijos_izquierdos(nodo_abb_t *actual, abb_iter_t *iter) {
    if (actual == NULL) {
        return;
    }
    pila_apilar(iter->pila, actual);
    while (actual->izq) {
        pila_apilar(iter->pila, actual->izq);
        actual = actual->izq;
    }
    return;
}

abb_iter_t *abb_iter_in_crear(const abb_t *arbol) {
    abb_iter_t* abb_iter = malloc(sizeof(abb_iter_t));
    if (abb_iter == NULL) {
        return NULL;
    }
    abb_iter->pila = pila_crear();
    apilar_raiz_e_hijos_izquierdos(arbol->raiz, abb_iter);
    return abb_iter;
}

bool abb_iter_in_avanzar(abb_iter_t *iter) {
    if (abb_iter_in_al_final(iter)) {
        return false;
    }
    nodo_abb_t* desapilado = pila_desapilar(iter->pila);
    if (desapilado->der != NULL) {
        apilar_raiz_e_hijos_izquierdos(desapilado->der, iter);
    }
    return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter) {
    nodo_abb_t* nodo = pila_ver_tope(iter->pila);
    if (nodo == NULL) {
        return NULL;
    }
    return nodo->clave;
}

bool abb_iter_in_al_final(const abb_iter_t *iter) {
    return pila_esta_vacia(iter->pila);
}

void abb_iter_in_destruir(abb_iter_t* iter) {
    pila_destruir(iter->pila);
    free(iter);
}
