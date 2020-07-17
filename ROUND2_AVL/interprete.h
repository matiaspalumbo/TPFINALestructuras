#ifndef __INTERPRETE_H__
#define __INTERPRETE_H__
#include "tablahash.h"
#include "set.h"
#include "estado.h"


// enum EstadoInput {
//   Salir = 0,
//   Imprimir = 1,
//   CrearPorExtension = 2,
//   CrearPorComprension = 3,
//   Unir = 4,
//   Intersecar = 5,
//   Resta = 6,
//   Complemento = 7,
// };

// enum TipoError {
//   ComandoNoValido = 0,
//   ConjuntoInexistente = 1,
//   NoError = 2
// };

// typedef struct {
//   enum EstadoInput estadoInput;
//   enum TipoError tipoError;
//   char* alias[3];
//   // char* conjInexistente;
//   Set elements;
// } Estado;



/* Función que encapsula el comportamiento del intérprete. */
void interface();


#endif /* __INTERPRETE_H__ */
