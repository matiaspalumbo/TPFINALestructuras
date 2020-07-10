#ifndef __INTERPRETE_H__
#define __INTERPRETE_H__
// #include "itree.h"
#include "tablahash.h"


enum EstadoInput {
  Salir = 0,
  Imprimir = 1,
  CrearPorExtension = 2,
  CrearPorComprension = 3,
  Unir = 4,
  Intersecar = 5,
  Resta = 6,
  Complemento = 7,
};

enum TipoError {
  ComandoNoValido,
  ConjuntoInexistente
};

typedef struct {
  enum EstadoInput estadoInput;
  enum TipoError tipoError;
  char* alias1;
  char* alias2;
  char* alias3;
} Estado;

/* En los casos de Insertar, Eliminar e Intersecar, determina si los caracteres no numéricos
son los esperados. */
void validar_char(char c, Estado* estado);

/* Dado el input del usuario y un puntero a un intervalo, determina si el input es válido
y devuelve el aviso e información correspondiente. */
void validar_input(char *input, TablaHash* listaConjuntos, Estado* estado);

/* Función que encapsula el comportamiento del intérprete. */
void interface();


#endif /* __INTERPRETE_H__ */
