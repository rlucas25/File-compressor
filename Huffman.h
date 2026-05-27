#ifndef HUFFMAN_H
#define HUFFMAN_H
#define MAX_ALTURA 50
#define MAX_LARGURA 300

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Heap.h"
extern const char *ascii[256];



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
Arvore *huffman(const char *arquivo);
Arvore *criarArvoreHuffman(int *tabelaFrequencias);

void gerarDicionario(No *raiz, char dicionario[256][256], char caminho[256], int indice);

void codificar(FILE *entrada, FILE *saida, char codigos[256][256]);
bool decodificar(No *raiz, FILE *entrada, FILE *saida);

void comprimir(Arvore *arvore, const char *extensao, const char *nomeEntrada, char *nomeSaida);
void descomprimir(Arvore *arvoreRecuperada, const char *nomeEntrada, char *nomeSaida);

void liberarArvore(Arvore *arvore);
void auxLiberarArvore(No *raiz);

int altura(No *raiz);
void escreverNo(char matriz[MAX_ALTURA][MAX_LARGURA], int linha, int coluna, No *no);
void imprimirArvore(No *raiz);
void montarArvore(No *raiz, char matriz[MAX_ALTURA][MAX_LARGURA],int linha, int coluna, int espaco);
#endif
