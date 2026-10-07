#ifndef CANCELAMENTO_FORA_DO_PRAZO_EXCEPTION_HPP
#define CANCELAMENTO_FORA_DO_PRAZO_EXCEPTION_HPP

#include "EstudioException.hpp"

class CancelamentoForaDoPrazoException : public EstudioException {
    public:
        CancelamentoForaDoPrazoException(const std::string& mensagem);
};

#endif // CANCELAMENTO_FORA_DO_PRAZO_EXCEPTION_HPP