// Arquivo de cabeçalho (.h) com as assinaturas das funções relativas ao TAD árvore AVL para sites

#ifndef AVL_SITE_H_
#define AVL_SITE_H_

	#include "site.h"

	#define boolean int 
        #define TRUE 1
        #define FALSE 0

	typedef struct Avl AVL;
        typedef struct Node NODE;

	AVL* avl_create(void);
        void avl_delete(AVL** T);

        boolean avl_insert(AVL* T, SITE* site);
        boolean avl_remove(AVL* T, short int code);
        SITE* avl_search(AVL* T, short int code);
       
       	NODE* avl_getRoot(AVL* T);
	NODE* get_left_child(NODE* root);
	NODE* get_right_child(NODE* root);
	SITE* get_site(NODE* root);
	int avl_get_size(AVL* T);
	
#endif 
