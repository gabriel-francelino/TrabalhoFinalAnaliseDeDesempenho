#ifndef FILA_H
#define FILA_H

#include <queue>
#include <limits>

struct Conexao
{
    double tempo_duracao;
    double tempo_chegada_pacote;
};

class MinhaFila
{
private:
    std::queue<Conexao> fila;

public:
    Conexao top();
    void push(Conexao nova_conexao);
    void pop();
    bool empty();
};

#endif