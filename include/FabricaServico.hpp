#ifndef FABRICA_SERVICO_HPP
#define FABRICA_SERVICO_HPP

#include <memory>
#include <string>
#include <vector>
#include "Servico.hpp"

struct Pergunta {
    std::string texto;
    std::vector<std::string> opcoes; // vazia para campo livre (ex.: duração, descrição)
};

// Factory Method: cada fábrica concreta cria um tipo de serviço e informa
// as perguntas que o menu deve fazer (entrada 30 de docs/decisoes.md)
class FabricaServico {
    public:
        virtual ~FabricaServico() = default;

        // A ordem das perguntas deve ser exatamente a mesma das colunas no CSV
        // e dos elementos esperados em criar(campos).
        virtual std::vector<Pergunta> perguntas() const = 0;
        virtual std::unique_ptr<Servico> criar(const std::vector<std::string>& campos) const = 0;
};

#endif // FABRICA_SERVICO_HPP
