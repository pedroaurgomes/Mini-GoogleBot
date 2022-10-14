// Implementação das funções correspondenetes às operações do TAD FILA DE PRIORIDADE EM HEAP SEQUENCIAL
// OBS: MAXHEAP

#include "heap.h"
#include <stdio.h>
#include <stdlib.h>

// Fila de prioridades de site implementada implementada em Heap
// Obs: A prioridade é medida de acordo com a relevância dos sites
typedef struct heap {

        SITE** array;
        int end;
	
} HEAP;

// Cria (aloca) e retorna uma estrutura heap vazia, com o tamanho definido por parâmetro
// Obs: O tamanho máximo da heap será o tamanho da AVL (nesse projeto Googlebot parte2)
HEAP* heap_create(int size) {

        HEAP* heap;

        heap = (HEAP*) malloc(sizeof(HEAP));

        if(heap != NULL) {
         	heap->array = (SITE**) calloc(size,sizeof(SITE*));
	 	heap->end = -1; // heap vazia tem fim = -1
	}

        return heap;
}

// Verifica se uma heap está vazia
boolean heap_isEmpty(HEAP* heap) {

	if(heap->end == -1)
		return TRUE;
	
	return FALSE;
}

// Restaura a ordem da heap, árvore acima, após uma inserção
void heap_fix_up(HEAP* heap) {

        int w; // posição do último item da heap
        int pai; // posição do pai do último item da heap
	SITE* tmp; // variável temporária para swapping

        w = heap->end;
        pai = (w - 1) / 2;

        // Enquanto o filho for maior que o pai, trocamos (swapping)
	while ((w > 0) && (site_getRelevance(heap->array[w]) > site_getRelevance(heap->array[pai]))) {
                // Swapping
                tmp = heap->array[w];
                heap->array[w] = heap->array[pai];
                heap->array[pai] = tmp;

                // Subindo na árvore heap (atualizando w e pai)
                w = pai;
                pai = (pai - 1) / 2;
        }
}

// Inserção (enfileiramento) em heap (inserção no fim de uma fila de prioridades (vetor))
boolean heap_insert(HEAP* heap, SITE* site) {

        if(heap != NULL) {

                heap->end++; // incrementando fim para inserir no último elemento
                heap->array[heap->end] = site;
                heap_fix_up(heap); // restaurando a ordem na heap

                return TRUE;
        }

        return FALSE;
}

// Restaura a ordem da heap, árvore abaixo, após uma remoção
void heap_fix_down(HEAP* heap) {

        int r = 0; // posição da raiz (primeiro item) da heap
        int filho; // posição do maior filho da raiz (pois se houverem trocas, o maior de todos deve ficar na raiz)
        SITE* tmp; // variável temporária para swapping

	// Não há filhos, a raiz é o único elemento na heap
	if(heap->array[1] == NULL)
		return;

	else if(heap->array[2] == NULL)
		filho = 1;
	
	else 	
		filho = (site_getRelevance(heap->array[1]) >= site_getRelevance(heap->array[2])) ? 1 : 2;

        // Enquanto não for um nó folha (possuir um filho esquerdo, já que heaps são árvores completas) e for menor do que o filho,
        // trocamos (swapping)
        while ((heap->array[filho] != NULL) && (site_getRelevance(heap->array[r]) < site_getRelevance(heap->array[filho]))) {

                // Swapping
                tmp = heap->array[r];
                heap->array[r] = heap->array[filho];
                heap->array[filho] = tmp;

                // Descendo na árvore heap (atualizando r e filho)
                r = filho;
	
		// Atualizando filho para seu maior filho
		if((heap->array[filho * 2 + 1] != NULL) && (heap->array[filho * 2 + 2] != NULL))
			filho = (site_getRelevance(heap->array[filho * 2 + 1]) >= site_getRelevance(heap->array[filho * 2 + 2])) 
									? filho * 2 + 1 : filho * 2 + 2;	
        	else 
			filho = filho * 2 + 1;
	}
}

// Função que remove (desenfileira) da fila de prioridades (heap). A remoção ocorre sempre na raiz, a qual em MAXHEAP possui a maior
// prioridade, então faz sentido sair da fila primeiro, e que deve ser retornada
SITE* heap_remove(HEAP* heap) {

        SITE* raiz = NULL; // auxiliar para salvar o item da raiz da heap, o qual será removido e retornado

        if((heap != NULL) && (!heap_isEmpty(heap))) {

                raiz = heap->array[0]; // Armazenando o site da raiz para retorno

                heap->array[0] = heap->array[heap->end]; // Substituindo o site da raiz pelo último site da heap
		heap->array[heap->end] = NULL;
                heap->end--; // atualizando o fim da heap

		if(heap->end != -1)
                	heap_fix_down(heap); //restaurando a ordem na heap
        }

	return raiz;
}

// Apaga todo o vetor da heap e libera a estrutura heap
boolean heap_delete(HEAP** heap) {

        int i;

        if ((*heap) != NULL ) {

                free((*heap)->array);
                free(*heap);
                *heap = NULL;

                return TRUE;
        }

        return FALSE;
}

// Retorna o tamanho de uma heap
int heap_size(HEAP* heap) {
	
	return (heap->end + 1);

}

