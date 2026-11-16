#include "Retoque.hpp"
#include "UtilsCsv.hpp"

constexpr double TAXA_FIXA_SESSAO = 30;
constexpr double TAXA_MAO_DE_OBRA = 40;
constexpr double PORCENTAGEM_DESCONTO = 30;

Retoque::Retoque(int duracaoMinutos, bool feitaNoEstudio) : Servico(duracaoMinutos, Especialidade::Tatuagem, TAXA_MAO_DE_OBRA, TAXA_FIXA_SESSAO), feitaNoEstudio_(feitaNoEstudio) {}

bool Retoque::isFeitaNoEstudio() const {
    return feitaNoEstudio_;
}

double Retoque::calcularPreco() const {
    double multiplicadorDesconto = 1 - PORCENTAGEM_DESCONTO/100;
    double precoSemDesconto = getTaxaFixaSessao() + getTaxaMaoDeObra();
    if (feitaNoEstudio_) 
        return multiplicadorDesconto * precoSemDesconto;
    return precoSemDesconto;
}

std::string Retoque::detalhes() const {
    return "Retoque (feita " + std::string(feitaNoEstudio_ ? "no" : "fora do") + " estúdio, " + Servico::detalhes() + ")";
}

std::string Retoque::gerarCSV() const {
    // Decisão 18: Bools gravados como sim/nao
    std::string feitaStr = feitaNoEstudio_ ? "sim" : "nao";
    return std::string("Retoque") + UtilsCsv::SEPARADOR
         + Servico::gerarCSV() + UtilsCsv::SEPARADOR
         + feitaStr;
}