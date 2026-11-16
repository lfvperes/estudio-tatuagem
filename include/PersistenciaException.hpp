#ifndef PERSISTENCIA_EXCEPTION_HPP
#define PERSISTENCIA_EXCEPTION_HPP

#include "EstudioException.hpp"
#include <string>

class PersistenciaException : public EstudioException {
    public:
        // Construtor para erros de formatação/parsing em uma linha específica do CSV
        PersistenciaException(const std::string& arquivo, int linha, const std::string& detalhes);

        // Segundo construtor para erros de I/O (ex: falha ao abrir/criar arquivo) sem linha
        PersistenciaException(const std::string& arquivo, const std::string& detalhes);
};

#endif // PERSISTENCIA_EXCEPTION_HPP