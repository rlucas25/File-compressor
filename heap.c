#include "heap.h"

Heap *criarHeap(int capacidade)
{
    Heap *heap = (Heap *)malloc(sizeof(Heap));
    heap->array = malloc(capacidade * sizeof(void *));
    if (!heap)
    {
        return NULL;
    }
    if (!heap->array)
    {
        free(heap);
        return NULL;
    }
    heap->tamanho = 0;
    heap->capacidade = capacidade;
    return heap;
}
// Arrumar a comparaçãeap->array[i]->frequencia < heap->array[i\2]->frequenciao e usar a função de comparação passada como parâmetro
void MinHeapify(Heap *heap, int i, bool (*ehMenor)(void *, void *))
{
    if (!heap)
        return;

    int esq, dir, menor;

    esq = 2 * i;
    dir = 2 * i + 1;

    if (esq < heap->tamanho && ehMenor(heap->array[esq], heap->array[i]))
    {
        menor = esq;
    }
    else
    {
        menor = i;
    }
    if (dir < heap->tamanho && ehMenor(heap->array[dir], heap->array[menor]))
    {
        menor = dir;
    }
    if (menor != i)
    {
        void *aux = heap->array[i];
        heap->array[i] = heap->array[menor];
        heap->array[menor] = aux;
        MinHeapify(heap, menor, ehMenor);
    }
}

void Insere(Heap *heap, void *no, bool (*ehMenor)(void *, void *))
{

    if (heap->tamanho == heap->capacidade)
    {
        printf("Erro: Heap cheio");
        return;
    }

    heap->tamanho++;
    int i = heap->tamanho - 1;
    heap->array[i] = no;
    while (i > 0 && ehMenor(heap->array[i], heap->array[i / 2]))
    {
        void *temp = heap->array[i];
        heap->array[i] = heap->array[i / 2];
        heap->array[i / 2] = temp;
        i = i / 2;
    }
}

void *RetiraMinimo(Heap *heap, bool (*ehMenor)(void *, void *))
{
    if (!heap)
        return NULL;
    if (heap->tamanho < 1)
    {
        printf("Erro: Underflow");
        return NULL;
    }

    void *minimo = heap->array[0];
    heap->array[0] = heap->array[heap->tamanho - 1];
    heap->tamanho--;
    MinHeapify(heap, 0, ehMenor);

    return minimo;
}

void liberarHeap(Heap *heap, void (*liberarChave)(void *))
{
    if (!heap)
        return;

    for (int i = 0; i < heap->tamanho; i++)
    {
        liberarChave(heap->array[i]);
    }

    free(heap->array);
    free(heap);
}