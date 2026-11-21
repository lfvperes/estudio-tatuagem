#include "FabricaPiercing.hpp"
#include "Piercing.hpp"
#include "UtilsCsv.hpp"
#include "DadoInvalidoException.hpp"

std::vector<Pergunta> FabricaPiercing::perguntas() const {
    return {
        {"Duração (minutos)", {}},
        {"Tipo da Joia", {}},
        {"Preço da Joia", {}}
    };
}

std::unique_ptr<Servico> FabricaPiercing::criar(const std::vector<std::string>& campos) const {
    if (campos.size() != 3) {
        throw DadoInvalidoException("Número incorreto de campos para Piercing.");
    }

    int duracao = UtilsCsv::extrair<int>(campos[0], "Duração");
    double precoJoia = UtilsCsv::extrair<double>(campos[2], "Preço da Joia");

    return std::make_unique<Piercing>(duracao, campos[1], precoJoia);
}