#include <stdlib.h>
#include <string.h>
#include "grafo.h"

char *getLine(FILE *input){
    int max = 2050; //two strings with 1024 characters and space
    char *new_line = (char *) malloc((2050+1)*sizeof(char));
    if ( fgets(new_line,max,input) != NULL){ //if input line pass 2050 characteres the overflow goes to next fgets
        new_line = realloc(new_line,strlen(new_line)*sizeof(char));
        return new_line;
    } else{
        free(new_line);
        return NULL;
    }
}

void getStrings(char *line, char **str1, char **str2){
    char *aux = strtok(line," ");
    *str1 = (char *) malloc((strlen(aux)+1)*sizeof(char));
    strncpy(*str1,aux,strlen(aux));
    printf("str1-%s\n",*str1);
    aux = strtok(NULL," ");
    *str2 = (char *) malloc((strlen(aux)+1)*sizeof(char));
    strncpy(*str2,aux,strlen(aux)+1);
    printf("str2-%s\n",*str2);
}

grafo le_grafo(FILE *input){
    int end_of_file = 0;
    while(!end_of_file){
        char *line = getLine(input);
        if (line){
            char *str1 = NULL, *str2 = NULL;
            getStrings(line,&str1,&str2);

            vertice v1,v2;

            if(str1){
                v1 = criaVertice(str1);
                printf("v1->nome:%s\n",v1->nome);
            }
            if(str2){
                v2 = criaVertice(str2);
                printf("v2->nome:%s\n",v2->nome);
            }

            //if(v1 && v2)
              //  makeNeighbour(v1,v2);
        }else
            end_of_file = 1;
    }
    return NULL;
} 

vertice criaVertice(char *nome){
    vertice newVertice = (vertice) malloc(sizeof(struct vertice));
    newVertice->nome = nome;
    newVertice->vizinhos = NULL;
    return newVertice;
};