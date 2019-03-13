#include <stdio.h>
#include "grafo.h"

//------------------------------------------------------------------------------

int main(void) {
  grafo g = le_grafo(stdin);

  if ( !g )
    return 1;

  printf ("O coeficiente de agrupamento do grafo é %f\n", coeficiente_agrupamento_grafo(g));

  FILE *output = fopen("saida","w");
  escreve_grafo(output,g);

  return ! destroi_grafo(g);
}
