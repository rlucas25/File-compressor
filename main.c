#include "Heap.h"
#include "Huffman.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  int menu;
  char nome_entrada_C[256], nome_saida_C[256], nome_entrada_D[256],
      nome_saida_D[256], verificar_ocorrencia[256];
  char extensao_entrada[10];

  FILE *entrada = NULL;
  FILE *saida = NULL;

  Arvore *arvore = NULL;

  do {
    printf(
        "Menu de Opcoes:\n1. Comprimir um arquivo\n2. Contagem de ocorrencias "
        "no arquivo de texto\n3. Imprimir o codigo de Huffman "
        "para o arquivo de texto\n4. Descomprimir um arquivo\n5. Sair\n");

    scanf("%d", &menu);

    switch (menu) {

    case 1:
      printf("Digite o nome do arquivo que deseja comprimir: ");
      scanf("%s", nome_entrada_C);

      char *extensao = strrchr(nome_entrada_C, '.');

      if (extensao != NULL) {
        strncpy(extensao_entrada, extensao, 9);
      }

      printf("Digite o nome do arquivo de saida: ");
      scanf("%s", nome_saida_C);

      comprimir(arvore, extensao_entrada, nome_entrada_C, nome_saida_C);
      break;

    case 2:

      if (arvore == NULL) {
        printf("\n\tNenhuma arvore de Huffman foi criada ainda. Por favor, "
               "comprima um arquivo primeiro.\n");
        break;
      }

      int i = 0;

      printf("\tCÓDIGO ASCII\tCARACTERE\tFREQUÊNCIA\t|\tCÓDIGO "
             "ASCII\tCARACTERE\tFREQUÊNCIA\n");

      for (; i < 127; i++) {
        printf("\t\t%d\t%s\t\t%d\t", i, ascii[i],
               arvore->tabelaFrequencias[(unsigned char)i]);
        printf("\t|\t%d\t\t%s\t\t%d\n", i + 127, ascii[i + 127],
               arvore->tabelaFrequencias[(unsigned char)i + 127]);
      }
      printf("\t\t%d\t%s\t\t%d\n", i, ascii[i],
             arvore->tabelaFrequencias[(unsigned char)i]);

      break;

    case 3:
      imprimirArvore(arvore->raiz);
      break;

    case 4:
      printf(
          "Digite o nome do arquivo que deseja descomprimir (exemplo.huf): ");
      scanf("%s", nome_entrada_D);

      if (strlen(nome_entrada_D) < 4 ||
          strcmp(nome_entrada_D + strlen(nome_entrada_D) - 4, ".huf") != 0) {
        printf("\n\tErro: O arquivo deve ter a extensão .huf\n");
        break;
      }
      printf("Digite o nome do arquivo de saida: ");
      scanf("%s", nome_saida_D);

      printf("\n\tRealizando descomprensao do arquivo %s\n", nome_entrada_D);
      Arvore *arvoreRecuperada = NULL;
      descomprimir(arvoreRecuperada, nome_entrada_D, nome_saida_D);
      // Para esta implementação não precisamos da árvore recuperada após a descompressão, então podemos liberá-la
      liberarArvore(arvoreRecuperada);
      break;

    case 5:
      printf("\n\tEncerrando programa!");
      liberarArvore(arvore);
      exit(1);
      break;
    }
  } while (menu != 5);
}