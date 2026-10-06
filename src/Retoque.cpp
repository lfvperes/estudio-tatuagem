#include "Retoque.hpp"

constexpr double TAXA_FIXA_SESSAO = 30;
constexpr double TAXA_MAO_DE_OBRA = 40;
constexpr double PORCENTAGEM_DESCONTO = 30;

Retoque::Retoque(int duracaoMinutos, bool feitaNoEstudio) : Servico(duracaoMinutos, Especialidade::Tatuagem), feitaNoEstudio_(feitaNoEstudio) {}

bool Retoque::isFeitaNoEstudio() const {
    return feitaNoEstudio_;
}

double Retoque::calcularPreco() const {
    double multiplicadorDesconto = 1 - PORCENTAGEM_DESCONTO/100;
    double precoSemDesconto = TAXA_FIXA_SESSAO + TAXA_MAO_DE_OBRA;
    if (isFeitaNoEstudio()) 
        return multiplicadorDesconto * precoSemDesconto;
    return precoSemDesconto;
}