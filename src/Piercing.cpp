#include "Piercing.hpp"
#include <string>

constexpr double TAXA_FIXA_SESSAO = 30;
constexpr double TAXA_MAO_DE_OBRA = 30;

Piercing::Piercing(int duracaoMinutos, std::string tipoJoia, double precoJoia) : Servico(duracaoMinutos, Especialidade::Piercing, TAXA_MAO_DE_OBRA, TAXA_FIXA_SESSAO), tipoJoia_(tipoJoia), precoJoia_(precoJoia){}

double Piercing::calcularPreco() const {
    return precoJoia_ + getTaxaFixaSessao() + getTaxaMaoDeObra();
}