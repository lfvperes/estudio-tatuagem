#ifndef SERVICO_FACTORY_HPP
#define SERVICO_FACTORY_HPP

#include <string>
#include <vector>
#include <memory>
#include "Servico.hpp"
#include "FabricaServico.hpp"

// Escolhe a fábrica concreta pelo número do menu ou pela tag do CSV
// (entrada 30 de docs/decisoes.md)
class ServicoFactory {
    public:
        // só tem métodos estáticos, não é feita para ser instanciada
        ServicoFactory() = delete;

        static std::unique_ptr<FabricaServico> criarFabrica(int tipo);
        static std::unique_ptr<FabricaServico> criarFabrica(const std::string& tag);

        static std::unique_ptr<Servico> criarDoCSV(const std::string& tag,
                                                   const std::vector<std::string>& campos);
};

#endif // SERVICO_FACTORY_HPP
