#include "estado.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>



Estado* crear_estado() {
  Estado* estado = malloc(sizeof(Estado));
  for (int i=0; i<3; ++i) {
    estado->alias[i] = malloc(sizeof(char)*DEFAULT_STR_SIZE);
  }
  return estado;
}

void destruir_estado(Estado* estado) {
  for (int i=0; i<3; i++)
    free(estado->alias[i]);
  free(estado);
}

void preparar_estado(Estado* estado) {
  estado->tipoError = NoError;
  estado->estadoInput = Imprimir;
  estado->elements = NULL;
}


void actualizar_estado(Estado* estado, enum EstadoInput estadoInput, enum TipoError error, int condicion) {
  if (condicion) 
    estado->estadoInput = estadoInput;
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


void get_input(Estado* estado) {
  char c = fgetc(stdin);
  size_t len_input = DEFAULT_STR_SIZE;
  size_t i = 0;
  for (; c != '\n'; i++) {
    if (i == len_input-2) {
      len_input *= STR_INPUT_GROWTH_RATE;
      for (int i=0; i<3; i++)
        estado->alias[i] = realloc(estado->alias[i], sizeof(char)*len_input);
    }
    estado->alias[2][i] = c;
    c = fgetc(stdin);
  }
  estado->alias[2][i] = '\n';
  estado->alias[2][i+1] = '\0';
}


void* validar_conjunto(TablaHash* listaConjuntos, char* conjunto) {
  return tablahash_buscar(listaConjuntos, conjunto, Check);
}

void validar_input(TablaHash* conjs, Estado* estado) {
  int numEscaneos = sscanf(estado->alias[2], "%s %s %[^\n]\n", estado->alias[0], estado->alias[1], estado->alias[2]);
  if (numEscaneos == 1 && strcmp(estado->alias[0], "salir") == 0) { // SALIR
    estado->estadoInput = Salir;
  } else if (numEscaneos == 2 && strcmp(estado->alias[0], "imprimir") == 0) { // IMPRIMIR
    int cond = validar_conjunto(conjs, estado->alias[1]) != NULL;
    actualizar_estado(estado, Imprimir, ConjuntoInexistente, cond);
  } else if (numEscaneos == 3 && strcmp(estado->alias[1], "=") == 0) { // CREACIÓN U OPERACIONES
    if (estado->alias[2][0] == '~') { // COMPLEMENTO
      int cond = validar_conjunto(conjs, &(estado->alias[2][1])) != NULL;
      actualizar_estado(estado, Complemento, ConjuntoInexistente, cond);
    } else if (estado->alias[2][0] == '{') { // CREACION
      if (isalpha(estado->alias[2][1]) || estado->alias[2][1] == '}') { // POR COMPRENSION
        char c1, c2, c3, c4, space;
        int *par = malloc(sizeof(int)*2);
        numEscaneos = sscanf(estado->alias[2], "{%c%c: %d <= %c <= %d%c%c", &c1, &space, &(par[0]), &c2, &(par[1]), &c3, &c4);
        if (numEscaneos == 1 && c1 == '}') {
          estado->estadoInput = CrearPorComprension;
        } else if (numEscaneos == 6 && c1 == c2 && space == ' ' && c3 == '}') {
          estado->estadoInput = CrearPorComprension;
          if (par[0] <= par[1]) {
            Intervalo* intv = malloc(sizeof(Intervalo));
            intv->izq = par[0];
            intv->der = par[1];
            estado->elements = set_insertar(estado->elements, intv);
            free(intv);
          }
        } else
          estado->tipoError = ComandoNoValido;
      } else if (isdigit(estado->alias[2][1]) || estado->alias[2][1] == 45) { // POR EXTENSIÓN
        char buffChar, buffChar2;
        numEscaneos = sscanf(estado->alias[2], "{%[^}]%c%c", estado->alias[2], &buffChar, &buffChar2);
        if (numEscaneos == 2 && buffChar == '}') {
          numEscaneos = 3;
          char c = ',';
          int dato;
          estado->elements = set_crear();
          Intervalo* intv = malloc(sizeof(Intervalo));
          while (numEscaneos == 3 && c == ',') {
            numEscaneos = sscanf(estado->alias[2], "%d%c%s", &dato, &c, estado->alias[2]);
            if (numEscaneos > 0) {
              intv->izq = dato;
              intv->der = dato;
              estado->elements = set_insertar(estado->elements, intv);
            }
          }
          if (numEscaneos == 1) {
            estado->estadoInput = CrearPorExtension;
          } else {
            estado->tipoError = ComandoNoValido;
            set_destruir(estado->elements);
          }
          free(intv);
        } else
          estado->tipoError = ComandoNoValido;
      } else
        estado->tipoError = ComandoNoValido;
    } else { // OPERACIONES
      char oper;
      numEscaneos = sscanf(estado->alias[2], "%s %c %s", estado->alias[1], &oper, estado->alias[2]);
      if (numEscaneos == 3 && (oper == '-' || oper == '|' || oper == '&')) { // OPERACIONES BINARIAS
        int cond = validar_conjunto(conjs, estado->alias[1]) && validar_conjunto(conjs, estado->alias[2]);
        actualizar_estado(estado, validar_operacion(oper), ConjuntoInexistente, cond);
      } else if (numEscaneos == 1) { // IGUALACIÓN DE CONJUNTOS
        if (validar_conjunto(conjs, estado->alias[1])) {
          estado->elements = set_clonar(tablahash_buscar(conjs, estado->alias[1], Fetch));
          estado->estadoInput = CrearPorExtension;
        } else 
          estado->tipoError = ConjuntoInexistente;
      } else
        estado->tipoError = ComandoNoValido;
    }
  } else {
    estado->tipoError = ComandoNoValido;
  }
}
