#ifndef RETOQUE_HPP
#define RETOQUE_HPP

#include "Servico.hpp"
#include <string>

class Retoque : public Servico {
    private:
        // tatuagem feita originalmente no mesmo estudio ganha desconto
        bool feitaNoEstudio_;
    public:
        Retoque(int duracaoMinutos, bool feitaNoEstudio);
        double calcularPreco() const override;
        bool isFeitaNoEstudio() const;
        std::string detalhes() const override;
};

#endif // RETOQUE_HPP