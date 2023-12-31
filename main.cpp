/*===================================================================
* UNIFAL = Universidade Federal de Alfenas.
* BACHARELADO EM CIENCIA DA COMPUTACAO.
* Trabalho..: Simulador Discreto / Trabalho 2
* Disciplina: Análise de Desempenho
* Alunos....: Gabriel Francelino Nascimento e Gabriel Piva Pereira
*===================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <time.h>
#include <iostream>
#include <cstdio>
#include "fila.h"
#include <chrono>

using namespace std;

// --------------------------------------------------------------------------

#define INTERVALO_COLETA 10.0
#define TEMPO_SIMULACAO 432000

// --------------------------------------------------------------------------

// Struct com os parâmetros da simulação
typedef struct
{
    double media_chegada;
    double media_servico;
    double tempo_simulacao;
} parametros;

// Struct para o Calculo de Little
typedef struct
{
    unsigned long int no_eventos;
    double tempo_anterior;
    double soma_areas;
} little;

// Struct com os valores das taxas de chegada para as ocupações
typedef struct
{
    double taxa60;
    double taxa80;
    double taxa90;
    double taxa99;
} ocupacoes;

// --------------------------------------------------------------------------

// Calcula a taxa de chegada de acordo com a ocupação passada
double calcular_taxa(double ocupacao)
{
    double bytesPorPessoaPorSegundo = 9400;
    double bitsPorPessoaPorSegundo = bytesPorPessoaPorSegundo * 8;
    double gigabitsPorPessoaPorSegundo = bitsPorPessoaPorSegundo / 1000000000;
    double nPessoas = ocupacao / gigabitsPorPessoaPorSegundo;
    double nPessoasPorSegundo = (nPessoas / 2) / 60;

    return nPessoasPorSegundo;
}

// Define a taxa de chegada para as 4 ocupações
void definir_ocupacoes(ocupacoes *ocupacoes)
{
    // Para 60% de ocupação, 600Mbps em 1Gbps
    ocupacoes->taxa60 = calcular_taxa(0.6);
    // Para 80% de ocupação, 800Mbps em 1Gbps
    ocupacoes->taxa80 = calcular_taxa(0.8);
    // Para 90% de ocupação, 900Mbps em 1Gbps
    ocupacoes->taxa90 = calcular_taxa(0.9);
    // Para 99% de ocupação, 999Mbps em 1Gbps
    ocupacoes->taxa99 = calcular_taxa(0.99);
}

// ------------------------------------------------------------------------------------------------------------------------------

// Função para definir os parâmetros da simulação passando o cenário desejado
void define_parametros(parametros *params, int cenario, ocupacoes ocupacoes)
{
    printf("\n");
    switch (cenario)
    {
    case 1: // Ocupação 60%
        puts("Cenário 1 (60%):");
        printf("-> Taxa de chegada de pessoas (s): %.4f", ocupacoes.taxa60);
        params->media_chegada = ocupacoes.taxa60;
        break;
    case 2: // Ocupação 80%
        puts("Cenário 2 (80%):");
        printf("-> Taxa de chegada de pessoas (s): %.4f", ocupacoes.taxa80);
        params->media_chegada = ocupacoes.taxa80;
        break;
    case 3: // Ocupação 90%
        puts("Cenário 3 (90%):");
        printf("-> Taxa de chegada de pessoas (s): %.4f", ocupacoes.taxa90);
        params->media_chegada = ocupacoes.taxa90;
        break;
    case 4: // Ocupação 99%
        puts("Cenário 4 (99%):");
        printf("-> Taxa de chegada de pessoas (s): %.4f", ocupacoes.taxa99);
        params->media_chegada = ocupacoes.taxa99;
        break;
    default:
        printf("Cenário não encontrado.");
        exit(1);
        break;
    }

    printf("\n-> Tempo medio de servico (s): 120");
    params->media_servico = 120;

    printf("\n-> Tempo a ser simulado (s): %d\n", TEMPO_SIMULACAO);
    params->tempo_simulacao = TEMPO_SIMULACAO;
}

// --------------------------------------------------------------------------

// Função que inicia os valores para o cálculo de little
void inicia_little(little *l)
{
    l->no_eventos = 0;
    l->tempo_anterior = 0.0;
    l->soma_areas = 0.0;
}

// --------------------------------------------------------------------------

// Funções para o cálculo do tempo exponencial
double uniforme()
{
    double u = rand() / ((double)RAND_MAX + 1);
    u = 1.0 - u;
    return u;
}


// Função para gerar tempo baseado na função exponencial
double gerar_tempo(double media)
{
    return (-1.0 / media) * log(uniforme());
}

// --------------------------------------------------------------------------

// Funções Auxiliares
double min(double d1, double d2)
{
    if (d1 < d2)
        return d1;
    return d2;
}

// --------------------------------------------------------------------------

// Funções para escrever os resultados em arquivos CSV
void resultado_csv(int cenario, double ocupacao_simulada, double e_n, double e_w, int max_fila)
{
    // Cria um arquivo para os resultados
    char nome_arquivo[20];
    sprintf(nome_arquivo, "res/results_%d.csv", cenario);
    FILE *arquivo = std::fopen(nome_arquivo, "w");
    if (arquivo == nullptr)
    {
        perror("Erro ao gerar o arquivo");
        exit(1);
    }
    // Escreve as medidas de validação no arquivo
    fprintf(arquivo, "Cenario,Ocupação Simulada,E[N],E[W],Fila Máxima\n");
    fprintf(arquivo, "%d, %.20lf, %.20lf, %.20lf, %d\n", cenario, ocupacao_simulada, e_n, e_w, max_fila);
    fclose(arquivo);
}

void erro_csv(int cenario, int n_erros, double erros_little[])
{
    // Cria um arquivo para os erros do cenário
    char nome_arquivo[20];
    sprintf(nome_arquivo, "res/erros_%d.csv", cenario);
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (arquivo == nullptr)
    {
        perror("Erro ao gerar o arquivo");
        exit(1);
    }
    // Escreve o cabeçalho para o vetor de erros de Little
    fprintf(arquivo, "Tempo,Erro Little\n");
    // Escreve os valores do vetor de erros de Little
    for (int i = 0; i < n_erros; i++)
    {
        fprintf(arquivo, "%.2lf, %.20lf\n", (i + 1) * 10.0, erros_little[i]);
    }
    fclose(arquivo);
}

// --------------------------------------------------------------------------

// Função para acompanhar o andamento da execução

void registrar_andamento(double tempoDecorrido) 
{
    FILE *arquivo;
    arquivo = fopen("tempo.txt", "a");
    if (arquivo != NULL) {
        fprintf(arquivo, "Tempo decorrido: %f\n", tempoDecorrido);
        fclose(arquivo);
    }
}
void limparArquivo() 
{
    FILE *arquivo;
    arquivo = fopen("tempo.txt", "w");
    if (arquivo != NULL) {
        fclose(arquivo);
    }
}

// --------------------------------------------------------------------------

int main()
{
    auto start = chrono::high_resolution_clock::now();

    // ------------------------------------

    limparArquivo();

    // ------------------------------------

    int seed = time(NULL);
    srand(seed);
    printf("\nSeed: %d\n", seed);

    // ------------------------------------

    int cenario;
    printf("\nInforme o cenário desejado (1, 2, 3, ou 4): ");
    scanf("%d", &cenario);

    // ------------------------------------

    ocupacoes ocupacoes;
    definir_ocupacoes(&ocupacoes);

    // ------------------------------------

    // Parâmetros da simulação
    parametros params;
    define_parametros(&params, cenario, ocupacoes);

    // ------------------------------------

    // Little
    little e_n;         // E[N]
    little e_w_chegada; // E[W] Chegada
    little e_w_saida;   // E[W] Saída
    inicia_little(&e_n);
    inicia_little(&e_w_chegada);
    inicia_little(&e_w_saida);

    // ------------------------------------

    // Tempos para a simulação

    double tempo_decorrido = 0.0;

    double intervalo_conexao = 1.0 / params.media_chegada;
    double nova_conexao = gerar_tempo(1.0 / intervalo_conexao);

    double saida_pacote = DBL_MAX;
    double chegada_pacote = DBL_MAX;

    const double atraso_transmissao = ((188.0 * 8.0) / 1000000000.0);

    // ------------------------------------

    // Variáveis para coleta a cada 10s

    double tempo_coleta = INTERVALO_COLETA;
    int indice_erro = 0;
    int n_erros = (int)(params.tempo_simulacao / INTERVALO_COLETA);
    double erros_little[n_erros];

    // ------------------------------------

    // Variáveis de interesse para a simulação

    double soma_ocupacao = 0.0;
    unsigned long int fila = 0;
    int max_fila = 0;

    // ------------------------------------

    // Fila de Conexões

    MinhaFila minhaFila;

    while (tempo_decorrido < params.tempo_simulacao)
    {

        Conexao conexao_chegada_pacote = minhaFila.top();
        chegada_pacote = conexao_chegada_pacote.tempo_chegada_pacote;

        // Determinar o próximo evento
        tempo_decorrido = min(nova_conexao, min(tempo_coleta, min(chegada_pacote, saida_pacote)));

        if (tempo_decorrido == nova_conexao)
        {
            double tempo_duracao = tempo_decorrido + gerar_tempo((1.0 / params.media_servico));
            Conexao conexao = {tempo_duracao, nova_conexao+0.02};
            minhaFila.push(conexao);
            nova_conexao = tempo_decorrido + gerar_tempo((1.0 / intervalo_conexao));
        }
        else if (tempo_decorrido == chegada_pacote)
        {

            conexao_chegada_pacote.tempo_chegada_pacote += 0.020;
            minhaFila.pop();
            if (conexao_chegada_pacote.tempo_duracao >= conexao_chegada_pacote.tempo_chegada_pacote)
            {
                minhaFila.push(conexao_chegada_pacote);
            }

            if (!fila)
            {
                saida_pacote = tempo_decorrido + atraso_transmissao;
                soma_ocupacao += atraso_transmissao;
            }
            fila++;
            max_fila = max_fila > fila ? max_fila : fila;

            // Cálculo Little -> E[N]
            e_n.soma_areas += (tempo_decorrido - e_n.tempo_anterior) * e_n.no_eventos;
            e_n.no_eventos++;
            e_n.tempo_anterior = tempo_decorrido;

            // Cálculo Little -> E[W] Chegada
            e_w_chegada.soma_areas += (tempo_decorrido - e_w_chegada.tempo_anterior) * e_w_chegada.no_eventos;
            e_w_chegada.no_eventos++;
            e_w_chegada.tempo_anterior = tempo_decorrido;
        }
        else if (tempo_decorrido == saida_pacote)
        {

            fila--;
            if (fila)
            {
                saida_pacote = tempo_decorrido + atraso_transmissao;
                soma_ocupacao += atraso_transmissao;
            }
            else
            {
                saida_pacote = DBL_MAX;
            }

            // Cálculo Little -> E[N]
            e_n.soma_areas += (tempo_decorrido - e_n.tempo_anterior) * e_n.no_eventos;
            e_n.no_eventos--;
            e_n.tempo_anterior = tempo_decorrido;

            // Cálculo Little -> E[W] Saída
            e_w_saida.soma_areas += (tempo_decorrido - e_w_saida.tempo_anterior) * e_w_saida.no_eventos;
            e_w_saida.no_eventos++;
            e_w_saida.tempo_anterior = tempo_decorrido;
        }
        else if (tempo_decorrido == tempo_coleta)
        {

            e_n.soma_areas += (tempo_decorrido - e_n.tempo_anterior) * e_n.no_eventos;
            e_w_chegada.soma_areas += (tempo_decorrido - e_w_chegada.tempo_anterior) * e_w_chegada.no_eventos;
            e_w_saida.soma_areas += (tempo_decorrido - e_w_saida.tempo_anterior) * e_w_saida.no_eventos;
            e_n.tempo_anterior = tempo_decorrido;
            e_w_chegada.tempo_anterior = tempo_decorrido;
            e_w_saida.tempo_anterior = tempo_decorrido;

            double e_n_calculo = e_n.soma_areas / tempo_decorrido;
            double e_w_calculo = (e_w_chegada.soma_areas - e_w_saida.soma_areas) / e_w_chegada.no_eventos;
            double lambda = e_w_chegada.no_eventos / tempo_decorrido;
            double erro_little = e_n_calculo - (lambda * e_w_calculo);
            if (erro_little < 0)
            {
                erro_little = (-1.0) * erro_little;
            }
            erros_little[indice_erro] = erro_little;
            indice_erro++;

            tempo_coleta += INTERVALO_COLETA;

            registrar_andamento(tempo_decorrido);
        }
        else
        {
            printf("Evento inválido!\n");
            break;
        }
    }

    // --------------------------------------------------------------------------

    e_w_chegada.soma_areas += (tempo_decorrido - e_w_chegada.tempo_anterior) * e_w_chegada.no_eventos;
    e_w_saida.soma_areas += (tempo_decorrido - e_w_saida.tempo_anterior) * e_w_saida.no_eventos;

    // --------------------------------------------------------------------------

    // Informações finais sobre a simulação
    puts("\n---- Validação da Simulação ----");
    printf("-> Tamanho Máximo da Fila: %d\n", max_fila);
    double ocupacao = soma_ocupacao / tempo_decorrido;
    printf("-> Ocupação Simulada: %.20lF\n", ocupacao);
    double e_n_calculo = e_n.soma_areas / tempo_decorrido;
    double e_w_calculo = (e_w_chegada.soma_areas - e_w_saida.soma_areas) / e_w_chegada.no_eventos;
    double lambda = e_w_chegada.no_eventos / tempo_decorrido;
    printf("-> E[N] = %.20lF\n", e_n_calculo);
    printf("-> E[W] = %.20lF\n", e_w_calculo);
    double erro_little_final = e_n_calculo - (lambda * e_w_calculo);
    erro_little_final = erro_little_final < 0 ? (-1) * erro_little_final : erro_little_final;
    printf("-> Erro de Little Final: %.20lF\n\n", erro_little_final);

    // --------------------------------------------------------------------------

    // Gera o arquivo CSV de resultados e de erros de Little da simulação
    resultado_csv(cenario, ocupacao, e_n_calculo, e_w_calculo, max_fila);
    erro_csv(cenario, n_erros, erros_little);

    // --------------------------------------------------------------------------

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << endl << "-> Tempo de execucao: " << duration.count() << " segundos" << endl;

    return 0;
}
