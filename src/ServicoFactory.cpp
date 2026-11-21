#include "ServicoFactory.hpp"
#include "FabricaTatuagem.hpp"
#include "FabricaPiercing.hpp"
#include "FabricaRetoque.hpp"
#include "DadoInvalidoException.hpp"

std::unique_ptr<FabricaServico> ServicoFactory::criarFabrica(int tipo) {
    switch (tipo) {
        case 1:
            return std::make_unique<FabricaTatuagem>();
        case 2:
            return std::make_unique<FabricaPiercing>();
        case 3:
            return std::make_unique<FabricaRetoque>();
        default:
            throw DadoInvalidoException("Tipo de serviço inválido.");
    }
}

std::unique_ptr<FabricaServico> ServicoFactory::criarFabrica(const std::string& tag) {
    if (tag == "Tatuagem") return criarFabrica(1);
    if (tag == "Piercing") return criarFabrica(2);
    if (tag == "Retoque")  return criarFabrica(3);
    throw DadoInvalidoException("Tipo de serviço desconhecido no CSV: " + tag);
}

std::unique_ptr<Servico> ServicoFactory::criarDoCSV(const std::string& tag,
                                                    const std::vector<std::string>& campos) {
    return criarFabrica(tag)->criar(campos);
}
