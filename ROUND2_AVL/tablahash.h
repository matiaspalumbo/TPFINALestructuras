#ifndef __TABLAHASH_H__
#define __TABLAHASH_H__


#define LIM_FACTOR_CARGA 0.7
#define INTV_HASH_DOBLE 7

typedef unsigned long (*FuncionHash)(void* clave);
typedef int (*FuncionIgualdad)(void *, void *);
typedef void (*FuncionDestructora)(void *);

/**
 * Casillas en la que almacenaremos los datos de la tabla hash.
 */
typedef struct {
  void* clave;
  void* dato;
  int eliminada; // 0 si es vacía, 1 si fue eliminada.
} CasillaHash;


/**
 * Estructura principal que representa la tabla hash.
 */
typedef struct _TablaHash {
  CasillaHash* tabla;
  unsigned numElems;
  unsigned capacidad;
  FuncionHash hash;
  FuncionIgualdad iguales;
  FuncionDestructora destruir;

} TablaHash;

typedef enum _TipoBusqueda {Check, Fetch} TipoBusqueda;


TablaHash* tablahash_crear(unsigned capacidad, FuncionHash fun, FuncionIgualdad iguales, FuncionDestructora destruir);

void tablahash_insertar(TablaHash* tabla, void* clave, void* dato);

// void* tablahash_buscar(TablaHash* tabla, void* clave);
void* tablahash_buscar(TablaHash* tabla, void* clave, TipoBusqueda tipoBusqueda);

void tablahash_eliminar(TablaHash* tabla, void* clave);

void tablahash_redimensionar(TablaHash* tabla);

void tablahash_destruir(TablaHash* tabla);

void imprimir_th(TablaHash* th);


#endif /* __TABLAHASH_H__ */
