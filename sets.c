#include "sets.h"
#include "itree.h"
#include <stdlib.h>
#include <stdio.h>


Set set_create(Intervalo *elements) {
  Set set = itree_crear();
  while (*elements++) {
    set = itree_insertar()
  }
}