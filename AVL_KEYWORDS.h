// Arquivo com as assinaturas das funções relativas às operações do TAD AVL (TREE, para não conflitar com a AVL dos sites ) para as 
// palavras-chave de um site

#ifndef AVL_KEYWORDS_H_
#define AVL_KEYWORDS_H_

                #define boolean int 
                #define TRUE 1
                #define FALSE 0

                typedef struct Tree TREE;
                typedef struct No NO;

                TREE* tree_create(void);
                void tree_delete(TREE** T);
                
                boolean tree_insert(TREE* T, char* word);
                boolean tree_search(TREE* T, char* word);

	        int tree_get_size(TREE* T);
		NO* tree_getRoot(TREE* T);
		NO* get_left_tree(NO* root);
	        NO* get_right_tree(NO* root);
        	char* get_keyword(NO* root);

#endif


