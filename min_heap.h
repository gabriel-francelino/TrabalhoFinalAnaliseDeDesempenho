// min_heap.h

#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#include <iostream>
#include <queue>
#include <vector>

struct Conexao {
    double tempo_duracao;
    double tempo_chegada_pacote;

    bool operator<(const Conexao& outra) const {
        return tempo_chegada_pacote > outra.tempo_chegada_pacote;
    }
};

struct Evento {
    double tempo_evento;
    double tempo_duracao;
    short type; // 0 = Nova Conexão // 1 = Pacote // 2 = Serviço (Saída Pacote) // 3 = Coleta

    bool operator<(const Evento& outra) const {
        return tempo_evento > outra.tempo_evento;
    }
};

class MinHeap {
private:
    std::priority_queue<Conexao> minHeap;

public:
    void adicionarConexao(Conexao novaConexao);
    void removerConexao();
    Conexao mostrarMenorConexao();
    bool estaVazio();
};

#endif /* MIN_HEAP_H */
