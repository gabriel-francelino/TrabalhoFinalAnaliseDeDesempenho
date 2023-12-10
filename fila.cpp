#include "fila.h"
#include <float.h>

Conexao MinhaFila::top(){
    if (fila.empty()) {
        return Conexao{DBL_MAX, DBL_MAX};
    }
    return fila.front();
}

void MinhaFila::push(Conexao nova_conexao) {
    fila.push(nova_conexao);
}

void MinhaFila::pop() {
    if (!fila.empty()) {
        fila.pop();
    }
}

bool MinhaFila::empty(){
    return fila.empty();
}