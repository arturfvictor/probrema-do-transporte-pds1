// Trabalho desenvolvido por: 
// Artur Filardi Victoriano
// Ernandes José Vieira Filho

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PONTOS 20

// Inicializa todas as posicoes de uma matriz[20][20] como o valor selecionado
void inicializa_matriz(int matriz[PONTOS][PONTOS], int valor) {
    for (int linha = 0; linha < PONTOS; linha++) {
        for (int coluna = 0; coluna < PONTOS; coluna++) {
            matriz[linha][coluna] = valor;
        }
    }
}

// Inicializa todas as posicoes de um vetor[20] como o valor selecionado
void inicializa_vetor(int vetor[PONTOS], int valor) {
    for (int pos = 0; pos < PONTOS; pos++) {
        vetor[pos] = valor;
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

// Retorna a posicao de um numero em um vetor ou -1 caso não seja encontrado
int encontra_no_vetor(int vetor[PONTOS], int valor) {
    for (int pos = 0; pos < PONTOS; pos++) {
        if (vetor[pos] == valor) {
            return pos;
        }
    }

    return -1;
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

void printa_vetor(int vetor[PONTOS]) {
    printf("[ ");
    for (int pos = 0; pos < PONTOS; pos++) {
        if (vetor[pos] != -1) {
            printf("%d ", vetor[pos]);
        } else {
            printf("? ");
        }
    }
    printf("]\n");
}

void gera_ordem_de_custos(int vetor[PONTOS], int vetor_ordenado[PONTOS]) {
    inicializa_vetor(vetor_ordenado, -1);

    for (int pos = 0; pos < PONTOS; pos++) {
        for (int pos2 = 0; pos2 < PONTOS; pos2++) {
            if (vetor_ordenado[pos] == -1) {
                if (encontra_no_vetor(vetor_ordenado, pos2) == -1) {
                    vetor_ordenado[pos] = pos2;
                }
            } else {
                if (vetor[pos2] < vetor[vetor_ordenado[pos]]) {
                    if (encontra_no_vetor(vetor_ordenado, pos2) == -1) {
                        vetor_ordenado[pos] = pos2;
                    }
                }
            }
        }
    }
}

void copia_vetor(int vetor_origem[PONTOS], int vetor_destino[PONTOS]) {
    for (int pos = 0; pos < PONTOS; pos++) {
        vetor_destino[pos] = vetor_origem[pos];
    }
}

void preenche_matriz_rotas(int rota[PONTOS], int rotas_possiveis[PONTOS][PONTOS]) {
    for (int i = 0; i < PONTOS; i++) {
        copia_vetor(rota, rotas_possiveis[i]);
    }
}

int encontra_menor_caminho(int custos[PONTOS][PONTOS], int rota[PONTOS]) {
    int comeco = 0, final = PONTOS - 1;
    int custos_rotas[PONTOS], rotas_possiveis[PONTOS][PONTOS];
    
    while(rota[comeco] != -1) {
        comeco++;
    }
    
    while (rota[final] != -1) {
        final--;
    }

    preenche_matriz_rotas(rota, rotas_possiveis);

    for (int i = 0; i < PONTOS; i++) {
        custos_rotas[i] = 0;

        if (encontra_no_vetor(rota, i) != -1) {
            continue;
        }

        rotas_possiveis[i][comeco] = i;

        for (int pos = comeco + 1; pos <= final; pos++) {
            int custos_ordenados[PONTOS];
            int ponto_anterior = rotas_possiveis[i][pos-1];

            gera_ordem_de_custos(custos[ponto_anterior], custos_ordenados);

            for (int cont = 0; cont < PONTOS; cont++) {
                int custo_atual = custos[ponto_anterior][custos_ordenados[cont]];

                if (custo_atual != -1 && encontra_no_vetor(rotas_possiveis[i], custos_ordenados[cont]) == -1) {
                    rotas_possiveis[i][pos] = custos_ordenados[cont];
                    custos_rotas[i] += custo_atual;
                    break;
                }
            }
        }
    }

    int custos_totais_ordenados[PONTOS];
    gera_ordem_de_custos(custos_rotas, custos_totais_ordenados);

    for (int pos = 0; pos < PONTOS; pos++) {
        if (custos_rotas[custos_totais_ordenados[pos]] != 0) {
            printf("Melhor Rota: ");
            printa_vetor(rotas_possiveis[custos_totais_ordenados[pos]]);
            printf("Custo: %d\n", custos_rotas[custos_totais_ordenados[pos]]);
            break;
        }
    }

    return 0;
}



int main() {
    int ponto_inicial, ponto_final; // Declarando as variaveis ponto_inical e ponto_final
    int rota_aux[PONTOS], menor_rota[PONTOS];

    int criminalidade[PONTOS][PONTOS]; // Declarando a matriz criminalidade
    int transito[PONTOS][PONTOS]; // Declarando a matriz transito
    int distancia[PONTOS][PONTOS]; // Declarando a matriz distancia
    int custo[PONTOS][PONTOS]; // Declarando a matriz custo

    inicializa_vetor(rota_aux, -1); // Inicializando todas as casas do vetor rota_aux como -1
    inicializa_vetor(menor_rota, -1); // Inicializando todas as casas do vetor menor_rota como -1

    inicializa_matriz(criminalidade, -1); // Inicializando todas as casas da matriz criminalidade como -1
    inicializa_matriz(transito, -1); // Inicializando todas as casas da matriz transito como -1
    inicializa_matriz(distancia, -1); // Inicializando todas as casas da matriz distancia como -1

    FILE *arq_criminalidade = fopen("criminalidade.txt", "r"); // Abrindo o arquivo de criminalidade
    FILE *arq_transito = fopen("transito.txt", "r"); // Abrindo o arquivo de transito
    FILE *arq_distancia = fopen("distancia.txt", "r"); // Abrindo o arquivo de distancia

    if (arq_criminalidade == NULL) { // Indentificando erro na leitura do arquivo criminalidade
        printf("Erro ao ler aquivo de criminalidade");
        return -1;
    }
    if (arq_transito == NULL) { // Indentificando erro na leitura do arquivo transito
        printf("Erro ao ler aquivo de transito");
        return -1;
    }
    if (arq_distancia == NULL) { // Indentificando erro na leitura do arquivo distancia
        printf("Erro ao ler aquivo de distancia");
        return -1;
    }

    carrega_arquivo(arq_criminalidade, criminalidade); // Carrengando os dados do arquivo criminalidade na respectiva matriz
    carrega_arquivo(arq_transito, transito); // Carrengando os dados do arquivo transito na respectiva matriz
    carrega_arquivo(arq_distancia, distancia); // Carrengando os dados do arquivo distancia na respectiva matriz

    fclose(arq_criminalidade); // Fecha o arquivo criminalidade
    fclose(arq_transito); // Fecha o arquivo transito
    fclose(arq_distancia); // Fecha o arquivo distancia

    constroi_matriz_custo(criminalidade, transito, distancia, custo); // Monta a matriz de custos com base nas outras 3 matrizes

    // Lendo o ponto inicial; A leitura e repetida ate que seja inserido um numero valido
    do {
        printf("Insira o ponto inicial: ");
        scanf("%d", &rota_aux[0]);
    } while (rota_aux[0] < 0 || rota_aux[0] > PONTOS - 1);

    // Lendo o ponto final; A leitura e repetida ate que seja inserido um numero valido
    do {
        printf("Insira o ponto final: ");
        scanf("%d", &rota_aux[PONTOS-1]);
    } while (rota_aux[PONTOS-1] < 0 || rota_aux[PONTOS-1] > PONTOS - 1);

    printf("Rota Base: ");
    printa_vetor(rota_aux);

    encontra_menor_caminho(custo, rota_aux);

    return 0;
}
