#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main()
{
    int quantidade = 100000;
    FILE *arquivo;

    arquivo = fopen("teste.txt", "w");

    if (arquivo == NULL)
    {
        printf("Erro ao criar o arquivo!\n");
        return 1;
    }

    srand(time(NULL));
    for (int i = 0; i < quantidade; i++)
    {
        char caractere = 'A' + (rand() % 26);
        fprintf(arquivo, "%c", caractere);
    }

    fclose(arquivo);
    printf("Arquivo teste.txt criado com %d caracteres.\n", quantidade);

    return 0;
}
