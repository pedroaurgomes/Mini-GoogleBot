// Arquivo com as implementações das funções relativas às operações do TAD AVL para as palavras-chave de um site

#include"AVL_KEYWORDS.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define max(a,b) ((a > b) ? a : b)

typedef struct No {

        char* word; // palavra chave armazenada em um nó
        NO* right; // filho direito
        NO* left; // filho esquerdo
        int height; // usada para calcular os fatores de balanceamento de cada nó

} NO;

typedef struct Tree {

        NO* root; // raiz da árvore avl
        int depth; // profundidade da árvore avl
	int size; // quantidade de nós na árvore AVL
} TREE;


// Cria e retorna uma árvore AVL vazia
TREE* tree_create(void) {

        TREE* T;

        T = (TREE*) malloc(sizeof(TREE));

        if(T != NULL) {
                T->root = NULL;
                T->depth = -1; // Arvore vazia tem profundidade -1
        	T->size = 0;
	}

        return T;
}

// Auxiliar da função de apagar avl em post-order 
void tree_delete_aux(NO** root) {

        if (*root != NULL) {

               	tree_delete_aux(&((*root)->left));
                tree_delete_aux(&((*root)->right));
                free((*root)->word);
		free(*root);
		*root = NULL;
        }
}

// Apaga todos os nós de uma árvore AVL e depois a própria estrutura da árvore AVL
void tree_delete(TREE** T) {

        tree_delete_aux(&((*T)->root));
        free(*T);
        *T = NULL;
}

// Retorna a altura de um determinado Nó na AVL
int tree_node_height(NO* root) {

        // Árvore vazia tem altura -1
        if(root == NULL)
                return -1;
        else
                return root->height;
}

// Cria e retorna um Nó a partir de uma determinada palavra chave, com altura 0 e sem filhos (esq = right = NULL)
NO* tree_create_node(char* word) {

        NO* node;

        node = (NO*) malloc(sizeof(NO));

        if(node != NULL){

                node->height = 0;
                node->right = NULL;
                node->left = NULL;
		node->word = (char*) malloc(strlen(word) + 1);
                strcpy(node->word,word);
        }

        return node;
}

// Retorna o tamanho (quantidade de nós) de uma AVL (TREE)
int tree_get_size(TREE* T) {

	return T->size;
}

// Executa a rotação a direita em um nó desbalanceado A (com fator de balanceamento > 1) para rebalancear a árvore
NO* rotate_right(NO* A) {

        NO* B; // ponteiro para o filho esquerdo do nó A (nó desbalanceado)

        B = A->left;
        A->left = B->right;
        B->right = A;

        // A altura de um nó é igual à maior altura dentre seus filhos + 1
        A->height = max(tree_node_height(A->left), tree_node_height(A->right)) + 1;
        B->height = max(tree_node_height(B->left), tree_node_height(B->right)) + 1;

        return B; // retornamos a raiz da sub-árvore resultante da rotação (quem vai receber é o nó pai de A)
}

// Executa a rotação a esquerda em um nó desbalanceado A (com fator de balanceamento < -1) para rebalancear a árvore
NO* rotate_left(NO* A) {

        NO* B; // ponteiro para o filho direito do nó A (desbalanceado)

        B = A->right;
        A->right = B->left;
        B->left = A;

        // A altura de um nó é igual à maior altura dentre seus filhos + 1
        A->height = max(tree_node_height(A->left), tree_node_height(A->right)) + 1;
        B->height = max(tree_node_height(B->left), tree_node_height(B->right)) + 1;

        return B; // retornamos a raiz da sub-árvore resultante da rotação (quem vai receber é o nó pai de A)
}

// Rotação Dupla (esquerda - direita)
NO* rotate_left_right(NO* A) {

        A->left = rotate_left(A->left); // Rotacionando primeiro o B (que é o filho esquerdo de A) a esquerda

        return rotate_right(A); // rotacionando A a direita
}

// Rotação Dupla (direita - esquerda)
NO* rotate_right_left(NO* A) {

        A->right = rotate_right(A->right); // Rotacionando primeiro o B (que é o filho direito de A) a direita

        return rotate_left(A); // Rotacionando A a esquerda
}

// Função auxiliar de inserção
NO* tree_insert_node(NO* root, char* word) {

        // Algoritmo de inserção em ABB's
        if(root == NULL)
                root = tree_create_node(word);

        else if(strcmp (word,root->word) > 0)
                root->right = tree_insert_node(root->right, word);

        else if(strcmp (word,root->word) < 0)
                root->left = tree_insert_node(root->left,word);

        // Ajustando a altura dos nós
        root->height = max(tree_node_height(root->left), tree_node_height(root->right)) + 1;

        // Checando o balanceamento dos nós da árvore
        if(tree_node_height(root->left) - tree_node_height(root->right) == -2)
                if(strcmp(word,root->right->word) > 0)
                        root = rotate_left(root);
                else
                        root = rotate_right_left(root);

        if(tree_node_height(root->left) - tree_node_height(root->right) == 2)
                if(strcmp(word,root->left->word) < 0)
                        root = rotate_right(root);
                else
                        root = rotate_left_right(root);

        return root;
}

// Insere uma nova palavra chave na árvore AVL (TREE)
boolean tree_insert(TREE* T, char* word) {
	
	T->root = tree_insert_node(T->root, word);

        if(T != NULL) {
         	T->size++;
	 	return TRUE;
	}
        return FALSE;
}

boolean tree_search(TREE* T, char* word) {
	
	NO* aux; // auxiliar para não perdermos a referência da raiz da árvore

        aux = T->root;

        while(1){

                if(aux == NULL)
                        return FALSE;

                if(strcmp(aux->word,word) == 0)
                        return TRUE;

                if( strcmp(aux->word,word) > 0) 
                        aux = aux->left;
                else
                        aux = aux->right;
        }
}

NO* tree_getRoot(TREE* T){
	
	if(T != NULL)
		return T->root;

	return NULL;
}

NO* get_left_tree(NO* root) {
	
	if (root != NULL)
                return root->left;

        return NULL;
}

NO* get_right_tree(NO* root) {

	if (root != NULL)
                return root->right;

        return NULL;
}

char* get_keyword(NO* root) {

	if(root != NULL)
        	return root->word;

        return NULL;
}

