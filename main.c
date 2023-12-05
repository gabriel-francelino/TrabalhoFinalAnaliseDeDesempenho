/*
    Proposta do Trabalho: // ATUALIZAR DEFINIÇÃO
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
            Tende a aproximar de 0 com o tempo. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <time.h>

#define INTERVALO_COLETA 10.0

typedef struct {
    double media_chegada;
    double media_servico;
    double tempo_simulacao;
} parametros;

typedef struct {
    unsigned long int no_eventos;
    double tempo_anterior;
    double soma_areas;
} little;

// definir ocupações
// Taxas de Chegada para Ocupações
typedef struct {
   double taxa60;
   double taxa80;
   double taxa90;
   double taxa99;
} ocupacoes;

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
   printf("Taxa de Chegada para 0.60: %.20f pessoas por segundo\n", ocupacoes->taxa60);

   // Para 80% de ocupação, 800Mbps em 1Gbps
   ocupacoes->taxa80 = calcular_taxa(0.8);
   printf("Taxa de Chegada para 0.80: %.20f pessoas por segundo\n", ocupacoes->taxa80);

   // Para 90% de ocupação, 900Mbps em 1Gbps
   ocupacoes->taxa90 = calcular_taxa(0.9);
   printf("Taxa de Chegada para 0.90: %.20f pessoas por segundo\n", ocupacoes->taxa90);

   // Para 99% de ocupação, 999Mbps em 1Gbps
   ocupacoes->taxa99 = calcular_taxa(0.99);
   printf("Taxa de Chegada para 0.99: %.20f pessoas por segundo\n", ocupacoes->taxa99);
}

void inicia_little(little *l) {
    l->no_eventos = 0;
    l->tempo_anterior = 0.0;
    l->soma_areas = 0.0;
}

void define_parametros(parametros *params, int cenario){
    printf("\n");
    switch (cenario) {
    case 1: // Ocupação 60%
        puts("Cenário 1 (60%):");
        printf("-> Tempo medio entre clientes (s): 0.5");
        params->media_chegada = 1.0/0.5;

        printf("\n-> Tempo medio de servico (s): 0.3");
        params->media_servico = 1.0/0.3;
        break;
    case 2: // Ocupação 80%
        puts("Cenário 2 (80%):");
        printf("-> Tempo medio entre clientes (s): 0.5");
        params->media_chegada = 1.0/0.5;

        printf("\n-> Tempo medio de servico (s): 0.4");
        params->media_servico = 1.0/0.4;
        break;
    case 3: // Ocupação 90%
        puts("Cenário 3 (90%):");
        printf("-> Tempo medio entre clientes (s): 0.5");
        params->media_chegada = 1.0/0.5;

        printf("\n-> Tempo medio de servico (s): 0.45");
        params->media_servico = 1.0/0.45;
        break;
    case 4: // Ocupação 99%
        puts("Cenário 4 (99%):");
        printf("-> Tempo medio entre clientes (s): 0.5");
        params->media_chegada = 1.0/0.5;

        printf("\n-> Tempo medio de servico (s): 0.495");
        params->media_servico = 1.0/0.495;
        break;
    default:
        printf("Cenário não encontrado.");
        exit(1);
        break;
    }
    puts("\n-> Tempo a ser simulado (s): 864.000");
    params->tempo_simulacao = 864000;
}

double uniforme() {
    double u = rand() / ((double) RAND_MAX + 1);
    u = 1.0 - u;
    return u;
}

double min(double d1, double d2) {
    if(d1 < d2) return d1;
    return d2;
}

double gerar_tempo(double media) {
    return (-1.0/media) * log(uniforme());
}

void resultado_csv(int cenario, double ocupacao_objetivo, double ocupacao_simulada, double e_n, double e_w) {
    // Cria um arquivo para os resultados
    char nome_arquivo[20];
    sprintf(nome_arquivo, "res/results_%d.csv", cenario);
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (arquivo == NULL) {
        perror("Erro ao gerar o arquivo");
        exit(1);
    }
    // Escreve as medidas de validação no arquivo    
    fprintf(arquivo, "Cenario,Ocupação Objetivo,Ocupação Simulada,E[N],E[W]\n");
    fprintf(arquivo, "%d, %.6lf, %.6lf, %.6lf, %.6lf\n", cenario, ocupacao_objetivo, ocupacao_simulada, e_n, e_w);
    
    fclose(arquivo);
}

void erro_csv(int cenario, int n_erros, double erros_little[n_erros]) {
    // Cria um arquivo para os erros do cenário
    char nome_arquivo[20];
    sprintf(nome_arquivo, "res/erros_%d.csv", cenario);
    FILE *arquivo = fopen(nome_arquivo, "w");
    if (arquivo == NULL) {
        perror("Erro ao gerar o arquivo");
        exit(1);
    }
    // Escreve o cabeçalho para o vetor de erros de Little    
    fprintf(arquivo, "Tempo,Erro Little\n");
    
    // Escreve os valores do vetor de erros de Little    
    for (int i = 0; i < n_erros; i++) {
        fprintf(arquivo, "%.2lf, %.20lf\n", (i+1) * 10.0, erros_little[i]);
    }
    fclose(arquivo);
}

int main() {
    int seed = time(NULL);
    srand(seed);
    printf("\nSeed: %d\n", seed);

    int cenario;
    printf("\nInforme o cenário desejado (1, 2, 3, ou 4): ");
    scanf("%d", &cenario);
    
    // Parâmetros da simulação
    parametros params;
    define_parametros(&params, cenario);

    double tempo_decorrido = 0.0;
    double tempo_chegada = gerar_tempo(params.media_chegada);
    double tempo_saida = DBL_MAX;
    unsigned long int fila = 0;
    int max_fila = 0;
    
    // Variáveis de Medidas de Interesse
    double soma_ocupacao = 0.0;

    // Little
    little e_n;         // E[N]
    little e_w_chegada; // E[W] Chegada
    little e_w_saida;   // E[W] Saída
    inicia_little(&e_n);
    inicia_little(&e_w_chegada); 
    inicia_little(&e_w_saida);

    // Erros de Little a cada 10 segundos
    double tempo_coleta = INTERVALO_COLETA;
    int indice_erro = 0;
    int n_erros = (int)(params.tempo_simulacao / INTERVALO_COLETA);
    double erros_little[n_erros];

    while (tempo_decorrido < params.tempo_simulacao)
    {
        // determina o proximo evento
        double tempo_proximo_evento = min(tempo_chegada, tempo_saida);
        tempo_proximo_evento = min(tempo_proximo_evento, tempo_coleta);

        // avança o tempo para o proximo evento
        tempo_decorrido = tempo_proximo_evento;

        if (tempo_decorrido == tempo_chegada)
        {
            // Evento: chegada de um usuario
            // atualiza métricas, calculo little, geraçao de tempo...

            // nos dois primeiros minutos só tem chegada
            // no fim de 2 min tem que ter uma taxa de x = 7978,7234... ativas
        } else if (tempo_decorrido == tempo_saida)
        {
            // saida do usuario
            // atualiza metricas
        }else if (tempo_decorrido == tempo_coleta)
        {
            // coleta de dados para calculo de little
            // atualiza metricas

            // avança no temoi de coleta para o proximo intervalo
            tempo_coleta += INTERVALO_COLETA;
        } else {
            // evento inválido
            printf("Evento inválido!\n");
            break;
        }
        
        
        
    }
    

    // while(tempo_decorrido < params.tempo_simulacao) {
    //     tempo_decorrido = min(tempo_coleta, min(tempo_chegada, tempo_saida));
    //     // printf("%lF\n", tempo_decorrido);

    //     if(tempo_decorrido == tempo_chegada) {
    //         // * Chegada
    //         if(!fila){
    //             double tempo_servico = gerar_tempo(params.media_servico);
    //             tempo_saida = tempo_decorrido + tempo_servico;
    //             soma_ocupacao += tempo_servico;
    //         }
    //         fila++;
    //         max_fila = max_fila > fila ? max_fila : fila;
    //         tempo_chegada = tempo_decorrido + gerar_tempo(params.media_chegada);

    //         // Cálculo Little -> E[N]
    //         e_n.soma_areas += (tempo_decorrido - e_n.tempo_anterior) * e_n.no_eventos;
    //         e_n.no_eventos++;
    //         e_n.tempo_anterior = tempo_decorrido;

    //         // Cálculo Little -> E[W] Chegada
    //         e_w_chegada.soma_areas += (tempo_decorrido - e_w_chegada.tempo_anterior) * e_w_chegada.no_eventos;
    //         e_w_chegada.no_eventos++;
    //         e_w_chegada.tempo_anterior = tempo_decorrido;

    //     } else if(tempo_decorrido == tempo_saida) {
    //         // * Saída
    //         fila--;
    //         if(fila) {
    //             double tempo_servico = gerar_tempo(params.media_servico);
    //             tempo_saida = tempo_decorrido + tempo_servico;
    //             soma_ocupacao += tempo_servico;
    //         } else {
    //             tempo_saida = DBL_MAX;
    //         }

    //         // Cálculo Little -> E[N]
    //         e_n.soma_areas += (tempo_decorrido - e_n.tempo_anterior) * e_n.no_eventos;
    //         e_n.no_eventos--;
    //         e_n.tempo_anterior = tempo_decorrido;

    //         // Cálculo Little -> E[W] Saída
    //         e_w_saida.soma_areas += (tempo_decorrido - e_w_saida.tempo_anterior) * e_w_saida.no_eventos;
    //         e_w_saida.no_eventos++;
    //         e_w_saida.tempo_anterior = tempo_decorrido;

    //     } else if (tempo_decorrido == tempo_coleta){
    //         // Cálculo do Erro de Little

    //         e_n.soma_areas += (tempo_decorrido - e_n.tempo_anterior) * e_n.no_eventos;
    //         e_w_chegada.soma_areas += (tempo_decorrido - e_w_chegada.tempo_anterior) * e_w_chegada.no_eventos;
    //         e_w_saida.soma_areas += (tempo_decorrido - e_w_saida.tempo_anterior) * e_w_saida.no_eventos;
    //         e_n.tempo_anterior = tempo_decorrido;
    //         e_w_chegada.tempo_anterior = tempo_decorrido;
    //         e_w_saida.tempo_anterior = tempo_decorrido;

    //         double e_n_calculo = e_n.soma_areas / tempo_decorrido;
    //         double e_w_calculo = (e_w_chegada.soma_areas - e_w_saida.soma_areas) / e_w_chegada.no_eventos;
    //         double lambda = e_w_chegada.no_eventos / tempo_decorrido;
    //         double erro_little = e_n_calculo - (lambda * e_w_calculo);
    //         if(erro_little < 0) {
    //             erro_little = (-1.0)*erro_little;
    //         }           
    //         erros_little[indice_erro] = erro_little;

    //         // printf("Erro calculado em %d segundos: %.20lF\n", tempo_intervalo, erros_little[indice_erro]);
    //         tempo_coleta += INTERVALO_COLETA;
    //         indice_erro++;
    //     } else {
    //         puts("Evento Inválido!\n");
    //         return (1);
    //     }
    // }
    e_w_chegada.soma_areas += (tempo_decorrido - e_w_chegada.tempo_anterior) * e_w_chegada.no_eventos;
    e_w_saida.soma_areas += (tempo_decorrido - e_w_saida.tempo_anterior) * e_w_saida.no_eventos;

    // Informações Finais sobre a simulação
    puts("\n---- Validação da Simulação ----"); 
    printf("-> Tamanho Máximo da Fila: %d\n", max_fila);
    double ocupacao_objetivo = params.media_chegada / params.media_servico;
    printf("-> Ocupação Objetivo: %lF\n", ocupacao_objetivo);
    double ocupacao = soma_ocupacao / tempo_decorrido;
    printf("-> Ocupação Alcançada: %lF\n", ocupacao);
    double e_n_calculo = e_n.soma_areas / tempo_decorrido;
    double e_w_calculo = (e_w_chegada.soma_areas - e_w_saida.soma_areas) / e_w_chegada.no_eventos;
    double lambda = e_w_chegada.no_eventos / tempo_decorrido;
    printf("-> E[N] = %lF\n", e_n_calculo);
    printf("-> E[W] = %lF\n", e_w_calculo);
    printf("-> Erro de Little Final: %.20lF\n\n", e_n_calculo - (lambda * e_w_calculo));

    // Gera o arquivo CSV de resultados e de erros de Little da simulação
    resultado_csv(cenario, ocupacao_objetivo, ocupacao, e_n_calculo, e_w_calculo);
    erro_csv(cenario, n_erros, erros_little);

    return 0;
}