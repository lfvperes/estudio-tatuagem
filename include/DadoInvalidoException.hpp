#ifndef DADO_INVALIDO_EXCEPTION_HPP
#define DADO_INVALIDO_EXCEPTION_HPP

#include "EstudioException.hpp"

class DadoInvalidoException : public EstudioException {
    public:
        DadoInvalidoException(const std::string& mensagem);
};

#endif // DADO_INVALIDO_EXCEPTION_HPP