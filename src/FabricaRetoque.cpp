#include "FabricaRetoque.hpp"
#include "Retoque.hpp"
#include "UtilsCsv.hpp"
#include "DadoInvalidoException.hpp"

std::vector<Pergunta> FabricaRetoque::perguntas() const {
    return {
        {"Duração (minutos)", {}},
        {"Tatuagem original foi feita neste estúdio?", {"sim", "nao"}}
    };
}

std::unique_ptr<Servico> FabricaRetoque::criar(const std::vector<std::string>& campos) const {
    if (campos.size() != 2) {
        throw DadoInvalidoException("Número incorreto de campos para Retoque.");
    }

    int duracao = UtilsCsv::extrair<int>(campos[0], "Duração");

    bool feitaNoEstudio;
    if (campos[1] == "sim") feitaNoEstudio = true;
    else if (campos[1] == "nao") feitaNoEstudio = false;
    else throw DadoInvalidoException("Valor inválido para 'feita no estúdio': " + campos[1]);

    return std::make_unique<Retoque>(duracao, feitaNoEstudio);
}