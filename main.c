#include <stdio.h>
#include <stdlib.h>
#include "Heap.h"
#include "Huffman.h"

int main()
{
    int menu;
    char nome_entrada[256], nome_saida[256], verificar_ocorrencia[256];
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
            scanf("%s", nome_entrada);

            printf("Digite o nome do arquivo de saida: ");
            scanf("%s", nome_saida);

            arvore = Huffman(nome_entrada);
            if (arvore == NULL)
            {
                printf("Erro ao cria a árvore de Huffman.\n");
                ;
                break;
            }
            char caminho[256];
            gerarDicionario(arvore->raiz, dicionario, caminho, 0);

            entrada = fopen(nome_entrada, "rb");
            saida = fopen(nome_saida, "wb");
            if (entrada == NULL || saida == NULL)
            {
                printf("\tFalha ao abrir o arquivo! Voltando ao menu inicial!\n\n");
                break;
            }
            fwrite(arvore->tabelaFrequencias, sizeof(int), 256, saida);
            codificar(entrada, saida, dicionario);

            printf("Codificado!");
            fclose(entrada);
            fclose(saida);
            break;

        case 2:
            printf("Digite qual elemento voce deseja verificar a ocorrencia: ");
            scanf("%s", &verificar_ocorrencia);

            if (arvore == NULL)
            {
                printf("Nenhuma arvore de Huffman foi criada ainda. Por favor, comprima um arquivo primeiro.\n");
                break;
            }
            else
            {
                printf("A letra '%s' ocorre %d vezes no arquivo.\n", verificar_ocorrencia, arvore->tabelaFrequencias[(unsigned char)verificar_ocorrencia[0]]);
            }
            break;

        case 3:
            imprimeHuffman(arvore->raiz);
            break;

        case 4:
            printf("Digite o nome do arquivo: ");
            scanf("%s", nome_entrada);

            printf("Digite o nome do arquivo de saida: ");
            scanf("%s", nome_saida);

            printf("Realizando descomprensao do arquivo %s\n", nome_entrada);

            saida = fopen(nome_saida, "wb");
            entrada = fopen(nome_entrada, "rb");

            int tabelaRecuperada[256] = {0};
            fread(tabelaRecuperada, sizeof(int), 256, entrada);
            Arvore *arvoreRecuperada = criarArvoreHuffman(tabelaRecuperada);
            decodificar(arvoreRecuperada->raiz, entrada, saida);

            liberarHuffman(arvoreRecuperada->raiz);
            free(arvoreRecuperada);
            break;

        case 5:
            printf("Encerrando programa!");
            exit(1);
            break;
        }
    } while (menu != 5);
}