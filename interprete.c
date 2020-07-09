#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "interprete.h"

#define DEFAULT_STR_SIZE 50
#define INPUT_GROWTH_RATE 1.5

/* Archivo con las funciones que componen el intérprete. */


unsigned long hashStrings(char *str) {
  unsigned long hash = 1297; // Primo arbitrario
  int c;
  while ((c = *str++)) {
    hash = ((hash << 5) + hash) + c;
    printf("it: %lu - %c\n", hash, c);
  }
  return hash;
}

int igualesStr(void* clave1, void* clave2) {
  return strcmp((char*)clave1, (char*)clave2);
}



/* validar_char busca verificar si el caracter dado es el esperado en los comandos de
Insertar, Eliminar e Intersecar. */
enum EstadoInput validar_char(enum EstadoInput estado, char input, int c) {
  char *plantilla = "  [, ]"; /* Se utiliza una "plantilla" con los caracteres correctos para comparar. */
  if (c == 0) {
    if (input == 'i')
      return Insertar;
    else if (input == 'e')
      return Eliminar;
    else
      return (input == '?') ? Intersecar : ComandoNoValido;
  }
  else if (plantilla[c] != input) /* Si el caracter no es el esperado, el comando no es válido. */
    return ComandoNoValido;
  else
    return estado;
}


enum EstadoInput validar_input(char *input) {
  enum EstadoInput estado;
  if (strcmp(input, "salir\n") == 0)
    estado = Salir;
//   else {
//     char *charsNoNumeros = malloc(sizeof(char) * 6), *finalInput = malloc(sizeof(char) * DEFAULT_STR_SIZE);
//     int numEscaneos = sscanf(input, "%c%c%c%lf%c%c%lf%c%s", &charsNoNumeros[0], &charsNoNumeros[1], &charsNoNumeros[2],
//       &(intv->izq), &charsNoNumeros[3], &charsNoNumeros[4], &(intv->der), &charsNoNumeros[5], finalInput);
//     if (numEscaneos == 8) {
//       estado = Salir;
//       for (int i = 0; i < 6 && estado!= ComandoNoValido; i++) {
//         estado = validar_char(estado, charsNoNumeros[i], i);
//       }
//       if (estado != ComandoNoValido && intv->izq > intv->der)
//         estado = IntervaloNoValido;
//     } else
//       estado = ComandoNoValido;
//     free(charsNoNumeros);
//     free(finalInput);
//   }
// return estado; /* Se devuelve el estado correspondiente. */
}

/* validar_input primero determina si el comando indica salir o imprimir el árbol,
y en caso contrario valida el input según las posibilidades de Insertar, Eliminar e Intersecar. */
enum EstadoInput validar_input(char *input, Intervalo *intv) {
  enum EstadoInput estado;
  if (strcmp(input, "salir\n") == 0)
    estado = Salir;
  else if (strcmp(input, "bfs\n") == 0)
    estado = RecorridoBFS;
  else if (strcmp(input, "dfs\n") == 0)
    estado = RecorridoDFS;
  else {
    char *charsNoNumeros = malloc(sizeof(char) * 6), *finalInput = malloc(sizeof(char) * DEFAULT_STR_SIZE);
    /* Se escanea el input de acuerdo al formato válido, y si se asignaron correctamente exactamente 8 valores
    (que siempre serán los primeros 8), el formato en cuanto a cantidad de caracteres es correcto y se procede
    a validad caracter por caracter. */
    int numEscaneos = sscanf(input, "%c%c%c%lf%c%c%lf%c%s", &charsNoNumeros[0], &charsNoNumeros[1], &charsNoNumeros[2],
      &(intv->izq), &charsNoNumeros[3], &charsNoNumeros[4], &(intv->der), &charsNoNumeros[5], finalInput);
    if (numEscaneos == 8) {
      estado = Salir;
      for (int i = 0; i < 6 && estado!= ComandoNoValido; i++) {
        estado = validar_char(estado, charsNoNumeros[i], i);
      }
      if (estado != ComandoNoValido && intv->izq > intv->der)
        /* Se contempla el caso en el que el formato sea válido pero el intervalo no. */
        estado = IntervaloNoValido;
    } else
      estado = ComandoNoValido;
    free(charsNoNumeros);
    free(finalInput);
  }
return estado; /* Se devuelve el estado correspondiente. */
}


void get_input(char** input) {
  char c = fgetc(stdin);
  size_t len_input = DEFAULT_STR_SIZE;
  for (int i = 0; c != '\n'; i++) {
    if (i == len_input) {
      len_input *= INPUT_GROWTH_RATE;
      *input = realloc(input, sizeof(char)*len_input);
    }
    (*input)[i] = c;
    c = fgetc(stdin);
  }
}



/* La función interface recibe input mientras no se desee salir del intérprete
y realiza las acciones correspondientes sobre el ITree creado. */
void interface() {
  char* input = malloc(sizeof(char)*DEFAULT_STR_SIZE);
  // printf("Ingrese un char: ");
  get_input(&input);

  enum EstadoInput estado = validar_input(input);
  
  // ....
  while  (estado != Salir) {
    switch (estado) {
      // ....
    }
    get_input(&input);
    estado = validar_input(input);
  }
  free(input);
  // ....
}





//   ITree arbol = itree_crear();
//   ITree interseccion;
//   enum EstadoInput estado;
//   Intervalo* intv = malloc(sizeof(Intervalo));
//   fgets(input, STR_SIZE, stdin);
//   estado = validar_input(input, intv);
//   while  (estado != Salir) {
//     /* El siguiente switch contempla los posibles valores del EstadoInput estado. */
//     switch(estado) {
//       case RecorridoBFS:
//         itree_recorrer_bfs(arbol, imprimir_intervalo);
//         puts("");
//       break;
//       case RecorridoDFS:
//         itree_recorrer_dfs(arbol, imprimir_intervalo);
//         puts("");
//       break;
//       case Insertar:
//         arbol = itree_insertar(arbol, intv);
//       break;
//       case Eliminar:
//         arbol = itree_eliminar(arbol, intv);
//       break;
//       case Intersecar:
//         interseccion = itree_intersecar(arbol, intv);
//         if (interseccion) {
//           printf("Si, ");
//           imprimir_intervalo(interseccion);
//           puts("");
//         } else
//           puts("No");
//       break;
//       case ComandoNoValido:
//         puts("Comando no válido.");
//       break;
//       default:
//         puts("Intervalo no válido.");
//     }
//     fgets(input, STR_SIZE, stdin);
//     estado = validar_input(input, intv);
//   }
//   /* Se libera la memoria requerida para buffers y se destruye el ITree. */
//   free(input);
//   free(intv);
//   itree_destruir(arbol);
// }


