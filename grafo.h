#ifndef _GRAFO_H
#define _GRAFO_H

#include <stdio.h>

//------------------------------------------------------------------------------
// (apontador para) estrutura de dados para representar um grafo
// 
// o grafo tem um nome, que é uma "string"

typedef struct grafo *grafo;
typedef struct node *node;
typedef struct vertice *vertice;

struct grafo{
    vertice vertices;
};

//------------------------------------------------------------------------------
// (apontador para) estrutura de dados para representar um vértice
// 
// o vértice tem um nome, que é uma "string"

struct vertice{
    char *nome;
    vertice next;
    node vizinhos;
};

struct node{
    vertice vertice;
    node next;
};

char *getLine(FILE *input);

int *splitStr(char *line, char **str1, char **str2);

vertice criaVertice(char *nome);

vertice insereVertice(grafo g, vertice v);

void printGrafo(grafo g);

void printVertices(vertice v);

void printVizinhos(vertice v);

node criaNode(vertice vizinho);

void insereVizinho(vertice v, node n);

void freeNodes(node n);

void freeVertices(vertice v);


//------------------------------------------------------------------------------
// desaloca toda a memória usada em *g
// 
// devolve 1 em caso de sucesso,
//         ou 
//         0, caso contrário


int destroi_grafo(grafo g);

//------------------------------------------------------------------------------
// lê um grafo de input.
// um grafo é um arquivo onde cada linha tem zero, uma ou duas
// strings (sequência de caracteres sem "whitespace").
// 
// devolve o grafo lido. Caso o arquivo esteja mal formado o
// comportamento da função é indefinido

grafo le_grafo(FILE *input);  

//------------------------------------------------------------------------------
// escreve o grafo g em output, no mesmo formato que o usado por le_grafo()
//
// devolve o grafo escrito,
//         ou 
//         NULL, em caso de erro 

grafo escreve_grafo(FILE *output, grafo g);

//------------------------------------------------------------------------------
// devolve o coeficiente de agrupamento de g
// ou seja, o número de tríades fechadas divido pelo 
// total de tríades (ou seja, a soma de tríades abertas e fechadas).
// 

double coeficiente_agrupamento_grafo(grafo g);

int temVizinhoComum(vertice a, vertice b);

int ehVizinho(vertice a, vertice b);

//------------------------------------------------------------------------------
#endif
