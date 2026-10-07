#include "Servico.hpp"

Servico::Servico(int duracaoMinutos, Especialidade especialidade, double taxaMaoDeObra, double taxaFixaSessao) : duracaoMinutos_(duracaoMinutos), especialidade_(especialidade), taxaMaoDeObra_(taxaMaoDeObra), taxaFixaSessao_(taxaFixaSessao) {
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

double Servico::getTaxaMaoDeObra() const {
    return taxaMaoDeObra_;
}

double Servico::getTaxaFixaSessao() const {
    return taxaFixaSessao_;
}

std::string Servico::detalhes() const {
    return std::to_string(duracaoMinutos_) + " min";
}