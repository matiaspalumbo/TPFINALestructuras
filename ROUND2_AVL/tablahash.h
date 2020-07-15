#ifndef __TABLAHASH_H__
#define __TABLAHASH_H__


#define LIM_FACTOR_CARGA 0.75
#define INTV_HASH_DOBLE 7

typedef unsigned long (*FuncionHash)(void* clave);
typedef int (*FuncionIgualdad)(void *, void *);
typedef void (*FuncionDestructora)(void *);


typedef struct {
  void* clave;
  void* dato;
  int eliminada; // 0 si es vacía, 1 si fue eliminada.
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

typedef enum _TipoBusqueda {Check, Fetch} TipoBusqueda;


TablaHash* tablahash_crear(
  unsigned capacidad,
  FuncionHash hash, 
  FuncionIgualdad iguales, 
  FuncionDestructora destClaves, 
  FuncionDestructora destDatos);

void tablahash_insertar(TablaHash* tabla, void* clave, void* dato);

void* tablahash_buscar(TablaHash* tabla, void* clave, TipoBusqueda tipoBusqueda);

void tablahash_eliminar(TablaHash* tabla, void* clave);

void tablahash_redimensionar(TablaHash* tabla);

void tablahash_destruir(TablaHash* tabla);

void imprimir_th(TablaHash* th);


#endif /* __TABLAHASH_H__ */
