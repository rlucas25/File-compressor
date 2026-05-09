#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct NoHuffman
{
    unsigned char c;
    int freq;
} No;

bool ehMenor_Huffman(void *pA, void *pB)
{
    No *noA = (No *)pA;
    No *noB = (No *)pB;
    return (noA->freq <= noB->freq);
}

void liberarHuffman(No *raiz){
    free(raiz);
}



