#include "Servico.hpp"
#include "DadoInvalidoException.hpp"

Servico::Servico(int duracaoMinutos, Especialidade especialidade, double taxaMaoDeObra, double taxaFixaSessao) : duracaoMinutos_(duracaoMinutos), especialidade_(especialidade), taxaMaoDeObra_(taxaMaoDeObra), taxaFixaSessao_(taxaFixaSessao) {
    if (duracaoMinutos <= 0) {
        throw DadoInvalidoException("Duração fornecida foi " + std::to_string(duracaoMinutos) + ", mas deve ser maior que 0.");
    }
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