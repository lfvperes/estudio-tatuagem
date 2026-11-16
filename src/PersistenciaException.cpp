#include "PersistenciaException.hpp"

PersistenciaException::PersistenciaException(const std::string& arquivo, int linha, const std::string& detalhes)
    : EstudioException("Erro de persistência no arquivo '" + arquivo + "' (linha " + std::to_string(linha) + "): " + detalhes) {}