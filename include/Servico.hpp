#ifndef SERVICO_HPP
#define SERVICO_HPP

#include "Especialidade.hpp"
#include <string>

class Servico {
    private:
        int duracaoMinutos_;
        Especialidade especialidade_;
        double taxaMaoDeObra_;
        double taxaFixaSessao_;
    public:
    // Gera a string formatada com os dados do serviço para o CSV (Decisão 20)
        virtual std::string gerarCSV() const = 0;
        Servico(int duracaoMinutos, Especialidade especialidade, double taxaMaoDeObra, double taxaFixaSessao);
        virtual ~Servico();
        int getDuracaoMinutos() const;
        Especialidade getEspecialidade() const;
        virtual double calcularPreco() const = 0;
        double getTaxaMaoDeObra() const;
        double getTaxaFixaSessao() const;
        virtual std::string detalhes() const;
};

#endif // SERVICO_HPP