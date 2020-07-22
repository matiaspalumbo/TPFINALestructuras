#ifndef __INTERPRETE_H__
#define __INTERPRETE_H__

#include "tablahash.h"
#include "set.h"
#include "estado.h"

/* Archivo de cabecera del Intérprete. */

#define HASH_TABLE_DEF_SIZE 32 // Tamaño inicial de la Tabla Hash..


/* Función hash para strings. */
unsigned long hashStrings(void *str);

/* Determina si dos strings son iguales. */
int igualesStr(void* clave1, void* clave2);

/* Encapsula el comportamiento del intérprete. */
void interface();


#endif /* __INTERPRETE_H__ */
