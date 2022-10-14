// Implementação das funções relativas às operações do TAD AVREL para sites (uma AVL de sites, onde a chave primária é a relevância do site)

#define max(a,b) ((a > b) ? a : b)

#include "AVREL_SITE.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct Nod {

        SITE* site; // informação de um site armazenado no nó
        NOD* right; // filho direito
        NOD* left; // filho esquerdo
        int height; // usada para calcular os fatores de balanceamento de cada nó
} NOD;

typedef struct Avrel {

        NOD* root; // raiz da árvore avrel
        int depth; // profundidade da árvore avrel
        int size; // quantidade de nós presentes na árvore AVREL
} AVREL;

// Cria e retorna uma árvore AVREL vazia
AVREL* avrel_create(void) {

        AVREL* T;

        T = (AVREL*) malloc(sizeof(AVREL));

        if(T != NULL) {
                T->root = NULL;
                T->depth = -1; // Arvore vazia tem profundidade -1
                T->size = 0;
        }

        return T;
}

// Auxiliar da função de apagar avrel em post-order
void avrel_delete_aux(NOD** root) {

        if (*root != NULL) {

                avrel_delete_aux(&((*root)->left));
                avrel_delete_aux(&((*root)->right));
                free(*root);
                *root = NULL;
        }
}

// Apaga todos os nós de uma árvore AVREL e depois a própria estrutura da árvore AVL
void avrel_delete(AVREL** T) {

        avrel_delete_aux(&((*T)->root));
        free(*T);
        *T = NULL;
}

// Retorna a altura de um determinado Nó na AVREL
int avrel_node_height(NOD* root) {

        // Árvore vazia tem altura -1
        if(root == NULL)
                return -1;
        else
                return root->height;
}

// Cria e retorna um Nó a partir de um determinado site, com altura 0 e sem filhos (esq = right = NULL)
NOD* avrel_create_node(SITE* site) {

        NOD* node;

        node = (NOD*) malloc(sizeof(NOD));

        if(node != NULL){

                node->height = 0;
                node->right = NULL;
                node->left = NULL;
                node->site = site;
        }

        return node;
}

// Executa a rotação a direita em um nó desbalanceado A (com fator de balanceamento > 1) para rebalancear a árvore
NOD* r_right(NOD* A) {

        NOD* B; // ponteiro para o filho esquerdo do nó A (nó desbalanceado)

        B = A->left;
        A->left = B->right;
        B->right = A;

        // A altura de um nó é igual à maior altura dentre seus filhos + 1
        A->height = max(avrel_node_height(A->left), avrel_node_height(A->right)) + 1;
        B->height = max(avrel_node_height(B->left), avrel_node_height(B->right)) + 1;

        return B; // retornamos a raiz da sub-árvore resultante da rotação (quem vai receber é o nó pai de A)
}

// Executa a rotação a esquerda em um nó desbalanceado A (com fator de balanceamento < -1) para rebalancear a árvore
NOD* r_left(NOD* A) {

        NOD* B; // ponteiro para o filho direito do nó A (desbalanceado)

        B = A->right;
        A->right = B->left;
        B->left = A;

        // A altura de um nó é igual à maior altura dentre seus filhos + 1
        A->height = max(avrel_node_height(A->left), avrel_node_height(A->right)) + 1;
        B->height = max(avrel_node_height(B->left), avrel_node_height(B->right)) + 1;

        return B; // retornamos a raiz da sub-árvore resultante da rotação (quem vai receber é o nó pai de A)
}

// Rotação Dupla (esquerda - direita)
NOD* r_left_right(NOD* A) {

        A->left = r_left(A->left); // Rotacionando primeiro o B (que é o filho esquerdo de A) a esquerda

        return r_right(A); // rotacionando A a direita
}

// Rotação Dupla (direita - esquerda)
NOD* r_right_left(NOD* A) {

        A->right = r_right(A->right); // Rotacionando primeiro o B (que é o filho direito de A) a direita

        return r_left(A); // Rotacionando A a esquerda
}

NOD* avrel_insert_node(NOD* root, SITE* site) {

        // Algoritmo de inserção em ABB's
        if(root == NULL)
                root = avrel_create_node(site);

        else if(site_getRelevance(site) > site_getRelevance(root->site))
                root->right = avrel_insert_node(root->right, site);

        else if(site_getRelevance(site) < site_getRelevance(root->site))
                root->left = avrel_insert_node(root->left,site);

        // Ajustando a altura dos nós
        root->height = max(avrel_node_height(root->left), avrel_node_height(root->right)) + 1;

        // Checando o balanceamento dos nós da árvore
        if(avrel_node_height(root->left) - avrel_node_height(root->right) == -2) {
                if(site_getRelevance(site) > site_getRelevance(root->right->site))
                        root = r_left(root);
                else
                        root = r_right_left(root);
	}

        if(avrel_node_height(root->left) - avrel_node_height(root->right) == 2) {
                if(site_getRelevance(site) < site_getRelevance(root->left->site))
                        root = r_right(root);
                else
                        root = r_left_right(root);
	}

        return root;
}

// Insere um dado site em uma dada árvore AVREL
boolean avrel_insert(AVREL* T, SITE* site) {

        if (T != NULL)
                T->root = avrel_insert_node(T->root, site);
        else
                return FALSE;

        if(T != NULL) {
                T->size++;
                return TRUE;
        }

        return FALSE;
}

// Retorno o nó raiz de uma árvore
NOD* avrel_getRoot(AVREL* T) {

        if (T != NULL)
                return T->root;

        return NULL;
}

// Retorna o filho esquerdo de um nó
NOD* get_left_avrel(NOD* root) {

        if (root != NULL)
                return root->left;

        return NULL;
}

// Retorna o filho direito de um nó
NOD* get_right_avrel(NOD* root) {

        if(root != NULL)
                return root->right;

        return NULL;
}

// Retorna o site armazenado em uma nó
SITE* avrel_site(NOD* root) {

        if(root != NULL)
                return root->site;

        return NULL;
}

