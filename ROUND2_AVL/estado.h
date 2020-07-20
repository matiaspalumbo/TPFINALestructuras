#ifndef __ESTADO_H__
#define __ESTADO_H__

#include "set.h"
#include "tablahash.h"

#define DEFAULT_STR_SIZE 80
#define STR_INPUT_GROWTH_RATE 1.5


enum EstadoInput {
  Salir = 0,
  Imprimir,
  CrearPorExtension,
  CrearPorComprension,
  Unir,
  Intersecar,
  Resta,
  Complemento,
};

enum TipoError {
  ComandoNoValido = 0,
  ConjuntoInexistente,
  ElementosNoValidos,
  NoError
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

