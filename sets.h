#ifndef __SETS_H__
#define __SETS_H__
#include "itree.h"

typedef ITNodo SetElem;
typedef ITree Set;


Set set_create(int *elements);

Set set_union(Set set1, Set set2);

Set set_intersection(Set set1, Set set2);

Set set_substraction(Set set1, Set set2);

Set set_complement(Set set);

void set_print(Set set);












#endif /* __SETS_H__ */