// Arquivo de interface de usuário com as assinaturas das funções relativas ao TAD FILA DE PRIORIDADE EM HEAP SEQUENCIAL

#ifndef HEAP_H_
#define HEAP_H_
	
	#include "site.h"

	#define boolean int 
        #define TRUE 1
        #define FALSE 0

	typedef struct heap HEAP;

	HEAP* heap_create(int size);
	boolean heap_isEmpty(HEAP* heap);
	boolean heap_insert(HEAP* heap, SITE* site);
	SITE* heap_remove(HEAP* heap);
	boolean heap_delete(HEAP** heap);
	int heap_size(HEAP* heap);

#endif 
