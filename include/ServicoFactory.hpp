#ifndef SERVICO_FACTORY_HPP
#define SERVICO_FACTORY_HPP

#include <string>
#include <vector>
#include <memory>
#include "Servico.hpp"

namespace ServicoFactory {
    // Recebe a tag (ex: "Tatuagem") e os campos subsequentes lidos do CSV
    std::unique_ptr<Servico> criarDoCSV(const std::string& tag, const std::vector<std::string>& campos);
}

#endif // SERVICO_FACTORY_HPP