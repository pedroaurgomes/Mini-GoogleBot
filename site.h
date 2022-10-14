// Arquivo de cabeçalho (.h) com as assinaturas das funções relativas ao TAD SITE

#ifndef SITE_H
#define SITE_H

	#include "AVL_KEYWORDS.h" 

        #define TRUE 1
        #define FALSE 0
        #define boolean int
        #define ERRO -1

        typedef struct site_ SITE;

        SITE* site_create (short int code, char* name, short int relevance, char* link, TREE* keywords);
        boolean site_erase (SITE** site);

        short int site_getCode (SITE* site);
        char* site_getName(SITE* site);
        short int site_getRelevance(SITE* site);
        char* site_getLink(SITE* site);
	TREE* site_getKeywords(SITE* site);

        boolean site_addKeyword (SITE* site, char* newword);
        boolean site_updateRelevance (SITE* site, short int newRel);

#endif

