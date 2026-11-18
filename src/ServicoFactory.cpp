#include "ServicoFactory.hpp"
#include "Tatuagem.hpp"
#include "Piercing.hpp"
#include "Retoque.hpp"
#include "DadoInvalidoException.hpp"
#include "UtilsCsv.hpp"

namespace ServicoFactory {

    std::unique_ptr<Servico> criarDoCSV(const std::string& tag, const std::vector<std::string>& campos) {
        // erros de dado viram DadoInvalidoException; quem lê o arquivo
        // acrescenta arquivo e linha (entrada 22 de docs/decisoes.md)

        if (tag == "Tatuagem") {
            if (campos.size() != 4) { // quantidade exata de colunas da tag
                throw DadoInvalidoException("Colunas insuficientes/excessivas para a tag Tatuagem (esperado 4).");
            }

            int duracao = UtilsCsv::extrair<int>(campos.at(0), "Duração");

            // texto fora da lista é recusado, não vira o último valor
            Tamanho tam;
            if (campos.at(1) == "Pequena") tam = Tamanho::Pequena;
            else if (campos.at(1) == "Media") tam = Tamanho::Media;
            else if (campos.at(1) == "Grande") tam = Tamanho::Grande;
            else throw DadoInvalidoException("Tamanho inválido para tatuagem: " + campos.at(1));

            // texto fora da lista é recusado, não vira o último valor
            Complexidade comp;
            if (campos.at(2) == "Baixa") comp = Complexidade::Baixa;
            else if (campos.at(2) == "Media") comp = Complexidade::Media;
            else if (campos.at(2) == "Alta") comp = Complexidade::Alta;
            else throw DadoInvalidoException("Complexidade inválida para tatuagem: " + campos.at(2));

            return std::make_unique<Tatuagem>(duracao, tam, comp, campos.at(3));

        } else if (tag == "Piercing") {
            if (campos.size() != 3) { // quantidade exata de colunas da tag
                throw DadoInvalidoException("Colunas insuficientes/excessivas para a tag Piercing (esperado 3).");
            }

            int duracao = UtilsCsv::extrair<int>(campos.at(0), "Duração");
            double preco = UtilsCsv::extrair<double>(campos.at(2), "Preço da Joia");

            return std::make_unique<Piercing>(duracao, campos.at(1), preco);

        } else if (tag == "Retoque") {
            if (campos.size() != 2) { // quantidade exata de colunas da tag
                throw DadoInvalidoException("Colunas insuficientes/excessivas para a tag Retoque (esperado 2).");
            }

            int duracao = UtilsCsv::extrair<int>(campos.at(0), "Duração");

            // só "sim" ou "nao" (entrada 18 de docs/decisoes.md)
            bool feitaAqui;
            if (campos.at(1) == "sim") feitaAqui = true;
            else if (campos.at(1) == "nao") feitaAqui = false;
            else throw DadoInvalidoException("Campo booleano de Retoque inválido (esperado sim/nao): " + campos.at(1));

            return std::make_unique<Retoque>(duracao, feitaAqui);
        }

        throw DadoInvalidoException("Tag de serviço polimórfico desconhecida: " + tag);
    }
}