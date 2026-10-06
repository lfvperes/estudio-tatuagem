#ifndef PIERCING_HPP
#define PIERCING_HPP

#include "Servico.hpp"
#include <string>

class Piercing : public Servico {
    private:
        std::string tipoJoia_;
        double precoJoia_;
    public:
        Piercing(int duracaoMinutos, std::string tipoJoia, double precoJoia);
        double calcularPreco() const override;
};


#endif // PIERCING_HPP