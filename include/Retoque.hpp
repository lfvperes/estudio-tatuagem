#ifndef RETOQUE_HPP
#define RETOQUE_HPP

#include "Servico.hpp"

class Retoque : public Servico {
    private:
        // tatuagem feita originalmente no mesmo estudio ganha desconto
        bool feitaNoEstudio_;
    public:
        Retoque(int duracaoMinutos, bool feitaNoEstudio);
        double calcularPreco() const override;
        bool isFeitaNoEstudio() const;
};

#endif // RETOQUE_HPP