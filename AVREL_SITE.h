// Interface de usuário com as assinaturas das funções relativas às operações do TAD AVREL para sites (uma AVL de sites, onde a chave 
// primária é a relevância do site)

#ifndef AVREL_SITE_H_
#define AVREL_SITE_H_

        #include "site.h"

        #define boolean int
        #define TRUE 1
        #define FALSE 0

        typedef struct Avrel AVREL;
        typedef struct Nod NOD;

        AVREL* avrel_create(void);
        void avrel_delete(AVREL** T);

        boolean avrel_insert(AVREL* T, SITE* site);
        
	NOD* avrel_getRoot(AVREL* T);
        NOD* get_left_avrel(NOD* root);
        NOD* get_right_avrel(NOD* root);
        
	SITE* avrel_site(NOD* root);

#endif


