#include "Pessoa.hpp"
#include "DadoInvalidoException.hpp"
#include "UtilsCsv.hpp"

Pessoa::Pessoa(const std::string& nome, const std::string& cpf)
    : nome_(nome), cpf_(cpf) {
    // texto gravado no CSV, ver entrada 19 de docs/decisoes.md
    UtilsCsv::validarTextoLivre(nome_, "Nome da Pessoa");

    if (nome_.empty()) {
        throw DadoInvalidoException("O nome não pode ser vazio.");
    }
    
    if (cpf_.length() != 11) {
        throw DadoInvalidoException("CPF inválido. O CPF deve conter exatamente 11 dígitos.");
    }
    // só dígitos: evita o mesmo CPF em formatos diferentes e o separador do CSV (entrada 19)
    if (cpf_.find_first_not_of("0123456789") != std::string::npos) {
        throw DadoInvalidoException("CPF inválido. O CPF deve conter apenas números.");
    }
}

Pessoa::~Pessoa() {}

std::string Pessoa::getNome() const {
    return nome_;
}

std::string Pessoa::getCpf() const {
    return cpf_;
}