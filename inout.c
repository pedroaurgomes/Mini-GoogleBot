// Arquivo .c com as implementações das funções responsáveis pelo input/output do programa e manipulação de arquivos

#include "inout.h"

// Função que lê um elemento do arquivo CSV
boolean readElement (FILE *googlebot, char* element){

        int i = 0;
        char character;

        do{
                character = (char)fgetc(googlebot);

                if(character != '\r'){
                        element[i] = character;
                        i++;
                  }

        }while((element[i-1] != ',') && (element[i-1] != '\n'));

	// Se for \n, a linha do arquivo acabou (retorna TRUE)
        if(element[i-1] == '\n'){
                element[i-1] = '\0';
                return TRUE;
        }

	// Linha ainda não foi lida completamente
        element[i-1] = '\0';

        return FALSE;
}

// Função que lê uma linha de um arquivo
char* readline (FILE *stream, int maxsize){

        int pos = 0;
        char* string;
        char character;

        string = (char*) malloc(maxsize);

        do{
                character = fgetc(stream);

                if((character != '\r')){
                        string[pos] = character;
                        pos++;
                }

        } while(string[pos-1] != '\n');

        string[pos-1] = '\0';

        return string;
}

// Função que lê o arquivo googlebot e armazena as informações em uma árvore AVL
AVL* extract_from_file(FILE *googlebot){

        int i;
        AVL* sites;
        boolean flag; // Flag para controle de leitura do arquivo.

        int p1,p2; // variáveis auxiliares que marcam pontos distintos no arquivo. -> servem para indicar se o arquivo chegou ao fim
                  // Se p2-p1 = 0, a distância do ponto atual até o fim do arquivo é 0, ou seja, o cursos se encontra no fim do arquivo

        // Declaração de auxiliares das especificações de um site em forma de strings para leitura do arquivo texto
        char code[5];
        char name[51];
        char relevance[5];
        char link[101];
        char auxkeyword[51];
        TREE* keywords;

        googlebot = fopen("googlebot.txt","r");

        sites = avl_create();

        // Lendo especificações de um site, e armazenando na estrutura de dados, em cada iteração
        do {
                /*Verificando se  chegamos ao final do arquivo*/
                // Tomando a posição atual do cursor no arquivo
                p1 = (int)ftell(googlebot);

                // Avançando até o final do arquivo e salvando o offset final
                fseek(googlebot,0,SEEK_END);
                p2 = (int)ftell(googlebot);

                // Retornando ao ponto atual do arquivo (p1)
                fseek(googlebot,p1,SEEK_SET);

                if((p2-p1) != 0){

                        keywords = tree_create();

                        readElement(googlebot,code);
                        readElement(googlebot,name);
                        readElement(googlebot,relevance);
                        readElement(googlebot,link);

                        do {
                                flag = readElement(googlebot,auxkeyword);
				tree_insert(keywords,auxkeyword);

                        } while (!flag);

                        avl_insert(sites, site_create((short int)atoi(code),name,(short int)atoi(relevance),link,keywords));
	
		}

        } while ((p2-p1) != 0);

        fclose(googlebot);

        return sites;
}

short int input_Number(){

        short int num;

        scanf("%hu",&num);
        getchar(); // Trantando o /n no buffer

        return num;
}

char* input_Name(){

        return readline(stdin,51);
}

char* input_Link(){

        return readline(stdin,101);

}

char* input_Keyword() {

        return readline(stdin,51);
}

// Algoritmo com percurso em-ordem para escrever as palavras-cheve armazenadas em uma AVL (TREE) no arquivo
void writing_keywords(TREE* keywords, NO* root, FILE* googlebot) {
	
	if(root != NULL) {

		// Sub-árvore esquerda
		writing_keywords(keywords, get_left_tree(root), googlebot);
	
		// Visita
		fprintf(googlebot,"%s,",get_keyword(root));

		// Sub-árvore direita
		writing_keywords(keywords, get_right_tree(root), googlebot);
	}
}

// Percurso em-ordem para escrever, de forma ordenada crescente, as informações dos sites - presentes na AVL -  de volta no arquivo
void write_back_in_file(AVL* sites, NODE* root, FILE* googlebot) {

	SITE* auxsite; // auxiliar para escrever as informações de um site

	if(root != NULL) {
		
		// Sub-árvore esquerda
		write_back_in_file(sites, get_left_child(root),googlebot);	

		// Visita 
		auxsite = get_site(root);

		fprintf(googlebot,"%hu,",site_getCode(auxsite));
                fprintf(googlebot,"%s,",site_getName(auxsite));
                fprintf(googlebot,"%hu,",site_getRelevance(auxsite));
                fprintf(googlebot,"%s,",site_getLink(auxsite));
		
		// Outro percurso para escrever as palavras-chave, as quais estão armazenadas por si só em uma AVL (TREE)
		writing_keywords(site_getKeywords(auxsite),tree_getRoot(site_getKeywords(auxsite)), googlebot);

		// Substituindo a última vírgula da linha por \n
		fseek(googlebot,-1,SEEK_CUR);
                fprintf(googlebot,"\n");

		// Sub-árvore direita
		write_back_in_file(sites, get_right_child(root), googlebot);
	}
}

// Função que atualiza o arquivo googlebot.txt, escrevendo os sites da lista no arquivo
void update_file(AVL* sites,FILE* googlebot){

	// Abrindo o arquivo para escrita
        googlebot = fopen("googlebot.txt","w");
	
	// Executando um algoritmo para percorrer a árvore AVL que contém as informações dos sites e escrevê-las de volta no arquivo
	write_back_in_file(sites, avl_getRoot(sites), googlebot);

	// Fechando o arquivo atualizado
        fclose(googlebot);
}
