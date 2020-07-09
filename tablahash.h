#ifndef __TABLAHASH_H__
#define __TABLAHASH_H__


#define LIM_FACTOR_CARGA 0.7

typedef unsigned long (*FuncionHash)(void* clave);
typedef int (*FuncionIgualdad)(void *, void *);

typedef struct _TablaHash TablaHash;


TablaHash* tablahash_crear(unsigned capacidad, FuncionHash fun, FuncionIgualdad iguales);

void tablahash_insertar(TablaHash* tabla, void* clave, void* dato);

void* tablahash_buscar(TablaHash* tabla, void* clave);

void tablahash_eliminar(TablaHash* tabla, void* clave);

void tablahash_redimensionar(TablaHash* tabla);

void tablahash_destruir(TablaHash* tabla);

void imprimir_th(TablaHash* th);


#endif /* __TABLAHASH_H__ */
