#include "Servico.hpp"

Servico::Servico(int duracaoMinutos, Especialidade especialidade) : duracaoMinutos_(duracaoMinutos), especialidade_(especialidade) {
    // TODO: lançar DadoInvalidoException se duracaoMinutos <= 0
    // (a exceção ainda não existe; ver feat/excecoes)
}

Servico::~Servico() {}

int Servico::getDuracaoMinutos() const {
    return duracaoMinutos_;
}

Especialidade Servico::getEspecialidade() const {
    return especialidade_;
}