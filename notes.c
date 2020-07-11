#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sets.h"
#include "gdclist.h"

#define DEFAULT_STR_SIZE 100
#define INPUT_GROWTH_RATE 1.5

// unsigned long hashInt(int n1, int n2) {
//   printf("%u - %u - %u - %u\n", (unsigned) n1, (unsigned) n2, ((unsigned) n1)+4, ((unsigned) n1)+5);
//   return (5381 * 33 + n1) * 33 + n2;
// }

// unsigned long hashStr(unsigned char *str) {
//   unsigned long hash = 5381;
//   int c;
//   while ((c = *str++)) {
//     hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
//     printf("it: %lu - %c\n", hash, c);
//   }
//   return hash;
// }

// void foo() {
//   char* input = malloc(sizeof(char)*DEFAULT_STR_SIZE);
//   size_t len_input = DEFAULT_STR_SIZE;
//   printf("Ingrese un char: ");
//   char c = fgetc(stdin);
//   for (int i = 0; c != '\n'; i++) {
//     if (i == len_input) {
//       len_input *= INPUT_GROWTH_RATE;
//       input = realloc(input, sizeof(char)*len_input);
//     }
//     input[i] = c;
//     c = fgetc(stdin);
//   }
// }

// int main() {
//   unsigned char str[] = "paradoxicalistically";
//   // printf("FINAL HASH: %lu\n", hashStr(str));
//   // printf("FINAL HASH: %lu\n", ULONG_MAX);
//   printf("%lu - %lu\n", hashInt(-4, 6)%20, hashInt(4, 6)%20);
//   return 0;
// }




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

void imprimir(int dato) { // TO DELETE. !!! ALSO DELETE EVERYTHING RELATED TO COLAS IN THIS FILE
  printf("%d - ", dato);
}

void destruir_int(void* n) {
  free(n);
}

void validar_input(char *input) {

          // char buffer[DEFAULT_STR_SIZE];
          GList elements = gdclist_crear();
          puts("Entró a CREAR POR EXTENSION");
          char c = ',';
          int i = 3;
          unsigned nElems = 0;
          printf("input3: %s - i: %d - c: %c\n", input, i, c);
          sscanf(input, "{%s", input);
          printf("input3: %s - i: %d - c: %c\n", input, i, c);
          while (i == 3 && c == ',') {
            printf("input3: %s - i: %d - c: %c\n", input, i, c);
            int *dato = malloc(sizeof(int));
            i = sscanf(input, "%d%c%s", dato, &c, input);
            if (i > 0) {
              elements = gdclist_agregar_final(elements, dato);
              puts("Agrgó");
            }
            nElems++;
          }
          gdclist_imprimir(elements);
          printf("input3: %s - i: %d - c: %c\n", input, i, c);

          puts("Hola 2");

          Set* newSet = set_create(Extension, nElems, elements);
          printf("newSet: %u elementos\n", newSet->nElems);
          GNodo* temp = newSet->set;
          for (int it = 0; it<newSet->nElems; it++) {
            printf("%d:%d - ", ((Intervalo*)temp->dato)->left, ((Intervalo*)temp->dato)->right);
            temp = temp->sig;
          }
          gdclist_destruir(elements, destruir_int);

}
    // char *buffer1 = malloc(sizeof(char) * DEFAULT_STR_SIZE);
    // char *buffer2 = malloc(sizeof(char) * DEFAULT_STR_SIZE);
    // char *buffer3 = malloc(sizeof(char) * DEFAULT_STR_SIZE);
    // int numEscaneos = sscanf(input, "%s %s %[^\n]\n", buffer1, buffer2, buffer3);
    // printf("%s\n", buffer1);
    // sscanf(buffer3, "%s", buffer1);
    // printf("%s\n", buffer1);
    // // rewind(stdin);
    // printf("escaneos: %s : %d — %s/%s/%s\n", input, numEscaneos, buffer1, buffer2, buffer3);
    // printf("escaneos: %s\n", buffer3);
    // // numEscaneos = sscanf(input, "%c%c", &chars[3], &chars[4]);
    // // printf("escaneos: %d - %c - %c .\n", numEscaneos, chars[3], chars[4]);

    // sscanf(input, "%s%c%c%c%c%s%c%c%c%s", );
/*      %s     %c     %c     %c     %c     %s     %c     %c     %c     %s
       salir
    imprimir  alias1
      alias1   ' '    '='    ' '     ~    alias2  
      alias1   ' '    '='    ' '     alias2       ' '    '|'    ' '   alias3
      alias1   ' '    '='    ' '     alias2       ' '    '&'    ' '   alias3
      alias1   ' '    '='    ' '     alias2       ' '    '-'    ' '   alias3
      alias1   ' '    '='    ' '     ~    alias2  
      
o mejor

1. chequear si quiere salir
2. sscanf(input, "%s %s", buff, input);
3. chequear si quiere imprimir
4. sscanf(input, " = %c%s", char1, input);
5. chequear:
      a. Si quiere complemento (char1 == '~'). En ese caso, input debe estar almacenado en memoria.
      b. Si quiere crear un conjunto (char1 == '{'). En ese caso, 
         sscanf(input, "%c%s", char3, input). Si char3 es una letra, se quiere definir por comprensión.
         En ese caso: sscanf(input, " : %d <= %c <= %d%c", izq, char4, der, rightBrace); (chequear que char3 == char4 y rightBrace == '}')
         Caso contrario, iterar el sscanf chequeando las comas y llaves para ver cuando terminar el conjunto.
      c. Si no quiere ninguna de las dos, GUARDAR el char (porque es el primer char de alias1)
6. sscanf(input, "%s %c %s", alias1, char2, alias2);
7. Para que el comando sea valido:
      a. strcat(char1, alias1) y alias2 deben estar almacenados en memoria
      b. char2 debe ser '|', '&' o '-'



      */



//     if (numEscaneos == 8) {
//       estado = Salir;
//       for (int i = 0; i < 6 && estado!= ComandoNoValido; i++) {
//         estado = validar_char(estado, charsNoNumeros[i], i);
//       }
//       if (estado != ComandoNoValido && intv->izq > intv->der)
//         estado = IntervaloNoValido;
//     } else
//       estado = ComandoNoValido;
//     free(buffer1);
//     free(buffer2);
//     free(buffer3);
// //   }
// return estado; /* Se devuelve el estado correspondiente. */
// }



void get_input(char** input) {
  char c = fgetc(stdin);
  size_t len_input = DEFAULT_STR_SIZE;
  size_t i = 0;
  for (; c != '\n'; i++) {
    if (i == len_input-2) {
      len_input *= INPUT_GROWTH_RATE;
      *input = realloc(input, sizeof(char)*len_input);
    }
    (*input)[i] = c;
    c = fgetc(stdin);
  }
  (*input)[i] = '\n';
  (*input)[i+1] = '\0';
}



/* La función interface recibe input mientras no se desee salir del intérprete
y realiza las acciones correspondientes sobre el ITree creado. */
int main() {
  char* input = malloc(sizeof(char)*DEFAULT_STR_SIZE);


  get_input(&input);
  while (strcmp(input, "aloha") != 0) {
  validar_input(input);
  get_input(&input);
  }
  free(input);
  // ....
  return 0;
}
