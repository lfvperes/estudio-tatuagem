#ifndef PERSISTENCIA_EXCEPTION_HPP
#define PERSISTENCIA_EXCEPTION_HPP

#include "EstudioException.hpp"
#include <string>

class PersistenciaException : public EstudioException {
    public:
        PersistenciaException(const std::string& arquivo, int linha, const std::string& detalhes);
};

#endif // PERSISTENCIA_EXCEPTION_HPP