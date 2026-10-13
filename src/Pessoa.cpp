#include "Pessoa.hpp"

// Passagem de string por const reference, mantendo o padrão adotado no refactor do Luís
Pessoa::Pessoa(const std::string& nome, const std::string& cpf) 
    : nome_(nome), cpf_(cpf) {}

Pessoa::~Pessoa() {}

std::string Pessoa::getNome() const {
    return nome_;
}

std::string Pessoa::getCpf() const {
    return cpf_;
}