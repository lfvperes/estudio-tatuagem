#ifndef CLIENTE_HPP
#define CLIENTE_HPP

#include "Pessoa.hpp"
#include <string>

class Cliente : public Pessoa {
    private:
        std::string alergiasObservacoes_;
    public:
        Cliente(const std::string& nome, const std::string& cpf, const std::string& alergiasObservacoes = "");
        
        std::string getAlergiasObservacoes() const;
        void exibirInfo() const override;
};

#endif // CLIENTE_HPP