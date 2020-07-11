#include "interprete.h"
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

/* validar_char busca verificar si el caracter dado es el esperado en los comandos de
Insertar, Eliminar e Intersecar. */
void validar_operacion(char c, Estado* estado) {
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

void imprimir(int dato) { // TO DELETE. !!! ALSO DELETE EVERYTHING RELATED TO COLAS IN THIS FILE
  printf("%d - ", dato);
}

void validar_input(char *input, TablaHash* conjs, Estado* estado, GList elements) {
  // char buffer1[DEFAULT_STR_SIZE];
  // char buffer2[DEFAULT_STR_SIZE];
  // char buffer3[DEFAULT_STR_SIZE];
  int cond = 0;
  int izq, der;
  puts("Entró a validar_input");
  int numEscaneos = sscanf(input, "%s %s %[^\n]\n", estado->alias[0], estado->alias[1], estado->alias[2]);
  printf("Pudo escanear %d. BUFFERS: %s-%s-%s\n", numEscaneos, estado->alias[0], estado->alias[1], estado->alias[2]);
  if (numEscaneos == 1 && strcmp(estado->alias[0], "salir") == 0) {
    printf("Entró a SALIR: %s - %s - %s\n", estado->alias[0], estado->alias[1], estado->alias[2]);
    estado->estadoInput = Salir;
  } else if (numEscaneos == 2 && strcmp(estado->alias[0], "imprimir") == 0) {
    cond = validar_conjunto(conjs, estado->alias[1]) != NULL;
    actualizar_estado(estado, Imprimir, ConjuntoInexistente, cond);
    printf("Entró a IMPRIMIR: %s - %s - %s\n", estado->alias[0], estado->alias[1], estado->alias[2]);
  } else if (numEscaneos == 3 && strcmp(estado->alias[1], "=") == 0) {
    printf("Entró a VER QUE MIERDA PASA: %s - %s - %s\n", estado->alias[0], estado->alias[1], estado->alias[2]);
    switch (estado->alias[2][0]) {
      puts("Entró al switch");
      case '~':
        puts("Entró a COMPLEMENTO");
        cond = validar_conjunto(conjs, &(estado->alias[2][1])) != NULL;
        actualizar_estado(estado, Complemento, ConjuntoInexistente, cond);
      break;
      case '{':
        if (isalpha(estado->alias[2][1])) {
          puts("Entró a CREAR POR COMPRENSION");
          char c1, c2, c3;
          // char buffer4[DEFAULT_STR_SIZE];
          numEscaneos = sscanf(estado->alias[2], "{%c : %d <= %c <= %d%c", &c1, &izq, &c2, &der, &c3);
          cond = numEscaneos = 5 && /* strcmp(buffer4, "}") == 0 && */ c1 == c2 && c3 == '}';
          printf("CHARS: %d %d %d\n", c1, c2, c3);
          actualizar_estado(estado, CrearPorComprension, ComandoNoValido, cond);
        } else if (isdigit(estado->alias[2][1]) || estado->alias[2][1] == 45) {
          puts("Entró a CREAR POR EXTENSION");
          // int dato;
          char c = ',';
          int i = 3;
          printf("buffer3: %s - i: %d - c: %c\n", estado->alias[2], i, c);
          sscanf(estado->alias[2], "{%s", estado->alias[2]);
          printf("buffer3: %s - i: %d - c: %c\n", estado->alias[2], i, c);
          while (i == 3 && c == ',') {
            int *dato = malloc(sizeof(int));
            printf("buffer3: %s - i: %d - c: %c\n", estado->alias[2], i, c);
            i = sscanf(estado->alias[2], "%d%c%s", dato, &c, estado->alias[2]);
            if (i > 0) gdclist_agregar_final(elements, dato);
          }
          printf("buffer3: %s - i: %d - c: %c\n", estado->alias[2], i, c);
          if (i == 2 && c == '}') {
            estado->estadoInput = CrearPorExtension;
          } else
            estado->tipoError = ComandoNoValido;
          // printf("Quiere imprimir la cola: "); gdcli(elements, imprimir);
        } else estado->tipoError = ComandoNoValido;
      break;
      default:
        puts("Operación entre conjuntos");
    }
  } else {
    puts("error");
    estado->tipoError = ComandoNoValido;
  }
  printf("cond: %d\n", cond);
}


void preparar_estado(Estado* estado) {
  estado->tipoError = NoError;
  estado->estadoInput = Imprimir;
  estado->alias[0][0] = '\0';
  estado->alias[1][0] = '\0';
  estado->alias[2][0] = '\0';
}


size_t get_input(char** input) {
  puts("entró a get_input");
  char c = fgetc(stdin);
  size_t len_input = DEFAULT_STR_SIZE;
  size_t i = 0;
  for (; c != '\n'; i++) {
    if (i == len_input-2) {
      len_input *= INPUT_GROWTH_RATE;
      puts("Reallocating input");
      *input = realloc(*input, sizeof(char)*len_input);
    }
    (*input)[i] = c;
    c = fgetc(stdin);
  }
  (*input)[i] = '\n';
  (*input)[i+1] = '\0';
  puts("successfuly salió de get_input");
  return len_input;
}

void destruir_int(void* n) {
  free(n);
}


/* La función interface recibe input mientras no se desee salir del intérprete
y realiza las acciones correspondientes sobre el ITree creado. */
void interface() {
  char* input = malloc(sizeof(char)*DEFAULT_STR_SIZE);
  puts("entro a interface");
  // char** operandos = malloc(sizeof(char*)*3);
  size_t len_input = get_input(&input); // fix this, probably don't need the ampersand
  size_t len_input2;
  puts("successfully got input");
  // for (int i=0; i<3; i++)
    // operandos[i] = malloc(sizeof(char)*len_input);

  TablaHash* listaConjuntos = tablahash_crear(HASH_TABLE_DEF_SIZE, hashStrings, igualesStr);
  puts("Created hash table");

  Estado *estado = malloc(sizeof(Estado));
  for (int i=0; i<3; ++i) {
    puts("Allocating aliases");
    estado->alias[i] = malloc(sizeof(char)*len_input);
  }
  // estado->alias1 = malloc(sizeof(char)*len_input);
  // estado->alias2 = malloc(sizeof(char)*len_input);
  // estado->alias3 = malloc(sizeof(char)*len_input);
  preparar_estado(estado);
  GList elements = gdclist_crear();

  puts("Initialized and prepared state");

  validar_input(input, listaConjuntos, estado, elements);

  printf("%u - %u\n", estado->estadoInput, estado->tipoError);
  
  // ....
  while (estado->estadoInput != Salir) {
    puts("interface: Entro al while");
    if (estado->tipoError != NoError) {
      printf("Hubo un error en el comando: %d\n", estado->tipoError);
    } else {
      switch (estado->estadoInput) {
        case Imprimir:
          puts("Imprimir.");
        break;
        case CrearPorExtension:
          puts("CrearPorExtension.");
          gdclist_destruir(elements, destruir_int);
          elements = gdclist_crear();
        break;
        case CrearPorComprension:
          puts("CrearPorComprension.");
          gdclist_destruir(elements, destruir_int);
          elements = gdclist_crear();
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
    len_input2 = get_input(&input);
    if (len_input2 > len_input) {
      len_input = len_input2;
      puts("ReaLLOCEd");
      for (int i=0; i<3; ++i)
        estado->alias[i] = realloc(estado->alias[i], len_input);
    }
    preparar_estado(estado);
    validar_input(input, listaConjuntos, estado, elements);
  }
  puts("Salir");
  free(input);
  free(estado);
  gdclist_destruir(elements, destruir_int);
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

