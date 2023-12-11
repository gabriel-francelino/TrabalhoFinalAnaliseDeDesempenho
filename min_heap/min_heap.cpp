#include <math.h>
#include <float.h>

#include "min_heap.h"

void MinHeap::adicionarConexao(Conexao novaConexao) {
    minHeap.push(novaConexao);
}

void MinHeap::removerConexao() {
    if (!minHeap.empty()) {
        minHeap.pop();
    } else {
        // std::cout << "O heap está vazio. Não é possível remover." << std::endl;
    }
}

Conexao MinHeap::mostrarMenorConexao() {
    if (!minHeap.empty()) {
        return minHeap.top();
    } else {
        // std::cout << "O heap está vazio." << std::endl;
        return Conexao{DBL_MAX, DBL_MAX};
    }
}

bool MinHeap::estaVazio() {
    return minHeap.empty();
}
