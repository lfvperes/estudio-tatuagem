#ifndef SERVICO_HPP
#define SERVICO_HPP

#include "Especialidade.hpp"

class Servico {
    private:
        int duracaoMinutos_;
        Especialidade especialidade_;
    public:
        Servico(int duracaoMinutos, Especialidade especialidade);
        virtual ~Servico();
        int getDuracaoMinutos() const;
        Especialidade getEspecialidade() const;
        virtual double calcularPreco() const = 0;
};

#endif // SERVICO_HPP