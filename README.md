# TPFINALestructuras

*** Implementación de conjuntos en C ***

Se implementa una interfaz para crear y manipular conjuntos de enteros. Todos los conjuntos creados son pueden ser accedidos en memoria mediante una tabla hash, y los conjuntos en sí son representados mediante árboles de intervalos AVL.

Creación de conjuntos:
- Por extensión: ```A = {1, 2, 3, 4}```
- Por comprensión: ```A = {x : 1 <= x <= 4}```

Operaciones entre conjuntos soportadas:
- *Unión* de dos conjuntos
- *Intersección* de dos conjuntos
- *Resta* de un conjunto con otro
- *Complemento* de un conjunto