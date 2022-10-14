// Interface com as funções das novas funcionalidades do googlebot, incluindo busca por palavras-chave e sugestão de sites


#ifndef NEWFUN_H_
#define NEWFUN_H_

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "AVL_KEYWORDS.h"
	#include "AVL_SITE.h"
	#include "site.h"
	#include "heap.h"
	#include "AVREL_SITE.h"		
	
	HEAP* inspect_sites(AVL* sites, char* word);
	AVREL* site_suggestion(AVL* sites, char* word); 
	void print_top_suggestions(AVREL* suggested);

#endif
