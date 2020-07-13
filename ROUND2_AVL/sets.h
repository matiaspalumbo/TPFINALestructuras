#ifndef __SETS_H__
#define __SETS_H__
// #include "itree.h"

// #include "cola.h"

// #include "gdclist.h"

// typedef ITNodo SetElem;
// typedef ITree Set;

typedef enum _TipoDefinicion {
  Comprension,
  Extension
} TipoDefinicion;

typedef struct {
  int left, right;
} Intervalo;

typedef struct {
  unsigned nElems;
  GList set;
} Set;

// typedef struct {
//   unsigned nElems;
//   Intervalo* set;
// } Set;



Set* set_create(TipoDefinicion tipo, unsigned nElems, GList elements);

void set_destroy(Set* set);

// Set set_union(Set set1, Set set2);

// Set set_intersection(Set set1, Set set2);

// Set set_substraction(Set set1, Set set2);

// Set set_complement(Set set);

// void set_print(Set set);












#endif /* __SETS_H__ */