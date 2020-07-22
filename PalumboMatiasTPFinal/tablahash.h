#ifndef __TABLAHASH_H__
#define __TABLAHASH_H__

/* Archivo de cabecera de la implementación de Tablas Hash */

/* Límite del factor de carga luego del cual se redimensiona la tabla. */
#define LIM_FACTOR_CARGA 0.75
/* Intervalo de sondeo. */
#define INTV_HASH_DOBLE 7


/* Puntero a funciones que devuelven claves hash. */
typedef unsigned long (*FuncionHash)(void* clave);

/* Puntero a funciones que verifican la igualdad de claves. */
typedef int (*FuncionIgualdad)(void *, void *);

/* Puntero a funciones que destruyen claves y datos. */
typedef void (*FuncionDestructora)(void *);

/* enum utilizado en tablahash_buscar. */
typedef enum _TipoBusqueda {Check, Fetch} TipoBusqueda;

/* Estructura que representa las casillas de la Tabla Hash. */
typedef struct {
  void* clave;
  void* dato;
} CasillaHash;

/* Estructura que representa la Tabla Hash. */
typedef struct _TablaHash {
  CasillaHash* tabla;
  unsigned numElems; // Cantidad de elementos de la tabla.
  unsigned capacidad; // Número de casillas de la tabla.
  FuncionHash hash;
  FuncionIgualdad iguales;
  FuncionDestructora destClaves;
  FuncionDestructora destDatos;
} TablaHash;


/* Crea una Tabla Hash con los parámetros dados. */
TablaHash* tablahash_crear(unsigned capacidad, FuncionHash hash, FuncionIgualdad iguales, 
  FuncionDestructora destClaves, FuncionDestructora destDatos);

/* Inserta un elemento en una Tabla Hash. */
void tablahash_insertar(TablaHash* tabla, void* clave, void* dato);

/* Busca un elemento en una Tabla Hash. */
void* tablahash_buscar(TablaHash* tabla, void* clave, TipoBusqueda tipoBusqueda);

/* Elimina un elemento de una Tabla Hash. */
void tablahash_eliminar(TablaHash* tabla, void* clave);

/* Duplica la capacidad de una Tabla Hash. */
void tablahash_redimensionar(TablaHash* tabla);

/* Destruye una Tabla Hash. */
void tablahash_destruir(TablaHash* tabla);


#endif /* __TABLAHASH_H__ */
