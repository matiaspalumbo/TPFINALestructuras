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

PROCESO DE VALIDACIÓN DE INPUT:


1. chequear si quiere salir
2. sscanf(input, "%s %s", buff, input);
3. chequear si quiere imprimir
4. sscanf(input, " = %c%s", char1, input);
5. chequear:
      1. Si quiere complemento (char1 == '~'). En ese caso, input debe estar almacenado en memoria.
      2. Si quiere crear un conjunto (char1 == '{'). En ese caso, 
         sscanf(input, "%c%s", char3, input). Si char3 es una letra, se quiere definir por comprensión.
         En ese caso: sscanf(input, " : %d <= %c <= %d%c", izq, char4, der, rightBrace); (chequear que char3 == char4 y rightBrace == '}').
         Caso contrario, iterar el sscanf chequeando las comas y llaves para ver cuando terminar el conjunto.
      3. Si no quiere ninguna de las dos, GUARDAR el char (porque es el primer char de alias1)
6. sscanf(input, "%s %c %s", alias1, char2, alias2);
7. Para que el comando sea valido:
      1. strcat(char1, alias1) y alias2 deben estar almacenados en memoria
      2. char2 debe ser '|', '&' o '-'
