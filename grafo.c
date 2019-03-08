#include <stdlib.h>
#include <string.h>
#include "grafo.h"

char *getLine(FILE *input, int *end){
    size_t max = 10;
    char *new_line = (char *) malloc((max+1)*sizeof(char));
    size_t i = 0;
    char c = getc(input);
    while(c != '\n' && c != EOF){
        if(i >= max-1){
            max+=10;
            new_line=realloc(new_line,(max+1)*sizeof(char));
        }
        new_line[i++]=c;
        c = getc(input);
    }
    new_line[i]='\0';
    
    if(c == EOF)
        *end = 1;

    return new_line;
}

grafo le_grafo(FILE *input){
    int end_of_file = 0;
    int count = 1;
    while(!end_of_file){
        printf("linha:%d -->%s\n",count++,getLine(input,&end_of_file));
    }
    return NULL;
} 

vertice criaVertice(char *nome){
    vertice newVertice = (vertice) malloc(sizeof(struct vertice));
    newVertice->nome = nome;
    newVertice->vizinhos = NULL;
    return newVertice;
};