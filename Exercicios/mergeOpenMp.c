#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
void merge(int vetor[], int comeco, int meio, int fim);
void mergeSort(int vetor[], int comeco, int fim);
void Listar(int vetor[], int tamanho);
void PreencheVetor(int vetor[],int tamanho);
int main(int argc, char** argv){
    clock_t inicio,fim;
    int TAMANHO_TOTAL = 10;
    printf("paralelo\n" );
    int num_threads;
    int *vetor =(int *) malloc(TAMANHO_TOTAL * sizeof(int));
    #pragma omp parallel
    {
        #pragma omp master
        {
            num_threads = omp_get_num_threads();
        }
    }
    PreencheVetor(vetor,TAMANHO_TOTAL);
    Listar(vetor,TAMANHO_TOTAL);
    printf("\n");
    inicio = clock();
    mergeSort(vetor, 0,TAMANHO_TOTAL);
    printf("valor do tempo e %.4f\n",(clock()-inicio)*1000 / (double)CLOCKS_PER_SEC);
    Listar(vetor,TAMANHO_TOTAL);
    return 0;
}


void PreencheVetor(int vetor[],int tamanho){
    srand(time(NULL));
    for(int i = 0; i < tamanho; i++)
        {
            vetor[i] = rand()%100;
        }
}
void merge(int vetor[], int comeco, int meio, int fim) {
    int com1 = comeco, com2 = meio+1, comAux = 0, tam = fim-comeco+1;
    int *vetAux;
    vetAux = (int*)malloc(tam * sizeof(int));

    while(com1 <= meio && com2 <= fim){
        if(vetor[com1] < vetor[com2]) {
            vetAux[comAux] = vetor[com1];
            com1++;
        } else {
            vetAux[comAux] = vetor[com2];
            com2++;
        }
        comAux++;
    }

    while(com1 <= meio){  //Caso ainda haja elementos na primeira metade
        vetAux[comAux] = vetor[com1];
        comAux++;
        com1++;
    }

    while(com2 <= fim) {   //Caso ainda haja elementos na segunda metade
        vetAux[comAux] = vetor[com2];
        comAux++;
        com2++;
    }
    #pragma parallel for
    for(comAux = comeco; comAux <= fim; comAux++){    //Move os elementos de volta para o vetor original
        vetor[comAux] = vetAux[comAux-comeco];
    }

}

void mergeSort(int vetor[], int comeco, int fim){
    if (comeco < fim) {
        int meio = (fim+comeco)/2;
        #pragma omp task firstprivate (vetor, comeco, meio)
            mergeSort(vetor, comeco, meio);
        #pragma omp task firstprivate (vetor, meio, fim)
            mergeSort(vetor, meio+1, fim);
        #pragma omp taskwait
        merge(vetor, comeco, meio, fim);
    }
}

void Listar(int vetor[], int tamanho)
{
   
    for (int i = 0; i < tamanho; i++)
    {
        printf("%i\n", vetor[i]);
    }
}