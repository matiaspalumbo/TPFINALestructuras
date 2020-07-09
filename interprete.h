#ifndef __INTERPRETE_H__
#define __INTERPRETE_H__
#include "itree.h"



enum EstadoInput {ComandoNoValido, IntervaloNoValido, Salir, RecorridoBFS, RecorridoDFS, Insertar, Eliminar, Intersecar};

/* En los casos de Insertar, Eliminar e Intersecar, determina si los caracteres no numéricos
son los esperados. */
enum EstadoInput validar_char(enum EstadoInput estado, char input, int c);

/* Dado el input del usuario y un puntero a un intervalo, determina si el input es válido
y devuelve el aviso e información correspondiente. */
enum EstadoInput validar_input(char *input);

/* Función que encapsula el comportamiento del intérprete. */
void interface();


#endif /* __INTERPRETE_H__ */
