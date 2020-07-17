# TPFINALestructuras

**_ Implementación de conjuntos en C _**

Se implementa una interfaz para crear y manipular conjuntos de enteros. Todos los conjuntos creados son pueden ser accedidos en memoria mediante una tabla hash, y los conjuntos en sí son representados mediante árboles de intervalos AVL.

Creación de conjuntos:

- Por extensión: `A = {1,2,3,4}`
- Por comprensión: `A = {x : 1 <= x <= 4}`

Operaciones entre conjuntos soportadas:

- _Unión_ de dos conjuntos
- _Intersección_ de dos conjuntos
- _Resta_ de un conjunto con otro
- _Complemento_ de un conjunto

OBS: Los nombres de conjuntos no pueden comenzar con los caracteres '{' ni '~'.

Impresión de conjuntos:

- Los intervalos dentro de un conjunto son impresos de la forma `a:b`, donde a y b son los extremos izquierdo y derecho, respectivamente. Los elementos del conjunto son impresos separados por `,`: `-4:7, 10, 45`

COMANDOS:

- `salir`
- `imprimir A`
- `A = {1,2,3,4}`
- `B = {x : 2 <= x <= 7}`.
- `A = A & B`
- `A = A | B`
- `B = B - A`
- `A = ~A`

<!-- PROCESO DE VALIDACIÓN DE INPUT:


1. chequear si quiere salir
2. `sscanf(input, "%s %s", buff, input);`
3. chequear si quiere imprimir
4. `sscanf(input, " = %c%s", char1, input);`
5. chequear:
      1. Si quiere complemento (`char1 == '~'`). En ese caso, `input` debe estar almacenado en memoria.
      2. Si quiere crear un conjunto (`char1 == '{'`). En ese caso, 
         `sscanf(input, "%c%s", char3, input)`. Si `char3` es una letra, se quiere definir por comprensión.
         En ese caso: `sscanf(input, " : %d <= %c <= %d%c", izq, char4, der, rightBrace)`; (chequear que `char3 == char4` y `rightBrace == '}'`).
         Caso contrario, iterar el sscanf chequeando las comas y llaves para ver cuando terminar el conjunto.
      3. Si no quiere ninguna de las dos, GUARDAR `char1` (porque es el primer char de `alias1`)
6. `sscanf(input, "%s %c %s", alias1, char2, alias2);`
7. Para que el comando sea valido:
      1. `strcat(char1, alias1)` y `alias2` deben estar almacenados en memoria
      2. `char2` debe ser `'|'`, `'&'` o `'-'` -->

PROCESO DE VALIDACIÓN DE INPUT (ACTUALIZADO):

1. `int a = sscanf(input, "%s %s %s", s1, s2, s3);`
2. si `a == 1` y `s1 == "salir"`, salir
3. si `a == 2` y `s1 == "imprimir"` y `s2 existe`, imprimir
4. si `a == 3` y `s2 == "="`:
    1. si `s3[0] == '~'` y `s3[1:] existe`, complemento
    2. si `s3[0] == '{'` y `s3[1]` es numérico, hacer `sscanf(s3, "{%s", s3); while (s3 no termina) sscanf(input, "%d,", agregar a set?)`
    3. si `s3[0]=='{'` y `s3[1]` es una letra, hacer `sscanf(s3, "{%s", s3); sscanf(s3, "%c : %d <= %x <= %d}", ...)`
    4. en otro caso, `sscanf(s3, "%s %c %s", alias2, operacion, alias3)` y ver que `operacion` es validod y que `alias2` y `alias3` existen.



    \-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-\-

**_NUEVA IDEA PARA SETS: ARBOLES AVL**

- Un set va a estar compuesto de un árbol AVL ordenado según SOLO los extremos izquierdos
- En el intérprete, cada vez que se inserta un elemento se fija si puede agregarse a un intervalo existente
- Entonces, en todos los sets creados por extensión y comprensión, los nodos del árbol son disjuntos dos a dos.

(La función insertar del AVL COPIA los datos pasados en el intervalo)

En el interprete: 
Se crea un intervalo buffer.
si es por comprensión, se escanean los dos números, y si no es vacío se inserta.
si es por extensión, se copian iterativamente los números en ambos datos del intv buffer, y se inserta en el árbol.









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




------------

      // switch (estado->estadoInput) {
      //   case Imprimir:
      //     itree_imprimir(tablahash_buscar(listaConjuntos, estado->alias[1], Fetch));
      //     puts("");
      //     // puts("Imprimir.");
      //   break;
      //   case CrearPorExtension:
      //     // puts("CrearPorExtension.");
      //     insertar_conjunto(listaConjuntos, estado);
      //     puts("Conjunto creado.");
      //   break;
      //   case CrearPorComprension:
      //     // puts("CrearPorComprension.")
      //     insertar_conjunto(listaConjuntos, estado);
      //     puts("Conjunto creado.");
      //   break;
      //   case Unir:
      //     estado->elements = itree_union(tablahash_buscar(listaConjuntos, estado->alias[1], Fetch), 
      //                                    tablahash_buscar(listaConjuntos, estado->alias[2], Fetch));
      //     insertar_conjunto(listaConjuntos, estado);
      //     // puts("Unir.");
      //   break;
      //   case Intersecar:
      //     estado->elements = itree_interseccion(tablahash_buscar(listaConjuntos, estado->alias[1], Fetch),
      //                                           tablahash_buscar(listaConjuntos, estado->alias[2], Fetch));
      //     insertar_conjunto(listaConjuntos, estado);
      //     // puts("Intersecar.");
      //   break;
      //   case Resta: {
      //     estado->elements = itree_resta(tablahash_buscar(listaConjuntos, estado->alias[1], Fetch),
      //                                    tablahash_buscar(listaConjuntos, estado->alias[2], Fetch));
      //     insertar_conjunto(listaConjuntos, estado);
      //     // puts("Resta.");
      //   }
      //   break;
      //   case Complemento:
      //     estado->elements = itree_complemento(tablahash_buscar(listaConjuntos, &(estado->alias[2][1]), Fetch));
      //     insertar_conjunto(listaConjuntos, estado);
      //     // puts("Complemento.");
      //   break;
      //   default: return;
      // }




      // printf("0: %s\n1: %s\n2: %s\n", estado->alias[0], estado->alias[1], estado->alias[2]);