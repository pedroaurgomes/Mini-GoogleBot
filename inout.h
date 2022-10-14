// Arquivo com a assinatura das funções responsáveis pelo input/output do programa e manipulação de arquivos

#ifndef INOUT_H
#define INOUT_H

        #include <stdlib.h>
        #include <stdio.h>
        #include "site.h"
        #include "AVL_SITE.h"
        #include "AVL_KEYWORDS.h"
	#include "AVREL_SITE.h"	

        #define TRUE 1
        #define FALSE 0
        #define boolean int
        #define ERRO -1

        boolean readElement (FILE *googlebot, char* element);
        AVL* extract_from_file(FILE *googlebot);
        void update_file(AVL* sites,FILE *googlebot);
        char* readline (FILE *stream, int maxsize);

        short int input_Number ();
        char* input_Name();
        char* input_Link();
        char* input_Keyword();
	
#endif

