#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#define CAPACIDAD_INICIAL 10
#define AGRANDAMIENTO 2
#define ACHICAR 2

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

size_t calculo_maximo(void** datos, size_t cant, size_t* pos, size_t* izq, size_t* der, cmp_func_t cmp) {
    size_t max = *pos;
    if (*izq < cant && *der < cant) {
        if (cmp(datos[*izq], datos[*der]) > 0) {
            max = *izq;
        } else {
            max = *der;
        }
        if (cmp(datos[max], datos[*pos]) < 0) {
            max = *pos;
        }
    } else if (*izq < cant && cmp(datos[*izq], datos[*pos]) > 0) {
        max = *izq;
    }
    return max;
}

void downheap(void** datos, size_t cant, size_t pos, cmp_func_t cmp) {
    if (pos >= cant) return;
    
    size_t pos_hijo_izq = (2 * pos) + 1;
    size_t pos_hijo_der =  (2 * pos) + 2;
    size_t max = pos; 
    max = calculo_maximo(datos, cant, &pos, &pos_hijo_izq, &pos_hijo_der, cmp);
    if (max != pos) {
        swap(&(datos[max]), &(datos[pos]));
        downheap(datos, cant, max, cmp);
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
    
    heap->datos = malloc(CAPACIDAD_INICIAL * sizeof(void*));
    if (heap->datos == NULL) {
        free(heap);
        return NULL;
    }
    heap->tam = CAPACIDAD_INICIAL;
    heap->cant = 0;
    heap->comparar = cmp;
    return heap;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp) {
    if (arreglo == NULL) {
        heap_t* heap = heap_crear(cmp);
        return heap;
    } 
    heap_t* heap = malloc(sizeof(heap_t));
    if (!heap) return NULL;
    heap->datos = malloc(n * sizeof(void*));
    
    for (size_t j = 0; j < n; j++) {
        heap->datos[j] = arreglo[j];
    }

    heap->tam = n;
    heap->cant = n;
    heap->comparar = cmp;

    for (int i = (int)n - 1; i >= 0; i--) {
        downheap(heap->datos, heap->cant, i, heap->comparar);
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
    
    if (heap->cant >= heap->tam - 1) {
        size_t capacidad_nueva = heap->tam * AGRANDAMIENTO;
        if (!heap_redimensionar(heap, capacidad_nueva)) return false;
    }
    heap->datos[heap->cant] = elem;
    upheap(heap, heap->cant);
    heap->cant++;
    return true;
}

void *heap_desencolar(heap_t *heap) {
    if (heap_esta_vacio(heap)) return NULL;
    
    if (heap->cant * 4 <= heap->tam && heap->tam > CAPACIDAD_INICIAL) {
        size_t capacidad_nueva = heap->tam / ACHICAR;
        heap_redimensionar(heap, capacidad_nueva);
    }
    void* valor = heap->datos[0];
    swap(&(heap->datos[0]), &(heap->datos[heap->cant - 1]));
    
    heap->cant--;
    downheap(heap->datos, heap->cant, 0, heap->comparar);
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
    for(int i = (int)(cant / 2) - 1; i >= 0; i--){
    	downheap(elementos, cant, i, cmp);
    }

    for(int i = (int)cant - 1; i > 0 ; i--){
        swap(&(elementos[i]), &(elementos[0]));
        downheap(elementos, i, 0, cmp);
    }
}
