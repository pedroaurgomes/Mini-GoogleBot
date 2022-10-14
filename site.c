// Arquivo .c com a implementação das funções relativas às operações do TAD SITE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "site.h"


struct site_ {

        short int code; // Código de um site (Chave primária)
        char name[51]; // nome do site
        short int relevance; // Relevância = grau de importância do site
        char link[101]; // link para acessar o  site
	TREE* keywords;
};

// Função que cria um site a partir das informações recebidas
SITE* site_create (short int code, char* name, short int relevance, char* link, TREE* keywords){

        SITE* site;

        site = (SITE*) malloc(sizeof(SITE));

        if (site != NULL){
                site->code = code;
                strncpy(site->name,name,51);
                site->relevance = relevance;
                strncpy(site->link,link,101);
                site->keywords = keywords;
        }

        return site;
}

// Função que apaga um site
boolean site_erase (SITE** site){

        if(site != NULL){
                tree_delete(&((*site)->keywords));
		free(*site);
                *site = NULL;

                return TRUE;
        }

        return FALSE;
}


// Função que retorna o código de um site
short int site_getCode (SITE* site){

        if(site != NULL)
                return site->code;
        else {
                printf("O site cujo código você procura não existe\n");
                return ERRO;
        }
}

// Função que retorna um nome de um site
char* site_getName(SITE* site){

        return site->name;
}

// Função que retorna relevância de um site
short int site_getRelevance(SITE* site){

        return site->relevance;
}

// Função que retorna o link de um site
char* site_getLink(SITE* site){

        return site->link;
}

// Função que retorna as palavras-chave relativas a um site
TREE* site_getKeywords(SITE* site){
        
	return site->keywords;
}

// Função que atualiza a relevância de um site
boolean site_updateRelevance (SITE* site, short int newRel){

        if(site != NULL) {
                site->relevance = newRel;
                return TRUE;
        }

        return FALSE;
}

