#include "interprete.h"
// #include "sets.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define DEFAULT_STR_SIZE 50
#define INPUT_GROWTH_RATE 1.5
#define HASH_TABLE_DEF_SIZE 29

/* Archivo con las funciones que componen el intérprete. */




unsigned long hashStrings(void* str) {
  char* toHash = (char*)str;
  unsigned long hash = 1297; // Primo arbitrario
  int c = toHash[0];
  int j=0;
  while (c != '\0') {
    hash = ((hash << 5) + hash) + c;
    j++;
    c = toHash[j];
    // printf("it: %lu - %c\n", hash, c);
  }
  return hash;
}


int igualesStr(void* clave1, void* clave2) {
  return (strcmp((char*)clave1, (char*)clave2) == 0) ? 1 : 0;
}


// void validar_char(Estado* estado, char input, int c) {
//   char *plantilla = "  [, ]"; /* Se utiliza una "plantilla" con los caracteres correctos para comparar. */
//   if (c == 0) {
//     if (input == 'i')
//       return Insertar;
//     else if (input == 'e')
//       return Eliminar;
//     else
//       return (input == '?') ? Intersecar : ComandoNoValido;
//   }
//   else if (plantilla[c] != input) /* Si el caracter no es el esperado, el comando no es válido. */
//     return ComandoNoValido;
//   else
//     return estado;
// }


void* validar_conjunto(TablaHash* listaConjuntos, char* conjunto) {
  return tablahash_buscar(listaConjuntos, conjunto);
}


// int validar_comando(char* input, Estado* estado) {
//   char buffer[DEFAULT_STR_SIZE];
//   switch (estado->estadoInput) {
//     case Salir:\
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

void actualizar_estado(Estado* estado, enum EstadoInput estInput, enum TipoError error, int condicion) {
  if (condicion) 
    estado->estadoInput = estInput;
  else 
    estado->tipoError = error;
  }

enum EstadoInput validar_operacion(char operacion) {
  switch (operacion) {
    case '-':
      return Resta;
    case '|':
      return Unir;
    default:
      return Intersecar;
  }
}

void imprimir(int dato) { // TO DELETE. !!! ALSO DELETE EVERYTHING RELATED TO COLAS IN THIS FILE
  printf("%d - ", dato);
}


void validar_input(char *input, TablaHash* conjs, Estado* estado) {
  int cond = 0;
  // puts("Entró a validar_input");
  int numEscaneos = sscanf(input, "%s %s %[^\n]\n", estado->alias[0], estado->alias[1], estado->alias[2]);
  // printf("Pudo escanear %d. BUFFERS: %s-%s-%s\n", numEscaneos, estado->alias[0], estado->alias[1], estado->alias[2]);
  if (numEscaneos == 1 && strcmp(estado->alias[0], "salir") == 0) {
    // printf("Entró a SALIR: %s - %s - %s\n", estado->alias[0], estado->alias[1], estado->alias[2]);
    estado->estadoInput = Salir;
  } else if (numEscaneos == 2 && strcmp(estado->alias[0], "imprimir") == 0) {
    cond = validar_conjunto(conjs, estado->alias[1]) != NULL;
    actualizar_estado(estado, Imprimir, ConjuntoInexistente, cond);
    // printf("Entró a IMPRIMIR: %s - %s - %s\n", estado->alias[0], estado->alias[1], estado->alias[2]);
  } else if (numEscaneos == 3 && strcmp(estado->alias[1], "=") == 0) {
    // printf("Entró a VER QUE MIERDA PASA: %s - %s - %s\n", estado->alias[0], estado->alias[1], estado->alias[2]);
    // switch (estado->alias[2][0]) {
      // puts("Entró al switch");
    if (estado->alias[2][0] == '~') {
      // puts("Entró a COMPLEMENTO");
      cond = validar_conjunto(conjs, &(estado->alias[2][1])) != NULL;
      actualizar_estado(estado, Complemento, ConjuntoInexistente, cond);
    } else if (estado->alias[2][0] == '{') {
      // case '{':
      if (isalpha(estado->alias[2][1])) {
        // puts("Entró a CREAR POR COMPRENSION");
        char c1, c2, c3, c4;
        int *par = malloc(sizeof(int)*2);
        numEscaneos = sscanf(estado->alias[2], "{%c : %d <= %c <= %d%c%c", &c1, &(par[0]), &c2, &(par[1]), &c3, &c4);
        if (numEscaneos == 5 && c1 == c2 && c3 == '}') {
          estado->estadoInput = CrearPorComprension;
          Intervalo* intv = malloc(sizeof(Intervalo));
          // for (int i = 0; i<2; i++)
          //   estado->elements = gdclist_agregar_final(estado->elements, &(par[i]));
          intv->izq = par[0];
          intv->der = par[1];
          estado->elements = itree_insertar(estado->elements, intv);
          free(intv);
          estado->nElems = 2;
          // cond = numEscaneos = 5 && c1 == c2 && c3 == '}';
          // printf("CHARS: %d %d %d\n", c1, c2, c3);
          // actualizar_estado(estado, CrearPorComprension, ComandoNoValido, cond);
        } else if ((numEscaneos == 2 && c1 == '}') || (numEscaneos == 5 && par[0] > par[1])) {
          estado->elements = NULL;
          estado->estadoInput = CrearPorComprension;
        } else 
          estado->tipoError = ComandoNoValido;
      } else if (isdigit(estado->alias[2][1]) || estado->alias[2][1] == 45) {
        // puts("Entró a CREAR POR EXTENSION");
        char c = ',', buffChar;
        // printf("buffer3: %s - i: %d - c: %c\n", estado->alias[2], numEscaneos, c);
        numEscaneos = sscanf(estado->alias[2], "{%[^}]%c", estado->alias[2], &buffChar);
        // printf("buffer3: %s - i: %d - c: %c\n", estado->alias[2], numEscaneos, c);
        if (numEscaneos == 2 && buffChar == '}') {
          numEscaneos = 3;
          int dato;
          estado->elements = itree_crear();
          Intervalo* intv = malloc(sizeof(Intervalo));
          // puts("ALOOO2");
          while (numEscaneos == 3 && c == ',') {
            numEscaneos = sscanf(estado->alias[2], "%d%c%s", &dato, &c, estado->alias[2]);
            if (numEscaneos > 0) {
              intv->izq = dato;
              intv->der = dato;
              estado->elements = itree_insertar(estado->elements, intv);
              // int* elem = malloc(sizeof(int));
              // *elem = dato;
              // estado->elements = gdclist_agregar_final(estado->elements, elem);
              (estado->nElems)++;
              // puts("ALOOO");
            }
          }
          // gdclist_imprimir(estado->elements);
          // printf("buffer3: %s - i: %d - c: %c\n", estado->alias[2], numEscaneos, c);
          if (numEscaneos == 1) {
            estado->estadoInput = CrearPorExtension;
            // itree_imprimir(estado->elements);
          } else {
            estado->tipoError = ComandoNoValido;
            itree_destruir(estado->elements);
          }
          free(intv);
        } else estado->tipoError = ComandoNoValido;
        // printf("Quiere imprimir la cola: "); gdcli(estado->elements, imprimir);
      } else
        estado->tipoError = ComandoNoValido;
    } else {
    // default:
      // puts("Operación entre conjuntos");
      char oper;

      numEscaneos = sscanf(estado->alias[2], "%s %c %s", estado->alias[1], &oper, estado->alias[2]);
      if (numEscaneos == 3 && (oper == '-' || oper == '|' || oper == '&')) {
        cond = validar_conjunto(conjs, estado->alias[1]) && validar_conjunto(conjs, estado->alias[2]);
        actualizar_estado(estado, validar_operacion(oper), ConjuntoInexistente, cond);
      } else
        estado->tipoError = ComandoNoValido;
    }
  } else {
    // puts("error");
    estado->tipoError = ComandoNoValido;
  }
}


void preparar_estado(Estado* estado) {
  estado->tipoError = NoError;
  estado->estadoInput = Imprimir;
  estado->alias[0][0] = '\0';
  estado->alias[1][0] = '\0';
  estado->alias[2][0] = '\0';
  estado->nElems = 0;
  estado->elements = NULL;
}


size_t get_input(char** input) {
  // puts("entró a get_input");
  char c = fgetc(stdin);
  size_t len_input = DEFAULT_STR_SIZE;
  size_t i = 0;
  for (; c != '\n'; i++) {
    if (i == len_input-2) {
      len_input *= INPUT_GROWTH_RATE;
      // puts("Reallocating input");
      *input = realloc(*input, sizeof(char)*len_input);
    }
    (*input)[i] = c;
    c = fgetc(stdin);
  }
  (*input)[i] = '\n';
  (*input)[i+1] = '\0';
  // puts("successfuly salió de get_input");
  return len_input;
}

void destruir_int(void* n) {
  free(n);
}

void insertar_conjunto(TablaHash* conjs, Estado* estado) {
  // printf("ELEMENTOS: %u\n", estado->nElems);
  // Set* newSet = set_create(tipo, estado->nElems, estado->elements);
  char* nombreConjunto = malloc(sizeof(char)*(strlen(estado->alias[0])+1));
  // printf("ALIAS 0 : %s\n", estado->alias[0]);
  // printf("NOMBRECONJUNTO : %s\n", nombreConjunto);
  strcpy(nombreConjunto, estado->alias[0]);
  // printf("NOMBRECONJUNTO : %s\n", nombreConjunto);
  tablahash_insertar(conjs, nombreConjunto, estado->elements);
}

// CREAR CABECERA APARTE PARA ESTADO

// Estado* crear_estado();

void destruir_estado(Estado* estado) {
  for (int i=0; i<3; i++)
    free(estado->alias[i]);
  free(estado);
}

// void destruir_set(void* set) {
//   set_destroy((Set*)set);
// }

//   MEJORAR MANEJO DE LA MEMORIA


void destruir_set(void* set) {
  itree_destruir((ITree) set);
}

// long generar1(int elem) {
//   return ((long)elem)+8;
// }


/* La función interface recibe input mientras no se desee salir del intérprete
y realiza las acciones correspondientes sobre el ITree creado. */
void interface() {
  char* input = malloc(sizeof(char)*DEFAULT_STR_SIZE);
  // puts("entro a interface");
  size_t len_input = get_input(&input); // fix this, probably don't need the ampersand
  size_t len_input2;
  // puts("successfully got input");
  TablaHash* listaConjuntos = tablahash_crear(HASH_TABLE_DEF_SIZE, hashStrings, igualesStr, destruir_set);
  // puts("Created hash table");

  Estado *estado = malloc(sizeof(Estado));
  for (int i=0; i<3; ++i) {
    // puts("Allocating aliases");
    estado->alias[i] = malloc(sizeof(char)*len_input);
  }
  preparar_estado(estado);

  // GList elements = gdclist_crear();
  // ITree elements = itree_crear();
  

  // puts("Initialized and prepared state");

  // gdclist_imprimir(elements);

  validar_input(input, listaConjuntos, estado);

  // gdclist_imprimir(elements);

  // printf("%u - %u\n", estado->estadoInput, estado->tipoError);
  
  // ....
  while (estado->estadoInput != Salir) {
    // puts("interface: Entro al while");
    if (estado->tipoError != NoError) {
      puts("Comando no válido.");
    } else {
      switch (estado->estadoInput) {
        case Imprimir:
          itree_imprimir(tablahash_buscar(listaConjuntos, estado->alias[1]));
          puts("");
          // puts("Imprimir.");
        break;
        case CrearPorExtension:
          // puts("CrearPorExtension.");
          insertar_conjunto(listaConjuntos, estado);
          puts("Conjunto creado.");
        break;
        case CrearPorComprension:
          // puts("CrearPorComprension.")
          insertar_conjunto(listaConjuntos, estado);
          puts("Conjunto creado.");
        break;
        case Unir:
          estado->elements = itree_union(tablahash_buscar(listaConjuntos, estado->alias[1]), 
                                         tablahash_buscar(listaConjuntos, estado->alias[2]));
          insertar_conjunto(listaConjuntos, estado);
          // puts("Unir.");
        break;
        case Intersecar:
          estado->elements = itree_interseccion(tablahash_buscar(listaConjuntos, estado->alias[1]),
                                                tablahash_buscar(listaConjuntos, estado->alias[2]));
          insertar_conjunto(listaConjuntos, estado);
          // puts("Intersecar.");
        break;
        case Resta: {
          estado->elements = itree_complemento(tablahash_buscar(listaConjuntos, estado->alias[2]));
          estado->elements = itree_interseccion(tablahash_buscar(listaConjuntos, estado->alias[1]), estado->elements);
          insertar_conjunto(listaConjuntos, estado);
          // puts("Resta.");
        }
        break;
        case Complemento:
          estado->elements = itree_complemento(tablahash_buscar(listaConjuntos, &(estado->alias[2][1])));
          insertar_conjunto(listaConjuntos, estado);
          // puts("Complemento.");
        break;
        default: return;
      }
    }
    len_input2 = get_input(&input);
    if (len_input2 > len_input) {
      len_input = len_input2;
    //  puts("ReaLLOCEd");
      for (int i=0; i<3; ++i)
        estado->alias[i] = realloc(estado->alias[i], len_input);
    }
    preparar_estado(estado);
    validar_input(input, listaConjuntos, estado);
  }

  // estado->elements = itree_crear_sucesion(generar1, Positivos, 0);
  // strcpy(estado->alias[0], "DRAFT");
  // insertar_conjunto(listaConjuntos, estado);


  // puts("Salir");
  imprimir_th(listaConjuntos);
  free(input);
  destruir_estado(estado);
  // gdclist_destruir(elements, destruir_int);
  // for (int i=0; i<3; i++) free(operandos[i]);
  // free(operandos);
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

