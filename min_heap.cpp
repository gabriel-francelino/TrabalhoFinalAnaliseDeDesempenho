#include <iostream>
#include <queue>
#include <vector>

using namespace std;

struct Conexao {
    double tempo_duracao;
    double tempo_chegada_pacote;

    // Sobrecarga do operador < para definir a ordem no min heap
    bool operator<(const Conexao& outra) const {
        return tempo_chegada_pacote > outra.tempo_chegada_pacote;
    }
};

class MinHeap {
private:
    priority_queue<Conexao> minHeap;

public:
    // Adicionar uma conexão à árvore
    void adicionarConexao(Conexao novaConexao) {
        minHeap.push(novaConexao);
    }

    // Remover a conexão com menor tempo_chegada_pacote
    void removerConexao() {
        if (!minHeap.empty()) {
            minHeap.pop();
        } else {
            cout << "O heap está vazio. Não é possível remover." << endl;
        }
    }

    // Mostrar a conexão com menor tempo_chegada_pacote (topo do heap)
    Conexao mostrarMenorConexao() {
        if (!minHeap.empty()) {
            return minHeap.top();
        } else {
            cout << "O heap está vazio." << endl;
            // Retornar uma conexão vazia ou lançar uma exceção, dependendo do caso
            return Conexao{0.0, 0.0}; // Conexão vazia
        }
    }

    // Verificar se o heap está vazio
    bool estaVazio() {
        return minHeap.empty();
    }
};

int main() {
    MinHeap meuHeap;

    // Adicionar algumas conexões de exemplo
    meuHeap.adicionarConexao({2.5, 10.0});
    meuHeap.adicionarConexao({1.0, 5.0});
    meuHeap.adicionarConexao({3.0, 8.0});

    // Mostrar a conexão com menor tempo_chegada_pacote
    Conexao menor = meuHeap.mostrarMenorConexao();
    cout << "Menor tempo_chegada_pacote: " << menor.tempo_chegada_pacote << endl;

    // Remover a conexão com menor tempo_chegada_pacote
    meuHeap.removerConexao();

    // Mostrar a nova conexão com menor tempo_chegada_pacote
    menor = meuHeap.mostrarMenorConexao();
    cout << "Novo menor tempo_chegada_pacote: " << menor.tempo_chegada_pacote << endl;

    return 0;
}
