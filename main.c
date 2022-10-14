/*
 * Projeto Mini-Googlebot - parte 2
 *
 * Nome: Pedro Augusto Ribeiro Gomes
 * N USP: 11819125
 * Turma: BCC020 ICMC-USP
 * email: parg07@usp.br
 * Professor: Rudinei Goularte
 * Disciplina: Algoritmo e Estruturas de Dados 1
 * =========================================
 */

// Programa cliente para a utilização do Googlebot

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "site.h"
#include "AVL_SITE.h"
#include "AVL_KEYWORDS.h"
#include "AVREL_SITE.h"
#include "newFun.h"
#include "heap.h"
#include "inout.h"

int main (int argc, char* argv[]){

	short int op; // variável que controla as operações realizadas pelo usuário
        FILE* googlebot = NULL;
        AVL* sites = NULL; // Estrutura que armazena as especificações dos sites do arquivo googlebot
	SITE* target = NULL; // site auxiliar para testes de verificação
	short int qtd_kw; // Quantidade de palavras-chave de um site
        int i; // variável  auxiliar para laços e contador
	HEAP* priorlist = NULL; // Fila de prioridade com os sites que possuírem determinada palavra-chave, 
			       // onde a prioridade é a relevância de cada site (operação 5)
	AVREL* suggested; // Armazena os sites sugeridos ao usuário, dado uma palavra-chave (operação 6)

	// Variáveis auxiliares para inputs do usuário
        short int auxcode;
        char* auxname;
        short int auxrelevance;
        char* auxlink;
        char* auxkeyword;
	TREE* auxtree;

	// Extraindo as informações do arquivo googlebot.txt e armazenando em uma estrutura de dados AVL
        sites = extract_from_file(googlebot);

        // Mensagem inicial do programa
        printf("\nBem-vindo ao menu de operações do googlebot!\n");

        do {
                // Menu de Operações
                printf("\n Digite o código da operação que deseja realizar, digite enter e siga as instruções:\n");
                printf("Formato: <código da operação>. <operação>\n");
                printf("1. Inserir um site\n");
                printf("2. Remover um site\n");
                printf("3. Inserir palavra-chave referente a um site\n");
                printf("4. Atualizar a relevância de um site\n");
                printf("5. Buscar por sites que contenham uma dada palavra-chave\n");
		printf("6. Sugerir sites a partir de uma determinada palavra-chave\n");
		printf("7. Aplicar alterações no googlebot e Sair do programa\n");
                printf("\n --> Digite o código escolhido: ");

                // Lendo o código de operação que o usuário deseja realizar
                op = input_Number();
		printf("\n");

                // Direcionamento das operações
                switch(op) {

                        // Inserir um site
                        case 1:
                                printf("\nVocê deseja INSERIR um site.\n");

                                printf("\nDigite o CÓDIGO do site que deseja inserir: ");
                                auxcode = input_Number();

                                // Caso o código já seja relacionado a algum site existente no googlebot, o usuário deve fornecer outro código
                                while (avl_search(sites,auxcode)) {        
					printf("\nEsse código já está relacionado com outro site no googlebot.");
                                        printf("\nDigite outro código: ");
                                        auxcode = input_Number();
                                }
				
				printf("\nDigite o NOME do site que deseja inserir: ");
                                auxname = input_Name();

                                printf("\nDigite a RELEVÂNCIA do site que deseja inserir: ");
                                auxrelevance = input_Number();

                                printf("\nDigite o LINK do site que deseja inserir: ");
                                auxlink = input_Link();

				auxtree = tree_create();

                                printf("\nDigite a QUANTIDADE de palavras-chave que deseja inserir para o site: ");
                                qtd_kw = input_Number();

                                printf("\nDigite as PALAVRAS-CHAVE do site que deseja inserir: ");

                                for(i = 0; i < qtd_kw; i++){
                                        auxkeyword = readline(stdin,51);
                                        tree_insert(auxtree,auxkeyword);
					free(auxkeyword);
                                        auxkeyword = NULL;
                                }

                                avl_insert(sites,site_create(auxcode,auxname,auxrelevance,auxlink,auxtree));
				printf("\n%s inserido com sucesso no googlebot.\n",auxname);

				free(auxname);
                                free(auxlink);

                                break;

                        // Remover um site
                        case 2:
                                printf("\nVocê deseja REMOVER um site.\n");

                                printf("\nDigite o CÓDIGO do site que deseja remover: ");
                                auxcode = input_Number();

				if(avl_search(sites,auxcode)){        
					printf("\n %s removido com sucesso.\n\n",site_getName(avl_search(sites,auxcode)));	
					avl_remove(sites,auxcode);
                                }

                                else
                                        printf("\nO código do site que tentou remover não se encontra no googlebot\n");

                                break;

			// Inserir palavra-chave        
                        case 3:
                                printf("\nVocê deseja inserir PALAVRAS-CHAVE relativas a um site.\n");

                                printf("\nDigite o CÓDIGO do site cujas palavras-chave você deseja inserir: ");
                                auxcode = input_Number();

				target = avl_search(sites,auxcode);
                                
				while(target == NULL){
                                        printf("\nEsse código não está relacionado com nenhum site no googlebot.\n");
                                        printf("\nDigite outro código: ");
                                        auxcode = input_Number();
					target = avl_search(sites,auxcode);
                                }

				if(tree_get_size(site_getKeywords(target)) >= 10) 
					printf("\nO limite máximo de 10 palavras-chave desse site já foi atingido.\n");

                                else {
                                        printf("\nDigite a palavra-chave que deseja inserir: ");
                                        auxkeyword = input_Keyword();

					tree_insert(site_getKeywords(target), auxkeyword);

                                        printf("\nA palavra-chave '%s' foi inserida com sucesso.\n",auxkeyword);
                                	
					free(auxkeyword);
				}

                                break;

                        // Atualizar Relevância 
                        case 4:
                                printf("\nVocê deseja atualizar a RELEVÂNCIA de um site.\n");

                                printf("\nDigite o CÓDIGO do site cuja relevância você deseja alterar: ");
                                auxcode = input_Number();

				target = avl_search(sites,auxcode);

                                while(target == NULL){
                                        printf("\nEsse código não está relacionado com nenhum site no googlebot.");
                                        printf("\nDigite outro código: ");
                                        auxcode = input_Number();
					target = avl_search(sites,auxcode);
                                }

				printf("\nDigite o novo valor de relevância para o site: ");
                                auxrelevance = input_Number();

                                if(site_updateRelevance(target,auxrelevance))
                                        printf("\nA relevância de %s foi atualizada com sucesso.\n",site_getName(target));

                                break;

			// Buscar por sites que contenham uma determinada palavra-chave	
			case 5:
			        printf("Você deseja buscar por sites que contenham uma dada palavra-chave\n");	
				
	 			printf("Digite a palavra-chave: \n");
			
				auxkeyword = input_Keyword();

				priorlist = inspect_sites(sites, auxkeyword);

				// Ajustando contador para os sites a serem impressos
				i = 1;
				
				if(heap_isEmpty(priorlist))
					printf("\nNão foram encontrados sites que contenham a palavra-chave\n");
				else 
					printf("\nO nome e o link dos sites que contêm a palavra-chave '%s' são: \n\n",auxkeyword);

				while(!heap_isEmpty(priorlist)) {
					
					target = heap_remove(priorlist);
					
					printf("SITE[%d]\n",i);
					printf("\t NOME: %s\n",site_getName(target));
					printf("\t LINK: %s\n",site_getLink(target));
					printf("\t RELEVÂNCIA: %hu",site_getRelevance(target));
					printf("\n---------------------------------------------------------\n");	
					
					target = NULL;

					i++;
				}			

				heap_delete(&priorlist);
				priorlist = NULL;

				free(auxkeyword);

				break;

			// Sugerir sites com base em uma palavra-chave	
			case 6:
				printf("Você deseja sugestão de sites através de uma palavra-chave\n");

                                printf("Digite a palavra-chave: \n");

                                auxkeyword = input_Keyword();
				
				suggested = site_suggestion(sites,auxkeyword);
					
				print_top_suggestions(suggested);
				
				avrel_delete(&suggested);
				suggested = NULL;
				
				free(auxkeyword);

				break;

                        // Sair do programa
                        case 7:

                                // Atualizando o arquivo googlebot.txt
                                update_file(sites,googlebot);

                                // Apagar a AVL que contém as informações dos sites
				avl_delete(&sites);
                                
				// Mensagem de despedida
                                printf("As aplicações foram devidamente aplicadas e o googlebot atualizado.\n");
                                printf("\n\t... Programa finalizado ... \n\n");

                                break;

                        default:
                                printf("Escolha uma operação válida\n");
                }


        } while(op != 7);

	return 0;
}
