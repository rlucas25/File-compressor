#include "Huffman.h"
#include <string.h>

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
    // Cria uma tabela temporária vazia
    int tabelaTemporaria[256] = {0};

    frequenciahuffman(arquivo, tabelaTemporaria);

    return criarArvoreHuffman(tabelaTemporaria);
}
Arvore *criarArvoreHuffman(int *tabelaFrequencias)
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
    
    for (int i = 0; i < 256; i++)
    {
        arvore->tabelaFrequencias[i] = tabelaFrequencias[i];
    }
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
        // cria um nó com a frequencia dos filhos e vai para o proxim
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

void gerarDicionario(No *raiz, char dicionario[256][256], char caminho[256], int indice)
{
    if (raiz == NULL)
        return;

    if (raiz->left == NULL && raiz->right == NULL)
    {
        caminho[indice] = '\0';
        strcpy(dicionario[raiz->c], caminho);
        return;
    }

    caminho[indice] = '0';
    gerarDicionario(raiz->left, dicionario, caminho, indice + 1);

    caminho[indice] = '1';
    gerarDicionario(raiz->right, dicionario, caminho, indice + 1);
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

void codificar(FILE *entrada, FILE *saida, char codigos[256][256])
{
    // o byte que foi lido
    int byteLido;

    // byte que será montado bit por bit
    unsigned char byteSaida = 0;

    // quantidade de bits já colocados dentro do byteSaida
    int quantidadeBits = 0;

    // bruxaria ele volta para o ponteiro do file para o começo do arquivo, é nativa da biblioteca
    rewind(entrada);

    // roda enquanto não chega no final do arquivo EOF End of file
    while ((byteLido = fgetc(entrada)) != EOF)
    {
        // o bit é o encontrado no código de acordo com o byte lido
        char *codigo = codigos[(unsigned char)byteLido];

        // Ele roda até o final da frase/mensagem
        for (int i = 0; codigo[i] != '\0'; i++)
        {
            // desloca os bits para a esquerda, abrindo espaço para o novo bit
            byteSaida = byteSaida << 1;

            // se o bit do código for '1', coloca 1 na última posição
            if (codigo[i] == '1')
            {
                byteSaida = byteSaida | 1;
            }

            quantidadeBits++;

            // quando chegar em 8 bits, escreve o byte no arquivo
            if (quantidadeBits == 8)
            {
                fputc(byteSaida, saida);

                // reseta para montar o proximo byte
                byteSaida = 0;
                quantidadeBits = 0;
            }
        }
    }
    // se o arquivo acabou, mas o último byte não encheu os 8 bits
    if (quantidadeBits > 0)
    {
        // empurra os bits que já temos para a esquerda preenchendo com zeros na direita.
        byteSaida = byteSaida << (8 - quantidadeBits);

        // grava esse último byte incompleto (agora preenchido com zeros) no arquivo
        fputc(byteSaida, saida);
    }
}

void decodificar(No *raiz, FILE *entrada, FILE *saida)
{
    No *atual = raiz;
    unsigned char byte;

    // 1. Descobre o tamanho do texto original olhando para a raiz
    int totalLetras = raiz->freq;

    // 2. Cria o contador de letras
    int letrasImpressas = 0;

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
                // 3. Aumenta o contador e verifica a CONDIÇÃO DE PARADA
                letrasImpressas++;

                if (letrasImpressas == totalLetras)
                {
                    // Já imprimimos todas as letras! O resto do byte é lixo.
                    // O return encerra a função na hora.
                    return;
                }
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
extern char *strdup(const char *s);

void imprimeHuffman(No *raiz, char *prefixo, int ehUltimo) {
    if (raiz == NULL)
        return;

    printf("%s", prefixo);

    if (ehUltimo) {
        printf("└── ");
    } else {
        printf("├── ");
    }

    if (raiz->left == NULL && raiz->right == NULL) {
        printf("%d:'%c'\n", raiz->freq, raiz->c);
        return;
    } else {
        printf("%d\n", raiz->freq);
    }

    char *novoPrefixo = malloc(strlen(prefixo) + 10);

    if (ehUltimo) {
        sprintf(novoPrefixo, "%s    ", prefixo);
    } else {
        sprintf(novoPrefixo, "%s│   ", prefixo);
    }

    if (raiz->left != NULL) {
        imprimeHuffman(raiz->left, novoPrefixo, 0);
    }

    if (raiz->right != NULL) {
        imprimeHuffman(raiz->right, novoPrefixo, 1);
    }

    free(novoPrefixo);
}