#include <stdlib.h>
#include <string.h>
#include "grafo.h"

typedef struct node *node;

struct grafo{
    vertice vertices; // Ponteiro para o primeiro vértice do grafo
};

struct vertice{
    char *nome; // String/Nome do vértice
    vertice next; //Ponteiro para o próximo vértice
    node vizinhos;  //Ponteiro para o primeiro nó dos vizinhos
};

struct node{
    vertice vertice; //Ponteiro para o vértice vizinho
    node next;  // Ponteiro para o próximo nó de vizinho
};

// Percore a lista de vértices até o último e volta liberando a memória alocada
void freeVertices(vertice v);

// Percore os nós até achar o último e volta liberando a memória alocada
void freeNodes(node n);

// Lê uma linha do arquivo
//
//  Retorna a linha lida ou NULL caso seja o fim do arquivo
char *getLine(FILE *input);

// Verifica se a linha lida do arquivo tem 2 strings
//
//  Retorna o index do espaço que separa as strings, ou 0 se não tem duas strings
size_t haveTwoString(char *line);

// Divide e aloca cada string da linha em dois novos ponteiros
void splitStr(char *line, char **c1, char **c2);

// Pega a string e cria uma struct vertice
//
// Retorna um ponteiro para a struct criada
vertice criaVertice(char *nome);

// Insere o novo vertice na lista de vertices da struct, caso não exista
//
//  Retorna o ponteiro do vertice inserido ou existente
vertice insereVertice(grafo g, vertice v);

// Cria nó que guarda o ponteiro dos vertices vizinhos e forma uma lista encadeada simples
// Retorna o nó criado
node criaVizinho(vertice vizinho);

// Insere o nó na lista de vizinhos do vértice, caso não exista
void insereVizinho(vertice v, node n);

// Verifica se os vertices a e b tem vizinhos em comum
//
//  Retorna o número de vizinhos em comum
int temVizinhosComum(vertice a, vertice b);

// Verifica se os vertices a e b são vizinhos
//
//  Retorna 1 se são e 0 se não são
int ehVizinho(vertice a, vertice b);


// Imprime a lista de vertices do grafo, com os vizinhos, na saida padrão (sdtout)
void printGrafo(grafo g);

// Imprime os vizinhos do vertice v na saida padrão (sdtout)
void printVizinhos(vertice v);

int destroi_grafo(grafo g){
    freeVertices(g->vertices); //função recursiva que vai até o ultimo vértice e volta liberando memória
    free(g);
    return 1;
};

void freeVertices(vertice v){
    if(v!=NULL){
        if (v->next!=NULL)
            freeVertices(v->next);
        free(v->nome);
        freeNodes(v->vizinhos); //função recursiva que vai até o ultimo node e volta liberando memória
        free(v);
    }
};

void freeNodes(node n){
    if(n!=NULL){
        if (n->next!=NULL)
            freeNodes(n->next);
        free(n);
    }
};

grafo le_grafo(FILE *input){
    grafo newGraph = (grafo) malloc(sizeof(struct grafo));
    newGraph->vertices = NULL;

    char *line = getLine(input);
    while(line){ //Enquanto lê linha
        char *str1 = NULL, *str2 = NULL;
        splitStr(line,&str1,&str2); //Divide as strings

        vertice v1,v2;

        if(str1){                       //Se tiver a primeira
            v1 = criaVertice(str1);
            v1 = insereVertice(newGraph,v1);

            if(str2){                               //Se triver a segunda
                v2 = criaVertice(str2);
                v2 = insereVertice(newGraph,v2);

                insereVizinho(v1,criaVizinho(v2));    //Liga os vértices
                insereVizinho(v2,criaVizinho(v1));
            }
        }

        line = getLine(input);
    }
    //printGrafo(newGraph);
    return newGraph;
};

char *getLine(FILE *input){
    char *new_line = (char *) malloc((2050+1)*sizeof(char));  //Aloca memória para o maximo de caracteres especificados numa linha
    if ( fgets(new_line,2050,input) != NULL){ //if input line pass 2050 characteres the overflow goes to next fgets
        new_line = realloc(new_line,(strlen(new_line)+1)*sizeof(char)); //Realoca memória para o espaço utilizado
        if(new_line[strlen(new_line)-1]=='\n')  //Remove o pula linha (/n)
            new_line[strlen(new_line)-1]='\0';  //E coloca o caracter nulo
        return new_line;
    } else{
        free(new_line); //É o fim do arquivo lido, libera memoria alocada e retorna NULL
        return NULL;
    }
}

size_t haveTwoString(char *line){
    for(size_t i=0; line[i]!='\0'; i++)             //Percorre a linha até achar um caractere nulo
        if(line[i]==' '){                           //se for espaço
            if(line[i+1]=='\0' || line[i+1]==' '){  //e se o próximo caractere for nulo ou outro espaço
                line[i]='\0';                         //insere caractere nulo no primeiro espaço
                return 0;                             //e retorna 0, pois tem só uma string
            }
            else                                    //senão retorna o index do espaço
                return i;
        }
    return 0;
}

void splitStr(char *line, char **c1, char **c2){
    size_t index_space = haveTwoString(line); //Retorna zero se não tem duas strings
    if(index_space){
        char *str1 = (char *) malloc(1025*sizeof(char));  //aloca o maximo especificado de cada string
        char *str2 = (char *) malloc(1025*sizeof(char));
        size_t i;
        for(i=0; i<index_space; i++){ //percorre a linha até o espaço
            str1[i]=line[i];          //e copia para a primeira string
        }
        str1[i]='\0';  //coloca caractere nulo na primeira string
        str1 = realloc(str1,(i+1)*sizeof(char));
        size_t j,k;
        for(j=index_space+1, k=0; line[j]!='\0' && line[j]!=' '; j++, k++){ //percorre a linha depois do espaço
            str2[k]=line[j];  //e copia para a segunda string
        }
        str2[k]='\0'; //coloca caractere nulo na segunda string
        str2 = realloc(str2,(k+1)*sizeof(char));

        free(line); //libera espaço da linha

        *c1 = str1; //coloca os ponteiros das strings divididas nos endereços dos ponteiros das strings (2 returns)
        *c2 = str2; //
    }else{
        if(strlen(line)){ // verifica se não é uma string
            line = realloc(line,(strlen(line)+1)*sizeof(char));
            *c1 = line; //coloca o ponteiro da linha no endereço do ponteiro da string
        }
    }
};

vertice criaVertice(char *nome){
    vertice newVertice = (vertice) malloc(sizeof(struct vertice));
    newVertice->nome = nome;
    newVertice->vizinhos = NULL;
    newVertice->next = NULL;
    return newVertice;
};

vertice insereVertice(grafo g, vertice v){
    if(g->vertices==NULL){          //Insere o primeiro vertice da lista
        g->vertices=v;
    }
    else{
        vertice aux = g->vertices;  //Procura o fim da lista
        if(strcmp(aux->nome,v->nome)==0){ //Se já tiver o vértice na lista
            free(v->nome);                //Libera a memória usada e retorna o vertice
            free(v);
            return aux;
        }
        while(aux->next!=NULL){
            aux = aux->next;
            if(strcmp(aux->nome,v->nome)==0){ //Se já tiver o vértice na lista
                free(v->nome);                //Libera a memória usada e retorna o vertice
                free(v);
                return aux;
            }
        }                           //Se chegar no ultimo vértice da lista
        aux->next = v;              //Adiciona o novo vertice no próximo
    }
    return v; // return the new vertice
}

node criaVizinho(vertice vizinho){
    node n = (node) malloc(sizeof(struct node));
    n->vertice = vizinho;
    n->next = NULL;
    return n;
}

void insereVizinho(vertice v, node n){
    if(v->vizinhos==NULL){          //Igualmente insereVertice, porém com nodes
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

grafo escreve_grafo(FILE *output, grafo g){
    if(output==NULL){
      printf("output is NULL\n");
      return g;
    }
    vertice aux = g->vertices;
    while(aux){                             //Percorre todos os vértices do grafo
        node n = aux->vizinhos;
        if(n==NULL)                           //Se não tiver vizinhos, escreve no arquivo o vertice e pula uma linha
            fprintf(output,"%s\n",aux->nome);
        while(n){
            fprintf(output,"%s %s\n",aux->nome,n->vertice->nome); //Se tiver, escreve o vertice e o vizinho e pula uma linha
            n=n->next;                                            //Até acabar os vizinhos
        }
        aux=aux->next;
    }
    return g;
}

double coeficiente_agrupamento_grafo(grafo g){
    int triade_fechada = 0;
    int triade_aberta = 0;
    int nv;   //Numero de vizinhos
    vertice u,v;

    u = g->vertices; //Recebe o primeiro vertice do grafo e percorre até o fim
    while(u!=NULL){
        v = g->vertices;  //Recebe o primeiro vertice do grafo e percorre até o fim
        while(v!=NULL){                               //Se u e v não estão apontando para o mesmo vertice
            if(u!=v && (nv = temVizinhosComum(u,v))){ //Verifica tem vizinhos em comum
                if(ehVizinho(u,v)){                     //Verifica se são vizinhos e
                    triade_fechada+=nv;                 //Incrementa as triades_fechadas se são
                }else{                                  //ou
                    triade_aberta+=nv;                  //Incrementa as triades_abertas se não são
                }
            }
            v = v->next;                               //Aponta ao próximo vértice de v, mantendo igual u
        };
        u = u->next;                                   //Aponta ao próximo vertice de u, fazendo com que v volte no começo
    };

    if(triade_fechada+triade_aberta==0)
        return 0;

    return (double)triade_fechada/(double)(triade_aberta+triade_fechada);
}

int temVizinhosComum(vertice a, vertice b){
    node va, vb;              //Pega os ponteiros para os primeiros vizinhos do vertice a e b
    va = a->vizinhos;
    vb = b->vizinhos;
    int nv = 0;               //Numero de vizinhos em comum

    while(va!=NULL){          //Percorre as listas de vizinhos
        while(vb!=NULL){      //
            if(va->vertice==vb->vertice){ //Compara se são iguais
                nv++;       //Se sim, incrementa o número de vizinhos
            }
            vb = vb->next;  //anda na lista dos vizinhos de b
        }
                            //Quando chegar no fim
        vb = b->vizinhos;   //Retorna o ponteiro para o vizinho da lista
        va = va->next;      //Anda na lista dos vizinhos de a
    }

    return nv;
}

int ehVizinho(vertice a, vertice b){
    node aux = a->vizinhos;
    while (aux!=NULL){          //Percorre a lista de vizinhos de a
        if(aux->vertice==b){    //Se encontrar b, retorna 1
            return 1;
        }                       //Se não, continua até o fim da lista e retorna 0
        aux = aux->next;
    }
    return 0;
}

void printGrafo(grafo g){
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
