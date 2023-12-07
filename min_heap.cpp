#include "min_heap.h"

void MinHeap::adicionarConexao(Conexao novaConexao) {
    minHeap.push(novaConexao);
}

void MinHeap::removerConexao() {
    if (!minHeap.empty()) {
        minHeap.pop();
    } else {
        std::cout << "O heap está vazio. Não é possível remover." << std::endl;
    }
}

Conexao MinHeap::mostrarMenorConexao() {
    if (!minHeap.empty()) {
        return minHeap.top();
    } else {
        std::cout << "O heap está vazio." << std::endl;
        return Conexao{0.0, 0.0};
    }
}

bool MinHeap::estaVazio() {
    return minHeap.empty();
}
