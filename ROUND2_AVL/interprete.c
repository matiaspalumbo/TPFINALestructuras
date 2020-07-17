#include "interprete.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define HASH_TABLE_DEF_SIZE 32

/* Archivo con las funciones que componen el intérprete. */



unsigned long hashStrings(void* str) {
  char* toHash = (char*)str;
  unsigned long hash = 1297; // Primo arbitrario
  int c = toHash[0];
  int j=0;
  while (c != '\0') {
    hash = ((hash << 5) + hash) + c;
    j++;
    c = toHash[j];
  }
  return hash;
}


int igualesStr(void* clave1, void* clave2) {
  return (strcmp((char*)clave1, (char*)clave2) == 0) ? 1 : 0;
}




void insertar_conjunto(TablaHash* conjs, Estado* estado) {
  char* nombreConjunto = malloc(sizeof(char)*(strlen(estado->alias[0])+1));
  strcpy(nombreConjunto, estado->alias[0]);
  tablahash_insertar(conjs, nombreConjunto, estado->elements);
}

// CREAR CABECERA APARTE PARA ESTADO


//   MEJORAR MANEJO DE LA MEMORIA


void destruir_set(void* set) {
  set_destruir((Set) set);
}

void destruir_alias(void* alias) {
  free(alias);
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



void interface() {
  TablaHash* listaConjuntos = tablahash_crear(HASH_TABLE_DEF_SIZE, hashStrings, igualesStr, destruir_alias, destruir_set);

  Estado *estado = crear_estado();
  get_input(estado);
  preparar_estado(estado);

  validar_input(listaConjuntos, estado);
  while (estado->estadoInput != Salir) {
    if (estado->tipoError == ComandoNoValido) {
      puts("Comando no válido.");
    } else if (estado->tipoError == ConjuntoInexistente) {
      puts("No se pueden realizar operaciones sobre conjuntos inexistentes.");
    } else if (estado->estadoInput == Imprimir) {
      set_imprimir(tablahash_buscar(listaConjuntos, estado->alias[1], Fetch));
      // printf("max: %d - min: %d\n", ((Set)(tablahash_busscar(listaConjuntos, estado->alias[1], Fetch)))->max, ((Set)(tablahash_buscar(listaConjuntos, estado->alias[1], Fetch)))->min);
    } else if (estado->estadoInput == CrearPorExtension || estado->estadoInput == CrearPorComprension) {
      insertar_conjunto(listaConjuntos, estado);
      puts("Conjunto creado.");
    } else if (estado->estadoInput == Complemento) {
      estado->elements = set_complemento(tablahash_buscar(listaConjuntos, &(estado->alias[2][1]), Fetch));
      insertar_conjunto(listaConjuntos, estado);
    } else {
      calcular_operacion(listaConjuntos, estado, generar_operacion_binaria(estado->estadoInput));
      insertar_conjunto(listaConjuntos, estado);
    }
    get_input(estado);
    preparar_estado(estado);
    validar_input(listaConjuntos, estado);
  }
  puts("1");
  imprimir_th(listaConjuntos); // TO DELETE
  puts("2");
  destruir_estado(estado);
  puts("3");
  tablahash_destruir(listaConjuntos);
}
