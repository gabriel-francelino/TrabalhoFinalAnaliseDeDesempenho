/*
    Proposta do Trabalho:
        * -- Trabalho 2 -- 

        ! Rede de um Trabalho Remoto
        (Várias chamadas dos funcionários)
        Cada chamada dura em média 2 minutos -> Exponencial
        Enquanto a chamada está ativa, gera pacotes em transmissão
        em tempo real (CBR Contant Bit Rate = a cada 20ms)
        O tempo médio entre os pacotes depende do CODEC (de audio e vídeo da aplicação)
            G711u/a - 50 pacotes por segundo (64Kbps - 160 bytes por pacote)
            160 bytes + 8 bytes (UDP) + 20 bytes (IP) = 188 bytes
        Link de 16bps 
        O intervalo entre ligações também é exponencial.

        Quantas pessoas são necessárias no link ao mesmo tempo para obter 
        ocupação de (60%, 80%, 90% e 99%)

        ? Depois disso, calcular os mesmos valores do último simulador:

        Ocupação, máximo de fila, E[N], E[W] e Erro de Little. 
        Tempo de Simulação = 864.000 = 10 dias
        4 cenários - 
            Ocupação Aprox 60%;
            Ocupação Aprox 80%;
            Ocupação Aprox 90%;
            Ocupação Aprox 99%;
        Para cada cenário:
            Qual o intervalo entre chegadas?
            Qual o intervalo entre serviços?

        Gerar um gráfico com as medidas de validação da simulação. (Gráficos Gerais)
            Ocupação Simulada x Ocupação Calculada
            E[N] x Ocupação Calculada
            E[W] x Ocupação Calculada

        Gráficos de Erro de Little
            Erro Little |E[N]-E[W]*Lambda| Aprox 0
            Medir erro a cada 10 segundos exatos. 
            Um gráfico para cada cenário -> Erro x Tempo
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <time.h>
#include "min_heap.h"

// --------------------------------------------------------------------------

#define INTERVALO_COLETA 10.0
#define TEMPO_SIMULACAO 864000
#define ATRASO_TRANSMISSAO ((9400 * 8) / 1000000000)

// --------------------------------------------------------------------------

// Struct com os parâmetros da simulação
typedef struct {
    double media_chegada;
    double media_servico;
    double tempo_simulacao;
} parametros;

// Struct para o Calculo de Little
typedef struct {
    unsigned long int no_eventos;
    double tempo_anterior;
    double soma_areas;
} little;

// Struct com os valores das taxas de chegada para as ocupações
typedef struct {
    double taxa60;
    double taxa80;
    double taxa90;
    double taxa99;
} ocupacoes;

// --------------------------------------------------------------------------

// * Funções para o cálculo das taxas de chegada para os cenários

// Calcula a taxa de chegada de acordo com a ocupação passada
double calcular_taxa(double ocupacao) {
   double bytesPorPessoaPorSegundo = 9400;
   double bitsPorPessoaPorSegundo = bytesPorPessoaPorSegundo * 8;
   double gigabitsPorPessoaPorSegundo = bitsPorPessoaPorSegundo / 1000000000;
   double nPessoas = ocupacao / gigabitsPorPessoaPorSegundo;
   double nPessoasPorSegundo = (nPessoas / 2) / 60;

   return nPessoasPorSegundo;
}

// Define a taxa de chegada para as 4 ocupações
void definir_ocupacoes(ocupacoes *ocupacoes) {
    // Para 60% de ocupação, 600Mbps em 1Gbps
    ocupacoes->taxa60 = calcular_taxa(0.6);
    //printf("Taxa de Chegada para 0.60: %.20f pessoas por segundo\n", ocupacoes->taxa60);

    // Para 80% de ocupação, 800Mbps em 1Gbps
    ocupacoes->taxa80 = calcular_taxa(0.8);
    //printf("Taxa de Chegada para 0.80: %.20f pessoas por segundo\n", ocupacoes->taxa80);

    // Para 90% de ocupação, 900Mbps em 1Gbps
    ocupacoes->taxa90 = calcular_taxa(0.9);
    //printf("Taxa de Chegada para 0.90: %.20f pessoas por segundo\n", ocupacoes->taxa90);

    // Para 99% de ocupação, 999Mbps em 1Gbps
    ocupacoes->taxa99 = calcular_taxa(0.99);
    //printf("Taxa de Chegada para 0.99: %.20f pessoas por segundo\n", ocupacoes->taxa99);
}

// ------------------------------------------------------------------------------------------------------------------------------

// Função para definir os parâmetros da simulação passando o cenário desejado
void define_parametros(parametros *params, int cenario, ocupacoes ocupacoes){
    printf("\n");
    switch (cenario) {
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

    puts("\n-> Tempo a ser simulado (s): 864.000");
    params->tempo_simulacao = TEMPO_SIMULACAO;
}

// --------------------------------------------------------------------------

// Função que inicia os valores para o cálculo de little
void inicia_little(little *l) {
    l->no_eventos = 0;
    l->tempo_anterior = 0.0;
    l->soma_areas = 0.0;
}

// --------------------------------------------------------------------------

// Funções para o cálculo do tempo exponencial

double uniforme() {
    double u = rand() / ((double) RAND_MAX + 1);
    u = 1.0 - u;
    return u;
}

double gerar_tempo(double media) {
    return (-1.0/media) * log(uniforme());
}

// --------------------------------------------------------------------------

// Funções Auxiliares

double min(double d1, double d2) {
    if(d1 < d2) return d1;
    return d2;
}

// --------------------------------------------------------------------------

// ? Funções para escrever os resultados em arquivos CSV

// ... adicionar depois 

// --------------------------------------------------------------------------

int main() {
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

    // ! Serão 2 de cada? Um para saída de pessoas 
    // ! e um para saída de pacotes

    // ------------------------------------

    // Tempos para a simulação

    double intervalo_conexao = 1.0 / params.media_chegada;
    double tempo_decorrido = 0.0;
    double nova_conexao = gerar_tempo(intervalo_conexao);

    // double tempo_chegada = gerar_tempo(params.media_chegada);
    // double tempo_saida = DBL_MAX;

    // ------------------------------------
    
    // Variáveis de interesse para a simulação
    
    double soma_ocupacao = 0.0;
    unsigned long int fila = 0;
    int max_fila = 0;

    // ! Como vai funcionar a file nesse problema?

    // ------------------------------------

    // Erros de Little a cada 10 segundos
    double tempo_coleta = INTERVALO_COLETA;
    // .. adicionar depois o restante das variáveis do cálculo da coleta

    // ------------------------------------

    MinHeap meuHeap;
    double saida_pacote = DBL_MAX;
    double chegada_pacote = DBL_MAX;

    while (tempo_decorrido < params.tempo_simulacao) {
        
        Conexao conexao_chegada_pacote = meuHeap.mostrarMenorConexao();
        chegada_pacote = conexao_chegada_pacote.tempo_chegada_pacote;

        // Determinar o próximo evento
        tempo_decorrido = min(nova_conexao, min(tempo_coleta, min(chegada_pacote, saida_pacote)));

        if (tempo_decorrido == nova_conexao) {

            double tempo_duracao = gerar_tempo(params.media_servico);
            double tempo_conexao_chegada_pacote = nova_conexao;
            Conexao conexao = {tempo_duracao, tempo_conexao_chegada_pacote};
            meuHeap.adicionarConexao(conexao);
            nova_conexao = tempo_decorrido + gerar_tempo(params.media_chegada);

            // TODO: Calcular os valores de little

            // ! Tratamento Primeiros 2 minutos?

        } else if (tempo_decorrido == chegada_pacote) {

            saida_pacote = chegada_pacote + ATRASO_TRANSMISSAO; 
            conexao_chegada_pacote.tempo_chegada_pacote += 0.020;
            meuHeap.removerConexao();
            if (conexao_chegada_pacote.tempo_duracao > conexao_chegada_pacote.tempo_chegada_pacote) {
                meuHeap.adicionarConexao(conexao_chegada_pacote);
            }

            // TODO: Calcular os valores de little

        } else if (tempo_decorrido == saida_pacote) {
            
            // TODO: Calcular os valores de little

            saida_pacote = DBL_MAX;

        } else if (tempo_decorrido == tempo_coleta) {

            // TODO: Calcular os valores de little

            tempo_coleta += INTERVALO_COLETA;

        } else {
            printf("Evento inválido!\n");
            break;
        }        
    }

    // --------------------------------------------------------------------------

    // e_w_chegada.soma_areas += (tempo_decorrido - e_w_chegada.tempo_anterior) * e_w_chegada.no_eventos;
    // e_w_saida.soma_areas += (tempo_decorrido - e_w_saida.tempo_anterior) * e_w_saida.no_eventos;

    // --------------------------------------------------------------------------

    // Informações finais sobre a simulação
    puts("\n---- Validação da Simulação ----"); 
    printf("-> Tamanho Máximo da Fila: %d\n", max_fila);
    double ocupacao = soma_ocupacao / tempo_decorrido;
    printf("-> Ocupação Simulada: %lF\n", ocupacao);
    double e_n_calculo = e_n.soma_areas / tempo_decorrido;
    double e_w_calculo = (e_w_chegada.soma_areas - e_w_saida.soma_areas) / e_w_chegada.no_eventos;
    double lambda = e_w_chegada.no_eventos / tempo_decorrido;
    printf("-> E[N] = %lF\n", e_n_calculo);
    printf("-> E[W] = %lF\n", e_w_calculo);
    printf("-> Erro de Little Final: %.20lF\n\n", e_n_calculo - (lambda * e_w_calculo));

    // --------------------------------------------------------------------------

    // TODO: Gera o arquivo CSV de resultados e de erros de Little da simulação
    // .. adicionar depois que o simulador estiver funcionando

    // --------------------------------------------------------------------------

    return 0;
}
