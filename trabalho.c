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

// Funcao recursiva que le um arquivo e preenche uma matriz[20][20] com os dados encontrados
int carrega_arquivo(FILE *arquivo, int matriz[PONTOS][PONTOS]) { 
    int ponto1, ponto2, valor, resultado; 
    
    resultado = fscanf(arquivo, "%d,%d,%d", &ponto1, &ponto2, &valor); // Lendo os pontos do arquivo e salvando nas variaveis ponto1, ponto2 e valor
    
    if (resultado == 3) { // Verificando se a leitura foi bem sucedida
        matriz[ponto1][ponto2] = valor; // Salvando o valor em matriz[ponto1][ponto2]
        carrega_arquivo(arquivo, matriz); // Chamando a funcao novamente para ler a proxima posicao
    }

    return 0;
}

// Funcao que verifica se temos todos os dados necessários para calcular o custo de um ponto a outro:
int calcula_custo(int criminalidade, int transito, int distancia) {
    if (criminalidade == -1 || transito == -1 || distancia == -1) { // Retorna -1 caso pelo menos um dos pontos não exista
        return -1;
    }

    return distancia + 3 * transito + criminalidade * criminalidade; // Retorna o valor do custo -> distancia + 3 * transito + criminalidade^2
}

// Preenche todas as posicoes da matriz custo com o resultado da execucao da funcao calcula_custo
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

    for (int pos = 0; pos < PONTOS; pos++) { // Itera por todas as posicoes do vetor
        if (vetor[pos] != -1) { // Verificando se um numero valido ocupa a posicao
            if (vetor[pos] < menor_valor || menor_valor == -1) { // Caso a variavel menor_valor ainda nao tenha sido preenchida ou o numero encontrado seja menor que o valor de menor_valor
                menor_valor = vetor[pos]; // Define menor_valor como o valor da posicao atual
                pos_menor_valor = pos; // Salva a posicao do menor valor na variavel pos_menor_valor
            }
        }
    }

    return pos_menor_valor;
}

// Mostra o vetor em tela -> [ X Y Z ]
void printa_vetor(int vetor[PONTOS]) {
    printf("[ ");
    for (int pos = 0; pos < PONTOS; pos++) {
        if (vetor[pos] != -1) { // Caso o numero que ocupa a posicao seja != -1 mostra o numero em tela
            printf("%d ", vetor[pos]);
        } else { // Caso o numero que ocupa a posicao seja == -1 mostra mostra uma ? em tela
            printf("? ");
        }
    }
    printf("]\n");
}

//Gera um vetor que contem as posicoes dos elementos de custo ordenados de maneira crescente
void gera_ordem_de_custos(int vetor[PONTOS], int vetor_ordenado[PONTOS]) {
    inicializa_vetor(vetor_ordenado, -1); //Inicializa todas as posicoes de vetor_ordenado como -1

    for (int pos = 0; pos < PONTOS; pos++) {
        for (int pos2 = 0; pos2 < PONTOS; pos2++) {
            if (vetor_ordenado[pos] == -1) { // Verifica se a posicao atual de vetor_ordenado ainda nao foi preenchida
                if (encontra_no_vetor(vetor_ordenado, pos2) == -1) { // Verifica se a posicao pos2 já esta presente em vetor_ordenado
                    vetor_ordenado[pos] = pos2; // Caso não esteja presente, salva a pos2 em vetor_ordenado[pos]
                }
            } else {
                if (vetor[pos2] < vetor[vetor_ordenado[pos]]) { // Compara os numeros vetor[pos2] com vetor[vetor_ordenado[pos]]
                    if (encontra_no_vetor(vetor_ordenado, pos2) == -1) { // Verifica se a posicao pos2 já esta presente em vetor_ordenado
                        vetor_ordenado[pos] = pos2; // Caso não esteja presente, salva a pos2 em vetor_ordenado[pos]
                    }
                }
            }
        }
    }
}

// Copia o conteudo de um vetor[20] para outro vetor[20]
void copia_vetor(int vetor_origem[PONTOS], int vetor_destino[PONTOS]) {
    for (int pos = 0; pos < PONTOS; pos++) {
        vetor_destino[pos] = vetor_origem[pos];
    }
}

// Preenche todas as linhas da matriz de rotas com a "rota base"
void preenche_matriz_rotas(int rota[PONTOS], int rotas_possiveis[PONTOS][PONTOS]) {
    for (int i = 0; i < PONTOS; i++) {
        copia_vetor(rota, rotas_possiveis[i]);
    }
}

// Algoritmo guloso que encontra o menor caminho entre 2 pontos passando por todos os 20 pontos
int encontra_menor_caminho(int custos[PONTOS][PONTOS], int rota[PONTOS]) {
    int comeco = 0, final = PONTOS - 1; // Declaracao das variaveis que demarcarao quantas casas da rota já foram preenchidas 
    int custos_rotas[PONTOS], rotas_possiveis[PONTOS][PONTOS]; // Declara um vetor que armazenara os custos de cada rota e uma matriz que armazenara as rotas testadas
    
    while(rota[comeco] != -1) { // Procurando a primeira posicao nao preenchida da rota
        comeco++;
    }
    
    while (rota[final] != -1) { // Procurando a ultima posicao nao preenchida da rota
        final--;
    }

    preenche_matriz_rotas(rota, rotas_possiveis); // Preenche todas as linhas da matriz de rotas possiveis com a rota base

    for (int i = 0; i < PONTOS; i++) {
        custos_rotas[i] = 0; // Inicializa o custo da rota atual como 0

        if (encontra_no_vetor(rota, i) != -1) { // Verifica se o valor de i faz parte da rota base
            continue; // Caso i já esteja presente na rota base, pula para a proxima execucao do for
        }

        rotas_possiveis[i][comeco] = i; // Define a primeira posicao nao definida da rota como o i

        for (int pos = comeco + 1; pos <= final; pos++) { // For que avanca pelas casas da rota
            int custos_ordenados[PONTOS]; // Define um vetor para armazenar a ordem dos custos
            int ponto_anterior = rotas_possiveis[i][pos-1]; // Procura pelo ultimo ponto percorrido

            gera_ordem_de_custos(custos[ponto_anterior], custos_ordenados); // Ordena os custos de viagem partindo de ponto_anterior

            for (int cont = 0; cont < PONTOS; cont++) { // For que avanca pelos custos ordenados
                int custo_atual = custos[ponto_anterior][custos_ordenados[cont]]; // Busca o custo atual

                if (custo_atual != -1 && encontra_no_vetor(rotas_possiveis[i], custos_ordenados[cont]) == -1) { // Caso o valor do custo seja valido e o ponto ainda nao tenha sido visitado
                    rotas_possiveis[i][pos] = custos_ordenados[cont]; // Define o ponto custos_ordenados[cont] como o proximo ponto da rota
                    custos_rotas[i] += custo_atual; // E soma o custo da viagem de ponto_aterior para o ponto custos_ordenados[cont] aos custos totais da rota
                    break; // Como o menor valor foi encontrado, interrompe a execucao do for
                }
            }
        }
    }

    int custos_totais_ordenados[PONTOS]; 
    gera_ordem_de_custos(custos_rotas, custos_totais_ordenados); // Ordena os custos de maneira crescente e salva no vetor custos_totais_ordenados

    // Mostra em tela a rota de menor custo
    for (int pos = 0; pos < PONTOS; pos++) {
        if (custos_rotas[custos_totais_ordenados[pos]] != 0) { // Verifica se a rota é valida
            printf("Melhor Rota: ");
            printa_vetor(rotas_possiveis[custos_totais_ordenados[pos]]);
            printf("Custo Total da Rota: %d\n", custos_rotas[custos_totais_ordenados[pos]]);
            break; // Para a execucao do for
        }
    }

    return 0;
}



int main() {
    int ponto_inicial, ponto_final; // Declarando as variaveis ponto_inical e ponto_final
    int rota[PONTOS]; // Declarando a variavel que armazenara a rota base

    int criminalidade[PONTOS][PONTOS]; // Declarando a matriz criminalidade
    int transito[PONTOS][PONTOS]; // Declarando a matriz transito
    int distancia[PONTOS][PONTOS]; // Declarando a matriz distancia
    int custo[PONTOS][PONTOS]; // Declarando a matriz custo

    inicializa_vetor(rota, -1); // Inicializando todas as casas do vetor rota_aux como -1

    inicializa_matriz(criminalidade, -1); // Inicializando todas as casas da matriz criminalidade como -1
    inicializa_matriz(transito, -1); // Inicializando todas as casas da matriz transito como -1
    inicializa_matriz(distancia, -1); // Inicializando todas as casas da matriz distancia como -1

    FILE *arq_criminalidade = fopen("criminalidade.txt", "r"); // Abrindo o arquivo de criminalidade
    FILE *arq_transito = fopen("transito.txt", "r"); // Abrindo o arquivo de transito
    FILE *arq_distancia = fopen("distancia.txt", "r"); // Abrindo o arquivo de distancia

    if (arq_criminalidade == NULL) { // Indentificando erro na leitura do arquivo criminalidade
        printf("Erro ao ler aquivo de criminalidade"); // Notificando usuario sobre o erro
        return -1; // Parando a execucao do programa caso seja encontrado um erro de leitura
    }
    if (arq_transito == NULL) { // Indentificando erro na leitura do arquivo transito
        printf("Erro ao ler aquivo de transito"); // Notificando usuario sobre o erro
        return -1; // Parando a execucao do programa caso seja encontrado um erro de leitura
    }
    if (arq_distancia == NULL) { // Indentificando erro na leitura do arquivo distancia
        printf("Erro ao ler aquivo de distancia"); // Notificando usuario sobre o erro
        return -1; // Parando a execucao do programa caso seja encontrado um erro de leitura
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
        scanf("%d", &rota[0]);
    } while (rota[0] < 0 || rota[0] > PONTOS - 1);

    // Lendo o ponto final; A leitura e repetida ate que seja inserido um numero valido
    do {
        printf("Insira o ponto final: ");
        scanf("%d", &rota[PONTOS-1]);
    } while (rota[PONTOS-1] < 0 || rota[PONTOS-1] > PONTOS - 1);

    // Mostrando em tela a rota base = [ ponto_inicial X X ... X X ponto_final ]
    printf("Rota Base: "); 
    printa_vetor(rota); 
    //

    encontra_menor_caminho(custo, rota); // Executando funcao que encontra e mostra em tela a menor rota
    return 0;
}
