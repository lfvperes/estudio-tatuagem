#ifndef ESPECIALIDADE_INCOMPATIVEL_EXCEPTION_HPP
#define ESPECIALIDADE_INCOMPATIVEL_EXCEPTION_HPP

#include "EstudioException.hpp"

class EspecialidadeIncompativelException : public EstudioException {
    public:
        EspecialidadeIncompativelException(const std::string& mensagem);
};

#endif // ESPECIALIDADE_INCOMPATIVEL_EXCEPTION_HPP