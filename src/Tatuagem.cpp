#include "Tatuagem.hpp"
#include "DadoInvalidoException.hpp"

constexpr double TAXA_FIXA_SESSAO = 30;
constexpr double TAXA_MAO_DE_OBRA = 50;

Tatuagem::Tatuagem(int duracaoMinutos, Tamanho tamanho, Complexidade complexidade, const std::string& descricao) : Servico(duracaoMinutos, Especialidade::Tatuagem, TAXA_MAO_DE_OBRA, TAXA_FIXA_SESSAO), tamanho_(tamanho), complexidade_(complexidade), descricao_(descricao) {}

std::string Tatuagem::getDescricao() const {
    return descricao_;
}

// valores definidos arbitrariamente, ver docs/decisoes.md
static double fatorTamanho(Tamanho tamanho) {
    switch (tamanho) {
    case Tamanho::Pequena:
        return 1.0;
    case Tamanho::Media:
        return 2.0;
    case Tamanho::Grande:
        return 4.0;
    // sem default de propósito: o -Wswitch avisa se um enumerador novo ficar sem fator
    }
    throw DadoInvalidoException("Tamanho \"" + std::to_string(static_cast<int>(tamanho)) + "\" não existe.");
}

// valores definidos arbitrariamente, ver docs/decisoes.md
static double fatorComplexidade(Complexidade complexidade) {
    switch (complexidade) {
    case Complexidade::Baixa:
        return 1.0;
    case Complexidade::Media:
        return 1.5;
    case Complexidade::Alta:
        return 2.0;
    // sem default de propósito: o -Wswitch avisa se um enumerador novo ficar sem fator
    }
    throw DadoInvalidoException("Complexidade \"" + std::to_string(static_cast<int>(complexidade)) + "\" não existe.");
}

double Tatuagem::calcularPreco() const {
    return getTaxaFixaSessao() + getTaxaMaoDeObra() * fatorTamanho(tamanho_) * fatorComplexidade(complexidade_);
}

static std::string tamanhoParaString(Tamanho tamanho) {
    switch (tamanho) {
    case Tamanho::Pequena:
        return "pequena";
    case Tamanho::Media:
        return "média";
    case Tamanho::Grande:
        return "grande";
    }
    throw DadoInvalidoException("Tamanho \"" + std::to_string(static_cast<int>(tamanho)) + "\" não existe.");
}

static std::string complexidadeParaString(Complexidade complexidade) {
    switch (complexidade) {
    case Complexidade::Baixa:
        return "baixa";
    case Complexidade::Media:
        return "média";
    case Complexidade::Alta:
        return "alta";
    }
    throw DadoInvalidoException("Complexidade \"" + std::to_string(static_cast<int>(complexidade)) + "\" não existe.");
}

std::string Tatuagem::detalhes() const {
    return "Tatuagem (" + tamanhoParaString(tamanho_) + ", complexidade " + complexidadeParaString(complexidade_) + ", " + Servico::detalhes() + "): " + descricao_;
}