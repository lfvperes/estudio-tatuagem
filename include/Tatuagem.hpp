#ifndef TATUAGEM_HPP
#define TATUAGEM_HPP

#include "Servico.hpp"
#include <string>

enum class Tamanho {Pequena, Media, Grande};
enum class Complexidade {Baixa, Media, Alta};

class Tatuagem : public Servico {
    private:
        Tamanho tamanho_;
        Complexidade complexidade_;
        std::string descricao_;
    public:
        Tatuagem(int duracaoMinutos, Tamanho tamanho, Complexidade complexidade, std::string descricao);
        double calcularPreco() const override;
        std::string getDescricao() const;
};


#endif // TATUAGEM_HPP
