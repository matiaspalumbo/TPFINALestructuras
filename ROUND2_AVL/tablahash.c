#include "tablahash.h"
#include "pila.h"
#include <stdio.h>
#include <stdlib.h>


/* Crea una nueva tabla Hash vacía, con la capacidad dada. */
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
  for (unsigned idx = 0; idx < capacidad; ++idx) {
    tabla->tabla[idx].clave = NULL;
    tabla->tabla[idx].dato = NULL;
  }
  return tabla;
}


/* Inserta el dato en la tabla, asociado a la clave dada. */
void tablahash_insertar(TablaHash* tabla, void* clave, void* dato) {
  if ((double)tabla->numElems/tabla->capacidad > LIM_FACTOR_CARGA)
    tablahash_redimensionar(tabla);
  unsigned idx = tabla->hash(clave) % tabla->capacidad;
  while (tabla->tabla[idx].clave && !tabla->iguales(tabla->tabla[idx].clave, clave)) {
    idx = (idx+INTV_HASH_DOBLE) % tabla->capacidad;
  }
  if (tabla->tabla[idx].clave && tabla->destDatos) {
    tabla->destDatos(tabla->tabla[idx].dato);
  } else
    tabla->numElems++;
  tabla->tabla[idx].clave = clave;
  tabla->tabla[idx].dato = dato;
}


void* tablahash_buscar(TablaHash* tabla, void* clave, TipoBusqueda tipoBusqueda) {
  unsigned idx = tabla->hash(clave) % tabla->capacidad;
  CasillaHash* spot = &(tabla->tabla[idx]);
  if (spot->clave && !tabla->iguales(clave, spot->clave)) {
    unsigned newIdx = (idx+INTV_HASH_DOBLE) % tabla->capacidad;
    while (newIdx != idx) {
      spot = &(tabla->tabla[newIdx]);
      if (spot->clave && !tabla->iguales(spot->clave, clave))
        newIdx = (idx+INTV_HASH_DOBLE) % tabla->capacidad;
      else 
        idx = newIdx;
    }
  }
  if (spot->clave && tabla->iguales(clave, spot->clave))
    return (tipoBusqueda == Fetch) ? spot->dato : spot->clave;
  else
    return NULL;
}


void eliminar_casilla(TablaHash* tabla, unsigned idx) {
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
    while (newIdx != idx) {
      spot = &(tabla->tabla[newIdx]);
      if (spot->clave && !tabla->iguales(spot->clave, clave))
        newIdx = (idx+INTV_HASH_DOBLE) % tabla->capacidad;
      else
        idx = newIdx;
    }
  }
  if (spot->clave && tabla->iguales(clave, spot->clave))
    eliminar_casilla(tabla, idx);
}


void tablahash_redimensionar(TablaHash* tabla) {
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
  tabla->capacidad *= 2;
  tabla->tabla = realloc(tabla->tabla, sizeof(CasillaHash)*tabla->capacidad);
  tabla->numElems = 0;
  while (!pila_es_vacia(pila_claves)) {
    tablahash_insertar(tabla, pila_ultimo(pila_claves), pila_ultimo(pila_datos));
    pila_desapilar(&pila_claves);
    pila_desapilar(&pila_datos);
  }
  pila_destruir(pila_claves);
  pila_destruir(pila_datos);
}


/* Destruye la tabla */
void tablahash_destruir(TablaHash* tabla) {
  int hayDestClaves = tabla->destClaves != NULL;
  int hayDestDatos = tabla->destDatos != NULL;
  for (unsigned i=0; i<tabla->capacidad; i++) {
    if (tabla->tabla[i].clave != NULL && hayDestClaves) {
      tabla->destClaves(tabla->tabla[i].clave);
      if (hayDestDatos)
        tabla->destDatos(tabla->tabla[i].dato);
    }
  }
  free(tabla->tabla);
  free(tabla);
}
