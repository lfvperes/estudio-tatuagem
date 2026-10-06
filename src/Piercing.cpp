#include "Piercing.hpp"
#include <string>

constexpr double TAXA_FIXA_SESSAO = 50;

Piercing::Piercing(int duracaoMinutos, std::string tipoJoia, double precoJoia) : Servico(duracaoMinutos, Especialidade::Piercing), tipoJoia_(tipoJoia), precoJoia_(precoJoia){}

double Piercing::calcularPreco() const {
    return precoJoia_ + TAXA_FIXA_SESSAO;
}