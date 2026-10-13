#include "Pessoa.hpp"
#include "DadoInvalidoException.hpp"

Pessoa::Pessoa(const std::string& nome, const std::string& cpf) 
    : nome_(nome), cpf_(cpf) {
    
    if (nome_.empty()) {
        throw DadoInvalidoException("O nome não pode ser vazio.");
    }
    
    // validação básica de tamanho de CPF (11 dígitos numéricos ou 14 com pontos e traço)
    // Pode ajustar o tamanho conforme o padrão que adotar no terminal
    if (cpf_.empty() || cpf_.length() < 11) {
        throw DadoInvalidoException("CPF inválido. Insira um CPF válido.");
    }
}

Pessoa::~Pessoa() {}

std::string Pessoa::getNome() const {
    return nome_;
}

std::string Pessoa::getCpf() const {
    return cpf_;
}