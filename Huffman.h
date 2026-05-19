#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Heap.h"

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
Arvore *criarArvoreHuffman(int *tabelaFrequencias);
void gerarDicionario(No *raiz, char dicionario[256][256], char caminho[256], int indice);
void codificar(FILE *entrada, FILE *saida, char codigos[256][256]);
void decodificar(No *raiz, FILE *entrada, FILE *saida);
void liberarHuffman(No *raiz);
void imprimeHuffman(No *raiz, char *prefixo, int ehUltimo);

#endif // HUFFMAN_H
