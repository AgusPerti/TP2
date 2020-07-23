#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#define TAM 10

struct heap {
    void** datos;
    size_t tam;
    size_t cant;
    cmp_func_t comparar;
};

/* *****************************************************************
*                    PRIMITIVAS DEL HEAP
* *****************************************************************/

void swap (void **x, void **y) {
    void* valor = *x;
    *x = *y;
    *y = valor;
}

void upheap(heap_t *heap, size_t pos) {
    if (pos == 0) return;
    
    size_t pos_padre = (pos - 1) / 2;
    if (heap->comparar(heap->datos[pos_padre], heap->datos[pos]) < 0) {
        swap(&(heap->datos[pos_padre]), &(heap->datos[pos]));
        upheap(heap, pos_padre);
    }
    return;
}

size_t calculo_maximo(heap_t* heap, size_t* pos, size_t* izq, size_t* der) {
    size_t max = *pos;
    if (*izq < heap->cant && *der < heap->cant) {
        if (heap->comparar(heap->datos[*izq], heap->datos[*der]) > 0) {
            max = *izq;
        } else {
            max = *der;
        }
        if (heap->comparar(heap->datos[max], heap->datos[*pos]) < 0) {
            max = *pos;
        }
    } else if (*izq < heap->cant && heap->comparar(heap->datos[*izq], heap->datos[*pos]) > 0) {
        max = *izq;
    }
    return max;
}

void downheap(heap_t *heap, size_t pos) {
    if (pos >= heap->cant) return;
    
    size_t pos_hijo_izq = (2 * pos) + 1;
    size_t pos_hijo_der =  (2 * pos) + 2;
    size_t max = pos; 
    max = calculo_maximo(heap, &pos, &pos_hijo_izq, &pos_hijo_der);
    if (max != pos) {
        swap(&(heap->datos[max]), &(heap->datos[pos]));
        downheap(heap, max);
    }
    return;
}

bool heap_redimensionar(heap_t *heap, size_t capacidad_nueva) {
    void** datos_nuevos = realloc(heap->datos, capacidad_nueva * sizeof(void*));
    if (datos_nuevos == NULL) return false;
    
    heap->datos = datos_nuevos;
    heap->tam = capacidad_nueva;
    return true;
}

heap_t *heap_crear(cmp_func_t cmp) {
    heap_t* heap = malloc(sizeof(heap_t));
    if (!(heap)) return NULL;
    
    heap->datos = malloc(TAM * sizeof(void*));
    if (heap->datos == NULL) {
        free(heap);
        return NULL;
    }
    heap->tam = TAM;
    heap->cant = 0;
    heap->comparar = cmp;
    return heap;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp) {
    heap_t* heap = heap_crear(cmp);
    for (size_t i = 0; i < n; i++) {
        if (!(heap_encolar(heap, arreglo[i]))){
            heap_destruir(heap, NULL);
            return NULL;
            }
    }
    return heap;
}

size_t heap_cantidad(const heap_t *heap) {
    return heap->cant;
}

bool heap_esta_vacio(const heap_t *heap) {
    return heap->cant == 0;
}

bool heap_encolar(heap_t *heap, void *elem) {
    if (elem == NULL) return NULL;
    
    if (heap->cant == heap->tam - 1) {
        size_t capacidad_nueva = heap->tam * 2;
        if (!heap_redimensionar(heap, capacidad_nueva)) return false;
    }
    heap->datos[heap->cant] = elem;
    upheap(heap, heap->cant);
    heap->cant++;
    return true;
}

void *heap_desencolar(heap_t *heap) {
    if (heap_esta_vacio(heap)) return NULL;
    
    if (heap->cant * 4 <= heap->tam && heap->tam > TAM) {
        size_t capacidad_nueva = heap->tam / 2;
        if (!heap_redimensionar(heap, capacidad_nueva)) return NULL;
    }
    void* valor = heap->datos[0];
    swap(&(heap->datos[0]), &(heap->datos[heap->cant - 1]));
    
    heap->cant--;
    downheap(heap, 0);
    return valor;
}

void *heap_ver_max(const heap_t *heap) {
    return (heap_esta_vacio(heap)) ? NULL: heap->datos[0];
}

void heap_destruir(heap_t *heap, void (*destruir_elemento)(void *e)) {
    for (size_t i = 0; i < heap->cant; i++) {
        if (destruir_elemento != NULL) destruir_elemento(heap->datos[i]);
    }
    free(heap->datos);
    free(heap);
}
void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
    heap_t* heap_aux = heap_crear_arr(elementos,cant,cmp);

    for(size_t i = cant-1; i>=0 && !(heap_esta_vacio(heap_aux));i--){
        elementos[i] = heap_desencolar(heap_aux);
    }
    heap_destruir(heap_aux, NULL);
}
