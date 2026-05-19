typedef struct NoHuffman
{
    unsigned char c;
    int freq;
    struct NoHuffman *left;
    struct NoHuffman *right;
} No;

typedef struct ArvoreHuffman
{
    No *raiz;
    int *tabelaFrequencias;
    int tamanhoTabela;
} Arvore;

bool ehMenor_Huffman(void *pA, void *pB);
void frequenciahuffman(const char *nomeArquivo, int *tabelaFrequencias);
No *criaNo(unsigned char c, int freq);
Arvore *Huffman(const char *arquivo);
void gerarDicionario(No *raiz, char dicionario[256][256], char caminho[256], int indice);
void codificar(FILE *entrada, FILE *saida, char codigos[256][256]);
void decodificar(No *raiz, FILE *entrada, FILE *saida);
void liberarHuffman(No *raiz);
void imprimeHuffman(No *raiz);
Arvore *criarArvoreHuffman(int *tabelaFrequencias);