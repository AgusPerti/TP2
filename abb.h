#ifndef ABB_H
#define ABB_H
#include<stdbool.h>
#include <stddef.h>

/*Struct del abb*/
typedef struct abb abb_t;

/*Struct abb_iter*/
typedef struct abb_iter abb_iter_t;

/*Devuelve valor > 0 si el primero es mayor. Devuelve < 0 si es menor,
devuelve 0 si son iguales.
*/
typedef int (*abb_comparar_clave_t)(const char *, const char *);
typedef void (*abb_destruir_dato_t)(void *);

/****************************************************************************
 *
 *                                  ABB
 *
 * *************************************************************************/

/* Recibe una funcion para comparar claves entre si, una funcion del tipo destruir_dato
y crea un abb. Devuelve NULL en caso de error.
Post: Se creo el abb y se lo devolvio. Se devolvio NULL en caso de error */
abb_t *abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

/* Recibe el abb, una clave y un dato. Guarda la clave asociada al dato o reemplaza
el dato si la clave ya se encuentra en el abb. Devuelve false en caso de error.
Pre: Se creo el abb.
Post: Se guardo la clave asociada al dato. Se devolvio false en caso de error*/
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

/* Recibe el abb y una clave y borra el elemento del abb, devolviendo el dato o
NULL en caso de que el elemento no este.
Pre: Se creo el abb.
Post: Se borro la clave y se devolvio el dato, NULL en caso de que la clave no este*/
void *abb_borrar(abb_t *arbol, const char *clave);

/* Recibe el abb y la clave y devuelve el dato asociado o NULL en caso de que no este
la clave en en el abb.
Pre: Se creo el abb.
Post: Se devolvio el dato, NULL en caso de que la clave no este*/
void *abb_obtener(const abb_t *arbol, const char *clave);

/* Recibe el abb y la clave y devuelve true si la clave esta en el abb
o false en caso contrario.
Pre: Se creo el abb.
Post: Se devolvio true si la clave esta, false en caso contrario*/
bool abb_pertenece(const abb_t *arbol, const char *clave);

/* Recibe el abb y devuelve la cantidad de elementos.
Pre: Se creo el abb.
Post: Se devolvio la cantidad de elementos */
size_t abb_cantidad(abb_t *arbol);

/* Recibe el abb y lo destruye, incluyendo a los datos que le paso al usuario si
es que paso una funcion de destruccion.
Pre: Se creo el abb.
Post: Se destruyo el abb*/
void abb_destruir(abb_t *arbol);

/* Recibe el abb, una funcion visitar con la firma indicada y un extra. Itera in order ejecutando
visitar hasta que se le indique o hasta pasar por todas las claves.
Pre: Se creo el abb.
Post: Se itero ejecutando visitar hasta el elemento indicado */
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra);


void abb_por_rangos(abb_t *arbol, bool visitar(const char *, void *, void *), void *extra, const char *inicio, const char *fin);

/*******************************************************************************
*
*                               ITERADOR EXTERNO
*
********************************************************************************/

/* Recibe un abb y crea un iterador externo que avanza iterando in order,
devuelve NULL en caso de error.
Pre: Se creo el abb
Post: Se creo el iterador, se devolvio NULL en caso de error*/
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

/* Recibe el iterador y avanza un elemento, devolviendo true si logro avanzar,
false en caso de estar al final.
Pre: Se creo el iterador
Post: Se avanzo un elemento y se devolvio true, false si se estaba al final*/
bool abb_iter_in_avanzar(abb_iter_t *iter);

/* Recibe el iterador y devuelve la clave actual, NULL en caso de estar al final
Pre: Se creo el iterador.
Post: Se devolvio la clave actual, NULL en caso de estar al final*/
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

/* Recibe el iter y devuelve true si esta al final, false en caso contrario
Pre: Se creo el iterador.
Post: Se devolvio true si el iterador esta al final, false en caso contrario*/
bool abb_iter_in_al_final(const abb_iter_t *iter);

/* Recibe el iterador y lo destruye.
Pre: Se creo el iterador.
Post: Se destruyo el iterador*/
void abb_iter_in_destruir(abb_iter_t* iter);


#endif

