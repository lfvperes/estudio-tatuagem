#ifndef FABRICA_RETOQUE_HPP
#define FABRICA_RETOQUE_HPP

#include "FabricaServico.hpp"

class FabricaRetoque : public FabricaServico {
    public:
        std::vector<Pergunta> perguntas() const override;
        std::unique_ptr<Servico> criar(const std::vector<std::string>& campos) const override;
};

#endif // FABRICA_RETOQUE_HPP