#ifndef __TABLAHASH_H__
#define __TABLAHASH_H__

/* Archivo de cabecera de la implementación de Tablas Hash */


#define LIM_FACTOR_CARGA 0.75
#define INTV_HASH_DOBLE 7


typedef unsigned long (*FuncionHash)(void* clave);

typedef int (*FuncionIgualdad)(void *, void *);

typedef void (*FuncionDestructora)(void *);

typedef enum _TipoBusqueda {Check, Fetch} TipoBusqueda;

typedef struct {
  void* clave;
  void* dato;
} CasillaHash;

typedef struct _TablaHash {
  CasillaHash* tabla;
  unsigned numElems;
  unsigned capacidad;
  FuncionHash hash;
  FuncionIgualdad iguales;
  FuncionDestructora destClaves;
  FuncionDestructora destDatos;
} TablaHash;


TablaHash* tablahash_crear(unsigned capacidad, FuncionHash hash, FuncionIgualdad iguales, 
  FuncionDestructora destClaves, FuncionDestructora destDatos);

void tablahash_insertar(TablaHash* tabla, void* clave, void* dato);

void* tablahash_buscar(TablaHash* tabla, void* clave, TipoBusqueda tipoBusqueda);

void tablahash_eliminar(TablaHash* tabla, void* clave);

void tablahash_redimensionar(TablaHash* tabla);

void tablahash_destruir(TablaHash* tabla);


#endif /* __TABLAHASH_H__ */
