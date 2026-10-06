#include "Tatuagem.hpp"

constexpr double TAXA_FIXA_SESSAO = 30;
constexpr double TAXA_MAO_DE_OBRA = 50;

Tatuagem::Tatuagem(int duracaoMinutos, Tamanho tamanho, Complexidade complexidade, std::string descricao) : Servico(duracaoMinutos, Especialidade::Tatuagem), tamanho_(tamanho), complexidade_(complexidade), descricao_(descricao) {}

std::string Tatuagem::getDescricao() const {
    return descricao_;
}

static double fatorTamanho(Tamanho tamanho) {
    switch (tamanho) {
    case Tamanho::Pequena:
        return 1.0;
    case Tamanho::Media:
        return 2.0;
    case Tamanho::Grande:
        return 4.0;
    }
    // TODO subir uma exception que ainda nao existe, como DadoInvalidoException
    return -1.0;
}

static double fatorComplexidade(Complexidade complexidade) {
    switch (complexidade) {
    case Complexidade::Baixa:
        return 1.0;
    case Complexidade::Media:
        return 1.5;
    case Complexidade::Alta:
        return 2.0;
    }
    // TODO subir uma exception que ainda nao existe, como DadoInvalidoException
    return -1.0;
}

double Tatuagem::calcularPreco() const {
    return TAXA_FIXA_SESSAO + TAXA_MAO_DE_OBRA * fatorTamanho(tamanho_) * fatorComplexidade(complexidade_);
}