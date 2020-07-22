#ifndef __ESTADO_H__
#define __ESTADO_H__

#include "set.h"
#include "tablahash.h"

/* Archivo de cabecera de la implementación del Estado del intérprete. */

#define DEFAULT_STR_SIZE 80 // Tamaño inicial de los strings.
#define STR_INPUT_GROWTH_RATE 1.5 // Tasa de crecimiento del tamaño de strings.


/* EstadoInput representa las diferentes
 acciones posibles en el intérprete. */
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

/* TipoError representa los diferentes errores 
contemplados en la validación del input. */
enum TipoError {
  ComandoNoValido = 0,
  ConjuntoInexistente,
  ElementosNoValidos,
  NoError
};

/* Estructura que representa el Estado del intérprete. */
typedef struct {
  enum EstadoInput estadoInput;
  enum TipoError tipoError;
  char* alias[3]; // Strings donde se almacenarán los conjuntos a manipular y las palabras clave.
  unsigned sizeAlias; // Tamaño actual de los strings.
  int numEspacios; // Cantidad de espacios (' ') detectados en el input.
  Set elements; // Buffer donde se almacenan los conjuntos a crear.
} Estado;


/* Crea el Estado. */
Estado* crear_estado();

/* Destruye el Estado. */
void destruir_estado(Estado* estado);

/* Prepara el Estado para recibir input nuevamente. */
void preparar_estado(Estado* estado);

/* Actualiza el Estado adecuadamente según la condición. */
void actualizar_estado(Estado* estado, enum EstadoInput estadoInput, enum TipoError error, int condicion);

/* Recibe input de la entrada estándar y lo almacena en el Estado. */
void get_input(Estado* estado);

/* Valida el input recibido. */
void validar_input(TablaHash* conjs, Estado* estado);


#endif /* __ESTADO_H__ */

