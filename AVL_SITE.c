// Arquivo .c com a implementação das funções relativas às operações do TAD AVL para sites

#define max(a,b) ((a > b) ? a : b)

#include "AVL_SITE.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {

        SITE* site; // informação de um site armazenado no nó
        NODE* right; // filho direito
        NODE* left; // filho esquerdo
        int height; // usada para calcular os fatores de balanceamento de cada nó
} NODE;

typedef struct Avl {

        NODE* root; // raiz da árvore avl
        int depth; // profundidade da árvore avl
	int size; // quantidade de nós presentes na árvore AVL
} AVL;

// Cria e retorna uma árvore AVL vazia
AVL* avl_create(void) {

        AVL* T;

        T = (AVL*) malloc(sizeof(AVL));

        if(T != NULL) {
                T->root = NULL;
                T->depth = -1; // Arvore vazia tem profundidade -1
        	T->size = 0;
	}

        return T;
}

// Auxiliar da função de apagar avl em pós-ordem
void avl_delete_aux(NODE** root) {

        if (*root != NULL) {

                avl_delete_aux(&((*root)->left));
                avl_delete_aux(&((*root)->right));
                site_erase((&(*root)->site));
		free(*root);
		*root = NULL;
        }
}

// Apaga todos os nós de uma árvore AVL e depois a própria estrutura da árvore AVL
void avl_delete(AVL** T) {

        avl_delete_aux(&((*T)->root));
        free(*T);
        *T = NULL;
}

// Retorna a altura de um determinado Nó na AVL
int avl_node_height(NODE* root) {

        // Árvore vazia tem altura -1
        if(root == NULL)
                return -1;
        else
                return root->height;
}

// Cria e retorna um Nó a partir de um determinado site, com altura 0 e sem filhos
NODE* avl_create_node(SITE* site) {

        NODE* node;

        node = (NODE*) malloc(sizeof(NODE));

        if(node != NULL){

                node->height = 0;
                node->right = NULL;
                node->left = NULL;
                node->site = site;
        }

        return node;
}

// Executa a rotação a direita em um nó desbalanceado A (com fator de balanceamento > 1) para rebalancear a árvore
NODE* rot_right(NODE* A) {

        NODE* B; // ponteiro para o filho esquerdo do nó A (nó desbalanceado)

        B = A->left;
        A->left = B->right;
        B->right = A;

        // A altura de um nó é igual à maior altura dentre seus filhos + 1
        A->height = max(avl_node_height(A->left), avl_node_height(A->right)) + 1;
        B->height = max(avl_node_height(B->left), avl_node_height(B->right)) + 1;

        return B; // retornamos a raiz da sub-árvore resultante da rotação (quem vai receber é o nó pai de A)
}

// Executa a rotação a esquerda em um nó desbalanceado A (com fator de balanceamento < -1) para rebalancear a árvore
NODE* rot_left(NODE* A) {

        NODE* B; // ponteiro para o filho direito do nó A (desbalanceado)

        B = A->right;
        A->right = B->left;
        B->left = A;

        // A altura de um nó é igual à maior altura dentre seus filhos + 1
        A->height = max(avl_node_height(A->left), avl_node_height(A->right)) + 1;
        B->height = max(avl_node_height(B->left), avl_node_height(B->right)) + 1;

        return B; // retornamos a raiz da sub-árvore resultante da rotação (quem vai receber é o nó pai de A)
}

// Rotação Dupla (esquerda - direita)
NODE* rot_left_right(NODE* A) {

        A->left = rot_left(A->left); // Rotacionando primeiro o B (que é o filho esquerdo de A) a esquerda

        return rot_right(A); // rotacionando A a direita
}

// Rotação Dupla (direita - esquerda)
NODE* rot_right_left(NODE* A) {

        A->right = rot_right(A->right); // Rotacionando primeiro o B (que é o filho direito de A) a direita

        return rot_left(A); // Rotacionando A a esquerda
}

// Inserindo em AVL ( = inserção em ABB + ajuste de altura + checagem de balanceamento (e rebalanceamento, se necessário))
// root = raiz da árvore em que desejamos inserir um nó. site = conteúdo do nó que será inserido
NODE* avl_insert_node(NODE* root, SITE* site) {

        // Algoritmo de inserção em ABB's
        if(root == NULL)
                root = avl_create_node(site);

        else if(site_getCode(site) > site_getCode(root->site))
                root->right = avl_insert_node(root->right, site);

        else if(site_getCode(site) < site_getCode(root->site))
                root->left = avl_insert_node(root->left,site);

        // Ajustando a altura dos nós
        root->height = max(avl_node_height(root->left), avl_node_height(root->right)) + 1;

        // Checando o balanceamento dos nós da árvore
        if(avl_node_height(root->left) - avl_node_height(root->right) == -2)
                if(site_getCode(site) > site_getCode(root->right->site))
                        root = rot_left(root);
                else
                        root = rot_right_left(root);

        if(avl_node_height(root->left) - avl_node_height(root->right) == 2)
                if(site_getCode(site) < site_getCode(root->left->site))
                        root = rot_right(root);
                else
                        root = rot_left_right(root);

        return root;
}

// Insere um dado site em uma dada árvore AVL
boolean avl_insert(AVL* T, SITE* site) {
	
	if (T != NULL)	
       		T->root = avl_insert_node(T->root, site);
	else 
		return FALSE;

        if(T != NULL) {
		T->size++;
                return TRUE;
	}

        return FALSE;
}

// Algortimo auxiliar para o caso da remoção em que o nó removido é de grau 2 (nesse caso será substituído pelo maior nó da sub-árvore 
// esquerda , ou menor da sub-árvore direita, e este será de fato removido)
void troca_max_esq(NODE* troca, NODE* rem, NODE* ant) {

        // Procurando o valor máximo na sub-árvore esquerda (aquele que não possui filho direito), o qual substiuirá o site removido
        if(troca->right != NULL) {

                // caminhamos com o nó que será o substituto do removido (troca), mas mantemos o nó rem, que foi encontrado na primeira 
                // busca do algotimo de remoção e deve ser removido (substituído) e seguimos atualizando o anterior ao troca            
                troca_max_esq(troca->right, rem, troca);
                troca->height = max(avl_node_height(troca->left), avl_node_height(troca->right)) + 1; // ajustes de altura
                return;
        }

        // Caso o nó removido seja a própria raiz
        if(rem == ant)
                ant->left = troca->left;
        else
                ant->right = troca->left; // recolocando o filho esquerdo do nó removido (sabemos que ele não tem filho direito, pois
                                      // ele é o maior da sub-árvore esquerda)

        site_erase(&(rem->site));
	rem->site = troca->site;
        free(troca);
        troca = NULL;
}

// Algoritmo auxiliar de remoção (procura o nó que será removido, trata cada um dos casos separadamente, ajusta as alturas e faz um 
// rebalanceamento, se necessário)
// Obs: Passagem por referência caso a raiz deva ser removida
NODE* avl_remove_aux(NODE** root, short int code) {

        NODE* p;

        if(*root == NULL)
                return NULL;

        // Encontrando o alvo da remoção
        else if(code == site_getCode((*root)->site)) {

                // Casos 1 e 2: a raiz a ser removida possui um filho ou nenhum
                if((*root)->left == NULL || (*root)->right == NULL) {

                        p = *root;

                        if((*root)->left == NULL)
                                *root = (*root)->right;
                        else
                                *root = (*root)->left;
			
			site_erase(&(p->site));
                        free(p);
                        p = NULL;
                }

                // Caso 3:  há ambos os filhos
                else
                        troca_max_esq((*root)->left, (*root), (*root));
        }

        // Continuando a busca pelo nó a ser removido na sub-árvore esquerda
        else if(code < site_getCode((*root)->site))
                (*root)->left = avl_remove_aux(&(*root)->left, code);

        // Continuando a busca pelo nó a ser removido na sub-árvore direita
        else if(code > site_getCode((*root)->site))
                (*root)->right = avl_remove_aux(&(*root)->right, code);

        // if necessário na remoção, pois assim que o nó raiz é removido ele passa a apontar para nulo, e não queremos um segfault      
        if(*root != NULL) {

                // Ajuste de altura dos nós
                (*root)->height = max(avl_node_height((*root)->left), avl_node_height((*root)->right)) + 1;
	
		// Checagem de balanceamento
                if((avl_node_height((*root)->left) - (avl_node_height((*root)->right)) == -2)) {

                        if(avl_node_height((*root)->right->left) - avl_node_height((*root)->right->right) <= 0)
                                *root = rot_left(*root);
                        else
                                *root = rot_right_left(*root);
                }

                if((avl_node_height((*root)->left) - (avl_node_height((*root)->right)) == 2)) {

                        if(avl_node_height((*root)->left->left) - avl_node_height((*root)->left->right) >= 0)
                               *root = rot_right(*root);

                        else
                                *root = rot_left_right(*root);
                }
        }

        return *root;
}

// Função presente na interface do usuário que remove determinado nó com uma determinado código em uma determinada árvore AVL
boolean avl_remove(AVL* T, short int code) {
	
	if(T != NULL)
		T->root = avl_remove_aux(&(T->root), code);
	else
		return FALSE;
	
	if(T->root != NULL) {
		T->size--;
		return TRUE;
	}

	else {
		printf("A árvore AVL não existe\n");
		return FALSE;
	}
}

// Dado um código, procura e retorna uma site na AVL que o contém.
// Obs: Versão iterativa da busca em AVL
SITE* avl_search(AVL* T, short int code){

        NODE* aux; // auxiliar para não perdermos a referência da raiz da árvore
	
	if((T != NULL) && (T->root != NULL)) {

	        aux = T->root;
	
	        while(1){
	
	                if(aux == NULL)
	                        return NULL;
	
	                if(site_getCode(aux->site) == code)
	                        return aux->site;
	
        	        if(site_getCode(aux->site) > code)
        	                aux = aux->left;
        	        else
        	                aux = aux->right;
        	}
	
	}

	return NULL;
}

// Retorno o nó raiz de uma árvore
NODE* avl_getRoot(AVL* T) {
	
	if (T != NULL)
		return T->root;
	
	return NULL;
}

// Retorna o filho esquerdo de um nó
NODE* get_left_child(NODE* root) {

	if (root != NULL)
		return root->left;
	
	return NULL;
}

// Retorna o filho direito de um nó
NODE* get_right_child(NODE* root) {

	if(root != NULL)
		return root->right;
	
	return NULL;
}

// Retorna o site armazenado em uma nó
SITE* get_site(NODE* root) {
	
	if(root != NULL)
		return root->site;

	return NULL;
}

// Retorna o tamanho (quantidade de nós) de uma AVL
int avl_get_size(AVL* T) {

        return T->size;
}

