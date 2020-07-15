#ifndef __SET_H__
#define __SET_H__

/* Archivo de cabecera de la implementación de Árboles de Intervalos utilizando 
Arboles AVL ordenados lexicográficamente. */

typedef struct {
  int izq; // Extremo izquierdo.
  int der; // Extremo derecho.
} Intervalo;

typedef struct _SetNodo {
  Intervalo* intv;
  int max, min, altura;
  struct _SetNodo *right;
  struct _SetNodo *left;
} SetNodo;

typedef SetNodo* Set;

typedef void (*FuncionVisitante) (Set arbol);


/* Crea un árbol de intervalos vacío. */
Set set_crear();

/* Destruye un árbol de intervalos. */
void set_destruir(Set nodo);

/* Indica si el árbol es vacío.*/
int set_empty(Set nodo);

/* Inserta un elemento en un árbol de intervalos. */
Set set_insertar(Set arbol, Intervalo *intv);

/* Elimina un elemento de un árbol de intervalos. */
Set set_eliminar(Set nodo, Intervalo *intv);

/* Determina si un intervalo se interseca con alguno de los elementos del 
árbol y, en caso afirmativo, retorna un puntero al nodo correspondiente. */
int set_intersecar(Set arbol, Intervalo *intv, Intervalo* buffer);

/* Realiza un recorrido primero en profundidad (DFS) in-order del árbol de intervalos, 
aplicando a cada nodo la funcion visitante dada como parámetro. */
void set_recorrer_dfs(Set arbol, FuncionVisitante visit);

/* Realiza un recorrido primero a lo ancho (BFS) del árbol de intervalos, 
aplicando a cada nodo la funcion visitante dada como parámetro. */
void set_recorrer_bfs(Set arbol, FuncionVisitante visit);



// /* Devuelve la altura del Set dado como parámetro.*/
// int itree_altura(Set nodo) ;

// /* Devuelve factor de balance del Set dado como parámetro,
// restando las alturas de sus subárboles izquierdo y derecho. */
// int set_balance_factor(Set nodo);



/* Función de tipo FuncionVisitante que dado un elemento de un arbol de 
intervalos, lo imprime. Formato: el intervalo con extremo izquierdo a 
y extremo derecho b se imprime como "[a, b] ". */
void imprimir_intervalo(Set nodo);


void set_imprimir(Set tree);

Set set_union(Set arbol1, Set arbol2);

Set set_complemento(Set arbol);

Set set_interseccion(Set arbol1, Set arbol2);

Set set_clonar(Set arbol);

Set set_resta(Set arbol1, Set arbol2);


typedef long (*FunGeneradora)(int); // TO DELETE
typedef enum Numeros {Positivos, Negativos, Ambos} Numeros; // TO DELETE
Set set_crear_sucesion(FunGeneradora generar, Numeros direccion, int posInicial); // TO DELETE


#endif /* __SET_H__ */
