#include "interprete.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_STR_SIZE 50
#define INPUT_GROWTH_RATE 1.5
#define HASH_TABLE_DEF_SIZE 29

/* Archivo con las funciones que componen el intérprete. */


unsigned long hashStrings(void* str) {
  char* toHash = (char*)str;
  unsigned long hash = 1297; // Primo arbitrario
  int c;
  while ((c = *toHash++)) {
    hash = ((hash << 5) + hash) + c;
    // printf("it: %lu - %c\n", hash, c);
  }
  return hash;
}


int igualesStr(void* clave1, void* clave2) {
  return strcmp((char*)clave1, (char*)clave2);
}



/* validar_char busca verificar si el caracter dado es el esperado en los comandos de
Insertar, Eliminar e Intersecar. */
void validar_char(char c, Estado* estado) {
  switch(c) {
    case '|':
      estado->estadoInput = Unir;
      break;
    case '&':
      estado->estadoInput = Intersecar;
      break;
    case '-':
      estado->estadoInput = Resta;
      break;
    default:
      estado->tipoError = ComandoNoValido;
  }
}


void* validar_conjunto(TablaHash* listaConjuntos, char* conjunto) {
  return tablahash_buscar(listaConjuntos, conjunto);
}


// int validar_comando(char* input, Estado* estado) {
//   char buffer[DEFAULT_STR_SIZE];
//   switch (estado->estadoInput) {
//     case Salir:
//       return strcmp(input, "salir\n") == 0;
//       break;
//     case Imprimir:
//       sscanf(input, "%s %s", buffer, estado->operandos[0]);
//       return validar_conjunto();
//   }
// }

/* 
sscanf(input, "%s %s %s %c %s", )
---
sscanf(input, "%s %s %s",);


*/

void validar_input(char *input, TablaHash* conjs, Estado* estado) {

}





// ———————————————————————————————

// void validar_input(char *input, TablaHash* conjs, Estado* estado) {
//   // char* buffer = malloc(sizeof(char)*DEFAULT_STR_SIZE);
//   int numEscaneos = sscanf(input, "%s %s", estado->operandos[0], input);
//   // Check if numEscaneos is useful somewhere, if not delete it
//   if (strcmp(input, "salir\n") == 0) {
//     estado->estadoInput = Salir;
//   } else if (strcmp(estado->operandos[0], "imprimir") == 0) {
//     if (validar_conjunto(conjs, input))
//       estado->estadoInput = Imprimir;
//     else estado->tipoError = ConjuntoInexistente;
//   } else {
//     char* chars = malloc(sizeof(char)*4);
//     numEscaneos = sscanf(input, "= %c%s", &(chars[0]), input);
//     void* validezConj = validar_conjunto(conjs, estado->operandos[0]);
//     switch(chars[0]) {
//       case '~':
//         if (validar_conjunto(conjs, input) && validezConj)
//           estado->estadoInput = Complemento;
//         else 
//           estado->tipoError = ConjuntoInexistente;
//       break;
//       case '{':
//         puts("Completar lectura de conjuntos");
//         estado->estadoInput = CrearPorExtension;
//       break;
//       default:
//         estado->operandos[1][0] = chars[0];
//         sscanf("%s %c %s", &(estado->operandos[1][1]), &(chars[1]), estado->operandos[2]);
//         validar_char(chars[1], estado);
//         if (estado->tipoError && !(validezConj && validar_conjunto(conjs, estado->operandos[1]) && validar_conjunto(conjs, estado->operandos[2])))
//           estado->tipoError = ConjuntoInexistente;

//     }
//   }
// }


// ———————————————————————————————

  // while (!terminar && !estado->tipoError) {
  //   switch (estado->estadoInput) {
  //     case Salir:
  //       if (strcmp(input, "salir\n") == 0)
  //         terminar = 1;
  //       break;
  //     case Imprimir: {
  //       sscanf(input, "%s %s", buffer, input);
  //       int a = strcmp(buffer, "imprimir") == 0, b = validar_conjunto(listaConjuntos, input);
  //       if (a ^ b)
  //         estado->tipoError = (a && !b) ? ConjuntoInexistente : ComandoNoValido;
  //       else if (a && b) {
  //         estado->operandos[0] = input;
  //         estado->estadoInput = Imprimir;
  //         terminar = 1;
  //       }
  //       break;
  //     }
  //     case CrearPorExtension:
      
  //       sscanf();

  //   }
  //   if (!terminar) estado->estadoInput++;
  // }
  //   rewind(stdin);

  //   free(buffer);


// ———————————————————————————————

  // estado->tipoError = NULL;
  // estado->estadoInput = NULL;
  // if (strcmp(input, "salir\n") == 0) {
  //   estado->estadoInput = Salir;
  // } else {
  //   char* buffer = malloc(sizeof(char)*DEFAULT_STR_SIZE);
  //   int numEscaneos = sscanf(input, "%s %s", buffer, input);
  //   if (numEscaneos == 2 && strcmp(buffer, "imprimir") == 0) {
  //     if (validar_conjunto(listaConjuntos, input))
  //       estado->estadoInput = Imprimir;
  //     else 
  //       estado->tipoError = ConjuntoInexistente;
  //   } else if (validar_conjunto(listaConjuntos, buffer)) {
  //     estado->operandos[0] = buffer;
  //     char* chars = malloc(sizeof(char)*5);
  //     numEscaneos = sscanf(input, "= %c%s", &(chars[0]), input);
      
  //     if (numEscaneos == 2 && chars[0] == '~' && validar_conjunto(listaConjuntos, input)) {
  //       estado->estadoInput = Complemento;
  //     } else if (chars[1] == '{') {
  //       creae_conjunto(input, estado);
  //     }
  //   }

//     free(buffer);
//   }

//   return estado;
// }

// ———————————————————————————————

// /* validar_input primero determina si el comando indica salir o imprimir el árbol,
// y en caso contrario valida el input según las posibilidades de Insertar, Eliminar e Intersecar. */
// enum EstadoInput validar_input(char *input, Intervalo *intv) {
//   enum EstadoInput estado;
//   if (strcmp(input, "salir\n") == 0)
//     estado = Salir;
//   else if (strcmp(input, "bfs\n") == 0)
//     estado = RecorridoBFS;
//   else if (strcmp(input, "dfs\n") == 0)
//     estado = RecorridoDFS;
//   else {
//     char *charsNoNumeros = malloc(sizeof(char) * 6), *finalInput = malloc(sizeof(char) * DEFAULT_STR_SIZE);
//     /* Se escanea el input de acuerdo al formato válido, y si se asignaron correctamente exactamente 8 valores
//     (que siempre serán los primeros 8), el formato en cuanto a cantidad de caracteres es correcto y se procede
//     a validad caracter por caracter. */
//     int numEscaneos = sscanf(input, "%c%c%c%lf%c%c%lf%c%s", &charsNoNumeros[0], &charsNoNumeros[1], &charsNoNumeros[2],
//       &(intv->izq), &charsNoNumeros[3], &charsNoNumeros[4], &(intv->der), &charsNoNumeros[5], finalInput);
//     if (numEscaneos == 8) {
//       estado = Salir;
//       for (int i = 0; i < 6 && estado!= ComandoNoValido; i++) {
//         estado = validar_char(estado, charsNoNumeros[i], i);
//       }
//       if (estado != ComandoNoValido && intv->izq > intv->der)
//         /* Se contempla el caso en el que el formato sea válido pero el intervalo no. */
//         estado = IntervaloNoValido;
//     } else
//       estado = ComandoNoValido;
//     free(charsNoNumeros);
//     free(finalInput);
//   }
// return estado; /* Se devuelve el estado correspondiente. */
// }


size_t get_input(char** input) {
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
  return len_input;
}



/* La función interface recibe input mientras no se desee salir del intérprete
y realiza las acciones correspondientes sobre el ITree creado. */
void interface() {
  char* input = malloc(sizeof(char)*DEFAULT_STR_SIZE);
  char** operandos = malloc(sizeof(char*)*3);
  size_t len_input = get_input(&input); // fix this, probably don't need the ampersand
  for (int i=0; i<3; i++)
    operandos[i] = malloc(sizeof(char)*len_input);

  TablaHash* listaConjuntos = tablahash_crear(HASH_TABLE_DEF_SIZE, hashStrings, igualesStr);

  Estado *estado = malloc(sizeof(Estado));

  validar_input(input, listaConjuntos, estado);
  
  // ....
  while  (estado->estadoInput != Salir) {
    if (estado->tipoError) {
      puts("Hubo un error en el comando.");
    } else {
      switch (estado->estadoInput) {
        case Imprimir:
        puts("Imprimir.");
        break;
        case CrearPorExtension:
        puts("CrearPorExtension.");
        break;
        case CrearPorComprension:
        puts("CrearPorComprension.");
        break;
        case Unir:
        puts("Unir.");
        break;
        case Intersecar:
        puts("Intersecar.");
        break;
        case Resta:
        puts("Resta.");
        break;
        case Complemento:
        puts("Complemento.");
        break;
        default: return;
      }
    }
    get_input(&input);
    validar_input(input, listaConjuntos, estado);
  }

  free(input);
  free(estado);
  for (int i=0; i<3; i++) free(operandos[i]);
  free(operandos);
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


