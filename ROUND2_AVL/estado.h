#ifndef __ESTADO_H__
#define __ESTADO_H__

#include "set.h"
#include "tablahash.h"

#define DEFAULT_STR_SIZE 80
#define STR_INPUT_GROWTH_RATE 1.5

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
  ComandoNoValido = 0,
  ConjuntoInexistente = 1,
  NoError = 2
};

typedef struct {
  enum EstadoInput estadoInput;
  enum TipoError tipoError;
  char* alias[3];
  unsigned sizeAlias;
  int numEspacios;
  Set elements;
} Estado;


Estado* crear_estado();

void destruir_estado(Estado* estado);

void preparar_estado(Estado* estado);

void actualizar_estado(Estado* estado, enum EstadoInput estadoInput, enum TipoError error, int condicion);

void get_input(Estado* estado);

void validar_input(TablaHash* conjs, Estado* estado);



#endif /* __ESTADO_H__ */

