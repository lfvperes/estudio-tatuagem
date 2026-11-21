#include "FabricaTatuagem.hpp"
#include "Tatuagem.hpp"
#include "UtilsCsv.hpp"
#include "DadoInvalidoException.hpp"

std::vector<Pergunta> FabricaTatuagem::perguntas() const {
    return {
        {"Duração (minutos)", {}},
        {"Tamanho", {"Pequena", "Media", "Grande"}},
        {"Complexidade", {"Baixa", "Media", "Alta"}},
        {"Descrição", {}}
    };
}

std::unique_ptr<Servico> FabricaTatuagem::criar(const std::vector<std::string>& campos) const {
    if (campos.size() != 4) {
        throw DadoInvalidoException("Número incorreto de campos para Tatuagem.");
    }

    int duracao = UtilsCsv::extrair<int>(campos[0], "Duração");

    Tamanho tamanho;
    if (campos[1] == "Pequena") tamanho = Tamanho::Pequena;
    else if (campos[1] == "Media") tamanho = Tamanho::Media;
    else if (campos[1] == "Grande") tamanho = Tamanho::Grande;
    else throw DadoInvalidoException("Tamanho inválido para Tatuagem: " + campos[1]);

    Complexidade complexidade;
    if (campos[2] == "Baixa") complexidade = Complexidade::Baixa;
    else if (campos[2] == "Media") complexidade = Complexidade::Media;
    else if (campos[2] == "Alta") complexidade = Complexidade::Alta;
    else throw DadoInvalidoException("Complexidade inválida para Tatuagem: " + campos[2]);

    return std::make_unique<Tatuagem>(duracao, tamanho, complexidade, campos[3]);
}