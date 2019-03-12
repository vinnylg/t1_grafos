#include <stdlib.h>
#include <string.h>
#include "grafo.h"

int temVizinhoComum(vertice a, vertice b){
    node va, vb;
    va = a->vizinhos; 
    vb = b->vizinhos;
    while(va!=NULL){                      
        while(vb!=NULL){
            if(va->vertice==vb->vertice){
                return 1;
            }
            vb = vb->next;
        }
        vb = b->vizinhos;
        va = va->next;
    }
    return 0;
}

int ehVizinho(vertice a, vertice b){
    node aux = a->vizinhos;
    while (aux!=NULL){
        if(aux->vertice==b){
            return 1;
        }
        aux = aux->next;
    }
    return 0;
}

double coeficiente_agrupamento_grafo(grafo g){
    int triade_fechada = 0;
    int triade_aberta = 0;

    vertice u,v;

    u = g->vertices; //received the first vertice of graph
    while(u!=NULL){     //goes vertice by vertice 
        v = u->next;    //received the next vertice of a
        while(v!=NULL){                       //goes vertice by vertice after a to end
            if(temVizinhoComum(u,v)){           
                if(ehVizinho(u,v)){
                    triade_fechada++; 
                }else{
                    triade_aberta++;
                }
            }
            v = v->next;                       
        };
        u = u->next;
    };

    printf("triades_fechada:%d triades_aberta:%d\n",triade_fechada,triade_aberta);

    if(triade_fechada+triade_aberta==0)
        return 0;
    
    return (double)triade_fechada/(double)(triade_aberta+triade_fechada); 
}

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

int *splitStr(char *line, char **str1, char **str2){
    for(size_t i=0; i < strlen(line); i++){
        if(line[i]==' '){
            *str1 = (char *) malloc((i+1)*sizeof(char));
            *str1[i]='\0';  
            for(size_t j = i-1; j >=0; j--)
                *str1[j]=line[j];

            *str2 = (char *) malloc((strlen(line)-i)*sizeof(char));
            i++;
            for(size_t k = 0; k < (strlen(line)-i-1) ; k++)
                *str2[k]=line[i+k];
            
            free(line);

            return 2;
        }
    };
    return 1;
};


vertice insereVertice(grafo g, vertice v){
    if(g->vertices==NULL){          //if there isn't any vertice
        g->vertices=v;              
    }
    else{
        vertice aux = g->vertices;  //else find the end of list
        if(strcmp(aux->nome,v->nome)==0){ // if already have the vertice v
            free(v->nome);
            free(v);
            return aux; //return pointer to vertice v in graph
        }
        while(aux->next!=NULL){
            aux = aux->next;
            if(strcmp(aux->nome,v->nome)==0){ // if already have the vertice v
                free(v->nome);
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


void printGrafo(grafo g){     //neither this
    vertice aux = g->vertices;
    while(aux!=NULL){
        printf("%s: ",aux->nome);
        printVizinhos(aux);
        aux=aux->next;
    }
}

void printVertices(vertice v){     //neither this
    vertice aux = v;
    while(aux!=NULL){
        printf("%s ",aux->nome);
        aux=aux->next;
    }
    printf("\n");
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

    char *line = getLine(input);
    while(line){ //while have str
        char *str1 = NULL, str2 = NULL;
        splitStr(line,&str1,&str2); //trynna split if it have a space, or return null
        
        vertice v1 = criaVertice(str1);
        v1 = insereVertice(newGraph,v1); //insert new vertice, return the poiter if it's already exist 

        if(str2){                              //if have second vertice
            vertice v2 = criaVertice(str2);     
            v2 = insereVertice(newGraph,v2);
            
            insereVizinho(v1,criaNode(v2));     //make neighbourhood
            insereVizinho(v2,criaNode(v1));
        }
        
        line = getLine(input);
    }
    printGrafo(newGraph);
    return newGraph;
}; 

vertice criaVertice(char *nome){
    vertice newVertice = (vertice) malloc(sizeof(struct vertice));
    newVertice->nome = nome;
    newVertice->vizinhos = NULL;
    newVertice->next = NULL;
    return newVertice;
};


void freeNodes(node n){
    if(n!=NULL){
        if (n->next!=NULL)
            freeNodes(n->next);        
        free(n);
    }
};

void freeVertices(vertice v){
    if(v!=NULL){
        if (v->next!=NULL)
            freeVertices(v->next);  
        free(v->nome);
        freeNodes(v->vizinhos);
        free(v);
    }
};


int destroi_grafo(grafo g){
    freeVertices(g->vertices);
    free(g);
    return 1;
};