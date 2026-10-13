#include "Cliente.hpp"
#include <iostream>

Cliente::Cliente(const std::string& nome, const std::string& cpf, const std::string& alergiasObservacoes)
    : Pessoa(nome, cpf), alergiasObservacoes_(alergiasObservacoes) {}

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