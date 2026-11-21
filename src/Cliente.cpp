#include "Cliente.hpp"
#include "UtilsCsv.hpp"
#include <iostream>

Cliente::Cliente(const std::string& nome, const std::string& cpf, const std::string& alergiasObservacoes)
    : Pessoa(nome, cpf), alergiasObservacoes_(alergiasObservacoes) {
    // texto gravado no CSV, ver entrada 19 de docs/decisoes.md
    UtilsCsv::validarTextoLivre(alergiasObservacoes_, "Alergias/Observações");
}

std::string Cliente::getAlergiasObservacoes() const {
    return alergiasObservacoes_;
}

void Cliente::exibirInfo() const {
    std::cout << "--- Cliente ---" << std::endl;
    std::cout << "Nome: " << getNome() << std::endl;
    std::cout << "CPF: " << getCpf() << std::endl;
    if (!alergiasObservacoes_.empty()) {
        std::cout << "Alergias/Observações: " << alergiasObservacoes_ << std::endl;
    } else {
        std::cout << "Alergias/Observações: Nenhuma" << std::endl;
    }
}