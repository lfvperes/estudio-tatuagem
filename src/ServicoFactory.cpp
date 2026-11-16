#include "ServicoFactory.hpp"
#include "Tatuagem.hpp"
#include "Piercing.hpp"
#include "Retoque.hpp"
#include "DadoInvalidoException.hpp"

namespace ServicoFactory {

    // lê um int do CSV e recusa texto depois do número ("60min")
    static int extrairInt(const std::string& texto, const std::string& nomeCampo) {
        try {
            std::size_t pos;
            int valor = std::stoi(texto, &pos);
            if (pos != texto.size()) {
                throw DadoInvalidoException("Lixo encontrado após o número no campo " + nomeCampo + ".");
            }
            return valor;
        } catch (const std::invalid_argument&) {
            throw DadoInvalidoException("O campo " + nomeCampo + " não é um número válido.");
        } catch (const std::out_of_range&) {
            throw DadoInvalidoException("O número no campo " + nomeCampo + " excedeu o limite de tamanho.");
        }
    }

    // lê um double do CSV e recusa texto depois do número ("4.5x")
    static double extrairDouble(const std::string& texto, const std::string& nomeCampo) {
        try {
            std::size_t pos;
            double valor = std::stod(texto, &pos);
            if (pos != texto.size()) {
                throw DadoInvalidoException("Lixo encontrado após o decimal no campo " + nomeCampo + ".");
            }
            return valor;
        } catch (const std::invalid_argument&) {
            throw DadoInvalidoException("O campo " + nomeCampo + " não é um decimal válido.");
        } catch (const std::out_of_range&) {
            throw DadoInvalidoException("O decimal no campo " + nomeCampo + " excedeu o limite de tamanho.");
        }
    }

    std::unique_ptr<Servico> criarDoCSV(const std::string& tag, const std::vector<std::string>& campos) {
        // erros de dado viram DadoInvalidoException; quem lê o arquivo
        // acrescenta arquivo e linha (entrada 22 de docs/decisoes.md)

        if (tag == "Tatuagem") {
            if (campos.size() != 4) { // quantidade exata de colunas da tag
                throw DadoInvalidoException("Colunas insuficientes/excessivas para a tag Tatuagem (esperado 4).");
            }

            int duracao = extrairInt(campos.at(0), "Duração");

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

            int duracao = extrairInt(campos.at(0), "Duração");
            double preco = extrairDouble(campos.at(2), "Preço da Joia");

            return std::make_unique<Piercing>(duracao, campos.at(1), preco);

        } else if (tag == "Retoque") {
            if (campos.size() != 2) { // quantidade exata de colunas da tag
                throw DadoInvalidoException("Colunas insuficientes/excessivas para a tag Retoque (esperado 2).");
            }

            int duracao = extrairInt(campos.at(0), "Duração");

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