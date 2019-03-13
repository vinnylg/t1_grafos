#include <stdlib.h>
#include <string.h>
#include "grafo.h"

int temVizinhoComum(vertice a, vertice b){
    node va, vb;
    va = a->vizinhos; 
    vb = b->vizinhos;
    int nv = 0;
    while(va!=NULL){                      
        while(vb!=NULL){
            if(va->vertice==vb->vertice){
                printf("%s -> %s <- %s\n",a->nome,va->vertice->nome,b->nome);
                nv++;
            }
            vb = vb->next;
        }
        vb = b->vizinhos;
        va = va->next;
    }
    return nv;
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

grafo escreve_grafo(FILE *output, grafo g){
    vertice aux = g->vertices;
    while(aux){
        node n = aux->vizinhos;
        if(aux->vizinhos==NULL)
            fprintf(output,"%s\n",aux->nome);
        while(n){
            fprintf(output,"%s %s\n",aux->nome,n->vertice->nome);
            n=n->next;
        }
        aux=aux->next;
    }
    return g;
}

double coeficiente_agrupamento_grafo(grafo g){
    int triade_fechada = 0;
    int triade_aberta = 0;
    int nv;
    vertice u,v;

    u = g->vertices; //received the first vertice of graph
    while(u!=NULL){     //goes vertice by vertice 
        v = u->next;    //received the next vertice of a
        while(v!=NULL){                       //goes vertice by vertice after a to end
            if((nv = temVizinhoComum(u,v))){           
                if(ehVizinho(u,v)){
                    printf("%s - %s\n",u->nome,v->nome);
                    triade_fechada+=nv; 
                }else{
                    printf("%s x %s\n",u->nome,v->nome);
                    triade_aberta+=nv;
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
        new_line = realloc(new_line,(strlen(new_line)+1)*sizeof(char)); //realloc to size of line
        if(new_line[strlen(new_line)-1]=='\n') //remove enter character
            new_line[strlen(new_line)-1]='\0';  //putting null character
        return new_line;
    } else{
        free(new_line); //is the end of file
        return NULL;
    }
}

// fazer uma funcao de verificação para todos os caracteres não permitidos

size_t haveTwoString(char *line){
    for(size_t i=0; line[i]!='\0'; i++)
        if(line[i]==' '){
            if(line[i+1]=='\0' || line[i+1]==' '){
                line[i]='\0';
                return 0;
            }
            else
                return i;
        }
    return 0;
}

void splitStr(char *line, char **c1, char **c2){
    size_t index_space = haveTwoString(line); //return 0 if doesn't have two string
    if(index_space){
        char *str1 = (char *) malloc(1025*sizeof(char));
        char *str2 = (char *) malloc(1025*sizeof(char));
        size_t i;
        for(i=0; i<index_space; i++){
            str1[i]=line[i];
        }
        str1[i]='\0';
        str1 = realloc(str1,(i+1)*sizeof(char));
        size_t j,k;
        for(j=index_space+1, k=0; line[j]!='\0' && line[j]!=' '; j++, k++){
            str2[k]=line[j];
        }
        str2[k]='\0';
        str2 = realloc(str2,(k+1)*sizeof(char));

        free(line);

        *c1 = str1;
        *c2 = str2;
    }else{
        if(strlen(line))
            *c1 = line;
    }
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
        char *str1 = NULL, *str2 = NULL;
        splitStr(line,&str1,&str2);
        
        vertice v1,v2;
        
        if(str1){
            v1 = criaVertice(str1);
            v1 = insereVertice(newGraph,v1); //insert new vertice, return the poiter if it's already exist 
        
            if(str2){                              //if have second vertice
                v2 = criaVertice(str2);     
                v2 = insereVertice(newGraph,v2);
                
                insereVizinho(v1,criaNode(v2));     //make neighbourhood
                insereVizinho(v2,criaNode(v1));
            }
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