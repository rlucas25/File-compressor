#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <heap.h>
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

bool ehMenor_Huffman(void *pA, void *pB)
{
    No *noA = (No *)pA;
    No *noB = (No *)pB;
    return (noA->freq <= noB->freq);
}

void frequenciahuffman(const char *nomeArquivo, int *tabelaFrequencias)
{

    // Zerar a frequencia da tabela por garantia que vai começar de 0
    for (int i = 0; i < 256; i++)
    {
        tabelaFrequencias[i] = 0;
    }
    int byteLido;

    // rb é para ler o arquivo em binário
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    // var para receber a letra
    int byteLido;

    // Coloca a primeira letra no bytelido para quando entrar noo while não ser lixo
    byteLido = fgetc(arquivo);

    // Mantem rodando até ler/chegar no EOF fim do arquivo/end of file
    while (byteLido != EOF)
    {
        // Procura onde ele está na tabela por meio de ASCII e depois adiciona 1 na tabela se achar repetido
        tabelaFrequencias[byteLido]++;

        // byteLido lê a próxima letra
        byteLido = fgetc(arquivo);
    }
    fclose(arquivo);
}

No *criaNo(unsigned char c, int freq)
{

    No *novo = malloc(sizeof(No));
    novo->c = c;
    novo->freq = freq;
    novo->left = NULL;
    novo->right = NULL;

    return novo;
}
// Recebe o arquivo e transforma em estrutura de arvore huffman
Arvore *Huffman(const char *arquivo)
{
    // três ponteiros para o no como aux
    No *x, *y, *z;

    // cria estrutura heap com tamanho tabela asc
    Heap *heap = criarHeap(256);

    // cria a estrutura da arvore
    Arvore *arvore = malloc(sizeof(Arvore));

    // tabela de frequencia para ela
    arvore->tabelaFrequencias = malloc(256 * sizeof(int));

    // tamanho da tabela
    arvore->tamanhoTabela = 256;

    // atribuir a frequencia na tabela de frequencia
    frequenciahuffman(arquivo, arvore->tabelaFrequencias);

    // Criar o heap
    for (int i = 0; i < heap->capacidade; i++)
    {

        // Só cria nó se a letra realmente apareceu no texto
        if (arvore->tabelaFrequencias[i] > 0)
        {
            // O próprio índice 'i' é a letra na tabela ASCII!
            z = criaNo((unsigned char)i, arvore->tabelaFrequencias[i]);
            Insere(heap, z, ehMenor_Huffman);
        }
    }
    // pegar o heap e retirar
    while (heap->tamanho > 1)
    {
        // aux vai receber o retira minimo, vai pegar o menor nó
        x = RetiraMinimo(heap, ehMenor_Huffman);

        // aux vai receber o retira minimo, vai pegar o segundo menor nó
        y = RetiraMinimo(heap, ehMenor_Huffman);

        // cria um nó $ é o caractere com maior valor

        //  cria um nó com a frequencia dos filhos e vai para o proxim
        z = criaNo('$', x->freq + y->freq);

        z->left = x;
        z->right = y;

        z->freq = x->freq + y->freq;

        Insere(heap, z, ehMenor_Huffman);
    }

    //  Após so ter um nó esta sendo usado este raiz
    arvore->raiz = RetiraMinimo(heap, ehMenor_Huffman);
    free(heap->array);
    free(heap);
    return arvore;
}
void gerarDicionario(No *raiz, char dicionario[256][256])
{
}

/*
void concatenarCodigo(char *texto, char codigos[255][255], char resultados[])
    {
    // começa o vetor com \0, para o strcat conseguir de onde concatenar
    resultados[0] = '\0';
    for (int i; texto[i] != '\0'; i++)
    {
        // concatena os codigos um atras do outro para encaixar nos bytes.
        strcat(resultados, codigos[(unsigned char)texto[i]]);
    }
}
*/

void codificar(FILE *entrada, FILE *saida, char codigos[255][255])
{
    // o byte que foi lido
    int byteLido;

    // ele salva 8 bits + o /0
    char texto[9];

    // indice para o vetor texto
    int j = 0;

    // bruxaria ele volta para o ponteiro do file para o começo do arquivo, é nativa da biblioteca
    rewind(entrada);

    // roda enquanto não chega no final do arquivo EOF End of file
    while ((byteLido = fgetc(entrada)) != EOF)
    {
        // o bit é o encontrado no código de acordo com o byte lido
        char *bit = codigos[(unsigned char)byteLido];

        // Ele roda até o final da frase/mensagem
        for (int i = 0; bit[i] != '\0'; i++)
        {
            // coloca os bits no vetor texto
            texto[j] = bit[i];

            // vai para o proximo para colocar o novo bit
            j++;

            // até ter 8 bits
            if (j == 8)
            {
                texto[8] = '\0';
                unsigned char byteConvertido = (unsigned char)strtol(texto, NULL, 2);
                fputc(byteConvertido, saida);
                j = 0;
            }
        }
    }
    if (j > 0)
    {
        while (j < 8)
        {
            texto[j] = '0';
            j++;
        }
        texto[8] = '\0';
        unsigned char byteConvertido = (unsigned char)strtol(texto, NULL, 2);
        fputc(byteConvertido, saida);
    }
}

void decodificar(No *raiz, FILE *entrada, FILE *saida)
{
    No *atual = raiz;
    unsigned char byte;

    // Realize enquanto for possível ler bytes, lê de byte em byte
    while (fread(&byte, 1, 1, entrada) == 1)
    {
        // Lê todo o byte, i é 7, pois o tamanho de um byte é 8
        for (int i = 7; i >= 0; i--)
        {
            // Se o bit for 0, vai para a esquerda
            // Usamos a operação BITWISE AND (&) para testar se o bit 'i' é zero ou um
            // int bit = (byte >> i) & 1;
            // 0 0 0 0 0 0 0 X  <- Este é o byte após o >> (O 'X' é o bit que você quer isolar)
            // &
            // 0 0 0 0 0 0 0 1  <- Esta é a máscara do "1"
            // -----------------
            // 0 0 0 0 0 0 0 X  <- O Resultado
            int bit = (byte >> i) & 1;

            // Se o bit for 0, vai para esquerda
            if (bit == 0)
            {
                atual = atual->left;
            }
            // Se o bit for 1, vai para a direita
            else
            {
                atual = atual->right;
            }

            // Ao encontrar um nó folha, insere a tradução do bit no arq saida
            if (atual->left == NULL && atual->right == NULL)
            {
                fputc(atual->c, saida);
                // Retorna a raiz, repetindo o processo
                atual = raiz;
            }
        }
    }
}

void liberarHuffman(No *raiz)
{
    if (raiz == NULL)
        return;

    // liberar raiz esquerda
    liberarHuffman(raiz->left);

    // liberar raiz direita
    liberarHuffman(raiz->right);

    // liberar a raiz
    free(raiz);
}
