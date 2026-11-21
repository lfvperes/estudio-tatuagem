#ifndef FABRICA_TATUAGEM_HPP
#define FABRICA_TATUAGEM_HPP

#include "FabricaServico.hpp"

class FabricaTatuagem : public FabricaServico {
    public:
        std::vector<Pergunta> perguntas() const override;
        std::unique_ptr<Servico> criar(const std::vector<std::string>& campos) const override;
};

#endif // FABRICA_TATUAGEM_HPP