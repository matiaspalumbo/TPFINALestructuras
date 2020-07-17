#include "tablahash.h"
#include "pila.h"
#include <stdio.h>
#include <stdlib.h>



#include "set.h" // TO DELETE

/*
La Tabla Hash posee las siguientes propiedades:
- double hashing
- redimensión ocasional
- límite del factor de carga
*/


/**
 * Crea una nueva tabla Hash vacía, con la capacidad dada.
 */
TablaHash* tablahash_crear(unsigned capacidad, FuncionHash hash, FuncionIgualdad iguales, FuncionDestructora destClaves, FuncionDestructora destDatos) {
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
    tabla->tabla[idx].eliminada = 0;
  }
  return tabla;
}

/**
 * Inserta el dato en la tabla, asociado a la clave dada.
 */
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
  tabla->tabla[idx].eliminada = 0;
}


/**
 * Busca un elemento dado en la tabla, y retorna un puntero al mismo.
 * En caso de no existir, se retorna un puntero nulo.
 */
void* tablahash_buscar(TablaHash* tabla, void* clave, TipoBusqueda tipoBusqueda) {
  unsigned idx = tabla->hash(clave) % tabla->capacidad, newIdx;
  CasillaHash* spot = &(tabla->tabla[idx]);
  CasillaHash* casillaEliminada = (spot->eliminada == 1) ? spot : NULL;
  if (spot->clave && tabla->iguales(clave, spot->clave))
    newIdx = idx;
  else if (spot->eliminada == 1 || spot->clave) {
    unsigned intervalo = INTV_HASH_DOBLE;
    newIdx = idx+intervalo % tabla->capacidad;
    int terminar = 1;
    while (terminar && newIdx != idx) {
      spot = &(tabla->tabla[newIdx]);
      if (spot->eliminada == 1 || (spot->clave && !tabla->iguales(spot->clave, clave))) {
        if (!casillaEliminada && spot->eliminada == 1)
          casillaEliminada = spot;
        newIdx = idx+intervalo % tabla->capacidad;
      }
      else 
        terminar = 0;
    }
  }
  if (spot->clave && tabla->iguales(clave, spot->clave)) {
    if (casillaEliminada) {
      casillaEliminada->clave = spot->clave;
      casillaEliminada->dato = spot->dato;
      casillaEliminada->eliminada = 0;
      spot->clave = NULL;
      spot->dato = NULL;
      return (tipoBusqueda == Fetch) ? casillaEliminada->dato : casillaEliminada->clave;
    } else 
      return (tipoBusqueda == Fetch) ? spot->dato : spot->clave;
  } else
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
  tabla->tabla[idx].eliminada = 1;
  tabla->numElems--;
}

/**
 * Elimina un elemento de la tabla.
 */
void tablahash_eliminar(TablaHash* tabla, void* clave) {
  unsigned idx = tabla->hash(clave) % tabla->capacidad;
  CasillaHash* spot = &(tabla->tabla[idx]);
  if (spot->clave && tabla->iguales(clave, spot->clave))
    eliminar_casilla(tabla, idx);
  else if (spot->eliminada == 1 || spot->clave) {
    unsigned newIdx = (idx+INTV_HASH_DOBLE) % tabla->capacidad;
    while (newIdx != idx) {
      spot = &(tabla->tabla[newIdx]);
      if (spot->eliminada == 1 || (spot->clave && !tabla->iguales(spot->clave, clave)))
        newIdx = (idx+INTV_HASH_DOBLE) % tabla->capacidad;
      else {
        idx = newIdx;
      }
    }
    if (spot->clave && tabla->iguales(clave, spot->clave))
      eliminar_casilla(tabla, idx);
  }
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
      tabla->tabla[i].eliminada = 0;
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


/**
 * Destruye la tabla.
 */
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


void imprimir_th(TablaHash* th) {
  printf("--- TABLA HASH (%u:%u) ---\n", th->numElems, th->capacidad);
  for (unsigned i = 0; i < th->capacidad; i++) {
    if (th->tabla[i].clave == NULL) puts("NULL");
    else {
      printf("%s : ", ((char*)th->tabla[i].clave));
      set_imprimir((Set) (th->tabla[i].dato));
      puts("");
      // GNodo* temp1 = ((GList)(((Set*)(th->tabla[i].dato))->set));
      // for (int it = 0; it<gdclist_longitud(((GList)(((Set*)(th->tabla[i].dato))->set))); it++) {
      //   printf("%d:%d    ", ((Intervalo*)temp1->dato)->left, ((Intervalo*)temp1->dato)->right);
      // temp1 = temp1->sig;
      // }
      // puts("");
    }
  }
  printf("-------------------------------%s%s\n", (th->numElems == 1) ? "" : "-", (th->numElems > 9) ? "-" : "");
}


