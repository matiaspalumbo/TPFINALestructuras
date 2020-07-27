#include "estado.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>


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
  estado->estadoInput = Imprimir; // Se elige Imprimir como placeholder, al no tener ninguna acción pautada.
  estado->numEspacios = 0;
  estado->elements = NULL;
}


/* Si se verifica la condición, se actualiza estado->estadoInput. Sino,
se actualiza estado->tipoError con los parámetros pasados. */
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
    /* Se lleva la cuenta de los espacios presentes 
    para facilitar la posterior validación del input. */
    if (c == ' ')
      estado->numEspacios++;
    /* Si el input recibido excede el tamaño de los alias, se pide más memoria. */
    if (i == estado->sizeAlias-1) {
      estado->sizeAlias *= STR_INPUT_GROWTH_RATE;
      for (int j=0; j<3; j++)
        estado->alias[j] = realloc(estado->alias[j], sizeof(char) * estado->sizeAlias);
    }
    estado->alias[2][i] = c;
    c = fgetc(stdin);
  }
  /* Se agrega un salto de línea para facilitar la posterior validación del input. */
  estado->alias[2][i] = '\n';
  estado->alias[2][i+1] = '\0';
  
}


/* Verifica si existe un conjunto con el nombre dado en la Tabla Hash. */
void* validar_conjunto(TablaHash* listaConjuntos, char* conjunto) {
  return tablahash_buscar(listaConjuntos, conjunto, Check);
}


/* Verifica que el nombre del conjunto es válido. */
int validar_nombre(Estado* estado) {
  return estado->alias[0][0] != '~' && estado->alias[0][0] != '{';
} 


/* Valida el input en el caso de que sea probable que se pida imprimir un conjunto. */
void validar_impresion(Estado* estado, TablaHash* conjs) {
  /* Si el comando es válido, se almacena el conjunto a imprimir en estado->alias[0]. */
  sscanf(estado->alias[2], "%s %[^\n]\n", estado->alias[1], estado->alias[0]);
  if (strcmp(estado->alias[1], "imprimir") == 0) {
    int cond = validar_conjunto(conjs, estado->alias[0]) && validar_nombre(estado);
    actualizar_estado(estado, Imprimir, ConjuntoInexistente, cond);
  } else
    estado->tipoError = ComandoNoValido;
}


/* Valida el input en el caso de que sea probable
que se pida el complemento de un conjunto. */
void validar_complemento(Estado* estado, TablaHash* conjs) {
  /* Se contempla el caso en que el comando sea válido pero el conjunto no exista. */
  int cond = validar_conjunto(conjs, &(estado->alias[2][1])) != NULL;
  actualizar_estado(estado, Complemento, ConjuntoInexistente, cond);
}


/* Verifica que los el dato dado respeta los límites
para poder pertenecer a un conjunto de enteros. */
int validar_elementos(long long dato) {
  return dato <= INT_MAX && dato >= INT_MIN;
}


/* Valida el input en el caso de que sea probable 
que se pida crear un conjunto por extensión. */
void validar_creacion_extension(Estado* estado) {
  if (strcmp(estado->alias[2], "{}") == 0) { // Si se quiere crear un conjunto vacío.
    estado->estadoInput = CrearPorExtension;
  } else {
    char llave, finalLinea;
    /* Si el comando es válido, se almacenan los números a escanear en estado->alias[2]. */
    int numEscaneos = sscanf(estado->alias[2], "{%[^}]%c%c", estado->alias[2], &llave, &finalLinea);
    if (numEscaneos == 2 && llave == '}') {
      numEscaneos = 3;
      char c = ',';
      /* El tipo de dato es long long para contemplar errores en
      donde los números ingresados no respetan los límites de int. */
      long long dato = 0;
      /* Bandera que indica si algún número escaneado no puede ser expresado como int. */
      int elemNovalido = 0;
      estado->elements = set_crear();
      Intervalo* intv = malloc(sizeof(Intervalo));
       /* Se escanean los números hasta que el comando no sea válido o se termine el conjunto. */
      while (numEscaneos == 3 && c == ',') {
        numEscaneos = sscanf(estado->alias[2], "%lld%c%s", &dato, &c, estado->alias[2]);
        if (!validar_elementos(dato))
          elemNovalido = 1;
        if (numEscaneos > 0 && !elemNovalido) {
          intv->izq = (int)dato;
          intv->der = (int)dato;
          estado->elements = set_insertar(estado->elements, intv);
        }
      }
      if ((numEscaneos > 1 && c != ',') || numEscaneos < 1) { // El comando no es válido.
        estado->tipoError = ComandoNoValido;
        set_destruir(estado->elements);
      } else if (elemNovalido) { // Algún elemento no es válido.
        estado->tipoError = ElementosNoValidos;
        set_destruir(estado->elements);
      } else if (numEscaneos == 1)
        estado->estadoInput = CrearPorExtension;
      free(intv);
    } else
      estado->tipoError = ComandoNoValido;
  }
}


/* Valida el input en el caso de que sea probable 
que se pida crear un conjunto igual a otro existente. */
void validar_igualacion_conjuntos(Estado* estado, TablaHash* conjs) {
  if (validar_conjunto(conjs, estado->alias[2])) {
    estado->elements = set_clonar(tablahash_buscar(conjs, estado->alias[2], Fetch));
    estado->estadoInput = CrearPorExtension;
  } else 
    estado->tipoError = ConjuntoInexistente;
}


/* Valida el input en el caso de que sea probable 
que se pida una operación binaria entre conjuntos. */
void validar_operaciones(Estado* estado, TablaHash* conjs) {
  char oper, buffChar;
  /* Si el comando es válido, se almacenan los conjuntos correspondientes en el Estado. */
  int numEscaneos = sscanf(estado->alias[2], "%s %c %s %c %s", 
    estado->alias[0], &buffChar, estado->alias[1], &oper, estado->alias[2]);
  /* Se verifica que la operación y los conjuntos ingresados sean válidos. */
  if (numEscaneos == 5 && buffChar == '=' && validar_nombre(estado) 
    && (oper == '-' || oper == '|' || oper == '&')) {
    /* Se contempla el caso en que el comando sea válido pero los conjuntos no existan. */
    int cond = validar_conjunto(conjs, estado->alias[1]) && validar_conjunto(conjs, estado->alias[2]);
    enum EstadoInput tipoOper = (oper == '-') ? (Resta) : ((oper == '|') ? Unir : Intersecar);
    actualizar_estado(estado, tipoOper, ConjuntoInexistente, cond);
  } else
    estado->tipoError = ComandoNoValido;
}



/* Valida el input en el caso de que sea probable 
que se pida crear un conjunto por comprensión. */
void validar_creacion_comprension(Estado* estado) {
  char c1, c2, lineSkip, emptyStr;
  long long *par = malloc(sizeof(long long)*2);
  /* Si el comando es válido, se almacenan los valores ingresados, 
  y se crea el conjunto correspondiente. */
  int numEscaneos = sscanf(estado->alias[2], "%s = {%c : %lld <= %c <= %lld}%c%c",
    estado->alias[0], &c1, &(par[0]), &c2, &(par[1]), &lineSkip, &emptyStr);
  if (numEscaneos == 6 && validar_nombre(estado) && c1 == c2
    && isalpha(c1) && validar_elementos(par[0]) && validar_elementos(par[1])) {
    estado->estadoInput = CrearPorComprension;
    if (par[0] <= par[1]) {
      Intervalo* intv = malloc(sizeof(Intervalo));
      intv->izq = (int)(par[0]);
      intv->der = (int)(par[1]);
      estado->elements = set_crear();
      estado->elements = set_insertar(estado->elements, intv);
      free(intv);
    }
  } else
    /* Se contempla el caso en que los números ingresados no entran en un int. */
    estado->tipoError = (!validar_elementos(par[0]) || !validar_elementos(par[1])) ? ElementosNoValidos : ComandoNoValido;
  free(par);
}


/* validar_input encapsula las funciones de validación anteriores y las llama 
de acuerdo a la cantidad de espacios detectados al ingresar el input. 
Lo hace teniendo en cuenta los espacios que poseen por su naturaleza los comandos:
  - 0 espacios: salir
  - 1 espacio: imprimir
  - 2 espacios: crear por extensión, complemento e igualar conjuntos
  - 4 espacios: operación binaria
  - 8 espacios: crear por extensión
  - otra cantidad: el comando no es válido */
void validar_input(TablaHash* conjs, Estado* estado) {
  if (estado->numEspacios == 0 && strcmp(estado->alias[2], "salir\n") == 0) // Salir
    estado->estadoInput = Salir;
  else if (estado->numEspacios == 1)
    validar_impresion(estado, conjs);
  else if (estado->numEspacios == 2) {
    sscanf(estado->alias[2], "%s %s %[^\n]\n", estado->alias[0], estado->alias[1], estado->alias[2]);
    if (validar_nombre(estado) && strcmp(estado->alias[1], "=") == 0) {
      if (estado->alias[2][0] == '~')
        validar_complemento(estado, conjs);
      else if (estado->alias[2][0] == '{')
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
