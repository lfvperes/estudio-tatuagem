#ifndef ESTUDIO_EXCEPTION_HPP
#define ESTUDIO_EXCEPTION_HPP

#include <stdexcept>
#include <string>

class EstudioException : public std::runtime_error {
    public:
        EstudioException(const std::string& mensagem);
};


#endif // ESTUDIO_EXCEPTION_HPP