#ifndef HEAP_H
#define HEAP_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Heap
{
    void **array;
    int tamanho;
    int capacidade;

} Heap;

Heap *criarHeap(int capacidade);

void MinHeapify(Heap *heap, int i, bool (*ehMenor)(void *, void *));

void Insere(Heap *heap, void *no, bool (*ehMenor)(void *, void *));

void *RetiraMinimo(Heap *heap, bool (*ehMenor)(void *, void *));

void liberarHeap(Heap *heap, void (*liberarChave)(void *));

#endif // HEAP_H
