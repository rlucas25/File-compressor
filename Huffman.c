/* 
    ALUNOS
Dennis Francisco Guimarães de Oliveira Baracho
Heitor Santos Maia
Lucas Rebouças Almeida
Marcos Vinícius Pimentel Gomes

*/

#include "Huffman.h"
#include "Heap.h"
#include <string.h>
const char *ascii[256] = {
    "NUL", "SOH", "STX", "ETX", "EOT", "ENQ", "ACK", "BEL", "BS",  "HT",  "LF",
    "VT",  "FF",  "CR",  "SO",  "SI",  "DLE", "DC1", "DC2", "DC3", "DC4", "NAK",
    "SYN", "ETB", "CAN", "EM",  "SUB", "ESC", "FS",  "GS",  "RS",  "US",

    " ",   "!",   "\"",  "#",   "$",   "%%",  "&",   "\'",  "(",   ")",   "*",
    "+",   ",",   "-",   ".",   "/",

    "0",   "1",   "2",   "3",   "4",   "5",   "6",   "7",   "8",   "9",   ":",
    ";",   "<",   "=",   ">",   "?",

    "@",   "A",   "B",   "C",   "D",   "E",   "F",   "G",   "H",   "I",   "J",
    "K",   "L",   "M",   "N",   "O",

    "P",   "Q",   "R",   "S",   "T",   "U",   "V",   "W",   "X",   "Y",   "Z",
    "[",   "\\",  "]",   "^",   "_",

    "`",   "a",   "b",   "c",   "d",   "e",   "f",   "g",   "h",   "i",   "j",
    "k",   "l",   "m",   "n",   "o",

    "p",   "q",   "r",   "s",   "t",   "u",   "v",   "w",   "x",   "y",   "z",
    "{",   "|",   "}",   "~",   "DEL",

    "Ç",   "ü",   "é",   "â",   "ä",   "à",   "å",   "ç",   "ê",   "ë",   "è",
    "ï",   "î",   "ì",   "Ä",   "Å",   "É",   "æ",   "Æ",   "ô",   "ö",   "ò",
    "û",   "ù",   "ÿ",   "Ö",   "Ü",   "¢",   "£",   "¥",   "₧",   "ƒ",

    "á",   "í",   "ó",   "ú",   "ñ",   "Ñ",   "ª",   "º",   "¿",   "⌐",   "¬",
    "½",   "¼",   "¡",   "«",   "»",   "░",   "▒",   "▓",   "│",   "┤",   "Á",
    "Â",   "À",   "©",   "╣",   "║",   "╗",   "╝",   "¢",   "¥",   "┐",

    "└",   "┴",   "┬",   "├",   "─",   "┼",   "ã",   "Ã",   "╚",   "╔",   "╩",
    "╦",   "╠",   "═",   "╬",   "¤",   "ð",   "Ð",   "Ê",   "Ë",   "È",   "ı",
    "Í",   "Î",   "Ï",   "┘",   "┌",   "█",   "▄",   "¦",   "Ì",   "▀",

    "Ó",   "ß",   "Ô",   "Ò",   "õ",   "Õ",   "µ",   "þ",   "Þ",   "Ú",   "Û",
    "Ù",   "ý",   "Ý",   "¯",   "´",   "≡",   "±",   "‗",   "¾",   "¶",   "§",
    "÷",   "¸",   "°",   "¨",   "·",   "¹",   "³",   "²",   "■",   " "};

bool ehMenor_Huffman(void *pA, void *pB) {
  if (pA == NULL || pB == NULL) {
    printf("Erro: Ponteiro para nó é nulo\n");
    return false;
  }
  No *noA = (No *)pA;
  No *noB = (No *)pB;

  return (noA->freq <= noB->freq);
}

void frequenciaHuffman(const char *nomeArquivo, int *tabelaFrequencias) {
  if (nomeArquivo == NULL || tabelaFrequencias == NULL) {
    printf("Erro: Nome do arquivo ou tabela de frequências é nulo\n");
    return;
  }

  for (int i = 0; i < 256; i++) {
    tabelaFrequencias[i] = 0;
  }

  FILE *arquivo = fopen(nomeArquivo, "rb");
  if (arquivo == NULL) {
    printf("Erro ao abrir o arquivo!\n");
    return;
  }

  int byteLido;
  byteLido = fgetc(arquivo);

  while (byteLido != EOF) {
    tabelaFrequencias[byteLido]++;
    byteLido = fgetc(arquivo);
  }
  fclose(arquivo);
}

No *criaNo(unsigned char c, int freq) {

  No *novo = malloc(sizeof(No));
  if (novo == NULL) {
    printf("Erro: Falha ao alocar memória para o nó\n");
    return NULL;
  }
  novo->c = c;
  novo->freq = freq;
  novo->left = NULL;
  novo->right = NULL;

  return novo;
}

Arvore *huffman(const char *arquivo) {
  if (arquivo == NULL) {
    printf("Erro: Nome do arquivo é nulo\n");
    return NULL;
  }

  int tabelaTemporaria[256] = {0};
  frequenciaHuffman(arquivo, tabelaTemporaria);

  return criarArvoreHuffman(tabelaTemporaria);
}

Arvore *criarArvoreHuffman(int *tabelaFrequencias) {
  if (tabelaFrequencias == NULL) {
    printf("Erro: Tabela de frequências é nula\n");
    return NULL;
  }

  No *x, *y, *z;

  Heap *heap = criarHeap(256);

  if (heap == NULL) {
    printf("Erro: Falha ao criar a heap\n");
    return NULL;
  }
  if (heap->array == NULL) {
    printf("Erro: Falha ao alocar memória para o array da heap\n");
    free(heap);
    return NULL;
  }

  Arvore *arvore = malloc(sizeof(Arvore));

  if (arvore == NULL) {
    printf("Erro: Falha ao alocar memória para a árvore de Huffman\n");
    free(heap->array);
    free(heap);
    return NULL;
  }
  arvore->raiz = NULL;
  arvore->tabelaFrequencias = NULL;
  arvore->tamanhoTabela = 0;

  arvore->tabelaFrequencias = malloc(256 * sizeof(int));

  if (arvore->tabelaFrequencias == NULL) {
    printf("Erro: Falha ao alocar memória para a tabela de frequências da "
           "árvore\n");
    liberarArvore(arvore);
    free(heap->array);
    free(heap);
    return NULL;
  }
  arvore->tamanhoTabela = 256;

  for (int i = 0; i < 256; i++) {
    arvore->tabelaFrequencias[i] = tabelaFrequencias[i];
  }

  for (int i = 0; i < 256; i++) {
    if (arvore->tabelaFrequencias[i] > 0) {
      z = criaNo((unsigned char)i, arvore->tabelaFrequencias[i]);
      if (z == NULL) {
        printf("Erro: Falha ao criar nó para o caractere %d\n", i);
        liberarArvore(arvore);
        free(heap->array);
        free(heap);
        return NULL;
      }
      inserir(heap, z, ehMenor_Huffman);
    }
  }
  if (heap->tamanho == 0) {
    arvore->raiz = criaNo('$', 0);
    free(heap->array);
    free(heap);
    return arvore; 
  }
  if (heap->tamanho == 1) {
    z = criaNo('$', 0);
    inserir(heap, z, ehMenor_Huffman);
  }
  while (heap->tamanho > 1) {
    x = retiraMinimo(heap, ehMenor_Huffman);
    y = retiraMinimo(heap, ehMenor_Huffman);
    z = criaNo('$', x->freq + y->freq);
    if (z == NULL) {
      printf("Erro: Falha ao criar nó interno para os nós %c e %c\n", x->c,
             y->c);
      liberarArvore(arvore);
      free(heap->array);
      free(heap);
      return NULL;
    }

    z->left = x;
    z->right = y;
    z->freq = x->freq + y->freq;

    inserir(heap, z, ehMenor_Huffman);
  }

  arvore->raiz = retiraMinimo(heap, ehMenor_Huffman);
  free(heap->array);
  free(heap);
  return arvore;
}

void gerarDicionario(No *raiz, char dicionario[256][256], char caminho[256],
                     int indice) {
  if (raiz == NULL || dicionario == NULL || caminho == NULL) {
    printf("Erro: Raiz, dicionário ou caminho é nulo\n");
    return;
  }

  if (raiz->left == NULL && raiz->right == NULL) {
    caminho[indice] = '\0';
    strcpy(dicionario[raiz->c], caminho);
    return;
  }

  caminho[indice] = '0';
  gerarDicionario(raiz->left, dicionario, caminho, indice + 1);

  caminho[indice] = '1';
  gerarDicionario(raiz->right, dicionario, caminho, indice + 1);
}

void codificar(FILE *entrada, FILE *saida, char codigos[256][256]) {
  if (entrada == NULL || saida == NULL || codigos == NULL) {
    printf("Erro: Arquivo de entrada, saída ou códigos é nulo\n");
    return;
  }

  int byteLido;
  unsigned char byteSaida = 0;
  int quantidadeBits = 0;

  rewind(entrada);

  while ((byteLido = fgetc(entrada)) != EOF) {
    char *codigo = codigos[(unsigned char)byteLido];

    for (int i = 0; codigo[i] != '\0'; i++) {
      byteSaida = byteSaida << 1;

      if (codigo[i] == '1') {
        byteSaida = byteSaida | 1;
      }

      quantidadeBits++;

      if (quantidadeBits == 8) {
        fputc(byteSaida, saida);

        byteSaida = 0;
        quantidadeBits = 0;
      }
    }
  }

  if (quantidadeBits > 0) {
    byteSaida = byteSaida << (8 - quantidadeBits);
    fputc(byteSaida, saida);
  }
}

bool decodificar(No *raiz, FILE *entrada, FILE *saida) {
  if (raiz == NULL || entrada == NULL || saida == NULL) {
    printf("Erro: Raiz, arquivo de entrada ou saída é nulo\n");
    return false;
  }
  
  if (raiz->freq == 0) {
    return true; 
  }

  if (raiz->freq < 0) {
    printf("Erro: A frequência da raiz é negativa, não é possível "
           "decodificar\n");
    return false;
  }

  No *atual = raiz;
  unsigned char byte;

  int totalLetras = raiz->freq;
  int letrasImpressas = 0;

  while (fread(&byte, 1, 1, entrada) == 1) {
    for (int i = 7; i >= 0; i--) {
      int bit = (byte >> i) & 1;

      if (bit == 0) {
        atual = atual->left;
      } else {
        atual = atual->right;
      }

      if (atual->left == NULL && atual->right == NULL) {
        fputc(atual->c, saida);
        atual = raiz;
        letrasImpressas++;

        if (letrasImpressas == totalLetras)
          return true;
      }
    }
  }
  return false;
}

Arvore *comprimir(const char *extensao, const char *nomeEntrada,
                  char *nomeSaida) {
  if (nomeEntrada == NULL || nomeSaida == NULL) {
    printf("Erro: Nome do arquivo de entrada ou saída é nulo\n");
    return NULL;
  }
  char dicionario[256][256] = {0};

  strcat(nomeSaida, ".huf");

  if (strcmp(nomeEntrada, nomeSaida) == 0) {

    char *ponto = strrchr(nomeSaida, '.');
    if (ponto != NULL) {
      *ponto = '\0';
    }
    strcat(nomeSaida, "(1).huf");
  }

  FILE *entrada = fopen(nomeEntrada, "rb");
  if (entrada == NULL){
    printf("\n\tFalha ao abrir o arquivo! Voltando ao menu inicial!\n\n");
    return NULL;
  }
  FILE *saida = fopen(nomeSaida, "wb"); 
  if (saida == NULL){
    fclose(entrada);
    printf("\n\tFalha ao abrir o arquivo! Voltando ao menu inicial!\n\n");
    return NULL;
  }

  fseek(entrada, 0, SEEK_END);
  long tamanhoEntrada = ftell(entrada);
  fseek(entrada, 0, SEEK_SET);

  Arvore *arvore = huffman(nomeEntrada);

  if (arvore == NULL) {
    printf("Erro ao cria a árvore de Huffman.\n");
    fclose(entrada);
    fclose(saida);
    return NULL;
  }

  char caminho[256];
  gerarDicionario(arvore->raiz, dicionario, caminho, 0);

  fwrite(arvore->tabelaFrequencias, sizeof(int), 256, saida);
  fwrite(extensao, sizeof(char), 10, saida);
  codificar(entrada, saida, dicionario);

  fseek(saida, 0, SEEK_END);
  long tamanhoSaida = ftell(saida);
  fseek(saida, 0, SEEK_SET);

  printf("\n\tComprimido!\nTamanho Original: %ld bytes (100%%)\nTamanho "
         "Comprimido: %ld bytes (%.2f%%)\n\n",
         tamanhoEntrada, tamanhoSaida,
         (float)tamanhoSaida / tamanhoEntrada * 100);
  fclose(entrada);
  fclose(saida);
  return arvore;
}

Arvore *descomprimir(const char *nomeEntrada, char *nomeSaida) {
  FILE *entrada = fopen(nomeEntrada, "rb");
  FILE *saida = NULL;

  if (entrada == NULL) {
    printf("\n\tErro ao abrir o arquivo compactado\n");
    return NULL;
  }

  int tabelaRecuperada[256] = {0};
  fread(tabelaRecuperada, sizeof(int), 256, entrada);

  char extensao_recuperada[10] = {0};
  fread(extensao_recuperada, sizeof(char), 10, entrada);

  char *ponto_huf = strrchr(nomeSaida, '.');
  if (ponto_huf != NULL) {
    *ponto_huf = '\0';
  }

  if (strlen(extensao_recuperada) > 0) {
    strcat(nomeSaida, extensao_recuperada);
  } else {
    strcat(nomeSaida, ".txt");
  }

  if (strcmp(nomeEntrada, nomeSaida) == 0) {
    char *ponto = strrchr(nomeSaida, '.');
    if (ponto != NULL) {
      *ponto = '\0';
    }
    strcat(nomeSaida, "(1)");
    if (strlen(extensao_recuperada) > 0) {
      strcat(nomeSaida, extensao_recuperada);
    } else {
      strcat(nomeSaida, ".txt");
    }
  }

  saida = fopen(nomeSaida, "wb");
  if (saida == NULL) {
    printf("\n\tErro ao criar o arquivo de saída\n");
    fclose(entrada);
    return NULL;
  }
  Arvore *arvoreRecuperada = criarArvoreHuffman(tabelaRecuperada);

  bool sucessoDecodificacao =
      decodificar(arvoreRecuperada->raiz, entrada, saida);

  if (sucessoDecodificacao) {
    printf("\n\tArquivo descomprimido com sucesso...\n\t Nome do arquivo: %s\n",
           nomeSaida);
  } else {
    printf("\n\tErro ao descomprimir o arquivo\n");
    liberarArvore(arvoreRecuperada);
    arvoreRecuperada = NULL;
  }

  fclose(entrada);
  fclose(saida);
  return arvoreRecuperada;
}

void liberarArvore(Arvore *arvore) {
  if (arvore == NULL)
    return;

  if (arvore->raiz != NULL)
    auxLiberarArvore(arvore->raiz);
  if (arvore->tabelaFrequencias != NULL)
    free(arvore->tabelaFrequencias);
  free(arvore);
}

void auxLiberarArvore(No *raiz) {
  if (raiz == NULL)
    return;

  auxLiberarArvore(raiz->left);

  auxLiberarArvore(raiz->right);

  free(raiz);
}

int altura(No *raiz) {
  if (raiz == NULL)
    return 0;

  int esq = altura(raiz->left);
  int dir = altura(raiz->right);

  if (esq > dir)
    return esq + 1;
  else
    return dir + 1;
}

void escreverNo(char matriz[MAX_ALTURA][MAX_LARGURA], int linha, int coluna,
                No *no) {
  if (matriz == NULL || no == NULL) {
    printf("Erro: Matriz ou nó é nulo\n");
    return;
  }
  if (linha < 0 || linha >= MAX_ALTURA || coluna < 0 || coluna >= MAX_LARGURA) {
    printf("Erro: Linha ou coluna fora dos limites da matriz\n");
    return;
  }

  char texto[30];

  if (no->left == NULL && no->right == NULL) {
    sprintf(texto, "[%s|%d]", ascii[(int)no->c], no->freq);
  } else {
    sprintf(texto, "[*|%d]", no->freq);
  }

  int tamanho = strlen(texto);

  for (int i = 0; i < tamanho; i++) {
    if (coluna + i >= 0 && coluna + i < MAX_LARGURA) {
      matriz[linha][coluna + i] = texto[i];
    }
  }
}

void montarArvore(No *raiz, char matriz[MAX_ALTURA][MAX_LARGURA], int linha,
                  int coluna, int espaco) {
  if (raiz == NULL)
    return;

  escreverNo(matriz, linha, coluna, raiz);

  if (raiz->left != NULL) {
    matriz[linha + 1][coluna - espaco / 2] = '/';
    montarArvore(raiz->left, matriz, linha + 2, coluna - espaco / 2, espaco / 2);
  }

  if (raiz->right != NULL) {
    matriz[linha + 1][coluna + espaco / 2] = '\\';
    montarArvore(raiz->right, matriz, linha + 2, coluna + espaco / 2, espaco / 2);
  }
}

void imprimirArvore(No *raiz) {
  if (raiz == NULL) {
    printf("Erro: Raiz é nula\n");
    return;
  }

  char matriz[MAX_ALTURA][MAX_LARGURA];

  for (int i = 0; i < MAX_ALTURA; i++) {
    for (int j = 0; j < MAX_LARGURA; j++) {
      matriz[i][j] = ' ';
    }
  }

  int h = altura(raiz);

  int colunaInicial = MAX_LARGURA / 4;
  int espacoInicial = MAX_LARGURA / 4;

  montarArvore(raiz, matriz, 0, colunaInicial, espacoInicial);

  for (int i = 0; i < h * 2; i++) {
    for (int j = 0; j < MAX_LARGURA; j++) {
      printf("%c", matriz[i][j]);
    }
    printf("\n");
  }
}
