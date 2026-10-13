#ifndef FUNCIONARIO_HPP
#define FUNCIONARIO_HPP

#include "Pessoa.hpp"
#include "Especialidade.hpp"

class Funcionario : public Pessoa {
    private:
        Especialidade especialidade_;
        double comissao_;
    public:
        Funcionario(const std::string& nome, const std::string& cpf, Especialidade especialidade, double comissao);
        
        Especialidade getEspecialidade() const;
        double getComissao() const;
        
        void exibirInfo() const override;
};

#endif // FUNCIONARIO_HPP