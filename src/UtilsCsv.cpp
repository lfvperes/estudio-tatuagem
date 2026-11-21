#include "UtilsCsv.hpp"
#include "DadoInvalidoException.hpp"

namespace UtilsCsv {

    void validarTextoLivre(const std::string& texto, const std::string& nomeDoCampo) {
        // Procurar o separador ';' ou a quebra de linha normal '\n'
        std::string caracteresInvalidos = std::string(1, SEPARADOR) + "\n";
        
        if (texto.find_first_of(caracteresInvalidos) != std::string::npos) {
            throw DadoInvalidoException("O campo " + nomeDoCampo + " não pode conter o caractere '" 
                                        + SEPARADOR + "' ou quebra de linha.");
        }
    }

    void limparRetornoDeCarro(std::string& linha) {
        if (!linha.empty() && linha.back() == '\r') {
            linha.pop_back();
        }
    }

    // lê um int do CSV e recusa texto depois do número ("60min")
    int extrairInt(const std::string& texto, const std::string& nomeCampo) {
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
    double extrairDouble(const std::string& texto, const std::string& nomeCampo) {
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
}