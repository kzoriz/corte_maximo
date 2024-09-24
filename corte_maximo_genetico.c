#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define VERTICES 12  // número de vértices no grafo
#define POPULACAO 1000  // tamanho da população
#define GERACOES 10000  // número de gerações
#define TAXA_MUTACAO 0.05  // taxa de mutação (10%)

int grafo[VERTICES][VERTICES] = {
    {0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0},
    {1, 0, 1, 0, 1, 0, 1, 1, 0, 0, 1, 0},
    {0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 0},
    {0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1},
    {1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0},
    {0, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 0},
    {0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1},
    {1, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 1},
    {0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1},
    {0, 1, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1},
    {0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0}
};

// int grafo[VERTICES][VERTICES] = {
//     {0, 1, 0, 1, 0},
//     {1, 0, 1, 0, 1},
//     {0, 1, 0, 1, 1},
//     {1, 0, 1, 0, 1},
//     {0, 1, 1, 1, 0}
//
// };

// Função para avaliar a aptidão (fitness) de um indivíduo
int avaliarFitness(int* individuo) {
    int fitness = 0;
    for (int i = 0; i < VERTICES; i++) {
        for (int j = i + 1; j < VERTICES; j++) {
            if (grafo[i][j] && individuo[i] != individuo[j]) {
                fitness++;
            }
        }
    }
    return fitness;
}

// Função para gerar um indivíduo aleatório
void gerarIndividuo(int* individuo) {
    for (int i = 0; i < VERTICES; i++) {
        individuo[i] = rand() % 2;  // Atribui 0 ou 1 aleatoriamente
    }
}

// Função para gerar a população inicial
void gerarPopulacao(int populacao[POPULACAO][VERTICES]) {
    for (int i = 0; i < POPULACAO; i++) {
        gerarIndividuo(populacao[i]);
    }
}

// Função de seleção usando torneio
int selecionarIndividuo(int populacao[POPULACAO][VERTICES], int fitnessPopulacao[POPULACAO]) {
    int i1 = rand() % POPULACAO;
    int i2 = rand() % POPULACAO;
    return (fitnessPopulacao[i1] > fitnessPopulacao[i2]) ? i1 : i2;
}

// Função de cruzamento (crossover)
void cruzar(int* pai1, int* pai2, int* filho) {
    int pontoDeCorte = rand() % VERTICES;
    for (int i = 0; i < VERTICES; i++) {
        filho[i] = (i < pontoDeCorte) ? pai1[i] : pai2[i];
    }
}

// Função de mutação
void mutar(int* individuo) {
    for (int i = 0; i < VERTICES; i++) {
        if ((float)rand() / RAND_MAX < TAXA_MUTACAO) {
            individuo[i] = 1 - individuo[i];  // Inverte o gene (0 -> 1 ou 1 -> 0)
        }
    }
}

// Função para encontrar o melhor indivíduo da população
int encontrarMelhorIndividuo(int fitnessPopulacao[POPULACAO]) {
    int melhor = 0;
    for (int i = 1; i < POPULACAO; i++) {
        if (fitnessPopulacao[i] > fitnessPopulacao[melhor]) {
            melhor = i;
        }
    }
    return melhor;
}

int main() {
    srand(time(NULL));

    int populacao[POPULACAO][VERTICES];
    int novaPopulacao[POPULACAO][VERTICES];
    int fitnessPopulacao[POPULACAO];
    int inicio, final;

    inicio = clock();
    // Gera a população inicial
    gerarPopulacao(populacao);

    // Avalia a aptidão inicial da população
    for (int i = 0; i < POPULACAO; i++) {
        fitnessPopulacao[i] = avaliarFitness(populacao[i]);
    }

    for (int geracao = 0; geracao < GERACOES; geracao++) {
        // Gera a nova população
        for (int i = 0; i < POPULACAO; i++) {
            // Seleção
            int pai1 = selecionarIndividuo(populacao, fitnessPopulacao);
            int pai2 = selecionarIndividuo(populacao, fitnessPopulacao);

            // Cruzamento
            cruzar(populacao[pai1], populacao[pai2], novaPopulacao[i]);

            // Mutação
            mutar(novaPopulacao[i]);
        }

        // Substitui a população antiga pela nova
        for (int i = 0; i < POPULACAO; i++) {
            for (int j = 0; j < VERTICES; j++) {
                populacao[i][j] = novaPopulacao[i][j];
            }
            fitnessPopulacao[i] = avaliarFitness(populacao[i]);
        }

        // Mostra o melhor indivíduo a cada geração
        int melhor = encontrarMelhorIndividuo(fitnessPopulacao);
        printf("Geração %d: Melhor fitness = %d\n", geracao, fitnessPopulacao[melhor]);
    }

    // Mostra o melhor indivíduo final
    int melhor = encontrarMelhorIndividuo(fitnessPopulacao);
    printf("Melhor solução encontrada: \n");
    for (int i = 0; i < VERTICES; i++) {
        printf("%d ", populacao[melhor][i]);
    }
    printf("\nCom fitness: %d\n", fitnessPopulacao[melhor]);

    // Mostra o corte no grafo final
    printf("Arestas cortadas no grafo:\n");
    for (int i = 0; i < VERTICES; i++) {
        for (int j = i + 1; j < VERTICES; j++) {
            if (grafo[i][j] && populacao[melhor][i] != populacao[melhor][j]) {
                printf("Aresta entre %d e %d\n", i, j);
            }
        }
    }
    final = clock();
    float tempo_execucao = ((double)(final - inicio)) / CLOCKS_PER_SEC;

    printf("Tempo de execução: %.4f segundos\n", tempo_execucao);
    return 0;
}
