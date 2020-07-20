#include "interprete.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>

/* Archivo con las funciones que componen el intérprete. */

#define HASH_TABLE_DEF_SIZE 32


/* Genera las claves hash para la tabla con conjuntos en memoria. */
unsigned long hashStrings(void *str) {
  unsigned long hash = 5381;
  char* str2 = (char*)str;
  for (; *str2 != '\0'; str2++)
    hash = ((hash << 5) + hash) + *str2;
  return hash;
}


/* Función verificadora de igualdad de la tabla con conjuntos en memoria */
int igualesStr(void* clave1, void* clave2) {
  return strcmp((char*)clave1, (char*)clave2) == 0;
}


/* Función destructora de datos en la tabla con conjuntos en memoria (destruye sets) */
void destruir_set(void* set) {
  set_destruir((Set) set);
}


/* Función destructora de claves en la tabla con conjuntos en memoria (destruye strings) */
void destruir_alias(void* alias) {
  free(alias);
}


void insertar_conjunto(TablaHash* conjs, Estado* estado) {
  char* nombreConjunto = malloc(sizeof(char)*(strlen(estado->alias[0])+1));
  strcpy(nombreConjunto, estado->alias[0]);
  tablahash_insertar(conjs, nombreConjunto, estado->elements);
}


void calcular_operacion(TablaHash* tabla, Estado* estado, OperacionBinaria operacion) {
  estado->elements = operacion(
    tablahash_buscar(tabla, estado->alias[1], Fetch), 
    tablahash_buscar(tabla, estado->alias[2], Fetch));
}


OperacionBinaria generar_operacion_binaria(enum EstadoInput estadoInput) {
  switch (estadoInput) {
    case Unir:
      return set_union;
    case Intersecar:
      return set_interseccion;
    default:
      return set_resta;
  }
}


/* Función que encapsula el comportamiento del intérprete */
void interface() {
  TablaHash* listaConjuntos = tablahash_crear(HASH_TABLE_DEF_SIZE, hashStrings, igualesStr, destruir_alias, destruir_set);

  Estado *estado = crear_estado();
  preparar_estado(estado);
  get_input(estado);

  validar_input(listaConjuntos, estado);
  while (estado->estadoInput != Salir) {
    if (estado->tipoError == ComandoNoValido) {
      puts("Comando no válido.");
    } else if (estado->tipoError == ConjuntoInexistente) {
      puts("No se pueden realizar operaciones sobre conjuntos inexistentes.");
    } else if (estado->tipoError == ElementosNoValidos) {
      printf("Los enteros \"x\" utilizados deben verificar %d <= x <= %d.\n", INT_MIN, INT_MAX);
    } else if (estado->estadoInput == Imprimir) {
      set_imprimir(tablahash_buscar(listaConjuntos, estado->alias[0], Fetch));
      puts("");
    } else if (estado->estadoInput == CrearPorExtension || estado->estadoInput == CrearPorComprension) {
      insertar_conjunto(listaConjuntos, estado);
    } else if (estado->estadoInput == Complemento) {
      estado->elements = set_complemento(tablahash_buscar(listaConjuntos, &(estado->alias[2][1]), Fetch));
      insertar_conjunto(listaConjuntos, estado);
    } else {
      calcular_operacion(listaConjuntos, estado, generar_operacion_binaria(estado->estadoInput));
      insertar_conjunto(listaConjuntos, estado);
    }
    preparar_estado(estado);
    get_input(estado);
    validar_input(listaConjuntos, estado);
  }
  destruir_estado(estado);
  tablahash_destruir(listaConjuntos);
}
