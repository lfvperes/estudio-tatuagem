#include "Servico.hpp"

Servico::Servico(int duracaoMinutos, Especialidade especialidade) : duracaoMinutos_(duracaoMinutos), especialidade_(especialidade) {}

Servico::~Servico() {}

int Servico::getDuracaoMinutos() const {
    return duracaoMinutos_;
}

Especialidade Servico::getEspecialidade() const {
    return especialidade_;
}