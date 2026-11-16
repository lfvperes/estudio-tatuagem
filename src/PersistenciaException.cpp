#include "PersistenciaException.hpp"

// Implementação do erro com linha
PersistenciaException::PersistenciaException(const std::string& arquivo, int linha, const std::string& detalhes)
    : EstudioException("Erro de persistência no arquivo '" + arquivo + "' (linha " + std::to_string(linha) + "): " + detalhes) {}

// Implementação do erro geral (sem linha)
PersistenciaException::PersistenciaException(const std::string& arquivo, const std::string& detalhes)
    : EstudioException("Erro de persistência no arquivo '" + arquivo + "': " + detalhes) {}