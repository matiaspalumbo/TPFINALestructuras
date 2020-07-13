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






