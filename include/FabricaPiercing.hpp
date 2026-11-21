#ifndef FABRICA_PIERCING_HPP
#define FABRICA_PIERCING_HPP

#include "FabricaServico.hpp"

class FabricaPiercing : public FabricaServico {
    public:
        std::vector<Pergunta> perguntas() const override;
        std::unique_ptr<Servico> criar(const std::vector<std::string>& campos) const override;
};

#endif // FABRICA_PIERCING_HPP