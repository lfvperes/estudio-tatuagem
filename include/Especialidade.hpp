#ifndef ESPECIALIDADE_HPP
#define ESPECIALIDADE_HPP

#include <string>

enum class Especialidade {Tatuagem, Piercing};

// Centralizado para reuso no Funcionario e no Estudio
inline std::string especialidadeParaString(Especialidade especialidade) {
    switch (especialidade) {
        case Especialidade::Tatuagem: return "Tatuagem";
        case Especialidade::Piercing: return "Piercing";
    }
    return "?";
}

#endif // ESPECIALIDADE_HPP