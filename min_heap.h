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
