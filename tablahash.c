#include "tablahash.h"
#include "pila.h"
#include <stdio.h>
#include <stdlib.h>

 
TablaHash* tablahash_crear(unsigned capacidad, FuncionHash hash, FuncionIgualdad iguales,
  FuncionDestructora destClaves, FuncionDestructora destDatos) {
  TablaHash* tabla = malloc(sizeof(TablaHash));
  tabla->hash = hash;
  tabla->iguales = iguales;
  tabla->capacidad = capacidad;
  tabla->destClaves = destClaves;
  tabla->destDatos = destDatos;
  tabla->tabla = malloc(sizeof(CasillaHash) * capacidad);
  tabla->numElems = 0;
  for (unsigned idx = 0; idx < capacidad; ++idx) { // Inicializa las casillas.
    tabla->tabla[idx].clave = NULL;
    tabla->tabla[idx].dato = NULL;
  }
  return tabla;
}


/* Inserta el dato en la tabla, asociado a la clave dada. */
void tablahash_insertar(TablaHash* tabla, void* clave, void* dato) {
  // Si el factor de carga es mayor a un cierto valor, redimensiona la tabla.
  if ((double)tabla->numElems/tabla->capacidad > LIM_FACTOR_CARGA)
    tablahash_redimensionar(tabla);
  unsigned idx = tabla->hash(clave) % tabla->capacidad;
  // Recorre casillas hasta encontrar una disponible.
  while (tabla->tabla[idx].clave && !tabla->iguales(tabla->tabla[idx].clave, clave))
    idx = (idx+INTV_HASH_DOBLE) % tabla->capacidad; 
  if (tabla->tabla[idx].clave && tabla->destDatos)
    // Si la casilla a utilizar tiene un dato, lo destruye.
    tabla->destDatos(tabla->tabla[idx].dato);
  else
    tabla->numElems++;
  tabla->tabla[idx].clave = clave;
  tabla->tabla[idx].dato = dato;
}


void* tablahash_buscar(TablaHash* tabla, void* clave, TipoBusqueda tipoBusqueda) {
  unsigned idx = tabla->hash(clave) % tabla->capacidad;
  CasillaHash* spot = &(tabla->tabla[idx]);
  if (spot->clave && !tabla->iguales(clave, spot->clave)) {
    unsigned newIdx = (idx+INTV_HASH_DOBLE) % tabla->capacidad;
    /* Recorre casillas hasta encontrar una coincidencia, haber recorrido
    toda la tabla o haber llegado a casillas vacías. */
    while (newIdx != idx) {
      spot = &(tabla->tabla[newIdx]);
      if (spot->clave && !tabla->iguales(spot->clave, clave))
        newIdx = (idx+INTV_HASH_DOBLE) % tabla->capacidad;
      else 
        idx = newIdx;
    }
  }
  /* Si el tipo de búsqueda es Fetch devuelve la clave. Caso contrario, devuelve el dato. */
  if (spot->clave && tabla->iguales(clave, spot->clave))
    return (tipoBusqueda == Fetch) ? spot->dato : spot->clave;
  else
    return NULL;
}


/* Elimina la casilla correspondiente al índice dado. */
void eliminar_casilla(TablaHash* tabla, unsigned idx) {
  /* Si se especificaron funciones destructoras, se destruye la clave y/o el dato. */
  if (tabla->destClaves)
    tabla->destClaves(tabla->tabla[idx].clave);
  else
    tabla->tabla[idx].clave = NULL;
  if (tabla->destDatos)
    tabla->destDatos(tabla->tabla[idx].dato);
  else
    tabla->tabla[idx].dato = NULL;
  tabla->numElems--;
}


void tablahash_eliminar(TablaHash* tabla, void* clave) {
  unsigned idx = tabla->hash(clave) % tabla->capacidad;
  CasillaHash* spot = &(tabla->tabla[idx]);
  if (spot->clave && !tabla->iguales(clave, spot->clave)) {
    unsigned newIdx = (idx+INTV_HASH_DOBLE) % tabla->capacidad;
    /* Recorre casillas hasta encontrar una coincidencia, haber recorrido
    toda la tabla o haber llegado a casillas vacías. */
    while (newIdx != idx) {
      spot = &(tabla->tabla[newIdx]);
      if (spot->clave && !tabla->iguales(spot->clave, clave))
        newIdx = (idx+INTV_HASH_DOBLE) % tabla->capacidad;
      else
        idx = newIdx;
    }
  }
  /* Si se encontró una coincidencia, elimina la casilla. */
  if (spot->clave && tabla->iguales(clave, spot->clave))
    eliminar_casilla(tabla, idx);
}


void tablahash_redimensionar(TablaHash* tabla) {
  /* Almacena las claves y datos en pilas, y vacía la tabla temporalmente. */
  Pila pila_claves = pila_crear();
  Pila pila_datos = pila_crear();
  for (unsigned i=0; i < tabla->capacidad; i++) {
    if (tabla->tabla[i].clave) {
      pila_claves = pila_apilar(pila_claves, tabla->tabla[i].clave);
      pila_datos = pila_apilar(pila_datos, tabla->tabla[i].dato);
      tabla->tabla[i].clave = NULL;
      tabla->tabla[i].dato = NULL;
    }
  }
  /* Duplica la capacidad de la tabla y solicita la memoria correspondiente. */
  tabla->capacidad *= 2;
  tabla->tabla = realloc(tabla->tabla, sizeof(CasillaHash)*tabla->capacidad);
  tabla->numElems = 0;
  while (!pila_es_vacia(pila_claves)) { // Vuelve a insertar los elementos.
    tablahash_insertar(tabla, pila_ultimo(pila_claves), pila_ultimo(pila_datos));
    pila_desapilar(&pila_claves);
    pila_desapilar(&pila_datos);
  }
  pila_destruir(pila_claves);
  pila_destruir(pila_datos);
}


void tablahash_destruir(TablaHash* tabla) {
  int hayDestClaves = tabla->destClaves != NULL;
  int hayDestDatos = tabla->destDatos != NULL;
  int j=0;
  /* Si se especificaron funciones destructoras, se 
  destruyen las claves y/o datos correspondientes. */
  for (unsigned i=0; i<tabla->capacidad && j<tabla->numElems; i++) {
    if (tabla->tabla[i].clave) {
      if (hayDestClaves)
        tabla->destClaves(tabla->tabla[i].clave);
      if (hayDestDatos)
        tabla->destDatos(tabla->tabla[i].dato);
      j++;
    }
  }
  /* Se libera la memoria requerida por la tabla. */
  free(tabla->tabla);
  free(tabla);
}
