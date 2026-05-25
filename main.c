#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Heap.h"
#include "Huffman.h"

int main()
{
    int menu;
    char nome_entrada_C[256], nome_saida_C[256], nome_entrada_D[256], nome_saida_D[256], verificar_ocorrencia[256];
    char extensao_entrada[10];
    char dicionario[256][256] = {0};
    FILE *entrada = NULL;
    FILE *saida = NULL;

    // Ponteiro para segurar a árvore quando ela for criada (não perder a árvore)
    Arvore *arvore = NULL;

    do
    {
        printf("Menu de Opcoes:\n1. Comprimir um arquivo\n2. Contagem de ocorrencias de um elemento no arquivo de texto\n3. Imprimir o codigo de Huffman para o arquivo de texto\n4. Descomprimir um arquivo\n5. Sair\n");

        scanf("%d", &menu);

        switch (menu)
        {
        case 1:
            printf("Digite o nome do arquivo: ");
            scanf("%s", nome_entrada_C);

            char *extensao = strrchr(nome_entrada_C, '.');

            if (extensao != NULL)
            {
                strncpy(extensao_entrada, extensao, 9);
            }

            printf("Digite o nome do arquivo de saida: ");
            scanf("%s", nome_saida_C);
            
            strcat(nome_saida_C, ".huf");

            //se for igual vai substituir .huf para (1).huf
            if (strcmp(nome_entrada_C, nome_saida_C) == 0){

                //ponteiro ponto para apontar para o ultimo ponto
                char *ponto = strrchr (nome_saida_C,'.');

                // se não for nulo ele faz esse ponto se \0 para mandar o strcat
                if (ponto != NULL){
                    *ponto = '\0';
                }
                //agora que é \0 ele vai concatenar (1).huf até o novo \0 que ficava o .huf sobescrevendo ele.
                strcat(nome_saida_C, "(1).huf");
            }

            entrada = fopen(nome_entrada_C, "rb");
            saida = fopen(nome_saida_C, "wb");
            
            if (entrada == NULL || saida == NULL)
            {
                printf("\n\tFalha ao abrir o arquivo! Voltando ao menu inicial!\n\n");
                break;
            }
            
            fseek(entrada, 0, SEEK_END);
            long tamanhoEntrada = ftell(entrada);
            fseek(entrada, 0, SEEK_SET);
            
            if ( arvore != NULL)
            {
                liberarArvore(arvore);
                arvore = NULL;
            }
            
            arvore = huffman(nome_entrada_C);
            
            if (arvore == NULL)
            {
                printf("Erro ao cria a árvore de Huffman.\n");
                break;
            }
            char caminho[256];
            gerarDicionario(arvore->raiz, dicionario, caminho, 0);
            

            fwrite(arvore->tabelaFrequencias, sizeof(int), 256, saida);
            fwrite(extensao_entrada, sizeof(char), 10, saida);
            codificar(entrada, saida, dicionario);

            fseek(saida, 0, SEEK_END);
            long tamanhoSaida = ftell(saida);
            fseek(saida, 0, SEEK_SET);

            printf("\n\tComprimido!\nTamanho Original: %ld bytes (100%%)\nTamanho Comprimido: %ld bytes (%.2f%%)\n\n", tamanhoEntrada, tamanhoSaida, (float)tamanhoSaida / tamanhoEntrada * 100);
            fclose(entrada);
            fclose(saida);
            break;

        case 2:
            printf("Digite qual elemento voce deseja verificar a ocorrencia: ");
            scanf("%s", verificar_ocorrencia);

            if (arvore == NULL)
            {
                printf("\n\tNenhuma arvore de Huffman foi criada ainda. Por favor, comprima um arquivo primeiro.\n");
                break;
            }
            else
            {
                printf("\n\tA letra '%s' ocorre %d vezes no arquivo.\n", verificar_ocorrencia, arvore->tabelaFrequencias[(unsigned char)verificar_ocorrencia[0]]);
            }
            break;

        case 3:
            imprimirArvore(arvore->raiz);
            break;

        case 4:
            printf("Digite o nome do arquivo: ");
            scanf("%s", nome_entrada_D);

            printf("\n\tRealizando descomprensao do arquivo %s\n", nome_entrada_D);

            entrada = fopen(nome_entrada_D, "rb");
            if (entrada == NULL)
            {
                printf("\n\tErro ao abrir o arquivo compactado\n");
                break;
            }
            // Lê os primeiros 256 caracteres no arquivo que salvamos de cabeçário
            int tabelaRecuperada[256] = {0};
            fread(tabelaRecuperada, sizeof(int), 256, entrada);

            // lê os caracteres depois dos 256 caracteres, ou seja, os 10 caracteres de extensão salvo no cabeçário.
            char extensao_recuperada[10] = {0};
            fread(extensao_recuperada, sizeof(char), 10, entrada);

            // copiar o nome de entrada colocado para o nome de saida
            strcpy(nome_saida_D, nome_entrada_D);

            char *ponto_huf = strrchr(nome_saida_D, '.');

            // Se não for nulo, trocar o . por \0 para ler só até antes do .huff
            if (ponto_huf != NULL)
            {
                *ponto_huf = '\0';
            }
            // colocar descomprimido para não sobescrever o original
            strcat(nome_saida_D, "_descomprimido");

            // se tiver uma extensão ele coloca ela no arquivo de saida, se não ele retorna como .txt no pior dos casos
            if (strlen(extensao_recuperada) > 0)
            {
                strcat(nome_saida_D, extensao_recuperada);
            }
            else
            {
                strcat(nome_saida_D, ".txt");
            }

            saida = fopen(nome_saida_D, "wb");
            if (saida == NULL)
            {
                printf("\n\tErro ao criar o arquivo de saída\n");
                fclose(entrada);
                break;
            }

            Arvore *arvoreRecuperada = criarArvoreHuffman(tabelaRecuperada);
            decodificar(arvoreRecuperada->raiz, entrada, saida);

            liberarArvore(arvoreRecuperada);
            fclose(entrada);
            fclose(saida);
            printf("\n\tArquivo descomprimido com sucesso...\n Nome do arquivo: %s\n", nome_saida_D);
            break;

        case 5:
            printf("\n\tEncerrando programa!");
            liberarArvore(arvore);
            exit(1);
            break;
        }
    } while (menu != 5);
}