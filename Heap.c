/* 
    ALUNOS
Dennis Francisco Guimarães de Oliveira Baracho
Heitor Santos Maia
Lucas Rebouças Almeida
Marcos Vinícius Pimentel Gomes

*/

#include "Heap.h"

Heap *criarHeap(int capacidade)
{
    if (capacidade <= 0)
    {
        printf("Erro: Capacidade do heap deve ser maior que zero\n");
        return NULL;
    }

    Heap *heap = (Heap *)malloc(sizeof(Heap));
    
    if (!heap)
    {
        return NULL;
    }

    heap->array = malloc(capacidade * sizeof(void *));
    
    if (!heap->array)
    {
        free(heap);
        return NULL;
    
    }
    heap->tamanho = 0;
    heap->capacidade = capacidade;
    return heap;
}

void minHeapify(Heap *heap, int i, bool (*ehMenor)(void *, void *))
{
    if (!heap || !heap->array || i < 0 || i >= heap->tamanho)
        return;

    int esq, dir, menor;

    esq = 2 * i + 1;
    dir = 2 * i + 2;

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
        minHeapify(heap, menor, ehMenor);
    }
}

void inserir(Heap *heap, void *no, bool (*ehMenor)(void *, void *))
{

    if (heap->tamanho == heap->capacidade)
    {
        printf("Erro: Heap cheio");
        return;
    }
    if (!heap || !heap->array || !no)
    {
        printf("Erro: Heap, array ou no do heap é nulo\n");
        return;
    }

    heap->tamanho++;
    int i = heap->tamanho - 1;
    heap->array[i] = no;
    while (i > 0 && ehMenor(heap->array[i], heap->array[(i - 1) / 2]))
    {
        void *temp = heap->array[i];
        heap->array[i] = heap->array[(i - 1) / 2];
        heap->array[(i - 1) / 2] = temp;
        i = (i - 1) / 2;
    }
}

void *retiraMinimo(Heap *heap, bool (*ehMenor)(void *, void *))
{
    if (!heap)
        return NULL;
    if (heap->tamanho < 1)
    {
        printf("Erro: Tamanho do heap invalido\n");
        return NULL;
    }

    void *minimo = heap->array[0];
    heap->array[0] = heap->array[heap->tamanho - 1];
    heap->tamanho--;

    minHeapify(heap, 0, ehMenor);
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