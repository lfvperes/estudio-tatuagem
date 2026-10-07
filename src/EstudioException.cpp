#include "EstudioException.hpp"

EstudioException::EstudioException(const std::string& mensagem) : std::runtime_error(mensagem) {}