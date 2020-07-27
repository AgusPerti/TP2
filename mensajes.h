#ifndef __MENSAJES__H_
#define __MENSAJES__H_

#define PACIENTE_ENCOLADO "Paciente %s encolado\n"
#define CANT_PACIENTES_ENCOLADOS "%zu paciente(s) en espera para %s\n"
#define ENOENT_PACIENTE "ERROR: no existe el paciente '%s'\n"
#define ENOENT_ESPECIALIDAD "ERROR: no existe la especialidad '%s'\n"
#define ENOENT_URGENCIA "Error: grado de urgencia no identificado ('%s')\n"

#define PACIENTE_ATENDIDO "Se atiende a %s\n"
#define SIN_PACIENTES "No hay pacientes en espera\n"
#define ENOENT_DOCTOR "ERROR: no existe el doctor '%s'\n"

#define DOCTORES_SISTEMA "%zu doctor(es) en el sistema\n"
#define INFORME_DOCTOR "%zu: %s, especialidad %s, %zu paciente(s) atendido(s)\n"

#define ENOENT_CANT_PARAMS "No se recibieron los 2 (dos) parametros: <archivo doctores> y <archivo pacientes>\n"
#define ENOENT_ARCHIVO "No se pudo leer archivo %s\n"
#define ENOENT_ANIO "Valor no numerico en campo de anio: %s\n"
#define ENOENT_FORMATO "ERROR: formato de comando incorrecto ('%s')\n"
#define ENOENT_CMD "ERROR: no existe el comando '%s'\n"

#define ENOENT_PARAMS "ERROR: cantidad de parametros invalidos para comando '%s'\n"

#define COMANDO_PEDIR_TURNO "PEDIR_TURNO"
#define COMANDO_ATENDER "ATENDER_SIGUIENTE"
#define COMANDO_INFORME "INFORME"
#define PRIORIDAD1 "URGENTE"
#define PRIORIDAD2 "REGULAR"

#define POS_CAMPO_NOMBRE_PAC 0 // posicion del nombre del paciente
#define POS_CAMPO_ESPECIALIDAD 1 // posicion del nombre de la especialidad
#define POS_CAMPO_URGENCIA 2 // posicion de la urgencia
#define POS_CAMPO_NOMBRE_DOC 0 // posicion del nombre del doctor
#define POS_CAMPO_INICIO 0 // posicion del nombre del doctor inicial
#define POS_CAMPO_FIN 1 // posicion del nombre del doctor final

#endif
