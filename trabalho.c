// Trabalho desenvolvido por: 
// Artur Filardi Victoriano
// Ernandes José Vieira Filho

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PONTOS 20

// Inicializa todas as posicoes de uma matriz[20][20] como -1
void inicializa_matriz(int matriz[PONTOS][PONTOS]) {
    for (int linha = 0; linha < PONTOS; linha++) {
        for (int coluna = 0; coluna < PONTOS; coluna++) {
            matriz[linha][coluna] = -1;
        }
    }
}

// Inicializa todas as posicoes de um vetor[20] como -1
void inicializa_vetor(int vetor[PONTOS]) {
    for (int pos = 0; pos < PONTOS; pos++) {
        vetor[pos] = -1;
    }
}

// Le um arquivo e preenche uma matriz[20][20] com os dados encontrados
int carrega_arquivo(FILE *arquivo, int matriz[PONTOS][PONTOS]) { 
    int ponto1, ponto2, valor, resultado;
    
    resultado = fscanf(arquivo, "%d,%d,%d", &ponto1, &ponto2, &valor);
    
    if (resultado == 3) {
        matriz[ponto1][ponto2] = valor;
        carrega_arquivo(arquivo, matriz);
    }

    return 0;
}

// Realiza os calculos necessarios para encontrar o custo de viajar de um ponto a outro
int calcula_custo(int criminalidade, int transito, int distancia) {
    if (criminalidade == -1 || transito == -1 || distancia == -1) { // Retorna -1 caso algum dos pontos não exista
        return -1;
    }

    return distancia * distancia + transito + 2 * criminalidade; // Retorna o valor do custo -> distancia² + transito + 2*criminalidade
}

// Preenche todas as posicoes da matriz custo
int constroi_matriz_custo(int criminalidade[PONTOS][PONTOS], int transito[PONTOS][PONTOS], int distancia[PONTOS][PONTOS], int custo[PONTOS][PONTOS]) {
    for (int linha = 0; linha < PONTOS; linha++) {
        for (int coluna = 0; coluna < PONTOS; coluna++) {
            custo[linha][coluna] = calcula_custo(criminalidade[linha][coluna], transito[linha][coluna], distancia[linha][coluna]);
        }
    }
}

// Percorre um vetor e retorna a posicao do menor valor
int encontra_pos_menor_valor(int vetor[PONTOS]) {
    int pos_menor_valor = -1, menor_valor = -1;

    for (int pos = 0; pos < PONTOS; pos++) {
        if (vetor[pos] != -1) {
            if (vetor[pos] < menor_valor || menor_valor == -1) {
                menor_valor = vetor[pos];
                pos_menor_valor = pos;    
            }
        }
    }

    return pos_menor_valor;
}

int encontra_menor_caminho(int custos[PONTOS][PONTOS], int rota[PONTOS]) {
    int comeco = -1;
    for (int pos = 0; pos < PONTOS; pos++) {
        if (rota[pos] == -1) {
            comeco = pos;
            break;
        }
    }

    for (int pos = comeco; pos < PONTOS; pos++) {
        
    }
}

int main() {
    int ponto_inicial, ponto_final; // Declarando as variaveis ponto_inical e ponto_final
    int rota_aux[PONTOS], menor_rota[PONTOS];

    int criminalidade[PONTOS][PONTOS]; // Declarando a matriz criminalidade
    int transito[PONTOS][PONTOS]; // Declarando a matriz transito
    int distancia[PONTOS][PONTOS]; // Declarando a matriz distancia
    int custo[PONTOS][PONTOS]; // Declarando a matriz custo

    inicializa_vetor(rota_aux); // Inicializando todas as casas do vetor rota_aux como -1
    inicializa_vetor(menor_rota); // Inicializando todas as casas do vetor menor_rota como -1

    inicializa_matriz(criminalidade); // Inicializando todas as casas da matriz criminalidade como -1
    inicializa_matriz(transito); // Inicializando todas as casas da matriz transito como -1
    inicializa_matriz(distancia); // Inicializando todas as casas da matriz distancia como -1

    FILE *arq_criminalidade = fopen("criminalidade.txt", "r"); // Abrindo o arquivo de criminalidade
    if (arq_criminalidade == NULL) { // Indentificando erro na leitura do arquivo criminalidade
        printf("Erro ao ler aquivo de criminalidade");
        return -1;
    }
    carrega_arquivo(arq_criminalidade, criminalidade); // Carrengando os dados do arquivo criminalidade na respectiva matriz
    fclose(arq_criminalidade); // Fecha o arquivo criminalidade

    FILE *arq_transito = fopen("transito.txt", "r"); // Abrindo o arquivo de transito
    if (arq_transito == NULL) { // Indentificando erro na leitura do arquivo transito
        printf("Erro ao ler aquivo de transito");
        return -1;
    }
    carrega_arquivo(arq_transito, transito); // Carrengando os dados do arquivo transito na respectiva matriz
    fclose(arq_transito); // Fecha o arquivo transito

    FILE *arq_distancia = fopen("distancia.txt", "r"); // Abrindo o arquivo de distancia
    if (arq_distancia == NULL) { // Indentificando erro na leitura do arquivo distancia
        printf("Erro ao ler aquivo de distancia");
        return -1;
    }
    carrega_arquivo(arq_distancia, distancia); // Carrengando os dados do arquivo distancia na respectiva matriz
    fclose(arq_distancia); // Fecha o arquivo distancia

    constroi_matriz_custo(criminalidade, transito, distancia, custo); // Monta a matriz de custos com base nas outras 3 matrizes

    // Lendo o ponto inicial; A leitura e repetida ate que seja inserido um numero valido
    do {
        printf("Insira o ponto inicial: ");
        scanf("%d", &ponto_inicial);
    } while (ponto_inicial < 0 || ponto_inicial > PONTOS - 1);

    // Lendo o ponto final; A leitura e repetida ate que seja inserido um numero valido
    do {
        printf("Insira o ponto final: ");
        scanf("%d", &ponto_final);
    } while (ponto_final < 0 || ponto_final > PONTOS - 1);

    encontra_menor_caminho(custo, rota_aux);

    

    return 0;
}
