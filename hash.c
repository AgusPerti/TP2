#define _POSIX_C_SOURCE 200809L
#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#define TAM 23
#define FACTOR_DE_CARGA 0.7
#define FACTOR_DE_CARGA_DE_MAS 0.35
#define FACTOR_DE_REDIMENSION 2
#define FACTOR_DE_REDIMENSION_HACIA_ABAJO 2

/* *****************************************************************
 *                    STRUCTS
 * *****************************************************************/

typedef enum {
    VACIO,
    OCUPADO,
    BORRADO,
} estados_t;

typedef struct campo_hash{
    char* clave;
    void* dato;
    int estado;
} campo_hash_t;

struct hash{
    campo_hash_t* tabla;
    size_t tam;
    size_t cant;
    hash_destruir_dato_t destruir_dato;
};

struct hash_iter{
    const hash_t* hash;
    struct campo_hash actual;
    size_t posicion_actual;
};

/* *****************************************************************
 *                    FUNCION DE HASHING
 * *****************************************************************/

/**
 * `murmurhash.h' - murmurhash
 *
 * copyright (c) 2014 joseph werle <joseph.werle@gmail.com>
 * https://github.com/jwerle/murmurhash.c.git
 */

uint32_t
murmurhash (const char *key, uint32_t len, uint32_t seed) {
  uint32_t c1 = 0xcc9e2d51;
  uint32_t c2 = 0x1b873593;
  uint32_t r1 = 15;
  uint32_t r2 = 13;
  uint32_t m = 5;
  uint32_t n = 0xe6546b64;
  uint32_t h = 0;
  uint32_t k = 0;
  uint8_t *d = (uint8_t *) key; // 32 bit extract from `key'
  const uint32_t *chunks = NULL;
  const uint8_t *tail = NULL; // tail - last 8 bytes
  int i = 0;
  int l = len / 4; // chunk length

  h = seed;

  chunks = (const uint32_t *) (d + l * 4); // body
  tail = (const uint8_t *) (d + l * 4); // last 8 byte chunk of `key'

  // for each 4 byte chunk of `key'
  for (i = -l; i != 0; ++i) {
    // next 4 byte chunk of `key'
    k = chunks[i];

    // encode next 4 byte chunk of `key'
    k *= c1;
    k = (k << r1) | (k >> (32 - r1));
    k *= c2;

    // append to hash
    h ^= k;
    h = (h << r2) | (h >> (32 - r2));
    h = h * m + n;
  }

  k = 0;

  // remainder
  switch (len & 3) { // `len % 4'
    case 3: k ^= (tail[2] << 16);
    case 2: k ^= (tail[1] << 8);

    case 1:
      k ^= tail[0];
      k *= c1;
      k = (k << r1) | (k >> (32 - r1));
      k *= c2;
      h ^= k;
  }

  h ^= len;

  h ^= (h >> 16);
  h *= 0x85ebca6b;
  h ^= (h >> 13);
  h *= 0xc2b2ae35;
  h ^= (h >> 16);

  return h;
}

/* *****************************************************************
 *                    PRIMITIVAS DEL HASH
 * *****************************************************************/

bool hash_redimensionar(hash_t* hash, bool disminuir) {
    size_t nuevo_tam = 0;
    if (disminuir == true) {
        nuevo_tam = (hash->tam) / FACTOR_DE_REDIMENSION_HACIA_ABAJO;
    } else {
        nuevo_tam = (hash->tam) * FACTOR_DE_REDIMENSION;
    }
    campo_hash_t* nuevo_campo_hash = malloc(nuevo_tam * sizeof(campo_hash_t));
    if (nuevo_campo_hash == NULL) {
        return false;
    }
    size_t tam_anterior = hash->tam;
    campo_hash_t* tabla_anterior = hash->tabla;
    hash->tam = nuevo_tam;
    hash->tabla = nuevo_campo_hash;

    for (size_t i = 0; i < hash->tam; i++) {
        hash->tabla[i].estado = VACIO;
        hash->tabla[i].clave = NULL;
        hash->tabla[i].dato = NULL;
    }
    hash->cant = 0;
    for (size_t i = 0; i < tam_anterior; i++) {
        if (tabla_anterior[i].estado == OCUPADO) {
            char* clave_aux = tabla_anterior[i].clave;
            hash_guardar(hash, tabla_anterior[i].clave, tabla_anterior[i].dato);
            free(clave_aux);
        }
    }
    free(tabla_anterior);
    return true;
}

hash_t *hash_crear(hash_destruir_dato_t destruir_dato) {
    hash_t* hash = malloc(sizeof(hash_t));
    if (hash == NULL) {
        return NULL;
    }
    hash->tam = TAM;
    hash->cant = 0;
    hash->destruir_dato = destruir_dato;
    campo_hash_t* campo_hash = malloc(TAM * sizeof(campo_hash_t));
    if (campo_hash == NULL) {
        free(hash);
        return NULL;
    }
    hash->tabla = campo_hash;
    for (size_t i = 0; i < hash->tam; i++) {
        hash->tabla[i].estado = VACIO;
        hash->tabla[i].clave = NULL;
        hash->tabla[i].dato = NULL;
    }
    return hash;
}

bool obtener_ocupado(const hash_t *hash, const char *clave, size_t *cont, size_t indice) {
    while (hash->tabla[*cont].estado != VACIO) {
        if (hash->tabla[*cont].estado == OCUPADO) {
            if (strcmp(hash->tabla[*cont].clave, clave) == 0) {
                return true;
            }
        }
        (*cont)++;
        if (*cont == hash->tam) {
            *cont = 0;
        }
        if (*cont == indice) {
            break;
        }
    }
    return false;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato) {
    if (((float) (hash->cant) / (float) (hash->tam)) >= FACTOR_DE_CARGA) {
        if (!hash_redimensionar(hash, false)) {
            return false;
        }
    }
    char* copia_clave = strdup(clave);
    uint32_t seed = 0;
    size_t indice = murmurhash(clave, (int) strlen(clave), seed) % hash->tam;
    size_t cont = indice;
    if (!obtener_ocupado(hash, clave, &cont, indice)) {
        hash->tabla[cont].estado = OCUPADO;
        hash->tabla[cont].clave = copia_clave;
        hash->tabla[cont].dato = dato;
        hash->cant++;
        return true;
    }
    if (hash->destruir_dato != NULL) {
        hash->destruir_dato(hash->tabla[cont].dato);
    }
    free(copia_clave);
    hash->tabla[cont].dato = dato;
    return true;
}

void *hash_borrar(hash_t *hash, const char *clave) {
    if ((hash->tam) > TAM  && ((float) (hash->cant) / (float) (hash->tam)) <= FACTOR_DE_CARGA_DE_MAS) {
        hash_redimensionar(hash, true);
    }
    uint32_t seed = 0;
    size_t indice = murmurhash(clave, (int) strlen(clave), seed) % hash->tam;
    size_t cont = indice;
    if (!obtener_ocupado(hash, clave, &cont, indice)) {
        return NULL;
    }
    void* valor = hash->tabla[cont].dato;
    hash->tabla[cont].estado = BORRADO;
    free(hash->tabla[cont].clave);
    hash->tabla[cont].clave = NULL;
    hash->tabla[cont].dato = NULL;
    hash->cant--;
    return valor;
}

void *hash_obtener(const hash_t *hash, const char *clave) {
    uint32_t seed = 0;
    size_t indice = murmurhash(clave, (int) strlen(clave), seed) % hash->tam;
    size_t cont = indice;
    if (!obtener_ocupado(hash, clave, &cont, indice)) {
        return NULL;
    }
    return hash->tabla[cont].dato;
}

bool hash_pertenece(const hash_t *hash, const char *clave) {
    uint32_t seed = 0;
    size_t indice = murmurhash(clave, (int) strlen(clave), seed) % hash->tam;
    size_t cont = indice;
    return obtener_ocupado(hash, clave, &cont, indice);
}

size_t hash_cantidad(const hash_t *hash) {
    return hash->cant;
}

void hash_destruir(hash_t *hash) {
    for (size_t i = 0; i < hash->tam; i++) {
        if (hash->tabla[i].estado == OCUPADO) {
            free(hash->tabla[i].clave);
            if (hash->destruir_dato != NULL) {
                hash->destruir_dato(hash->tabla[i].dato);
            }
        }
    }
    free(hash->tabla);
    free(hash);
}

/* *****************************************************************
 *                    PRIMITIVAS DEL ITERADOR
 * *****************************************************************/

hash_iter_t *hash_iter_crear(const hash_t *hash) {
    hash_iter_t* hash_iter = malloc(sizeof(hash_iter_t));
    if (hash_iter == NULL) {
        return NULL;
    }
    hash_iter->hash = hash;
    hash_iter->posicion_actual = 0;
    hash_iter->actual = hash->tabla[0];

    if (hash_iter->actual.estado != OCUPADO) {
        hash_iter_avanzar(hash_iter);
    }
    return hash_iter;
}

bool hash_iter_al_final(const hash_iter_t *iter) {
    return iter->posicion_actual >= iter->hash->tam;
}

bool hash_iter_avanzar(hash_iter_t *iter) {
    iter->posicion_actual++;
    if (hash_iter_al_final(iter)) {
        return false;
    }
    iter->actual = iter->hash->tabla[iter->posicion_actual];
    while (iter->actual.estado != OCUPADO) {
        iter->posicion_actual++;
        if (hash_iter_al_final(iter)) {
            return false;
        }
        iter->actual = iter->hash->tabla[iter->posicion_actual];
    }
    return true;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter) {
    if (hash_iter_al_final(iter)) {
        return NULL;
    }
    return iter->actual.clave;
}

void hash_iter_destruir(hash_iter_t* iter) {
    free(iter);
}
