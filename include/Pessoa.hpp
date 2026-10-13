#ifndef PESSOA_HPP
#define PESSOA_HPP

#include <string>

class Pessoa {
    private:
        std::string nome_;
        std::string cpf_;
    public:
        Pessoa(const std::string& nome, const std::string& cpf);
        virtual ~Pessoa();
        
        std::string getNome() const;
        std::string getCpf() const;
        
        virtual void exibirInfo() const = 0;
};

#endif // PESSOA_HPP