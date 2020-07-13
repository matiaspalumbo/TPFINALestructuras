#include "sets.h"
#include "algoritmos.h"
#include <stdlib.h>
#include <stdio.h>

int compare_intervals(const void* intv1, const void* intv2) {
  return ((Intervalo*)intv1)->left - ((Intervalo*)intv2)->left;
}

int compare_ints(void* n1, void* n2) {
  return *((int*)n1) - *((int*)n2);
}

GList agregar_intervalo(GList lista, int left, int right) {
  Intervalo* intv = malloc(sizeof(Intervalo));
  intv->left = left;
  intv->right = right;
  return gdclist_agregar_final(lista, intv);
}

Set* set_create(TipoDefinicion tipo, unsigned nElems, GList elements) {
  puts("WTF");
  Set* newSet = malloc(sizeof(Set));
  newSet->set = gdclist_crear();
  newSet->nElems = 0;
  printf("Useless nElems %d\n", nElems);
  if (tipo == Extension) {
    puts("WTF");
    elements = merge_sort(elements, compare_ints);
    puts("Sorted.");
    gdclist_imprimir(elements);
    elements->ant->sig = NULL;
    puts("Sorted.");
    int left = *(int*)(elements->dato), right = left;
    puts("Sorted.");
    GNodo* temp = elements->sig;
    for (; temp!= NULL; temp = temp->sig) {
      puts("Trying");
      if (*((int*)(temp->ant->dato))+1 == *((int*)(temp->dato))) {
        right = *((int*)(temp->dato));
        printf("ahora es [%d, %d]", left, right);
      }
      else {
        printf("agrega [%d, %d]", left, right);
        newSet->set = agregar_intervalo(newSet->set, left, right);
        (newSet->nElems)++;
        left = *((int*)(temp->dato));
        right = left;
      }
    }
    puts("Tried");
    newSet->set = agregar_intervalo(newSet->set, left, (left < right) ? right : left);
    (newSet->nElems)++;
    puts("Done");
  } else if (!gdclist_es_vacia(elements) || *(int*)(elements->dato) > *(int*)(elements->sig->dato)) {
    newSet->set = agregar_intervalo(newSet->set, *(int*)(elements->dato), *(int*)(elements->sig->dato));
    newSet->nElems = 1;
  }
  return newSet;
}

void destruir_intervalo(void* intv) {
  free(intv);
}

void set_destroy(Set* set) {
  gdclist_destruir(set->set, destruir_intervalo);
  free(set);
}

// Set* set_create(TipoDefinicion tipo, unsigned nElems, GList elements) {
//   puts("WTF");
//   Set* newSet = malloc(sizeof(Set));
//   // newSet->set = malloc(sizeof(Intervalo)*nElems);
//   if (tipo == Extension) {
//     puts("WTF");
//     int* buffer = malloc(sizeof(int)*nElems);
//     for (unsigned i=0; i<nElems; i++) {
//       buffer[i] = elements->primero->dato;
//       // newSet->set[i].left = elements->primero;
//       // newSet->set[i].right = elements->primero;
//       cola_desencolar(elements);
//     }
//     // set_simplify(buffer, nElems);
//     qsort(buffer, nElems, sizeof(int), compare_ints);
//     puts("WTF");
//     int pos = 1;
//     int inicioInt = buffer[0];
//     for (unsigned i=1; i<nElems; i++) {
//       printf("current buffer: %d - %d: inicioInt\n", buffer[i], inicioInt);
//       // puts("iterando...");
//       if (buffer[i] != inicioInt + 1) {
//         puts("Cambia de intervalo");
//         buffer[pos] = buffer[i-1];
//         buffer[pos+1] = buffer[i];
//         inicioInt = buffer[i];
//         pos += 2;
//       } else {
//         puts("Fue acá");
//         inicioInt = buffer[i];
//       }
//     }
//     buffer[pos] = inicioInt;

//     for (unsigned i=0; i<nElems; i++){
//       printf("%d - ", buffer[i]);
//     }
//     puts("");


//     puts("termino de iterar...");
//     newSet->set = malloc(sizeof(Intervalo)*pos);
//     puts("What");
//     int j = 0;
//     for (int i=0; i<pos; i += 2) {
//       puts("itera 2.0...");
//       newSet->set[j].left = buffer[i];
//       newSet->set[j].right = buffer[i+1];
//       j++;
//     }
//     puts("TERMINO");
//     newSet->nElems = pos/2;
//   } else {
//     newSet->set[0].left = elements->primero->dato;
//     newSet->set[0].right = elements->ultimo->dato;
//   }
//   puts("TERMINO");
//   return newSet;
// }

// Set *set_simplify(Set* set, unsigned nElems) {
//   qsort(set->set, set->nElems, sizeof(Intervalo), compare_intervals);
// }


// TODO: que la unión mantenja los intervalos disjuntos. IMPORTANTE

void set_destruir(Set* set) {
  free(set->set);
  free(set);
}