#ifndef CLIENTE_MENOR_DE_IDADE_EXCEPTION_HPP
#define CLIENTE_MENOR_DE_IDADE_EXCEPTION_HPP

#include "EstudioException.hpp"

class ClienteMenorDeIdadeException : public EstudioException {
    public:
        ClienteMenorDeIdadeException();
};

#endif // CLIENTE_MENOR_DE_IDADE_EXCEPTION_HPP