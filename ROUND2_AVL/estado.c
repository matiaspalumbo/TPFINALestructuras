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
  estado->sizeAlias = DEFAULT_STR_SIZE;
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
  estado->numEspacios = 0;
  estado->elements = NULL;
}


void actualizar_estado(Estado* estado, enum EstadoInput estadoInput, enum TipoError error, int condicion) {
  if (condicion) 
    estado->estadoInput = estadoInput;
  else 
    estado->tipoError = error;
}


void get_input(Estado* estado) {
  char c = fgetc(stdin);
  size_t i = 0;
  for (; c != '\n'; i++) {
    if (c == ' ')
      estado->numEspacios++;
    if (i == estado->sizeAlias-1) {
      estado->sizeAlias *= STR_INPUT_GROWTH_RATE;
      for (int j=0; j<3; j++)
        estado->alias[j] = realloc(estado->alias[j], sizeof(char) * estado->sizeAlias);
    }
    estado->alias[2][i] = c;
    c = fgetc(stdin);
  }
  estado->alias[2][i] = '\n';
  estado->alias[2][i+1] = '\0';
  printf("%s%u - espacios: %d\n", estado->alias[2], estado->sizeAlias, estado->numEspacios);
  
}


void* validar_conjunto(TablaHash* listaConjuntos, char* conjunto) {
  return tablahash_buscar(listaConjuntos, conjunto, Check);
}


int validar_nombre(Estado* estado) {
  return estado->alias[0][0] != '~' && estado->alias[0][0] != '{';
} 


void validar_impresion(Estado* estado, TablaHash* conjs) {
  sscanf(estado->alias[2], "%s %[^\n]\n", estado->alias[1], estado->alias[0]);
  if (strcmp(estado->alias[1], "imprimir") == 0) {
    int cond = validar_conjunto(conjs, estado->alias[0]) && validar_nombre(estado);
    actualizar_estado(estado, Imprimir, ConjuntoInexistente, cond);
  } else
    estado->tipoError = ComandoNoValido;
}


void validar_complemento(Estado* estado, TablaHash* conjs) {
  int cond = validar_conjunto(conjs, &(estado->alias[2][1])) != NULL;
  actualizar_estado(estado, Complemento, ConjuntoInexistente, cond);
}


void validar_creacion_extension(Estado* estado) {
  if (strcmp(estado->alias[2], "{}") == 0) {
    estado->estadoInput = CrearPorExtension;
  } else {
    char llave, finalLinea;
    int numEscaneos = sscanf(estado->alias[2], "{%[^}]%c%c", estado->alias[2], &llave, &finalLinea);
    if (numEscaneos == 2 && llave == '}') {
      if (estado->alias[2][0] == '\0') {
        estado->estadoInput = CrearPorExtension;
      } else {
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
      }
    } else
      estado->tipoError = ComandoNoValido;
  }
}


void validar_igualacion_conjuntos(Estado* estado, TablaHash* conjs) {
  if (validar_conjunto(conjs, estado->alias[2])) {
    estado->elements = set_clonar(tablahash_buscar(conjs, estado->alias[2], Fetch));
    estado->estadoInput = CrearPorExtension;
  } else 
    estado->tipoError = ConjuntoInexistente;
}


void validar_operaciones(Estado* estado, TablaHash* conjs) {
  char oper, buffChar;
  int numEscaneos = sscanf(estado->alias[2], "%s %c %s %c %s", estado->alias[0], &buffChar, estado->alias[1], &oper, estado->alias[2]);
  if (numEscaneos == 5 && buffChar == '=' && validar_nombre(estado) && (oper == '-' || oper == '|' || oper == '&')) { // OPERACIONES BINARIAS
    int cond = validar_conjunto(conjs, estado->alias[1]) && validar_conjunto(conjs, estado->alias[2]);
    enum EstadoInput tipoOper = (oper == '-') ? (Resta) : ((oper == '|') ? Unir : Intersecar);
    // switch (oper) {
    //   case '-':
    //     tipoOper = Resta;
    //   case '|':
    //     tipoOper = Unir;
    //   case '&':
    //     tipoOper = Intersecar;
    // }
    actualizar_estado(estado, tipoOper, ConjuntoInexistente, cond);
  } else
    estado->tipoError = ComandoNoValido;
}


void validar_creacion_comprension(Estado* estado) {
  char c1, c2, c3, igual, lineSkip, emptyStr;
  int *par = malloc(sizeof(int)*2);
  int numEscaneos = sscanf(estado->alias[2], "%s %c {%c : %d <= %c <= %d%c%c%c",
    estado->alias[0], &igual, &c1, &(par[0]),
    &c2, &(par[1]), &c3, &lineSkip, &emptyStr);
  if (numEscaneos == 8 && validar_nombre(estado) && c1 == c2 && igual == '=' && c3 == '}') {
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
}


void validar_input(TablaHash* conjs, Estado* estado) {
  if (estado->numEspacios == 0 && strcmp(estado->alias[2], "salir\n") == 0)
    estado->estadoInput = Salir;
  else if (estado->numEspacios == 1)
    validar_impresion(estado, conjs);
  else if (estado->numEspacios == 2) {
    sscanf(estado->alias[2], "%s %s %[^\n]\n", estado->alias[0], estado->alias[1], estado->alias[2]);
    if (validar_nombre(estado) && strcmp(estado->alias[1], "=") == 0) {
      if (estado->alias[2][0] == '~') // COMPLEMENTO
        validar_complemento(estado, conjs);
      else if (estado->alias[2][0] == '{') // CREACION POR EXTENSIÓN
        validar_creacion_extension(estado);
      else
        validar_igualacion_conjuntos(estado, conjs);
    } else
      estado->tipoError = ComandoNoValido;
  } else if (estado->numEspacios == 4)
    validar_operaciones(estado, conjs);
  else if (estado->numEspacios == 8)
    validar_creacion_comprension(estado);
  else
    estado->tipoError = ComandoNoValido;
}

