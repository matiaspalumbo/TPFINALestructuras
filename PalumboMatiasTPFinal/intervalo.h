#ifndef __INTV_H__
#define __INTV_H__


/* Archivo de cabecera de la implementación de intervalos enteros. */

typedef struct {
  int izq; // Extremo izquierdo.
  int der; // Extremo derecho.
} Intervalo;


/* Crea un intervalo. */
Intervalo* intv_crear();

/* Destruye el intervalo dado. */
void intv_destruir(Intervalo* intv);

/* Compara los intervalos dados. Devuelve 0 si intvA está contenido en intvB.
Caso contrario, los compara de acuerdo al orden lexicográfico. */
long intv_comparar(Intervalo *intvA, Intervalo *intvB);

/* Verifica si hay intersección entre los intervalos dados. */
int intv_hay_interseccion(Intervalo* intv1, Intervalo* intv2);

/* Imprime un intervalo. Si los extremos son iguales, imprime 
solo un extremo, y si no, imprime ambos separados por ":". */
void intv_imprimir(void* intv);


#endif  /* __INTV_H__ */

