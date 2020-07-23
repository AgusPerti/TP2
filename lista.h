#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

struct lista;
typedef struct lista lista_t;

struct lista_iter;
typedef struct lista_iter lista_iter_t;

/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

// Crea una lista.
// En caso de fallar devuelve NULL.
// Post: devuelve una lista vacía.
lista_t* lista_crear(void);

// Devuelve verdadero si la lista no tiene elementos, false en lo contrario.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t* lista);

// Agrega un elemento al principio de la lista y devuelve verdadero.
// Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, el elemento se encuentra al principio.
bool lista_insertar_primero(lista_t* lista, void* dato);

// Agrega un elemento al final de la lista y devuelve verdadero.
// Devuelve falso en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, el elemento se encuentra al final.
bool lista_insertar_ultimo(lista_t* lista, void* dato);

// Devuelve el largo de la lista, es decir la cantidad de elementos en la misma.
// Pre: la lista fue creada.
// Post: devolvió el largo de la lista.
size_t lista_largo(const lista_t* lista);

// Obtiene el valor del primer elemento de la lista. Si la lista tiene elementos,
// se devuelve el valor del primero, si esta vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el valor del primer elemento de la lista cuando no esta vacía.
void* lista_ver_primero(const lista_t* lista);

// Obtiene el valor del ultimo elemento de la lista. Si la lista tiene elementos,
// se devuelve el valor del ultimo, si esta vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el valor del ultimo elemento de la lista cuando no esta vacía.
void* lista_ver_ultimo(const lista_t* lista);

// Saca el primer elemento de la lista. Si la lista tiene elementos, se quita el primero
// y se devuelve su valor, si esta vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el valor del primer elemento anterior, la lista contiene un elemento
// menos, si es que la lista no estaba vacía.
void* lista_borrar_primero(lista_t* lista);

// Destruye la lista. Si se recibe la función destruir_dato por parámetro, para cada uno
// de los elementos de la lista se llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir los datos
// de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t* lista, void destruir_dato(void*));

/* ******************************************************************
 *                    PRIMITIVAS DEL ITERADOR INTERNO
 * *****************************************************************/

// Itera sobre la lista. Si recibe una función visitar por parámetro, por cada uno de
// los elementos de la lista se llama a visitar. En caso de que la lista no tenga elementos
// devuelve NULL. Visitar devuelve true si la lista a iterar esta bien y devolvera false en caso de que
// la lista este mal.
// Pre: la lista fue creada.
// Post: iteró sobre la lista y llamó a la función visitar para los elementos de la lista.
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);

/* ******************************************************************
 *                    PRIMITIVA DEL ITERADOR EXTERNO
 * *****************************************************************/

// Crea un iterador.
// En caso de fallar devuelve NULL.
lista_iter_t* lista_iter_crear(lista_t* lista);

// Avanza sobre la lista, al no poder avanzar mas devuelve false.
// Pre: el iterador y la lista fueron creados.
// Post: itera sobre la lista.
bool lista_iter_avanzar(lista_iter_t* iter);

// Devuelve el valor del elemento en la posicion actual del iterador, en caso de estar al final
// devuelve NULL.
// Pre: el iterador fue creado.
// Post: devuelve el valor del elemento en la posicion actual.
void* lista_iter_ver_actual(const lista_iter_t* iter);

// Se fija si el iterador se encuentra al final de la lista, en caso de que no lo este
// devuelve falso.
// Pre: el iterador fue creado.
bool lista_iter_al_final(const lista_iter_t* iter);

// Se encarga de destruir el iterador.
// Pre: el iterador fue creado.
// Post: destruyo el iterador.
void lista_iter_destruir(lista_iter_t* iter);

// Inserta un nuevo elemento en la posicion actual del iterador en la lista, en caso
// de no ser posible devuelve falso.
// Pre: el iterador fue creado.
// Post: se agrego un nuevo elemento en la lista.
bool lista_iter_insertar(lista_iter_t* iter, void* dato);

// Borra el elemento que se encuentra en la posicion actual del iterador en la lista
// y devuelve su valor.
// Pre: el iterador fue creado.
// Post: se devolvio el valor del elemento borrado, la lista contiene un elemento menos.
void* lista_iter_borrar(lista_iter_t* iter);

void pruebas_lista_alumno(void);

#endif //LISTA_H
