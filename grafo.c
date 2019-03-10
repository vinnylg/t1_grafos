#include <stdlib.h>
#include <string.h>
#include "grafo.h"

char *getLine(FILE *input){
    int max = 2050; //two strings with 1024 characters and space
    char *new_line = (char *) malloc((2050+1)*sizeof(char));
    if ( fgets(new_line,max,input) != NULL){ //if input line pass 2050 characteres the overflow goes to next fgets
        new_line = realloc(new_line,strlen(new_line)*sizeof(char)); //realloc to size of line
        if(new_line[strlen(new_line)-1]=='\n'){ //remove enter character
            new_line[strlen(new_line)-1]='\0';  //putting null character
        }
        // if(strlen(new_line)==0){ //if line is empty
        //     free(new_line);
        //     new_line = NULL;
        // };
        return new_line;
    } else{
        free(new_line); //is the end of file
        return NULL;
    }
}

char *splitStr(char **str){
    char *aux = *str;
    for(size_t i=0; i < strlen(aux); i++){
        if(aux[i]==' '){           //if space exists then have two vertice, making an edge
            aux[i]='\0';           //first string end in space 
            return &aux[i+1];      //return the second string that start after the space
        };
    };
    return NULL;                    //if ran all string and not found a space
};


vertice insereVertice(grafo g, vertice v){
    if(g->vertices==NULL){          //if there isn't any vertice
        g->vertices=v;              
    }
    else{
        vertice aux = g->vertices;  //else find the end of list
        if(strcmp(aux->nome,v->nome)==0){ // if already have the vertice v
            free(v);
            return aux; //return pointer to vertice v in graph
        }
        while(aux->next!=NULL){
            aux = aux->next;
            if(strcmp(aux->nome,v->nome)==0){ // if already have the vertice v
                free(v);
                return aux; //return pointer to vertice v in graph
            }
        }                           //and make the last vertice of list poiter to new
        aux->next = v;
    }
    return v; // return the new vertice
}

void insereVizinho(vertice v, node n){
    if(v->vizinhos==NULL){          //the same thing that insertVertice, but with node
        v->vizinhos=n;
    }
    else{
        node aux = v->vizinhos;
        if(aux->vertice==n->vertice){
            free(n);
            return;
        }
        while(aux->next!=NULL){
            aux = aux->next;
            if(aux->vertice==n->vertice){
                free(n);
                return;
            }
        }
        aux->next = n;
    };
}

node criaNode(vertice vizinho){  // i don't need explain this shit
    node n = (node) malloc(sizeof(struct node));
    n->vertice = vizinho;
    n->next = NULL;
    return n;    
}


void printVertices(grafo g){     //neither this
    vertice aux = g->vertices;
    while(aux!=NULL){
        printf("%s: ",aux->nome);
        printVizinhos(aux);
        aux=aux->next;
    }
}

void printVizinhos(vertice v){
    node aux = v->vizinhos;
    while(aux!=NULL){
        printf("%s%c",aux->vertice->nome,aux->next?'-':' '); //to print the neighbour side by side with hifen
        aux=aux->next;
    }
    printf("\n");
}


grafo le_grafo(FILE *input){
    grafo newGraph = (grafo) malloc(sizeof(struct grafo));
    newGraph->vertices = NULL;
    newGraph->nVertices = 0;

    char *str = getLine(input);
    while(str){
        char *str2 = splitStr(&str);

        vertice v1 = criaVertice(str);
        v1 = insereVertice(newGraph,v1);

        if(str2){
            vertice v2 = criaVertice(str2);
            v2 = insereVertice(newGraph,v2);
            
            insereVizinho(v1,criaNode(v2));
            insereVizinho(v2,criaNode(v1));
        }
        
        str = getLine(input);
    }
    printVertices(newGraph);
    return NULL;
} 

vertice criaVertice(char *nome){
    vertice newVertice = (vertice) malloc(sizeof(struct vertice));
    newVertice->nome = nome;
    newVertice->vizinhos = NULL;
    newVertice->next = NULL;
    return newVertice;
};