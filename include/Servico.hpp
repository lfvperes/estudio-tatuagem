#ifndef SERVICO_HPP
#define SERVICO_HPP

#include "Especialidade.hpp"

class Servico {
    private:
        int duracaoMinutos_;
        Especialidade especialidade_;
        double taxaMaoDeObra_;
        double taxaFixaSessao_;
    public:
        Servico(int duracaoMinutos, Especialidade especialidade, double taxaMaoDeObra, double taxaFixaSessao);
        virtual ~Servico();
        int getDuracaoMinutos() const;
        Especialidade getEspecialidade() const;
        virtual double calcularPreco() const = 0;
        double getTaxaMaoDeObra() const;
        double getTaxaFixaSessao() const;
};

#endif // SERVICO_HPP