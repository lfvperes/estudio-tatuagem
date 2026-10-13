#ifndef HORARIO_INDISPONIVEL_EXCEPTION_HPP
#define HORARIO_INDISPONIVEL_EXCEPTION_HPP

#include "EstudioException.hpp"

class HorarioIndisponivelException : public EstudioException {
    public:
        HorarioIndisponivelException(const std::string& mensagem);
};


#endif // HORARIO_INDISPONIVEL_EXCEPTION_HPP