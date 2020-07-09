# TPFINALestructuras

*** Implementación de conjuntos en C ***

Se implementa una interfaz para crear y manipular conjuntos de enteros. Todos los conjuntos creados son pueden ser accedidos en memoria mediante una tabla hash, y los conjuntos en sí son representados mediante árboles de intervalos AVL.

Creación de conjuntos:
- Por extensión: ```A = {1,2,3,4}```
- Por comprensión: ```A = {x : 1 <= x <= 4}```

Operaciones entre conjuntos soportadas:
- *Unión* de dos conjuntos
- *Intersección* de dos conjuntos
- *Resta* de un conjunto con otro
- *Complemento* de un conjunto

Impresión de conjuntos:
- Los intervalos dentro de un conjunto son impresos de la forma ```a:b```, donde a y b son los extremos izquierdo y derecho, respectivamente. Los elementos del conjunto son impresos separados por ```, ```: ```-4:7, 10, 45```


COMANDOS:
salir
imprimir A
A = {1,2,3,4}
B = {x : 2 <= x <= 7}
A = A & B
A = A | B
B = B - A
A = ~A