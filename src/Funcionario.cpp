#include "Funcionario.hpp"
#include "DadoInvalidoException.hpp"
#include <iostream>

Funcionario::Funcionario(const std::string& nome, const std::string& cpf, Especialidade especialidade, double comissao)
    : Pessoa(nome, cpf), especialidade_(especialidade), comissao_(comissao) {
    
    if (comissao_ < 0.0 || comissao_ > 100.0) {
        throw DadoInvalidoException("A comissão deve ser um valor entre 0 e 100.");
    }
}

Especialidade Funcionario::getEspecialidade() const {
    return especialidade_;
}

double Funcionario::getComissao() const {
    return comissao_;
}

static std::string especialidadeParaString(Especialidade especialidade) {
    switch (especialidade) {
        case Especialidade::Tatuagem:
            return "Tatuagem";
        case Especialidade::Piercing:
            return "Piercing";
    }
    return "?";
}

void Funcionario::exibirInfo() const {
    std::cout << "--- Funcionário ---" << std::endl;
    std::cout << "Nome: " << getNome() << std::endl;
    std::cout << "CPF: " << getCpf() << std::endl;
    std::cout << "Especialidade: " << especialidadeParaString(especialidade_) << std::endl;
    std::cout << "Comissão: " << comissao_ << "%" << std::endl;
}