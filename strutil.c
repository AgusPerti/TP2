#define _POSIX_C_SOURCE 200809L // strdup(), stpcpy()
#include "strutil.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/* solo puedo usar strlen(), strcpy()/strncpy(), strdup()/strndup(), snprintf()*/

char *substr(const char *str, size_t n) {
    char* nueva = strndup(str, n);
    if (nueva == NULL) {
        return NULL;
    }
    return nueva;
}

char **split(const char *str, char sep) {
    int cant_sep = 1;
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (str[i] == sep) {
            cant_sep++;
        }
    }
    char** strv = malloc((cant_sep + 1) * sizeof(char*) );
    const char* aux = str;
    int cant_car = 0;
    size_t campo_actual = 0;

    for (size_t i = 0; str[i] != '\0'; i++) {
        if (str[i] != sep) {
            cant_car++;
        }
        else {
            strv[campo_actual] = substr (aux, cant_car);
            aux = aux + cant_car + 1;
            campo_actual++;
            cant_car = 0;
        }
    }
    strv[campo_actual] = substr (aux, cant_car);
    strv[campo_actual + 1] = NULL;
    return strv;
}

char *join(char **strv, char sep) {
    if (strv[0] == NULL) {
        char* str = malloc(sizeof(char));
        str[0] = '\0';
        return str;
    }
    size_t cant_car = 0;
    for (size_t i = 0; strv[i] != NULL; i++) {
        cant_car += strlen(strv[i]) + 1;
    }
    char* str = malloc((cant_car + 1) * sizeof(char));
    int cont = 0;
    for (size_t i = 0; strv[i] != NULL; i++) {
        if (strlen(strv[i]) == 0 && strv[i + 1] != NULL) {
            str[cont] = sep;
            cont++;
            continue;
        }
        for (size_t j = 0; strv[i][j] != '\0'; j++) {
            str[cont] = strv[i][j];
            cont++;
        }
        if (strv[i + 1] != NULL && sep != '\0') {
            str[cont] = sep;
            cont++;
        }
    }
    str[cont] = '\0';
    return str;
}

void free_strv(char *strv[]) {
    for (size_t i = 0; strv[i] != NULL; i++) {
        free(strv[i]);
    }
    free(strv);
}
