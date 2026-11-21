#include "Piercing.hpp"
#include "UtilsCsv.hpp"
#include "DadoInvalidoException.hpp"

constexpr double TAXA_FIXA_SESSAO = 30;
constexpr double TAXA_MAO_DE_OBRA = 30;

Piercing::Piercing(int duracaoMinutos, const std::string& tipoJoia, double precoJoia)
    : Servico(duracaoMinutos, Especialidade::Piercing, TAXA_MAO_DE_OBRA, TAXA_FIXA_SESSAO),
      tipoJoia_(tipoJoia), precoJoia_(precoJoia) {
    // Validação estrita do preço que barra inclusive o "Not a Number" (NaN)
    if (!(precoJoia_ >= 0.0)) {
        throw DadoInvalidoException("O preço da joia não pode ser negativo ou inválido.");
    }
    // texto gravado no CSV, ver entrada 19 de docs/decisoes.md
    UtilsCsv::validarTextoLivre(tipoJoia_, "Tipo de Joia");
}

double Piercing::calcularPreco() const {
    return precoJoia_ + getTaxaFixaSessao() + getTaxaMaoDeObra();
}

std::string Piercing::detalhes() const {
    return "Piercing (" + tipoJoia_ + ", " + Servico::detalhes() + ")";
}

std::string Piercing::gerarCSV() const {
    return std::string("Piercing") + UtilsCsv::SEPARADOR
         + Servico::gerarCSV() + UtilsCsv::SEPARADOR
         + tipoJoia_ + UtilsCsv::SEPARADOR
         + std::to_string(precoJoia_);
}