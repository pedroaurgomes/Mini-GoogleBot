// Implementação das funções das novas funcionalidades do googlebot

#include "newFun.h"

// Algoritmo com percurso em-ordem para verificar a presença da palavra-chave em cada site da AVL e armazenar esses sites em uma 
// fila de prioridades
void avl_walkcheck(AVL* sites, NODE* root, HEAP* priorlist, char* word) {

	SITE* auxsite; // site auxiliar para verificação da presença de palavra-chave

	if(root != NULL) {

        	// Sub-árvore esquerda
        	avl_walkcheck(sites, get_left_child(root), priorlist, word);

        	// Visita
		auxsite = get_site(root);

		// Procurando a palavra-chave na TREE que contém as palavras chave do site (aux)
		if(tree_search(site_getKeywords(auxsite), word))
			heap_insert(priorlist,auxsite);

		auxsite = NULL;

        	// Sub-árvore direita
        	avl_walkcheck(sites, get_right_child(root), priorlist, word);
	}
}

// Função que inspeciona todos os sites presentes em uma AVL para verificar quais deles possuem a palavra-chave 'word', e retorna uma fila
// de prioridades com esses sites, na qual a prioridade é medida pela relevância do site
HEAP* inspect_sites(AVL* sites, char* word) {

	HEAP* priorlist; // Fila de prioridades com os sites que possuírem a a palavra chave em questão
	
	// Criando heap com  tamanho máximo = tamanho da AVL que contém os sites
	priorlist = heap_create(avl_get_size(sites));

	// Caminhado pelos sites na AVL, coletando aqueles que possuírem a palavra chave
	avl_walkcheck(sites, avl_getRoot(sites), priorlist, word);

	return priorlist;
}

// Armazena as palaras-chave de um site que possuir a 'word'  em uma TREE kwords
void collect_keywords(TREE* keywords, NO* root, TREE* kwords) {

        if(root != NULL) {

                // Sub-árvore esquerda
                collect_keywords(keywords, get_left_tree(root), kwords);

	        // Visita
		tree_insert(kwords,get_keyword(root));	

                // Sub-árvore direita
                collect_keywords(keywords, get_right_tree(root), kwords);
        }
}

// Coleta todas as palavras-chave de todos os sites que possuem a 'word'
void avl_walkcollect(AVL* sites, NODE* root, char* word, TREE* kwords) {

        TREE* auxkeywords = NULL; // TREE auxiliar para armazenar temporariamente cada TREE dos sites selecionados
        
	if(root != NULL) {
                
                // Sub-árvore esquerda
                avl_walkcollect(sites,get_left_child(root), word, kwords);
               
	        // Visita	
                auxkeywords = site_getKeywords(get_site(root));

                if(tree_search(auxkeywords, word)) {
                        collect_keywords(auxkeywords, tree_getRoot(auxkeywords), kwords);	
		}
		
                // Sub-árvore direita
                avl_walkcollect(sites,get_right_child(root), word, kwords);
        }
}

// Percorre a AVL sites, armazenando em um estrutura auxiliar (suggested) aqueles que possuírem a palavra-chave 'word'
void avl_walksuggest(AVL* sites, NODE* root, char* word, AVREL* suggested) {

        SITE* auxsite; // site auxiliar para verificação da presença de palavra-chave

        if(root != NULL) {

                // Sub-árvore esquerda
                avl_walksuggest(sites, get_left_child(root), word,suggested);

        	// Visita
                auxsite = get_site(root);

                // Procurando a palavra-chave na TREE que contém as palavras-chave de auxsite
                if(tree_search(site_getKeywords(auxsite), word))
			avrel_insert(suggested,auxsite);

               	// Sub-árvore direita
                avl_walksuggest(sites, get_right_child(root), word, suggested);
        }
}

// Percorre a TREE kwords, que tem  as palavras-chave coletadas e, para cada palavra-chave nela, verifica quais sites a possui e 
// armazena esses sites na AVREL suggested
void tree_walk(TREE* kwords, NO* root, AVL* sites, AVREL* suggested) {
	
	if (root != NULL) {
	
		// Sub-árvore esquerda
		tree_walk(kwords, get_left_tree(root),sites,suggested);

		// Visita
		avl_walksuggest(sites,avl_getRoot(sites),get_keyword(root),suggested);
	
		// Sub-árvore direita
		tree_walk(kwords, get_right_tree(root),sites,suggested);
	}
}

// Retorna uma AVREL (AVL com chave primária sendo a relevância do site), que contém os sites sugeridos, os quais foram selecionados por
// possuírem uma palavra-chave em comum com os sites que possuírem a palavra-chave dada 'word'
AVREL* site_suggestion(AVL* sites, char* word) {

        TREE* kwords = NULL; // Matriz (listagem) com todas as palavras chave dos sites selecionados
        int nkwords = 0; // Número de palavras coletadas
        AVREL* suggested = NULL; // Estrutura árvore AVL (onde a chave é a relevância de cada chave) que armazena os sites que possuem as 
				// palavras-chave coletadas
	int i;

	kwords = tree_create();
	
        // Buscar a palavra-chave na TREE (conjuto de palavras-chave) de cada site, e coletar as palavras-chave dos sites que a contém
	avl_walkcollect(sites,avl_getRoot(sites),word,kwords);

	suggested = avrel_create();

        // Buscar e armazenar os sites que possuírem  alguma das palavras chave coletadas em uma AVREL
	tree_walk(kwords,tree_getRoot(kwords),sites,suggested);

	tree_delete(&kwords);	

        return suggested;
}

// Percurso em-ordem invertido para imprimir os sites de forma decrescente por relevância
void print_sites(AVREL* suggested, NOD* root, int* counter) {

        if((root != NULL) && ((*counter) <= 5)) {

                // Sub-árvore direita
                print_sites(suggested,get_right_avrel(root),counter);
		
		// Visita
		if((*counter) <= 5) {
               		printf("SITE[%d]\n",(*counter));
               		printf("\t NOME: %s\n",site_getName(avrel_site(root)));
                	printf("\t LINK: %s\n",site_getLink(avrel_site(root)));
                	printf("\t RELEVÂNCIA: %hu",site_getRelevance(avrel_site(root)));
                	printf("\n---------------------------------------------------------\n\n");
		
			(*counter)++;
		}

                // Sub-árvore esquerda
                print_sites(suggested,get_left_avrel(root),counter);
        }
}

// Imprime os 5 (ou todos, se houverem menos que 5) primeiros sites (por ordem de reelvância decrescente) sugeridos ao usuário.
void print_top_suggestions(AVREL* suggested) {

        int counter = 1;

        printf("\nO nome e o link dos sites sugeridos são:\n\n");
        print_sites(suggested,avrel_getRoot(suggested),&counter);
}

